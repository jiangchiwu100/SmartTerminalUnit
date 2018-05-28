/**
  * @file   gwprotocal.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/04/12
  * @brief  gwxieyi
  */

#include "gwprotocal.h"
#include "systemconfig.h"
#include <string.h>

/* 接收控制 */
static struct ReceiveControl revControl;
/* 用户表 */
static UserTab userTab;
/* 发送队列 */
static UserSendQueue sendQueue;
/* 发送buff */
static uint8_t sendFrameBuff[SEND_MAXBYTE];
/* 计数器 */
static uint8_t sendCounter;


/* 获取时间 */
static uint32_t (* GetIntervalTick)(uint32_t beginTime);
static uint32_t (* GetCurrentTick)(void);

/**
  *@brief  底层接收函数
  *@param  None
  *@retval None
  */
void ReceiveDealWith(uint8_t data)
{
	if(revControl.flag == CANRECEIVE){
		revControl.buff[revControl.pInCount++] = data;
	}
	else{
		data = data;
	}
}

/**
  *@brief  底层接收完成函数
  *@param  None
  *@retval None
  */
void ReceiveFinish(void)
{
	revControl.flag = REVISBUSYING;
}


/**
  *@brief  16位CRC校验
  *@param  Pushdata：要校验数据指针
  *@param  length：要校验数据长度
  *@retval 校验值 低位在前
  */
static uint16_t CalculateCheck(uint8_t *Pushdata, uint16_t length)
{
	uint16_t Reg_CRC=0xffff;  
	uint8_t Temp_reg=0x00;  
	uint16_t i,j;   
	for( i = 0; i<length; i ++){  
	 Reg_CRC^= *Pushdata++;  
	 for (j = 0; j<8; j++){       
	   if(Reg_CRC & 0x0001)   
			 Reg_CRC=Reg_CRC>>1^0xA001;  
		 else  
			Reg_CRC >>=1;  
	 }    
	}  
	Temp_reg=Reg_CRC>>8;  
	return (Reg_CRC<<8|Temp_reg);
}

/**
  *@brief  校验函数
  *@param  pBuff 校验数组指针
  *@param  num 校验数
  *@retval None
  */
static ReturnStatus FrameCheck(uint8_t *pBuff, uint16_t num)
{
	Uint8To16 dataCheck;
	dataCheck.Uint16 = CalculateCheck(pBuff, num - 3);
	if(dataCheck.Uint8[0] == pBuff[num - 3] &&\
		dataCheck.Uint8[1] == pBuff[num - 2]){
		return ISTURE;
	}
	return ISFAILURE;
}

/**
  *@brief  用户数据交接处理
  *@param  thisUser 用户指针
  *@retval None
  */
static void UserInfoDealWith(UserInfo *thisUser)
{
	Uint8To16 uint8To16;
	
	uint8To16.Uint8[0] = revControl.buff[DATALEN_L];
	uint8To16.Uint8[1] = revControl.buff[DATALEN_H];
	
	if((revControl.buff[USERID] & ISMASTERBIT) == ISMASTERBIT){//主机用户
		if(uint8To16.Uint16 > thisUser->revBuffMax){
			return;
		}
		if(thisUser->status != IDLE){
			return;
		}
		thisUser->number = revControl.buff[FRAME_NUMBER];
		memcpy(thisUser->pRevBuff,&revControl.buff[DATALEN_L],uint8To16.Uint16);
		thisUser->status = ISMASTERCALL;
	}
	else{//从机用户
		if(thisUser->status != WAITACK){
			return;
		}
		if(revControl.buff[FRAME_NUMBER] != thisUser->number){
			thisUser->status = ACKFALSE;
			return;
		}
		if(uint8To16.Uint16 > thisUser->revBuffMax){
			thisUser->status = ACKFALSE;
			return;
		}
		memcpy(thisUser->pRevBuff,&revControl.buff[DATALEN_L],uint8To16.Uint16);
		thisUser->status = ISACK;
	}
}

/**
  *@brief  帧处理完成
  *@param  None
  *@retval None
  */
static void FrameDealWithFinish(void)
{
	revControl.pInCount = 0;
	revControl.flag = CANRECEIVE;
}

/**
  *@brief  开始帧处理
  *@param  None
  *@retval None
  */
static void FrameDealWith(void)
{
	uint8_t i;
	if(revControl.pInCount < 6){
		FrameDealWithFinish();	
		return;
	}
	if(FrameCheck(revControl.buff,revControl.pInCount) != ISTURE){//校验错误
		FrameDealWithFinish();	
		return;
	}
	for(i = 0; i < userTab.num; i++){//查找用户
		if((revControl.buff[USERID] >> ISMASTERBIT) == (userTab.user[i]->ID >> ISMASTERBIT)){
			UserInfoDealWith(userTab.user[i]);
			break;
		}
	}
	FrameDealWithFinish();
}

/**
  *@brief  用户数据发送填充
  *@param  sendInfo 用户
  *@retval pSendBuff 数据
  */
ReturnStatus UserDataSendFill(UserInfo *sendInfo,uint8_t *pSendBuff)
{
	if(sendQueue.pIn + 1 == sendQueue.pOut){
		return ISFAILURE;
	}
	sendQueue.user[sendQueue.pIn].user = sendInfo;
	sendQueue.user[sendQueue.pIn].pSendBuff = pSendBuff;
	sendQueue.pIn ++;
	if(sendQueue.pIn >= USERSEND_MAXNUM){
		sendQueue.pIn = 0;
	}
	if((sendInfo->ID & ISMASTERBIT) == ISMASTERBIT){
		sendInfo->timConut = GetCurrentTick();
	}
	sendInfo->status = SENDING;
	return ISTURE;
}

void SendDataDealWith(UserSendInfo *Info)
{
	Uint8To16 dataCheck,dataLen;
	sendFrameBuff[FRAME_BEGIN] = 0x55;
	sendFrameBuff[USERID] = Info->user->ID;
	if((Info->user->ID & ISMASTERBIT) == ISMASTERBIT){
		sendFrameBuff[FRAME_NUMBER] = sendCounter;
		Info->user->number = sendCounter;
	}
	else{
		sendFrameBuff[FRAME_NUMBER] = Info->user->number;
	}
	dataLen.Uint8[0] = Info->pSendBuff[0];
	dataLen.Uint8[1] = Info->pSendBuff[1];
	if(dataLen.Uint16 + 6 >= SEND_MAXBYTE){
		Info->user->status = SENDFALSE;
		return;
	}
	memcpy((char *)&sendFrameBuff[3],(char *)Info->pSendBuff,dataLen.Uint16);
	dataCheck.Uint16 = CalculateCheck(sendFrameBuff,dataLen.Uint16 + 3);
	sendFrameBuff[dataLen.Uint16 + 3] = dataCheck.Uint8[0];
	sendFrameBuff[dataLen.Uint16 + 4] = dataCheck.Uint8[1];
	sendFrameBuff[dataLen.Uint16 + 5] = 0xAA;
	if(Info->user->status != SENDING){
		return;
	}
	SendData(sendFrameBuff, dataLen.Uint16 + 6);
	if((Info->user->ID & ISMASTERBIT) == ISMASTERBIT){
		Info->user->status = WAITACK;
	}
	else{
		Info->user->status = IDLE;
	}
}
/**
  *@brief  发送队列查询发送
  *@param  sendInfo 用户
  *@retval pSendBuff 数据
  */
static void sendQueueData(void)
{
	if(sendQueue.pIn != sendQueue.pOut){
		sendCounter ++;
		SendDataDealWith(&sendQueue.user[sendQueue.pOut]);
		if(++sendQueue.pOut >= USERSEND_MAXNUM){
			sendQueue.pOut = 0;
		}
	}
}

/**
  *@brief  从机状态查询
  *@param  None
  *@retval None
  */
void SlaveRevScan(void)
{
	uint8_t i;
	for(i = 0; i < userTab.num; i++){//查找用户
		if((userTab.user[i]->ID & ISMASTERBIT) != ISMASTERBIT){
			if(userTab.user[i]->status == ISMASTERCALL){
				userTab.user[i]->slaveFun();
			}
		}
	}
}

/**
  *@brief  主机状态查询
  *@param  None
  *@retval None
  */
void MasterRevScan(void)
{
	uint8_t i;
	for(i = 0; i < userTab.num; i++){//查找用户
		if((userTab.user[i]->ID & ISMASTERBIT) == ISMASTERBIT){
			if(userTab.user[i]->status == SENDING || userTab.user[i]->status == WAITACK){
				if(userTab.user[i]->outTimeFlag == 1){
					if(GetIntervalTick(userTab.user[i]->timConut) > userTab.user[i]->outTime){
							userTab.user[i]->status = OUTTIME;
					}
				}
			}
		}
	}
}

void ProtocalInit(void)
{
	GetIntervalTick = GetTimer1IntervalTick;
	GetCurrentTick = GetTimer1Tick;
}
/**
  *@brief  协议主函数
  *@param  None
  *@retval None
  */
void Protocalmain(void)
{
	/* 接收帧处理 */
	if(revControl.flag == REVISBUSYING){
		FrameDealWith();
	}
	/* 发送处理 */
	sendQueueData();
	/* 主机接收处理 */
	MasterRevScan();
	/* 从机接收处理 */
	//SlaveRevScan();
}

/**
  *@brief  添加用户
  *@param  None
  *@retval None
  */
ReturnStatus AddUserInfo(UserInfo *info)
{
	if(userTab.num + 1 <= MAXUSER_NUM){
		userTab.user[userTab.num] = info;
		userTab.num ++;
		return ISTURE;
	}
	return ISFAILURE;
}

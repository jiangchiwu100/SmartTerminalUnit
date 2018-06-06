/**
  * @file   hmi101.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/10
  * @brief  101实体文件
  */

#include "hmi101.h"
#include "uartDriver.h"
#include "string.h"
#include "lkdGuiUse.h"
#include "dlt634_HMImaster_disk.h"
#include "inoutUser.h"
#include "imageArray.h"
#include "ds18b20.h"

/* Hmi101发送处理 */
Hmi101CmdSend hmiSendInfo;
/* Hmi101接收处理信息结构 */
static struct Hmi101Rev hmiRevInfo;
static uint8_t FirstRunState;
/**
  *@brief 接收接口
  *@param  pbuf 接收buff
  *@param  count 期望值
  *@retval 实际值
  */
uint16_t Readx(uint8_t *pbuf, uint16_t count)
{
	uint16_t useLen,tLen;
	
	if(hmiRevInfo.pout > hmiRevInfo.pIn){
		useLen = HMIREVBUFF_MAX - hmiRevInfo.pout + hmiRevInfo.pIn;
		if(count < useLen){
			useLen = count;
		}
		if(useLen + hmiRevInfo.pout > HMIREVBUFF_MAX){
			tLen = HMIREVBUFF_MAX - hmiRevInfo.pout;
			memcpy((char *)pbuf,&hmiRevInfo.revBuff[hmiRevInfo.pout],tLen);
			memcpy((char *)&pbuf[tLen],&hmiRevInfo.revBuff[0],useLen - tLen);
			hmiRevInfo.pout = useLen - tLen;
		}
		else{
			memcpy((char *)pbuf,&hmiRevInfo.revBuff[hmiRevInfo.pout],useLen);
			hmiRevInfo.pout += useLen;
			if(hmiRevInfo.pout >= HMIREVBUFF_MAX){
				hmiRevInfo.pout = 0;
			}
		}
	}
	else if(hmiRevInfo.pout < hmiRevInfo.pIn){
		useLen = hmiRevInfo.pIn - hmiRevInfo.pout;
		if(count < useLen){
			useLen = count;
		}
		memcpy((char *)pbuf,&hmiRevInfo.revBuff[hmiRevInfo.pout],useLen);
		hmiRevInfo.pout += useLen;
	}
	else{
		useLen = 0;
	}
	return useLen;
}

/**
  *@brief 发送接口
  *@param  pbuf 要发送的数据
  *@param  count 数据量
  *@retval 实际发送数据量
  */
uint16_t WriteX(uint8_t *pbuf, uint16_t count)
{
	uint16_t i;
	for(i = 0; i < count; i++){
		USART_SendData(USART1,(uint16_t)*(pbuf + i));	
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
	}
	return count;
}

void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET){
		USART1->SR;
		USART1->DR;
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET){
		hmiRevInfo.revBuff[hmiRevInfo.pIn ++] = (uint8_t)USART1->DR;
		if(hmiRevInfo.pIn >= HMIREVBUFF_MAX){
			hmiRevInfo.pIn = 0;
		}
	}
	else if(USART_GetFlagStatus(USART1, USART_FLAG_IDLE)){
		USART1->SR;
		USART1->DR;
	}
}

/**
  *@brief  结束101压栈
  *@param  None
  *@retval None
  */
void EndCmd101Down(void)
{
	union{
		uint16_t len16;
		uint8_t len8[2];
	}frameLen;	
	frameLen.len16 = hmiSendInfo.pIn;
	hmiSendInfo.state = 2;
	hmiSendInfo.packBuff[0] = frameLen.len8[0];
	hmiSendInfo.packBuff[1] = frameLen.len8[1];
	hmiSendInfo.packBuff[2] = hmiSendInfo.cmdNum;
	DLT634_HMI_MASTER_C_REPLY(0,hmiSendInfo.packBuff);
}

/**
  *@brief cmd101压栈控制函数
  *@param  长度
  *@retval 0 可以压栈 1 不可以压栈
  */
static uint8_t Cmd101DownControl(uint16_t len)
{
	if(hmiSendInfo.state == 0){//栈空,可以压栈
		hmiSendInfo.cmdNum = 0;
		hmiSendInfo.pIn = 3;
		hmiSendInfo.state = 1;
	}
	if(hmiSendInfo.state == 2){//正在发送,请等待
		return 1;
	}
	if(hmiSendInfo.state == 1){//压栈中
		if(hmiSendInfo.pIn + len >= PACKBUFFMAX - 20){
			EndCmd101Down();
			return 1;
		}
	}
	return 0;
}

/**
  *@brief cmd101发送完成函数
  *@param  None
  *@retval None
  */
void Cmd101SendFinish(void)
{
	hmiSendInfo.state = 0;
}

/**
  *@brief HMI001命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t HmiCmd001Fun(uint8_t *pbuff)
{
	uint8_t i;
	switch(pbuff[CMD001_TYPE])
	{
	case C001TYPE_DISCRETE://不连续的
		for(i = 0; i < pbuff[CMD001_NUM]; i++){
			SetUserLedStatus(pbuff[CMD001_NUMBER + i*2], pbuff[CMD001_VALUE + i*2]);
		}break;
	case C001TYPE_CONTINUOUS://连续的
		ContinuousOutResult(pbuff[CMD001_NUMBER],pbuff[CMD001_NUM],&pbuff[CMD001_VALUE]);
	default:break;
	}
	return pbuff[CMD001_LEN];
}

/**
  *@brief Gui100命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd100Fun(uint8_t *pbuff)
{
	switch(pbuff[CMD100_TYPE])
	{
	case C100TYPE_HLINE:
		GuiHPointLine(pbuff[CMD100_BX], pbuff[CMD100_BY],\
			pbuff[CMD100_EX], pbuff[CMD100_POINT], pbuff[CMD100_COLOUR]);break;
	case C100TYPE_RLINE:
		GuiRPointLine(pbuff[CMD100_BX], pbuff[CMD100_BY],\
			pbuff[CMD100_EY], pbuff[CMD100_POINT], pbuff[CMD100_COLOUR]);break;
	case C100TYPE_BLINE:
		GuiBiasLine(pbuff[CMD100_BX], pbuff[CMD100_BY],\
			pbuff[CMD100_EX], pbuff[CMD100_EY], pbuff[CMD100_COLOUR]);break;
	default:break;
	}
	return pbuff[CMD100_LEN];
}

/**
  *@brief Gui101命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd101Fun(uint8_t *pbuff)
{
	switch(pbuff[CMD101_TYPE])
	{
	case C101TYPE_RECT:
		GuiRect(pbuff[CMD101_BX], pbuff[CMD101_BY],\
			pbuff[CMD100_EX], pbuff[CMD101_EY], pbuff[CMD101_COLOUR]);break;
	case C101TYPE_FILLRECT:
		GuiFillRect(pbuff[CMD101_BX], pbuff[CMD101_BY],\
			pbuff[CMD100_EX], pbuff[CMD101_EY], pbuff[CMD101_COLOUR]);break;
	default:break;
	}
	return pbuff[CMD101_LEN];
}

/**
  *@brief Gui102命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd102Fun(uint8_t *pbuff)
{
	switch(pbuff[CMD102_TYPE])
	{
	case C102TYPE_DEFAULT:break;
	case C102TYPE_PASSWORD:break;
	case C102TYPE_MESSAGE:break;
	default:break;
	}
	return pbuff[CMD102_LEN];
}

/**
  *@brief Gui103命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd103Fun(uint8_t *pbuff)
{
	GuiUpdateDisplayAll();
	return pbuff[CMD103_LEN];
}

/**
  *@brief Gui104命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd104Fun(uint8_t *pbuff)
{
	switch(pbuff[CMD104_TYPE])
	{
	case C104TYPE_OFFBLA:LCD_BLA_OFF();break;
	case C104TYPE_ONBLA:LCD_BLA_ON();break;
	case C104TYPE_OFFSCREEN:CloseLcdDisplay();break;
	case C104TYPE_ONSCREEN:OpenLcdDisplay();break;
	default:break;
	}
	return pbuff[CMD104_LEN];
}

/**
  *@brief Gui105命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd105Fun(uint8_t *pbuff)
{
	SCROLL tScroll;
	switch(pbuff[CMD105_TYPE])
	{
	case C105TYPE_HSCROLL:break;
	case C105TYPE_RSCROLL:
		tScroll.x= pbuff[CMD105_BX];
		tScroll.y= pbuff[CMD105_BY];
		tScroll.hight = pbuff[CMD105_SLEN];
		tScroll.lump = pbuff[CMD105_SLUMP];
		tScroll.max = pbuff[CMD105_SMAX];
		GuiVScroll(&tScroll);break;
	default:break;
	}
	return pbuff[CMD105_LEN];
}

/**
  *@brief Gui106命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd106Fun(uint8_t *pbuff)
{
	switch(pbuff[CMD106_TYPE])
	{
	case C106TYPE_DEFAULT:
		GuiButton(pbuff[CMD106_BX],pbuff[CMD106_BY],&pbuff[CMD106_CONTENT],pbuff[CMD106_FlAG]);
		break;
	case C106TYPE_OK:
		GuiButton(pbuff[CMD106_BX],pbuff[CMD106_BY],"确认",pbuff[CMD106_FlAG]);
		break;
	case C106TYPE_ESC:
		GuiButton(pbuff[CMD106_BX],pbuff[CMD106_BY],"取消",pbuff[CMD106_FlAG]);
		break;
	default:break;
	}
	return pbuff[CMD106_LEN];
}

/**
  *@brief Gui107命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd107Fun(uint8_t *pbuff)
{
	lkdBitmap image;
	switch(pbuff[CMD107_TYPE])
	{
	case C107TYPE_DEFAULT:break;
	case C107TYPE_SOJO:
		image.beginx = 0;
		image.beginy = 0;
		image.bitmap = (uint8_t *)imageTab[0].image;
		image.wide = imageTab[0].wide;
		image.high = imageTab[0].high;
		GuiAreaBitMap(&image,pbuff[CMD107_BX],pbuff[CMD107_BY],\
			pbuff[CMD107_EX], pbuff[CMD107_EY], pbuff[CMD107_FlAG]);
		break;
	case C107TYPE_WAIT:break;
	default:break;
	}
	return pbuff[CMD107_LEN];
}

/**
  *@brief Gui108命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd108Fun(uint8_t *pbuff)
{
	switch(pbuff[CMD108_TYPE])
	{
	case C108TYPE_DEFAULT:
		switch(pbuff[CMD108_FlAG]){
		case C108FLAG_LEFT:
			GuiFont12Align(pbuff[CMD108_BX],pbuff[CMD108_BY],\
				pbuff[CMD108_WIDE], FONT_LEFT,&pbuff[CMD108_CONTENT]);
			break;
		case C108FLAG_MID:
			GuiFont12Align(pbuff[CMD108_BX],pbuff[CMD108_BY],\
				pbuff[CMD108_WIDE], FONT_MID,&pbuff[CMD108_CONTENT]);
			break;
		case C108FLAG_RIGHT:
			GuiFont12Align(pbuff[CMD108_BX],pbuff[CMD108_BY],\
				pbuff[CMD108_WIDE], FONT_RIGHT,&pbuff[CMD108_CONTENT]);
			break;
		case C108FLAG_FEEDLINE:break;
		default:break;
		}
		break;
	default:break;
	}
	return pbuff[CMD108_LEN];
}

/**
  *@brief Gui108命令处理
  *@param  pbuff 内容数组
  *@retval 内容大小
  */
uint16_t GuiCmd109Fun(uint8_t *pbuff)
{
	switch(pbuff[CMD109_TYPE])
	{
	case C109TYPE_FORE:GuiSetForecolor(pbuff[CMD109_COLOUR]);break;
	case C109TYPE_BACK:GuiSetbackcolor(pbuff[CMD109_COLOUR]);break;
	case C109TYPE_EXCHANGE:GuiExchangeColor();break;
	default:break;
	}
	return pbuff[CMD109_LEN];
}

/**
  *@brief Gui命令处理
  *@param  pbuff 内容数组
  *@retval None
  */
void HmiGuiCmdResult(Hmi101FrameResult *pFrame)
{
	/* 偏移0为长度 1 为命令 */
	switch(pFrame->cmdBegin[1])
	{
	case HmiCmd001:pFrame->pContent = HmiCmd001Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd100:pFrame->pContent = GuiCmd100Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd101:pFrame->pContent = GuiCmd101Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd102:pFrame->pContent = GuiCmd102Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd103:pFrame->pContent = GuiCmd103Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd104:pFrame->pContent = GuiCmd104Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd105:pFrame->pContent = GuiCmd105Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd106:pFrame->pContent = GuiCmd106Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd107:pFrame->pContent = GuiCmd107Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd108:pFrame->pContent = GuiCmd108Fun(&pFrame->cmdBegin[0]);break;
	case GuiCmd109:pFrame->pContent = GuiCmd109Fun(&pFrame->cmdBegin[0]);break;
	default:pFrame->pContent = 0;break;
	}
	//GuiUpdateDisplayAll();
}

/**
  *@brief hmi101查询处理主函数
  *@param  pbuff 内容数组
  *@retval 0 成功 1 失败
  */
uint8_t hmi101Scan(uint8_t *pBuff)
{
	Hmi101FrameResult frameIs;
	frameIs.frameLen.len8[0] = pBuff[H101CMD_LEN_L];
	frameIs.frameLen.len8[1] = pBuff[H101CMD_LEN_H];
	frameIs.cmdNum = pBuff[H101CMD_CMDNUM];
	if(frameIs.frameLen.len16 < 2 || frameIs.cmdNum < 1){
		return 1;
	}
	frameIs.pContent = 0;
	frameIs.cmdOffset = 0;
	while(frameIs.cmdNum){
		frameIs.cmdOffset += frameIs.pContent;
		if(frameIs.cmdOffset + H101CMD_CMD >= frameIs.frameLen.len16){
			return 1;
		}
		frameIs.cmdBegin = &pBuff[frameIs.cmdOffset + H101CMD_CMD];
		HmiGuiCmdResult(&frameIs);
		frameIs.cmdNum --;
	}
	return 0;
}

/**
  *@brief 开关变位下发
  *@param  None
  *@retval None
  */
void SwitchChangeCmdSend(void)
{
	struct SwitchQueue *tSwitch;
	tSwitch = GetswitchQueueP();
	if(tSwitch->pIn != tSwitch->pOut){
		if(Cmd101DownControl(6) != 0){
			return;
		}
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_CMD] = 2;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_TYPE] = C002TYPE_DISCRETE;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_NUM] = 1;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_NUMBER] = tSwitch->number[tSwitch->pOut] + 50;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_VALUE] = tSwitch->state[tSwitch->pOut];	
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_LEN] = 6;
		hmiSendInfo.pIn += 6;
		hmiSendInfo.cmdNum += 1;
		tSwitch->pOut ++;
		if(tSwitch->isfull != 0){
			tSwitch->isfull = 0;
		}
		if(tSwitch->pOut >= SWITCHQUEUE_MAX){
			tSwitch->pOut = 0;
		}
		EndCmd101Down();
	}
}

/**
  *@brief 开关变位下发
  *@param  None
  *@retval 0 成功 1 不可以压栈 2 参数有错
  */
uint8_t SwitchAllStateCmdSend(uint8_t num, uint8_t beginNumber,uint8_t *pBuff)
{
	if(num == 0){
		return 2;
	}
	if(Cmd101DownControl(num + 5) != 0){
		return 1;
	}
	hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_CMD] = 2;
	hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_TYPE] = C002TYPE_CONTINUOUS;
	hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_NUM] = num;
	hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_NUMBER] = beginNumber + 50;
	memcpy(&hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_VALUE],pBuff,num);
	hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_LEN] = num + 5;
	hmiSendInfo.pIn += hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_LEN];
	hmiSendInfo.cmdNum += 1;
	EndCmd101Down();
	return 0;
}
/**
  *@brief 开关全体状态下发
  *@param  None
  *@retval 0 成功 1 不可以压栈 2 参数有错
  */
void SwitchAllStateSendControl(uint8_t *flag)
{
	static uint32_t sendTick;
	if(*flag == 0){
		if(SwitchAllStateFill() == 0){
			sendTick = GetTimer1Tick();
			*flag = 1;
		}
	}
	else if(*flag == 1 && GetTimer1IntervalTick(sendTick) > 10000){
		if(SwitchAllStateFill() == 0){
			sendTick = GetTimer1Tick();
		}
	}
}
/**
  *@brief 按键变位下发
  *@param  None
  *@retval None
  */
void KeyChangeCmdSend(void)
{
	struct KeyQueue *tKey;
	tKey = GetkeyQueueP();
	if(tKey->pIn != tKey->pOut){
		if(Cmd101DownControl(6) != 0){
			return;
		}
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_CMD] = 2;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_TYPE] = C002TYPE_DISCRETE;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_NUM] = 1;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_NUMBER] = tKey->number[tKey->pOut];
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_VALUE] = tKey->state[tKey->pOut];	
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_LEN] = 6;
		hmiSendInfo.pIn += 6;
		hmiSendInfo.cmdNum += 1;
		tKey->pOut ++;
		if(tKey->isfull != 0){
			tKey->isfull = 0;
		}
		if(tKey->pOut >= SWITCHQUEUE_MAX){
			tKey->pOut = 0;
		}
		EndCmd101Down();
	}
}

/**
  *@brief 模拟量下发
  *@param  None
  *@retval None
  */
void AnalogCmdSend(void)
{
	static uint32_t sendTick;
	static float currentTemper;
	union{
		float tf;
		uint8_t t8[4];
	}temper;
	if(Cmd101DownControl(9) != 0){
		return;
	}
	temper.tf = GetTemperature();
	if(GetTimer1IntervalTick(sendTick) > 15000 ||\
		temper.tf + 0.5 >= currentTemper || temper.tf - 0.5 <= currentTemper)
	{
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD003_CMD] = 3;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD003_TYPE] = C003TYPE_DISCRETE;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD003_NUM] = 1;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD003_NUMBER] = 0x01;
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD003_VALUE_LL] = temper.t8[0];
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD003_VALUE_LH] = temper.t8[1];
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD003_VALUE_HL] = temper.t8[2];
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD003_VALUE_HH] = temper.t8[3];	
		hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD003_LEN] = 9;
		hmiSendInfo.pIn += hmiSendInfo.packBuff[hmiSendInfo.pIn + CMD002_LEN];
		hmiSendInfo.cmdNum += 1;
		EndCmd101Down();
		currentTemper = temper.tf;
		sendTick = GetTimer1Tick();
	}
}

/**
  *@brief hmi101主函数
  *@param  pbuff 内容数组
  *@retval 0 成功 1 失败
  */
void Hmi101Main(void)
{
	static uint32_t hmi101Tick;
	static uint8_t switchflag;
	if(FirstRunState == 0){
		KeyQueueInit();
		SwitchQueueInit();
		switchflag = 0;
		FirstRunState = 1;
	}
	if(GetTimer1IntervalTick(hmi101Tick) > 10){
		hmi101Tick = GetTimer1Tick();
		DLT634_HMI_MasterTask(0);
	}
	KeyChangeCmdSend();
	SwitchChangeCmdSend();
	SwitchAllStateSendControl(&switchflag);
	AnalogCmdSend();
}

/**
  *@brief hmi101初始化
  *@param  None
  *@retval None
  */
void Hmi101Init(void)
{
	FirstRunState = 0;
}

/* END */

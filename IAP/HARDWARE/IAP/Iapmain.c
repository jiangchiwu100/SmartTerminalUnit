/**
  * @file   Iapmain.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/04/13
  * @brief  在线升级
  */

#include "Iapmain.h"
#include "flashOperate.h"
#include "systemconfig.h"
#include "userInfo.h"

static void IapUserSlaveFun(void);

/* 获取时间 */
static uint32_t (* GetIntervalTick)(uint32_t beginTime);
static uint32_t (* GetCurrentTick)(void);

/* iapSlave接收buff */
static uint8_t RevIapBuffSlave[REVIAPSLAVE_MAX];
static uint8_t SendIapBuffSlave[SENDIAPSLAVE_MAX];
/* iapSlave */
static UserInfo IapSlave = {0x04,0,IDLE,1000,0,\
	REVIAPSLAVE_MAX,RevIapBuffSlave,IapUserSlaveFun};

/* 超时检测控制 */
static struct IapTimeOutCheck IapTimCheck;
/* iapmain 控制步骤 */
static enum IapStepEnum IapStep;

struct FileOperate saveFile;
/* iap寄存器 */
static struct IapRegInfo IapReg = {
	IAPREG_CONNECT,0,
	IAPREG_FIRSTPOWER,0,
	IAPREG_RUNSTATUS,0,
	IAPREG_UPDATA,0,
	IAPREG_GETPROGRAM,0,
};
struct RegisterTab RegTab[] = {
	{IAPREG_CONNECT,1,&(IapReg.connect.value)},
	{IAPREG_FIRSTPOWER,1,&(IapReg.firstPower.value)},
	{IAPREG_RUNSTATUS,1,&(IapReg.runStatus.value)},
	{IAPREG_UPDATA,1,&(IapReg.updata.value)},
	{IAPREG_GETPROGRAM,1,&(IapReg.GetProgram.value)},
};
/**
  *@brief  设置IapTimCheck
  *@param  outTime 超时时间
  *@param  flag 0 关闭超时检测 1 打开
  *@retval None
  */
void SetIapTimCheck(uint32_t outTime, uint8_t flag, uint8_t source)
{
	IapTimCheck.count = GetCurrentTick();
	IapTimCheck.source = source;
	IapTimCheck.flag = flag;
	IapTimCheck.outTime = outTime;
}

/**
  *@brief  设置开始时间
  *@param  None
  *@retval None
  */
void SetIapTimBeginTim(void)
{
	
}

/**
  *@brief  超时时间检测
  *@param  None
  *@retval None
  */
void IapOutTimCheck(void)
{
	if(IapTimCheck.flag == 1){//开启检测
		if(GetIntervalTick(IapTimCheck.count) > IapTimCheck.outTime){
			IapTimCheck.overFun(IapTimCheck.source);
		}
	}
}

/**
  *@brief  超时回掉函数
  *@param  None
  *@retval None
  */
static void IapTimOverFun(uint8_t source)
{
	IapTimCheck.flag = 0;
	switch(source){
	case 0:break;
	}
}

static void AckRegLook(void)
{
	uint8_t eachSize;
	uint16_t i,j;
	Uint8To16 sendNum;
	Uint8To16 regAddr;
	if(IapSlave.pRevBuff[M03_REG_ADDRNUM] >= 1){
		eachSize = IapSlave.pRevBuff[M03_REG_EACHSIZE];
		SendIapBuffSlave[S03_REG_EACHSIZE] = eachSize;
		SendIapBuffSlave[S03_REG_ADDRNUM] = IapSlave.pRevBuff[M03_REG_ADDRNUM];
		SendIapBuffSlave[S03_REG_SERIALNUM] = 0;
		for(i = 0; i < IapSlave.pRevBuff[M03_REG_ADDRNUM]; i++){
			regAddr.Uint8[0] = IapSlave.pRevBuff[M03_REG1_ADDRL + i*2];
			regAddr.Uint8[1] = IapSlave.pRevBuff[M03_REG1_ADDRH + i*2];
			for(j = 0; j < REGTABMUN; j++){
				if(RegTab[j].regAddr == regAddr.Uint16){
					SendIapBuffSlave[S03_REG1_ADDRL + i*(eachSize+2)] = regAddr.Uint8[0];
					SendIapBuffSlave[S03_REG1_ADDRL + i*(eachSize+2)] = regAddr.Uint8[1];
					SendIapBuffSlave[S03_REG1_ADDRL + i*(eachSize+2)] = *(RegTab[j].value);
					break;
				}
			}
		}
	}
	sendNum.Uint16 = (eachSize + 2)*SendIapBuffSlave[S03_REG_ADDRNUM] + 6;
	SendIapBuffSlave[S03_CMD] = 0x03;
	SendIapBuffSlave[S03_INFOBYTESIZE_L] = sendNum.Uint8[0];
	SendIapBuffSlave[S03_INFOBYTESIZE_H] = sendNum.Uint8[1];
	UserDataSendFill(&IapSlave,SendIapBuffSlave);
}

static void AckRegModfiy(void)
{
	uint8_t eachSize;
	uint16_t i,j;
	Uint8To16 regAddr;
	if(IapSlave.pRevBuff[M10_REG_ADDRNUM] >= 1){
		eachSize = IapSlave.pRevBuff[M10_REG_EACHSIZE];
		for(i = 0; i < IapSlave.pRevBuff[M03_REG_ADDRNUM]; i++){
			regAddr.Uint8[0] = IapSlave.pRevBuff[M10_REG1_ADDRL + i*(eachSize+2)];
			regAddr.Uint8[1] = IapSlave.pRevBuff[M10_REG1_ADDRH + i*(eachSize+2)];
			for(j = 0; j < REGTABMUN; j++){
				if(RegTab[j].regAddr == regAddr.Uint16){
					*(RegTab[j].value) = SendIapBuffSlave[M10_REG1_ADDRL + i*(eachSize+2) + 2];
					break;
				}
			}
		}
	}
	SendIapBuffSlave[S03_CMD] = 0x10;
	SendIapBuffSlave[S10_ACKFLAG] = 0x01;
	SendIapBuffSlave[S03_INFOBYTESIZE_L] = 0x04;
	SendIapBuffSlave[S03_INFOBYTESIZE_H] = 0x00;
	UserDataSendFill(&IapSlave,SendIapBuffSlave);
}
/**
  *@brief  从机接收处理
  *@param  None
  *@retval None
  */
static void IapUserSlaveFun(void)
{
	if(IapSlave.pRevBuff[M03_CMD] == 0x03){//查询寄存器
		AckRegLook();
	}
	else if(IapSlave.pRevBuff[M10_CMD] == 0x10){
		AckRegModfiy();
	}
	else if(IapSlave.pRevBuff[M04_CMD] == 0x04){
		if(IapSlave.pRevBuff[M04_FILENUMBER] == 0x01){
			saveFile.number = IapSlave.pRevBuff[M04_FILENUMBER];
			saveFile.flag = 1;
		}
	}
	else if(IapSlave.pRevBuff[M05_CMD] == 0x05){
		if(saveFile.flag == 2 && saveFile.number == IapSlave.pRevBuff[M04_FILENUMBER]){
			saveFile.flag = 3;
		}
	}
	else if(IapSlave.pRevBuff[M06_CMD] == 0x06){
		if(IapSlave.pRevBuff[M06_FILENUMBER] == 0x02){
			saveFile.number = IapSlave.pRevBuff[M06_FILENUMBER];
			saveFile.flag = 1;
		}
	}
	else if(IapSlave.pRevBuff[M07_CMD] == 0x07){
		if(saveFile.flag == 2 && saveFile.number == IapSlave.pRevBuff[M07_FILENUMBER]){
			saveFile.flag = 3;
		}
	}
}

/**
  *@brief  等待连接主板
  *@param  None
  *@retval None
  */
void WaitConnectMainBoard(void)
{
	if(IapReg.connect.value == 1){//连接到主板
		SetIapTimCheck(5000,0,0);
		IapStep = IAP_CONNECTFINISH;
	}
}

/**
  *@brief  发送当前程序文件
  *@param  None
  *@retval None
  */
static void SendProgramFile(uint32_t offset, uint16_t length)
{
	Uint8To16 realLen;
	realLen.Uint16 = ReadProgram(offset + IAPLOADAPP_ADDR,\
		&SendIapBuffSlave[S05_FILECONTENT],length);
	if(realLen.Uint16 == 0){
		SendIapBuffSlave[S05_FILEACKFLAG] = S05_REVERROR;
	}
	else if(offset + IAPLOADAPP_ADDR + realLen.Uint16 == PROGRAM_ENDADDR){
		saveFile.flag = 0;
		SendIapBuffSlave[S05_FILEACKFLAG] = S05_FINENED;
	}
	else{
		SendIapBuffSlave[S05_FILEACKFLAG] = S05_REVOK;
	}
	SendIapBuffSlave[S05_CMD] = 0x05;
	SendIapBuffSlave[S05_FILENUMBER] = 0x01;
	SendIapBuffSlave[S05_CONTENTLEN_L] = realLen.Uint8[0];
	SendIapBuffSlave[S05_CONTENTLEN_H] = realLen.Uint8[1];
	SendIapBuffSlave[S05_INFOBYTESIZE_L] = realLen.Uint8[0];
	SendIapBuffSlave[S05_INFOBYTESIZE_H] = realLen.Uint16 + 7;
	UserDataSendFill(&IapSlave,SendIapBuffSlave);
}

/**
  *@brief  保存当前程序
  *@param  None
  *@retval None
  */
static void SaveCurrentProgram(void)
{
	Uint8To32 dataOffset;
	Uint8To16 sendData;
	if(saveFile.flag == 1){//请求获取文件名
		SendIapBuffSlave[S04_CMD] = 0x04;
		SendIapBuffSlave[S04_FILENUMBER] = 0x01;
		SendIapBuffSlave[S04_ACKFLAG] = S04_REVOK;
		SendIapBuffSlave[S04_FILELENGTH_LL] = 0xFF;/* 不知道文件大小 */
		SendIapBuffSlave[S04_FILELENGTH_LH] = 0xFF;
		SendIapBuffSlave[S04_FILELENGTH_HL] = 0xFF;
		SendIapBuffSlave[S04_FILELENGTH_HH] = 0xFF;
		SendIapBuffSlave[S04_FILENAMELENGTH] = 7;
		SendIapBuffSlave[S04_FILENAME] = 'H';
		SendIapBuffSlave[S04_FILENAME + 1] = 'M';
		SendIapBuffSlave[S04_FILENAME + 2] = 'I';
		SendIapBuffSlave[S04_FILENAME + 3] = '.';
		SendIapBuffSlave[S04_FILENAME + 4] = 'b';
		SendIapBuffSlave[S04_FILENAME + 5] = 'i';
		SendIapBuffSlave[S04_FILENAME + 6] = 'n';
		SendIapBuffSlave[S04_INFOBYTESIZE_L] = 0x11;
		SendIapBuffSlave[S04_INFOBYTESIZE_H] = 0x00;
		UserDataSendFill(&IapSlave,SendIapBuffSlave);
		saveFile.flag = 2;
	}
	if(saveFile.flag == 3){
		IapSlave.pRevBuff[M05_FILEOFFSET_LL] = dataOffset.Uint8[0];
		IapSlave.pRevBuff[M05_FILEOFFSET_LH] = dataOffset.Uint8[1];
		IapSlave.pRevBuff[M05_FILEOFFSET_HL] = dataOffset.Uint8[2];
		IapSlave.pRevBuff[M05_FILEOFFSET_HH] = dataOffset.Uint8[3];
		IapSlave.pRevBuff[M05_CONTENTLEN_L] = sendData.Uint8[0];
		IapSlave.pRevBuff[M05_CONTENTLEN_H] = sendData.Uint8[1];
		if(sendData.Uint16 > 512){
			sendData.Uint16 = 512;
		}
		SendProgramFile(dataOffset.Uint32, sendData.Uint16);
		saveFile.flag = 2;
	}
}

/**
  *@brief  写当前程序文件
  *@param  None
  *@retval None
  */
static void WriteProgramFile(void)
{
	uint8_t checkflag = 0;
	uint8_t returnStatus;
	Uint8To32 dataOffset;
	Uint8To16 sendData;
	dataOffset.Uint8[0] = IapSlave.pRevBuff[M07_FILEOFFSET_LL];
	dataOffset.Uint8[1] = IapSlave.pRevBuff[M07_FILEOFFSET_LH];
	dataOffset.Uint8[2] = IapSlave.pRevBuff[M07_FILEOFFSET_HL];
	dataOffset.Uint8[3] = IapSlave.pRevBuff[M07_FILEOFFSET_HH];
	sendData.Uint8[0] = IapSlave.pRevBuff[M07_CONTENTLEN_L];
	sendData.Uint8[1] = IapSlave.pRevBuff[M07_CONTENTLEN_H];
	
	if(sendData.Uint16 > 512){
		SendIapBuffSlave[S07_FILEACKFLAG] = S07_REVERROR;
		checkflag = 1;
	}
	if(dataOffset.Uint32 + IAPLOADAPP_ADDR + sendData.Uint16 > PROGRAM_ENDADDR){
		SendIapBuffSlave[S07_FILEACKFLAG] = S07_REVERROR;
		checkflag = 1;	
	}
	if(checkflag == 0){
		returnStatus = WriteProgram(dataOffset.Uint32 + IAPLOADAPP_ADDR,&IapSlave.pRevBuff[M07_FILECONTENT],sendData.Uint16);
		if(returnStatus != 0){
			SendIapBuffSlave[S07_FILEACKFLAG] = S07_REVERROR;
		}
		else if(IapSlave.pRevBuff[M07_FILEFLAG] == M07_FILEEND){
			SendIapBuffSlave[S07_FILEACKFLAG] = S07_REVEND;
		}
		else{
			SendIapBuffSlave[S07_FILEACKFLAG] = S07_REVOK;
		}
	}
	if(SendIapBuffSlave[S07_FILEACKFLAG] != S07_REVEND){
		saveFile.flag = 2;
	}
	else{
		saveFile.flag = 2;
	}
	SendIapBuffSlave[S07_CMD] = 0x07;
	SendIapBuffSlave[S07_FILENUMBER] = saveFile.number;
	SendIapBuffSlave[S07_FILEOFFSET_LL] = dataOffset.Uint8[0];
	SendIapBuffSlave[S07_FILEOFFSET_LH] = dataOffset.Uint8[1];
	SendIapBuffSlave[S07_FILEOFFSET_HL] = dataOffset.Uint8[2];
	SendIapBuffSlave[S07_FILEOFFSET_HH] = dataOffset.Uint8[3];
	SendIapBuffSlave[S07_CONTENTLEN_L] = sendData.Uint8[0];
	SendIapBuffSlave[S07_CONTENTLEN_H] = sendData.Uint8[0];
	UserDataSendFill(&IapSlave,SendIapBuffSlave);
}
/**
  *@brief  接收新程序
  *@param  None
  *@retval None
  */
void GetNewProgram(void)
{
	if(saveFile.flag == 1){//请求获取文件名
		SendIapBuffSlave[S06_CMD] = 0x06;
		SendIapBuffSlave[S06_FILENUMBER] = saveFile.number;
		SendIapBuffSlave[S06_ACKFLAG] = S06_REVOK;
		SendIapBuffSlave[S06_INFOBYTESIZE_L] = 0x05;
		SendIapBuffSlave[S06_INFOBYTESIZE_L] = 0x00;
		UserDataSendFill(&IapSlave,SendIapBuffSlave);
		saveFile.flag = 2;
	}
	if(saveFile.flag == 3){
		WriteProgramFile();
	}
}
/**
  *@brief  控制处理函数
  *@param  None
  *@retval None
  */
void ControlDealWith(void)
{
	if(IapReg.iapJump.value == 1){//执行跳转
		IapReg.iapJump.value = 2;//开始跳转
		IapLoadApp(IAPLOADAPP_ADDR);
		IapReg.iapJump.value = 3;//跳转失败
	}
	if(IapReg.updata.value == 1){//保存程序
		SaveCurrentProgram();
	}
	else if(IapReg.updata.value == 2){//擦出扇区
		if(EreaseProgram() == 0){
			IapReg.updata.value = 3;
		}
		else{
			IapReg.updata.value = 4;
		}
	}
	else if(IapReg.updata.value == 5){//更新程序
		GetNewProgram();
	}
}

/**
  *@brief  Iapmain初始化
  *@param  None
  *@retval None
  */
void IapmainInit(void)
{
	GetIntervalTick = GetTimer1IntervalTick;
	GetCurrentTick = GetTimer1Tick;
	IapTimCheck.overFun = IapTimOverFun;
	IapStep = IAP_INIT;
	SetIapTimCheck(5000,0,0);
}

/**
  *@brief  主函数
  *@param  None
  *@retval None
  */
void IapMianScan(void)
{
	if(IapStep == IAP_INIT){//初始化
		SetIapTimCheck(5000,1,0);
		IapStep = IAP_WAITCONNECT;
	}
	if(IapStep == IAP_WAITCONNECT){//等待连接
		WaitConnectMainBoard();
	}
	if(IapStep == IAP_CONNECTFINISH){//设置控制处理超时
		//SetIapTimCheck(5000,1,0);
		IapStep = IAP_WAITCONTROL;
	}
	if(IapStep == IAP_WAITCONTROL){//控制处理
		ControlDealWith();
	}
	IapOutTimCheck();
}


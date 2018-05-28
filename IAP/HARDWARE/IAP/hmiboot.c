/**
  * @file   hmiboot.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/04/12
  * @brief  面板通信
  */

#include "hmiboot.h"
#include "systemconfig.h"
#include "gwprotocal.h"
#include "userInfo.h"
#include <string.h>
#include "ff.h"
#include "drv_fm25vxx.h"
#include "IapControl.h"

/* iapMaster接收buff */
static uint8_t RevIapBuffMaster[REVIAPMASTER_MAX];
static uint8_t SendIapBuffMaster[SENDIAPMASTER_MAX];
/* iapMaster */
static UserInfo IapMaster = {0x05,0,IDLE,1,10000,0,REVIAPMASTER_MAX,RevIapBuffMaster,NULL};
/* 超时检测控制 */
static struct IapTimeOutCheck IapTimCheck;

static uint8_t FileBuff[FILEBUFFSIZE];
static struct FileOperate saveFile = {0,0,0,0,FileBuff,FILEBUFFSIZE,0,0,0};
static FIL hmiFile;//文件1
uint8_t *hmiName	= "h.bin";
/* iap寄存器 */
static struct IapRegInfo IapReg = {
	IAPREG_CONNECT,0,
	IAPREG_FIRSTPOWER,0,
	IAPREG_RUNSTATUS,0,
	IAPREG_UPDATA,0,
	IAPREG_GETPROGRAM,0,
	IAPREG_IAPJUMP,0,
};
struct RegisterTab RegTab[] = {
	{IAPREG_CONNECT,1,&(IapReg.connect.value)},
	{IAPREG_FIRSTPOWER,1,&(IapReg.firstPower.value)},
	{IAPREG_RUNSTATUS,1,&(IapReg.runStatus.value)},
	{IAPREG_UPDATA,1,&(IapReg.updata.value)},
	{IAPREG_GETPROGRAM,1,&(IapReg.GetProgram.value)},
	{IAPREG_IAPJUMP,1,&(IapReg.iapJump.value)},
};

/* 获取时间 */
static uint32_t (* GetIntervalTick)(uint32_t beginTime);
static uint32_t (* GetCurrentTick)(void);
/* 超时检测控制 */
static struct IapTimeOutCheck IapTimCheck;

/**
  *@brief  设置IapTimCheck
  *@param  outTime 超时时间
  *@param  flag 0 关闭超时检测 1 打开
  *@retval None
  */
void SetIapTimCheck(uint32_t outTime, uint8_t flag, uint8_t source,uint8_t *name)
{
	IapTimCheck.count = GetCurrentTick();
	IapTimCheck.source = source;
	IapTimCheck.flag = flag;
	IapTimCheck.outTime = outTime;
	IapTimCheck.name = name;
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
			IapTimCheck.overFun(IapTimCheck.source,IapTimCheck.name);
		}
	}
}

/**
  *@brief  超时回掉函数
  *@param  None
  *@retval None
  */
static void IapTimOverFun(uint8_t source,uint8_t *name)
{
	IapTimCheck.flag = 0;
	switch(source){
	case TO_SAVEFILEFALSE:
		f_close(&hmiFile);	
		SetControlStep(CON_HMIPRO_FINISH);break;
	case TO_ERASEFALSE:
	case TO_WRITEPROFLASE:
		SetControlStep(CON_HMIPRO_BACK);break;
	case TOHMIFINISH:
		SetControlStep(CON_HMIPRO_FINISH);break;
	default:break;
	}
	DebugPrintf("%s-等待超时,源:%d\r\n",name,source);
}

uint32_t GetHmifileSize(void)
{
	 return (*(uint32_t *)(HMIPROGRAMSIZEADDR));
}
/**
  *@brief  获取文件内容
  *@param  pFile 文件
  *@param  pBuff 存放内容指针
  *@retval None
  */
void GetFileConnent(struct FileOperate *pFile,uint8_t *pBuff)
{
	uint16_t i;
	if(pFile->len + pFile->offset >= pFile->fileSize){
		pFile->len = pFile->fileSize - pFile->offset;
		pFile->fileFlag = 1;
	}
	if(pFile->offset > pFile->fileSize){
		pFile->len = 0;
		pFile->fileFlag = 1;
	}
	//DebugPrintf("\r\n");
	for(i = 0; i < pFile->len; i++){
		pBuff[i] = *(uint8_t *)(HMIPROGRAMSTARTADDR + i + pFile->offset);
		//DebugPrintf("%02X  ",pBuff[i]);
	}
	//DebugPrintf("\r\n");
}
/**
  *@brief  处理文件内容
  *@param  pFile 文件
  *@param  pBuff 内容指针
  *@param  len 长度
  *@retval None
  */
void DealWithRevFile(struct FileOperate *pFile,const uint8_t *pBuff)
{
	uint16_t i;
	uint32_t realWrite = 0;
	uint32_t fileSize;
	uint8_t res;
	//DebugPrintf("\r\n");
	for(i = 0; i < pFile->len; i++){
		pFile->pbuff[pFile->pBuffIn++] = pBuff[i];
		//DebugPrintf("%02X  ",pBuff[i]);
		if(pFile->pBuffIn >= pFile->buffSize){
			//DebugPrintf("\r\n");
			pFile->pBuffIn = 0;
			res = f_write(&hmiFile,pFile->pbuff,pFile->buffSize,(UINT *)&realWrite);
			DebugPrintf("resWrite:%d\r\n",res);
			DebugPrintf("fileFlag0realWrite:%d\r\n",realWrite);
		}
	}
	if(pFile->fileFlag == 1){
		DebugPrintf("\r\n");
		if(pFile->pBuffIn != 0){
			res = f_write(&hmiFile,pFile->pbuff,pFile->pBuffIn,(UINT *)&realWrite);
			DebugPrintf("resWrite:%d\r\n",res);
			DebugPrintf("fileFlag1realWrite:%d\r\n",realWrite);
		}
		DebugPrintf("saveEnd\r\n");
		fileSize = f_size(&hmiFile);
		DebugPrintf("fileSize:%ld\r\n",fileSize);
		f_close(&hmiFile);	
	}
}

/**
  *@brief  修改单个寄存器命令
  *@param  reg 寄存器
  *@param  value 值
  *@retval None
  */
void SendModfiyOneType1RegCmd(Type1Reg *reg,uint8_t value)
{
	Uint8To16 regAddr;
	regAddr.Uint16 = reg->addr;
	SendIapBuffMaster[M10_CMD] = 0x10;
	SendIapBuffMaster[M10_REG_ADDRNUM] = 1;
	SendIapBuffMaster[M10_REG_SERIALNUM] = 1;
	SendIapBuffMaster[M10_REG_EACHSIZE] = 1;
	SendIapBuffMaster[M10_REG1_ADDRL] = regAddr.Uint8[0];
	SendIapBuffMaster[M10_REG1_ADDRH] = regAddr.Uint8[1];
	SendIapBuffMaster[M10_REG1_CONTENT] = value;
	SendIapBuffMaster[M03_INFOBYTESIZE_L] = 0x09;
	SendIapBuffMaster[M03_INFOBYTESIZE_H] = 0x00;
	UserDataSendFill(&IapMaster,SendIapBuffMaster);
}

/**
  *@brief  等待修改单个Type1寄存器应答
  *@param  None
  *@retval 0 应答成功 1 等待中 2 应答失败
  */
uint8_t WaitModfiyRegAck(void)
{
	UserErrorStatus resultCode;
	resultCode = GetMasterUserInfoStatus(&IapMaster);
	if(resultCode == REVACKOK){
		if(IapMaster.pRevBuff[S10_CMD] == 0x10 &&\
			IapMaster.pRevBuff[S10_ACKFLAG] == S10_MODFIYOK)
		{
			return 0;
		}
		else{
			return 2;
		}
	}
	else if(resultCode == ISCONNECTFAIL){
	 return 2;
	}
	else{
		return 1;
	}
}

/**
  *@brief  查询单个寄存器命令
  *@param  reg 寄存器
  *@retval None
  */
void SendSeeOneType1RegCmd(Type1Reg *reg)
{
	Uint8To16 regAddr;
	regAddr.Uint16 = reg->addr;
	SendIapBuffMaster[M03_CMD] = 0x03;
	SendIapBuffMaster[M03_REG_ADDRNUM] = 1;
	SendIapBuffMaster[M03_REG_SERIALNUM] = 1;
	SendIapBuffMaster[M03_REG_EACHSIZE] = 1;
	SendIapBuffMaster[M03_REG1_ADDRL] = regAddr.Uint8[0];
	SendIapBuffMaster[M03_REG1_ADDRH] = regAddr.Uint8[1];
	SendIapBuffMaster[M03_INFOBYTESIZE_L] = 0x08;
	SendIapBuffMaster[M03_INFOBYTESIZE_H] = 0x00;
	UserDataSendFill(&IapMaster,SendIapBuffMaster);
}

/**
  *@brief  等待查询单个Type1寄存器应答
  *@param  reg 寄存器
  *@retval 0 应答成功 1 等待中 2 应答失败
  */
uint8_t WaitSeeOneType1RegAck(Type1Reg *reg)
{
	UserErrorStatus resultCode;
	Uint8To16 regAddr;
	resultCode = GetMasterUserInfoStatus(&IapMaster);
	if(resultCode == REVACKOK){
		regAddr.Uint8[0] = IapMaster.pRevBuff[S03_REG1_ADDRL];
		regAddr.Uint8[1] = IapMaster.pRevBuff[S03_REG1_ADDRH];
		if(IapMaster.pRevBuff[S03_CMD] == 0x03 && regAddr.Uint16 == reg->addr){
			reg->value = IapMaster.pRevBuff[S03_REG1_CONTENT];
			return 0;
		}
		else{
			return 2;
		}
	}
	else if(resultCode == ISCONNECTFAIL){
	 return 2;
	}
	else{
		return 1;
	}
}

/**
  *@brief  主机请求从机发送文件
  *@param  pFile 文件结构体
  *@retval None
  */
void SendRequestFileBeginCmd(struct FileOperate *pFile)
{
	SendIapBuffMaster[M04_CMD] = 0x04;
	SendIapBuffMaster[M04_FILENUMBER] = pFile->number;
	SendIapBuffMaster[M04_FILELENGTH] = 0;
	SendIapBuffMaster[M04_INFOBYTESIZE_L] = 0x05;
	SendIapBuffMaster[M04_INFOBYTESIZE_H] = 0x00;
	UserDataSendFill(&IapMaster,SendIapBuffMaster);
}

/**
  *@brief  主机请求从机发送文件等待回复
  *@param  pFile 文件结构体
  *@retval 0 成功 1 等待中 2 失败
  */
uint8_t WaitRequestFileBeginAck(struct FileOperate *pFile)
{
	UserErrorStatus resultCode;
	resultCode = GetMasterUserInfoStatus(&IapMaster);
	if(resultCode == REVACKOK){
		if(IapMaster.pRevBuff[S04_CMD] == 0x04 &&\
			IapMaster.pRevBuff[S04_FILENUMBER] == pFile->number &&\
			IapMaster.pRevBuff[S04_ACKFLAG] == S04_REVOK)
		{
			return 0;
		}
		else{
			return 2;
		}
	}
	else if(resultCode == ISCONNECTFAIL){
	 return 2;
	}
	else{
		return 1;
	}
}

/**
  *@brief  主机请求从机发送文件内容
  *@param  pFile 文件结构体
  *@retval None
  */
void SendRequestFileConnectCmd(struct FileOperate *pFile)
{
	Uint8To32 offset;
	Uint8To16 size;
	size.Uint16 = pFile->len;
	offset.Uint32 = pFile->offset;
	DebugPrintf("fileoffset:%08X\r\n",offset.Uint32);
	SendIapBuffMaster[M05_CMD] = 0x05;
	SendIapBuffMaster[M05_FILENUMBER] = pFile->number;
	SendIapBuffMaster[M05_FILEOFFSET_LL] = offset.Uint8[0];
	SendIapBuffMaster[M05_FILEOFFSET_LH] = offset.Uint8[1];
	SendIapBuffMaster[M05_FILEOFFSET_HL] = offset.Uint8[2];
	SendIapBuffMaster[M05_FILEOFFSET_HH] = offset.Uint8[3];
	
	SendIapBuffMaster[M05_CONTENTLEN_L] = size.Uint8[0];
	SendIapBuffMaster[M05_CONTENTLEN_H] = size.Uint8[1];
	SendIapBuffMaster[M05_INFOBYTESIZE_L] = 0x0A;
	SendIapBuffMaster[M05_INFOBYTESIZE_H] = 0x00;
	UserDataSendFill(&IapMaster,SendIapBuffMaster);
}

/**
  *@brief  主机请求从机发送文件内容等待回复
  *@param  pFile 文件结构体
  *@retval 0 成功 1文件结束  2 等待中 3 失败
  */
uint8_t WaitRequestFileConnectAck(struct FileOperate *pFile)
{
	UserErrorStatus resultCode;
	Uint8To16 currentLen;
	resultCode = GetMasterUserInfoStatus(&IapMaster);
	if(resultCode == REVACKOK){
		if(IapMaster.pRevBuff[S05_CMD] == 0x05 && IapMaster.pRevBuff[S05_FILENUMBER] == pFile->number){
			if(IapMaster.pRevBuff[S05_FILEACKFLAG] == S05_REVOK){
				currentLen.Uint8[0] = IapMaster.pRevBuff[S05_CONTENTLEN_L];
				currentLen.Uint8[1] = IapMaster.pRevBuff[S05_CONTENTLEN_H];
				pFile->len = currentLen.Uint16;
				DealWithRevFile(pFile,&IapMaster.pRevBuff[S05_FILECONTENT]);
				return 0;
			}
			else if(IapMaster.pRevBuff[S05_FILEACKFLAG] == S05_FINENED){
				currentLen.Uint8[0] = IapMaster.pRevBuff[S05_CONTENTLEN_L];
				currentLen.Uint8[1] = IapMaster.pRevBuff[S05_CONTENTLEN_H];
				pFile->fileFlag = 1;
				pFile->len = currentLen.Uint16;
				DealWithRevFile(pFile,&IapMaster.pRevBuff[S05_FILECONTENT]);
				return 1;
			}
			else{
				return 3;
			}
		}
		else{
			return 3;
		}
	}
	else if(resultCode == ISCONNECTFAIL){
	 return 3;
	}
	else{
		return 2;
	}
}

/**
  *@brief  主机发送文件给从机
  *@param  pFile 文件结构体
  *@retval None
  */
void SendFileToHmiBeginCmd(struct FileOperate *pFile)
{
	Uint8To32 fileSize;
	fileSize.Uint32 = pFile->fileSize;
	SendIapBuffMaster[M06_CMD] = 0x06;
	SendIapBuffMaster[M06_FILENUMBER] = pFile->number;
	SendIapBuffMaster[M06_FILELENGTH_LL] = fileSize.Uint8[0];
	SendIapBuffMaster[M06_FILELENGTH_LH] = fileSize.Uint8[1];
	SendIapBuffMaster[M06_FILELENGTH_HL] = fileSize.Uint8[2];
	SendIapBuffMaster[M06_FILELENGTH_HH] = fileSize.Uint8[3];
	SendIapBuffMaster[M06_FILELENGTH] = 0;
	SendIapBuffMaster[M06_INFOBYTESIZE_L] = 0x09;
	SendIapBuffMaster[M06_INFOBYTESIZE_H] = 0x00;
	UserDataSendFill(&IapMaster,SendIapBuffMaster);
}

/**
  *@brief  主机请求从机发送文件等待回复
  *@param  pFile 文件结构体
  *@retval 0 成功 1 等待中 2 失败
  */
uint8_t WaitFileToHmiBeginAck(struct FileOperate *pFile)
{
	UserErrorStatus resultCode;
	resultCode = GetMasterUserInfoStatus(&IapMaster);
	if(resultCode == REVACKOK){
		if(IapMaster.pRevBuff[S06_CMD] == 0x06 &&\
			IapMaster.pRevBuff[S06_FILENUMBER] == pFile->number &&\
			IapMaster.pRevBuff[S06_ACKFLAG] == S06_REVOK)
		{
			return 0;
		}
		else{
			return 2;
		}
	}
	else if(resultCode == ISCONNECTFAIL){
	 return 2;
	}
	else{
		return 1;
	}
}

/**
  *@brief  主机发送文件内容给从机
  *@param  pFile 文件结构体
  *@retval None
  */
void SendFileToHmiConnectCmd(struct FileOperate *pFile)
{
	Uint8To32 offset;
	Uint8To16 size,allLen;
	GetFileConnent(pFile,&SendIapBuffMaster[M07_FILECONTENT]);
	offset.Uint32 = pFile->offset;
	SendIapBuffMaster[M07_CMD] = 0x07;
	SendIapBuffMaster[M07_FILENUMBER] = pFile->number;
	SendIapBuffMaster[M07_FILEFLAG] = pFile->fileFlag;
	SendIapBuffMaster[M07_FILEOFFSET_LL] = offset.Uint8[0];
	SendIapBuffMaster[M07_FILEOFFSET_LH] = offset.Uint8[1];
	SendIapBuffMaster[M07_FILEOFFSET_HL] = offset.Uint8[2];
	SendIapBuffMaster[M07_FILEOFFSET_HH] = offset.Uint8[3];
	size.Uint16 = pFile->len;
	SendIapBuffMaster[M07_CONTENTLEN_L] = size.Uint8[0];
	SendIapBuffMaster[M07_CONTENTLEN_H] = size.Uint8[1];
	allLen.Uint16 = size.Uint16 + 11;
	SendIapBuffMaster[M07_INFOBYTESIZE_L] = allLen.Uint8[0];
	SendIapBuffMaster[M07_INFOBYTESIZE_H] = allLen.Uint8[1];
	UserDataSendFill(&IapMaster,SendIapBuffMaster);
}

/**
  *@brief  主机发送文件内容给从机等待回复
  *@param  pFile 文件结构体
  *@retval 0 成功 1文件结束  2 等待中 3 失败
  */
uint8_t WaitFileToHmiConnectAck(struct FileOperate *pFile)
{
	UserErrorStatus resultCode;
	resultCode = GetMasterUserInfoStatus(&IapMaster);
	if(resultCode == REVACKOK){
		if(IapMaster.pRevBuff[S07_CMD] == 0x07 && IapMaster.pRevBuff[S07_FILENUMBER] == pFile->number){
			if(IapMaster.pRevBuff[S07_FILEACKFLAG] == S07_REVOK){
				return 0;
			}
			else if(IapMaster.pRevBuff[S07_FILEACKFLAG] == S07_REVEND){
				return 1;
			}
			else{
				return 3;
			}
		}
		else{
			return 3;
		}
	}
	else if(resultCode == ISCONNECTFAIL){
	 return 3;
	}
	else{
		return 2;
	}
}

/**
  *@brief  Iapmain初始化
  *@param  None
  *@retval None
  */
void IapHmiInit(void)
{
	GetIntervalTick = GetTimer1IntervalTick;
	GetCurrentTick = GetTimer1Tick;
	IapTimCheck.overFun = IapTimOverFun;
	SetIapTimCheck(20000,TC_DISENABLE,TO_NONE,"upData");
	AddUserInfo(&IapMaster);
}

/**
  *@brief  主函数
  *@param  None
  *@retval None
  */
void IapHmiScan(void)
{
	IapOutTimCheck();
}

/**
  *@brief  保存面板程序
  *@param  None
  *@retval 0 执行完成 1正在执行
  */
uint8_t SaveHmiProgram(uint8_t *flag)
{
	uint8_t result;
	if(*flag == 0){//启动保存
		SetIapTimCheck(20000,TC_ENABLE,TOHMIFINISH,"SavePro_flag0");
		*flag = 1;
	}
	if(*flag == 1){//发送保存请求
		SendModfiyOneType1RegCmd(&IapReg.updata,1);
		*flag = 2;
	}
	else if(*flag == 2){
		result = WaitModfiyRegAck();
		if(result == 0){//接收成功
			SetIapTimCheck(20000,TC_ENABLE,TOHMIFINISH,"SavePro_flag2");
			*flag = 3;
		}
		else if(result == 2){
			*flag = 1;/* 接收失败，重新接收 */
		}
	}
	/* 请求接收面板文件 */
	if(*flag == 3){
		saveFile.number = 1;
		SendRequestFileBeginCmd(&saveFile);
		*flag = 4;
	}
	else if(*flag == 4){
		result = WaitRequestFileBeginAck(&saveFile);
		if(result == 0){//接收成功
			result = f_open(&hmiFile,(TCHAR *)hmiName, FA_CREATE_ALWAYS | FA_WRITE);
			DebugPrintf("SaveFileOpen%d\r\n",result);
			if(result == FR_OK){
				SetIapTimCheck(40000,TC_ENABLE,TO_SAVEFILEFALSE,"SavePro_flag4");
				saveFile.number = 1;
				saveFile.offset = 0;
				saveFile.len = 512;
				*flag = 5;
			}
			else{
				*flag = 3;
			}
		}
		else if(result == 2){
			*flag = 3;
		}
	}
	/* 开始文件传送 */
	if(*flag == 5){
		SendRequestFileConnectCmd(&saveFile);
		*flag = 6;
	}
	else if(*flag == 6){
		result = WaitRequestFileConnectAck(&saveFile);
		if(result == 0){//接收成功
			SetIapTimCheck(40000,TC_ENABLE,TO_SAVEFILEFALSE,"SavePro_flag6");
			saveFile.offset += saveFile.len;
			saveFile.len = 512;
			*flag = 5;
		}
		else if(result == 1){
			SetIapTimCheck(5000,TC_DISENABLE,TO_NONE," ");
			*flag = 7;
		}
		else if(result == 3){
			*flag = 5;
		}
	}
	if(*flag == 7){
		return 0;
	}
	else{
		return 1;
	}
}

/**
  *@brief  擦除面板程序
  *@param  None
  *@retval 0 执行完成 1正在执行
  */
uint8_t HmiProgramErase(uint8_t *flag)
{
	uint8_t result;
	if(*flag == 0){
		SetIapTimCheck(20000,TC_ENABLE,TOHMIFINISH,"EraseProflag0");
		*flag = 1;
	}
	if(*flag == 1){//发送擦除命令
		SendModfiyOneType1RegCmd(&IapReg.updata,2);
		*flag = 2;
	}
	else if(*flag == 2){
		result = WaitModfiyRegAck();
		if(result == 0){//接收成功
			SetIapTimCheck(20000,TC_ENABLE,TO_ERASEFALSE,"EraseProflag2");
			*flag = 3;
		}
		else if(result == 2){
			DebugPrintf("等待应答失败\r\n");
			*flag = 1;
		}
	}
	else if(*flag == 3){
		DebugPrintf("等待面板擦除完成\r\n");
		SendSeeOneType1RegCmd(&IapReg.updata);
		*flag = 4;
	}
	else if(*flag == 4){
		result = WaitSeeOneType1RegAck(&IapReg.updata);
		if(result == 0){//接收成功
			if(IapReg.updata.value == 3){//擦除成功
				DebugPrintf("面板擦除完成\r\n");
				SetIapTimCheck(20000,TC_DISENABLE,TO_ERASEFALSE,"");
				*flag = 5;
			}
			else if(IapReg.updata.value == 4){//擦除失败
				SetIapTimCheck(0,TC_ENABLE,TO_ERASEFALSE,"EraseProflag6");
				DebugPrintf("面板擦除失败\r\n");
				*flag = 6;
			}	
		}
	}
	if(*flag == 5){
		return 0;
	}
	else{
		return 1;
	}
}

/**
  *@brief  写面板程序
  *@param  flag 函数内部使用
  *@retval 0 执行完成 1正在执行
  */
uint8_t WriteHmiProgram(uint8_t *flag)
{
	uint8_t result;
	/* 发送升级命令*/
	if(*flag == 0){
		SetIapTimCheck(20000,TC_ENABLE,TOHMIFINISH,"WriteProflag0");
		*flag = 1;
	}
	if(*flag == 1){
		SendModfiyOneType1RegCmd(&IapReg.updata,5);
		*flag = 2;
	}
	else if(*flag == 2){
		result = WaitModfiyRegAck();
		if(result == 0){//接收成功
			SetIapTimCheck(20000,TC_ENABLE,TOHMIFINISH,"WriteProflag2");
			*flag = 3;
		}
		else if(result == 2){
			*flag = 1;
		}
	}
	/* 开始向面板发送文件信息 */
	if(*flag == 3){
		saveFile.fileSize = GetHmifileSize();
		saveFile.number = 2;
		SendFileToHmiBeginCmd(&saveFile);
		*flag = 4;
	}
	else if(*flag == 4){
		result = WaitFileToHmiBeginAck(&saveFile);
		if(result == 0){//接收成功
			SetIapTimCheck(20000,TC_ENABLE,TO_WRITEPROFLASE,"WriteProflag4");
			*flag = 5;
		}
		else if(result == 2){
			*flag = 3;
		}
	}
	/* 开始向面板发送文件内容 */
	if(*flag == 5){
		saveFile.offset = 0;
		saveFile.fileFlag = 0;
		saveFile.len = 512;
		*flag = 6;
	}
	if(*flag == 6){
		SendFileToHmiConnectCmd(&saveFile);
		*flag = 7;
	}
	else if(*flag == 7){
		result = WaitFileToHmiConnectAck(&saveFile);
		if(result == 0){//接收成功
			SetIapTimCheck(20000,TC_ENABLE,TO_WRITEPROFLASE,"WriteProflag6");
			saveFile.offset += saveFile.len;
			DebugPrintf("offset:0x%08X\r\n",saveFile.offset);
			*flag = 6;
		}
		else if(result == 1){
			SetIapTimCheck(20000,TC_DISENABLE,TO_WRITEPROFLASE,"");
			DebugPrintf("写面板文件结束\r\n");
			*flag = 8;
		}
		else if(result == 3){
			*flag = 6;
		}
	}
	if(*flag == 8){
		return 0;
	}
	else{
		return 1;
	}
}

/**
  *@brief  更新面板程序
  *@param  flag 函数内部使用
  *@retval 0 执行完成 1正在执行
  */
uint8_t HmiProgramUpadta(uint8_t *flag)
{
	uint8_t tempVaule = 0xFF;
	static uint8_t subStepFlag;
	if(*flag == 0){
		subStepFlag = 0;
		*flag = 1;
	}
	if(*flag == 1){//保存程序
		if(SaveHmiProgram(&subStepFlag) == 0){
			subStepFlag = 0;
			*flag = 2;
		}
	}
	if(*flag == 2){//擦除程序
		tempVaule = IS_ROLLBACK_FLAG;
		FM25VxxWriteData(PRO_STATE_BEGINADDR + PRO_HMIBOARD_STATE,NULL,&tempVaule,1);
		if(HmiProgramErase(&subStepFlag) == 0){
			subStepFlag = 0;
			*flag = 3;
		}
	}
	if(*flag == 3){
		if(WriteHmiProgram(&subStepFlag) == 0){
			subStepFlag = 0;
			*flag = 4;
		}
	}
	if(*flag == 4){
		return 0;
	}
	else{
		return 1;
	}
}
/**
  *@brief  将文件迁移到sdram
  *@param  flag 函数内部使用
  *@retval 0 成功 1失败
  */
uint8_t GetprogramTofile(void)
{
	uint32_t i;
	uint8_t res;
	uint32_t realLen,tempSize = 0,fileSize,saveAddr;
	saveAddr = HMIPROGRAMSTARTADDR + 4;
	res = f_open(&hmiFile,(TCHAR *)hmiName, FA_READ);
	if(res != FR_OK){
		return 1;
	}
	fileSize = f_size(&hmiFile);
	DebugPrintf("文件大小0x%08X\r\n",fileSize);
	*(uint32_t *)(HMIPROGRAMSTARTADDR) = fileSize;
	for(tempSize = 0; tempSize < fileSize; tempSize += FILEBUFFSIZE){
		res = f_read(&hmiFile,FileBuff,FILEBUFFSIZE,(UINT*)&realLen);
		if(res != FR_OK){
			DebugPrintf("读文件出错\r\n");
			return 1;
		}
		for(i = 0; i < realLen; i++){
			*(uint8_t *)(saveAddr + tempSize + i) = FileBuff[i];
		}
	}
	DebugPrintf("迁移文件到SDRAM成功\r\n");
	f_close(&hmiFile);
	return 0;
}

/**
  *@brief  回退
  *@param  flag 函数内部使用
  *@retval 0 成功 1失败
  */
uint8_t HmiProgramBack(uint8_t *flag)
{
	static uint8_t subStepFlag;
	if(*flag == 0){
		subStepFlag = 0;
		*flag = 1;
	}
	if(*flag == 1){
		if(HmiProgramErase(&subStepFlag) == 0){
			subStepFlag = 0;
			*flag = 2;
		}
	}
	if(*flag == 2){
		if(GetprogramTofile() == 0){
			subStepFlag = 0;
			*flag = 3;
		}
		else{
			return 1;
		}
	}
	if(*flag == 3){
		if(WriteHmiProgram(&subStepFlag) == 0){
			subStepFlag = 0;
			*flag = 4;
		}
	}
	if(*flag == 4){
		return 0;
	}
	else{
		return 1;
	}
}

/**
  *@brief  给面板发送跳转命令
  *@param  None
  *@retval None
  */
void HmiProgramJump(void)
{
	SendModfiyOneType1RegCmd(&IapReg.iapJump,1);
}

/**
  *@brief  测试文件系统使用
  *@param  None
  *@retval None
  */
void testCreatFile(void)
{
	uint8_t testbuff[4] = {1,2,3,4};
	uint8_t res;
	uint32_t realWrite;
	res = f_open(&hmiFile,(TCHAR *)hmiName, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(res != FR_OK){
		return ;
	}
	res = f_write(&hmiFile,testbuff,4,(UINT *)&realWrite);
	if(res == FR_OK){
		f_close(&hmiFile);
	}
}

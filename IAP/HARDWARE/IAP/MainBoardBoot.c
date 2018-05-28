/**
  * @file   mainBoardBoot.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/04/12
  * @brief  主板boot
  */
#include "systemconfig.h"
#include "MainBoardBoot.h"
#include "MainFlashOperate.h"
#include "ff.h"
#include "IapControl.h"
#include "drv_fm25vxx.h"

static FIL mainfile;
static uint8_t *mainName="m.bin";
static uint8_t writefilebuff[4096];

/**
  *@brief  保存主板程序
  *@param  None
  *@retval 0 成功 1失败
  */
uint8_t SaveMainProgram(void)
{
	uint32_t res;
	uint32_t i;
	uint16_t reallen;
	uint32_t tempSize;
	res = f_open(&mainfile,(TCHAR *)mainName, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(res == FR_OK){
		for(i = MAINPRO_BEGINADDR; i < MAINPRO_ENDADDR; i += WRITEFILEBUFF){
			reallen = ReadProgram(i, writefilebuff,WRITEFILEBUFF);
			if(reallen){
				res = f_write(&mainfile,writefilebuff,reallen,(UINT *)&tempSize);
				if(res != FR_OK){
					f_close(&mainfile);
					DebugPrintf("WriteFile出错2\r\n");
					return 2;
				}
				if(tempSize != reallen){
					DebugPrintf("WriteFile出错3\r\n");
					f_close(&mainfile);	
					return 3;
				}
			}
			else{
				DebugPrintf("WriteFile出错4\r\n");
				return 4;
			}
		}
	}
	else{
		DebugPrintf("打开文件出错\r\n");
		return 5;
	}
	DebugPrintf("保存文件成功\r\n");
	f_close(&mainfile);
	return 0;
}

/**
  *@brief 更新主板程序
  *@param  None
  *@retval 0 成功 1失败
  */
uint8_t UpdataMainProgram(void)
{
	uint32_t i;
	uint32_t fileSize,res;
	uint8_t tempVaule = 0xFF;
	for(i = 0; i < 3; i++){
		if(SaveMainProgram() == 0){
			break;
		}
		if(i == 2){
			DebugPrintf("保存文件失败\r\n");
			return 1;
		}
	}
	for(i = 0; i < 3; i++){
		tempVaule = IS_ROLLBACK_FLAG;
		FM25VxxWriteData(PRO_STATE_BEGINADDR + PRO_MAINBOARD_STATE,NULL,&tempVaule,1);
		if(EreaseProgram() == 0){
			break;
		}
		if(i == 2){
			DebugPrintf("擦除失败\r\n");
			return 2;
		}
	}
	fileSize = *((uint32_t *)(MAIN_SDRAM_STARTADDR));
	res = WriteProgram(MAINPRO_BEGINADDR,\
		(uint8_t *)(MAIN_SDRAM_STARTADDR + 4),fileSize);
	if(res != 0){
		DebugPrintf("写程序出错\r\n");
		return 3;
	}
	DebugPrintf("更新程序成功\r\n");
	return 0;
}

/**
  *@brief  回退主板程序
  *@param  None
  *@retval 0 成功 1失败
  */
uint8_t MainProgramBack(void)
{
	uint32_t res;
	uint32_t i,k;
	uint32_t tempSize = 0,realLen;
	uint32_t fileSize;
	for(i = 0; i < 3; i++){
		res = f_open(&mainfile,(TCHAR *)mainName, FA_READ);
		if(res != FR_OK){
			f_close(&mainfile);
		}
		else{
			break;
		}
		if(i == 2){
			DebugPrintf("打开文件失败\r\n");
			return 1;
		}
	}
	for(i = 0; i < 3; i++){
		if(EreaseProgram() == 0){
			break;
		}
		if(i == 2){
			DebugPrintf("擦除失败\r\n");
			return 2;
		}
	}
	fileSize = f_size(&mainfile);
	for(k = MAINPRO_BEGINADDR; k < MAINPRO_ENDADDR; k += WRITEFILEBUFF){
		res = f_read(&mainfile,writefilebuff,WRITEFILEBUFF,(UINT*)&realLen);
		if(res == FR_OK){
			res = WriteProgram(k,writefilebuff,realLen);
			if(res != 0){
				DebugPrintf("写程序出错\r\n");
				return 3;
			}
			DebugPrintf("写地址_0x%08X,size:%d\r\n",k,realLen);
			tempSize += realLen;
			if(realLen < WRITEFILEBUFF && tempSize < fileSize){
				DebugPrintf("写程序出错,realLen有问题\r\n");
				return 4;
			}
			if(tempSize >= fileSize){
				break;
			}
		}
		else{
			DebugPrintf("读文件出错\r\n");
			return 5;
		}
	}
	DebugPrintf("回退成功\r\n");
	return 0;
}

void MtestCreatFile(void)
{
	uint8_t testbuff[4] = {1,2,3,4};
	uint8_t res;
	uint32_t realWrite;
	res = f_open(&mainfile,(TCHAR *)mainName, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(res != FR_OK){
		return ;
	}
	res = f_write(&mainfile,testbuff,4,(UINT *)&realWrite);
	if(res == FR_OK){
		f_close(&mainfile);
	}
}

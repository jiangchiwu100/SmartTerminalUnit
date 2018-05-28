/**
  * @file   IapControl.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/20
  * @brief  IapBoot跳转控制
  */
#include "systemconfig.h"
#include "IapControl.h"
#include "drv_fm25vxx.h"
#include "hmiboot.h"
#include "MainBoardBoot.h"
#include "MainFlashOperate.h"
#include "ff.h"

/* 逻辑磁盘工作区. */
static FATFS fs[_VOLUMES];
/* 工作步骤 */
enum IapControlStep controlStep;

/* 程序运行状态 */
static struct ProgramRunState pRunState;

/**
  *@brief  程序更新包检测
  *@param  proBeginAddr 存放程序包的首地址
  *@retval 0 成功 1失败
  */
uint8_t UpadaProgramCheck(uint32_t proBeginAddr)
{
	uint32_t appxaddr;
	appxaddr = *(volatile uint32_t *)proBeginAddr;
	DebugPrintf("地址检测: 0x%08X \r\n",appxaddr);
	if((appxaddr & 0x2FFE0000) == 0x20000000){
		return 0;
	}
	return 1;
}
void ModfiyPowerOnCount(void)
{
	uint8_t powerOnIs;
	uint8_t tempflag;
	
	tempflag = FM25VxxReadData(PRO_STATE_BEGINADDR + PRO_BOOTCOUNT, NULL, &powerOnIs, 1);
	if(tempflag == TRUE){
		powerOnIs ++;
		FM25VxxWriteData(PRO_STATE_BEGINADDR + PRO_BOOTCOUNT,NULL,&powerOnIs,1);
		DebugPrintf("开机次数: %d \r\n",powerOnIs);
	}
}
/**
  *@brief  读取程序状态标志
  *@param  None
  *@retval 0 成功 1失败
  */
uint8_t ReadProgramState(void)
{
	uint8_t tempreadbuff[PRO_STATE_ALLBYTE];
	uint8_t tempflag;
	tempflag = FM25VxxReadData(PRO_STATE_BEGINADDR, NULL,\
		tempreadbuff, PRO_STATE_ALLBYTE);//特意多读一次
	tempflag = FM25VxxReadData(PRO_STATE_BEGINADDR, NULL,\
		tempreadbuff, PRO_STATE_ALLBYTE);
	if(tempflag == TRUE){
		pRunState.firstPower = tempreadbuff[PRO_FIRSTPOWER];
		pRunState.hBoradState = tempreadbuff[PRO_HMIBOARD_STATE];
		pRunState.mBoradState = tempreadbuff[PRO_MAINBOARD_STATE];
		pRunState.bootCount = tempreadbuff[PRO_BOOTCOUNT];
		DebugPrintf("读取程序状态成功：");
		DebugPrintf("firstPower:0x%02X  ",pRunState.firstPower);
		DebugPrintf("hBoradState:0x%02X  ",pRunState.hBoradState);
		DebugPrintf("mBoradState:0x%02X  ",pRunState.mBoradState);
		DebugPrintf("bootCount:%d\r\n",pRunState.bootCount);
		return 0;
	}
	DebugPrintf("读取程序状态出错!\r\n");
	return 1;	
}

/**
  * @Description: 文件系统初始化
  * @param:  void
  * @return: void
  * @updata: 
  */
void FatfsInit(void)
{
	uint8_t res=0;
	res=f_mount(&fs[0],"0:",1);//挂载FLASH.
	if(res != 0)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		DebugPrintf("磁盘格式化\r\n");
		res = f_mkfs("0:",0,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res == 0)
		{
			f_setlabel((const TCHAR *)"1:A");	//设置Flash磁盘的名字
		}
		else{
			Delay_ms(1000);
		} 
	}
}

/**
  * @Description: 卸载文件系统
  * @param:  None
  * @return: None
  */
void FatfsUninstall(void)
{
	f_mount(NULL,"0:",1);
}

void IapControlInit(void)
{
	uint8_t i;
	for(i = 0; i < 10; i++){
		if(ReadProgramState() != 0){
			if(i >= 9){
				//读不到状态处理
			}
			Delay_ms(100);
		}
		else{
			//状态处理
			break;
		}
	}
	ModfiyPowerOnCount();
}

/**
  *@brief  IAP控制主函数
  *@param  None
  *@retval None
  */
void IapControlMain(void)
{
	static uint8_t hmistepFlag;
	static uint8_t jumpflag;
	if(controlStep == CON_INIT){
		hmistepFlag = 0;
		if(pRunState.firstPower != NOFIRSTPOWER){//第一次上电
			DebugPrintf("第一次上电\r\n");
			controlStep = CON_TOJUMP;
		}
		else{//先进行面板控制
			if(pRunState.hBoradState == HMIPRO_RUNTURE){//运行正常
				DebugPrintf("面板运行正常\r\n");
				controlStep = CON_HMIPRO_RUNOK;
			}
			else if(pRunState.hBoradState == HMIPRO_UPDATA){//更新
				hmistepFlag = 0;
				if(UpadaProgramCheck(HMIPROGRAMSIZEADDR + 4) == 0){
					if(pRunState.mBoradState == MAINPRO_UPDATA){
						if(UpadaProgramCheck(MAIN_SDRAM_STARTADDR + 4) == 0){
							DebugPrintf("面板开始升级\r\n");
							controlStep = CON_HMIPRO_UPDATA;
						}
						else{
							controlStep = CON_MAINPRO_UPDATAPROERROE;
						}
					}
					else{
						DebugPrintf("面板开始升级\r\n");
						controlStep = CON_HMIPRO_UPDATA;
					}
				}
				else{
					controlStep = CON_HMIPRO_UPDATAPROERROE;
				}
			}
			else if(pRunState.hBoradState == HMIPRO_SAVE){//保存
				DebugPrintf("保存面板程序\r\n");
				hmistepFlag = 0;
				controlStep = CON_HMIPRO_SAVE;
			}
			else{
				DebugPrintf("面板程序回退\r\n");
				hmistepFlag = 0;
				controlStep = CON_HMIPRO_BACK;
			}
		}
	}
	if(controlStep == CON_HMIPRO_FINISH){//主板控制
		if(pRunState.mBoradState == MAINPRO_RUNTURE){
			controlStep = CON_MAINPRO_RUNOK;
		}
		else if(pRunState.mBoradState == MAINPRO_UPDATA){
			if(UpadaProgramCheck(MAIN_SDRAM_STARTADDR + 4) == 0){
				controlStep = CON_MAINPRO_UPDATA;
			}
			else{
				controlStep = CON_MAINPRO_UPDATAPROERROE;
			}
		}
		else if(pRunState.mBoradState == MAINPRO_SAVE){
			controlStep = CON_MAINPRO_SAVE;
		}
		else{//主板回退
			controlStep = CON_MAINPRO_BACK;
		}
	}
	/* -------面板----------- */
	if(controlStep == CON_HMIPRO_RUNOK){//
		controlStep = CON_HMIPRO_FINISH;
	}
	if(controlStep == CON_HMIPRO_UPDATA){//面板更新
		if(HmiProgramUpadta(&hmistepFlag) == 0){//保存完成,1正在执行
			controlStep = CON_HMIPRO_FINISH;
		}
	}
	if(controlStep == CON_HMIPRO_SAVE){//面板保存
		if(SaveHmiProgram(&hmistepFlag) == 0){//保存完成
			controlStep = CON_HMIPRO_FINISH;
		}
	}

	if(controlStep == CON_HMIPRO_BACK){//面板回退
		if(HmiProgramBack(&hmistepFlag) == 0){//保存完成
			controlStep = CON_HMIPRO_FINISH;
		}	
	}
	/* -------主板----------- */
	if(controlStep == CON_MAINPRO_RUNOK){//主板运行正常
		controlStep = CON_TOJUMP;
	}
	if(controlStep == CON_MAINPRO_UPDATA){//主板更新
		if(UpdataMainProgram() == 0){//保存完成
			controlStep = CON_TOJUMP;
		}
		else{//更新失败,先面板回退
			controlStep = CON_MAINPRO_BACK;
		}
	}
	if(controlStep == CON_MAINPRO_SAVE){//主板保存
		if(SaveMainProgram() == 0){//保存完成
			controlStep = CON_TOJUMP;
		}
		else{
			controlStep = CON_TOJUMP;
		}
	}
	if(controlStep == CON_MAINPRO_BACK){//主板回退
		if(MainProgramBack() == 0){//保存完成
			controlStep = CON_TOJUMP;
		}
		else{
			controlStep = CON_TOJUMP;
		}
	}
	if(controlStep == CON_HMIPRO_UPDATAPROERROE){//检测到面板更新程序有错
		DebugPrintf("HMI升级包出错,请下载正确升级包");
		controlStep = CON_TOJUMP;
	}
	if(controlStep == CON_MAINPRO_UPDATAPROERROE){//检测到主板更新程序有错
		DebugPrintf("主板升级包出错,请下载正确升级包");
		controlStep = CON_TOJUMP;
	}
	/* -------跳转----------- */
	if(controlStep == CON_TOJUMP){//丢弃面板，直接跳转
		DebugPrintf("开始跳转\r\n");
		if(jumpflag ++ < 3){
			HmiProgramJump();
		}
		else{
			IapLoadApp(MAINPRO_BEGINADDR);
			while(1);
		}
		//NVIC_SystemReset();//重启
	}
	if(controlStep < CON_HMIPRO_FINISH){
		IapOutTimCheck();
	}
}

void SetControlStep(enum IapControlStep step)
{
	controlStep = step;
}

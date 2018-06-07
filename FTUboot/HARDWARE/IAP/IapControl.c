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
	if((appxaddr & 0x2FFC0000) == 0x20000000 ||\
		(appxaddr & 0x1FFE0000) == 0x10000000){
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
	FM25VxxReadData(PRO_STATE_BEGINADDR, NULL,tempreadbuff, PRO_STATE_ALLBYTE);
	Delay_ms(50);
	tempflag = FM25VxxReadData(PRO_STATE_BEGINADDR, NULL,\
		tempreadbuff, PRO_STATE_ALLBYTE);
	if(tempflag == TRUE){
		pRunState.firstPower = tempreadbuff[PRO_FIRSTPOWER];
		pRunState.mBoradState = tempreadbuff[PRO_MAINBOARD_STATE];
		pRunState.bootCount = tempreadbuff[PRO_BOOTCOUNT];
		DebugPrintf("读取程序状态成功：");
		DebugPrintf("firstPower:0x%02X  ",pRunState.firstPower);
		DebugPrintf("mBoradState:0x%02X  ",pRunState.mBoradState);
		DebugPrintf("bootCount:%d\r\n",pRunState.bootCount);
		return 0;
	}
	DebugPrintf("读取程序状态出错!\r\n");
	return 1;	
}

/**
  *@brief 获取文件系统信息
  *@param  None
  *@retval None
  */
void GetFatfsInfo(void)
{
	DWORD fre_clust, fre_sect, tot_sect;
	FATFS *fs;
	uint8_t res = 0;
	res = f_getfree("0:", &fre_clust, &fs);
	if(res == 0){
		tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;
		DebugPrintf("%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", tot_sect / 2, fre_sect / 2);
	}
}

/**
  * @Description: IAP初始化
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
		if(res == 0){
			f_setlabel((const TCHAR *)"1:A");	//设置Flash磁盘的名字为
		}
	}
	GetFatfsInfo();
}

void FatfsReFormat(void)
{
	uint8_t res = 0;
	DebugPrintf("磁盘重新格式化\r\n");
	res = f_mkfs("0:",0,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
	if(res == 0){
		f_setlabel((const TCHAR *)"1:A");	//设置Flash磁盘的名字为
	}
	Delay_ms(200);
	GetFatfsInfo();
}
/**
  *@brief 文件系统卸载
  *@param  None
  *@retval None
  */
void FatfsUninstall(void)
{
	f_mount(0,"0:",1);//挂载FLASH.
}

/**
  *@brief Iap控制初始化
  *@param  None
  *@retval None
  */
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
	if(pRunState.firstPower != NOFIRSTPOWER){//第一次上电
		DebugPrintf("第一次上电\r\n");
		IapLoadApp(MAINPRO_BEGINADDR);
	}
	else if(pRunState.mBoradState == MAINPRO_RUNTURE){
		DebugPrintf("运行正常\r\n");
		IapLoadApp(MAINPRO_BEGINADDR);
	}
	else if(pRunState.mBoradState == MAINPRO_UPDATA){
		DebugPrintf("程序有更新\r\n");
		if(UpadaProgramCheck(MAIN_SDRAM_STARTADDR + 4) == 0){
			uint8_t tempVaule = IS_RUNOK_FLAG;
			FM25VxxWriteData(PRO_STATE_BEGINADDR + PRO_MAINBOARD_STATE,NULL,&tempVaule,1);
			if(UpdataMainProgram() == 0){//保存完成
				IapLoadApp(MAINPRO_BEGINADDR);
			}
		}
		else{
			DebugPrintf("程序有错,直接跳转\r\n");
			IapLoadApp(MAINPRO_BEGINADDR);
		}
	}
	else{//主板回退
		if(MainProgramBack() == 0){//保存完成
			IapLoadApp(MAINPRO_BEGINADDR);
		}
	}
	/* 程序出现错误,重启,直接跳转 */
	uint8_t tempVaule = !IS_FIRSTRUN_FLAG;
	FM25VxxWriteData(PRO_STATE_BEGINADDR + PRO_FIRSTPOWER,NULL,&tempVaule,1);
	Delay_ms(500);
	NVIC_SystemReset();//重启
}

void VersionInfo(void)
{
	DebugPrintf("\r\n= = = = = = = = = = = = = = = = = = = = = = = =\r\n");
	DebugPrintf("FtuBootVersion:%s\r\n",FTUBOOT_VERSIONCODE);
}

/* END */

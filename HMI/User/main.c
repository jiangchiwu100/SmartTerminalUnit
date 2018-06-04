
#include "stm32f10x.h"
#include "systemconfig.h"
#include "uartDriver.h"
#include "ledDriver.h"
#include "lkdGuiUse.h"
#include "dlt634_HMImaster_disk.h"
#include "hmi101.h"
#include "switchDriver.h"
#include "keyDriver.h"
#include "GuiDisplay.h"
#include "inoutUser.h"

static void IwdgInit(void);
static void IwdgReloadCounter(void);

int main(void)
{
	CLOSE_ALL_INT();
	SystemconfigInit();
	LedDriverInit();
	KeyDriverInit();
	SwitchDriverInit();
	uartInit();
	LcdInit();
	ZiKuInit();
	OPEN_ALL_INT();
	GUIStartInterface();
	DLT634_HMI_MASTER_INIT(0);
	IwdgInit();
	while(1)                            
	{
		ScanSwitchStatus();
		ScanKeyStatus();
		Hmi101Main();
		InOutUserMain();
		IwdgReloadCounter();
	}
}

/**
  *@brief 看门狗初始化
  *@param  None
  *@retval None
  */
static void IwdgInit(void)
{
	uint16_t reload; /* 加载值 ,不能大于0x0FFF*/
	
	/* 
	 *reload = (LSi / ( 4*2^IWDG_Prescaler )) * time
	 * LSi 看门狗时钟 40KHZ
	 * reload 加载值
	 * IWDG_Prescaler 分频
	 * time 设定时间 s
	*/
	reload = (uint16_t)1250*2.5;//2.5S 
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(reload);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

/**
  *@brief 喂狗
  *@param  None
  *@retval None
  */
static void IwdgReloadCounter(void)
{
	IWDG_ReloadCounter(); /* 喂狗 */
}
/* END */

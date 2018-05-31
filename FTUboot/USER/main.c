
#include "systemconfig.h"
#include "led.h"
#include "w25qxx.h"
#include "ff.h"
#include "drv_fm25vxx.h"
#include "sdram.h"
#include "uartDriver.h"
#include "IapControl.h"
#include "MainFlashOperate.h"

int main(void)
{
  HAL_Init(); 
  Stm32_Clock_Init(360,25,2,8);//…Ë÷√ ±÷”,180Mhz 
	SystemconfigInit();
	uartInit();
  LED_Init();  
	SDRAM_Init();
	W25QXX_Init();
	FM25VxxInit();
	VersionInfo();
	FatfsInit();
	Delay_ms(500);
	IapControlInit();

	while(1)
	{
		IapControlMain();
	}	
}


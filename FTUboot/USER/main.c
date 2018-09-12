
#include "systemconfig.h"
#include "led.h"
#include "w25qxx.h"
#include "ff.h"
#include "drv_fm25vxx.h"
#include "sdram.h"
#include "uartDriver.h"
#include "IapControl.h"
#include "MainFlashOperate.h"


 extern void tftp_demo(void);
int main(void)
{
    HAL_Init(); 
    Stm32_Clock_Init(360,25,2,8);//…Ë÷√ ±÷”,180Mhz 
    SystemconfigInit();
    uartInit();
    LED_Init();  
    SDRAM_Init();
    W25QXX_Init();
    //FM25VxxInit();
    VersionInfo();
    FatfsInit();
    FRAM_Init();
    Delay_ms(500);

    tftp_demo();

    IapControlInit();

	while(1)
	{
		IapControlMain();
	}	
}


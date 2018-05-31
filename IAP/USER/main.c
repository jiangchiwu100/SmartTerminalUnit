#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "string.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ff.h"
#include "string.h"
#include "IAP.h"
#include "drv_fm25vxx.h"
#include "commun.h"
#include "sdram.h"
#include "iwdg.h" 

int main(void)
{
	uint8_t count = 1;
	uint8_t returnFlag;
	HAL_Init();
	Stm32_Clock_Init(360,25,2,8);
	LED_Init();
	delay_init(180);
	uart_init(115200);
	uart3_init(115200);
	SDRAM_Init();
	W25QXX_Init();//初始化W25Q256  
	FM25VxxInit();
	FATFSInit();
	delay_ms(1000);
	PowerOnCount();/* 开机计数 */
	while(1)
	{
		for(count = 0;count < 3; count ++){
			Iapmain();
			#ifdef DEBUG_
			printf("第 %d 次跳转失败\r\n",count);
			#endif 
		}
		returnFlag = SetFlagIsFirstRun();
		if(returnFlag != 0){//修改失败
			NVIC_SystemReset();//重启
		}
	}
}


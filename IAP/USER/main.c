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

extern uint8_t rxfinish;

int main(void)
{
	uint8_t count = 1;
	uint8_t returnFlag;
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz 
    //IWDG_Init(IWDG_PRESCALER_128, 1000);
    LED_Init();                     //初始化LED  
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化UART5 串口打印
	uart3_init(115200);
	//uart2_init(115200);
	SDRAM_Init();
	W25QXX_Init();				    //初始化W25Q256  
	FM25VxxInit();
	FATFSInit();//挂载文件系统

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

//#ifdef DEBUG_
//printf("第 %d 次跳转失败\r\n",count);
//#endif
//if(count++ >= 3){
//	{//------------------调试代码start
//		#ifdef DEBUG_
//		printf("启动异常!\r\n");
//		printf("进入调试模式\r\n");
//		#endif
//		testIAP();
//		Iapmain();
//		#ifdef DEBUG_
//		printf("死机!\r\n");
//		#endif
//		while(1){						
//		}
//	}//------------------调试代码end		
//}


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
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz 
    //IWDG_Init(IWDG_PRESCALER_128, 1000);
    LED_Init();                     //��ʼ��LED  
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��UART5 ���ڴ�ӡ
	uart3_init(115200);
	//uart2_init(115200);
	SDRAM_Init();
	W25QXX_Init();				    //��ʼ��W25Q256  
	FM25VxxInit();
	FATFSInit();//�����ļ�ϵͳ

	delay_ms(1000);
	PowerOnCount();/* �������� */
	while(1)
	{
		for(count = 0;count < 3; count ++){
			Iapmain();
			#ifdef DEBUG_
			printf("�� %d ����תʧ��\r\n",count);
			#endif 
		}
		returnFlag = SetFlagIsFirstRun();
		if(returnFlag != 0){//�޸�ʧ��
			NVIC_SystemReset();//����
		}
	}
}

//#ifdef DEBUG_
//printf("�� %d ����תʧ��\r\n",count);
//#endif
//if(count++ >= 3){
//	{//------------------���Դ���start
//		#ifdef DEBUG_
//		printf("�����쳣!\r\n");
//		printf("�������ģʽ\r\n");
//		#endif
//		testIAP();
//		Iapmain();
//		#ifdef DEBUG_
//		printf("����!\r\n");
//		#endif
//		while(1){						
//		}
//	}//------------------���Դ���end		
//}

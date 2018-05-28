#include "commun.h"
#include "drv_fm25vxx.h"
#include "IAP.h"

enum communState CommunState;
uint8_t USART3_Rxbuffer[256];
uint8_t USART3_RxBuffsize = 0;
uint32_t USART3_RxOver = 0;
uint32_t USART3_RxOverFlag = 0;
uint8_t USART3_Rxfinish = 0;
uint32_t MaxWait =1000;

static uint16_t CRC16_Check(uint8_t *Pushdata,uint8_t length)  
{	
   uint16_t Reg_CRC=0xffff;  
   uint8_t Temp_reg=0x00;  
   uint8_t i,j;   
   for( i = 0; i<length; i ++){  
     Reg_CRC^= *Pushdata++;  
     for (j = 0; j<8; j++){       
       if(Reg_CRC & 0x0001)   
			 Reg_CRC=Reg_CRC>>1^0xA001;  
		 else  
		    Reg_CRC >>=1;  
     }    
  }  
   Temp_reg=Reg_CRC>>8;  
   return (Reg_CRC<<8|Temp_reg); 
}
void sendData(uint8_t type,uint8_t addr,uint8_t len,uint8_t *pvalue)
{
	uint8_t sendbuff[16];
	uint8_t i;
	uint16_t tempcrc;
	sendbuff[0] = 0x01;
	sendbuff[1] = type;
	sendbuff[2] = addr;
	sendbuff[3] = len;
	if(sendbuff[1] == 0x03){
		tempcrc = CRC16_Check(sendbuff,4);
		sendbuff[4] = (uint8_t)(tempcrc>>8);
		sendbuff[5] = (uint8_t)(tempcrc);
		for(i=0;i<6;i++){
			while((USART3->SR&0X40)==0);//循环发送,直到发送完毕 
			USART3->DR = sendbuff[i];
		}
	}
	else{
		for(i=0;i<len;i++){
			sendbuff[i+4] = pvalue[i];
		}
		tempcrc = CRC16_Check(sendbuff,len+4);
		sendbuff[len+4] = (uint8_t)(tempcrc>>8);
		sendbuff[len+5] = (uint8_t)(tempcrc);
		for(i=0;i<len+6;i++){
			while((USART3->SR&0X40)==0);//循环发送,直到发送完毕 
			USART3->DR = sendbuff[i];
		}
	}
	USART3_RxBuffsize = 0;
}

void dealWithRxData(void)
{
	uint16_t temp_check;
	uint16_t rx_check;
	rx_check = (uint16_t)((USART3_Rxbuffer[USART3_RxBuffsize-2]<<8)|USART3_Rxbuffer[USART3_RxBuffsize-1]);
	temp_check = CRC16_Check(USART3_Rxbuffer,USART3_RxBuffsize-2);
	if(rx_check == temp_check){
		switch(CommunState){
			case WaitDebugBack:
				if((USART3_Rxbuffer[2] == 0x01) && (USART3_Rxbuffer[4] == 0x01)){
					#ifdef DEBUG_
					printf("进入调试模式\r\n");
					#endif
					CommunState = IsSendCurrentState;
				}
				else{
					CommunState = IsNoUse;
				}
				break;
			case WaitSendStateBack:
				if(USART3_Rxbuffer[2] == 0x02){
					CommunState = IsModifyState;
				}
				break;
			case WaitModfiyBack:
				#ifdef DEBUG_
				printf("修改写入\r\n");
				#endif
				if((USART3_Rxbuffer[2] == 0x02) && (USART3_Rxbuffer[3] == 0x02)){
					FM25VxxWriteData(ADDR_FRAM_UPDATE, NULL, &USART3_Rxbuffer[4], 2);
					CommunState = IsNoUse;
				}
				else{
					CommunState = IsNoUse;
				}
				break;
			default: 
				CommunState = IsNoUse;
				break;
		}		
	}
	else{
		CommunState = IsNoUse;
	}
}
void communInit(void)
{
	CommunState = IsRequestDebug;
}
enum communState CommunicationMian(void)
{
	uint8_t tempValue[2]={0};
	
	switch(CommunState){
		case IsRequestDebug:
			#ifdef DEBUG_
			printf("请求调试\r\n");
			#endif
			sendData(0x03,0x01,0x01,tempValue);
			USART3_RxOver = 0;
			USART3_RxOverFlag = 1;
			MaxWait = 1000;
			CommunState = WaitDebugBack;
			break;
		case IsSendCurrentState:
			#ifdef DEBUG_
			printf("发送当前值\r\n");
			#endif
			FM25VxxReadData(ADDR_FRAM_UPDATE, NULL,tempValue, 2);
			sendData(0x10,0x02,0x02,tempValue);
			USART3_RxOver = 0;
			USART3_RxOverFlag = 1;
			MaxWait = 10000;
			CommunState = WaitSendStateBack;
			break;
		case IsModifyState:	
			#ifdef DEBUG_
			printf("获取修改值\r\n");
			#endif
			sendData(0x03,0x02,0x02,tempValue);
			USART3_RxOver = 0;
			USART3_RxOverFlag = 1;
			MaxWait = 1000*60;	
			CommunState = WaitModfiyBack;
			break;
		default:break;
	}
	if(USART3_Rxfinish == 1){
		USART3_Rxfinish = 0;
		USART3_RxOverFlag = 0;
		dealWithRxData();
	}
	else if(USART3_RxOver>MaxWait){
		#ifdef DEBUG_
		printf("等待超时:%d\r\n",CommunState);
		#endif
		USART3_RxOverFlag = 0;
		CommunState = IsNoUse;
	}
	return CommunState ;
}

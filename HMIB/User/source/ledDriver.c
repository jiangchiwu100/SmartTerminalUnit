/**
  * @file   ledDriver.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   –≈∫≈µ∆”≤º˛≈‰÷√
  */

#include "ledDriver.h"

/**
  *@brief ledGpio≥ı ºªØ
  *@param  None
  *@retval None
  */
void LedGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	/* LED0 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED0;
	GPIO_Init(PORT_LED0, &GPIO_InitStructure);
	/* LED1 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED1;
	GPIO_Init(PORT_LED1, &GPIO_InitStructure);
	/* LED2 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED2;
	GPIO_Init(PORT_LED2, &GPIO_InitStructure);
	/* LED3 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED3;
	GPIO_Init(PORT_LED3, &GPIO_InitStructure);
	/* LED4 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED4;
	GPIO_Init(PORT_LED4, &GPIO_InitStructure);
	/* LED5 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED5;
	GPIO_Init(PORT_LED5, &GPIO_InitStructure);
	/* LED6 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED6;
	GPIO_Init(PORT_LED6, &GPIO_InitStructure);
	/* LED7 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED7;
	GPIO_Init(PORT_LED7, &GPIO_InitStructure);
	/* LED8 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED8;
	GPIO_Init(PORT_LED8, &GPIO_InitStructure);
	/* LED9 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED9;
	GPIO_Init(PORT_LED9, &GPIO_InitStructure);
	/* LED10 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED10;
	GPIO_Init(PORT_LED10, &GPIO_InitStructure);
	/* LED11 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED11;
	GPIO_Init(PORT_LED11, &GPIO_InitStructure);
	/* LED12 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED12;
	GPIO_Init(PORT_LED12, &GPIO_InitStructure);
	/* LED13 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED13;
	GPIO_Init(PORT_LED13, &GPIO_InitStructure);
	/* LED14 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED14;
	GPIO_Init(PORT_LED14, &GPIO_InitStructure);
	/* LED15 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED15;
	GPIO_Init(PORT_LED15, &GPIO_InitStructure);
	/* LED16 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED16;
	GPIO_Init(PORT_LED16, &GPIO_InitStructure);
	/* LED17 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED17;
	GPIO_Init(PORT_LED17, &GPIO_InitStructure);
	/* LED18 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED18;
	GPIO_Init(PORT_LED18, &GPIO_InitStructure);
	/* LED19 */
	GPIO_InitStructure.GPIO_Pin = PIN_LED19;
	GPIO_Init(PORT_LED19, &GPIO_InitStructure);
	for(uint8_t i = 0; i < LED_ALLNUM; i++){
		SetLedState(i, LEDOFF);
	}
}

/**
  *@brief ªÒ»°ledµƒ◊¥Ã¨
  *@param  ledNum µ∆∫≈
  *@param  ledIs led◊¥Ã¨
  *@retval None
  */
void SetLedState(uint8_t ledNum, LedStatus ledIs)
{
#if LED_HARDWARE_NO
	if(ledIs == LEDON)
#else
	if(ledIs == LEDOFF)
#endif
	{
		switch(ledNum){
			case 0:GPIO_SetBits(PORT_LED0, PIN_LED0);break;
			case 1:GPIO_SetBits(PORT_LED1, PIN_LED1);break;
			case 2:GPIO_SetBits(PORT_LED2, PIN_LED2);break;
			case 3:GPIO_SetBits(PORT_LED3, PIN_LED3);break;
			case 4:GPIO_SetBits(PORT_LED4, PIN_LED4);break;
			case 5:GPIO_SetBits(PORT_LED5, PIN_LED5);break;
			case 6:GPIO_SetBits(PORT_LED6, PIN_LED6);break;
			case 7:GPIO_SetBits(PORT_LED7, PIN_LED7);break;
			case 8:GPIO_SetBits(PORT_LED8, PIN_LED8);break;
			case 9:GPIO_SetBits(PORT_LED9, PIN_LED9);break;
			case 10:GPIO_SetBits(PORT_LED10, PIN_LED10);break;
			case 11:GPIO_SetBits(PORT_LED11, PIN_LED11);break;
			case 12:GPIO_SetBits(PORT_LED12, PIN_LED12);break;
			case 13:GPIO_SetBits(PORT_LED13, PIN_LED13);break;
			case 14:GPIO_SetBits(PORT_LED14, PIN_LED14);break;
			case 15:GPIO_SetBits(PORT_LED15, PIN_LED15);break;
			case 16:GPIO_SetBits(PORT_LED16, PIN_LED16);break;
			case 17:GPIO_SetBits(PORT_LED17, PIN_LED17);break;
			case 18:GPIO_SetBits(PORT_LED18, PIN_LED18);break;
			case 19:GPIO_SetBits(PORT_LED19, PIN_LED19);break;
			default:break;
		}
	}
#if LED_HARDWARE_OFF
	else if(ledIs == LEDON)
#else
	else if(ledIs == LEDOFF)
#endif
	{
		switch(ledNum){
			case 0:GPIO_ResetBits(PORT_LED0, PIN_LED0);break;
			case 1:GPIO_ResetBits(PORT_LED1, PIN_LED1);break;
			case 2:GPIO_ResetBits(PORT_LED2, PIN_LED2);break;
			case 3:GPIO_ResetBits(PORT_LED3, PIN_LED3);break;
			case 4:GPIO_ResetBits(PORT_LED4, PIN_LED4);break;
			case 5:GPIO_ResetBits(PORT_LED5, PIN_LED5);break;
			case 6:GPIO_ResetBits(PORT_LED6, PIN_LED6);break;
			case 7:GPIO_ResetBits(PORT_LED7, PIN_LED7);break;
			case 8:GPIO_ResetBits(PORT_LED8, PIN_LED8);break;
			case 9:GPIO_ResetBits(PORT_LED9, PIN_LED9);break;
			case 10:GPIO_ResetBits(PORT_LED10, PIN_LED10);break;
			case 11:GPIO_ResetBits(PORT_LED11, PIN_LED11);break;
			case 12:GPIO_ResetBits(PORT_LED12, PIN_LED12);break;
			case 13:GPIO_ResetBits(PORT_LED13, PIN_LED13);break;
			case 14:GPIO_ResetBits(PORT_LED14, PIN_LED14);break;
			case 15:GPIO_ResetBits(PORT_LED15, PIN_LED15);break;
			case 16:GPIO_ResetBits(PORT_LED16, PIN_LED16);break;
			case 17:GPIO_ResetBits(PORT_LED17, PIN_LED17);break;
			case 18:GPIO_ResetBits(PORT_LED18, PIN_LED18);break;
			case 19:GPIO_ResetBits(PORT_LED19, PIN_LED19);break;
			default:break;
		}
	}
}

/**
  *@brief ªÒ»°ledµƒ◊¥Ã¨
  *@param  ledNum µ∆∫≈
  *@retval led◊¥Ã¨
  */
LedStatus GetLedStatus(uint8_t ledNum)
{
	uint8_t tledstatus = 2;
	switch(ledNum){
		case 0:tledstatus = GPIO_ReadOutputDataBit(PORT_LED0, PIN_LED0);break;
		case 1:tledstatus = GPIO_ReadOutputDataBit(PORT_LED1, PIN_LED1);break;
		case 2:tledstatus = GPIO_ReadOutputDataBit(PORT_LED2, PIN_LED2);break;
		case 3:tledstatus = GPIO_ReadOutputDataBit(PORT_LED3, PIN_LED3);break;
		case 4:tledstatus = GPIO_ReadOutputDataBit(PORT_LED4, PIN_LED4);break;
		case 5:tledstatus = GPIO_ReadOutputDataBit(PORT_LED5, PIN_LED5);break;
		case 6:tledstatus = GPIO_ReadOutputDataBit(PORT_LED6, PIN_LED6);break;
		case 7:tledstatus = GPIO_ReadOutputDataBit(PORT_LED7, PIN_LED7);break;
		case 8:tledstatus = GPIO_ReadOutputDataBit(PORT_LED8, PIN_LED8);break;
		case 9:tledstatus = GPIO_ReadOutputDataBit(PORT_LED9, PIN_LED9);break;
		case 10:tledstatus = GPIO_ReadOutputDataBit(PORT_LED10, PIN_LED10);break;
		case 11:tledstatus = GPIO_ReadOutputDataBit(PORT_LED11, PIN_LED11);break;
		case 12:tledstatus = GPIO_ReadOutputDataBit(PORT_LED12, PIN_LED12);break;
		case 13:tledstatus = GPIO_ReadOutputDataBit(PORT_LED13, PIN_LED13);break;
		case 14:tledstatus = GPIO_ReadOutputDataBit(PORT_LED14, PIN_LED14);break;
		case 15:tledstatus = GPIO_ReadOutputDataBit(PORT_LED15, PIN_LED15);break;
		case 16:tledstatus = GPIO_ReadOutputDataBit(PORT_LED16, PIN_LED16);break;
		case 17:tledstatus = GPIO_ReadOutputDataBit(PORT_LED17, PIN_LED17);break;
		case 18:tledstatus = GPIO_ReadOutputDataBit(PORT_LED18, PIN_LED18);break;
		case 19:tledstatus = GPIO_ReadOutputDataBit(PORT_LED19, PIN_LED19);break;
		default:tledstatus = 2;break;
	}
	if(tledstatus == LED_HARDWARE_NO){
		return LEDON;
	}
	else if(tledstatus == LED_HARDWARE_OFF){
		return LEDOFF;
	}
	else{
		return LEDERROR;
	}
}

void LedDriverInit(void)
{
	LedGpioInit();
	SetLedState(0, LEDON);
}

/* END */

#include "led.h"

uint32_t LedCount = 0;
uint8_t LedcountFlag = 0;

/**
  * @Description: led初始化
  * @param:  None
  * @return: None
  */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	LED_CLOCK_ENABLE();
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_LOW;
	
	/* LED0 */
	GPIO_Initure.Pin = PIN_LED0;
	HAL_GPIO_Init(PORT_LED0,&GPIO_Initure);
	
	/* 初始化 */
	HAL_GPIO_WritePin(PORT_LED0,PIN_LED0,GPIO_PIN_SET);	//PB0置1 
	LedCount = 0;
	LedcountFlag = 1;
}
/**
  * @Description:取反LED
  * @param:  None
  * @return: None
  */
void ToggleLed(void)
{
    if(HAL_GPIO_ReadPin(PORT_LED0, PIN_LED0)){
				HAL_GPIO_WritePin(PORT_LED0,PIN_LED0,GPIO_PIN_RESET);	//PB0置0
			}
			else{
				HAL_GPIO_WritePin(PORT_LED0,PIN_LED0,GPIO_PIN_SET);	//PB0置0 
			}
}

/**
  * @Description: led运行灯翻转
  * @param:  None
  * @return: None
  */
void LedRun(void)
{
	if(LedcountFlag == 1){
		LedCount ++;
		if(LedCount > 100){
			if(HAL_GPIO_ReadPin(PORT_LED0, PIN_LED0)){
				HAL_GPIO_WritePin(PORT_LED0,PIN_LED0,GPIO_PIN_RESET);	//PB0置0
			}
			else{
				HAL_GPIO_WritePin(PORT_LED0,PIN_LED0,GPIO_PIN_SET);	//PB0置0 
			}
			LedCount = 0;
		}
	}   
}

/**
  * @Description: led使用杀死
  * @param:  None
  * @return: None
  */
void ledUseKill(void)
{
	LED_CLOCK_DISABLE();
}

/* END */

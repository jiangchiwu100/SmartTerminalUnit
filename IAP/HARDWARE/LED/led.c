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
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();           //开启GPIOH时钟,看门狗
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PB0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    GPIO_Initure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOG,&GPIO_Initure);     //看门狗
    
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0置1 
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_SET);	//PH3置1  
    
    LedCount = 0;
    LedcountFlag = 1;
}
/**
  * @Description: led运行灯翻转
  * @param:  None
  * @return: None
  */
void LedRun(void)
{
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);	//PB0置0
    }
    else{
       HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0置0 
    }
}


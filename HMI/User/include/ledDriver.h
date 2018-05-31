/**
  * @file    led.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   
  */
  
#ifndef _LEDDRIVER_H_
#define _LEDDRIVER_H_

#include "stm32f10x.h"


/* 电源灯 */
#define LED_POWER_OFF()  		GPIO_SetBits(GPIOC, GPIO_Pin_9)		   
#define LED_POWER_ON()	 		GPIO_ResetBits(GPIOC, GPIO_Pin_9)
/* 运行灯 */
#define LED_RUN_OFF()			GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define LED_RUN_ON()			GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LED_RUN_GETSTATUS()		GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)

/* LED GPIO口时钟-APB2 */
#define RCC_APB2Periph_LED (RCC_APB2Periph_GPIOD |\
	RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE)

#define LED_ALLNUM		16

#define PORT_LED0		GPIOC
#define PIN_LED0		GPIO_Pin_9
#define PORT_LED1		GPIOA
#define PIN_LED1		GPIO_Pin_8
#define PORT_LED2		GPIOC
#define PIN_LED2		GPIO_Pin_7
#define PORT_LED3		GPIOC
#define PIN_LED3		GPIO_Pin_8
#define PORT_LED4		GPIOD
#define PIN_LED4		GPIO_Pin_15
#define PORT_LED5		GPIOC
#define PIN_LED5		GPIO_Pin_6
#define PORT_LED6		GPIOD
#define PIN_LED6		GPIO_Pin_13
#define PORT_LED7		GPIOD
#define PIN_LED7		GPIO_Pin_14
#define PORT_LED8		GPIOD
#define PIN_LED8		GPIO_Pin_11
#define PORT_LED9		GPIOD
#define PIN_LED9		GPIO_Pin_12
#define PORT_LED10		GPIOD
#define PIN_LED10			GPIO_Pin_9
#define PORT_LED11		GPIOD
#define PIN_LED11			GPIO_Pin_10
#define PORT_LED12		GPIOB
#define PIN_LED12			GPIO_Pin_15
#define PORT_LED13		GPIOD
#define PIN_LED13			GPIO_Pin_8
#define PORT_LED14		GPIOE
#define PIN_LED14			GPIO_Pin_10
#define PORT_LED15		GPIOE
#define PIN_LED15			GPIO_Pin_9

/* 和硬件相关 */
#define LED_HARDWARE_OFF		1
#define LED_HARDWARE_NO			0

/* led开关类型 */
typedef enum{
	LEDOFF,
	LEDON,
	LEDERROR
}LedStatus;

/* 设置Led状态 */
void SetLedState(uint8_t ledNum, LedStatus ledIs);
/* 获取Led状态 */
LedStatus GetLedStatus(uint8_t ledNum);
/* LedGpio口初始化 */
void LedGpioInit(void);

/* 总初始化 */
void LedDriverInit(void);

#endif /* END _LED_H_ */

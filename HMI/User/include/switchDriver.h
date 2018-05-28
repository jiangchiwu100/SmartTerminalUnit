/**
  * @file    keyDriver.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/16
  * @brief  
  */

#ifndef _SWITCHDRIVER_H_
#define _SWITCHDRIVER_H_

#include "stm32f10x.h"

/* 开关总数 */
#define SWITCH_NUM_ALL		6

#define RCC_APB2Periph_SWITCH (RCC_APB2Periph_GPIOE |\
	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD)

#define PORT_SWITCH0		GPIOE
#define PIN_SWITCH0			GPIO_Pin_0
#define PORT_SWITCH1		GPIOE
#define PIN_SWITCH1			GPIO_Pin_1
#define PORT_SWITCH2		GPIOA
#define PIN_SWITCH2			GPIO_Pin_11
#define PORT_SWITCH3		GPIOA
#define PIN_SWITCH3			GPIO_Pin_12
#define PORT_SWITCH4		GPIOD
#define PIN_SWITCH4			GPIO_Pin_3
#define PORT_SWITCH5		GPIOD
#define PIN_SWITCH5			GPIO_Pin_2

/* 开关处理 */
struct SwitchInfo{
	uint32_t filterCount[SWITCH_NUM_ALL];
	uint8_t  filterFlag[SWITCH_NUM_ALL];
	uint8_t  currentStatus[SWITCH_NUM_ALL];
};

/* 获取开关当前状态 */
uint8_t GetCurrentSwitchStatus(uint8_t switchNum);
void ScanSwitchStatus(void);
void SwitchDriverInit(void);

/* 调用外部函数 */
extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);
/**/
#define USE_SWITCHCHANGEDEALWITCH 1

#endif /* END _SWITCHDRIVER_H_ */

/* END */

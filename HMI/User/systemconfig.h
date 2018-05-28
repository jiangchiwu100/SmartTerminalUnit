#ifndef __SYSTEMCONFIG_H
#define __SYSTEMCONFIG_H

#include "stm32f10x.h"

#define IWDG_RELOADTIME 800 //看门狗喂狗时间 800ms

#define CLOSE_ALL_INT()  __set_PRIMASK(1)		
#define OPEN_ALL_INT() __set_PRIMASK(0)				 
#define Close_SysTick() SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk
#define OPen_SysTick() SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;
//void NVIC_Configuration(void);
//void SysTick_Init(int16_t clock_div);
void SystemconfigInit(void);
uint32_t GetTimer1Tick(void);
uint32_t GetTimer1IntervalTick(uint32_t beginTick);
void Delay_us(uint32_t nus);
void Delay_ms(uint16_t nms);

extern void LedRun(void);
#endif /* __GW_KEY_H */

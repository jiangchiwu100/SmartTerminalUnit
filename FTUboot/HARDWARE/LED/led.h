#ifndef _LED_H
#define _LED_H

#include "stm32f4xx.h"

/* LED ±÷” */
#define LED_CLOCK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_CLOCK_DISABLE()		(__HAL_RCC_GPIOB_CLK_DISABLE())

#define PORT_LED0		GPIOB
#define PIN_LED0		GPIO_PIN_0

void LED_Init(void);
void LedRun(void);
void ledUseKill(void);
void ToggleLed(void);
#endif

/* END */

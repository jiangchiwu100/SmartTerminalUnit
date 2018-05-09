/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_led.h
  * @brief:     The driver of led.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017.09.05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  * @updata  : [2018-02-27][Lexun][make code cleanup]
  */
  
#ifndef __DRV_LED_H__
#define __DRV_LED_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include "stm32f4xx_hal.h"


/* DEFINES -------------------------------------------------------------------*/
#ifdef  DEBUG
#define LED_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define LED_PRINTF(...)
#endif /* DEBUG */

#define LED_ON()                  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
#define LED_OFF()                 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

#define BOTTOM_POWER_ON()         HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7, GPIO_PIN_RESET);
#define BOTTOM_POWER_OFF()        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7, GPIO_PIN_SET);

#define BOTTOM_LED_ON()           HAL_GPIO_WritePin(GPIOH, GPIO_PIN_8, GPIO_PIN_RESET);
#define BOTTOM_LED_OFF()          HAL_GPIO_WritePin(GPIOH, GPIO_PIN_8, GPIO_PIN_SET);

#define ALARM_LED_ON()            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
#define ALARM_LED_OFF()           HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
#define ALARM_LED_RECOVER()       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
extern int rt_hw_led_init(void);
extern void rt_hw_led_task(void);
extern void rt_hw_alarm_led_operate(uint8_t act);


#endif /* END _DRV_LED_H_ */


/* END OF FILE ---------------------------------------------------------------*/


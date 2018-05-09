 /** 
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_iic.h
  * @brief:     The driver of iic,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    tanek
  * @date:      2017-06-05
  * @update:    [2017-11-22][Lexun][The STM32F7 version(V1.0.0)]
  */
#ifndef __DRV_IIC_H__
#define __DRV_IIC_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include <drivers/i2c.h>
#include "stm32f4xx_hal.h"


/* DEFINES -------------------------------------------------------------------*/
#ifdef DEBUG
#define IIC_PRINTF(...)    rt_kprintf(__VA_ARGS__)
#else
#define IIC_PRINTF(...)   
#endif /* DEBUG */

#define IIC0_SDA_RCC_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define IIC0_SDA_GPIO_PORT        GPIOB
#define IIC0_SDA_GPIO_PIN         GPIO_PIN_9

#define IIC0_SCL_RCC_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define IIC0_SCL_GPIO_PORT        GPIOB
#define IIC0_SCL_GPIO_PIN         GPIO_PIN_8


#endif /* __DRV_IIC_H__ */


/* END OF FILE ---------------------------------------------------------------*/


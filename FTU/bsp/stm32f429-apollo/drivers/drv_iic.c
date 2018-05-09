 /** 
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_iic.c
  * @brief:     The driver of iic,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    tanek
  * @date:      2017-06-05
  * @update:    [2017-11-22][Lexun][The STM32F7 version(V1.0.0)]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include "drv_iic.h"

#include <board.h>
#include <finsh.h>
#include <rtdevice.h>
#include <rthw.h>
#include "drv_sd2405.h"
#include "common_data.h"


/* PRIVATE FUNCTION ----------------------------------------------------------*/
static void _iic_set_sda(void *data, rt_int32_t state);
static void _iic_set_scl(void *data, rt_int32_t state);
static rt_int32_t _iic_get_sda(void *data);
static rt_int32_t _iic_get_scl(void *data);
static void _iic_udelay(rt_uint32_t us);


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static const struct rt_i2c_bit_ops bit_ops = {
    RT_NULL,
    _iic_set_sda,
    _iic_set_scl,
    _iic_get_sda,
    _iic_get_scl,
    _iic_udelay,
    5,
    100
};


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : iic gpio config.
  * @param : None.
  * @retval: None.
  */
static void _iic_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//	
//    /*PB8--I2C1_SCL; PB9--I2C1_SDA */
//    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
    IIC0_SDA_RCC_ENABLE();
    IIC0_SCL_RCC_ENABLE();
	
    /*PB8--I2C1_SCL; PB9--I2C1_SDA */
    GPIO_InitStruct.Pin = IIC0_SDA_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(IIC0_SDA_GPIO_PORT, &GPIO_InitStruct);
		
    GPIO_InitStruct.Pin = IIC0_SCL_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		
    HAL_GPIO_Init(IIC0_SCL_GPIO_PORT, &GPIO_InitStruct);

}

/**
  * @brief : set SDA gpio pin.
  * @param : [data] the data.
  * @param : [state] the state.
  * @retval: None.
  */
static void _iic_set_sda(void *data, rt_int32_t state)
{
    HAL_GPIO_WritePin(IIC0_SDA_GPIO_PORT, IIC0_SDA_GPIO_PIN, (GPIO_PinState)state);
}

/**
  * @brief : set SCL gpio pin.
  * @param : [data] the data.
  * @param : [state] the state.
  * @retval: None.
  */
static void _iic_set_scl(void *data, rt_int32_t state)
{
    HAL_GPIO_WritePin(IIC0_SCL_GPIO_PORT, IIC0_SCL_GPIO_PIN, (GPIO_PinState)state);
}

/**
  * @brief : get sda pin status.
  * @param : [data] the data.
  * @retval: return the status of SDA pin.
  */
static rt_int32_t _iic_get_sda(void *data)
{
    return (rt_int32_t)HAL_GPIO_ReadPin(IIC0_SDA_GPIO_PORT, IIC0_SDA_GPIO_PIN);
}

/**
  * @brief : get scl pin status.
  * @param : [data] the data.
  * @retval: return the status of SCL pin.
  */
static rt_int32_t _iic_get_scl(void *data)
{
    return (rt_int32_t)HAL_GPIO_ReadPin(IIC0_SCL_GPIO_PORT, IIC0_SCL_GPIO_PIN);
}

/**
  * @brief : iic delay function.
  * @param : [us] the us timer delay.
  * @retval: None.
  */
static void _iic_udelay(rt_uint32_t us)
{
    rt_int32_t i;
    for (; us > 0; us--)
    {
        i = 50;
        while (i > 0)
        {
            i--;
        }
    }
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : iic gpio config.
  * @param : None.
  * @retval: None.
  */
int rt_hw_iic_init(void)
{
    static struct rt_i2c_bus_device bus; // This variable must be static, otherwise an error will occur
	
//    bus = rt_malloc(sizeof(struct rt_i2c_bus_device));
//    if (bus == RT_NULL)
//    {
//        IIC_PRINTF("rt_malloc failed\n");
//        return -RT_ENOMEM;
//    }

    rt_memset((void *)&bus, 0, sizeof(struct rt_i2c_bus_device));

    bus.priv = (void *)&bit_ops;

    _iic_gpio_init();

//    rt_i2c_bit_add_bus(&bus, "i2c0");
    rt_i2c_bit_add_bus(&bus, RT_I2C0_NAME);
	
    return(RT_EOK);
}
INIT_DEVICE_EXPORT(rt_hw_iic_init);


/* END OF FILE ---------------------------------------------------------------*/


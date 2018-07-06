/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_io_spi.c
  * @brief:     The driver of SPI by GPIO.
  * @version:   V1.0.0
  * @author:    Lexun
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
#ifndef __DRV_IO_SPI_H__
#define __DRV_IO_SPI_H__


#ifdef __cplusplus
extern "C" {
#endif
	
	
/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include <drivers/spi.h>

#include "stm32f4xx_hal.h"


/* DEFINES -------------------------------------------------------------------*/
//#define RT_USING_IO_SPI1
	
	
/* SPI hardware configuration */
#define IO_SPI1_CS_RCC_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define IO_SPI1_CS_GPIO_PORT      GPIOB
#define IO_SPI1_CS_GPIO_PIN       GPIO_PIN_12

#define IO_SPI1_CLK_RCC_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()
#define IO_SPI1_CLK_GPIO_PORT     GPIOB
#define IO_SPI1_CLK_GPIO_PIN      GPIO_PIN_13

#define IO_SPI1_MOSI_RCC_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define IO_SPI1_MOSI_GPIO_PROT    GPIOB
#define IO_SPI1_MOSI_GPIO_PIN     GPIO_PIN_15

#define IO_SPI1_MISO_RCC_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define IO_SPI1_MISO_GPIO_PORT    GPIOB
#define IO_SPI1_MISO_GPIO_PIN     GPIO_PIN_14


/* ENUMERATION ---------------------------------------------------------------*/
typedef enum IO_SPI
{
    IO_SPI1,
    IO_SPI2,
    IO_SPI3,
    IO_SPI4,
}IO_SPI_TypeDef;


/* DEFINES -------------------------------------------------------------------*/
/* feature structure */
struct stm32_io_spi_bus
{
    struct rt_spi_bus parent;
};

struct stm32_io_spi 
{
    GPIO_TypeDef* CLK_GPIOx;
    uint16_t CLK_GPIO_Pin;
	
    GPIO_TypeDef* MOSI_GPIOx;
    uint16_t MOSI_GPIO_Pin;
	
    GPIO_TypeDef* MISO_GPIOx;
    uint16_t MISO_GPIO_Pin;
};


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
rt_err_t stm32_io_spi_bus_register(IO_SPI_TypeDef IO_SPI, struct stm32_io_spi_bus *stm32_spi, const char *spi_bus_name);


#ifdef __cplusplus
}
#endif /* END __cplusplus */ 
    
#endif /* END __BOARD_IO_SPI_H__ */ 


/* END OF FILE ---------------------------------------------------------------*/

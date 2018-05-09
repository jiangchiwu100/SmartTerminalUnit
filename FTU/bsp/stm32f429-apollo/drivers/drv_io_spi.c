/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_io_spi.c
  * @brief:     The driver of SPI by GPIO.
  * @version:   V1.0.0
  * @author:    Lexun
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "drv_io_spi.h"
#include "drv_spi.h"


/* PRIVATE FUNCTION ----------------------------------------------------------*/
static rt_err_t configure(struct rt_spi_device* device, struct rt_spi_configuration* configuration);
static rt_uint32_t xfer(struct rt_spi_device* device, struct rt_spi_message* message);


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct rt_spi_ops stm32_io_spi1_ops =
{
    configure,
    xfer
};

#ifdef RT_USING_IO_SPI1
static struct rt_spi_bus io_spi1_bus;
struct stm32_io_spi stm32f4_io_spi1 = 
{
    IO_SPI1_CLK_GPIO_PORT,
    IO_SPI1_CLK_GPIO_PIN,
    IO_SPI1_MOSI_GPIO_PROT,
    IO_SPI1_MOSI_GPIO_PIN,
    IO_SPI1_MISO_GPIO_PORT,
    IO_SPI1_MISO_GPIO_PIN
};
#endif /* RT_USING_IO_SPI1 */


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief  IO_SPI configuration.
  * @param  [device] The SPI device descriptor.
  * @param  [configuration] The SPI device configuration parameter.
  * @retval [RT_EOK] sucess.
  */
static rt_err_t configure(struct rt_spi_device* device, struct rt_spi_configuration* configuration)
{
    return(RT_EOK);
}

/**
  * @brief  IO_SPI xfer function.
  * @param  [device] The SPI device descriptor.
  * @param  [message] The SPI device message.
  * @retval return the length of message.
  */
static rt_uint32_t xfer(struct rt_spi_device* device, struct rt_spi_message* message)
{
    struct rt_spi_bus *stm32_spi_bus = (struct rt_spi_bus *)device->bus;
    struct stm32_io_spi *p_io_spi = (struct stm32_io_spi *)stm32_spi_bus->parent.user_data;
	
    struct stm32_spi_cs *stm32_spi_cs = device->parent.user_data;
    rt_uint32_t size = message->length;
  
    /* take CS */
    if (message->cs_take)
    {
         HAL_GPIO_WritePin(stm32_spi_cs->GPIOx, stm32_spi_cs->GPIO_Pin, GPIO_PIN_RESET);
    }
	
    {
        const rt_uint8_t * send_ptr = message->send_buf;
        rt_uint8_t * recv_ptr = message->recv_buf;
        rt_uint8_t i;
						
        while (size--) 
        {
            rt_uint8_t data = 0xFF;
            rt_uint8_t u8Rtn = 0;
            if (send_ptr != RT_NULL) 
            {
                data = *send_ptr++;
            }
      
            for (i = 0; i < 8; i++)
            {
                u8Rtn <<= 1;
                if (data & 0x80)
                {
                    HAL_GPIO_WritePin(p_io_spi->MOSI_GPIOx, p_io_spi->MOSI_GPIO_Pin, GPIO_PIN_SET);
                }
                else
                {
                    HAL_GPIO_WritePin(p_io_spi->MOSI_GPIOx, p_io_spi->MOSI_GPIO_Pin, GPIO_PIN_RESET);
                }
				
                data <<= 1;
				
                HAL_GPIO_WritePin(p_io_spi->CLK_GPIOx, p_io_spi->CLK_GPIO_Pin, GPIO_PIN_SET);
								
                if (HAL_GPIO_ReadPin(p_io_spi->MISO_GPIOx, p_io_spi->MISO_GPIO_Pin))
                {
                    u8Rtn |= 0x01;
                }
				
                HAL_GPIO_WritePin(p_io_spi->CLK_GPIOx, p_io_spi->CLK_GPIO_Pin, GPIO_PIN_RESET);
            }
      
            if (recv_ptr != RT_NULL) 
            {
                *recv_ptr++ = u8Rtn;
            }
        }
    }
  
    /* release CS */
    if (message->cs_release)
    {
         HAL_GPIO_WritePin(stm32_spi_cs->GPIOx, stm32_spi_cs->GPIO_Pin, GPIO_PIN_SET);
    }
  
    return message->length;
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief  IO_SPI driver initialization.
  * @param  None
  * @retval return the register status.
  */
rt_err_t stm32_io_spi_bus_register(IO_SPI_TypeDef IO_SPI, struct stm32_io_spi_bus *stm32_spi, const char *spi_bus_name)
{
    struct rt_spi_bus *spi_bus;
	
    RT_ASSERT(spi_bus_name != RT_NULL);
//    RT_ASSERT(stm32_spi != RT_NULL);
	
#ifdef RT_USING_IO_SPI1
    if (IO_SPI == IO_SPI1)
    {
        spi_bus = &io_spi1_bus;
        spi_bus->parent.user_data = &stm32f4_io_spi1;
    }
#endif /* RT_USING_IO_SPI1 */
	
    if ((IO_SPI != IO_SPI1) && (IO_SPI != IO_SPI2) && (IO_SPI != IO_SPI3) && (IO_SPI != IO_SPI4))
    {
        return (RT_ENOSYS);
    }
	  		
    return rt_spi_bus_register(spi_bus, spi_bus_name, &stm32_io_spi1_ops); 
}

/* END OF FILE ---------------------------------------------------------------*/

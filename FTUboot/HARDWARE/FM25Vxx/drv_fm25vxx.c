/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fm25vxx.c
  * @brief:     The driver of FM25Vxx10.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017.09.05
  * @update:    [YYYY-MM-DD] [更改人姓名][变更描述]
  */
/* INCLUDES ------------------------------------------------------------------*/
#include "string.h"
#include <stm32f4xx.h>
#include "drv_fm25vxx.h"

/******************************************SPI驱动********************************************/
/**
  * @Description: SPI单字节操作
  * @param:  TxData-发送的数据
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t IO_SPI1_ReadWriteByte(uint8_t byte)
{
    unsigned char BitCnt = 0;
	
    for (BitCnt = 0; BitCnt < 8; BitCnt++)
    {
        if (byte & 0x80)
        {
            IO_SPI1_MOSI_HIGH();
        }
        else
        {
            IO_SPI1_MOSI_LOW();
        }
				
        byte <<= 1;
				
        IO_SPI1_SCK_HIGH();
//        rt_thread_delay(10); 
								
        if (IO_SPI1_MISO_READ())
        {
            byte |= 0x01;
        }
				
        IO_SPI1_SCK_LOW();
//        rt_thread_delay(10); 
    }
		
    return (unsigned char)byte;
}

/**
  * @Description: SPI4多字节操作
  * @param:  pTxData-发送的数据指针
  * @param:  pRxData-接收数据指针
  * @param:  Size-数据长度
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void IO_SPI1_ReadWrite(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
    uint16_t i = 0;

    for (i = 0; i<Size; i++)
    {
        pRxData[i] = IO_SPI1_ReadWriteByte(pTxData[i]);
    }
}

/**
  * @Description: SPI4硬件初始化
  * @param:  hspi-SPI句柄
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void IO_SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/******************************************SPI应用********************************************/
/**
  * @Description: 从外设中读取指定长度的数据.
  * @param:  addr-起始地址
  * @param:  pBuf-读出数据缓冲区
  * @param:  len-读取数据长度
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t FM25VxxReadData(uint32_t addr, uint8_t *flag, uint8_t *pBuf, uint32_t len)
{
    uint32_t i;
    uint8_t cmd[4];

    if (len <= 0)
    {
        return FALSE;
    }

    if ((addr + len) > FM25V10_MAX_ADDR)
    {
        return FALSE;
    }

    cmd[0] = FRAM_READ;
    cmd[1] = addr >> 16 & 0xFF;
    cmd[2] = addr >> 8  & 0xFF;
    cmd[3] = addr & 0xFF;

    IO_SPI1_CS_LOW(); // 片选使能

    for (i = 0; i < 4; i++)
    {
        IO_SPI1_ReadWriteByte(cmd[i]);
    }

    if (flag != NULL)
    {
        *flag = IO_SPI1_ReadWriteByte(0xFF);
        for (i = 0; i < len - 1; i++)
        {
            *pBuf++ = IO_SPI1_ReadWriteByte(0xFF);
        }
    }
    else
    {
        for (i = 0; i < len; i++)
        {
            *pBuf++ = IO_SPI1_ReadWriteByte(0xFF);
        }
    }
    IO_SPI1_CS_HIGH(); // 片选关闭

    return TRUE;
}

/**
  * @Description: 向外设中写入指定长度的数据.
  * @param:  addr-起始地址
  * @param:  pBuf-写入数据缓冲区
  * @param:  len-写入数据长度
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t FM25VxxWriteData(uint32_t addr, uint8_t *flag, uint8_t *pBuf, uint32_t len)
{
    uint32_t i;
    uint8_t cmd[4];

    if (len <= 0)
    {
        return FALSE;
    }

    if ((addr + len) > FM25V10_MAX_ADDR)
    {
        return FALSE;
    }
    
    IO_SPI1_CS_LOW(); // 片选使能
    IO_SPI1_ReadWriteByte(FRAM_WREN); // 写入使能
    IO_SPI1_CS_HIGH(); // 片选禁能

    cmd[0] = FRAM_WRITE;
    cmd[1] = addr >> 16 & 0xFF;
    cmd[2] = addr >> 8  & 0xFF;
    cmd[3] = addr & 0xFF;

    IO_SPI1_CS_LOW(); // 片选使能

    for (i = 0; i < 4; i++)
    {
        IO_SPI1_ReadWriteByte(cmd[i]);
    }
    //SPI1_ReadWrite(cmd, NULL, 4);
    if (flag != NULL)
    {
        IO_SPI1_ReadWriteByte(*flag);
        for (i = 0; i < len - 1; i++)
        {
            IO_SPI1_ReadWriteByte(*pBuf++);
        }
    }
    else
    {
        for (i = 0; i < len; i++)
        {
            IO_SPI1_ReadWriteByte(*pBuf++);
        }
    }

    IO_SPI1_CS_HIGH(); // 片选关闭

    IO_SPI1_CS_LOW(); // 片选使能
    IO_SPI1_ReadWriteByte(FRAM_WRDI); // 写入禁止
    IO_SPI1_CS_HIGH(); // 片选禁能

    return TRUE;
}


/**
  * @Description: FramInit硬件初始化.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void FM25VxxInit(void)
{    
    IO_SPI1_Init();
}

/**
  * @Description: FM25Vxx杀死
  * @param:  None
  * @return: None
  */
void FM25VxxUseKill(void)
{
	__HAL_RCC_GPIOB_CLK_DISABLE();
}
	

/* END OF FILE ---------------------------------------------------------------*/

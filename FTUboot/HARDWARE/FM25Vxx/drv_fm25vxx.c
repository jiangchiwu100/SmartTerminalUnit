/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fm25v10.c
  * @brief:     The driver of FM25Vxx10.
  * @version:   ver 1.1
  * @author:    TianXiaoLiang
  * @date:      2018.07.31
  * @update:    [2018-08-05] [TianXiaoLiang][更新驱动]
  */
#include "drv_fm25vxx.h"

#include "spi.h"


static uint8_t framflag;					//!<标志为“1”选择SPI2收发，为“0”选择SPI5收发

uint8_t (*FramReadDate)(uint32_t address, uint32_t number, uint8_t* p);
void (*FramWriteDate)(uint32_t address, uint32_t number, uint8_t* p);

void delay_us(uint16_t us)
{
    for( ;us--;)
    {       
        for(uint16_t i = 0; i < 100;i++);
    }
}

/*
 *@brief:  FRAM采用SPI5初始化
 *@param:  无
 *@return: 无
 */
void FRAM_SPI5_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	GPIO_Initure.Pin=GPIO_PIN_6;
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Pull=GPIO_PULLUP;
	GPIO_Initure.Speed=GPIO_SPEED_HIGH; 
	
	HAL_GPIO_Init(GPIOF,&GPIO_Initure);
		
	FRAM_CS = 1;
	SPI5_Init();
	SPI5_SetSpeed(SPI_BAUDRATEPRESCALER_2);

}
/*
 *@brief:  FRAM写一个字节
 *@param:  address-写入数据地址
 *@param:  da-写入数据
 *@return: 无
 */
void FramWriteByte(uint32_t address, uint8_t da)
{
	uint8_t temH,temM,temL;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	
	if(framflag == 0)
	{
		FRAM_CS = 0;	
		SPI5_ReadWriteByte(WREN);	
		FRAM_CS = 1;
		
		delay_us(2);
		
		FRAM_CS = 0;
		SPI5_ReadWriteByte(WRITE);
		SPI5_ReadWriteByte(temH);
		SPI5_ReadWriteByte(temM);
		SPI5_ReadWriteByte(temL);
		SPI5_ReadWriteByte(da);	
		FRAM_CS = 1;
		
		delay_us(2);
		
		FRAM_CS = 0;	
		SPI5_ReadWriteByte(WRDI);	
		FRAM_CS = 1;
	}
	else if(framflag == 1)
	{
		IO_SPI2_CS_LOW(); 
		IO_SPI2_ReadWriteByte(WREN); 
		IO_SPI2_CS_HIGH();
		
		delay_us(2);
		
		IO_SPI2_CS_LOW();
		IO_SPI2_ReadWriteByte(WRITE);
		IO_SPI2_ReadWriteByte(temH);
		IO_SPI2_ReadWriteByte(temM);
		IO_SPI2_ReadWriteByte(temL);
		IO_SPI2_ReadWriteByte(da);
		IO_SPI2_CS_HIGH();
		
		delay_us(2);
		
		IO_SPI2_CS_LOW(); 
		IO_SPI2_ReadWriteByte(WRDI);
		IO_SPI2_CS_HIGH();
	}
}
/*
 *@brief:  FRAM读一个字节
 *@param:  address-读取数据地址
 *@return: 读取一个字节数据
 */
uint8_t FramReadByte(uint32_t address)
{
	uint8_t temp;
	uint8_t temH,temM,temL;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	
	if (framflag == 0)
	{
		FRAM_CS = 0;
	
		SPI5_ReadWriteByte(READ);
		SPI5_ReadWriteByte(temH);
		SPI5_ReadWriteByte(temM);
		SPI5_ReadWriteByte(temL);
			
		temp = SPI5_ReadWriteByte(0xF0);
	
		FRAM_CS = 1;
	}
	else if(framflag == 1)
	{
		IO_SPI2_CS_LOW();
		
		IO_SPI2_ReadWriteByte(READ);
		IO_SPI2_ReadWriteByte(temH);
		IO_SPI2_ReadWriteByte(temM);
		IO_SPI2_ReadWriteByte(temM);
		
		temp = IO_SPI2_ReadWriteByte(0xF0);

		IO_SPI2_CS_HIGH();
	}
	
	return temp;
}
/*
 *@brief:  FRAM写字符串
 *@param:  address-写入数据地址
 *@param:  number-写入数据长度
 *@param:  p-写入字符串首地址
 *@return: 无
 */
void FramWriteString(uint32_t address, uint32_t number, uint8_t* p)
{
	uint8_t temH,temM,temL;
	uint32_t i;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	
	if(framflag == 0)
	{
		FRAM_CS = 0;
		SPI5_ReadWriteByte(WREN);
		FRAM_CS = 1;
		
		delay_us(2);
		FRAM_CS = 0;
		SPI5_ReadWriteByte(WRITE);
		SPI5_ReadWriteByte(temH);
		SPI5_ReadWriteByte(temM);
		SPI5_ReadWriteByte(temL);
		for(i = 0; i < number; i++)
		{
			SPI5_ReadWriteByte(*p++);
		}
		FRAM_CS = 1;
		delay_us(2);
		FRAM_CS = 0;
		SPI5_ReadWriteByte(WRDI);	
		FRAM_CS = 1;
	}
	else if(framflag == 1)
	{
		IO_SPI2_CS_LOW(); 
		IO_SPI2_ReadWriteByte(WREN); 
		IO_SPI2_CS_HIGH();
		
		delay_us(2);
		IO_SPI2_CS_LOW();
		IO_SPI2_ReadWriteByte(WRITE);
		IO_SPI2_ReadWriteByte(temH);
		IO_SPI2_ReadWriteByte(temM);
		IO_SPI2_ReadWriteByte(temL);
		
		for(i = 0; i < number; i++)
		{
			IO_SPI2_ReadWriteByte(*p++);
		}
		IO_SPI2_CS_HIGH();
		delay_us(2);
		IO_SPI2_CS_LOW(); 
		IO_SPI2_ReadWriteByte(WRDI);
		IO_SPI2_CS_HIGH();		
	}
}
/*
 *@brief:  FRAM读字符串
 *@param:  address-读取数据地址
 *@param:  number-读取数据长度
 *@param:  p-读入缓冲区首地址
 *@return: 无
 */
uint8_t FramReadString(uint32_t address, uint32_t number, uint8_t* p)
{
	uint8_t temH,temM,temL;
	uint32_t i;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	if(framflag == 0)
	{
		FRAM_CS = 0;
	
		SPI5_ReadWriteByte(READ);
		SPI5_ReadWriteByte(temH);
		SPI5_ReadWriteByte(temM);
		SPI5_ReadWriteByte(temL);
	
		for (i = 0; i < number; i++)
		{
			*p++ = SPI5_ReadWriteByte(0xF0);
		}
	
		FRAM_CS = 1;
	}
	else if(framflag == 1)
	{
		IO_SPI2_CS_LOW();
		
		IO_SPI2_ReadWriteByte(READ);
		IO_SPI2_ReadWriteByte(temH);
		IO_SPI2_ReadWriteByte(temM);
		IO_SPI2_ReadWriteByte(temL);
		
		for (i = 0; i < number; i++)
		{
			*p++ = IO_SPI2_ReadWriteByte(0xF0);
		}
	
		IO_SPI2_CS_HIGH();
	}
	return TRUE;
}
/*
 *@brief:  模拟SPI2时钟读写一个字节
 *@param:  byte-读写数据
 *@return: 无
 */

uint8_t IO_SPI2_ReadWriteByte(uint8_t byte)
{
    unsigned char BitCnt = 0;
	
    for (BitCnt = 0; BitCnt < 8; BitCnt++)
    {
        if (byte & 0x80)
        {
            IO_SPI2_MOSI_HIGH();
        }
        else
        {
            IO_SPI2_MOSI_LOW();
        }
				
        byte <<= 1;
				
        IO_SPI2_SCK_HIGH();
		//delay_us(10); 
								
        if (IO_SPI2_MISO_READ())
        {
            byte |= 0x01;
        }
				
        IO_SPI2_SCK_LOW();
		//delay_us(10); 
    }
		
    return (unsigned char)byte;
}
/*
 *@brief:  SPI2读写字符串
 *@param:  pTxData-写字符串首地址
 *@param:  pRxData-读字符串首地址
 *@param:  Size-读写长度
 *@return: 无
 */

void IO_SPI2_ReadWrite(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
    uint16_t i = 0;

    for (i = 0; i<Size; i++)
    {
        pRxData[i] = IO_SPI2_ReadWriteByte(pTxData[i]);
    }
}

/*
 *@brief:  FRAM采用SPI2初始化
 *@param:  无
 *@return: 无
 */
void IO_SPI2_Init(void)
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
/*
 *@brief:  FRAM采用SPI2读ID
 *@param:  无
 *@return: 设备ID
 */
uint16_t FRAM_SPI2_ReadID(void)
{
	uint8_t tmp, i;
	uint16_t ID = 0;
	
	IO_SPI2_CS_LOW();
	
	IO_SPI2_ReadWriteByte(RDID);
			
	for (i = 0; i < 9; i++)
	{
		tmp = IO_SPI2_ReadWriteByte(0xF0);

		if (i == 7)									//!<设备ID1，成功0x24
		{
			ID = ID | (tmp << 8);
		}
		else if(i == 8)								//!<设备ID2，成功0x00
		{
			ID = ID | tmp;
		}
	}
	IO_SPI2_CS_HIGH();
	return ID;
}
/*
 *@brief:  FRAM采用SPI5读ID
 *@param:  无
 *@return: 设备ID
 */
uint16_t FRAM_SPI5_ReadID(void)
{
	uint8_t tmp, i;
	uint16_t ID = 0;
	
	FRAM_CS = 0;
	
	SPI5_ReadWriteByte(RDID);			
			
	for (i = 0; i < 9; i++)
	{
		tmp = SPI5_ReadWriteByte(0xF0);

		if (i == 7)									//!<设备ID1，成功0x24
		{
			ID = ID | (tmp << 8);
		}
		else if(i == 8)								//!<设备ID2，成功0x00
		{
			ID = ID | tmp;
		}
	}
	FRAM_CS = 1;
	return ID;
}
/*
 *@brief:  FRAM初始化
 *@param:  无
 *@return: 无
 */
void FM25VxxInit(void)
{  
    IO_SPI2_Init();
	if (DEVICEID == FRAM_SPI2_ReadID())
	{
		framflag = 1;
		printf(" FRAM is initialized with SPI2\r\n");
		return;
	}
	else 
	{
		FRAM_SPI5_Init();
		if(DEVICEID == FRAM_SPI5_ReadID())
		{
			framflag = 0;
			printf(" FRAM is initialized with SPI5\r\n");
			return;
		}			
	}
	
	printf(" FRAM initialized err!\r\n");
	
	return;
}
/*-----------------------------------------------------------------------------------------*/

void FramSPI5WriteString(uint32_t address, uint32_t number, uint8_t* p)
{
	uint8_t temH,temM,temL;
	uint32_t i;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	

	FRAM_CS = 0;
	SPI5_ReadWriteByte(WREN);
	FRAM_CS = 1;
		
	delay_us(2);
	FRAM_CS = 0;
	SPI5_ReadWriteByte(WRITE);
	SPI5_ReadWriteByte(temH);
	SPI5_ReadWriteByte(temM);
	SPI5_ReadWriteByte(temL);
	for(i = 0; i < number; i++)
	{
		SPI5_ReadWriteByte(*p++);
	}
	FRAM_CS = 1;
	delay_us(2);
	FRAM_CS = 0;
	SPI5_ReadWriteByte(WRDI);	
	FRAM_CS = 1;

}
void FramSPI2WriteString(uint32_t address, uint32_t number, uint8_t* p)
{
	uint8_t temH,temM,temL;
	uint32_t i;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	
	IO_SPI2_CS_LOW(); 
	IO_SPI2_ReadWriteByte(WREN); 
	IO_SPI2_CS_HIGH();
		
	delay_us(2);
	IO_SPI2_CS_LOW();
	IO_SPI2_ReadWriteByte(WRITE);
	IO_SPI2_ReadWriteByte(temH);
	IO_SPI2_ReadWriteByte(temM);
	IO_SPI2_ReadWriteByte(temL);
	
	for(i = 0; i < number; i++)
	{
		IO_SPI2_ReadWriteByte(*p++);
	}
	IO_SPI2_CS_HIGH();
	delay_us(2);
	IO_SPI2_CS_LOW(); 
	IO_SPI2_ReadWriteByte(WRDI);
	IO_SPI2_CS_HIGH();		
}

/*
 *@brief:  FRAM读字符串
 *@param:  address-读取数据地址
 *@param:  number-读取数据长度
 *@param:  p-读入缓冲区首地址
 *@return: 无
 */
uint8_t FramSPI5ReadString(uint32_t address, uint32_t number, uint8_t* p)
{
	uint8_t temH,temM,temL;
	uint32_t i;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);

	FRAM_CS = 0;
	
	SPI5_ReadWriteByte(READ);
	SPI5_ReadWriteByte(temH);
	SPI5_ReadWriteByte(temM);
	SPI5_ReadWriteByte(temL);
	
	for (i = 0; i < number; i++)
	{
		*p++ = SPI5_ReadWriteByte(0xF0);
	}
	
	FRAM_CS = 1;

	return TRUE;
}

uint8_t FramSPI2ReadString(uint32_t address, uint32_t number, uint8_t* p)
{
	uint8_t temH,temM,temL;
	uint32_t i;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	
	IO_SPI2_CS_LOW();
		
	IO_SPI2_ReadWriteByte(READ);
	IO_SPI2_ReadWriteByte(temH);
	IO_SPI2_ReadWriteByte(temM);
	IO_SPI2_ReadWriteByte(temL);
	
	for (i = 0; i < number; i++)
	{
		*p++ = IO_SPI2_ReadWriteByte(0xF0);
	}
	
	IO_SPI2_CS_HIGH();
	
	return TRUE;
}


void FRAM_Init(void)
{
	IO_SPI2_Init();
	if (DEVICEID == FRAM_SPI2_ReadID())
	{
		FramReadDate = FramSPI2ReadString;
		FramWriteDate = FramSPI2WriteString;
		printf(" FRAM is initialized with SPI2\r\n");
		return;
	}
	else 
	{
		FRAM_SPI5_Init();
		if(DEVICEID == FRAM_SPI5_ReadID())
		{
			FramReadDate = FramSPI5ReadString;
			FramWriteDate = FramSPI5WriteString;
			printf(" FRAM is initialized with SPI5\r\n");
			return;
		}			
	}
	
	printf(" FRAM initialized err!\r\n");
	
	return;
}

/*-----------------------------------------------------------------------------------------*/


/*
 *@brief:  检查FRAM
 *@param:  无
 *@return: 无
 */
void Fram_Check(void)
{
	uint8_t data_Tx[256];
	uint8_t data_Rx[1] = "0";
	for(uint16_t tmp = 0; tmp < 256; tmp++)
	{
		data_Tx[tmp] = 0xAA;							//!<给发送缓冲区赋值 0xAA
	}
		
	for(uint16_t tmp = 0; tmp < 256; tmp++)
	{
		if(data_Tx[tmp] != 0xAA)						//!<检查缓冲区值
		{
			printf("write buffer define err!\r\n");
		}
	}
	printf("fram write buffer succeed!\r\n");
	
	for (uint32_t i = 0; i < 1024; i=i+256)
	{
		FramWriteDate(i, 256, (uint8_t*)data_Tx);		//!<写入Fram中
	}
	for (uint32_t i = 0; i < 1024; i++)
	{
		FramReadDate(i, 1, (uint8_t*)data_Rx);		//!<读fram中的值
		if(data_Rx[0] == 0xAA)							//!<检查读取fram中的值
		{
			continue;
		}
		else
		{
			printf("fram Check Failed!\r\n");
			return;
		}
	}
	printf("fram Check succeed!\r\n");
}

/**
  * @Description: FM25Vxx杀死
  * @param:  None
  * @return: None
  */
void FM25VxxUseKill(void)
{
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOF_CLK_DISABLE();
}
	

/* END OF FILE ---------------------------------------------------------------*/

#include "fm25v10.h"

#include "spi.h"
#include "drv_fm25vxx.h"

#include "systemconfig.h"

/*
 *@brief:FRAM³õÊ¼»¯
 *
 *
 */
void FRAM_Init(void)
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
 *@brief:FRAMÐ´Ò»¸ö×Ö½Ú
 *
 *
 */
void FramWriteByte(uint32_t address, uint8_t da)
{
	uint8_t temH,temM,temL;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	
	FRAM_CS = 0;
	SPI5_ReadWriteByte(WREN);
	
	FRAM_CS = 1;
	Delay_us(2);
	
	FRAM_CS = 0;
	SPI5_ReadWriteByte(WRITE);
	SPI5_ReadWriteByte(temH);
	SPI5_ReadWriteByte(temM);
	SPI5_ReadWriteByte(temL);
	SPI5_ReadWriteByte(da);
	
	FRAM_CS = 1;
	Delay_us(2);
	
	FRAM_CS = 0;
	SPI5_ReadWriteByte(WRDI);
	FRAM_CS = 1;	
}
/*
 *@brief:FRAM¶ÁÒ»¸ö×Ö½Ú
 *
 *
 */
uint8_t FramReadByte(uint32_t address)
{
	uint8_t temp;
	uint8_t temH,temM,temL;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	FRAM_CS = 0;
	
	SPI5_ReadWriteByte(READ);
	SPI5_ReadWriteByte(temH);
	SPI5_ReadWriteByte(temM);
	SPI5_ReadWriteByte(temL);
	
	temp = SPI5_ReadWriteByte(0xF0);
	FRAM_CS = 1;
	return temp;
}
/*
 *@brief:FRAMÐ´×Ö·û´®
 *
 *
 */
uint8_t FramWriteInduce(uint32_t address, uint32_t number, uint8_t* p)
{
	uint8_t temH,temM,temL;
	uint32_t i;
	temH = (uint8_t)((address & 0xff0000) >> 16);
	temM = (uint8_t)((address & 0x00ff00) >> 8);
	temL = (uint8_t)(address & 0x0000ff);
	FRAM_CS = 0;
	SPI5_ReadWriteByte(WREN);
	FRAM_CS = 1;
	Delay_us(2);
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
	Delay_us(2);
	FRAM_CS = 0;
	SPI5_ReadWriteByte(WRDI);
	FRAM_CS = 1;
    return TRUE;
}
/*
 *@brief:FRAM¶Á×Ö·û´®
 *
 *
 */
uint8_t FramReadInduce(uint32_t address, uint32_t number, uint8_t* p)
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


void FM25VxxUseKill(void)
{
	__HAL_RCC_GPIOB_CLK_DISABLE();
}
#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stdint.h>

#include "stdio.h"
#define DEVICE_ID "W5500"

#define FW_VER_HIGH  		1
#define FW_VER_LOW    	0

#include "spi.h"

//#define WIZ_SCS			    GPIO_Pin_12	// out
//#define WIZ_SCLK				GPIO_Pin_13	// out
//#define WIZ_MISO				GPIO_Pin_14	// in
//#define WIZ_MOSI				GPIO_Pin_15	// out

#define WIZ_RESET		    PIout(8)
#define	WIZ_CS 		PIout(0)  		//W25QXXµÄÆ¬Ñ¡ÐÅºÅ
//#define WIZ_INT			    GPIO_Pin_6	// in

#define W5500_CS_SELECT()   {WIZ_CS = 0;}
#define W5500_CS_DESELECT()     {WIZ_CS = 1;}



typedef  void (*pFunction)(void);

void NVIC_Configuration(void);
uint8_t set_network(void);
void write_config_to_eeprom(void);
void set_default(void);
extern void Reset_W5500(void);

void reboot(void);
void get_config(void);

void w5500_spi_init(void);
void spi_dma_read(uint8_t* Addref, uint8_t* pRxBuf, uint16_t rx_len);
void spi_dma_write(uint8_t* Addref, uint8_t* pTxBuf, uint16_t tx_len);

void LL_SPI2_Init(void);
uint8_t SPI2_SendByte(uint8_t txData);
#endif


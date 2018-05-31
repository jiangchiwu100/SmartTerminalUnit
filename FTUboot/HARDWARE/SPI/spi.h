#ifndef __SPI_H
#define __SPI_H

#include "sys.h"

extern SPI_HandleTypeDef SPI4_Handler;  //SPI¾ä±ú

void SPI4_Init(void);
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI4_ReadWriteByte(u8 TxData);
void SpiUseKill(void);
#endif

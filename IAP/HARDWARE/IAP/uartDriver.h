
#ifndef __UARTDRIVER_H
#define	__UARTDRIVER_H


#include "stm32f4xx.h"
#include "stdio.h"

void uartInit(void);
void SendData(uint8_t *pBuff, uint16_t length);
void uartDisable(void);

#endif /* __UARTDRIVER_H */

/* END */


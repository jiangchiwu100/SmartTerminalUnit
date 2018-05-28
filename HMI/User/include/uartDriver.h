
#ifndef __UARTDRIVER_H
#define	__UARTDRIVER_H


#include "stm32f10x.h"
#include "stdio.h"

extern void uartInit(void);
extern void SendData(uint8_t *pBuff, uint16_t length);

#endif /* __UARTDRIVER_H */

/* END */


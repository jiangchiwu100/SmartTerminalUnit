
#ifndef __MAINFLASHOPERATE_H
#define	__MAINFLASHOPERATE_H

#include "stm32f4xx.h"

typedef void (*Iapfun)(void);

#define MAINPRO_BEGINADDR 0x08040000
#define MAINPRO_ENDADDR 	0x080CFFFF 
#define MAINPRO_BEGINSECTOR 6
#define MAINPRO_SECTORNUM	5

uint8_t EreaseProgram(void);
uint8_t WriteProgram(uint32_t addr,const uint8_t *pBuff,uint32_t sizeByte);
uint16_t ReadProgram(uint32_t addr, uint8_t *pBuff,uint32_t sizeByte);
uint32_t IapLoadApp(uint32_t appxaddr);


#endif /* __MAINFLASHOPERATE_H */

/* END */

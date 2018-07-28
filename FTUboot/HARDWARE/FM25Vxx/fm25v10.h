#ifndef __FM25V10_H
#define __FM25V10_H


#include "sys.h"

#define WREN 	0x06
#define WRDI	 0x04
#define RDSR	 0x05
#define WRSR	 0x01
#define READ	 0x03
#define FSTRD	 0x0B
#define WRITE	 0x02
#define SLEEP    0xB9
#define RDID	 0X9F
#define SNR		 0xC3
#define FRAM_CS  PFout(6)

void FRAM_Init(void);
void FramWriteByte(uint32_t address, uint8_t da);
uint8_t FramReadByte(uint32_t address);
uint8_t FramWriteInduce(uint32_t address, uint32_t number, uint8_t* p);
uint8_t FramReadInduce(uint32_t address, uint32_t number, uint8_t* p);


#endif
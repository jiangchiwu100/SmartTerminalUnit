#ifndef __W550_DEMO_H__
#define __W550_DEMO_H__

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "deviceIO.h"
#include "ult.h"
#include "device.h"
#include "w5500.h"
#include "socket.h"
#include "BasicModule.h"
#include "buffer.h"

#include "stdio.h"
#include "string.h"

extern uint16_t MacRawOutput(uint8_t* pData, uint16_t len);
extern uint16_t ReciveInputBlock(PointUint8** pPacket);
uint8_t UDPTest(void);

void MX_TIM10_Init(void);
void StartTimer(void);
void StopTimer(void);

#endif

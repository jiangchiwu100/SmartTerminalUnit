/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_HMISLAVE_disk.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */
#ifndef	_HMI_101_DISK_H_
#define _HMI_101_DISK_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* PUBLIC VARIABLE -----------------------------------------------------------*/

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern uint16_t DLT634_HMI_SLAVE_ReadData(uint8_t pdrv, uint8_t *pbuf, uint16_t count);
extern uint16_t DLT634_HMI_SLAVE_WriteData(uint8_t pdrv, uint8_t *pbuf, uint16_t count);

extern uint8_t DLT634_HMI_SLAVE_TO_HMI(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_HMI_SLAVE_H_HMIYX(uint8_t pdrv);
extern void DLT634_HMI_SLAVE_R_HMIYX(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_HMI_SLAVE_EMPTY(uint8_t pdrv);

extern void DLT634_HMI_SlaveTask(void);
extern int DLT634_HMI_SLAVE_INIT(void);

uint8_t DLT634_HMI_SLAVE_C_REPLY(uint8_t drvid,uint8_t *pbuf);

/* 调用外部 */
extern uint8_t hmi101Scan(uint8_t *pBuff);
extern void Cmd101SendFinish(void);
extern void Hmi101Init(void);
#endif /* END _HMI_101_DISK_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/

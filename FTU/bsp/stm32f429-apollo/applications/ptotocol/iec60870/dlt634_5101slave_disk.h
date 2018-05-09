/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5101slave_disk.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */
#ifndef	_DLT634_5101SLAVE_DISK_H_
#define _DLT634_5101SLAVE_DISK_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* PUBLIC VARIABLE -----------------------------------------------------------*/

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern uint16_t DLT634_5101_SLAVE_ReadData(uint8_t pdrv, uint8_t *pbuf, uint16_t count);
extern uint16_t DLT634_5101_SLAVE_WriteData(uint8_t pdrv, uint8_t *pbuf, uint16_t count);

extern void DLT634_5101_SLAVE_C_IC(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_SLAVE_C_SC(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_SLAVE_C_CS(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_SLAVE_C_RP(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_SLAVE_C_SR(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_SLAVE_F_FR(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5101_SLAVE_F_SR(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5101_SLAVE_H_COS(uint8_t pdrv);
extern void DLT634_5101_SLAVE_R_COS(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5101_SLAVE_H_SOE(uint8_t pdrv);
extern void DLT634_5101_SLAVE_R_SOE(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5101_SLAVE_H_NVA(uint8_t pdrv);
extern void DLT634_5101_SLAVE_R_NVA(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5101_SLAVE_H_FEvent(uint8_t pdrv);
extern void DLT634_5101_SLAVE_R_FEvent(uint8_t pdrv, uint8_t *pbuf);
extern uint16_t DLT634_5101_SLAVE_R_YXDATA(uint8_t pdrv,uint16_t addr,uint16_t num, uint8_t *pbuf);
extern uint16_t DLT634_5101_SLAVE_R_YCDATA(uint8_t pdrv,uint16_t addr,uint16_t num, uint8_t *pbuf);
extern uint8_t DLT634_5101_SLAVE_C_REPLY(uint8_t drvid,uint8_t *pbuf);
extern void DLT634_5101_SLAVE_R_IDLE(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5101_SLAVE_H_Encrypt(uint8_t pdrv);

extern void DLT634_5101_SlaveTask(void);
extern int DLT634_5101_SLAVE_INIT(void);

#endif /* END _DLT634_5101_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/

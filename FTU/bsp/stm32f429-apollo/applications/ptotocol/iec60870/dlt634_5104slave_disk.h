/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5104slave_disk.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */
  
#ifndef	_DLT634_5104SLAVE_APP_H_
#define _DLT634_5104SLAVE_APP_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* DEFINE --------------------------------------------------------------------*/

/* Enum ----------------------------------------------------------------------*/

/* PUBLIC VARIABLES ----------------------------------------------------------*/

/* PUBLIC FUNCTION -----------------------------------------------------------*/
extern int DLT634_5104_ParaInit(void);
extern void DLT634_5104_SlaveTask(void);
extern uint8_t DLT634_5104_SLAVE_C_REPLY(uint8_t drvid, uint8_t *pbuf);
extern uint16_t DLT634_5104_SLAVE_ReadData(uint8_t port, uint8_t *pbuf, uint16_t count);
extern uint16_t DLT634_5104_SLAVE_WriteData(uint8_t port, uint8_t *pbuf, uint16_t count);
extern uint8_t DLT634_5104_SLAVE_StopLink(uint8_t pdrv);
extern uint8_t DLT634_5104_SLAVE_CheckLink(uint8_t pdrv);

extern uint16_t DLT634_5104_SLAVE_ReadYxData(uint8_t port, uint16_t addr, uint16_t num, uint8_t *pbuf);
extern uint16_t DLT634_5104_SLAVE_ReadYcData(uint8_t port, uint16_t addr, uint16_t num, uint8_t *pbuf);
extern void DLT634_5104_SLAVE_CallAllDataProcess(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5104_SLAVE_HandleCtrlProcess(uint8_t port, uint8_t *pbuf);
extern void DLT634_5104_SLAVE_SYNProcess(uint8_t port, uint8_t *pbuf);
extern void DLT634_5104_SLAVE_FixedParaProcess(uint8_t port, uint8_t *pbuf);
extern void DLT634_5104_SLAVE_FileHandleProcess(uint8_t port, uint8_t *pbuf);
extern void DLT634_5104_SLAVE_SoftwareUpdate(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5104_SLAVE_H_COS(uint8_t pdrv);
extern void DLT634_5104_SLAVE_R_COS(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5104_SLAVE_H_SOE(uint8_t pdrv);
extern void DLT634_5104_SLAVE_R_SOE(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5104_SLAVE_H_NVA(uint8_t pdrv);
extern void DLT634_5104_SLAVE_R_NVA(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5104_SLAVE_H_FEvent(uint8_t pdrv);
extern void DLT634_5104_SLAVE_R_FEvent(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5104_SLAVE_ResetProcess(uint8_t pdrv, uint8_t *pbuf);
extern void DLT634_5104_SLAVE_IdleProcess(uint8_t pdrv, uint8_t *pbuf);
#endif /* END _DLT634_5104SLAVE_APP_H_*/
    

/* END OF FILE ---------------------------------------------------------------*/

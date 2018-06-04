/*
** ------------------------------- Copyright (c) -------------------------------                                     
**
**                           http://www.sojoline.com
**
** note:    Copyright SOJO. ALL RIGHTS RESERVED.
**          This software is provided under license and contains proprietary and
**          confidential material which is the property of Company Name tech. 
**    
** -------------------------------- File Info ----------------------------------
** File name:               dlt634_HMImaster_disk.h
** Descriptions:            The application layer of DL/T634.HMI_2002
**
** -----------------------------------------------------------------------------
** Created by:              Mr.j
** Created date:            2015.10.25
** System:                  Protocol Software System
** Version:                 V3.01
** Descriptions:            The original version
**
** -----------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
** ---------------------------------------------------------------------------*/
#ifndef	_DLT634_HMIMASTER_DISK_H_
#define _DLT634_HMIMASTER_DISK_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "string.h"
#include "stdint.h"



/* PUBLIC VARIABLE -----------------------------------------------------------*/

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
uint8_t DLT634_HMI_MASTER_TO_HMI(uint8_t pdrv, uint8_t *pbuf);
uint16_t DLT634_HMI_MASTER_ReadData(uint8_t pdrv, uint8_t *pbuf, uint16_t count);
uint16_t DLT634_HMI_MASTER_WriteData(uint8_t pdrv, uint8_t *pbuf, uint16_t count);

extern void DLT634_HMI_SLAVE_EMPTY(uint8_t pdrv);

/* 遥信变位查询 */
uint8_t DLT634_HMI_MASTER_H_HMIYX(uint8_t pdrv);
/* 所有遥信下发 */
void DLT634_HMI_MASTER_R_HMIYX(uint8_t pdrv, uint8_t *pbuf);
/* 特殊东西往下发 */
uint8_t DLT634_HMI_MASTER_C_REPLY(uint8_t drvid,uint8_t *pbuf);
/* 10ms一次的任务 */
void DLT634_HMI_MasterTask(void* arg);

void DLT634_HMI_MASTER_INIT(uint8_t pdrv);

/* ------由外部编写------- */
/* 读函数 */
extern uint16_t Readx(uint8_t *pbuf, uint16_t count);
/* 写函数 */
extern uint16_t WriteX(uint8_t *pbuf, uint16_t count);
/* 命令处理 */
extern uint8_t hmi101Scan(uint8_t *pBuff);
extern void Cmd101SendFinish(void);
extern void Hmi101Init(void);
#endif /* END _DLT634_HMI_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/

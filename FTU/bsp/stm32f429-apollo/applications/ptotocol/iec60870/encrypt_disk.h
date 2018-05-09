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
** File name:               encrypt_disk.h
** Descriptions:            The application layer of encrypt_disk
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
#ifndef	_ENCRYPT_DISK_H_
#define _ENCRYPT_DISK_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "string.h"
#include <rtthread.h>

/* PUBLIC VARIABLE -----------------------------------------------------------*/


/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern rt_uint8_t Encrypt_AddDevice(rt_device_t device,rt_uint16_t(*Read_drv)(rt_device_t device, rt_uint8_t *pbuf, rt_uint16_t count),rt_uint16_t (*Write_drv)(rt_device_t device, rt_uint8_t *pbuf, rt_uint16_t count));
extern rt_uint16_t Encrypt_ReadData(rt_uint8_t *pbuf, rt_uint16_t count, rt_uint8_t pdrv);
extern rt_uint16_t Encrypt_WriteData(rt_uint8_t *pbuf, rt_uint16_t count, rt_uint8_t pdrv);
extern void Encrypt_ChipCOM(rt_uint8_t *txbuff, rt_uint8_t *rxbuff);


extern rt_uint16_t Encrypt_Readx(rt_uint8_t *pbuf, rt_device_t device);
extern rt_uint16_t Encrypt_WriteX(rt_uint8_t *pbuf, rt_uint16_t count, rt_uint8_t type, rt_device_t device);
extern rt_uint8_t Encrypt_CheckSend(rt_device_t device);

#endif /* END _ENCRYPT_DISK_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/

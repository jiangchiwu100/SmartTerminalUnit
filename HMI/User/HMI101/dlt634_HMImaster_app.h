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
** File name:               dlt634_HMImaster_app.h
** Descriptions:            The application layer of DL/T634.HMI_2002
**
** -----------------------------------------------------------------------------
** Created by:              Mr.Lee
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
#ifndef	_DLT634_HMIMASTER_APP_H_
#define _DLT634_HMIMASTER_APP_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "dlt634_HMImaster_config.h"
#include "stdint.h"

#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif

/* PUBLIC VARIABLE -----------------------------------------------------------*/

typedef struct 
{
	uint8_t Port;            //端口
	
	uint8_t IEC_DIR;         // 终端0x80,主站0x00
	uint8_t SourceAddr;	    // 源地址，即本机地址
	
	uint8_t LinkAddrSize;   //链路地址长度
	uint8_t ASDUCotSize;    //传送原因长度
	
	uint8_t ASDUAddr;       //ASDU地址
	uint8_t ASDUAddrSize;    //ASDU地址长度
	uint8_t FixFrmLength;		//固定帧长度
	
	uint32_t ClockTimers;		//调用超出次数，程序执行一次，控制运行周期
	uint32_t TimeOutValue;		//超时重发间隔
	
	uint32_t AskStaOutValue;		//超时重发间隔
	uint32_t AllDataOutValue;		//超时重发间隔
	uint32_t LinkTestValue;		//超时重发间隔
	uint32_t LinkIdleValue;		//链路空闲计时器
	
	uint32_t DevidValue;		   //设备占用计时器
	
}DLT634_HMIMASTER_PAD;

extern DLT634_HMIMASTER_PAD      DLT634_HMIMaster_Pad[_DLT634_HMIMASTER_VOLUMES];

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern void DLT634_HMI_MASTER_Clock(uint8_t pdrv);
extern uint8_t DLT634_HMI_MASTER_REPLY(uint8_t pdrv, uint8_t *pbuf);//回复

#endif /* END _DLT634_HMI_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/



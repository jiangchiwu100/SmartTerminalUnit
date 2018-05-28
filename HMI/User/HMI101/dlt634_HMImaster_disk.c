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
** File name:               DLT634_HMI_master_disk.c
** Descriptions:            The application of DLT634.HMI_2002
**
** -----------------------------------------------------------------------------
** Created by:              Mr.J
** Created date:            2017.5.16
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

#include "dlt634_HMImaster_disk.h"
#include "dlt634_HMImaster_app.h"

/* PRIVATE VARIABLES ----------------------------------------------------------*/
/* PUBLIC VARIABLES ----------------------------------------------------------*/


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/

/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
//link
/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_ReadData
** 功能描述: 读数据
** 输　  入: pdrv：设备序号
**           pbuf：数据内容指针
**           count：读数量
** 输　  出: 无
** 全局变量: 
** 调用模块: Readx  向底层读数据
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
uint16_t DLT634_HMI_MASTER_ReadData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{ 
	uint16_t len = 0;  
	
	switch(pdrv)
	{
		case 0:
			len = Readx(pbuf, count);

			break;
		default:
			break;
	}
	return(len);
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_WriteData
** 功能描述: 写数据
** 输　  入: pdrv：设备序号
**           pbuf：数据内容指针
**           count：写数量
** 输　  出: 无
** 全局变量: 
** 调用模块: WriteX  向底层写数据
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
uint16_t DLT634_HMI_MASTER_WriteData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{ 
	uint16_t len = 0; 
	
	switch(pdrv)
	{
		case 0:
			len = WriteX(pbuf,count);
			break;
		default:
			break;
	}
	return(len);
}

//app
/**
  * @brief : TO HMI.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_MASTER_TO_HMI(uint8_t pdrv, uint8_t *pbuf)//TO HMI
{
	return hmi101Scan(pbuf);
}

/**
  * @brief : EMPTY.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_HMI_SLAVE_EMPTY(uint8_t pdrv)
{   
	Cmd101SendFinish();
}

/**
  * @brief : judg soe.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_MASTER_H_HMIYX(uint8_t pdrv)//
{
	Cmd101SendFinish();
    return(0);    
}

/**
  * @brief : read soe.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_HMI_MASTER_R_HMIYX(uint8_t pdrv, uint8_t *pbuf)//?HMIYX
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/Array(InfoAddr_L/InfoAddr_H/Value/CP56Time2a) 
    uint16_t length;
 
    switch(pdrv)
    {
        case DLT634_HMIMASTER_DISK0:
				length = 6;
        pbuf[0] = length&0xff;
        pbuf[1] = (length>>8)&0xff;
        pbuf[2] = 0;
        pbuf[3] = 0;
        pbuf[4] = 0;
        pbuf[5] = 0;
        break;
    }
}
/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_SLAVE_C_REPLY
** 功能描述: 其他设备回复
** 输　  入: drvid：应用设备序号
**         : pbuf：回复内容数据指针
** 输　  出: 无
** 全局变量: 
** 调用模块: DBSend  发送给其他设备函数
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/
uint8_t DLT634_HMI_MASTER_C_REPLY(uint8_t drvid,uint8_t *pbuf)//其他设备回复
{
    DLT634_HMI_MASTER_REPLY(0, pbuf); 

	return(TRUE);
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_INIT
** 功能描述: 设置参数初始化
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 调用模块: 
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
void DLT634_HMI_MASTER_INIT(uint8_t pdrv)//初始化
{
    switch(pdrv)
	{
        case 0:
            DLT634_HMIMaster_Pad[pdrv].IEC_DIR = 0;
            DLT634_HMIMaster_Pad[pdrv].SourceAddr = 1;
            DLT634_HMIMaster_Pad[pdrv].LinkAddrSize = 2;//1:97,2:02
            DLT634_HMIMaster_Pad[pdrv].ASDUCotSize = 2;//1:97,2:02
            DLT634_HMIMaster_Pad[pdrv].ASDUAddr = 1;
            DLT634_HMIMaster_Pad[pdrv].ASDUAddrSize = 2;//1:97,2:02
            DLT634_HMIMaster_Pad[pdrv].FixFrmLength = 6;//5:97,6:02
            DLT634_HMIMaster_Pad[pdrv].ClockTimers = 1;//程序运行间隔节拍
            DLT634_HMIMaster_Pad[pdrv].TimeOutValue = 10;//重发间隔节拍
            DLT634_HMIMaster_Pad[pdrv].AskStaOutValue = 100;//链路请求间隔节拍
						DLT634_HMIMaster_Pad[pdrv].AllDataOutValue = 500;
            break;
        default:
            break;
	}
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MasterTask
** 功能描述: 101主站任务函数
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 调用模块: DLT634_HMI_MASTER_Clock  定时执行函数
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
void DLT634_HMI_MasterTask(void* arg)
{
	DLT634_HMI_MASTER_Clock(DLT634_HMIMASTER_DISK0);
}

/* END OF FILE ---------------------------------------------------------------*/


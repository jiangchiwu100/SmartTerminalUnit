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
** File name:               encrypt_disk.c
** Descriptions:            The disk of encrypt
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

#include "encrypt_app.h"
#include "encrypt_disk.h"
#include "drv_sc1161y.h"

/* PRIVATE VARIABLES ----------------------------------------------------------*/
typedef struct 
{
	rt_device_t dev;            //端口
    rt_uint16_t (*Read_drv)(rt_device_t device, uint8_t *pbuf, uint16_t count);
    rt_uint16_t (*Write_drv)(rt_device_t device, uint8_t *pbuf, uint16_t count);
}ENCRYPT_PAD;

ENCRYPT_PAD      Encrypt_Pad[_ENCRYPT_VOLUMES];

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/

/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
rt_uint8_t Encrypt_AddDevice(rt_device_t device,rt_uint16_t(*Read_drv)(rt_device_t device, uint8_t *pbuf, uint16_t count),rt_uint16_t (*Write_drv)(rt_device_t device, uint8_t *pbuf, uint16_t count))//添加加密设备
{ 
    static rt_uint8_t first = 1;
	rt_uint8_t res = FALSE;
    rt_uint8_t i=0;
    
    if(first)
    {
        first = 0;
        memset(&Encrypt_Pad,0,sizeof(Encrypt_Pad));
    }
        
    for(i=0;i<_ENCRYPT_VOLUMES;i++)
    {
        if(Encrypt_Pad[i].dev == RT_NULL)
        {
           Encrypt_Pad[i].dev =  device; 
           Encrypt_Pad[i].Read_drv = Read_drv;
           Encrypt_Pad[i].Write_drv = Write_drv;        	
           Encrypt_AppInit(i);
           res = TRUE;
        }
    }
	return(res);
}

rt_uint16_t Encrypt_ReadData(rt_uint8_t *pbuf, rt_uint16_t count, rt_uint8_t pdrv)//读取通讯口数据
{ 
	rt_uint16_t len = 0; 	
    
    len = Encrypt_Pad[pdrv].Read_drv(Encrypt_Pad[pdrv].dev,pbuf,count);

//    len = Readx(pbuf, count, Encrypt_Pad[pdrv].Port);

	return(len);
}

rt_uint16_t Encrypt_WriteData(rt_uint8_t *pbuf, rt_uint16_t count, rt_uint8_t pdrv)//发送通讯口数据
{ 
	rt_uint16_t len = 0; 	

    len = Encrypt_Pad[pdrv].Write_drv(Encrypt_Pad[pdrv].dev,pbuf,count);
    
//    len = WriteX(pbuf, count, Encrypt_Pad[pdrv].Port);

	return(len);
}

void Encrypt_ChipCOM(rt_uint8_t *txbuff, rt_uint8_t *rxbuff)//向加密芯片写数据并读数据
{
    sc1161y_ReadWrite(txbuff,rxbuff);
}

rt_uint16_t Encrypt_Readx(rt_uint8_t *pbuf, rt_device_t device)//读解密以后的数据
{
	rt_uint16_t len = 0;
    rt_uint8_t i=0;
	
    for(i=0;i<_ENCRYPT_VOLUMES;i++)///i = 0 ； i<1 ；i++
    {
        if(Encrypt_Pad[i].dev == device)
        {
           len = Encrypt_ReadMISIData(pbuf , i);///读取数据处理（pbuf,0）
           break;
        }
    }
   
	return(len);
}

rt_uint16_t Encrypt_WriteX(rt_uint8_t *pbuf, rt_uint16_t count, rt_uint8_t type, rt_device_t device)//需要加密的数据
{
    rt_uint8_t i=0;
	
    for(i=0;i<_ENCRYPT_VOLUMES;i++)
    {
        if(Encrypt_Pad[i].dev == device)
        {
            Encrypt_IECSendData(pbuf , count, type , i);//type:&0x80加密，0x7f应用类型
           break;
        }
    }
    
	return(count);
}

rt_uint8_t Encrypt_CheckSend(rt_device_t device)//检查是否有待发送加密交互数据，并发送
{
	rt_uint8_t res = FALSE;
    rt_uint8_t i=0;
	
    for(i=0;i<_ENCRYPT_VOLUMES;i++)
    {
        if(Encrypt_Pad[i].dev == device)
        {
           res = Encrypt_ENCSendData(i);
           break;
        }
    }
	return(res);
}

/* END OF FILE ---------------------------------------------------------------*/


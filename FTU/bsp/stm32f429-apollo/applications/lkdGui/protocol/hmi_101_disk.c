/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_HMISLAVE_disk.c
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */
#include "hmi_101_app.h"
#include "hmi_101_disk.h"
#include "common_data.h"
#include "file_operate.h"
#include "drv_do.h"
#include "encrypt_disk.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "string.h"

/* PRIVATE VARIABLES ----------------------------------------------------------*/
static rt_device_t dev[_DLT634_HMISLAVE_VOLUMES];

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/

/**
  * @brief : read data from drv.
  * @param : [pdrv]
  * @param : [pbuf]
  * @param : [count]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint16_t DLT634_HMI_SLAVE_ReadData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{ 
    uint16_t len = 0;
	  uint16_t i = 0;  
    
    len = rt_device_read(dev[pdrv],0,pbuf,count);
	if(len){
		rt_kprintf("\r\n--KMI  ");
	}
		for(i=0;i<len;i++)
			rt_kprintf("%02X ",pbuf[i]);
    
	return(len);
}

/**
  * @brief : write data to drv.
  * @param : [pdrv]
  * @param : [pbuf]
  * @param : [count]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint16_t DLT634_HMI_SLAVE_WriteData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{ 	
//uint16_t i = 0;
	
    rt_device_write(dev[pdrv],0,pbuf,count);
//	  rt_kprintf("\r\n--ZK   ");
//		for(i=0;i<count;i++)
//			rt_kprintf("%02X ",pbuf[i]);
    
    return(count);
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
  * @brief : TO HMI.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_SLAVE_TO_HMI(uint8_t pdrv, uint8_t *pbuf)//TO HMI
{
    return hmi101Scan(pbuf);    
}

/**
  * @brief : judg soe.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_SLAVE_H_HMIYX(uint8_t pdrv)//判断是否有soe
{

    return(0);    
}

/**
  * @brief : read soe.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_HMI_SLAVE_R_HMIYX(uint8_t pdrv, uint8_t *pbuf)//读HMIYX
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/Array(InfoAddr_L/InfoAddr_H/Value/CP56Time2a) 
    uint16_t length;
 
    switch(pdrv)
    {
        case DLT634_HMISLAVE_DISK0:
				length= 6;
        pbuf[0] = length&0xff;
        pbuf[1] = (length>>8)&0xff;
        pbuf[2] = 0;
        pbuf[3] = 0;
        pbuf[4] = 0;
        pbuf[5] = 0;
        break;
    }
}

/**
  * @brief : other reply.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_SLAVE_C_REPLY(uint8_t drvid,uint8_t *pbuf)//其他设备回复
{ 
    DLT634_HMI_SLAVE_REPLY(0,pbuf);
    return(0);
}

/**
  * @brief : init
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
int DLT634_HMI_SLAVE_INIT(void)
{
    struct rt_serial_device *serial;
    uint8_t pdrv;
    
    for(pdrv=0;pdrv<_DLT634_HMISLAVE_VOLUMES;pdrv++)
    {
        switch(pdrv)
        {
            case DLT634_HMISLAVE_DISK0:
                DLT634_HMI_SLAVE_AppInit(pdrv);        
                dev[pdrv] = rt_device_find(RT_USART6_NAME);
                serial = (struct rt_serial_device *)(dev[pdrv]);
                serial->config.baud_rate = BAUD_RATE_230400;
                rt_device_open(dev[pdrv], RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);            
                DLT634_HMISLAVE_Pad[pdrv].IEC_DIR = 0x80;
                DLT634_HMISLAVE_Pad[pdrv].SourceAddr = 0x01;
                DLT634_HMISLAVE_Pad[pdrv].FixFrmLength = 6;
                DLT634_HMISLAVE_Pad[pdrv].ClockTimers = 300*1000/DLT_101SLAVE_MS/115200 + 1;
                DLT634_HMISLAVE_Pad[pdrv].TimeOutValue = 1000/DLT_101SLAVE_MS/DLT634_HMISLAVE_Pad[pdrv].ClockTimers;//重发超时节拍
                DLT634_HMISLAVE_Pad[pdrv].AskStaOutValue = 5000/DLT_101SLAVE_MS/DLT634_HMISLAVE_Pad[pdrv].ClockTimers;;//链路请求间隔节拍
                DLT634_HMISLAVE_Pad[pdrv].AllDataOutValue = 5000/DLT_101SLAVE_MS/DLT634_HMISLAVE_Pad[pdrv].ClockTimers;;//链路请求间隔节拍 
                break;
								
            default:
                break;
        }
    }
    
    return(RT_EOK);
}

/**
  * @brief : task.
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_HMI_SlaveTask(void)
{
    static uint8_t disk0 = DLT634_HMISLAVE_DISK0;
	
		static uint8_t hmi_init = 1;
    
    if(DLT634_HMI_SLAVE_Clock(disk0))
		{
				if(hmi_init)
				{
						hmi_init = 0;	
						Hmi101Init();
					
				}					
		}
		else
		{
				if(hmi_init == 0)
				{
						hmi_init = 1;	
				}		
		}
}

/* END OF FILE ---------------------------------------------------------------*/


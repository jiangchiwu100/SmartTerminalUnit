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
** File name:               DLT634_HMI_app.c
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


/* INCLUDE FILES -------------------------------------------------------------*/
#include "dlt634_HMImaster_app.h"
#include "dlt634_HMImaster_disk.h"

/* PRIVATE VARIABLES ----------------------------------------------------------*/
#pragma pack(push,1)

typedef struct 
{
    struct STATUS
    {
        uint16_t Length;
    }status;

    uint8_t	buff[_DLT634_HMIMASTER_LPDUSIZE-sizeof(struct STATUS)];
}DLT634_HMIMASTER_PASDU;

typedef struct 
{
//link	
    enum
	{
        RXDSTART = 0,	          // 寻找起始标志状态
        RXDHEAD,	              // 寻找报文头
        RXDCONTINUE             // 接收后续报文
	}RxdStatus;           			// 接收状态
	
	uint16_t TxdHead;	      				// 发送缓冲区头指针
	uint16_t TxdTail;	     					// 发送缓冲区尾指针
	uint8_t TxdBuf[_DLT634_HMIMASTER_LPDUSIZE];     	// 发送缓冲区
	
	uint16_t RxdHead;	      				// 接收数据区头指针
    uint16_t RxdTail;       				// 接收数据区尾指针
	uint16_t RxdLength;     				// 接收一帧数据长度
	uint8_t RxdBuf[_DLT634_HMIMASTER_LPDUSIZE];			// 接收缓冲区
	
	uint16_t TxdTail_Pri;
	uint8_t TxdBuf_Pri[_DLT634_HMIMASTER_LPDUSIZE]; // 发送缓冲区，暂存发送的数据，重发时用
	
//app
	
	uint8_t StoreDATABuf[_DLT634_HMIMASTER_LPDUSIZE];     	// 数据待发送缓冲区
	
	DLT634_HMIMASTER_PASDU TX_Temp;              //发送数据处理
	DLT634_HMIMASTER_PASDU RX_Temp;              //接受数据处理
	
	uint32_t LinkFlag;              //回复链路数据标志
	uint32_t DataFlag;             //应用数据标志
	
	uint32_t ClockCounter;  				//程序运行周期控制
	uint32_t TimeOutTick_Pri; 			//超时节拍计时器
	uint32_t RetryCount;						//超时记数器
	
	uint32_t TimeOutTick_AskSta; 			//召唤链路节拍计时器
    uint32_t TimeOutTick_AllData; 
	
	uint8_t SendConCode;	  // 发送的控制码FCB位记录

}DLT634_HMIMASTER_APPINFO;
#pragma pack(pop)
/* PUBLIC VARIABLES ----------------------------------------------------------*/
DLT634_HMIMASTER_PAD      DLT634_HMIMaster_Pad[_DLT634_HMIMASTER_VOLUMES];
DLT634_HMIMASTER_APPINFO  DLT634_HMIMaster_App[_DLT634_HMIMASTER_VOLUMES];

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/


/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_CKS
** 功能描述: 计算CKS
** 输　  入: pdrv：设备序号
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
static uint8_t DLT634_HMI_MASTER_CKS(uint8_t pdrv ,uint8_t *pBuf)
{
    uint16_t sum = 0;
    uint16_t j =0;
    uint8_t *p = 0;
    uint16_t i = 0;

    if (*(pBuf) == _DLT634_HMIMASTER_STARTCODE10)
    {
        for (j = 0; j < (DLT634_HMIMaster_Pad[pdrv].LinkAddrSize + 1); j++)
        {
            sum += *(pBuf + 1 + j);
        }
    }
    else if(*(pBuf) == _DLT634_HMIMASTER_STARTCODE68)
    {
        p = pBuf+4;
        i = *(uint16_t *)(pBuf+1);

        while (i--)
        {
            sum += (*p++);
        }
    }

    return (uint8_t)(sum&0xff);
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_CheckFram68Valid
** 功能描述: 可变帧报文检查
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**         : DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_CKS 计算CKS
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static uint8_t DLT634_HMI_MASTER_CheckFram68Valid(uint8_t pdrv)
{
    uint8_t *pBuf = 0;
    
    pBuf = (DLT634_HMIMaster_App[pdrv].RxdBuf + DLT634_HMIMaster_App[pdrv].RxdHead);

	  // 报头校验
    if (pBuf[0] != pBuf[3])
    {
        return(0);
    }

	  // 公共地址校验
    {
        if (((pBuf[5]|(pBuf[6]<<8)) != DLT634_HMIMaster_Pad[pdrv].SourceAddr) && ((pBuf[5]|(pBuf[6]<<8)) != 0xffff))
        {
            return (0);
        }
    }
    
    return (1);
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_CheckFram10Valid
** 功能描述: 固定帧报文检查
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**         : DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_CKS 计算CKS
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static uint8_t DLT634_HMI_MASTER_CheckFram10Valid(uint8_t pdrv)
{
    uint8_t *pBuf = 0;
    uint16_t addr = 0;

    pBuf = &DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead];

	  // 固定帧长数据的校验和检测
    if (pBuf[2+2] != (DLT634_HMI_MASTER_CKS(pdrv, pBuf)&0xFF))	
    {
        return (0);
    }

	  // 固定帧长数据的结束帧检测
    if (pBuf[3+2] != _DLT634_HMIMASTER_ENDCODE)
    {
        return (0);
    }

	  // 固定帧数据公共地址检测
    {
        addr = pBuf[2] | ((pBuf[3]<<8));
    }
		
    if (addr != DLT634_HMIMaster_Pad[pdrv].SourceAddr)
    {
        return (0);
    }
    
    return (1);
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_DecodeFrame10
** 功能描述: 固定帧报文解析
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**         : DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: 
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_DecodeFrame10(uint8_t pdrv)
{
    uint8_t stop = 0;
    uint8_t control = 0;

    control = DLT634_HMIMaster_App[pdrv].RxdBuf[1];
    if (control & _DLT634_HMIMASTER_PRM) // 主站作为从动站接收主站的命令
    {
        switch (control & _DLT634_HMIMASTER_FUNCODE)
        {
            case _DLT634_HMIMASTER_M_FUN9: // 召唤链路状态
                DLT634_HMIMaster_App[pdrv].TimeOutTick_AskSta = DLT634_HMIMaster_Pad[pdrv].AskStaOutValue;
                DLT634_HMIMaster_App[pdrv].LinkFlag = 0;
                DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_REQSTATUS;
                /* 可以避免答非所问的过程，尽快建立链路 */ 
                while (((DLT634_HMIMaster_App[pdrv].RxdTail - DLT634_HMIMaster_App[pdrv].RxdHead) >= DLT634_HMIMaster_Pad[pdrv].FixFrmLength) && (stop == 0)) // 收到召唤链路状态命令时，清掉后续相同命令。
                {
                    if (memcmp(DLT634_HMIMaster_App[pdrv].RxdBuf, DLT634_HMIMaster_App[pdrv].RxdBuf + DLT634_HMIMaster_App[pdrv].RxdHead, DLT634_HMIMaster_Pad[pdrv].FixFrmLength) == 0)
                    {
                        DLT634_HMIMaster_App[pdrv].RxdHead += DLT634_HMIMaster_Pad[pdrv].FixFrmLength;
                    }
                    else
                    {
                        stop = 1;
                    }
                }
                break;

            case _DLT634_HMIMASTER_M_FUN0: // 复位链路
                DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_RECONFIRM;
								DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_INITEND;
								DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_SENDABLE;
								DLT634_HMIMaster_App[pdrv].DataFlag |= _DLT634_HMIMASTER_CALLALLDATA;
                DLT634_HMIMaster_App[pdrv].SendConCode = 0x20;
                memset(DLT634_HMIMaster_App[pdrv].StoreDATABuf,0,sizeof(DLT634_HMIMaster_App[pdrv].StoreDATABuf));
								DLT634_HMI_MASTER_H_HMIYX(pdrv);	
                break;
            default:
                break;
        }
        return;
    }
    else // Prm=0 平衡模式主站作为启动站接收到主站的命令。
    {
        switch (control & _DLT634_HMIMASTER_FUNCODE)
        {
            case _DLT634_HMIMASTER_S_FUN0:	// 确认
                DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_SENDABLE;
                DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri = 0; // 链路有数据就可以不重发
                break;

            case _DLT634_HMIMASTER_S_FUN11: // 应答链路
                DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri = 0; // 链路有数据就可以不重发
                DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_ASKRELINK;	
                break;

            default:
                break;
        }
    }
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_AppProcess
** 功能描述: 应用处理函数
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**         : DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_C_SC  向其他设备发送遥控
**         : DLT634_HMI_MASTER_C_CS  向其他设备发送定值
**         : DLT634_HMI_MASTER_W_YXDATA  写YXDATA
**         : DLT634_HMI_MASTER_W_SOE  写SOE
**         : DLT634_HMI_MASTER_W_YCDATA  写YCDATA
**         : DLT634_HMI_MASTER_W_FEvent  写FEvent
**         : DLT634_HMI_MASTER_StoreIN  发送缓冲区堆栈 
**         : DLT634_HMI_MASTER_F_FR  向其他设备发送文件传输
**         : DLT634_HMI_MASTER_F_SR  向其他设备发送软件升级
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_AppProcess(uint8_t pdrv,uint8_t Control)
{		
    //整理成02版报文，便于控制和传输
    memset((uint8_t *)&DLT634_HMIMaster_App[pdrv].RX_Temp,0,sizeof(DLT634_HMIMASTER_PASDU));
    DLT634_HMIMaster_App[pdrv].RX_Temp.status.Length = *(uint16_t *)&DLT634_HMIMaster_App[pdrv].RxdBuf[1] -1 -2;
    memcpy((uint8_t *)&DLT634_HMIMaster_App[pdrv].RX_Temp.buff,&DLT634_HMIMaster_App[pdrv].RxdBuf[5+2],DLT634_HMIMaster_App[pdrv].RX_Temp.status.Length);

    DLT634_HMIMaster_App[pdrv].RX_Temp.status.Length += sizeof(DLT634_HMIMaster_App[pdrv].RX_Temp.status);
    
    if (Control & _DLT634_HMIMASTER_PRM) // 主站作为从动站接收子站链路报文。
    {
			
			DLT634_HMI_MASTER_TO_HMI(pdrv,(uint8_t *)&DLT634_HMIMaster_App[pdrv].RX_Temp);

    }
    else  // Prm=0；主站作为启动站接收子站链路报文,需回复报文
    {
    }
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_DecodeFrame68
** 功能描述: 解析可变帧桢报文
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_AppProcess  应用处理函数
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_DecodeFrame68(uint8_t pdrv)
{
    uint8_t control;
    
    control = DLT634_HMIMaster_App[pdrv].RxdBuf[4];

    DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_RECONFIRM;
    DLT634_HMI_MASTER_AppProcess(pdrv,control);
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_SearchValidFrame
** 功能描述: 数据处理函数
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_CheckFram68Valid  检查可变帧报文
**         : DLT634_HMI_MASTER_CheckFram10Valid  检查固定帧报文
**         : DLT634_HMI_MASTER_DecodeFrame68  解析可变桢报文
**         : DLT634_HMI_MASTER_DecodeFrame10  解析固定桢报文
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_SearchValidFrame(uint8_t pdrv)
{
    uint8_t stop = 0;
    uint16_t length = 0;
    
    while ((DLT634_HMIMaster_App[pdrv].RxdHead < DLT634_HMIMaster_App[pdrv].RxdTail) && (!stop))
    {
        if (DLT634_HMIMaster_App[pdrv].RxdStatus == RXDSTART)
        {
            while ((DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead] != _DLT634_HMIMASTER_STARTCODE10)
                    && (DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead] != _DLT634_HMIMASTER_STARTCODE68)
                    && (DLT634_HMIMaster_App[pdrv].RxdHead < DLT634_HMIMaster_App[pdrv].RxdTail))
            {
                DLT634_HMIMaster_App[pdrv].RxdHead++;
            }

            if (DLT634_HMIMaster_App[pdrv].RxdHead < DLT634_HMIMaster_App[pdrv].RxdTail) // 找到启动字符，并将报文与缓冲区对齐。
            {
                DLT634_HMIMaster_App[pdrv].RxdStatus = RXDHEAD;
                if (DLT634_HMIMaster_App[pdrv].RxdHead != 0)
                {
                    memcpy(DLT634_HMIMaster_App[pdrv].RxdBuf,DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead,DLT634_HMIMaster_App[pdrv].RxdTail-DLT634_HMIMaster_App[pdrv].RxdHead);
                    DLT634_HMIMaster_App[pdrv].RxdTail -= DLT634_HMIMaster_App[pdrv].RxdHead;
                    DLT634_HMIMaster_App[pdrv].RxdHead = 0;
                }
            }
        }
        
        DLT634_HMIMaster_App[pdrv].RxdLength = DLT634_HMIMaster_App[pdrv].RxdTail - DLT634_HMIMaster_App[pdrv].RxdHead;
        if (DLT634_HMIMaster_App[pdrv].RxdStatus == RXDHEAD) // 报文够一帧则处理
        {
            switch (DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead])
            {
                case _DLT634_HMIMASTER_STARTCODE68:
                    if (DLT634_HMIMaster_App[pdrv].RxdLength >= 5+2) // 启动字符到链路地址共6字节。
                    {
                        if (!DLT634_HMI_MASTER_CheckFram68Valid(pdrv)) // 判断是否报文头
                        {
                            DLT634_HMIMaster_App[pdrv].RxdHead++;
                            DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            DLT634_HMIMaster_App[pdrv].RxdStatus = RXDCONTINUE;
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
                    
                case _DLT634_HMIMASTER_STARTCODE10:
                    if (DLT634_HMIMaster_App[pdrv].RxdLength >= 4 + 2)
                    {
                        if (!DLT634_HMI_MASTER_CheckFram10Valid(pdrv))
                        {
                            DLT634_HMIMaster_App[pdrv].RxdHead++;
                            DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            DLT634_HMIMaster_App[pdrv].RxdStatus = RXDCONTINUE;
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
            }
        }
        if (DLT634_HMIMaster_App[pdrv].RxdStatus == RXDCONTINUE)
        {
            switch (DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead])
            {
            case _DLT634_HMIMASTER_STARTCODE68:
                length = *(uint16_t *)&DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead+1];
                if (DLT634_HMIMaster_App[pdrv].RxdLength >= length+6) // 报文收全，进行处理
                {
                    if ((DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead+length+4] == (DLT634_HMI_MASTER_CKS(pdrv, DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead)&0xFF))//校验正确
                            && (DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdHead+length+4+1] == _DLT634_HMIMASTER_ENDCODE)) // 结束码正确
 					          {
                        if ( DLT634_HMIMaster_App[pdrv].RxdHead > 0 )
                        {
                            memcpy(DLT634_HMIMaster_App[pdrv].RxdBuf,DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead,DLT634_HMIMaster_App[pdrv].RxdTail-DLT634_HMIMaster_App[pdrv].RxdHead);//报文与缓冲区对齐
                            DLT634_HMIMaster_App[pdrv].RxdTail -= DLT634_HMIMaster_App[pdrv].RxdHead;
                            DLT634_HMIMaster_App[pdrv].RxdHead = 0;
                        }
												/* 头指针后移到报文后 */
                        DLT634_HMIMaster_App[pdrv].RxdHead += (length+6);
                        DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;

                        DLT634_HMI_MASTER_DecodeFrame68(pdrv);
                    }
                    else
                    {
                        DLT634_HMIMaster_App[pdrv].RxdHead += 6;
                        DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;
                    }
                }
                else // 报文未收全，不处理，继续等待数据。
                {
                    stop = 1;
                }
                break;
                
            case _DLT634_HMIMASTER_STARTCODE10:
                if (DLT634_HMIMaster_App[pdrv].RxdHead > 0)
                {
                    memcpy(DLT634_HMIMaster_App[pdrv].RxdBuf,DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead,DLT634_HMIMaster_App[pdrv].RxdTail-DLT634_HMIMaster_App[pdrv].RxdHead);
                    DLT634_HMIMaster_App[pdrv].RxdTail -= DLT634_HMIMaster_App[pdrv].RxdHead;
                    DLT634_HMIMaster_App[pdrv].RxdHead = 0;
                }
                DLT634_HMIMaster_App[pdrv].RxdHead += 4 + DLT634_HMIMaster_Pad[pdrv].LinkAddrSize;
                DLT634_HMIMaster_App[pdrv].RxdStatus = RXDSTART;
			
                DLT634_HMI_MASTER_DecodeFrame10(pdrv);
                break;
            }
        }
    }
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_ReadMISIData
** 功能描述: 读取函数
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_ReadData  读取函数
**         : DLT634_HMI_MASTER_SearchValidFrame  数据处理函数
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_ReadMISIData(uint8_t pdrv)
{
    uint16_t count = 0;
    
    if (DLT634_HMIMaster_App[pdrv].RxdHead < DLT634_HMIMaster_App[pdrv].RxdTail) // 接收缓冲区中有尚未处理的数据
    {
        if (DLT634_HMIMaster_App[pdrv].RxdHead != 0)
        {
            memcpy(DLT634_HMIMaster_App[pdrv].RxdBuf, DLT634_HMIMaster_App[pdrv].RxdBuf+DLT634_HMIMaster_App[pdrv].RxdHead, DLT634_HMIMaster_App[pdrv].RxdTail-DLT634_HMIMaster_App[pdrv].RxdHead);//将未处理的数据移到缓冲区头
            DLT634_HMIMaster_App[pdrv].RxdTail -= DLT634_HMIMaster_App[pdrv].RxdHead;
            DLT634_HMIMaster_App[pdrv].RxdHead = 0;
        }
    }
    else
    {
        DLT634_HMIMaster_App[pdrv].RxdHead = 0;
		    DLT634_HMIMaster_App[pdrv].RxdTail = 0;
    }
    
    count = DLT634_HMI_MASTER_ReadData(pdrv,(uint8_t *)&DLT634_HMIMaster_App[pdrv].RxdBuf[DLT634_HMIMaster_App[pdrv].RxdTail], _DLT634_HMIMASTER_LPDUSIZE);
    if (count > 0)
    {
        DLT634_HMIMaster_App[pdrv].RxdTail += count;
        DLT634_HMI_MASTER_SearchValidFrame(pdrv);
    }
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_WriteDataToMISI
** 功能描述: 发送函数
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_WriteData  发送函数
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static uint8_t DLT634_HMI_MASTER_WriteDataToMISI(uint8_t pdrv)
{
    uint16_t SendLen = 0;
   
    SendLen = DLT634_HMI_MASTER_WriteData(pdrv ,DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdHead, DLT634_HMIMaster_App[pdrv].TxdTail - DLT634_HMIMaster_App[pdrv].TxdHead);
	
    DLT634_HMIMaster_App[pdrv].TxdHead += SendLen;
    if (DLT634_HMIMaster_App[pdrv].TxdHead >= DLT634_HMIMaster_App[pdrv].TxdTail) // 该次任务数据已经发完
    {
        DLT634_HMIMaster_App[pdrv].TxdHead = 0;
        DLT634_HMIMaster_App[pdrv].TxdTail = 0;
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_SlaveFrame10Packet
** 功能描述: 固定帧从动站打包
** 输　  入: pdrv：设备序号
**         : Function  功能码
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_CKS  计算CKS
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_SlaveFrame10Packet(uint8_t pdrv, uint8_t Function)
{
    uint8_t *pBuf;
    uint8_t i = 0;
									
    pBuf = DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdTail;
    pBuf[0] = _DLT634_HMIMASTER_STARTCODE10;
    pBuf[1] = Function&0x0f;
    pBuf[1] |= DLT634_HMIMaster_Pad[pdrv].IEC_DIR; // 主站向子站传输

    for (i=0; i<DLT634_HMIMaster_Pad[pdrv].LinkAddrSize; i++)
    {
        pBuf[2+i] = (DLT634_HMIMaster_Pad[pdrv].SourceAddr>>(8*i)) & 0xff;
    }

    pBuf[2+DLT634_HMIMaster_Pad[pdrv].LinkAddrSize] = DLT634_HMI_MASTER_CKS(pdrv, pBuf);
    pBuf[3+DLT634_HMIMaster_Pad[pdrv].LinkAddrSize] = _DLT634_HMIMASTER_ENDCODE;
    DLT634_HMIMaster_App[pdrv].TxdTail += DLT634_HMIMaster_Pad[pdrv].FixFrmLength;//移动发送尾指针
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_MasterFrame10Packet
** 功能描述: 固定帧启动站打包
** 输　  入: pdrv：设备序号
**         : Function  功能码
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_CKS  计算CKS
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_MasterFrame10Packet(uint8_t pdrv, uint8_t Function)
{
    uint8_t *pBuf;
    uint8_t i = 0;

    pBuf = DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdTail;
    pBuf[0] = _DLT634_HMIMASTER_STARTCODE10;
    pBuf[1] = Function&0x0f;
    pBuf[1] |= _DLT634_HMIMASTER_PRM;
    pBuf[1] |= DLT634_HMIMaster_Pad[pdrv].IEC_DIR; // 主站向子站传输

    for (i=0; i<DLT634_HMIMaster_Pad[pdrv].LinkAddrSize; i++)
    {
        pBuf[2+i] = (DLT634_HMIMaster_Pad[pdrv].SourceAddr>>(8*i)) & 0xff;
    }

    pBuf[2+DLT634_HMIMaster_Pad[pdrv].LinkAddrSize] = DLT634_HMI_MASTER_CKS(pdrv, pBuf);
    pBuf[3+DLT634_HMIMaster_Pad[pdrv].LinkAddrSize] = _DLT634_HMIMASTER_ENDCODE;
    DLT634_HMIMaster_App[pdrv].TxdTail += DLT634_HMIMaster_Pad[pdrv].FixFrmLength;//移动发送尾指针

    memcpy(DLT634_HMIMaster_App[pdrv].TxdBuf_Pri, pBuf, DLT634_HMIMaster_Pad[pdrv].FixFrmLength); // 将发送数据保存到启动站重发数据区
    DLT634_HMIMaster_App[pdrv].TxdTail_Pri = DLT634_HMIMaster_Pad[pdrv].FixFrmLength;
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_Link10Process
** 功能描述: 固定帧发送处理
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_WriteDataToMISI  发送函数
**           DLT634_HMI_MASTER_SlaveFrame10Packet  固定帧从动站打包
**           DLT634_HMI_MASTER_MasterFrame10Packet  固定帧启动站打包
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_Link10Process(uint8_t pdrv)
{ 
	if(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_REQSTATUS)
	{
		DLT634_HMI_MASTER_SlaveFrame10Packet(pdrv, _DLT634_HMIMASTER_S_FUN11);
		DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
		DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_REQSTATUS);
		return;
	}
	
	if(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_RECONFIRM)
	{
		DLT634_HMI_MASTER_SlaveFrame10Packet(pdrv, _DLT634_HMIMASTER_S_FUN0);
		DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
		DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_RECONFIRM);
		return;
	}
	
	if((DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_ASKSTATUS)&&(!(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_INITEND)))
	{
		DLT634_HMI_MASTER_MasterFrame10Packet(pdrv, _DLT634_HMIMASTER_M_FUN9);
		DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
		DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_ASKSTATUS);
		DLT634_HMIMaster_App[pdrv].TimeOutTick_AskSta = DLT634_HMIMaster_Pad[pdrv].AskStaOutValue;
		return;
	}
	
	if(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_ASKRELINK)
	{
		DLT634_HMI_MASTER_MasterFrame10Packet(pdrv, _DLT634_HMIMASTER_M_FUN0);
		DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
		DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_ASKRELINK);
		return;
	}	
}
/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_Data1Process
** 功能描述: 1级数据发送处理函数
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_ReadAllDataProcess  总召函数
**           DLT634_HMI_MASTER_ReadTime  对时
**           DLT634_HMI_SLAVE_R_SOE  SOE读取函数
**           DLT634_HMI_MASTER_LinkTestProcess  测试命令
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_Data1Process(uint8_t pdrv)
{
    uint16_t length;
    
    length = *(uint16_t *)&DLT634_HMIMaster_App[pdrv].StoreDATABuf[0];
    
    if(length != 0)
    {
        memcpy((uint8_t *)&DLT634_HMIMaster_App[pdrv].TX_Temp, (uint8_t *)DLT634_HMIMaster_App[pdrv].StoreDATABuf, length);
        memset((uint8_t *)DLT634_HMIMaster_App[pdrv].StoreDATABuf,0,sizeof(DLT634_HMIMaster_App[pdrv].StoreDATABuf)); 
				DLT634_HMI_SLAVE_EMPTY(pdrv);
        return;        
    }
    
//    if((DLT634_HMI_MASTER_H_HMIYX(pdrv))||(DLT634_HMIMaster_App[pdrv].DataFlag&_DLT634_HMIMASTER_CALLALLDATA))
//    {
//        DLT634_HMI_MASTER_R_HMIYX(pdrv,(uint8_t *)&DLT634_HMIMaster_App[pdrv].TX_Temp);
//        DLT634_HMIMaster_App[pdrv].DataFlag &= ~_DLT634_HMIMASTER_CALLALLDATA;
//        return; 
//    }	
}
/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_MasterFrame68Packet
** 功能描述: 可变帧启动站数据打包
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_CKS  计算CKS
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_MasterFrame68Packet(uint8_t pdrv, uint8_t Function)
{
    uint16_t length = 0;
    uint8_t num = 0;
    uint8_t *pBuf;
    
    pBuf = DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdTail;

    pBuf[0] = pBuf[3] = _DLT634_HMIMASTER_STARTCODE68;
    pBuf[1] = pBuf[2] = DLT634_HMIMaster_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_HMIMaster_App[pdrv].TX_Temp.status)+ 1 + 2;
    pBuf[4] = Function&0x0f;
    if(DLT634_HMIMaster_App[pdrv].SendConCode&_DLT634_HMIMASTER_FCB)
    {DLT634_HMIMaster_App[pdrv].SendConCode = 0;}
    else
    {DLT634_HMIMaster_App[pdrv].SendConCode = _DLT634_HMIMASTER_FCB;}
    pBuf[4] |= _DLT634_HMIMASTER_FCV;
    pBuf[4] |= DLT634_HMIMaster_App[pdrv].SendConCode;
    pBuf[4] |= _DLT634_HMIMASTER_PRM;

    pBuf[5] = DLT634_HMIMaster_Pad[pdrv].SourceAddr&0xff;
    pBuf[6] = (DLT634_HMIMaster_Pad[pdrv].SourceAddr>>8)&0xff;
    
    num = 7;
    memcpy(&pBuf[num], &DLT634_HMIMaster_App[pdrv].TX_Temp.buff, DLT634_HMIMaster_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_HMIMaster_App[pdrv].TX_Temp.status));
    num += DLT634_HMIMaster_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_HMIMaster_App[pdrv].TX_Temp.status);
		
    length = num + 2; 
    pBuf[1] = (length - 6)&0xff;
    pBuf[2] = ((length - 6)>>8)&0xff;
    
    pBuf[length-2] = DLT634_HMI_MASTER_CKS(pdrv, pBuf)&0xFF;
    pBuf[length-1] = _DLT634_HMIMASTER_ENDCODE;
    DLT634_HMIMaster_App[pdrv].TxdTail += length;

    memcpy(DLT634_HMIMaster_App[pdrv].TxdBuf_Pri, pBuf, length);
    DLT634_HMIMaster_App[pdrv].TxdTail_Pri = length;
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_Link68Process
** 功能描述: 可变帧发送函数
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_MasterFrame68Packet  可变帧启动站打包
**           DLT634_HMI_MASTER_WriteDataToMISI  数据发送函数
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_Link68Process(uint8_t pdrv)
{
    if(DLT634_HMIMaster_App[pdrv].TX_Temp.status.Length)
    {
        DLT634_HMI_MASTER_MasterFrame68Packet(pdrv, _DLT634_HMIMASTER_M_FUN3);
        DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
        DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri = DLT634_HMIMaster_Pad[pdrv].TimeOutValue;
        DLT634_HMIMaster_App[pdrv].RetryCount  = _DLT634_HMIMASTER_NUMOF_MAXRETRY;
        DLT634_HMIMaster_App[pdrv].LinkFlag &= (~_DLT634_HMIMASTER_SENDABLE);
        memset((uint8_t *)&DLT634_HMIMaster_App[pdrv].TX_Temp,0,sizeof(DLT634_HMIMASTER_PASDU));
    }
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_SendProcess
** 功能描述: 发送处理区
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_Link10Process  固定帧发送数据函数
**           DLT634_HMI_MASTER_Data1Process  1级数据发送数据函数
**           DLT634_HMI_MASTER_Link68Process  可变帧发送函数
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
static void DLT634_HMI_MASTER_SendProcess(uint8_t pdrv)
{   	
    if(DLT634_HMIMaster_App[pdrv].LinkFlag&0x000000ff)
    {
        DLT634_HMI_MASTER_Link10Process(pdrv);
        return;
    }
    

    if((DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_SENDABLE)&&(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_INITEND))
    {
        DLT634_HMI_MASTER_Data1Process(pdrv);
        DLT634_HMI_MASTER_Link68Process(pdrv);
        return;
    }
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_LinkOnTimer
** 功能描述: 链路层定时器处理区
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_WriteDataToMISI  发送数据函数
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
void DLT634_HMI_MASTER_LinkOnTimer(uint8_t pdrv)
{		
    if (DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri) // 单位是秒
    {
        DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri--;
        if (!DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri)
        {
            if (!DLT634_HMIMaster_App[pdrv].RetryCount)
            {
                    return;
            }

            DLT634_HMIMaster_App[pdrv].RetryCount--;
            if (DLT634_HMIMaster_App[pdrv].RetryCount)
            {
                memcpy(DLT634_HMIMaster_App[pdrv].TxdBuf + DLT634_HMIMaster_App[pdrv].TxdTail, DLT634_HMIMaster_App[pdrv].TxdBuf_Pri, DLT634_HMIMaster_App[pdrv].TxdTail_Pri);
                DLT634_HMIMaster_App[pdrv].TxdTail += DLT634_HMIMaster_App[pdrv].TxdTail_Pri;

                DLT634_HMI_MASTER_WriteDataToMISI(pdrv);
                DLT634_HMIMaster_App[pdrv].TimeOutTick_Pri = DLT634_HMIMaster_Pad[pdrv].TimeOutValue;
                return;
            }
            else // 超过重发次数后，重新建立链路，并通知应用层。
            {
                DLT634_HMIMaster_App[pdrv].LinkFlag = 0;
            }
        }
    }	
    
    if(!(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_INITEND))
    {
        if (!DLT634_HMIMaster_App[pdrv].TimeOutTick_AskSta)
        {
            DLT634_HMIMaster_App[pdrv].LinkFlag |= _DLT634_HMIMASTER_ASKSTATUS;
        }
        else
        {
            DLT634_HMIMaster_App[pdrv].TimeOutTick_AskSta--;
        }
    }
    
    if(DLT634_HMIMaster_App[pdrv].LinkFlag&_DLT634_HMIMASTER_INITEND)
    {
        if (!DLT634_HMIMaster_App[pdrv].TimeOutTick_AllData)
        {
            DLT634_HMIMaster_App[pdrv].TimeOutTick_AllData = DLT634_HMIMaster_Pad[pdrv].AllDataOutValue;
            DLT634_HMIMaster_App[pdrv].DataFlag |= _DLT634_HMIMASTER_CALLALLDATA;
        }
        else
        {
            DLT634_HMIMaster_App[pdrv].TimeOutTick_AllData--;
        }
    }
}

/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_REPLY
** 功能描述: 其他设备应答处理
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**         : DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_StoreIN  发送缓冲区堆栈
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
uint8_t DLT634_HMI_MASTER_REPLY(uint8_t pdrv, uint8_t *pbuf)//回复
{ 
    uint16_t length;
    
    length = *(uint16_t *)&pbuf[0];
    
    memcpy(DLT634_HMIMaster_App[pdrv].StoreDATABuf,pbuf,length);
    
	return(TRUE);
}

/* -----------------------------------------------------------------------------
** 函数名称: DLT634_HMI_MASTER_Clock
** 功能描述: 定时处理函数
** 输　  入: pdrv：设备序号
** 输　  出: 无
** 全局变量: DLT634_HMIMaster_App[pdrv]：运行参数结构体
**           DLT634_HMIMaster_Pad[pdrv]：设定参数结构体
** 调用模块: DLT634_HMI_MASTER_ReadMISIData  查询MISI接收缓冲区
**           DLT634_HMI_MASTER_SendProcess  发送处理区
**           DLT634_HMI_MASTER_LinkOnTimer  链路层定时器处理区
** 作　  者: Mr.J
** 日　  期: 2017.8.3
** -----------------------------------------------------------------------------
** 修 改 人: 
** 修改内容:
** 日　  期: 
** ---------------------------------------------------------------------------*/
void DLT634_HMI_MASTER_Clock(uint8_t pdrv)
{
	DLT634_HMIMaster_App[pdrv].ClockCounter++;
	if (DLT634_HMIMaster_App[pdrv].ClockCounter >= DLT634_HMIMaster_Pad[pdrv].ClockTimers)
	{
        DLT634_HMIMaster_App[pdrv].ClockCounter = 0;
                                            
        // 查询MISI接收缓冲区
        DLT634_HMI_MASTER_ReadMISIData(pdrv);
    
        DLT634_HMI_MASTER_SendProcess(pdrv);
            
        // 链路层定时器
        DLT634_HMI_MASTER_LinkOnTimer(pdrv);
	}
}




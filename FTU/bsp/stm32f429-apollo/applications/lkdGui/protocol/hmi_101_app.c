/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_HMISLAVE_app.c
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include "string.h"
#include "common_data.h"
#include "hmi_101_app.h"
#include "hmi_101_disk.h"


/* PRIVATE VARIABLES ----------------------------------------------------------*/
/* PUBLIC VARIABLES ----------------------------------------------------------*/
DLT634_HMISLAVE_PAD      DLT634_HMISLAVE_Pad[_DLT634_HMISLAVE_VOLUMES];
DLT634_HMISLAVE_APPINFO  *DLT634_HMISLAVE_App = RT_NULL; //__attribute__((at(SDRAM_ADDR_101)));

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : sum of CKS.
  * @param : [pdrv]
  * @param : [pBuf]
  * @return: sum
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_HMI_SLAVE_CKS(uint8_t pdrv ,uint8_t *pBuf)
{
    uint16_t sum = 0;
    uint16_t j =0;
    uint8_t *p = 0;
    uint16_t i = 0;

    if (*(pBuf) == _DLT634_HMISLAVE_STARTCODE10)
    {
        for (j = 0; j < (2 + 1); j++)
        {
            sum += *(pBuf + 1 + j);
        }
    }
    else if(*(pBuf) == _DLT634_HMISLAVE_STARTCODE68)
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

/**
  * @brief : check iec 68.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_HMI_SLAVE_CheckFram68Valid(uint8_t pdrv)
{
    uint8_t *pBuf = 0;
    
    pBuf = (DLT634_HMISLAVE_App[pdrv].RxdBuf + DLT634_HMISLAVE_App[pdrv].RxdHead);

	  // 报头校验
    if (pBuf[0] != pBuf[3])
    {
        return(0);
    }

	  // 公共地址校验
    if (((pBuf[5]|(pBuf[6]<<8)) != DLT634_HMISLAVE_Pad[pdrv].SourceAddr) && ((pBuf[5]|(pBuf[6]<<8)) != 0xffff))
    {
        return (0);
    }
    
    return (1);
}

/**
  * @brief : check iec 10.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_HMI_SLAVE_CheckFram10Valid(uint8_t pdrv)
{
    uint8_t *pBuf = 0;

    pBuf = &DLT634_HMISLAVE_App[pdrv].RxdBuf[DLT634_HMISLAVE_App[pdrv].RxdHead];

	  // 固定帧长数据的校验和检测
    if (pBuf[2+2] != (DLT634_HMI_SLAVE_CKS(pdrv, pBuf)&0xFF))	
    {
        return (0);
    }

	  // 固定帧长数据的结束帧检测
    if (pBuf[3+2] != _DLT634_HMISLAVE_ENDCODE)
    {
        return (0);
    }

    // 固定帧数据公共地址检测 
    		
    if ((pBuf[2] | ((pBuf[3]<<8))) != DLT634_HMISLAVE_Pad[pdrv].SourceAddr)
    {
        return (0);
    }
    
    return (1);
}

/**
  * @brief : send to cache.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_SLAVE_StoreIN(uint8_t pdrv,DLT634_HMISLAVE_PASDU *buf)
{	
	return(TRUE);
}

/**
  * @brief : analysis.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_HMI_SLAVE_AppProcess(uint8_t pdrv)
{		
	//整理成02版报文，便于控制和传输
	memset((uint8_t *)&DLT634_HMISLAVE_App[pdrv].RX_Temp,0,sizeof(DLT634_HMISLAVE_PASDU));
	DLT634_HMISLAVE_App[pdrv].RX_Temp.status.Length = *(uint16_t *)&DLT634_HMISLAVE_App[pdrv].RxdBuf[1] -1 -2;
	memcpy((uint8_t *)&DLT634_HMISLAVE_App[pdrv].RX_Temp.buff,&DLT634_HMISLAVE_App[pdrv].RxdBuf[5+2],DLT634_HMISLAVE_App[pdrv].RX_Temp.status.Length);

	DLT634_HMISLAVE_App[pdrv].RX_Temp.status.Length += sizeof(DLT634_HMISLAVE_App[pdrv].RX_Temp.status);
	
	DLT634_HMI_SLAVE_TO_HMI(pdrv,(uint8_t *)&DLT634_HMISLAVE_App[pdrv].RX_Temp);
}

/**
  * @brief : write data to misi.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_HMI_SLAVE_WriteDataToMISI(uint8_t pdrv)
{
    uint16_t sendlen = 0;
   
    sendlen = DLT634_HMI_SLAVE_WriteData(pdrv ,DLT634_HMISLAVE_App[pdrv].TxdBuf + DLT634_HMISLAVE_App[pdrv].TxdHead, DLT634_HMISLAVE_App[pdrv].TxdTail - DLT634_HMISLAVE_App[pdrv].TxdHead);
    
    DLT634_HMISLAVE_App[pdrv].TxdHead += sendlen;
    if (DLT634_HMISLAVE_App[pdrv].TxdHead >= DLT634_HMISLAVE_App[pdrv].TxdTail) // 该次任务数据已经发完
    {
        DLT634_HMISLAVE_App[pdrv].TxdHead = 0;
        DLT634_HMISLAVE_App[pdrv].TxdTail = 0;
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/**
  * @brief : 68 analysis.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_DecodeFrame68(uint8_t pdrv)
{
    uint8_t control;
    
    control = DLT634_HMISLAVE_App[pdrv].RxdBuf[4];

    if (control & _DLT634_HMISLAVE_PRM) // 子站作为从动站接收主站链路报文。
    {
        if ((!(control&_DLT634_HMISLAVE_FCV)) || ((control&_DLT634_HMISLAVE_FCB) != ((DLT634_HMISLAVE_App[pdrv].RlaConCode) & 0x20)) || (!(DLT634_HMISLAVE_App[pdrv].LinkFlag & _DLT634_HMISLAVE_FIRST_RlaConCode)))
        {
            if(DLT634_HMISLAVE_App[pdrv].LinkFlag & _DLT634_HMISLAVE_INITEND)
            {
                if (control & _DLT634_HMISLAVE_FCV) // FCV有效而且FCB变位
                {
                    DLT634_HMISLAVE_App[pdrv].RlaConCode = DLT634_HMISLAVE_App[pdrv].RxdBuf[4]; // 保存最新的功能码，这里只用到FCB
                    DLT634_HMISLAVE_App[pdrv].FCBNoTurnNum = _DLT634_HMISLAVE_NUMOF_MAXRETRY; // 设置重发次数，指下次收到主站报文FCB未翻转时重发缓冲区内的报文。
                    DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_FIRST_RlaConCode;
                }
                DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_RECONFIRM;
                DLT634_HMI_SLAVE_AppProcess(pdrv);
            }
        }
        else  // FCB 未翻转
        {
            DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_RECONFIRM;
            return;
        }
    }
    else  // Prm=0；子站作为启动站接收主站链路报文
    {
    }
}

/**
  * @brief : status reset.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_StatusReset(uint8_t pdrv)
{
    DLT634_HMISLAVE_App[pdrv].LinkFlag = 0;
    DLT634_HMISLAVE_App[pdrv].DataFlag = 0;
	
		DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_INITEND;//初始化结束标志
		DLT634_HMISLAVE_App[pdrv].DataFlag |= _DLT634_HMISLAVE_CALLALLDATA;
}

/**
  * @brief : 10 analysis.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_DecodeFrame10(uint8_t pdrv)
{
    uint8_t stop = 0;
    uint8_t control = 0;

    control = DLT634_HMISLAVE_App[pdrv].RxdBuf[1];
    if (control & _DLT634_HMISLAVE_PRM) // 子站作为从动站接收主站的命令
    {
        if ((!(control&_DLT634_HMISLAVE_FCV)) || ((control&_DLT634_HMISLAVE_FCB) != ((DLT634_HMISLAVE_App[pdrv].RlaConCode) & 0x20)) || (!(DLT634_HMISLAVE_App[pdrv].LinkFlag & _DLT634_HMISLAVE_FIRST_RlaConCode)))
        {
            if (control & _DLT634_HMISLAVE_FCV) // FCV有效，FCB翻转
            {
                DLT634_HMISLAVE_App[pdrv].RlaConCode = DLT634_HMISLAVE_App[pdrv].RxdBuf[1];
                DLT634_HMISLAVE_App[pdrv].FCBNoTurnNum = _DLT634_HMISLAVE_NUMOF_MAXRETRY;
								DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_FIRST_RlaConCode;
            }
            switch (control & _DLT634_HMISLAVE_FUNCODE)
            {
                case _DLT634_HMISLAVE_M_FUN2: // 心跳测试
                    DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_RECONFIRM;
                    break;
										
                case _DLT634_HMISLAVE_M_FUN9: // 召唤链路状态
					DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_REQSTATUS;
								    /* 可以避免答非所问的过程，尽快建立链路 */ 
                    while (((DLT634_HMISLAVE_App[pdrv].RxdTail - DLT634_HMISLAVE_App[pdrv].RxdHead) >= DLT634_HMISLAVE_Pad[pdrv].FixFrmLength) && (stop == 0)) // 收到召唤链路状态命令时，清掉后续相同命令。
                    {
                        if (memcmp(DLT634_HMISLAVE_App[pdrv].RxdBuf, DLT634_HMISLAVE_App[pdrv].RxdBuf + DLT634_HMISLAVE_App[pdrv].RxdHead, DLT634_HMISLAVE_Pad[pdrv].FixFrmLength) == 0)
                        {
                            DLT634_HMISLAVE_App[pdrv].RxdHead += DLT634_HMISLAVE_Pad[pdrv].FixFrmLength;
                        }
                        else
                        {
                            stop = 1;
                        }
                    }
                    break;

                case _DLT634_HMISLAVE_M_FUN0: // 复位链路					
                    DLT634_HMI_SLAVE_StatusReset(pdrv);
            
                    DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_RECONFIRM;
                    DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_ASKSTATUS;
                    break;

                default:
                    break;
            }
            return;
        }
        else // FCB未翻转
        {
            if (DLT634_HMISLAVE_App[pdrv].FCBNoTurnNum)
            {
                DLT634_HMISLAVE_App[pdrv].FCBNoTurnNum--;
                memcpy(DLT634_HMISLAVE_App[pdrv].TxdBuf + DLT634_HMISLAVE_App[pdrv].TxdTail, DLT634_HMISLAVE_App[pdrv].TxdBuf_Pri, DLT634_HMISLAVE_App[pdrv].TxdTail_Pri);
                DLT634_HMISLAVE_App[pdrv].TxdTail += DLT634_HMISLAVE_App[pdrv].TxdTail_Pri;
                DLT634_HMI_SLAVE_WriteDataToMISI(pdrv);
            }
        }
    }
    else // Prm=0 平衡模式子站作为启动站接收到主站的命令。
    {
        switch (control & _DLT634_HMISLAVE_FUNCODE)
        {
            case _DLT634_HMISLAVE_S_FUN0:	// 确认
                DLT634_HMISLAVE_App[pdrv].TimeOutTick_Pri = 0; // 链路有数据就可以不重发
                DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_SENDABLE;
                break;

            case _DLT634_HMISLAVE_S_FUN11: // 应答链路
                DLT634_HMI_SLAVE_StatusReset(pdrv);
        
                DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_ASKRELINK;
                break;

            default:
                break;
        }
    }
}

/**
  * @brief : Search Valid Frame.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_HMI_SLAVE_SearchValidFrame(uint8_t pdrv)
{
    uint8_t stop = 0;
    uint16_t length = 0;
    
    while ((DLT634_HMISLAVE_App[pdrv].RxdHead < DLT634_HMISLAVE_App[pdrv].RxdTail) && (!stop))
    {
        if (DLT634_HMISLAVE_App[pdrv].RxdStatus == RXDSTART)
        {
            while ((DLT634_HMISLAVE_App[pdrv].RxdBuf[DLT634_HMISLAVE_App[pdrv].RxdHead] != _DLT634_HMISLAVE_STARTCODE10)
                    && (DLT634_HMISLAVE_App[pdrv].RxdBuf[DLT634_HMISLAVE_App[pdrv].RxdHead] != _DLT634_HMISLAVE_STARTCODE68)
                    && (DLT634_HMISLAVE_App[pdrv].RxdHead < DLT634_HMISLAVE_App[pdrv].RxdTail))
            {
                DLT634_HMISLAVE_App[pdrv].RxdHead++;
            }

            if (DLT634_HMISLAVE_App[pdrv].RxdHead < DLT634_HMISLAVE_App[pdrv].RxdTail) // 找到启动字符，并将报文与缓冲区对齐。
            {
                DLT634_HMISLAVE_App[pdrv].RxdStatus = RXDHEAD;
                if (DLT634_HMISLAVE_App[pdrv].RxdHead != 0)
                {
                    memcpy(DLT634_HMISLAVE_App[pdrv].RxdBuf,DLT634_HMISLAVE_App[pdrv].RxdBuf+DLT634_HMISLAVE_App[pdrv].RxdHead,DLT634_HMISLAVE_App[pdrv].RxdTail-DLT634_HMISLAVE_App[pdrv].RxdHead);
                    DLT634_HMISLAVE_App[pdrv].RxdTail -= DLT634_HMISLAVE_App[pdrv].RxdHead;
                    DLT634_HMISLAVE_App[pdrv].RxdHead = 0;
                }
            }
        }
        
        DLT634_HMISLAVE_App[pdrv].RxdLength = DLT634_HMISLAVE_App[pdrv].RxdTail - DLT634_HMISLAVE_App[pdrv].RxdHead;
        if (DLT634_HMISLAVE_App[pdrv].RxdStatus == RXDHEAD) // 报文够一帧则处理
        {
            switch (DLT634_HMISLAVE_App[pdrv].RxdBuf[DLT634_HMISLAVE_App[pdrv].RxdHead])
            {
                case _DLT634_HMISLAVE_STARTCODE68:
                    if (DLT634_HMISLAVE_App[pdrv].RxdLength >= 5+2) // 启动字符到链路地址共6字节。
                    {
                        if (!DLT634_HMI_SLAVE_CheckFram68Valid(pdrv)) // 判断是否报文头
                        {
                            DLT634_HMISLAVE_App[pdrv].RxdHead++;
                            DLT634_HMISLAVE_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            DLT634_HMISLAVE_App[pdrv].RxdStatus = RXDCONTINUE;
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
                    
                case _DLT634_HMISLAVE_STARTCODE10:
                    if (DLT634_HMISLAVE_App[pdrv].RxdLength >= 4 + 2)
                    {
                        if (!DLT634_HMI_SLAVE_CheckFram10Valid(pdrv))
                        {
                            DLT634_HMISLAVE_App[pdrv].RxdHead++;
                            DLT634_HMISLAVE_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            DLT634_HMISLAVE_App[pdrv].RxdStatus = RXDCONTINUE;
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
            }
        }
        if (DLT634_HMISLAVE_App[pdrv].RxdStatus == RXDCONTINUE)
        {
            switch (DLT634_HMISLAVE_App[pdrv].RxdBuf[DLT634_HMISLAVE_App[pdrv].RxdHead])
            {
            case _DLT634_HMISLAVE_STARTCODE68:
                length = *(uint16_t *)&DLT634_HMISLAVE_App[pdrv].RxdBuf[DLT634_HMISLAVE_App[pdrv].RxdHead+1];
                if (DLT634_HMISLAVE_App[pdrv].RxdLength >= length+6) // 报文收全，进行处理
                {
                    if ((DLT634_HMISLAVE_App[pdrv].RxdBuf[DLT634_HMISLAVE_App[pdrv].RxdHead+length+4] == (DLT634_HMI_SLAVE_CKS(pdrv, DLT634_HMISLAVE_App[pdrv].RxdBuf+DLT634_HMISLAVE_App[pdrv].RxdHead)&0xFF))//校验正确
                            && (DLT634_HMISLAVE_App[pdrv].RxdBuf[DLT634_HMISLAVE_App[pdrv].RxdHead+length+4+1] == _DLT634_HMISLAVE_ENDCODE)) // 结束码正确
 					          {
                        if ( DLT634_HMISLAVE_App[pdrv].RxdHead > 0 )
                        {
                            memcpy(DLT634_HMISLAVE_App[pdrv].RxdBuf,DLT634_HMISLAVE_App[pdrv].RxdBuf+DLT634_HMISLAVE_App[pdrv].RxdHead,DLT634_HMISLAVE_App[pdrv].RxdTail-DLT634_HMISLAVE_App[pdrv].RxdHead);//报文与缓冲区对齐
                            DLT634_HMISLAVE_App[pdrv].RxdTail -= DLT634_HMISLAVE_App[pdrv].RxdHead;
                            DLT634_HMISLAVE_App[pdrv].RxdHead = 0;
                        }
												/* 头指针后移到报文后 */
                        DLT634_HMISLAVE_App[pdrv].RxdHead += (length+6);
                        DLT634_HMISLAVE_App[pdrv].RxdStatus = RXDSTART;

                        DLT634_HMI_SLAVE_DecodeFrame68(pdrv);
                    }
                    else
                    {
                        DLT634_HMISLAVE_App[pdrv].RxdHead += 6;
                        DLT634_HMISLAVE_App[pdrv].RxdStatus = RXDSTART;
                    }
                }
                else // 报文未收全，不处理，继续等待数据。
                {
                    stop = 1;
                }
                break;
                
            case _DLT634_HMISLAVE_STARTCODE10:
                if (DLT634_HMISLAVE_App[pdrv].RxdHead > 0)
                {
                    memcpy(DLT634_HMISLAVE_App[pdrv].RxdBuf,DLT634_HMISLAVE_App[pdrv].RxdBuf+DLT634_HMISLAVE_App[pdrv].RxdHead,DLT634_HMISLAVE_App[pdrv].RxdTail-DLT634_HMISLAVE_App[pdrv].RxdHead);
                    DLT634_HMISLAVE_App[pdrv].RxdTail -= DLT634_HMISLAVE_App[pdrv].RxdHead;
                    DLT634_HMISLAVE_App[pdrv].RxdHead = 0;
                }
                DLT634_HMISLAVE_App[pdrv].RxdHead += 4 + 2;
                DLT634_HMISLAVE_App[pdrv].RxdStatus = RXDSTART;
			
                DLT634_HMI_SLAVE_DecodeFrame10(pdrv);
                break;
            }
        }
    }
}

/**
  * @brief : read MISI Data.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_ReadMISIData(uint8_t pdrv)
{
    uint16_t count = 0;
    
    if (DLT634_HMISLAVE_App[pdrv].RxdHead < DLT634_HMISLAVE_App[pdrv].RxdTail) // 接收缓冲区中有尚未处理的数据
    {
        if (DLT634_HMISLAVE_App[pdrv].RxdHead != 0)
        {
            memcpy(DLT634_HMISLAVE_App[pdrv].RxdBuf, DLT634_HMISLAVE_App[pdrv].RxdBuf+DLT634_HMISLAVE_App[pdrv].RxdHead, DLT634_HMISLAVE_App[pdrv].RxdTail-DLT634_HMISLAVE_App[pdrv].RxdHead);//将未处理的数据移到缓冲区头
            DLT634_HMISLAVE_App[pdrv].RxdTail -= DLT634_HMISLAVE_App[pdrv].RxdHead;
            DLT634_HMISLAVE_App[pdrv].RxdHead = 0;
        }
    }
    else
    {
        DLT634_HMISLAVE_App[pdrv].RxdHead = 0;
		    DLT634_HMISLAVE_App[pdrv].RxdTail = 0;
    }
    
    count = DLT634_HMI_SLAVE_ReadData(pdrv,(uint8_t *)&DLT634_HMISLAVE_App[pdrv].RxdBuf[DLT634_HMISLAVE_App[pdrv].RxdTail], _DLT634_HMISLAVE_LPDUSIZE);
    if (count > 0)
    {
        DLT634_HMISLAVE_App[pdrv].RxdTail += count;
        DLT634_HMI_SLAVE_SearchValidFrame(pdrv);
    }
}

/**
  * @brief : Slave Frame 10 Packet.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_SlaveFrame10Packet(uint8_t pdrv, uint8_t Function)
{
    uint8_t *pBuf;
    uint8_t i = 0;
									
    pBuf = DLT634_HMISLAVE_App[pdrv].TxdBuf + DLT634_HMISLAVE_App[pdrv].TxdTail;
    pBuf[0] = _DLT634_HMISLAVE_STARTCODE10;
    pBuf[1] = Function&0x0f;

    
    pBuf[1] |= DLT634_HMISLAVE_Pad[pdrv].IEC_DIR; // 子站向主站传输，平衡模式为1，非平衡模式为0

    for (i=0; i<2; i++)
    {
        pBuf[2+i] = (DLT634_HMISLAVE_Pad[pdrv].SourceAddr>>(8*i)) & 0xff;
    }

    pBuf[2+2] = DLT634_HMI_SLAVE_CKS(pdrv, pBuf);
    pBuf[3+2] = _DLT634_HMISLAVE_ENDCODE;
    DLT634_HMISLAVE_App[pdrv].TxdTail += DLT634_HMISLAVE_Pad[pdrv].FixFrmLength;//移动发送尾指针
}

/**
  * @brief : Master Frame 10 Packet.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_MasterFrame10Packet(uint8_t pdrv, uint8_t Function)
{
    uint8_t *pBuf;
    uint8_t i = 0;

    pBuf = DLT634_HMISLAVE_App[pdrv].TxdBuf + DLT634_HMISLAVE_App[pdrv].TxdTail;
    pBuf[0] = _DLT634_HMISLAVE_STARTCODE10;
    pBuf[1] = Function&0x0f;
    pBuf[1] |= _DLT634_HMISLAVE_PRM;
    pBuf[1] |= DLT634_HMISLAVE_Pad[pdrv].IEC_DIR; // 子站向主站传输，平衡模式为1，非平衡模式为0

    for (i=0; i<2; i++)
    {
        pBuf[2+i] = (DLT634_HMISLAVE_Pad[pdrv].SourceAddr>>(8*i)) & 0xff;
    }

    pBuf[2+2] = DLT634_HMI_SLAVE_CKS(pdrv, pBuf);
    pBuf[3+2] = _DLT634_HMISLAVE_ENDCODE;
    DLT634_HMISLAVE_App[pdrv].TxdTail += DLT634_HMISLAVE_Pad[pdrv].FixFrmLength;//移动发送尾指针

    memcpy(DLT634_HMISLAVE_App[pdrv].TxdBuf_Pri, pBuf, DLT634_HMISLAVE_Pad[pdrv].FixFrmLength); // 将发送数据保存到启动站重发数据区
    DLT634_HMISLAVE_App[pdrv].TxdTail_Pri = DLT634_HMISLAVE_Pad[pdrv].FixFrmLength;
}

/**
  * @brief : Link 10 Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_Link10Process(uint8_t pdrv)
{ 
	if(DLT634_HMISLAVE_App[pdrv].LinkFlag&_DLT634_HMISLAVE_REQSTATUS)
	{
		DLT634_HMI_SLAVE_SlaveFrame10Packet(pdrv, _DLT634_HMISLAVE_S_FUN11);
		DLT634_HMI_SLAVE_WriteDataToMISI(pdrv);
		DLT634_HMISLAVE_App[pdrv].LinkFlag &= (~_DLT634_HMISLAVE_REQSTATUS);
		return;
	}
	
	if(DLT634_HMISLAVE_App[pdrv].LinkFlag&_DLT634_HMISLAVE_RECONFIRM)
	{
		DLT634_HMI_SLAVE_SlaveFrame10Packet(pdrv, _DLT634_HMISLAVE_S_FUN0);
		DLT634_HMI_SLAVE_WriteDataToMISI(pdrv);
		DLT634_HMISLAVE_App[pdrv].LinkFlag &= (~_DLT634_HMISLAVE_RECONFIRM);
		return;
	}
	
	if(DLT634_HMISLAVE_App[pdrv].LinkFlag&_DLT634_HMISLAVE_ASKSTATUS)
	{
		DLT634_HMI_SLAVE_MasterFrame10Packet(pdrv, _DLT634_HMISLAVE_M_FUN9);
		DLT634_HMI_SLAVE_WriteDataToMISI(pdrv);
		DLT634_HMISLAVE_App[pdrv].LinkFlag &= (~_DLT634_HMISLAVE_ASKSTATUS);
		DLT634_HMISLAVE_App[pdrv].TimeOutTick_AskSta = DLT634_HMISLAVE_Pad[pdrv].AskStaOutValue;
		return;
	}
	if(DLT634_HMISLAVE_App[pdrv].LinkFlag&_DLT634_HMISLAVE_ASKRELINK)
	{
		DLT634_HMI_SLAVE_MasterFrame10Packet(pdrv, _DLT634_HMISLAVE_M_FUN0);
		DLT634_HMI_SLAVE_WriteDataToMISI(pdrv);
		DLT634_HMISLAVE_App[pdrv].LinkFlag &= (~_DLT634_HMISLAVE_ASKRELINK);
		return;
	}	
}

/**
  * @brief : Data1 Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_DataProcess(uint8_t pdrv)
{ 
    uint16_t length;
    
    length = *(uint16_t *)&DLT634_HMISLAVE_App[pdrv].StoreDATABuf[0];
    
    if(length != 0)
    {
        memcpy((uint8_t *)&DLT634_HMISLAVE_App[pdrv].TX_Temp, (uint8_t *)DLT634_HMISLAVE_App[pdrv].StoreDATABuf, length);
        memset((uint8_t *)DLT634_HMISLAVE_App[pdrv].StoreDATABuf,0,sizeof(DLT634_HMISLAVE_App[pdrv].StoreDATABuf)); 
			  DLT634_HMI_SLAVE_EMPTY(pdrv);
        return;        
    }
    
//    if((DLT634_HMI_SLAVE_H_HMIYX(pdrv))||(DLT634_HMISLAVE_App[pdrv].DataFlag&_DLT634_HMISLAVE_CALLALLDATA))
//    {
//        DLT634_HMI_SLAVE_R_HMIYX(pdrv,(uint8_t *)&DLT634_HMISLAVE_App[pdrv].TX_Temp);
//        DLT634_HMISLAVE_App[pdrv].DataFlag &=  ~_DLT634_HMISLAVE_CALLALLDATA;
//        return; 
//    }
}

/**
  * @brief : Master Frame 68 Packet.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_MasterFrame68Packet(uint8_t pdrv, uint8_t Function)
{
    uint16_t length = 0;
    uint8_t *pBuf;
    uint16_t num;
    
    pBuf = DLT634_HMISLAVE_App[pdrv].TxdBuf + DLT634_HMISLAVE_App[pdrv].TxdTail;

    pBuf[0] = pBuf[3] = _DLT634_HMISLAVE_STARTCODE68;
    pBuf[4] = Function&0x0f;
    pBuf[4] |= _DLT634_HMISLAVE_DIR;
    pBuf[4] |= _DLT634_HMISLAVE_PRM;
    
    pBuf[5] = DLT634_HMISLAVE_Pad[pdrv].SourceAddr&0xff;
    pBuf[6] = (DLT634_HMISLAVE_Pad[pdrv].SourceAddr>>8)&0xff;
    
    num = 7;
    memcpy(&pBuf[num], &DLT634_HMISLAVE_App[pdrv].TX_Temp.buff, DLT634_HMISLAVE_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_HMISLAVE_App[pdrv].TX_Temp.status));
    num += DLT634_HMISLAVE_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_HMISLAVE_App[pdrv].TX_Temp.status);
		
    length = num + 2; // 全部报文长度	
    pBuf[1] = (length - 6)&0xff;
    pBuf[2] = ((length - 6)>>8)&0xff;

    pBuf[length-2] = DLT634_HMI_SLAVE_CKS(pdrv, pBuf)&0xFF;
    pBuf[length-1] = _DLT634_HMISLAVE_ENDCODE;
    DLT634_HMISLAVE_App[pdrv].TxdTail += length;

    memcpy(DLT634_HMISLAVE_App[pdrv].TxdBuf_Pri, pBuf, length);
    DLT634_HMISLAVE_App[pdrv].TxdTail_Pri = length;
}

/**
  * @brief : Link 68 Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_Link68Process(uint8_t pdrv)
{
    if(DLT634_HMISLAVE_App[pdrv].TX_Temp.status.Length)
    {
        DLT634_HMI_SLAVE_MasterFrame68Packet(pdrv, _DLT634_HMISLAVE_M_FUN3);
        DLT634_HMI_SLAVE_WriteDataToMISI(pdrv);
        DLT634_HMISLAVE_App[pdrv].TimeOutTick_Pri = DLT634_HMISLAVE_Pad[pdrv].TimeOutValue;
        DLT634_HMISLAVE_App[pdrv].RetryCount  = _DLT634_HMISLAVE_NUMOF_MAXRETRY;
        DLT634_HMISLAVE_App[pdrv].LinkFlag &= (~_DLT634_HMISLAVE_SENDABLE);
        memset((uint8_t *)&DLT634_HMISLAVE_App[pdrv].TX_Temp,0,sizeof(DLT634_HMISLAVE_PASDU));
    }
}

/**
  * @brief : Send Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_HMI_SLAVE_SendProcess(uint8_t pdrv)
{   
	
    if(DLT634_HMISLAVE_App[pdrv].LinkFlag&0x000000ff)
    {
        DLT634_HMI_SLAVE_Link10Process(pdrv);
        return;
    }
    
    if(DLT634_HMISLAVE_App[pdrv].LinkFlag&_DLT634_HMISLAVE_SENDABLE)
    {
        DLT634_HMI_SLAVE_DataProcess(pdrv);

        DLT634_HMI_SLAVE_Link68Process(pdrv);
        return;
    }
}

/**
  * @brief : Link On Timer.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_HMI_SLAVE_LinkOnTimer(uint8_t pdrv)
{
    if (DLT634_HMISLAVE_App[pdrv].TimeOutTick_Pri) // 单位是秒
    {
        DLT634_HMISLAVE_App[pdrv].TimeOutTick_Pri--;
        if (!DLT634_HMISLAVE_App[pdrv].TimeOutTick_Pri)
        {
            if (!DLT634_HMISLAVE_App[pdrv].RetryCount)
            {
                return;
            }

            DLT634_HMISLAVE_App[pdrv].RetryCount--;
            if (DLT634_HMISLAVE_App[pdrv].RetryCount)
            {
                memcpy(DLT634_HMISLAVE_App[pdrv].TxdBuf + DLT634_HMISLAVE_App[pdrv].TxdTail, DLT634_HMISLAVE_App[pdrv].TxdBuf_Pri, DLT634_HMISLAVE_App[pdrv].TxdTail_Pri);
                DLT634_HMISLAVE_App[pdrv].TxdTail += DLT634_HMISLAVE_App[pdrv].TxdTail_Pri;

                DLT634_HMI_SLAVE_WriteDataToMISI(pdrv);
                DLT634_HMISLAVE_App[pdrv].TimeOutTick_Pri = DLT634_HMISLAVE_Pad[pdrv].TimeOutValue;
                return;
            }
            else // 超过重发次数后，重新建立链路，并通知应用层。
            {
                DLT634_HMISLAVE_App[pdrv].LinkFlag = 0;
                DLT634_HMISLAVE_App[pdrv].TimeOutTick_AskCount = _DLT634_HMISLAVE_NUMOF_MAXRETRY;
                DLT634_HMISLAVE_App[pdrv].TimeOutTick_AskSta = DLT634_HMISLAVE_Pad[pdrv].AskStaOutValue;
            }
        }
    }
		
    if(DLT634_HMISLAVE_App[pdrv].TimeOutTick_AskSta)
    {
        DLT634_HMISLAVE_App[pdrv].TimeOutTick_AskSta--;
        if (!DLT634_HMISLAVE_App[pdrv].TimeOutTick_AskSta)
        {
            if (!DLT634_HMISLAVE_App[pdrv].TimeOutTick_AskCount)
            {
                return;
            }
            if(!(DLT634_HMISLAVE_App[pdrv].LinkFlag&_DLT634_HMISLAVE_INITEND))
            {
                DLT634_HMISLAVE_App[pdrv].LinkFlag |= _DLT634_HMISLAVE_ASKSTATUS;
                DLT634_HMISLAVE_App[pdrv].TimeOutTick_AskCount--;
            }
        }
    }
    
    if(DLT634_HMISLAVE_App[pdrv].LinkFlag&_DLT634_HMISLAVE_INITEND)
    {
        if (!DLT634_HMISLAVE_App[pdrv].TimeOutTick_AllData)
        {
            DLT634_HMISLAVE_App[pdrv].TimeOutTick_AllData = DLT634_HMISLAVE_Pad[pdrv].AllDataOutValue;
            DLT634_HMISLAVE_App[pdrv].DataFlag |= _DLT634_HMISLAVE_CALLALLDATA;
        }
        else
        {
            DLT634_HMISLAVE_App[pdrv].TimeOutTick_AllData--;
        }
    }
}
/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : SLAVE_REPLY.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_SLAVE_REPLY(uint8_t pdrv, uint8_t *pbuf)//回复
{
    uint16_t length;
    
    length = *(uint16_t *)&pbuf[0];
    
    memcpy(DLT634_HMISLAVE_App[pdrv].StoreDATABuf,pbuf,length);
    	
	return(TRUE);
}

/**
  * @brief : App Init.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_SLAVE_AppInit(uint8_t pdrv)
{
	if (DLT634_HMISLAVE_App == RT_NULL)
	{
		DLT634_HMISLAVE_App = rt_malloc(sizeof(DLT634_HMISLAVE_APPINFO) * _DLT634_HMISLAVE_VOLUMES);
        memset (DLT634_HMISLAVE_App, 0, sizeof (DLT634_HMISLAVE_APPINFO) * _DLT634_HMISLAVE_VOLUMES);
	}
	  
    return(TRUE);
}

/**
  * @brief : Clock work.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_HMI_SLAVE_Clock(uint8_t pdrv)
{
    uint8_t sta = FALSE;
    
	DLT634_HMISLAVE_App[pdrv].ClockCounter++;
	if (DLT634_HMISLAVE_App[pdrv].ClockCounter >= DLT634_HMISLAVE_Pad[pdrv].ClockTimers)
	{
        DLT634_HMISLAVE_App[pdrv].ClockCounter = 0;
                                            
        // 查询MISI接收缓冲区
        DLT634_HMI_SLAVE_ReadMISIData(pdrv);
    
        // 发送处理
        DLT634_HMI_SLAVE_SendProcess(pdrv);
            
        // 链路层定时器
        DLT634_HMI_SLAVE_LinkOnTimer(pdrv);
	}
    
        // 返回链路状态
    if(DLT634_HMISLAVE_App[pdrv].LinkFlag&_DLT634_HMISLAVE_INITEND)
    {sta = TRUE;}
    return(sta);
}


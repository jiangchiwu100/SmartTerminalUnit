/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5101slave_app.c
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include "string.h"
#include "common_data.h"
#include "dlt634_5101slave_app.h"
#include "dlt634_5101slave_disk.h"
#include "channel_monitor.h"

/* PRIVATE VARIABLES ----------------------------------------------------------*/
/* PUBLIC VARIABLES ----------------------------------------------------------*/
DLT634_5101SLAVE_PAD      DLT634_5101Slave_Pad[_DLT634_5101SLAVE_VOLUMES];
DLT634_5101SLAVE_APPINFO  *DLT634_5101Slave_App = RT_NULL; //__attribute__((at(SDRAM_ADDR_101)));

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : sum of CKS.
  * @param : [pdrv]
  * @param : [pBuf]
  * @return: sum
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_5101_SLAVE_CKS(uint8_t pdrv ,uint8_t *pBuf)
{
    uint16_t sum = 0;
    uint16_t j =0;
    uint8_t *p = 0;
    uint8_t i = 0;

    if (*(pBuf) == _DLT634_5101SLAVE_STARTCODE10)
    {
        for (j = 0; j < (DLT634_5101Slave_Pad[pdrv].LinkAddrSize + 1); j++)
        {
            sum += *(pBuf + 1 + j);
        }
    }
    else if(*(pBuf) == _DLT634_5101SLAVE_STARTCODE68)
    {
        p = pBuf+4;
        i = *(pBuf+1);

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
static uint8_t DLT634_5101_SLAVE_CheckFram68Valid(uint8_t pdrv)
{
    uint8_t *pBuf = 0;
    
    pBuf = (DLT634_5101Slave_App[pdrv].RxdBuf + DLT634_5101Slave_App[pdrv].RxdHead);

	  // 报头校验
    if ((pBuf[0] != pBuf[3]) || (pBuf[1] != pBuf[2]))
    {
        return(0);
    }

	  // 公共地址校验
    if (DLT634_5101Slave_Pad[pdrv].LinkAddrSize == 1)
    {
        if ((pBuf[5] != DLT634_5101Slave_Pad[pdrv].SourceAddr) && (pBuf[5] != 0xff))
        {
            return(0);
        }
    }
    else
    {
        if (((pBuf[5]|(pBuf[6]<<8)) != DLT634_5101Slave_Pad[pdrv].SourceAddr) && ((pBuf[5]|(pBuf[6]<<8)) != 0xffff))
        {
            return (0);
        }
    }
    
    return (1);
}

/**
  * @brief : check iec 10.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_5101_SLAVE_CheckFram10Valid(uint8_t pdrv)
{
    uint8_t *pBuf = 0;
    uint16_t addr = 0;

    pBuf = &DLT634_5101Slave_App[pdrv].RxdBuf[DLT634_5101Slave_App[pdrv].RxdHead];

	  // 固定帧长数据的校验和检测
    if (pBuf[2+DLT634_5101Slave_Pad[pdrv].LinkAddrSize] != (DLT634_5101_SLAVE_CKS(pdrv, pBuf)&0xFF))	
    {
        return (0);
    }

	  // 固定帧长数据的结束帧检测
    if (pBuf[3+DLT634_5101Slave_Pad[pdrv].LinkAddrSize] != _DLT634_5101SLAVE_ENDCODE)
    {
        return (0);
    }

	  // 固定帧数据公共地址检测
    if (DLT634_5101Slave_Pad[pdrv].LinkAddrSize == 1)
    {
        addr = pBuf[2];
    }
    else
    {
        addr = pBuf[2] | ((pBuf[3]<<8));
    }
		
    if (addr != DLT634_5101Slave_Pad[pdrv].SourceAddr)
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
uint8_t DLT634_5101_SLAVE_StoreIN(uint8_t pdrv,DLT634_5101SLAVE_PASDU *buf)
{	
	DLT634_5101SLAVE_PASDU *temp;
	DLT634_5101SLAVE_PASDU tempbuf;
	
	if(buf->status.symbol.priority<_DLT634_5101SLAVE_M_FT_NA_P)
	{
		temp = DLT634_5101Slave_App[pdrv].StoreDATA1IN;
		if(++temp >= DLT634_5101Slave_App[pdrv].StoreDATA1Buf + _DLT634_5101SLAVE_STOREDATA1NUM)
		{
			temp = DLT634_5101Slave_App[pdrv].StoreDATA1Buf;
		}
		
		if(temp == DLT634_5101Slave_App[pdrv].StoreDATA1OUT)
		{
			return(FALSE);
		}
	
		temp = DLT634_5101Slave_App[pdrv].StoreDATA1OUT;
		while(temp != DLT634_5101Slave_App[pdrv].StoreDATA1IN)
		{
			if(((temp->status.symbol.priority > buf->status.symbol.priority)||(temp->status.symbol.priority == 0))&&(temp != DLT634_5101Slave_App[pdrv].StoreDATA1OUT))
			{
				memcpy(&tempbuf,temp,sizeof(DLT634_5101SLAVE_PASDU));
				memcpy(temp,buf,sizeof(DLT634_5101SLAVE_PASDU));
				memcpy(buf,&tempbuf,sizeof(DLT634_5101SLAVE_PASDU));
			}
			if(++temp >= DLT634_5101Slave_App[pdrv].StoreDATA1Buf + _DLT634_5101SLAVE_STOREDATA1NUM)
			{
				temp = DLT634_5101Slave_App[pdrv].StoreDATA1Buf;
			}
		}
		
		memcpy(temp,buf,sizeof(DLT634_5101SLAVE_PASDU));	
		if(++DLT634_5101Slave_App[pdrv].StoreDATA1IN >= DLT634_5101Slave_App[pdrv].StoreDATA1Buf + _DLT634_5101SLAVE_STOREDATA1NUM)
		{
			DLT634_5101Slave_App[pdrv].StoreDATA1IN = DLT634_5101Slave_App[pdrv].StoreDATA1Buf;
		}
	}
	else
	{
		temp = DLT634_5101Slave_App[pdrv].StoreDATA2IN;
		if(++temp >= DLT634_5101Slave_App[pdrv].StoreDATA2Buf + _DLT634_5101SLAVE_STOREDATA2NUM)
		{
			temp = DLT634_5101Slave_App[pdrv].StoreDATA2Buf;
		}
		
		if(temp == DLT634_5101Slave_App[pdrv].StoreDATA2OUT)
		{
			return(FALSE);
		}
		
		temp = DLT634_5101Slave_App[pdrv].StoreDATA2OUT;
		while(temp != DLT634_5101Slave_App[pdrv].StoreDATA2IN)
		{
			if(((temp->status.symbol.priority > buf->status.symbol.priority)||(temp->status.symbol.priority == 0))&&(temp != DLT634_5101Slave_App[pdrv].StoreDATA2OUT))
			{
				memcpy(&tempbuf,temp,sizeof(DLT634_5101SLAVE_PASDU));
				memcpy(temp,buf,sizeof(DLT634_5101SLAVE_PASDU));
				memcpy(buf,&tempbuf,sizeof(DLT634_5101SLAVE_PASDU));
			}
			if(++temp >= DLT634_5101Slave_App[pdrv].StoreDATA2Buf + _DLT634_5101SLAVE_STOREDATA2NUM)
			{
				temp = DLT634_5101Slave_App[pdrv].StoreDATA2Buf;
			}
		}
		
		memcpy(temp,buf,sizeof(DLT634_5101SLAVE_PASDU));
		if(++DLT634_5101Slave_App[pdrv].StoreDATA2IN >= DLT634_5101Slave_App[pdrv].StoreDATA2Buf + _DLT634_5101SLAVE_STOREDATA2NUM)
		{
			DLT634_5101Slave_App[pdrv].StoreDATA2IN = DLT634_5101Slave_App[pdrv].StoreDATA2Buf;
		}
	}
	return(TRUE);
}

/**
  * @brief : analysis.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5101_SLAVE_AppProcess(uint8_t pdrv)
{		
	//整理成02版报文，便于控制和传输
	memset((uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp,0,sizeof(DLT634_5101SLAVE_PASDU));
	DLT634_5101Slave_App[pdrv].RX_Temp.status.Length = DLT634_5101Slave_App[pdrv].RxdBuf[1] -1 -DLT634_5101Slave_Pad[pdrv].LinkAddrSize;
	memcpy((uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp.Head.TypeID,&DLT634_5101Slave_App[pdrv].RxdBuf[5+DLT634_5101Slave_Pad[pdrv].LinkAddrSize],DLT634_5101Slave_App[pdrv].RX_Temp.status.Length);
	if(DLT634_5101Slave_Pad[pdrv].ASDUCotSize == 1)
	{
		DLT634_5101Slave_App[pdrv].RX_Temp.Head.COT_H = 0;
		memcpy((uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp.Head.PubAddr_L,&DLT634_5101Slave_App[pdrv].RxdBuf[5+DLT634_5101Slave_Pad[pdrv].LinkAddrSize+2+DLT634_5101Slave_Pad[pdrv].ASDUCotSize]
		,DLT634_5101Slave_App[pdrv].RX_Temp.status.Length-2-DLT634_5101Slave_Pad[pdrv].ASDUCotSize);	
		DLT634_5101Slave_App[pdrv].RX_Temp.status.Length = DLT634_5101Slave_App[pdrv].RX_Temp.status.Length - DLT634_5101Slave_Pad[pdrv].ASDUCotSize + 2;
	}
	if(DLT634_5101Slave_Pad[pdrv].ASDUAddrSize == 1)
	{
		DLT634_5101Slave_App[pdrv].RX_Temp.Head.PubAddr_H = 0;
		memcpy((uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp.Data.buff[0],&DLT634_5101Slave_App[pdrv].RxdBuf[5+DLT634_5101Slave_Pad[pdrv].LinkAddrSize+2+DLT634_5101Slave_Pad[pdrv].ASDUCotSize+DLT634_5101Slave_Pad[pdrv].ASDUAddrSize]
		,DLT634_5101Slave_App[pdrv].RX_Temp.status.Length-2-DLT634_5101Slave_Pad[pdrv].ASDUCotSize-DLT634_5101Slave_Pad[pdrv].ASDUAddrSize);	
		DLT634_5101Slave_App[pdrv].RX_Temp.status.Length = DLT634_5101Slave_App[pdrv].RX_Temp.status.Length - DLT634_5101Slave_Pad[pdrv].ASDUAddrSize + 2;		
	}
	DLT634_5101Slave_App[pdrv].RX_Temp.status.Length += sizeof(DLT634_5101Slave_App[pdrv].RX_Temp.status);
	
	
	switch (DLT634_5101Slave_App[pdrv].RX_Temp.Head.TypeID)
	{
        //控制方向过程信息
        case _DLT634_5101SLAVE_C_SC_NA_1: // 单点遥控命令
        case _DLT634_5101SLAVE_C_SC_NB_1: // 双点遥控命令
            DLT634_5101_SLAVE_C_SC(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp);
                break;

        //控制方向命令信息
        case _DLT634_5101SLAVE_C_IC_NA_1: // 总召唤或分组召唤
            DLT634_5101Slave_App[pdrv].Data1Flag |= _DLT634_5101SLAVE_CALLALLDATA;
            DLT634_5101Slave_App[pdrv].AllDataFlag = 0;
            DLT634_5101_SLAVE_C_IC(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp);
                break;

        case _DLT634_5101SLAVE_C_CI_NA_1: // 电度总召唤或分组召唤
                break;

        case _DLT634_5101SLAVE_C_CS_NA_1: // 对时命令
            DLT634_5101_SLAVE_C_CS(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp);
                break;

        case _DLT634_5101SLAVE_C_TS_NA_1: // 测试命令
            DLT634_5101Slave_App[pdrv].RX_Temp.status.symbol.priority = _DLT634_5101SLAVE_C_TS_NA_P;
            DLT634_5101Slave_App[pdrv].RX_Temp.Head.COT_L = _DLT634_5101SLAVE_COT_ACTCON;
            DLT634_5101_SLAVE_StoreIN(pdrv,&DLT634_5101Slave_App[pdrv].RX_Temp);
                break;

        case _DLT634_5101SLAVE_C_RP_NA_1: // 复位进程命令
            DLT634_5101Slave_App[pdrv].Data1Flag |= _DLT634_5101SLAVE_HAVERESET;
            DLT634_5101_SLAVE_C_RP(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp);
                break;
        
        case _DLT634_5101SLAVE_C_SR_NA_1: // 切换定值区
        case _DLT634_5101SLAVE_C_RR_NA_1: // 读定值区号
        case _DLT634_5101SLAVE_C_RS_NA_1: // 读参数和定值命令
        case _DLT634_5101SLAVE_C_WS_NA_1: // 写参数和定值命令			
            DLT634_5101_SLAVE_C_SR(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp);
                break;

        case _DLT634_5101SLAVE_F_FR_NA_1: // 文件传输
            DLT634_5101_SLAVE_F_FR(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp);
                break;

        case _DLT634_5101SLAVE_F_SR_NA_1: // 软件升级
            DLT634_5101_SLAVE_F_SR(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].RX_Temp);
                break;			

        default: // 类型标识有错误或不支持
                break;
	}
}

/**
  * @brief : write data to misi.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_5101_SLAVE_WriteDataToMISI(uint8_t pdrv)
{
    uint16_t sendlen = 0;
   
    sendlen = DLT634_5101_SLAVE_WriteData(pdrv ,DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdHead, DLT634_5101Slave_App[pdrv].TxdTail - DLT634_5101Slave_App[pdrv].TxdHead);
    
    DLT634_5101Slave_App[pdrv].TxdHead += sendlen;
    if (DLT634_5101Slave_App[pdrv].TxdHead >= DLT634_5101Slave_App[pdrv].TxdTail) // 该次任务数据已经发完
    {
        DLT634_5101Slave_App[pdrv].TxdHead = 0;
        DLT634_5101Slave_App[pdrv].TxdTail = 0;
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
static void DLT634_5101_SLAVE_DecodeFrame68(uint8_t pdrv)
{
    uint8_t control;
    
    control = DLT634_5101Slave_App[pdrv].RxdBuf[4];

    if (control & _DLT634_5101SLAVE_PRM) // 子站作为从动站接收主站链路报文。
    {
        if ((!(control&_DLT634_5101SLAVE_FCV)) || ((control&_DLT634_5101SLAVE_FCB) != ((DLT634_5101Slave_App[pdrv].RlaConCode) & 0x20)) || (!(DLT634_5101Slave_App[pdrv].LinkFlag & _DLT634_5101SLAVE_FIRST_RlaConCode)))
        {
            if(DLT634_5101Slave_App[pdrv].LinkFlag & _DLT634_5101SLAVE_INITEND)
            {
                if (control & _DLT634_5101SLAVE_FCV) // FCV有效而且FCB变位
                {
                    DLT634_5101Slave_App[pdrv].RlaConCode = DLT634_5101Slave_App[pdrv].RxdBuf[4]; // 保存最新的功能码，这里只用到FCB
                    DLT634_5101Slave_App[pdrv].FCBNoTurnNum = _DLT634_5101SLAVE_NUMOF_MAXRETRY; // 设置重发次数，指下次收到主站报文FCB未翻转时重发缓冲区内的报文。
                    DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_FIRST_RlaConCode;
                }
                DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_RECONFIRM;
                DLT634_5101_SLAVE_AppProcess(pdrv);
            }
        }
        else  // FCB 未翻转
        {
            if(DLT634_5101Slave_Pad[pdrv].BalanMode)
            {
                DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_RECONFIRM;
            }
            else
            {
                if (DLT634_5101Slave_App[pdrv].FCBNoTurnNum)
                {
                    DLT634_5101Slave_App[pdrv].FCBNoTurnNum--;
                    memcpy(DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail, DLT634_5101Slave_App[pdrv].TxdBuf_Pri, DLT634_5101Slave_App[pdrv].TxdTail_Pri);
                    DLT634_5101Slave_App[pdrv].TxdTail += DLT634_5101Slave_App[pdrv].TxdTail_Pri;
                    DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
                }
            }
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
static void DLT634_5101_SLAVE_StatusReset(uint8_t pdrv)
{
    DLT634_5101Slave_App[pdrv].LinkFlag = 0;
    DLT634_5101Slave_App[pdrv].Data1Flag = 0;
    DLT634_5101Slave_App[pdrv].Data2Flag = 0;
    
    DLT634_5101Slave_App[pdrv].StoreDATA1IN = DLT634_5101Slave_App[pdrv].StoreDATA1Buf;
    DLT634_5101Slave_App[pdrv].StoreDATA1OUT = DLT634_5101Slave_App[pdrv].StoreDATA1Buf;
    memset(DLT634_5101Slave_App[pdrv].StoreDATA1Buf,0,sizeof(DLT634_5101Slave_App[pdrv].StoreDATA1Buf));
    DLT634_5101Slave_App[pdrv].StoreDATA2IN = DLT634_5101Slave_App[pdrv].StoreDATA2Buf;
    DLT634_5101Slave_App[pdrv].StoreDATA2OUT = DLT634_5101Slave_App[pdrv].StoreDATA2Buf;
    memset(DLT634_5101Slave_App[pdrv].StoreDATA2Buf,0,sizeof(DLT634_5101Slave_App[pdrv].StoreDATA2Buf));
}

/**
  * @brief : 10 analysis.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_DecodeFrame10(uint8_t pdrv)
{
    uint8_t stop = 0;
    uint8_t control = 0;

    control = DLT634_5101Slave_App[pdrv].RxdBuf[1];
    if (control & _DLT634_5101SLAVE_PRM) // 子站作为从动站接收主站的命令
    {
        if ((!(control&_DLT634_5101SLAVE_FCV)) || ((control&_DLT634_5101SLAVE_FCB) != ((DLT634_5101Slave_App[pdrv].RlaConCode) & 0x20)) || (!(DLT634_5101Slave_App[pdrv].LinkFlag & _DLT634_5101SLAVE_FIRST_RlaConCode)))
        {
            if (control & _DLT634_5101SLAVE_FCV) // FCV有效，FCB翻转
            {
                DLT634_5101Slave_App[pdrv].RlaConCode = DLT634_5101Slave_App[pdrv].RxdBuf[1];
                DLT634_5101Slave_App[pdrv].FCBNoTurnNum = _DLT634_5101SLAVE_NUMOF_MAXRETRY;
								DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_FIRST_RlaConCode;
            }
            switch (control & _DLT634_5101SLAVE_FUNCODE)
            {
                case _DLT634_5101SLAVE_M_FUN2: // 心跳测试
                    DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_RECONFIRM;
                    break;
										
                case _DLT634_5101SLAVE_M_FUN9: // 召唤链路状态
					DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_REQSTATUS;
								    /* 可以避免答非所问的过程，尽快建立链路 */ 
                    while (((DLT634_5101Slave_App[pdrv].RxdTail - DLT634_5101Slave_App[pdrv].RxdHead) >= DLT634_5101Slave_Pad[pdrv].FixFrmLength) && (stop == 0)) // 收到召唤链路状态命令时，清掉后续相同命令。
                    {
                        if (memcmp(DLT634_5101Slave_App[pdrv].RxdBuf, DLT634_5101Slave_App[pdrv].RxdBuf + DLT634_5101Slave_App[pdrv].RxdHead, DLT634_5101Slave_Pad[pdrv].FixFrmLength) == 0)
                        {
                            DLT634_5101Slave_App[pdrv].RxdHead += DLT634_5101Slave_Pad[pdrv].FixFrmLength;
                        }
                        else
                        {
                            stop = 1;
                        }
                    }
                    #if _DLT634_5101SLAVE_COMMUNICATION
                    DLT634_5101Slave_App[pdrv].RX_Temp.status.Length = sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.status) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Head) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Data.C_105);
                    DLT634_5101Slave_App[pdrv].RX_Temp.Head.TypeID = _DLT634_5101SLAVE_C_RP_NA_1;
                    DLT634_5101Slave_App[pdrv].RX_Temp.Head.VSQ = 1;
                    DLT634_5101Slave_App[pdrv].RX_Temp.Head.COT_L = _DLT634_5101SLAVE_COT_ACT;
                    DLT634_5101Slave_App[pdrv].RX_Temp.Head.PubAddr_H = (uint8_t)((DLT634_5101Slave_Pad[pdrv].ASDUAddr>>8)&0xff);
                    DLT634_5101Slave_App[pdrv].RX_Temp.Head.PubAddr_L = (uint8_t)(DLT634_5101Slave_Pad[pdrv].ASDUAddr&0xff);
                    DLT634_5101Slave_App[pdrv].RX_Temp.Data.C_105.QRP = 1;
                    #endif
                    break;

                case _DLT634_5101SLAVE_M_FUN0: // 复位链路					
                    DLT634_5101_SLAVE_StatusReset(pdrv);
            
                    DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_INITEND;//初始化结束标志
                    if(DLT634_5101Slave_Pad[pdrv].BalanMode)
                    {
                        DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_RECONFIRM;
                        DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_ASKSTATUS;
                    }
                    else
                    {
                        DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_RECONFIRM;
                        DLT634_5101Slave_App[pdrv].Data1Flag |= _DLT634_5101SLAVE_HAVEINITEND;
                    }
                    break;

                case _DLT634_5101SLAVE_M_FUN10: // 功能码-10，召唤用户一级数据
                    // 应用层调用
                    if (!(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_INITEND))
                    {
                        return;
                    }  
                    DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_REDATA1;
                    break;

                case _DLT634_5101SLAVE_M_FUN11: // 功能码-11，召唤用户二级数据
                    // 应用层调用
                    if (!(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_INITEND))
                    {
                        return;
                    }  
                    DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_REDATA2;
                    break;

                default:
                    break;
            }
            return;
        }
        else // FCB未翻转
        {
            if (DLT634_5101Slave_App[pdrv].FCBNoTurnNum)
            {
                DLT634_5101Slave_App[pdrv].FCBNoTurnNum--;
                memcpy(DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail, DLT634_5101Slave_App[pdrv].TxdBuf_Pri, DLT634_5101Slave_App[pdrv].TxdTail_Pri);
                DLT634_5101Slave_App[pdrv].TxdTail += DLT634_5101Slave_App[pdrv].TxdTail_Pri;
                DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
            }
        }
    }
    else // Prm=0 平衡模式子站作为启动站接收到主站的命令。
    {
        switch (control & _DLT634_5101SLAVE_FUNCODE)
        {
            case _DLT634_5101SLAVE_S_FUN0:	// 确认
                DLT634_5101Slave_App[pdrv].TimeOutTick_Pri = 0; // 链路有数据就可以不重发
                DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_SENDABLE;
                break;

            case _DLT634_5101SLAVE_S_FUN11: // 应答链路
                DLT634_5101_SLAVE_StatusReset(pdrv);
        
                DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_ASKRELINK;
                DLT634_5101Slave_App[pdrv].Data1Flag |= _DLT634_5101SLAVE_HAVEINITEND;
        
                DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_INITEND;//初始化结束标志
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
void DLT634_5101_SLAVE_SearchValidFrame(uint8_t pdrv)
{
    uint8_t stop = 0;
    uint8_t length = 0;
    
    while ((DLT634_5101Slave_App[pdrv].RxdHead < DLT634_5101Slave_App[pdrv].RxdTail) && (!stop))
    {
        if (DLT634_5101Slave_App[pdrv].RxdStatus == RXDSTART)
        {
            while ((DLT634_5101Slave_App[pdrv].RxdBuf[DLT634_5101Slave_App[pdrv].RxdHead] != _DLT634_5101SLAVE_STARTCODE10)
                    && (DLT634_5101Slave_App[pdrv].RxdBuf[DLT634_5101Slave_App[pdrv].RxdHead] != _DLT634_5101SLAVE_STARTCODE68)
                    && (DLT634_5101Slave_App[pdrv].RxdHead < DLT634_5101Slave_App[pdrv].RxdTail))
            {
                DLT634_5101Slave_App[pdrv].RxdHead++;
            }

            if (DLT634_5101Slave_App[pdrv].RxdHead < DLT634_5101Slave_App[pdrv].RxdTail) // 找到启动字符，并将报文与缓冲区对齐。
            {
                DLT634_5101Slave_App[pdrv].RxdStatus = RXDHEAD;
                if (DLT634_5101Slave_App[pdrv].RxdHead != 0)
                {
                    memcpy(DLT634_5101Slave_App[pdrv].RxdBuf,DLT634_5101Slave_App[pdrv].RxdBuf+DLT634_5101Slave_App[pdrv].RxdHead,DLT634_5101Slave_App[pdrv].RxdTail-DLT634_5101Slave_App[pdrv].RxdHead);
                    DLT634_5101Slave_App[pdrv].RxdTail -= DLT634_5101Slave_App[pdrv].RxdHead;
                    DLT634_5101Slave_App[pdrv].RxdHead = 0;
                }
            }
        }
        
        DLT634_5101Slave_App[pdrv].RxdLength = DLT634_5101Slave_App[pdrv].RxdTail - DLT634_5101Slave_App[pdrv].RxdHead;
        if (DLT634_5101Slave_App[pdrv].RxdStatus == RXDHEAD) // 报文够一帧则处理
        {
            switch (DLT634_5101Slave_App[pdrv].RxdBuf[DLT634_5101Slave_App[pdrv].RxdHead])
            {
                case _DLT634_5101SLAVE_STARTCODE68:
                    if (DLT634_5101Slave_App[pdrv].RxdLength >= 5+DLT634_5101Slave_Pad[pdrv].LinkAddrSize) // 启动字符到链路地址共6字节。
                    {
                        if (!DLT634_5101_SLAVE_CheckFram68Valid(pdrv)) // 判断是否报文头
                        {
                            DLT634_5101Slave_App[pdrv].RxdHead++;
                            DLT634_5101Slave_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            DLT634_5101Slave_App[pdrv].RxdStatus = RXDCONTINUE;
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
                    
                case _DLT634_5101SLAVE_STARTCODE10:
                    if (DLT634_5101Slave_App[pdrv].RxdLength >= 4 + DLT634_5101Slave_Pad[pdrv].LinkAddrSize)
                    {
                        if (!DLT634_5101_SLAVE_CheckFram10Valid(pdrv))
                        {
                            DLT634_5101Slave_App[pdrv].RxdHead++;
                            DLT634_5101Slave_App[pdrv].RxdStatus = RXDSTART;
                        }
                        else
                        {
                            DLT634_5101Slave_App[pdrv].RxdStatus = RXDCONTINUE;
                        }
                    }
                    else
                    {
                        stop = 1;
                    }
                    break;
            }
        }
        if (DLT634_5101Slave_App[pdrv].RxdStatus == RXDCONTINUE)
        {
            switch (DLT634_5101Slave_App[pdrv].RxdBuf[DLT634_5101Slave_App[pdrv].RxdHead])
            {
            case _DLT634_5101SLAVE_STARTCODE68:
                length = DLT634_5101Slave_App[pdrv].RxdBuf[DLT634_5101Slave_App[pdrv].RxdHead+1];
                if (DLT634_5101Slave_App[pdrv].RxdLength >= length+6) // 报文收全，进行处理
                {
                    if ((DLT634_5101Slave_App[pdrv].RxdBuf[DLT634_5101Slave_App[pdrv].RxdHead+length+4] == (DLT634_5101_SLAVE_CKS(pdrv, DLT634_5101Slave_App[pdrv].RxdBuf+DLT634_5101Slave_App[pdrv].RxdHead)&0xFF))//校验正确
                            && (DLT634_5101Slave_App[pdrv].RxdBuf[DLT634_5101Slave_App[pdrv].RxdHead+length+4+1] == _DLT634_5101SLAVE_ENDCODE)) // 结束码正确
 					          {
                        if ( DLT634_5101Slave_App[pdrv].RxdHead > 0 )
                        {
                            memcpy(DLT634_5101Slave_App[pdrv].RxdBuf,DLT634_5101Slave_App[pdrv].RxdBuf+DLT634_5101Slave_App[pdrv].RxdHead,DLT634_5101Slave_App[pdrv].RxdTail-DLT634_5101Slave_App[pdrv].RxdHead);//报文与缓冲区对齐
                            DLT634_5101Slave_App[pdrv].RxdTail -= DLT634_5101Slave_App[pdrv].RxdHead;
                            DLT634_5101Slave_App[pdrv].RxdHead = 0;
                        }
												/* 头指针后移到报文后 */
                        DLT634_5101Slave_App[pdrv].RxdHead += (DLT634_5101Slave_App[pdrv].RxdBuf[1]+6);
                        DLT634_5101Slave_App[pdrv].RxdStatus = RXDSTART;

                        DLT634_5101_SLAVE_DecodeFrame68(pdrv);
                    }
                    else
                    {
                        DLT634_5101Slave_App[pdrv].RxdHead += 6;
                        DLT634_5101Slave_App[pdrv].RxdStatus = RXDSTART;
                    }
                }
                else // 报文未收全，不处理，继续等待数据。
                {
                    stop = 1;
                }
                break;
                
            case _DLT634_5101SLAVE_STARTCODE10:
                if (DLT634_5101Slave_App[pdrv].RxdHead > 0)
                {
                    memcpy(DLT634_5101Slave_App[pdrv].RxdBuf,DLT634_5101Slave_App[pdrv].RxdBuf+DLT634_5101Slave_App[pdrv].RxdHead,DLT634_5101Slave_App[pdrv].RxdTail-DLT634_5101Slave_App[pdrv].RxdHead);
                    DLT634_5101Slave_App[pdrv].RxdTail -= DLT634_5101Slave_App[pdrv].RxdHead;
                    DLT634_5101Slave_App[pdrv].RxdHead = 0;
                }
                DLT634_5101Slave_App[pdrv].RxdHead += 4 + DLT634_5101Slave_Pad[pdrv].LinkAddrSize;
                DLT634_5101Slave_App[pdrv].RxdStatus = RXDSTART;
			
                DLT634_5101_SLAVE_DecodeFrame10(pdrv);
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
static void DLT634_5101_SLAVE_ReadMISIData(uint8_t pdrv)
{
    uint16_t count = 0;
    
    if (DLT634_5101Slave_App[pdrv].RxdHead < DLT634_5101Slave_App[pdrv].RxdTail) // 接收缓冲区中有尚未处理的数据
    {
        if (DLT634_5101Slave_App[pdrv].RxdHead != 0)
        {
            memcpy(DLT634_5101Slave_App[pdrv].RxdBuf, DLT634_5101Slave_App[pdrv].RxdBuf+DLT634_5101Slave_App[pdrv].RxdHead, DLT634_5101Slave_App[pdrv].RxdTail-DLT634_5101Slave_App[pdrv].RxdHead);//将未处理的数据移到缓冲区头
            DLT634_5101Slave_App[pdrv].RxdTail -= DLT634_5101Slave_App[pdrv].RxdHead;
            DLT634_5101Slave_App[pdrv].RxdHead = 0;
        }
    }
    else
    {
        DLT634_5101Slave_App[pdrv].RxdHead = 0;
		DLT634_5101Slave_App[pdrv].RxdTail = 0;
    }
    
    count = DLT634_5101_SLAVE_ReadData(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].RxdBuf[DLT634_5101Slave_App[pdrv].RxdTail], _DLT634_5101SLAVE_LPDUSIZE);
    if (count > 0)
    {
        if(!(DLT634_ChannelToMonitor_SLAVE_SearchMonitorFrame(pdrv, DLT634_5101, DLT634_5101Slave_App[pdrv].RxdBuf, _DLT634_5101SLAVE_LPDUSIZE)))    //无监听时启动
        {
            DLT634_5101Slave_App[pdrv].RxdTail += count;			
            DLT634_5101_SLAVE_SearchValidFrame(pdrv);
        }
        DLT634_5101Slave_App[pdrv].TimeOutLink = DLT634_5101Slave_Pad[pdrv].TimeOutLink;
    }
}

/**
  * @brief : Slave Frame 10 Packet.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_SlaveFrame10Packet(uint8_t pdrv, uint8_t Function)
{
    uint8_t *pBuf;
    uint8_t i = 0;
									
    pBuf = DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail;
    pBuf[0] = _DLT634_5101SLAVE_STARTCODE10;
    pBuf[1] = Function&0x0f;
    if(DLT634_5101Slave_Pad[pdrv].BalanMode)
    {
        pBuf[1] |= DLT634_5101Slave_Pad[pdrv].IEC_DIR; // 子站向主站传输，平衡模式为1，非平衡模式为0
    }
    else
    {
        if(DLT634_5101Slave_App[pdrv].Data1Flag)
        {pBuf[1] |= _DLT634_5101SLAVE_ACD;}
    }

    for (i=0; i<DLT634_5101Slave_Pad[pdrv].LinkAddrSize; i++)
    {
        pBuf[2+i] = (DLT634_5101Slave_Pad[pdrv].SourceAddr>>(8*i)) & 0xff;
    }

    pBuf[2+DLT634_5101Slave_Pad[pdrv].LinkAddrSize] = DLT634_5101_SLAVE_CKS(pdrv, pBuf);
    pBuf[3+DLT634_5101Slave_Pad[pdrv].LinkAddrSize] = _DLT634_5101SLAVE_ENDCODE;
    DLT634_5101Slave_App[pdrv].TxdTail += DLT634_5101Slave_Pad[pdrv].FixFrmLength;//移动发送尾指针
}

/**
  * @brief : Master Frame 10 Packet.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_MasterFrame10Packet(uint8_t pdrv, uint8_t Function)
{
    uint8_t *pBuf;
    uint8_t i = 0;

    pBuf = DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail;
    pBuf[0] = _DLT634_5101SLAVE_STARTCODE10;
    pBuf[1] = Function&0x0f;
    pBuf[1] |= _DLT634_5101SLAVE_PRM;
    pBuf[1] |= DLT634_5101Slave_Pad[pdrv].IEC_DIR; // 子站向主站传输，平衡模式为1，非平衡模式为0

    for (i=0; i<DLT634_5101Slave_Pad[pdrv].LinkAddrSize; i++)
    {
        pBuf[2+i] = (DLT634_5101Slave_Pad[pdrv].SourceAddr>>(8*i)) & 0xff;
    }

    pBuf[2+DLT634_5101Slave_Pad[pdrv].LinkAddrSize] = DLT634_5101_SLAVE_CKS(pdrv, pBuf);
    pBuf[3+DLT634_5101Slave_Pad[pdrv].LinkAddrSize] = _DLT634_5101SLAVE_ENDCODE;
    DLT634_5101Slave_App[pdrv].TxdTail += DLT634_5101Slave_Pad[pdrv].FixFrmLength;//移动发送尾指针

    memcpy(DLT634_5101Slave_App[pdrv].TxdBuf_Pri, pBuf, DLT634_5101Slave_Pad[pdrv].FixFrmLength); // 将发送数据保存到启动站重发数据区
    DLT634_5101Slave_App[pdrv].TxdTail_Pri = DLT634_5101Slave_Pad[pdrv].FixFrmLength;
}

/**
  * @brief : Link 10 Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_Link10Process(uint8_t pdrv)
{ 
	if(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_REQSTATUS)
	{
		DLT634_5101_SLAVE_SlaveFrame10Packet(pdrv, _DLT634_5101SLAVE_S_FUN11);
		DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
		DLT634_5101Slave_App[pdrv].LinkFlag &= (~_DLT634_5101SLAVE_REQSTATUS);
		return;
	}
	
	if(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_RECONFIRM)
	{
		DLT634_5101_SLAVE_SlaveFrame10Packet(pdrv, _DLT634_5101SLAVE_S_FUN0);
		DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
		DLT634_5101Slave_App[pdrv].LinkFlag &= (~_DLT634_5101SLAVE_RECONFIRM);
		return;
	}
	
	if(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_ASKSTATUS)
	{
		DLT634_5101_SLAVE_MasterFrame10Packet(pdrv, _DLT634_5101SLAVE_M_FUN9);
		DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
		DLT634_5101Slave_App[pdrv].LinkFlag &= (~_DLT634_5101SLAVE_ASKSTATUS);
		DLT634_5101Slave_App[pdrv].TimeOutTick_AskSta = DLT634_5101Slave_Pad[pdrv].AskStaOutValue;
		return;
	}
	if(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_ASKRELINK)
	{
		DLT634_5101_SLAVE_MasterFrame10Packet(pdrv, _DLT634_5101SLAVE_M_FUN0);
		DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
		DLT634_5101Slave_App[pdrv].LinkFlag &= (~_DLT634_5101SLAVE_ASKRELINK);
		return;
	}	
}

/**
  * @brief : Rest Reply.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_RestReply(uint8_t pdrv)
{
    DLT634_5101Slave_App[pdrv].TX_Temp.status.Length = sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.status) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Head) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Data.C_105);
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.TypeID = _DLT634_5101SLAVE_C_RP_NA_1;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.VSQ = 1;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_L = _DLT634_5101SLAVE_COT_ACTCON;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.PubAddr_H = (uint8_t)((DLT634_5101Slave_Pad[pdrv].ASDUAddr>>8)&0xff);
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.PubAddr_L = (uint8_t)(DLT634_5101Slave_Pad[pdrv].ASDUAddr&0xff);
    DLT634_5101Slave_App[pdrv].TX_Temp.Data.C_105.QRP = 1;
}

/**
  * @brief : Init End Reply.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_InitEndReply(uint8_t pdrv)
{
    DLT634_5101Slave_App[pdrv].TX_Temp.status.Length = sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.status) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Head) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Data.C_70);
    DLT634_5101Slave_App[pdrv].TX_Temp.status.symbol.priority = _DLT634_5101SLAVE_M_EI_NA_P;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.TypeID = _DLT634_5101SLAVE_M_EI_NA_1;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.VSQ = 1;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_L = _DLT634_5101SLAVE_COT_INIT;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.PubAddr_H = (uint8_t)((DLT634_5101Slave_Pad[pdrv].ASDUAddr>>8)&0xff);
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.PubAddr_L = (uint8_t)(DLT634_5101Slave_Pad[pdrv].ASDUAddr&0xff);
    DLT634_5101Slave_App[pdrv].TX_Temp.Data.C_70.COI = 2;
}

/**
  * @brief : All Data Con Reply.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_AllDataConReply(uint8_t pdrv)
{
    DLT634_5101Slave_App[pdrv].TX_Temp.status.Length = sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.status) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Head) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Data.C_100);
    DLT634_5101Slave_App[pdrv].TX_Temp.status.symbol.priority = _DLT634_5101SLAVE_C_IC_NA_PF;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.TypeID = _DLT634_5101SLAVE_C_IC_NA_1;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.VSQ = 1;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_L = _DLT634_5101SLAVE_COT_ACTCON;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.PubAddr_H = (uint8_t)((DLT634_5101Slave_Pad[pdrv].ASDUAddr>>8)&0xff);
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.PubAddr_L = (uint8_t)(DLT634_5101Slave_Pad[pdrv].ASDUAddr&0xff);
    DLT634_5101Slave_App[pdrv].TX_Temp.Data.C_70.COI = 20;
}

/**
  * @brief : All Data End Reply.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_AllDataEndReply(uint8_t pdrv)
{
    DLT634_5101Slave_App[pdrv].TX_Temp.status.Length = sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.status) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Head) + sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Data.C_100);
    DLT634_5101Slave_App[pdrv].TX_Temp.status.symbol.priority = _DLT634_5101SLAVE_C_IC_NA_PF;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.TypeID = _DLT634_5101SLAVE_C_IC_NA_1;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.VSQ = 1;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_L = _DLT634_5101SLAVE_COT_ACCTTERM;
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.PubAddr_H = (uint8_t)((DLT634_5101Slave_Pad[pdrv].ASDUAddr>>8)&0xff);
    DLT634_5101Slave_App[pdrv].TX_Temp.Head.PubAddr_L = (uint8_t)(DLT634_5101Slave_Pad[pdrv].ASDUAddr&0xff);
    DLT634_5101Slave_App[pdrv].TX_Temp.Data.C_70.COI = 20;
}

/**
  * @brief : Read All Data Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_ReadAllDataProcess(uint8_t pdrv)
{
	switch(DLT634_5101Slave_App[pdrv].AllDataFlag)
	{
		case 0:
            DLT634_5101_SLAVE_AllDataConReply(pdrv);
            DLT634_5101Slave_App[pdrv].AllData_yx_sendaddr = DLT634_5101Slave_Pad[pdrv].YX_FirstAddr;
            DLT634_5101Slave_App[pdrv].AllData_yc_sendaddr = DLT634_5101Slave_Pad[pdrv].YC_FirstAddr;
            DLT634_5101Slave_App[pdrv].AllDataFlag = 1;
			break;
		case 1:
            if(DLT634_5101Slave_Pad[pdrv].YX_AllNum != 0)
            {
                DLT634_5101Slave_App[pdrv].AllData_yx_sendaddr = DLT634_5101_SLAVE_R_YXDATA(pdrv ,DLT634_5101Slave_App[pdrv].AllData_yx_sendaddr ,DLT634_5101Slave_Pad[pdrv].YX_FrameNum ,(uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp);
                if(DLT634_5101Slave_App[pdrv].AllData_yx_sendaddr >= DLT634_5101Slave_Pad[pdrv].YX_FirstAddr + DLT634_5101Slave_Pad[pdrv].YX_AllNum)
                {
                    DLT634_5101Slave_App[pdrv].AllDataFlag = 2;
                }
                break;
            }
		case 2:
            if(DLT634_5101Slave_Pad[pdrv].YC_AllNum != 0)
            {
                DLT634_5101Slave_App[pdrv].AllData_yc_sendaddr = DLT634_5101_SLAVE_R_YCDATA(pdrv ,DLT634_5101Slave_App[pdrv].AllData_yc_sendaddr ,DLT634_5101Slave_Pad[pdrv].YC_FrameNum ,(uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp);
                if(DLT634_5101Slave_App[pdrv].AllData_yc_sendaddr >= DLT634_5101Slave_Pad[pdrv].YC_FirstAddr + DLT634_5101Slave_Pad[pdrv].YC_AllNum)
                {
                    DLT634_5101Slave_App[pdrv].AllDataFlag = 3;
                }
                break;
            }
		case 3:
            DLT634_5101Slave_App[pdrv].AllDataFlag = 0;
            DLT634_5101_SLAVE_AllDataEndReply(pdrv);
            DLT634_5101Slave_App[pdrv].Data1Flag &= (~_DLT634_5101SLAVE_CALLALLDATA);
			break;
		default:
			break;
		
	}
}


/**
  * @brief : Data1 Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_Data1Process(uint8_t pdrv)
{ 
    if (DLT634_5101Slave_App[pdrv].Data1Flag & _DLT634_5101SLAVE_HAVERESET) // 复位进程
    {
        DLT634_5101_SLAVE_RestReply(pdrv);
        DLT634_5101Slave_App[pdrv].Data1Flag &= (~_DLT634_5101SLAVE_HAVERESET);
        DLT634_5101Slave_App[pdrv].LinkFlag = 0;
        return;
    }
		
    if ((DLT634_5101Slave_App[pdrv].Data1Flag & _DLT634_5101SLAVE_HAVEINITEND)&&
			((DLT634_5101Slave_App[pdrv].StoreDATA1IN->status.symbol.priority > _DLT634_5101SLAVE_M_EI_NA_P)||
			(DLT634_5101Slave_App[pdrv].StoreDATA1IN->status.symbol.priority == 0))) // 初始化结束回复过程
    {
        DLT634_5101_SLAVE_InitEndReply(pdrv);
        DLT634_5101Slave_App[pdrv].Data1Flag &= (~_DLT634_5101SLAVE_HAVEINITEND);
        return;
    }
		
    if(DLT634_5101Slave_App[pdrv].TxdTail_Special)//特殊数据重发
    {
        memcpy(DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail, DLT634_5101Slave_App[pdrv].TxdBuf_Special, DLT634_5101Slave_App[pdrv].TxdTail_Special);
        DLT634_5101Slave_App[pdrv].TxdTail += DLT634_5101Slave_App[pdrv].TxdTail_Special;
        DLT634_5101Slave_App[pdrv].TxdTail_Special = 0;
        memcpy(DLT634_5101Slave_App[pdrv].TxdBuf_Pri, DLT634_5101Slave_App[pdrv].TxdBuf_Special, DLT634_5101Slave_App[pdrv].TxdTail_Special);
        DLT634_5101Slave_App[pdrv].TxdTail_Pri = DLT634_5101Slave_App[pdrv].TxdTail_Special;
        DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
        DLT634_5101Slave_App[pdrv].TimeOutTick_Pri = DLT634_5101Slave_Pad[pdrv].TimeOutValue;
        DLT634_5101Slave_App[pdrv].RetryCount  = _DLT634_5101SLAVE_NUMOF_MAXRETRY;
        DLT634_5101Slave_App[pdrv].LinkFlag &= (~_DLT634_5101SLAVE_SENDABLE);
        return;
    }

    if ((DLT634_5101_SLAVE_H_COS(pdrv))&&
        ((DLT634_5101Slave_App[pdrv].StoreDATA1IN->status.symbol.priority > _DLT634_5101SLAVE_M_SP_TB_P)||
        (DLT634_5101Slave_App[pdrv].StoreDATA1IN->status.symbol.priority == 0))) 
    {
        DLT634_5101_SLAVE_R_COS(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp);
        DLT634_5101Slave_App[pdrv].Data1Flag &= (~_DLT634_5101SLAVE_HAVECOS);
        return;
    }
    
    if ((DLT634_5101_SLAVE_H_SOE(pdrv))&&
        ((DLT634_5101Slave_App[pdrv].StoreDATA1IN->status.symbol.priority > _DLT634_5101SLAVE_M_SP_TB_P)||
        (DLT634_5101Slave_App[pdrv].StoreDATA1IN->status.symbol.priority == 0))) 
    {
        DLT634_5101_SLAVE_R_SOE(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp);
        DLT634_5101Slave_App[pdrv].Data1Flag &= (~_DLT634_5101SLAVE_HAVESOE);
        return;
    }

	  if (DLT634_5101Slave_App[pdrv].Data1Flag & _DLT634_5101SLAVE_CALLALLDATA) // 召唤全数据
    {
        DLT634_5101_SLAVE_ReadAllDataProcess(pdrv);
        return;
    }
		
    if(DLT634_5101Slave_App[pdrv].StoreDATA1OUT != DLT634_5101Slave_App[pdrv].StoreDATA1IN)
    {
        memcpy((uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp, (uint8_t *)DLT634_5101Slave_App[pdrv].StoreDATA1OUT, DLT634_5101Slave_App[pdrv].StoreDATA1OUT->status.Length);
        memset((uint8_t *)DLT634_5101Slave_App[pdrv].StoreDATA1OUT,0,sizeof(DLT634_5101SLAVE_PASDU));
        if(++DLT634_5101Slave_App[pdrv].StoreDATA1OUT >= DLT634_5101Slave_App[pdrv].StoreDATA1Buf + _DLT634_5101SLAVE_STOREDATA1NUM)
        {
            DLT634_5101Slave_App[pdrv].StoreDATA1OUT = DLT634_5101Slave_App[pdrv].StoreDATA1Buf;
        }
        return;
    }
}

/**
  * @brief : Data2 Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_Data2Process(uint8_t pdrv)
{
    if(DLT634_5101Slave_App[pdrv].TxdTail_Special)//特殊数据重发
    {
        memcpy(DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail, DLT634_5101Slave_App[pdrv].TxdBuf_Special, DLT634_5101Slave_App[pdrv].TxdTail_Special);
        DLT634_5101Slave_App[pdrv].TxdTail += DLT634_5101Slave_App[pdrv].TxdTail_Special;
        memcpy(DLT634_5101Slave_App[pdrv].TxdBuf_Pri, DLT634_5101Slave_App[pdrv].TxdBuf_Special, DLT634_5101Slave_App[pdrv].TxdTail_Special);
        DLT634_5101Slave_App[pdrv].TxdTail_Pri = DLT634_5101Slave_App[pdrv].TxdTail_Special;
        DLT634_5101Slave_App[pdrv].TxdTail_Special = 0;
        DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
        DLT634_5101Slave_App[pdrv].TimeOutTick_Pri = DLT634_5101Slave_Pad[pdrv].TimeOutValue;
        DLT634_5101Slave_App[pdrv].RetryCount  = _DLT634_5101SLAVE_NUMOF_MAXRETRY;
        DLT634_5101Slave_App[pdrv].LinkFlag &= (~_DLT634_5101SLAVE_SENDABLE);
        return;
    }
    
    if ((DLT634_5101_SLAVE_H_NVA(pdrv))&&
        ((DLT634_5101Slave_App[pdrv].StoreDATA2IN->status.symbol.priority > _DLT634_5101SLAVE_M_ME_NC_P)||
        (DLT634_5101Slave_App[pdrv].StoreDATA2IN->status.symbol.priority == 0))) 
    {
        DLT634_5101_SLAVE_R_NVA(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp);
        return;
    }
    
    if ((DLT634_5101_SLAVE_H_FEvent(pdrv))&&
        ((DLT634_5101Slave_App[pdrv].StoreDATA2IN->status.symbol.priority > _DLT634_5101SLAVE_M_FT_NA_P)||
        (DLT634_5101Slave_App[pdrv].StoreDATA2IN->status.symbol.priority == 0))) 
    {
        DLT634_5101_SLAVE_R_FEvent(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp);
        return;
    }
    
    if(DLT634_5101Slave_App[pdrv].StoreDATA2OUT != DLT634_5101Slave_App[pdrv].StoreDATA2IN)
    {
        memcpy((uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp, (uint8_t *)DLT634_5101Slave_App[pdrv].StoreDATA2OUT, DLT634_5101Slave_App[pdrv].StoreDATA2OUT->status.Length);
        memset((uint8_t *)DLT634_5101Slave_App[pdrv].StoreDATA2OUT,0,sizeof(DLT634_5101SLAVE_PASDU));
        if(++DLT634_5101Slave_App[pdrv].StoreDATA2OUT >= DLT634_5101Slave_App[pdrv].StoreDATA2Buf + _DLT634_5101SLAVE_STOREDATA2NUM)
        {
            DLT634_5101Slave_App[pdrv].StoreDATA2OUT = DLT634_5101Slave_App[pdrv].StoreDATA2Buf;
        }
        return;
    }
    
    DLT634_5101_SLAVE_R_IDLE(pdrv,(uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp);
}

/**
  * @brief : Master Frame 68 Packet.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_MasterFrame68Packet(uint8_t pdrv, uint8_t Function)
{
    uint16_t length = 0;
    uint8_t *pBuf;
    uint8_t num;
    
    pBuf = DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail;

    pBuf[0] = pBuf[3] = _DLT634_5101SLAVE_STARTCODE68;
    pBuf[4] = Function&0x0f;
    pBuf[4] |= _DLT634_5101SLAVE_DIR;
    pBuf[4] |= _DLT634_5101SLAVE_PRM;
    
    num = 5;
    
    if(DLT634_5101Slave_Pad[pdrv].LinkAddrSize == 1)
    {
        pBuf[num++] = DLT634_5101Slave_Pad[pdrv].SourceAddr&0xff;
    }
    else
    {
        pBuf[num++] = DLT634_5101Slave_Pad[pdrv].SourceAddr&0xff;
        pBuf[num++] = (DLT634_5101Slave_Pad[pdrv].SourceAddr>>8)&0xff;
    }
    
    pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.TypeID;
    pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.VSQ;
    
    if(DLT634_5101Slave_Pad[pdrv].ASDUCotSize == 1)
    {
        pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_L;	
    }
    else
    {
        pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_L;
        pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_H;
    }
    
    if(DLT634_5101Slave_Pad[pdrv].ASDUAddrSize == 1)
    {
        pBuf[num++] = DLT634_5101Slave_Pad[pdrv].ASDUAddr&0xff;	
    }
    else
    {
        pBuf[num++] = DLT634_5101Slave_Pad[pdrv].ASDUAddr&0xff;	
        pBuf[num++] = (DLT634_5101Slave_Pad[pdrv].ASDUAddr>>8)&0xff;	
    }

    memcpy(&pBuf[num], &DLT634_5101Slave_App[pdrv].TX_Temp.Data, DLT634_5101Slave_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.status) - sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Head));
    num += DLT634_5101Slave_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.status) - sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Head);
		
    length = num + 2; // 全部报文长度	
    pBuf[1] = pBuf[2] = length - 6;

    pBuf[length-2] = DLT634_5101_SLAVE_CKS(pdrv, pBuf)&0xFF;
    pBuf[length-1] = _DLT634_5101SLAVE_ENDCODE;
    DLT634_5101Slave_App[pdrv].TxdTail += length;

    memcpy(DLT634_5101Slave_App[pdrv].TxdBuf_Pri, pBuf, length);
    DLT634_5101Slave_App[pdrv].TxdTail_Pri = length;
}

/**
  * @brief : Slave Frame 68 Packet.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_SlaveFrame68Packet(uint8_t pdrv, uint8_t Function)
{
    uint16_t length = 0;
    uint8_t *pBuf = 0;
    uint8_t num;
    
    pBuf = DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail;

    pBuf[0] = pBuf[3] = _DLT634_5101SLAVE_STARTCODE68;
    pBuf[4] = Function&0x0f;
    pBuf[4] |= _DLT634_5101SLAVE_DIR;
    pBuf[4] |= _DLT634_5101SLAVE_PRM;
    
    num = 5;
    
    if(DLT634_5101Slave_Pad[pdrv].LinkAddrSize == 1)
    {
        pBuf[num++] = DLT634_5101Slave_Pad[pdrv].SourceAddr&0xff;
    }
    else
    {
        pBuf[num++] = DLT634_5101Slave_Pad[pdrv].SourceAddr&0xff;
        pBuf[num++] = (DLT634_5101Slave_Pad[pdrv].SourceAddr>>8)&0xff;
    }
    
    pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.TypeID;
    pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.VSQ;
    
    if(DLT634_5101Slave_Pad[pdrv].ASDUCotSize == 1)
    {
        pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_L;	
    }
    else
    {
        pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_L;
        pBuf[num++] = DLT634_5101Slave_App[pdrv].TX_Temp.Head.COT_H;
    }
    
    if(DLT634_5101Slave_Pad[pdrv].ASDUAddrSize == 1)
    {
        pBuf[num++] = DLT634_5101Slave_Pad[pdrv].ASDUAddr&0xff;	
    }
    else
    {
        pBuf[num++] = DLT634_5101Slave_Pad[pdrv].ASDUAddr&0xff;	
        pBuf[num++] = (DLT634_5101Slave_Pad[pdrv].ASDUAddr>>8)&0xff;	
    }

    memcpy(&pBuf[num], &DLT634_5101Slave_App[pdrv].TX_Temp.Data, DLT634_5101Slave_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.status) - sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Head));
    num += DLT634_5101Slave_App[pdrv].TX_Temp.status.Length - sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.status) - sizeof(DLT634_5101Slave_App[pdrv].TX_Temp.Head);
		
    length = num + 2; // 全部报文长度	
    pBuf[1] = pBuf[2] = length - 6;
    
    pBuf[length-2] = DLT634_5101_SLAVE_CKS(pdrv, pBuf)&0xFF;
    pBuf[length-1] = _DLT634_5101SLAVE_ENDCODE;
    DLT634_5101Slave_App[pdrv].TxdTail += length;

    memcpy(DLT634_5101Slave_App[pdrv].TxdBuf_Pri, pBuf, length);
    DLT634_5101Slave_App[pdrv].TxdTail_Pri = length;
}

/**
  * @brief : Set Flag Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_SetFlagProcess(uint8_t pdrv)
{
    if(DLT634_5101_SLAVE_H_COS(pdrv))//soe
    {DLT634_5101Slave_App[pdrv].Data1Flag |= _DLT634_5101SLAVE_HAVECOS;}
    if(DLT634_5101_SLAVE_H_SOE(pdrv))//soe
    {DLT634_5101Slave_App[pdrv].Data1Flag |= _DLT634_5101SLAVE_HAVESOE;}
    if(DLT634_5101Slave_App[pdrv].StoreDATA1OUT->status.Length)
    {DLT634_5101Slave_App[pdrv].Data1Flag |= _DLT634_5101SLAVE_HAVESTDATA1;}
}

/**
  * @brief : Link 68 Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_Link68Process(uint8_t pdrv)
{
    DLT634_5101_SLAVE_SetFlagProcess(pdrv);

    if(DLT634_5101Slave_App[pdrv].TX_Temp.status.Length)
    {
        if(DLT634_5101Slave_Pad[pdrv].BalanMode)
        {
            DLT634_5101_SLAVE_MasterFrame68Packet(pdrv, _DLT634_5101SLAVE_M_FUN3);
            DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
            DLT634_5101Slave_App[pdrv].TimeOutTick_Pri = DLT634_5101Slave_Pad[pdrv].TimeOutValue;
            DLT634_5101Slave_App[pdrv].RetryCount  = _DLT634_5101SLAVE_NUMOF_MAXRETRY;
            DLT634_5101Slave_App[pdrv].LinkFlag &= (~_DLT634_5101SLAVE_SENDABLE);
        }
        else
        {
            DLT634_5101_SLAVE_SlaveFrame68Packet(pdrv, _DLT634_5101SLAVE_M_FUN8);
            DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
        }
        memset((uint8_t *)&DLT634_5101Slave_App[pdrv].TX_Temp,0,sizeof(DLT634_5101SLAVE_PASDU));
    }
    else
    {
        if (!DLT634_5101Slave_Pad[pdrv].BalanMode)
        {
            if(_DLT634_5101SLAVE_E5)
            {
                *(DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail) = 0xE5;
                DLT634_5101Slave_App[pdrv].TxdTail += 1;
            }
            else
            {
                DLT634_5101_SLAVE_SlaveFrame10Packet(pdrv, _DLT634_5101SLAVE_S_FUN9);
            }
            DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
        }
    }
}

/**
  * @brief : Send Process.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5101_SLAVE_SendProcess(uint8_t pdrv)
{   
	if(DLT634_5101Slave_Pad[pdrv].Encrypt)
	{
		if(DLT634_5101_SLAVE_H_Encrypt(pdrv))
		{return;}
	}
	
    if(DLT634_5101Slave_App[pdrv].LinkFlag&0x000000ff)
    {
        DLT634_5101_SLAVE_Link10Process(pdrv);
        return;
    }
    
    if(DLT634_5101Slave_Pad[pdrv].BalanMode)
    {
        if(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_SENDABLE)
        {
            DLT634_5101_SLAVE_Data1Process(pdrv);
            if(DLT634_5101Slave_App[pdrv].TX_Temp.status.Length == 0)
            {DLT634_5101_SLAVE_Data2Process(pdrv);}
            DLT634_5101_SLAVE_Link68Process(pdrv);
            return;
        }
    }
    else
    {
        if(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_REDATA1)
        {	
            DLT634_5101_SLAVE_Data1Process(pdrv);
            DLT634_5101_SLAVE_Link68Process(pdrv);
        }	
        else if(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_REDATA2)
        {
            DLT634_5101_SLAVE_Data2Process(pdrv);
            if(DLT634_5101Slave_App[pdrv].TX_Temp.status.Length == 0)
            {DLT634_5101_SLAVE_Data1Process(pdrv);}
            DLT634_5101_SLAVE_Link68Process(pdrv);
        }
        DLT634_5101Slave_App[pdrv].LinkFlag &= (~(_DLT634_5101SLAVE_REDATA1|_DLT634_5101SLAVE_REDATA2));
        return;
    }
}

/**
  * @brief : Link On Timer.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5101_SLAVE_LinkOnTimer(uint8_t pdrv)
{
    if (DLT634_5101Slave_App[pdrv].TimeOutTick_Pri) // 单位是秒
    {
        DLT634_5101Slave_App[pdrv].TimeOutTick_Pri--;
        if (!DLT634_5101Slave_App[pdrv].TimeOutTick_Pri)
        {
            if (!DLT634_5101Slave_App[pdrv].RetryCount)
            {
                return;
            }

            DLT634_5101Slave_App[pdrv].RetryCount--;
            if (DLT634_5101Slave_App[pdrv].RetryCount)
            {
                memcpy(DLT634_5101Slave_App[pdrv].TxdBuf + DLT634_5101Slave_App[pdrv].TxdTail, DLT634_5101Slave_App[pdrv].TxdBuf_Pri, DLT634_5101Slave_App[pdrv].TxdTail_Pri);
                DLT634_5101Slave_App[pdrv].TxdTail += DLT634_5101Slave_App[pdrv].TxdTail_Pri;

                DLT634_5101_SLAVE_WriteDataToMISI(pdrv);
                DLT634_5101Slave_App[pdrv].TimeOutTick_Pri = DLT634_5101Slave_Pad[pdrv].TimeOutValue;
                return;
            }
            else // 超过重发次数后，重新建立链路，并通知应用层。
            {
                DLT634_5101Slave_App[pdrv].LinkFlag = 0;
                DLT634_5101Slave_App[pdrv].TimeOutTick_AskCount = _DLT634_5101SLAVE_NUMOF_MAXRETRY;
                DLT634_5101Slave_App[pdrv].TimeOutTick_AskSta = DLT634_5101Slave_Pad[pdrv].AskStaOutValue;
                if(DLT634_5101Slave_App[pdrv].TxdBuf_Pri[5+DLT634_5101Slave_Pad[pdrv].LinkAddrSize]& (_DLT634_5101SLAVE_M_SP_TB_1|_DLT634_5101SLAVE_M_DP_TB_1))
                {
                    DLT634_5101Slave_App[pdrv].TxdTail_Special= DLT634_5101Slave_App[pdrv].TxdTail_Pri;
                    memcpy(DLT634_5101Slave_App[pdrv].TxdBuf_Special, DLT634_5101Slave_App[pdrv].TxdBuf_Pri, DLT634_5101Slave_App[pdrv].TxdTail_Pri);
                }
            }
        }
    }
		
    if(DLT634_5101Slave_App[pdrv].TimeOutTick_AskSta)
    {
        DLT634_5101Slave_App[pdrv].TimeOutTick_AskSta--;
        if (!DLT634_5101Slave_App[pdrv].TimeOutTick_AskSta)
        {
            if (!DLT634_5101Slave_App[pdrv].TimeOutTick_AskCount)
            {
                return;
            }
            if((!(DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_INITEND))&&(DLT634_5101Slave_Pad[pdrv].BalanMode))
            {
                DLT634_5101Slave_App[pdrv].LinkFlag |= _DLT634_5101SLAVE_ASKSTATUS;
                DLT634_5101Slave_App[pdrv].TimeOutTick_AskCount--;
            }
        }
    }
    
    if(DLT634_5101Slave_App[pdrv].TimeOutLink)
    {
        DLT634_5101Slave_App[pdrv].TimeOutLink--;
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
uint8_t DLT634_5101_SLAVE_REPLY(uint8_t pdrv, uint8_t *pbuf)//回复
{ 
	uint8_t res = FALSE;
	
	memcpy(&DLT634_5101Slave_App[pdrv].ST_Temp, pbuf,_DLT634_5101SLAVE_LPDUSIZE);
	switch (DLT634_5101Slave_App[pdrv].ST_Temp.Head.TypeID)
	{
        //控制方向过程信息
        case _DLT634_5101SLAVE_C_SC_NA_1: // 单点遥控命令
        case _DLT634_5101SLAVE_C_SC_NB_1: // 双点遥控命令
            DLT634_5101Slave_App[pdrv].ST_Temp.status.symbol.priority = _DLT634_5101SLAVE_C_SC_NA_P;
            res = DLT634_5101_SLAVE_StoreIN(pdrv,&DLT634_5101Slave_App[pdrv].ST_Temp);
                break;

        case _DLT634_5101SLAVE_C_CS_NA_1: // 对时命令
            DLT634_5101Slave_App[pdrv].ST_Temp.status.symbol.priority = _DLT634_5101SLAVE_C_CS_NA_P;
            res = DLT634_5101_SLAVE_StoreIN(pdrv,&DLT634_5101Slave_App[pdrv].ST_Temp);
                break;

        case _DLT634_5101SLAVE_C_RP_NA_1: // 复位进程命令
                break;
        
        case _DLT634_5101SLAVE_C_RR_NA_1: // 读定值区号
        case _DLT634_5101SLAVE_C_RS_NA_1: // 读参数和定值命令
            DLT634_5101Slave_App[pdrv].ST_Temp.status.symbol.priority = _DLT634_5101SLAVE_C_IC_NA_P;
            res = DLT634_5101_SLAVE_StoreIN(pdrv,&DLT634_5101Slave_App[pdrv].ST_Temp);
                break;
        
        case _DLT634_5101SLAVE_C_SR_NA_1: // 切换定值区
        case _DLT634_5101SLAVE_C_WS_NA_1: // 写参数和定值命令
            DLT634_5101Slave_App[pdrv].ST_Temp.status.symbol.priority = _DLT634_5101SLAVE_C_SC_NA_P;
            res = DLT634_5101_SLAVE_StoreIN(pdrv,&DLT634_5101Slave_App[pdrv].ST_Temp);
                break;

        case _DLT634_5101SLAVE_F_FR_NA_1: // 文件传输
            DLT634_5101Slave_App[pdrv].ST_Temp.status.symbol.priority = _DLT634_5101SLAVE_F_FW_NA_P;
            res = DLT634_5101_SLAVE_StoreIN(pdrv,&DLT634_5101Slave_App[pdrv].ST_Temp);
                break;

        case _DLT634_5101SLAVE_F_SR_NA_1: // 软件升级
            DLT634_5101Slave_App[pdrv].ST_Temp.status.symbol.priority = _DLT634_5101SLAVE_F_FW_NA_P;
            res = DLT634_5101_SLAVE_StoreIN(pdrv,&DLT634_5101Slave_App[pdrv].ST_Temp);
                break;			

        default: // 类型标识有错误或不支持
                break;
	}
	return(res);
}

/**
  * @brief : App Init.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5101_SLAVE_AppInit(uint8_t pdrv)
{
	if (DLT634_5101Slave_App == RT_NULL)
	{
		DLT634_5101Slave_App = rt_malloc(sizeof(DLT634_5101SLAVE_APPINFO) * _DLT634_5101SLAVE_VOLUMES);
        memset (DLT634_5101Slave_App, 0, sizeof (DLT634_5101SLAVE_APPINFO) * _DLT634_5101SLAVE_VOLUMES);
	}
	  
    return(TRUE);
}

/**
  * @brief : Clock work.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5101_SLAVE_Clock(uint8_t pdrv)
{
    uint8_t sta = FALSE;
    
	DLT634_5101Slave_App[pdrv].ClockCounter++;
	if (DLT634_5101Slave_App[pdrv].ClockCounter >= DLT634_5101Slave_Pad[pdrv].ClockTimers)
	{
        DLT634_5101Slave_App[pdrv].ClockCounter = 0;
                                            
        // 查询MISI接收缓冲区
        DLT634_5101_SLAVE_ReadMISIData(pdrv);
    
        // 发送处理
        if(!(channel_monitor.MonitorFlag[pdrv] & _ChannelToMonitor1_START))
        {
            DLT634_5101_SLAVE_SendProcess(pdrv);
            
            // 链路层定时器
            DLT634_5101_SLAVE_LinkOnTimer(pdrv);
        }
	}
    
        // 返回链路状态
    if((DLT634_5101Slave_App[pdrv].LinkFlag&_DLT634_5101SLAVE_INITEND)&&(DLT634_5101Slave_App[pdrv].TimeOutLink))
    {sta = TRUE;}
    return(sta);
}


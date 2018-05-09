/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5104slave_disk.c
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    Mr.Sun
  * @date:      2017.12.19
  * @update:    
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include "string.h"
#include "common_data.h"
#include "dlt634_5104slave_app.h"
#include "dlt634_5104slave_disk.h"
#include "channel_monitor.h"
/* PRIVATE VARIABLES ----------------------------------------------------------*/

/* PUBLIC VARIABLES ----------------------------------------------------------*/
struct DLT634_5104SLAVE_PAD DLT634_5104Slave_Pad[DLT634_5104SLAVE_VOLUMES]; // DL/T634_5101规约参数
struct DLT634_5104SLAVE_LINKINFO *DLT104SlaveLink = RT_NULL;//[DLT634_5104SLAVE_VOLUMES]; // 链路层变量
struct DLT634_5104SLAVE_APPINFO *DLT104SlaveApp = RT_NULL;//[DLT634_5104SLAVE_VOLUMES] __attribute__((at(0x00002000 + SDRAM_ADDR_104)));

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : Set Timer.
  * @param : [pdrv]
  * @param : [No]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_SetTimer(uint8_t pdrv, uint8_t No)
{
    uint8_t	i = 0;
    
    switch (No)
    {
        case 0:
            DLT104SlaveLink[pdrv].Tick.TValue0 = 0;
            DLT104SlaveLink[pdrv].Tick.Flag |= DLT634_5104SLAVE_T0FLAG;
            break;
            
        case 1:
            for (i = 0; i < DLT634_5104SLAVE_K; i++)
            {
                DLT104SlaveLink[pdrv].Tick.TValue1[i] = 0;
            }
            DLT104SlaveLink[pdrv].Tick.Flag |= DLT634_5104SLAVE_T1FLAG;			
            break;
            
        case 2:
            DLT104SlaveLink[pdrv].Tick.TValue2 = 0;
            DLT104SlaveLink[pdrv].Tick.Flag |= DLT634_5104SLAVE_T2FLAG;			
            break;
            
        case 3:
            DLT104SlaveLink[pdrv].Tick.TValue3 = 0;
            DLT104SlaveLink[pdrv].Tick.Flag |= DLT634_5104SLAVE_T3FLAG;			
            break;
        
        default:
            break;
    }
}

/**
  * @brief : Kill Timer.
  * @param : [pdrv]
  * @param : [No]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_KillTimer(uint8_t pdrv, uint8_t No)
{
    switch (No)
    {
        case 0:
            DLT104SlaveLink[pdrv].Tick.Flag &= (~DLT634_5104SLAVE_T0FLAG);
            break;
            
        case 1:
            DLT104SlaveLink[pdrv].Tick.Flag &= (~DLT634_5104SLAVE_T1FLAG);
            break;
            
        case 2:
            DLT104SlaveLink[pdrv].Tick.Flag &= (~DLT634_5104SLAVE_T2FLAG);			
            break;
            
        case 3:
            DLT104SlaveLink[pdrv].Tick.Flag &= (~DLT634_5104SLAVE_T3FLAG);			
            break;
            
        default:
            break;
    }
}

/**
  * @brief : Kill Timer.
  * @param : [pdrv]
  * @param : [LastNoAckNum]
  * @param : [NowNoAckNum]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_BackupProcess(uint8_t pdrv, uint16_t LastNoAckNum, uint16_t NowNoAckNum)
{
    uint16_t num;
    struct DLT634_5104SLAVE_APPINFO *pMsg;

    pMsg = &DLT104SlaveApp[pdrv]; 	
    num = LastNoAckNum - NowNoAckNum; // 本次被确认的数量
	
    memcpy(pMsg->SData, &pMsg->SData[num], sizeof(struct DLT634_5104SLAVE_SendData) * LastNoAckNum); 

    pMsg->CurSData = (pMsg->CurSData - num) >= 0 ? pMsg->CurSData - num : 0;
	pMsg->SDataIn = (pMsg->SDataIn - num) >= 0 ? pMsg->SDataIn - num : 0;;
}

/**
  * @brief : Peer No Ack Num Process.
  * @param : [pdrv]
  * @param : [PeerNR]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_5104_SLAVE_PeerNoAckNumProcess(uint8_t pdrv, uint16_t PeerNR)
{
    uint16_t i = 0;
    uint16_t tmp = 0, num = 0;
    
    if (PeerNR <= DLT104SlaveLink[pdrv].NS)
    {
        num = DLT104SlaveLink[pdrv].NS - PeerNR;
    }
    else
    {
        num = 32767 + DLT104SlaveLink[pdrv].NS - PeerNR;	  	
    }
    
    if (num > DLT104SlaveLink[pdrv].PeerNoAckNum)
    {
        return (FALSE);
    }

    for (i = 0, tmp = 0; i < DLT104SlaveLink[pdrv].PeerNoAckNum - num; i++)
    {
        if (DLT104SlaveLink[pdrv].DataType[i])
        {
            tmp++;
        }
    }

	DLT634_5104_SLAVE_BackupProcess(pdrv, DLT104SlaveLink[pdrv].PeerNoAckNum, num); // 未被确认I帧备份处理
    
    for (i = 0; i < num; i++)
    {
        DLT104SlaveLink[pdrv].Tick.TValue1[i] = DLT104SlaveLink[pdrv].Tick.TValue1[num+i];
        DLT104SlaveLink[pdrv].DataType[i] = DLT104SlaveLink[pdrv].DataType[num+i];
    }
    
    for (i = num; i < DLT634_5104SLAVE_K; i++)
    {
        DLT104SlaveLink[pdrv].Tick.TValue1[i] = 0;
        DLT104SlaveLink[pdrv].DataType[i] = FALSE;
    }
    
    DLT104SlaveLink[pdrv].PeerNoAckNum = num;	
    
    return (TRUE);
}

/**
  * @brief : Store IN.
  * @param : [pdrv]
  * @param : [buf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t DLT634_5104_SLAVE_StoreIN(uint8_t pdrv, DLT634_5104SLAVE_PASDU *buf)
{		
	DLT634_5104SLAVE_PASDU *temp;
	DLT634_5104SLAVE_PASDU tempbuf;
	
	if(buf->status.symbol.priority<DLT634_5104SLAVE_M_FT_NA_P)
	{
		temp = DLT104SlaveApp[pdrv].StoreDATA1IN;
		if(++temp >= DLT104SlaveApp[pdrv].StoreDATA1Buf + DLT634_5104SLAVE_STOREDATA1NUM)
		{
			temp = DLT104SlaveApp[pdrv].StoreDATA1Buf;
		}
		
		if(temp == DLT104SlaveApp[pdrv].StoreDATA1OUT)
		{
			return(FALSE);
		}
	
		temp = DLT104SlaveApp[pdrv].StoreDATA1OUT;
		while(temp != DLT104SlaveApp[pdrv].StoreDATA1IN)
		{
			if(((temp->status.symbol.priority > buf->status.symbol.priority)||(temp->status.symbol.priority == 0))&&(temp != DLT104SlaveApp[pdrv].StoreDATA1OUT))
			{
				memcpy(&tempbuf,temp,sizeof(DLT634_5104SLAVE_PASDU));
				memcpy(temp,buf,sizeof(DLT634_5104SLAVE_PASDU));
				memcpy(buf,&tempbuf,sizeof(DLT634_5104SLAVE_PASDU));
			}
			if(++temp >= DLT104SlaveApp[pdrv].StoreDATA1Buf + DLT634_5104SLAVE_STOREDATA1NUM)
			{
				temp = DLT104SlaveApp[pdrv].StoreDATA1Buf;
			}
		}
		
		memcpy(temp,buf,sizeof(DLT634_5104SLAVE_PASDU));	
		if(++DLT104SlaveApp[pdrv].StoreDATA1IN >= DLT104SlaveApp[pdrv].StoreDATA1Buf + DLT634_5104SLAVE_STOREDATA1NUM)
		{
			DLT104SlaveApp[pdrv].StoreDATA1IN = DLT104SlaveApp[pdrv].StoreDATA1Buf;
		}
	}
	else
	{
		temp = DLT104SlaveApp[pdrv].StoreDATA2IN;
		if(++temp >= DLT104SlaveApp[pdrv].StoreDATA2Buf + DLT634_5104SLAVE_STOREDATA2NUM)
		{
			temp = DLT104SlaveApp[pdrv].StoreDATA2Buf;
		}
		
		if(temp == DLT104SlaveApp[pdrv].StoreDATA2OUT)
		{
			return(FALSE);
		}
		
		temp = DLT104SlaveApp[pdrv].StoreDATA2OUT;
		while(temp != DLT104SlaveApp[pdrv].StoreDATA2IN)
		{
			if(((temp->status.symbol.priority > buf->status.symbol.priority)||(temp->status.symbol.priority == 0))&&(temp != DLT104SlaveApp[pdrv].StoreDATA2OUT))
			{
				memcpy(&tempbuf,temp,sizeof(DLT634_5104SLAVE_PASDU));
				memcpy(temp,buf,sizeof(DLT634_5104SLAVE_PASDU));
				memcpy(buf,&tempbuf,sizeof(DLT634_5104SLAVE_PASDU));
			}
			if(++temp >= DLT104SlaveApp[pdrv].StoreDATA2Buf + DLT634_5104SLAVE_STOREDATA2NUM)
			{
				temp = DLT104SlaveApp[pdrv].StoreDATA2Buf;
			}
		}
		
		memcpy(temp,buf,sizeof(DLT634_5104SLAVE_PASDU));
		if(++DLT104SlaveApp[pdrv].StoreDATA2IN >= DLT104SlaveApp[pdrv].StoreDATA2Buf + DLT634_5104SLAVE_STOREDATA2NUM)
		{
			DLT104SlaveApp[pdrv].StoreDATA2IN = DLT104SlaveApp[pdrv].StoreDATA2Buf;
		}
	}
	return(TRUE);
}

/**
  * @brief : App Init Reset.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_AppInitReset(uint8_t pdrv)
{
    DLT104SlaveApp[pdrv].AllDataFlag = FALSE;
    DLT104SlaveApp[pdrv].AppNextFlag = FALSE;    
	DLT104SlaveApp[pdrv].Data1Flag = 0;
    DLT104SlaveApp[pdrv].SDataEn = 1;	
    DLT104SlaveApp[pdrv].SDataOut = 0;
	
    // Link
    DLT104SlaveLink[pdrv].Connect = FALSE;
    DLT104SlaveLink[pdrv].NR = 0;
    DLT104SlaveLink[pdrv].NS = 0;
    DLT104SlaveLink[pdrv].PeerNoAckNum = 0;
    DLT104SlaveLink[pdrv].FrameHead = 0;
    DLT104SlaveLink[pdrv].TxdHead = 0;
    DLT104SlaveLink[pdrv].TxdTail = 0;	
	DLT104SlaveLink[pdrv].StopSendFlag = FALSE; // 清停止发送标志
	
    memset(DLT104SlaveLink[pdrv].TxdBuf, 0, sizeof(DLT104SlaveLink[pdrv].RxdBuf)); // 清空接收缓冲区
    memset(DLT104SlaveLink[pdrv].RxdBuf, 0, sizeof(DLT104SlaveLink[pdrv].TxdBuf)); // 清空发送缓冲区

    DLT104SlaveApp[pdrv].StoreDATA1IN = DLT104SlaveApp[pdrv].StoreDATA1Buf;
    DLT104SlaveApp[pdrv].StoreDATA1OUT = DLT104SlaveApp[pdrv].StoreDATA1Buf;
    memset(DLT104SlaveApp[pdrv].StoreDATA1Buf,0,sizeof(DLT104SlaveApp[pdrv].StoreDATA1Buf));
    DLT104SlaveApp[pdrv].StoreDATA2IN = DLT104SlaveApp[pdrv].StoreDATA2Buf;
    DLT104SlaveApp[pdrv].StoreDATA2OUT = DLT104SlaveApp[pdrv].StoreDATA2Buf;
    memset(DLT104SlaveApp[pdrv].StoreDATA2Buf,0,sizeof(DLT104SlaveApp[pdrv].StoreDATA2Buf));

    return (TRUE);
}

/**
  * @brief : App Process.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_AppProcess(uint8_t pdrv, struct DLT634_5104SLAVE_AppMsg* Msg)
{	
    DLT104SlaveApp[pdrv].TxLen = 0;
    
//    DLT104SlaveApp[pdrv].RxMsg = (struct DLT634_5104SLAVE_PASDU_104*)Msg->pData;
//    memcpy((uint8_t *)&DLT104SlaveApp[pdrv].RxMsg.Head,Msg->pData, Msg->Len);
//  这里将104报文转为02版104报文	
	memset((uint8_t *)&DLT104SlaveApp[pdrv].RX_Temp,0,sizeof(DLT634_5104SLAVE_PASDU));
	memcpy((uint8_t *)&DLT104SlaveApp[pdrv].RX_Temp.Head,Msg->pData, Msg->Len);
    DLT104SlaveApp[pdrv].RX_Temp.status.Length = Msg->Len + sizeof(DLT104SlaveApp[pdrv].RX_Temp.status);

	switch (Msg->Cmd)
    {
        case DLT634_5104SLAVE_LINK_CALLDATA:   
            switch (DLT104SlaveApp[pdrv].RX_Temp.Head.TypeID) // 具体类型标识处理
            {
                case DLT634_5104SLAVE_C_SC_NA_1:	// <45>单点遥控命令
                case DLT634_5104SLAVE_C_SC_NB_1:	// <46>双点遥控命令
                    DLT634_5104_SLAVE_HandleCtrlProcess(pdrv, (uint8_t *)&DLT104SlaveApp[pdrv].RX_Temp);
                    break;                        
                        
                case DLT634_5104SLAVE_C_IC_NA_1:	// <100>总召唤或分组召唤
                    DLT104SlaveApp[pdrv].Data1Flag |= DLT634_5104SLAVE_CALLALLDATA;
                    DLT104SlaveApp[pdrv].AllDataFlag = 0;
                    DLT634_5104_SLAVE_CallAllDataProcess(pdrv,(uint8_t *)&DLT104SlaveApp[pdrv].RX_Temp);
                    break;
                        
                case DLT634_5104SLAVE_C_CI_NA_1:	// <101>电度总召唤或分组召唤
                    break;
                        
                case DLT634_5104SLAVE_C_CS_NA_1:	// <103>对钟命令
                    DLT634_5104_SLAVE_SYNProcess(pdrv, (uint8_t *)&DLT104SlaveApp[pdrv].RX_Temp);
                    break;
                        
                case DLT634_5104SLAVE_C_RP_NA_1:	// <105>复位进程命令
                    DLT104SlaveApp[pdrv].Data1Flag |= DLT634_5104SLAVE_CallReset;
                    DLT634_5104_SLAVE_ResetProcess(pdrv, (uint8_t *)&DLT104SlaveApp[pdrv].RX_Temp);
                    break;                        
                        
                case DLT634_5104SLAVE_C_TS_NA_1:	// <104>测试命令
                    DLT104SlaveApp[pdrv].RX_Temp.status.symbol.priority = DLT634_5104SLAVE_C_TS_NA_P;
                    DLT104SlaveApp[pdrv].RX_Temp.Head.COT_L = DLT634_5104SLAVE_COT_ACTCON;
                    DLT634_5104_SLAVE_StoreIN(pdrv,&DLT104SlaveApp[pdrv].RX_Temp);								
                    break;   
                
                case DLT634_5104SLAVE_C_SR_NA_1: // <200>切换定值区
                case DLT634_5104SLAVE_C_RR_NA_1: // <201>读定值区号
                case DLT634_5104SLAVE_C_RS_NA_1: // <202>读参数和定值
                case DLT634_5104SLAVE_C_WS_NA_1: // <203>写参数和定值	
				    DLT634_5104_SLAVE_FixedParaProcess(pdrv,(uint8_t *)&DLT104SlaveApp[pdrv].RX_Temp);			
                    break;
                
                case DLT634_5104SLAVE_F_FR_NA_1: // <210>文件传输
                    DLT634_5104_SLAVE_FileHandleProcess(pdrv,(uint8_t *)&DLT104SlaveApp[pdrv].RX_Temp);
                    break;
								
                case DLT634_5104SLAVE_F_SR_NA_1: // <211>软件升级
                    DLT634_5104_SLAVE_SoftwareUpdate(pdrv,(uint8_t *)&DLT104SlaveApp[pdrv].RX_Temp);  
                    break;
								
                default:	// 类型标识有错误或不支持
                    break;
            }
            break;
                
        case DLT634_5104SLAVE_LINK_APPCON:
            break;	

        case DLT634_5104SLAVE_LINK_WORK:
            DLT104SlaveApp[pdrv].Connect = TRUE; // 收到STSRTDT后
            break;
                
        case DLT634_5104SLAVE_LINK_NOWORK:
            DLT634_5104_AppInitReset(pdrv);
            DLT104SlaveApp[pdrv].Connect = FALSE;
            break;
        
        default:
            break; 
    }				  
}

/**
  * @brief : Stop DT Oper.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_StopDTOper(uint8_t pdrv)
{
    struct DLT634_5104SLAVE_AppMsg	msg;
	
    msg.Cmd = DLT634_5104SLAVE_LINK_NOWORK;
    msg.ConNum = 0;
    msg.pData = NULL;
    msg.Len = 0;
    DLT634_5104_SLAVE_AppProcess(pdrv, &msg);	
    
    DLT634_5104_SLAVE_StopLink(pdrv);
    DLT104SlaveLink[pdrv].Connect = FALSE;	// 清链路连接标志
}

/**
  * @brief : Write Data To MISI.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_WriteDataToMISI(uint8_t pdrv)
{
    uint16_t count = 0, num = 0;
    
    if (DLT104SlaveLink[pdrv].TxdHead >= DLT104SlaveLink[pdrv].TxdTail)
    {
        return ;
    }
    
    num = DLT104SlaveLink[pdrv].TxdTail - DLT104SlaveLink[pdrv].TxdHead;
    
    // 写数据库函数
    count = DLT634_5104_SLAVE_WriteData(pdrv,(uint8_t *)&DLT104SlaveLink[pdrv].TxdBuf[DLT104SlaveLink[pdrv].TxdHead], num);
    DLT104SlaveLink[pdrv].TxdHead += count;
    
    if (DLT104SlaveLink[pdrv].TxdHead >= DLT104SlaveLink[pdrv].TxdTail)
    {
        DLT104SlaveLink[pdrv].TxdTail = 0;
        DLT104SlaveLink[pdrv].TxdHead = 0;
    }
}

/**
  * @brief : Send Ctrl Frame.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_S104SendCtrlFrame(uint8_t pdrv, uint8_t ctrl)
{
    struct DLT634_5104SLAVE_PAPCI *pFrame;
    
    pFrame = (struct DLT634_5104SLAVE_PAPCI*)&DLT104SlaveLink[pdrv].TxdBuf[DLT104SlaveLink[pdrv].TxdHead];
    
    pFrame->StartCode = DLT634_5104SLAVE_STARTCODE68;
    pFrame->Length = 0x04;
    pFrame->NS = ctrl;
    if (ctrl == DLT634_5104SLAVE_S_FRAME)
    {
        DLT104SlaveLink[pdrv].NRACK = DLT104SlaveLink[pdrv].NR;
        pFrame->NR = DLT104SlaveLink[pdrv].NR << 1;

        DLT634_5104_SLAVE_KillTimer(pdrv, 2);
    }
    else
    {
        pFrame->NR = 0x00;
			
        DLT634_5104_SLAVE_SetTimer(pdrv, 3);
			
        if (ctrl == DLT634_5104SLAVE_U_TESTFRACT)
        {
            DLT634_5104_SLAVE_SetTimer(pdrv, 1); // U测试针应在t1时间内得到确认
            DLT104SlaveLink[pdrv].PeerNoAckNum++;
        }
    }
    
    DLT104SlaveLink[pdrv].TxdTail += 0x06;
    
    // 发送										
    DLT634_5104_SLAVE_WriteDataToMISI(pdrv);
    
    DLT104SlaveApp[pdrv].Data1Flag |= DLT634_5104SLAVE_SENDDATA;
}

/**
  * @brief : Decode S Frame.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_DecodeSFrame(uint8_t pdrv)
{
    uint16_t tmp = 0;
    struct DLT634_5104SLAVE_PAPCI *pAPCI;
    struct DLT634_5104SLAVE_AppMsg	msg;
    
    pAPCI = (struct DLT634_5104SLAVE_PAPCI*)DLT104SlaveLink[pdrv].RxdBuf;
    
    if (!DLT104SlaveLink[pdrv].Connect)
    {
        return;
    }
    
    tmp = DLT104SlaveLink[pdrv].PeerNoAckNum;
    if (!DLT634_5104_SLAVE_PeerNoAckNumProcess(pdrv, pAPCI->NR >> 1))
    {
        return;
    }
    DLT104SlaveLink[pdrv].StopSendFlag = FALSE;
		
    msg.Cmd = DLT634_5104SLAVE_LINK_APPCON; 
    msg.ConNum = tmp - DLT104SlaveLink[pdrv].PeerNoAckNum;
    msg.pData = NULL;
    msg.Len = 0;
    DLT634_5104_SLAVE_AppProcess(pdrv, &msg);
}

/**
  * @brief : Decode U Frame.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_DecodeUFrame(uint8_t pdrv)
{
    struct DLT634_5104SLAVE_PAPCI *pAPCI;
    struct DLT634_5104SLAVE_AppMsg	msg;
            
    pAPCI = (struct DLT634_5104SLAVE_PAPCI*)DLT104SlaveLink[pdrv].RxdBuf;
    
    switch (pAPCI->NS & 0xFF)
    {
        case DLT634_5104SLAVE_U_STARTDTACT:
            // 是否初始化
            DLT104SlaveLink[pdrv].Tick.Flag = DLT104SlaveLink[pdrv].Tick.FlagBak;
            DLT104SlaveApp[pdrv].Data1Flag |= DLT634_5104SLAVE_HAVEINITEND;
            
            DLT634_5104_SLAVE_S104SendCtrlFrame(pdrv, DLT634_5104SLAVE_U_STARTDTCON);  
            DLT104SlaveLink[pdrv].Connect = TRUE; 
        
            msg.Cmd = DLT634_5104SLAVE_LINK_WORK;
            msg.ConNum = 0;
            msg.pData = NULL;
            msg.Len = 0;
            DLT634_5104_SLAVE_AppProcess(pdrv, &msg);					
            break;
            
        case DLT634_5104SLAVE_U_STARTDTCON: // 终端 应该不会收到该命令
            break;
    
        case DLT634_5104SLAVE_U_STOPDTACT:
            DLT104SlaveLink[pdrv].Tick.FlagBak = DLT104SlaveLink[pdrv].Tick.Flag;
            DLT104SlaveLink[pdrv].Tick.Flag = 0;
				    /*回复停止确认之前需对控制站未确认报文进行确认，若被控站存在未确认报文 需等待确认*/
            if (DLT104SlaveLink[pdrv].NR > DLT104SlaveLink[pdrv].NRACK)
            {
                DLT634_5104_SLAVE_S104SendCtrlFrame(pdrv, DLT634_5104SLAVE_S_FRAME); // 收到主站U_STOPDTACT命令
            }
            else
            {
                DLT634_5104_SLAVE_S104SendCtrlFrame(pdrv, DLT634_5104SLAVE_U_STOPDTCON);
    
                msg.Cmd = DLT634_5104SLAVE_LINK_NOWORK;
                msg.ConNum = 0;
                msg.pData = NULL;
                msg.Len = 0;
                DLT634_5104_SLAVE_AppProcess(pdrv, &msg);									
            }
            break;
    
        case DLT634_5104SLAVE_U_STOPDTCON: // 终端 应该不会收到该命令
            break;
    
        case DLT634_5104SLAVE_U_TESTFRACT:
            DLT634_5104_SLAVE_S104SendCtrlFrame(pdrv, DLT634_5104SLAVE_U_TESTFRCON); 
            break;
    
        case DLT634_5104SLAVE_U_TESTFRCON:
				    /*收到测试确认后 应清t1, 此时t1应该只有测试计时*/	   
            break;
            
        default:
            break;
    }
}

/**
  * @brief : Decode I Frame.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_DecodeIFrame(uint8_t pdrv)
{
    uint16_t tmp = 0;
    struct DLT634_5104SLAVE_AppMsg msg;
    struct DLT634_5104SLAVE_PAPCI *pAPCI;
    
    pAPCI = (struct DLT634_5104SLAVE_PAPCI*)DLT104SlaveLink[pdrv].RxdBuf;
    
    if (!DLT104SlaveLink[pdrv].Connect)  
    {
        return;
    }
    
    tmp = DLT104SlaveLink[pdrv].PeerNoAckNum;
    if (!DLT634_5104_SLAVE_PeerNoAckNumProcess(pdrv, pAPCI->NR >> 1))
    {
        DLT634_5104_SLAVE_StopDTOper(pdrv);
        return;
    }

    if (DLT104SlaveLink[pdrv].NR == 32767)
    {
        DLT104SlaveLink[pdrv].NR = 0;
    }
    else
    {
        DLT104SlaveLink[pdrv].NR++;
    }

    DLT104SlaveLink[pdrv].StopSendFlag = FALSE; // 清停止发送标志
		
    DLT634_5104_SLAVE_KillTimer(pdrv, 1);
    DLT634_5104_SLAVE_SetTimer(pdrv, 2);
    DLT634_5104_SLAVE_SetTimer(pdrv, 3);
    
    msg.Cmd = DLT634_5104SLAVE_LINK_CALLDATA;
    msg.ConNum = tmp - DLT104SlaveLink[pdrv].PeerNoAckNum;
    msg.pData = &DLT104SlaveLink[pdrv].RxdBuf[6];
    msg.Len = pAPCI->Length - 4;
    
    DLT634_5104_SLAVE_AppProcess(pdrv, &msg);
}

/**
  * @brief : Search Valid Frame.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_SearchValidFrame(uint8_t pdrv)
{
    uint8_t stop = FALSE;
    struct DLT634_5104SLAVE_PAPCI *pAPCI;
    
    while ((DLT104SlaveLink[pdrv].RxdHead < DLT104SlaveLink[pdrv].RxdTail) && (!stop))
    {
        if (DLT104SlaveLink[pdrv].RxdStatus == RXDSTART)
        {
            if (DLT104SlaveLink[pdrv].RxdBuf[DLT104SlaveLink[pdrv].RxdHead] != DLT634_5104SLAVE_STARTCODE68)
            {
                DLT104SlaveLink[pdrv].RxdHead++;
				if (DLT104SlaveLink[pdrv].RxdHead >= DLT104SlaveLink[pdrv].RxdTail)
				{
					DLT104SlaveLink[pdrv].RxdLength = DLT104SlaveLink[pdrv].RxdTail - DLT104SlaveLink[pdrv].RxdHead;
					memcpy (DLT104SlaveLink[pdrv].RxdBuf, &DLT104SlaveLink[pdrv].RxdBuf[DLT104SlaveLink[pdrv].RxdHead], DLT104SlaveLink[pdrv].RxdLength);
					DLT104SlaveLink[pdrv].RxdHead = 0;
					DLT104SlaveLink[pdrv].RxdTail = DLT104SlaveLink[pdrv].RxdLength;
				}
                continue;
            }
            
            // 找到后，清除无用的缓冲区报文
            if (DLT104SlaveLink[pdrv].RxdTail > DLT104SlaveLink[pdrv].RxdHead)
            {
                DLT104SlaveLink[pdrv].RxdLength = DLT104SlaveLink[pdrv].RxdTail - DLT104SlaveLink[pdrv].RxdHead;
                memcpy (DLT104SlaveLink[pdrv].RxdBuf, &DLT104SlaveLink[pdrv].RxdBuf[DLT104SlaveLink[pdrv].RxdHead], DLT104SlaveLink[pdrv].RxdLength);
                DLT104SlaveLink[pdrv].RxdHead = 0;
                DLT104SlaveLink[pdrv].RxdTail = DLT104SlaveLink[pdrv].RxdLength;
            }
            
            DLT104SlaveLink[pdrv].RxdStatus = RXDHEAD;
        }
            
        if (DLT104SlaveLink[pdrv].RxdStatus == RXDHEAD)
        {
            pAPCI = (struct DLT634_5104SLAVE_PAPCI*)DLT104SlaveLink[pdrv].RxdBuf;
            if (DLT104SlaveLink[pdrv].RxdLength >= sizeof (struct DLT634_5104SLAVE_PAPCI))
            {
                switch (pAPCI->NS & DLT634_5104SLAVE_U_FRAME)
                {
                    case DLT634_5104SLAVE_S_FRAME:
                        if (pAPCI->Length == 4)
                        {
                            DLT634_5104_SLAVE_KillTimer(pdrv, 1);
                            DLT634_5104_SLAVE_SetTimer(pdrv, 3);
                            DLT634_5104_SLAVE_DecodeSFrame(pdrv);
                        }
                        break;
                
                    case DLT634_5104SLAVE_U_FRAME:
                        if (pAPCI->Length == 4)
                        {
                            DLT634_5104_SLAVE_KillTimer(pdrv, 1);
                            DLT634_5104_SLAVE_SetTimer(pdrv, 3);
                            DLT634_5104_SLAVE_DecodeUFrame(pdrv);
                        }
                        break;
                        
                    default:
                        if ((DLT104SlaveLink[pdrv].RxdLength >= (pAPCI->Length + 2)))
                        {
                            DLT634_5104_SLAVE_DecodeIFrame(pdrv);
                        }
                        else
                        {
                            stop = TRUE;
                        }
                        break;
                }
            }
            else
            {
                stop = TRUE;
            }
            
            if (!stop)
            {
                DLT104SlaveLink[pdrv].RxdStatus = RXDCONTINUE;
            }
        }
            
        if (DLT104SlaveLink[pdrv].RxdStatus == RXDCONTINUE)
        {
            DLT104SlaveLink[pdrv].RxdLength = DLT104SlaveLink[pdrv].RxdTail - (pAPCI->Length + 2);
            memcpy (DLT104SlaveLink[pdrv].RxdBuf, &DLT104SlaveLink[pdrv].RxdBuf[pAPCI->Length + 2], DLT104SlaveLink[pdrv].RxdLength);
            DLT104SlaveLink[pdrv].RxdHead = 0;
            DLT104SlaveLink[pdrv].RxdTail = DLT104SlaveLink[pdrv].RxdLength;
            DLT104SlaveLink[pdrv].RxdStatus = RXDSTART;
        
            if (DLT104SlaveLink[pdrv].RxdTail > DLT104SlaveLink[pdrv].RxdHead)
            {
                stop = TRUE;
            }
        }	
    }
}

/**
  * @brief : All Data Conf Reply.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_AllDataConfReply(uint8_t pdrv)
{
    DLT104SlaveApp[pdrv].TX_Temp.status.Length = sizeof(DLT104SlaveApp[pdrv].TX_Temp.status) + sizeof(DLT104SlaveApp[pdrv].TX_Temp.Head) + sizeof(DLT104SlaveApp[pdrv].TX_Temp.Data.C_100);
    DLT104SlaveApp[pdrv].TX_Temp.status.symbol.priority = DLT634_5104SLAVE_C_IC_NA_PF;
    DLT104SlaveApp[pdrv].TX_Temp.Head.TypeID = DLT634_5104SLAVE_C_IC_NA_1; 
    DLT104SlaveApp[pdrv].TX_Temp.Head.VSQ = 1;
    DLT104SlaveApp[pdrv].TX_Temp.Head.COT_L = DLT634_5104SLAVE_COT_ACTCON;
    DLT104SlaveApp[pdrv].TX_Temp.Head.PubAddr_H = (uint8_t)((DLT634_5104Slave_Pad[pdrv].PubAddress>>8)&0xff);
    DLT104SlaveApp[pdrv].TX_Temp.Head.PubAddr_L = (uint8_t)(DLT634_5104Slave_Pad[pdrv].PubAddress&0xff);
    DLT104SlaveApp[pdrv].TX_Temp.Data.C_100.InfoAddr_L = 0;  
    DLT104SlaveApp[pdrv].TX_Temp.Data.C_100.QOI = 20;  
}


/**
  * @brief : Init End Reply.
  * @param : [pdrv]
  * @return: res
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_InitEndReply(uint8_t pdrv)
{
    DLT104SlaveApp[pdrv].TX_Temp.status.Length = sizeof(DLT104SlaveApp[pdrv].TX_Temp.status) + sizeof(DLT104SlaveApp[pdrv].TX_Temp.Head) + sizeof(DLT104SlaveApp[pdrv].TX_Temp.Data.C_70);
    DLT104SlaveApp[pdrv].TX_Temp.status.symbol.priority = DLT634_5104SLAVE_M_EI_NA_P;
    DLT104SlaveApp[pdrv].TX_Temp.Head.TypeID = DLT634_5104SLAVE_M_EI_NA_1;
    DLT104SlaveApp[pdrv].TX_Temp.Head.VSQ = 1;
    DLT104SlaveApp[pdrv].TX_Temp.Head.COT_L = DLT634_5104SLAVE_COT_INIT;
    DLT104SlaveApp[pdrv].TX_Temp.Head.PubAddr_H = (uint8_t)((DLT634_5104Slave_Pad[pdrv].PubAddress>>8)&0xff);
    DLT104SlaveApp[pdrv].TX_Temp.Head.PubAddr_L = (uint8_t)(DLT634_5104Slave_Pad[pdrv].PubAddress&0xff);
    DLT104SlaveApp[pdrv].TX_Temp.Data.C_70.COI = 2;
}

/**
  * @brief : All Data End Reply.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_AllDataEndReply(uint8_t pdrv)
{
    DLT104SlaveApp[pdrv].TX_Temp.status.Length = sizeof(DLT104SlaveApp[pdrv].TX_Temp.status) + sizeof(DLT104SlaveApp[pdrv].TX_Temp.Head) + sizeof(DLT104SlaveApp[pdrv].TX_Temp.Data.C_100);
    DLT104SlaveApp[pdrv].TX_Temp.status.symbol.priority = DLT634_5104SLAVE_C_IC_NA_PF;
    DLT104SlaveApp[pdrv].TX_Temp.Head.TypeID = DLT634_5104SLAVE_C_IC_NA_1;
    DLT104SlaveApp[pdrv].TX_Temp.Head.VSQ = 1;
    DLT104SlaveApp[pdrv].TX_Temp.Head.COT_L = DLT634_5104SLAVE_COT_ACCTTERM;
    DLT104SlaveApp[pdrv].TX_Temp.Head.PubAddr_H = (uint8_t)((DLT634_5104Slave_Pad[pdrv].PubAddress>>8)&0xff);
    DLT104SlaveApp[pdrv].TX_Temp.Head.PubAddr_L = (uint8_t)(DLT634_5104Slave_Pad[pdrv].PubAddress&0xff);
    DLT104SlaveApp[pdrv].TX_Temp.Data.C_100.QOI = 20;    
}

/**
  * @brief : Reset Processe Reply.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_ResetProcesseReply(uint8_t pdrv)
{
    DLT104SlaveApp[pdrv].TX_Temp.status.Length = sizeof(DLT104SlaveApp[pdrv].TX_Temp.status) + sizeof(DLT104SlaveApp[pdrv].TX_Temp.Head) + sizeof(DLT104SlaveApp[pdrv].TX_Temp.Data.C_105);
    DLT104SlaveApp[pdrv].TX_Temp.status.symbol.priority = DLT634_5104SLAVE_C_RP_NA_P;
    DLT104SlaveApp[pdrv].TX_Temp.Head.TypeID = DLT634_5104SLAVE_C_RP_NA_1;
    DLT104SlaveApp[pdrv].TX_Temp.Head.VSQ = 1;
    DLT104SlaveApp[pdrv].TX_Temp.Head.COT_L = DLT634_5104SLAVE_COT_ACTCON;
    DLT104SlaveApp[pdrv].TX_Temp.Head.PubAddr_H = (uint8_t)((DLT634_5104Slave_Pad[pdrv].PubAddress>>8)&0xff);
    DLT104SlaveApp[pdrv].TX_Temp.Head.PubAddr_L = (uint8_t)(DLT634_5104Slave_Pad[pdrv].PubAddress&0xff);
    DLT104SlaveApp[pdrv].TX_Temp.Data.C_105.QRP = 1;    
}

/**
  * @brief : Link Send Process.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_LinkSendProcess(uint8_t pdrv, uint8_t* pData, uint8_t Len)
{
    struct DLT634_5104SLAVE_PAPCI *pFrame;
    
    pFrame = (struct DLT634_5104SLAVE_PAPCI*)&DLT104SlaveLink[pdrv].TxdBuf[DLT104SlaveLink[pdrv].TxdHead];
    
    if (Len == 0)
    {
        return;
    }
	
    DLT104SlaveLink[pdrv].NRACK = DLT104SlaveLink[pdrv].NR;
		
    pFrame->StartCode = DLT634_5104SLAVE_STARTCODE68;
    pFrame->Length = Len + 0x04;
    pFrame->NS = DLT104SlaveLink[pdrv].NS << 1;
    pFrame->NR = DLT104SlaveLink[pdrv].NR << 1;
    memcpy((uint8_t*)(pFrame+1), pData, Len);
    DLT104SlaveLink[pdrv].TxdTail += Len + 0x06;		

    DLT104SlaveLink[pdrv].DataType[DLT104SlaveLink[pdrv].PeerNoAckNum] = TRUE;
    DLT104SlaveLink[pdrv].PeerNoAckNum++;
    DLT104SlaveLink[pdrv].NS++;
		
    if (DLT104SlaveLink[pdrv].PeerNoAckNum >= DLT634_5104SLAVE_K) // 未被确认的I帧数量达到K个时  停止发送
    {
        DLT104SlaveLink[pdrv].StopSendFlag = TRUE;
    }
	
    // 发送
    DLT634_5104_SLAVE_KillTimer(pdrv, 2);
    DLT634_5104_SLAVE_SetTimer(pdrv, 3);
    DLT634_5104_SLAVE_WriteDataToMISI(pdrv);
    DLT634_5104_SLAVE_SetTimer(pdrv, 1); // 		
		
}

/**
  * @brief : Save BackUp Data.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void SaveBackUpData(uint8_t pdrv)
{
	if (!DLT104SlaveApp[pdrv].SDataEn && DLT104SlaveApp[pdrv].SDataIn <= DLT634_5104SLAVE_K)
	{
		DLT104SlaveApp[pdrv].TxLen = DLT104SlaveApp[pdrv].TX_Temp.status.Length - sizeof(DLT104SlaveApp[pdrv].TX_Temp.status);		
		DLT104SlaveApp[pdrv].SData[DLT104SlaveApp[pdrv].SDataIn].TxLen = DLT104SlaveApp[pdrv].TxLen; 
		memcpy(&DLT104SlaveApp[pdrv].SData[DLT104SlaveApp[pdrv].SDataIn].TxData, (uint8_t*)&DLT104SlaveApp[pdrv].TX_Temp.Head, DLT104SlaveApp[pdrv].TxLen);
		DLT104SlaveApp[pdrv].SDataIn++;
		DLT104SlaveApp[pdrv].CurSData++;				
	}	
}
/**
  * @brief : Read All Data Process.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
/**
  * @brief : Read All Data Process.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void ReadAllDataProcess(uint8_t pdrv)
{
	switch(DLT104SlaveApp[pdrv].AllDataFlag)
	{
		case 0:
            DLT634_5104_SLAVE_AllDataConfReply(pdrv);
            DLT104SlaveApp[pdrv].AllData_yx_sendaddr = DLT634_5104Slave_Pad[pdrv].YX_FirstAddr;
            DLT104SlaveApp[pdrv].AllData_yc_sendaddr = DLT634_5104Slave_Pad[pdrv].YC_FirstAddr;
            DLT104SlaveApp[pdrv].AllDataFlag = 1;
			break;
		case 1:
            if(DLT634_5104Slave_Pad[pdrv].YX_AllNum != 0)
            {
                DLT104SlaveApp[pdrv].AllData_yx_sendaddr = DLT634_5104_SLAVE_ReadYxData(pdrv ,DLT104SlaveApp[pdrv].AllData_yx_sendaddr ,DLT634_5104Slave_Pad[pdrv].YX_FrameNum ,(uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp);
                if(DLT104SlaveApp[pdrv].AllData_yx_sendaddr >= DLT634_5104Slave_Pad[pdrv].YX_FirstAddr + DLT634_5104Slave_Pad[pdrv].YX_AllNum)
                {
                    DLT104SlaveApp[pdrv].AllDataFlag = 2;
                }
                break;
            }
		case 2:
            if(DLT634_5104Slave_Pad[pdrv].YC_AllNum != 0)
            {
                DLT104SlaveApp[pdrv].AllData_yc_sendaddr = DLT634_5104_SLAVE_ReadYcData(pdrv ,DLT104SlaveApp[pdrv].AllData_yc_sendaddr ,DLT634_5104Slave_Pad[pdrv].YC_FrameNum ,(uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp);
                if(DLT104SlaveApp[pdrv].AllData_yc_sendaddr >= DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + DLT634_5104Slave_Pad[pdrv].YC_AllNum)
                {
                    DLT104SlaveApp[pdrv].AllDataFlag = 3;
                }
                break;
            }
		case 3:
            DLT634_5104_SLAVE_AllDataEndReply(pdrv);
            DLT104SlaveApp[pdrv].Data1Flag &= (~DLT634_5104SLAVE_CALLALLDATA);
			break;
		default:
			break;
		
	}  
}


/**
  * @brief : Read Data1 Process.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void ReadData1Process(uint8_t pdrv)
{
    if ((DLT104SlaveApp[pdrv].Data1Flag & DLT634_5104SLAVE_HAVEINITEND)&&
        ((DLT104SlaveApp[pdrv].StoreDATA1IN->status.symbol.priority > DLT634_5104SLAVE_M_EI_NA_P)||
        (DLT104SlaveApp[pdrv].StoreDATA1IN->status.symbol.priority == 0))) 
    {
        DLT634_5104_SLAVE_InitEndReply(pdrv);
        DLT104SlaveApp[pdrv].Data1Flag &= (~DLT634_5104SLAVE_HAVEINITEND);
        return;
    }

	if (DLT104SlaveApp[pdrv].SDataEn)
	{
		if (DLT104SlaveApp[pdrv].CurSData && DLT104SlaveApp[pdrv].SData[DLT104SlaveApp[pdrv].SDataOut].TxLen)
		{
			if (DLT104SlaveApp[pdrv].SDataOut != DLT104SlaveApp[pdrv].CurSData)
			{
				DLT634_5104_SLAVE_LinkSendProcess(pdrv, (uint8_t*)&DLT104SlaveApp[pdrv].SData[DLT104SlaveApp[pdrv].SDataOut].TxData, DLT104SlaveApp[pdrv].SData[DLT104SlaveApp[pdrv].SDataOut].TxLen);
				DLT104SlaveApp[pdrv].SDataOut++;    
			}
			else
			{
				DLT104SlaveApp[pdrv].SDataEn = 0;		
			}
       		return;			
		}
		else
		{
		    DLT104SlaveApp[pdrv].SDataEn = 0;
			return;
		}
	}
    
    if ((DLT634_5104_SLAVE_H_COS(pdrv))&&
        ((DLT104SlaveApp[pdrv].StoreDATA1IN->status.symbol.priority > DLT634_5104SLAVE_M_SP_TB_P)||
        (DLT104SlaveApp[pdrv].StoreDATA1IN->status.symbol.priority == 0))) 
    {
        DLT634_5104_SLAVE_R_COS(pdrv,(uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp);		
		SaveBackUpData(pdrv);
        DLT104SlaveApp[pdrv].Data1Flag &= (~DLT634_5104SLAVE_HAVECOS);
        return;
    }
    
    if ((DLT634_5104_SLAVE_H_SOE(pdrv))&&
        ((DLT104SlaveApp[pdrv].StoreDATA1IN->status.symbol.priority > DLT634_5104SLAVE_M_SP_TB_P)||
        (DLT104SlaveApp[pdrv].StoreDATA1IN->status.symbol.priority == 0))) 
    {
        DLT634_5104_SLAVE_R_SOE(pdrv,(uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp);		
        SaveBackUpData(pdrv);					
        DLT104SlaveApp[pdrv].Data1Flag &= (~DLT634_5104SLAVE_HAVESOE);
        return;
    }
		
    if (DLT104SlaveApp[pdrv].Data1Flag & DLT634_5104SLAVE_CALLALLDATA)
    {
        ReadAllDataProcess(pdrv);	
        return;
    }
    if (DLT104SlaveApp[pdrv].Data1Flag & DLT634_5104SLAVE_CallReset) // 复位进程
    {
        DLT104SlaveApp[pdrv].Data1Flag &= ~DLT634_5104SLAVE_CallReset;
        DLT634_5104_SLAVE_ResetProcesseReply(pdrv);
        DLT634_5104_AppInitReset(pdrv);				
        DLT104SlaveApp[pdrv].Connect = FALSE;
        return;
    }
    
    if (DLT104SlaveApp[pdrv].StoreDATA1OUT != DLT104SlaveApp[pdrv].StoreDATA1IN)
    {
        memcpy((uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp, (uint8_t *)DLT104SlaveApp[pdrv].StoreDATA1OUT, DLT104SlaveApp[pdrv].StoreDATA1OUT->status.Length);
        memset((uint8_t *)DLT104SlaveApp[pdrv].StoreDATA1OUT,0,sizeof(DLT634_5104SLAVE_PASDU));
        if (++DLT104SlaveApp[pdrv].StoreDATA1OUT >= DLT104SlaveApp[pdrv].StoreDATA1Buf + DLT634_5104SLAVE_STOREDATA1NUM)
        {
            DLT104SlaveApp[pdrv].StoreDATA1OUT = DLT104SlaveApp[pdrv].StoreDATA1Buf;
        }
        return;
    }		
}

/**
  * @brief : Read Data2 Process.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void ReadData2Process(uint8_t pdrv)
{
    if ((DLT634_5104_SLAVE_H_NVA(pdrv))&&
        ((DLT104SlaveApp[pdrv].StoreDATA2IN->status.symbol.priority > DLT634_5104SLAVE_M_ME_NC_P)||
        (DLT104SlaveApp[pdrv].StoreDATA2IN->status.symbol.priority == 0))) 
    {
        DLT634_5104_SLAVE_R_NVA(pdrv,(uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp);
        return;
    }
    
    if ((DLT634_5104_SLAVE_H_FEvent(pdrv))&&
        ((DLT104SlaveApp[pdrv].StoreDATA2IN->status.symbol.priority > DLT634_5104SLAVE_M_FT_NA_P)||
        (DLT104SlaveApp[pdrv].StoreDATA2IN->status.symbol.priority == 0))) 
    {
        DLT634_5104_SLAVE_R_FEvent(pdrv,(uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp);
        return;
    }

    if (DLT104SlaveApp[pdrv].StoreDATA2OUT != DLT104SlaveApp[pdrv].StoreDATA2IN)
    {
        memcpy((uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp, (uint8_t *)DLT104SlaveApp[pdrv].StoreDATA2OUT, DLT104SlaveApp[pdrv].StoreDATA2OUT->status.Length);
        memset((uint8_t *)DLT104SlaveApp[pdrv].StoreDATA2OUT,0,sizeof(DLT634_5104SLAVE_PASDU));
        if (++DLT104SlaveApp[pdrv].StoreDATA2OUT >= DLT104SlaveApp[pdrv].StoreDATA2Buf + DLT634_5104SLAVE_STOREDATA2NUM)
        {
            DLT104SlaveApp[pdrv].StoreDATA2OUT = DLT104SlaveApp[pdrv].StoreDATA2Buf;
        }
        return;
    }
    
    DLT634_5104_SLAVE_IdleProcess(pdrv,(uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp);	
}

/**
  * @brief : Net Tes.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_NetTest(uint8_t pdrv)
{   
    if (!DLT634_5104_SLAVE_CheckLink(pdrv))//是否链接
    {            
        DLT634_5104_AppInitReset(pdrv);				
        DLT104SlaveApp[pdrv].Connect = 0;
    }
    else if (!DLT104SlaveLink[pdrv].Connect)
    {				
        DLT104SlaveLink[pdrv].Connect = 1;
    }
}

/**
  * @brief : Send Process.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_SendProcess(uint8_t pdrv)
{	
    if (DLT104SlaveLink[pdrv].StopSendFlag)
    {
        return;
    }
		
    if (DLT104SlaveApp[pdrv].Data1Flag & DLT634_5104SLAVE_SENDDATA)
    {
        DLT104SlaveApp[pdrv].Data1Flag &= (~DLT634_5104SLAVE_SENDDATA);
        return;
    }
	
    ReadData1Process(pdrv);    
	
    if (DLT104SlaveApp[pdrv].TX_Temp.status.Length)
    {
        DLT104SlaveApp[pdrv].TxLen = DLT104SlaveApp[pdrv].TX_Temp.status.Length - sizeof(DLT104SlaveApp[pdrv].TX_Temp.status);		
        memcpy(&DLT104SlaveApp[pdrv].TxMsg, &DLT104SlaveApp[pdrv].TX_Temp.Head, DLT104SlaveApp[pdrv].TxLen); 			
        DLT634_5104_SLAVE_LinkSendProcess(pdrv, (uint8_t*)&DLT104SlaveApp[pdrv].TxMsg, DLT104SlaveApp[pdrv].TxLen);			
        memset((uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp, 0, sizeof(DLT634_5104SLAVE_PASDU));			
    }	
    else
    {
        ReadData2Process(pdrv);
        if (DLT104SlaveApp[pdrv].TX_Temp.status.Length)
        {
            DLT104SlaveApp[pdrv].TxLen = DLT104SlaveApp[pdrv].TX_Temp.status.Length - sizeof(DLT104SlaveApp[pdrv].TX_Temp.status);		
            memcpy(&DLT104SlaveApp[pdrv].TxMsg, &DLT104SlaveApp[pdrv].TX_Temp.Head, DLT104SlaveApp[pdrv].TxLen); 			
            DLT634_5104_SLAVE_LinkSendProcess(pdrv, (uint8_t*)&DLT104SlaveApp[pdrv].TxMsg, DLT104SlaveApp[pdrv].TxLen);						
            memset((uint8_t *)&DLT104SlaveApp[pdrv].TX_Temp, 0, sizeof(DLT634_5104SLAVE_PASDU));			
        }				
    }	
}

/**
  * @brief : Link On Timer.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_LinkOnTimer(uint8_t pdrv)
{
    uint8_t	i = 0;
    uint16_t num = 0;     
    
    if (DLT104SlaveLink[pdrv].Tick.Flag & DLT634_5104SLAVE_T0FLAG)
    {
        DLT104SlaveLink[pdrv].Tick.TValue0++;

        if (DLT104SlaveLink[pdrv].Tick.TValue0 > DLT634_5104Slave_Pad[pdrv].TickValue[0])
        {
            DLT634_5104_SLAVE_KillTimer(pdrv, 0);
            DLT634_5104_SLAVE_StopDTOper(pdrv);
        }
    }

    if (DLT104SlaveLink[pdrv].Tick.Flag & DLT634_5104SLAVE_T1FLAG)
    {
        for (i=0; i<DLT104SlaveLink[pdrv].PeerNoAckNum; i++)
        {
            DLT104SlaveLink[pdrv].Tick.TValue1[i]++;

            if (DLT104SlaveLink[pdrv].Tick.TValue1[i] > DLT634_5104Slave_Pad[pdrv].TickValue[1])
            {
                DLT634_5104_SLAVE_KillTimer(pdrv, 1);
                DLT634_5104_SLAVE_StopDTOper(pdrv);
            }
        }
    }

    if (DLT104SlaveLink[pdrv].NRACK <= DLT104SlaveLink[pdrv].NR)
    {
        num = DLT104SlaveLink[pdrv].NR - DLT104SlaveLink[pdrv].NRACK;
    }
    else
    {
        num = 32767 + DLT104SlaveLink[pdrv].NR - DLT104SlaveLink[pdrv].NRACK;	  	
    }
    
    if (num >= DLT634_5104SLAVE_W)
    {
        DLT104SlaveLink[pdrv].NRACK = DLT104SlaveLink[pdrv].NR;
        DLT634_5104_SLAVE_KillTimer(pdrv, 2);
        DLT634_5104_SLAVE_S104SendCtrlFrame(pdrv, DLT634_5104SLAVE_S_FRAME);
    }
    
    if (DLT104SlaveLink[pdrv].Tick.Flag & DLT634_5104SLAVE_T2FLAG)
    {
        DLT104SlaveLink[pdrv].Tick.TValue2++;

        if ( DLT104SlaveLink[pdrv].Tick.TValue2 > DLT634_5104Slave_Pad[pdrv].TickValue[2] )
        {
            DLT634_5104_SLAVE_KillTimer(pdrv, 2);
            DLT634_5104_SLAVE_S104SendCtrlFrame(pdrv, DLT634_5104SLAVE_S_FRAME);
        }
    }
    
    if (DLT104SlaveLink[pdrv].Tick.Flag & DLT634_5104SLAVE_T3FLAG)
    {
        DLT104SlaveLink[pdrv].Tick.TValue3++;

        if (DLT104SlaveLink[pdrv].Tick.TValue3 > DLT634_5104Slave_Pad[pdrv].TickValue[3])
        {
            DLT634_5104_SLAVE_KillTimer(pdrv, 3);
            
            if (DLT104SlaveLink[pdrv].PeerNoAckNum == 0)
            {
                DLT634_5104_SLAVE_S104SendCtrlFrame(pdrv, DLT634_5104SLAVE_U_TESTFRACT);
            }
            else if (!(DLT104SlaveLink[pdrv].Tick.Flag & DLT634_5104SLAVE_T1FLAG))
            {
                DLT634_5104_SLAVE_SetTimer(pdrv, 1);
            }
        }
    }	
}

/**
  * @brief : Read MISI Data.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void DLT634_5104_SLAVE_ReadMISIData(uint8_t pdrv, uint8_t Flag)
{
    uint16_t count = 0;
    
	  /*读取接收缓冲区*/
	if (DLT104SlaveLink[pdrv].RxdTail < sizeof(DLT104SlaveLink[pdrv].RxdBuf) - DLT634_5104SLAVE_FRAMEBUFSIZE)
	{
        count = DLT634_5104_SLAVE_ReadData(pdrv, (uint8_t *)&DLT104SlaveLink[pdrv].RxdBuf[DLT104SlaveLink[pdrv].RxdTail], DLT634_5104SLAVE_FRAMEBUFSIZE);   
	}
    
    if(!DLT634_ChannelToMonitor_SLAVE_SearchMonitorFrame(pdrv, DLT634_5104, DLT104SlaveLink[pdrv].RxdBuf, 256))
    {
		if (Flag)
		{
			DLT104SlaveLink[pdrv].RxdTail += count;
			DLT104SlaveLink[pdrv].RxdLength += count;
		}		
        DLT634_5104_SLAVE_SearchValidFrame(pdrv); 
    }
}

/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : Reply.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_SLAVE_Reply(uint8_t pdrv, uint8_t *pbuf) //回复
{ 
    uint8_t res;
    memcpy(&DLT104SlaveApp[pdrv].ST_Temp, pbuf, DLT634_5104SLAVE_LPDUSIZE);
    switch (DLT104SlaveApp[pdrv].ST_Temp.Head.TypeID)
    {
        //控制方向过程信息
        case DLT634_5104SLAVE_C_SC_NA_1: // 单点遥控命令
        case DLT634_5104SLAVE_C_SC_NB_1: // 双点遥控命令
            DLT104SlaveApp[pdrv].ST_Temp.status.symbol.priority = DLT634_5104SLAVE_C_SC_NA_P;
            res = DLT634_5104_SLAVE_StoreIN(pdrv,&DLT104SlaveApp[pdrv].ST_Temp);
            break;

        case DLT634_5104SLAVE_C_CS_NA_1: // 对时命令
            DLT104SlaveApp[pdrv].ST_Temp.status.symbol.priority = DLT634_5104SLAVE_C_CS_NA_P;
            res = DLT634_5104_SLAVE_StoreIN(pdrv,&DLT104SlaveApp[pdrv].ST_Temp);
            break;

        case DLT634_5104SLAVE_C_RP_NA_1: // 复位进程命令
            break;

        case DLT634_5104SLAVE_C_RR_NA_1: // 读定值区号
        case DLT634_5104SLAVE_C_RS_NA_1: // 读参数和定值命令
            DLT104SlaveApp[pdrv].ST_Temp.status.symbol.priority = DLT634_5104SLAVE_C_IC_NA_P;
            res = DLT634_5104_SLAVE_StoreIN(pdrv,&DLT104SlaveApp[pdrv].ST_Temp);
            break;

        case DLT634_5104SLAVE_C_SR_NA_1: // 切换定值区
        case DLT634_5104SLAVE_C_WS_NA_1: // 写参数和定值命令
            DLT104SlaveApp[pdrv].ST_Temp.status.symbol.priority = DLT634_5104SLAVE_C_SC_NA_P;
            res = DLT634_5104_SLAVE_StoreIN(pdrv,&DLT104SlaveApp[pdrv].ST_Temp);
            break;

        case DLT634_5104SLAVE_F_FR_NA_1: // 文件传输
            DLT104SlaveApp[pdrv].ST_Temp.status.symbol.priority = DLT634_5104SLAVE_F_FW_NA_P;
            res = DLT634_5104_SLAVE_StoreIN(pdrv,&DLT104SlaveApp[pdrv].ST_Temp);
            break;

        case DLT634_5104SLAVE_F_SR_NA_1: // 软件升级
            DLT104SlaveApp[pdrv].ST_Temp.status.symbol.priority = DLT634_5104SLAVE_F_FW_NA_P;
            res = DLT634_5104_SLAVE_StoreIN(pdrv,&DLT104SlaveApp[pdrv].ST_Temp);
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
uint8_t DLT634_5104_SLAVE_AppInit(uint8_t pdrv)
{
	if (DLT104SlaveLink == RT_NULL)
	{
	    DLT104SlaveLink = rt_malloc(sizeof(struct DLT634_5104SLAVE_LINKINFO) * DLT634_5104SLAVE_VOLUMES);
        memset (DLT104SlaveLink, 0, sizeof (struct DLT634_5104SLAVE_LINKINFO) * DLT634_5104SLAVE_VOLUMES);
	}
	if (DLT104SlaveApp == RT_NULL)
	{
	    DLT104SlaveApp = rt_malloc(sizeof(struct DLT634_5104SLAVE_APPINFO) * DLT634_5104SLAVE_VOLUMES);
        memset (DLT104SlaveApp, 0, sizeof (struct DLT634_5104SLAVE_APPINFO) * DLT634_5104SLAVE_VOLUMES);
	}	

    DLT104SlaveApp[pdrv].YXTypeID = DLT634_5104SLAVE_M_SP_NA_1;
    DLT104SlaveApp[pdrv].YCTypeID = DLT634_5104SLAVE_M_ME_NC_1;
    DLT634_5104_AppInitReset(pdrv);
	  
    return(TRUE);
}

/**
  * @brief : Clock.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_SLAVE_Clock(uint8_t pdrv)
{
    DLT104SlaveLink[pdrv].ClockCounter++;
    if (DLT104SlaveLink[pdrv].ClockCounter >= DLT634_5104Slave_Pad[pdrv].ClockTimers)
    {
        DLT104SlaveLink[pdrv].ClockCounter = 0;
        // 查询MISI接收缓冲区
			
        if(!DLT104SlaveApp[pdrv].Connect) // 状态连接
        {
            DLT634_5104_SLAVE_NetTest(pdrv);
            DLT634_5104_SLAVE_ReadMISIData(pdrv, TRUE);
        }
        else
        {
            DLT634_5104_SLAVE_WriteDataToMISI(pdrv);
            
            if(DLT104SlaveLink[pdrv].TxdHead != DLT104SlaveLink[pdrv].TxdTail)
            {
                return(DLT104SlaveApp[pdrv].Connect);
            }
        
            DLT634_5104_SLAVE_ReadMISIData(pdrv, TRUE); // 链路层数据查询

            if(!(channel_monitor.MonitorFlag[pdrv] & _ChannelToMonitor2_START))
            {
                DLT634_5104_SLAVE_SendProcess(pdrv);	// 发送数据				
                    
                DLT634_5104_SLAVE_LinkOnTimer(pdrv); // 链路层定时器
            }
                                            
            DLT634_5104_SLAVE_NetTest(pdrv); // 网络判断 
        }					
	}  
    return(DLT104SlaveApp[pdrv].Connect);   
}




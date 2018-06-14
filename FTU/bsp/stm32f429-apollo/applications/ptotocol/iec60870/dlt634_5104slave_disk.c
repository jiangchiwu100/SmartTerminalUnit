/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5104slave_disk.c
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */

#include "tcp_server.h"
#include "common_data.h"
#include "drv_do.h"
#include "string.h"
#include "dlt634_5104slave_app.h"
#include "dlt634_5104slave_disk.h"
#include "file_operate.h"
#include "channel_monitor.h"

/* PRIVATE VARIABLES ----------------------------------------------------------*/
static uint8_t file_array[DLT634_5104SLAVE_VOLUMES][256];
static uint8_t temp_array[DLT634_5104SLAVE_VOLUMES][256];

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : read data from drv.
  * @param : [pdrv]
  * @param : [pbuf]
  * @param : [count]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint16_t DLT634_5104_SLAVE_WriteData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{ 
    uint16_t len = 0; 
	
    switch (pdrv)
    {
        case DLT634_5104SLAVE_DISK0:
          #if RT_USING_DP83848 
            len = dp83848_tcpserver_putc(pbuf, count);
          #endif /* RT_USING_DP83848 */ 
            break;
        case DLT634_5104SLAVE_DISK1:
          #if RT_USING_W5500 
            len = w5500_tcpserver_putc(pbuf, count);
          #endif /* RT_USING_W5500 */ 
            break;
		default:
            break;
	}
//	if((channel_monitor.MonitorFlag[0]||channel_monitor.MonitorFlag[1]) && (pdrv == DLT634_5104SLAVE_DISK0)&&(channel_monitor.ByAddr == InternetAccess1) && (len>0))
//    {
//        MonitoringDataTransmission(pbuf, len, ReceivedData);
//    }
//    if((channel_monitor.MonitorFlag[0]||channel_monitor.MonitorFlag[1]) && (pdrv == DLT634_5104SLAVE_DISK1)&&(channel_monitor.ByAddr == InternetAccess2) && (len>0))
//    {
//        MonitoringDataTransmission(pbuf, len, ReceivedData);
//    }
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
uint16_t DLT634_5104_SLAVE_ReadData(uint8_t pdrv, uint8_t *pbuf, uint16_t count)
{ 
	uint16_t len = 0;  
	
    switch(pdrv)
    {
		case DLT634_5104SLAVE_DISK0:
          #if RT_USING_DP83848 
            len = dp83848_tcpserver_getc(pbuf, count);
          #endif /* RT_USING_DP83848 */ 
            break;
		
        case DLT634_5104SLAVE_DISK1:
          #if RT_USING_W5500 
            len = w5500_tcpserver_getc(pbuf, count);
          #endif /* RT_USING_W5500 */ 
            break;
				
        default:
            break;
    }
//	if((channel_monitor.MonitorFlag[0]||channel_monitor.MonitorFlag[1]) && (pdrv == DLT634_5104SLAVE_DISK0)&&(channel_monitor.ByAddr == InternetAccess1) && (len>0))
//    {
//        MonitoringDataTransmission(pbuf, len, TransmittedData);
//    }
//    if((channel_monitor.MonitorFlag[0]||channel_monitor.MonitorFlag[1]) && (pdrv == DLT634_5104SLAVE_DISK1)&&(channel_monitor.ByAddr == InternetAccess2) && (len>0))
//    {
//        MonitoringDataTransmission(pbuf, len, TransmittedData);
//    }
	return(len);
}

/**
  * @brief : Check Link.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_SLAVE_CheckLink(uint8_t pdrv)
{ 
    uint8_t res = 0;  

    switch(pdrv)
    {
		case DLT634_5104SLAVE_DISK0:
          #if RT_USING_DP83848 
            if(dp83848_tcpserver_linkstate())	
            {res = 1;}
          #endif /* RT_USING_DP83848 */ 
            break;
						
        case DLT634_5104SLAVE_DISK1:
          #if RT_USING_W5500 
            if(w5500_tcpserver_linkstate())	
            {res = 1;}
          #endif /* RT_USING_W5500 */ 
            break;
						
        default:
            break;
    }
    return(res);
}

/**
  * @brief : Stop Link.
  * @param : [pdrv]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_SLAVE_StopLink(uint8_t pdrv)
{ 
    switch(pdrv)
    {
		case DLT634_5104SLAVE_DISK0:
          #if RT_USING_DP83848 
            dp83848_tcpserver_disconnect(); // 断开链接	
          #endif /* RT_USING_DP83848 */ 
            break;
		
        case DLT634_5104SLAVE_DISK1:
          #if RT_USING_W5500 
            w5500_tcpserver_disconnect(); // 断开链接	
          #endif /* RT_USING_W5500 */ 
            break;
				
        default:
            break;
    }
    return(TRUE);
}

/**
  * @brief : all data command.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_CallAllDataProcess(uint8_t pdrv, uint8_t *pbuf)//总召
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/QOI 
    g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port] = g_NVADBIn;
    g_COSDBOut[DLT634_5104Slave_Pad[pdrv].Port] = g_COSDBIn;
    
    DBClearNVA();
}

/**
  * @brief : remote command.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_HandleCtrlProcess(uint8_t pdrv, uint8_t *pbuf)
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/SCO_L/SCO_H
 //LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/DCO_L/DCO_H
    static uint16_t addr,addrsuc;
    static uint8_t value,valuesuc;
    
    addr = pbuf[8]|(pbuf[9]<<8);
    
    if(pbuf[2] == DLT634_5104SLAVE_C_SC_NA_1)
    {
        value = (pbuf[11]&0x03)+1;
    }
    else
    {
        value = pbuf[11]&0x03;
    }
    
    memcpy(temp_array[pdrv],pbuf,pbuf[0]);
    
    if(((addr >= REMOTE_START_ADDR)&&(addr < REMOTE_START_ADDR + REMOTE_TOTAL_NUM))&&(g_NewToOldRemote[addr - REMOTE_START_ADDR] != 0)&&\
        ((g_CommunicatFlag[COM_YK]&(1<<DLT634_5104Slave_Pad[pdrv].Port))||(!(g_CommunicatFlag[COM_YK]&COMMUNICATLOCKUSERSTA))))
    {
        if((g_NewToOldPropertyRemote[addr - REMOTE_START_ADDR]>>NEWPROPERTY_NEG) & NEWPROPERTY_JUDG)
        {
            value = (~value)&0x03;        
        }
        addr = g_NewToOldRemote[addr - REMOTE_START_ADDR] - REMOTE_START_ADDR;
    }
    else
    {                            
        temp_array[pdrv][4] = DLT634_5104SLAVE_COT_DEACTCON;
        DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
        return;
    }    
            
    if(pbuf[11]&0x80)//预制
    {
        if(pbuf[4] == DLT634_5104SLAVE_COT_ACT)
        {
            if((value&0x7f) == OFF)
            {
                if(rt_multi_telecontrl_proof(addr, DO_OPEN))				                                
                {
                    valuesuc = OFF;
                    addrsuc = addr;
                    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ACTCON;
                    g_CommunicatFlag[COM_YK] |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
                else
                {
                    valuesuc = 0;
                    addrsuc = 0;
                    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ERR;
                    g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
            }
            else
            {
		        if(rt_multi_telecontrl_proof(addr, DO_CLOSE)) 				      
                {
                    valuesuc = ON;
                    addrsuc = addr;
                    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ACTCON;
                    g_CommunicatFlag[COM_YK] |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
                else
                {
                    valuesuc = 0;
                    addrsuc = 0;
                    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ERR;
                    g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
            }
        }
        else
        {
            DBWriteCO(addr, DISTANT_REMOTE_CANCEL);
            valuesuc = 0;
            addrsuc = 0;
            temp_array[pdrv][4] = DLT634_5104SLAVE_COT_DEACTCON;
            g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
        }
        DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
    }
    else//执行
    {    
        if(pbuf[4] == DLT634_5104SLAVE_COT_ACT)
        {
            if((value&0x7f) == OFF)
            {
                if(rt_multi_telecontrl_proof(addr, DO_OPEN)&&(valuesuc==OFF)&&(addrsuc==addr)&&\
                    (g_CommunicatFlag[COM_YK]&(1<<DLT634_5104Slave_Pad[pdrv].Port)))      
                {
					if(rt_multi_telecontrl_operate(addr, DO_OPEN))
                    {
                        valuesuc = 0;
                        addrsuc = 0;
                        temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ACTCON;
                        g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);                    
                    }
                    else
                    {
                        valuesuc = 0;
                        addrsuc = 0;
                        temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ERR;
                        g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
                    }
                }
                else
                {
                    valuesuc = 0;
                    addrsuc = 0;
                    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ERR;
                    g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
            }
            else
            {
                if(rt_multi_telecontrl_proof(addr, DO_CLOSE)&&(valuesuc==ON)&&(addrsuc==addr)&&\
                    (g_CommunicatFlag[COM_YK]&(1<<DLT634_5104Slave_Pad[pdrv].Port)))  
                {
					if(rt_multi_telecontrl_operate(addr, DO_CLOSE))
                    {
                        valuesuc = 0;
                        addrsuc = 0;
                        temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ACTCON;
                        g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);                    
                    }
                    else
                    {
                        valuesuc = 0;
                        addrsuc = 0;
                        temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ERR;
                        g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);                   
                    }
                }
                else
                {
                    valuesuc = 0;
                    addrsuc = 0;
                    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ERR;
                    g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
            }
        }
        else
        {
            DBWriteCO(addr, DISTANT_REMOTE_CANCEL);
            valuesuc = 0;
            addrsuc = 0;
            temp_array[pdrv][4] = DLT634_5104SLAVE_COT_DEACTCON;
            g_CommunicatFlag[COM_YK] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
        }
        DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
		temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ACCTTERM;
		DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
    }
}

/**
  * @brief : clock synchronization command.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_SYNProcess(uint8_t pdrv, uint8_t *pbuf)
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/CP56Time2a  
    struct CP56Time2a_t time;
    
    memcpy(temp_array[pdrv],pbuf,sizeof(temp_array[pdrv]));

    switch(pbuf[4])
    {
        case DLT634_5104SLAVE_COT_REQ:
            temp_array[pdrv][4] = DLT634_5104SLAVE_COT_REQ;
            DBReadSystemTime(&time);
            memcpy(&temp_array[pdrv][11],(uint8_t *)&time,sizeof(struct CP56Time2a_t));
            break;
        case DLT634_5104SLAVE_COT_ACT:
            temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ACTCON;
            time = *(struct CP56Time2a_t *)&temp_array[pdrv][11];								
            DBWriteSystemTime(&time);
            break;
        default:
            temp_array[pdrv][4] = 47;
            break;
    }

    DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);  
}

/**
  * @brief : reset command.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_ResetProcess(uint8_t pdrv, uint8_t *pbuf)
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/QRP  
    temp_array[pdrv][4] = 7;
    memcpy(temp_array[pdrv],pbuf,sizeof(temp_array[pdrv]));
    DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);  
    g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKRESET;
}

/**
  * @brief : reset command.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_FixedParaProcess(uint8_t pdrv, uint8_t *pbuf)//定值操作
{//切换定值区号//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/SN_L/SN_H
 //读当前定值区号//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H
 //读多个定值//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/SN1_L/SN1_H/Array(InfoAddr_L/InfoAddr_M/InfoAddr_H)
 //读全部定值//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/SN1_L/SN1_H
 //写定值//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/SN_L/SN_H/PI//Array(InfoAddr_L/InfoAddr_M/InfoAddr_H/Tag/Len/Value)
 //固化定值//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/SN1_L/SN1_H/PI	
    uint8_t SN_temp;
    uint8_t NUM_temp;
    uint16_t i,j,k,m;
    uint16_t temp1,temp2;
    uint8_t txtemp[256];
    uint16_t ADD_temp[30];
    uint16_t addr;
    uint32_t length;
    static uint8_t NumendFlag;
    struct CommonInfo Info;
    
    memcpy(temp_array[pdrv],pbuf,pbuf[0]);
    
    if((g_CommunicatFlag[COM_PARA]&(1<<DLT634_5104Slave_Pad[pdrv].Port))||(!(g_CommunicatFlag[COM_PARA]&COMMUNICATLOCKUSERSTA)))
    {
    }
    else
    {                            
        temp_array[pdrv][4] = DLT634_5104SLAVE_COT_ERR;
		DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
        return;
    } 
    
    switch(pbuf[2])
    {
        case DLT634_5104SLAVE_C_SR_NA_1:
            if(DBSwitchValueArea(pbuf[11]))
            {
                temp_array[pdrv][4]=7;
            }
            else
            {
                temp_array[pdrv][4]=47;
            }
            DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
            break;
        case DLT634_5104SLAVE_C_RR_NA_1:
            SN_temp =  DBReadValueAreaSN();
            temp_array[pdrv][0] = temp_array[pdrv][0] + 6;
            temp_array[pdrv][4] = 7;
            temp_array[pdrv][11] = SN_temp;
            temp_array[pdrv][12] = 0;
            temp_array[pdrv][13] = SN_MIN;
            temp_array[pdrv][14] = 0;
            temp_array[pdrv][15] = SN_MAX;
            temp_array[pdrv][16] = 0;
            DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
            break;
        case DLT634_5104SLAVE_C_RS_NA_1:
            if((pbuf[3]&0x7f)==0)//vsq
            {
                if(pbuf[8] == 0)
                {
                            addr = INTRIPARAME_START_ADDR;
//                            addr = RUNPARAMETER_START_ADDR;
                            NUM_temp = INHERENT_PARAMETER_NUM+g_ParameterCfg_Len;
//                            NUM_temp = RUN_PARAMETER_NUM+CALIBRATE_FACTOR_NUM;
                }
                else
                {
                    addr = FIXED_VALUE_START_ADDR;
                    NUM_temp = g_FixedValueCfg1_Len;
                }
            }
            else
            {
                NUM_temp = pbuf[3]&0x7f;//vsq
                addr = 0;
            }

            memcpy(temp_array[pdrv],pbuf,pbuf[0]);
            if(NUM_temp)
            {
                for(i=0,m=0xff;(i<NUM_temp)&(m!=0);i+=m)
                {
                    if((pbuf[3]&0x7f)==0)//vsq
                    {
                        for(j=0,k=0,m=0;j<200;m++,j+=k)
                        {
                            ADD_temp[m] = addr;
                            if((addr<INTRIPARAME_START_ADDR+INHERENT_PARAMETER_NUM)&&(addr>=INTRIPARAME_START_ADDR))
                            {
                                k=29;
                            }
                            else
                            {
                                k=9;
                            }
                            if(pbuf[8] == 0)
                            {
                                if(++addr > RUNPARAMETER_START_ADDR+g_ParameterCfg_Len)
                                {
                                    break;
                                }
                                else if(addr == INTRIPARAME_START_ADDR+INHERENT_PARAMETER_NUM)
                                {
                                    addr = RUNPARAMETER_START_ADDR;
                                }
                            }
                            else
                            {
                                if(++addr > FIXED_VALUE_START_ADDR + g_FixedValueCfg1_Len)
                                {
                                    break;
                                }
//                                else if(addr == BREAKER_VALUE_START_ADDR+BREAKER_VALUE_NUM)
//                                {
//                                    addr = LOAD_SWITCH_VALUE_START_ADDR;
//                                }
//                                else if(addr == PUBLIC_VALUE_START_ADDR+PUBLIC_VALUE_NUM)
//                                {
//                                    addr = BREAKER_VALUE_START_ADDR;
//                                }
                            }
                        }
                    }
                    else
                    {
                        for(j=0,k=0,m=0;j<200;m++,j+=k)
                        {
                            if((*(uint16_t *)&pbuf[10+(addr*3)]<INTRIPARAME_START_ADDR+INHERENT_PARAMETER_NUM)&&(*(uint16_t *)&pbuf[10+(addr*3)]>=INTRIPARAME_START_ADDR))
                            {
                                k=29;
                            }
                            else
                            {
                                k=9;
                            }
                            ADD_temp[m] = *(uint16_t *)&pbuf[10+(addr*3)];
                            if(++addr>NUM_temp)
                            {break;}
                        }                            
                    }
                    //读函数
                    DBReadValue(ADD_temp,m,&temp_array[pdrv][11],pbuf[8],&length);
                    temp_array[pdrv][0] = 0x0a + 1 + length;
                    temp_array[pdrv][3] = m;
                    temp_array[pdrv][4] = 7;
                    temp_array[pdrv][5] = 0;
                    if((i+m) >= NUM_temp)
                    {temp_array[pdrv][10] = 0x00;}
                    else
                    {temp_array[pdrv][10] = 0x01;}
                    memcpy(txtemp,temp_array[pdrv],temp_array[pdrv][0]);
                    for(temp1=0,temp2=0,k=0;(k<txtemp[3])&&(txtemp[0]-13-temp2>0);k++)
                    {
                        memcpy(&temp_array[pdrv][14+temp1],&txtemp[13+temp2],txtemp[0]-13-temp2);
                        temp_array[pdrv][13+temp1] = 0;
                        temp1 += 1+1+temp_array[pdrv][14+1+temp1]+3;
                        temp2 += 1+1+txtemp[13+1+temp2]+2;
                    }
                    temp_array[pdrv][0] += temp_array[pdrv][3];
                    DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);                      
                }
            }
            else
            {
                temp_array[pdrv][4] = 47;
                DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
            }
            break;
        case DLT634_5104SLAVE_C_WS_NA_1:
            if(pbuf[10]&0x40)//取消预制
            {
                Info.operateType = DB_VALUE_REVOKE;
                DBWriteValue(NULL, &Info);
                NumendFlag  = 0;
                g_CommunicatFlag[COM_PARA] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
                temp_array[pdrv][4] = 9;
                temp_array[pdrv][5] = 0;
                DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
            }
            else if(pbuf[10]&0x80)//预制
            {
                if(!(g_CommunicatFlag[COM_PARA]&COMMUNICATLOCKUSERSTA))
                {
                    Info.operateType = DB_VALUE_REVOKE;
                    DBWriteValue(NULL, &Info);
                    NumendFlag  = 0;
                    g_CommunicatFlag[COM_PARA] |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
                if(!(NumendFlag&0x80))
                {
                    g_CommunicatFlag[COM_PARA] |= COMMUNICATLOCKCLRCON;
                    if(!(pbuf[10]&0x01))
                    {
                        NumendFlag  |= 0x80;//结束
                    }
                    Info.num = pbuf[3]&0x7f;
                    Info.operateType = DB_VALUE_PRESET;
                    Info.sn = pbuf[8];
					temp_array[pdrv][4] = 7;
                    temp_array[pdrv][5] = 0;
                    DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
                    memcpy(temp_array[pdrv],pbuf,pbuf[0]);
                    for(temp1=0,temp2=0,k=0;(k<pbuf[3])&&(pbuf[0]-14-temp2>0);k++)
                    {
                        memcpy(&temp_array[pdrv][13+temp1],&pbuf[14+temp2],pbuf[0]-14-temp2);
                        temp1 += 1+1+temp_array[pdrv][13+1+temp1]+2;
                        temp2 += 1+1+pbuf[14+1+temp2]+3;
                    }
                    temp_array[pdrv][0] -= pbuf[3];
                    DBWriteValue(&temp_array[pdrv][11], &Info);
                }
                else
                {
                    temp_array[pdrv][4] = 47;
                    temp_array[pdrv][5] = 0;
                    DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
                    Info.operateType = DB_VALUE_REVOKE;
                    DBWriteValue(NULL, &Info);
                    NumendFlag  = 0;
                    g_CommunicatFlag[COM_PARA] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
            }
            else//固化
            {    
                if((g_CommunicatFlag[COM_PARA]&(1<<DLT634_5104Slave_Pad[pdrv].Port))&&(NumendFlag&0x80))
                {
                    Info.operateType = DB_VALUE_SOLID;
                    DBWriteValue(NULL, &Info);
                    temp_array[pdrv][4] = 7;
                    temp_array[pdrv][5] = 0;
                    DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
                }
                else
                {
                    Info.operateType = DB_VALUE_REVOKE;
                    DBWriteValue(NULL, &Info);
                    temp_array[pdrv][4] = 47;
                    temp_array[pdrv][5] = 0;
                    DLT634_5104_SLAVE_Reply(pdrv, temp_array[pdrv]);
                }
                NumendFlag  = 0;
                g_CommunicatFlag[COM_PARA] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
            }
            break;
    }   
}



/**
  * @brief : judg cos.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_SLAVE_H_COS(uint8_t pdrv)//判断是否有COS
{
    if(g_COSDBIn != g_COSDBOut[DLT634_5104Slave_Pad[pdrv].Port])
    {
        return(1);
    }
    return(0);     
}

/**
  * @brief : read cos.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_R_COS(uint8_t pdrv, uint8_t *pbuf)//读cos
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/Array(InfoAddr_L/InfoAddr_H/Value/CP56Time2a) 
    uint16_t i = 0 ,j = 0,temp1 = 0;  
    uint8_t sendnum = 0;  
    uint16_t Property = 0xffff;
    struct COS_Str cos_temp;
    
    for(i=0;g_COSDBIn != g_COSDBOut[DLT634_5104Slave_Pad[pdrv].Port];i++)
    {
        for(j = 0,temp1 = 0;j< g_COSDB[g_COSDBOut[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YX_FirstAddr;j++)
        {
            temp1 += (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_NUM) + 1 + 1;            
        }
            
        if((sendnum != 0)&&(((g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_PROPERTY)&0xff) != (Property)))
        {
            break;//与上一个属性不同
        }
        
        Property = (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_PROPERTY)&0xff;
        
        memcpy(&cos_temp,&(g_COSDB[g_COSDBOut[DLT634_5104Slave_Pad[pdrv].Port]]),sizeof(struct COS_Str));
        
        if((Property>>NEWPROPERTY_NEG)&NEWPROPERTY_JUDG)//取反
        {
            cos_temp.value = (~cos_temp.value)&0x03;        
        }
            
        if(((Property>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG) == DLT634_5104SLAVE_M_SP_NA_1)//单点
        {
            cos_temp.value = (cos_temp.value>>1)&0x01;        
        }  

        temp_array[pdrv][sendnum*(sizeof(struct COS_Str)+1) + 8] = LOBYTE(cos_temp.addr);
        temp_array[pdrv][sendnum*(sizeof(struct COS_Str)+1) + 9] = HIBYTE(cos_temp.addr);
        temp_array[pdrv][sendnum*(sizeof(struct COS_Str)+1) + 10] = 0;
        memcpy(&temp_array[pdrv][sendnum*(sizeof(struct COS_Str)+1) + 11],&cos_temp.value,sizeof(struct COS_Str)-2);         

        if (++(g_COSDBOut[DLT634_5104Slave_Pad[pdrv].Port]) >= COS_MAX_NUM)
        {
            g_COSDBOut[DLT634_5104Slave_Pad[pdrv].Port] = 0;
        }        
        
        sendnum++;
        
        if(sendnum >= 10)
        {
            break;//大于每包数量
        }
        
    }
    
    temp_array[pdrv][0] = 8 + sendnum*(sizeof(struct COS_Str)+1);
    temp_array[pdrv][2] = ((Property>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG);
    temp_array[pdrv][3] = sendnum;
    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_SPONT;
    temp_array[pdrv][5] = 0;
    temp_array[pdrv][6] = LOBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);
    temp_array[pdrv][7] = HIBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);

    memcpy(pbuf,temp_array[pdrv],temp_array[pdrv][0]);
}

/**
  * @brief : judg soe.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_SLAVE_H_SOE(uint8_t pdrv)//判断是否有soe
{ 
    if(g_FlagDB.queue_soe_new.in != g_FlagDB.queue_soe_new.out[DLT634_5104Slave_Pad[pdrv].Port])
    {
        return(1);
    }

    return(0); 
}

/**
  * @brief : read soe.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_R_SOE(uint8_t pdrv, uint8_t *pbuf)//读soe
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/Array(InfoAddr_L/InfoAddr_M/InfoAddr_H/Value/CP56Time2a) 
    uint16_t i = 0 ,j = 0,temp1 = 0;  
    uint8_t sendnum = 0;  
    uint16_t Property = 0xffff;
    struct SOE_Str soe_temp;
    
    for(i=0;g_FlagDB.queue_soe_new.in != g_FlagDB.queue_soe_new.out[DLT634_5104Slave_Pad[pdrv].Port];i++)
    {
        for(j = 0,temp1 = 0;j< g_SOENewDB[g_FlagDB.queue_soe_new.out[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YX_FirstAddr;j++)
        {
            temp1 += (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_NUM) + 1 + 1;            
        }
            
        if((sendnum != 0)&&(((g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_PROPERTY)&0xff) != (Property)))
        {
            break;//与上一个属性不同
        }
        
        Property = (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_PROPERTY)&0xff;
        
        memcpy(&soe_temp,&(g_SOENewDB[g_FlagDB.queue_soe_new.out[DLT634_5104Slave_Pad[pdrv].Port]]),sizeof(struct SOE_Str));
        
        if((Property>>NEWPROPERTY_NEG)&NEWPROPERTY_JUDG)//取反
        {
            soe_temp.value = (~soe_temp.value)&0x03;        
        }
            
        if(((Property>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG) == DLT634_5104SLAVE_M_SP_NA_1)//单点
        {
            soe_temp.value = (soe_temp.value>>1)&0x01;        
        }  

        temp_array[pdrv][sendnum*(sizeof(struct SOE_Str)+1) + 8] = LOBYTE(soe_temp.addr);
        temp_array[pdrv][sendnum*(sizeof(struct SOE_Str)+1) + 9] = HIBYTE(soe_temp.addr);
        temp_array[pdrv][sendnum*(sizeof(struct SOE_Str)+1) + 10] = 0;
        memcpy(&temp_array[pdrv][sendnum*(sizeof(struct SOE_Str)+1) + 11],&soe_temp.value,sizeof(struct SOE_Str)-2);   

        if (++(g_FlagDB.queue_soe_new.out[DLT634_5104Slave_Pad[pdrv].Port]) >= SOE_NEW_MAX_NUM)
        {
            g_FlagDB.queue_soe_new.out[DLT634_5104Slave_Pad[pdrv].Port] = 0;
        }        
        
        sendnum++;
        
        if(sendnum >= 10)
        {
            break;//大于每包数量
        }        
    }
   
    temp_array[pdrv][0] = 8 + sendnum*(sizeof(struct SOE_Str)+1);
    if(((Property>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG) == DLT634_5104SLAVE_M_SP_NA_1)
    {temp_array[pdrv][2] = DLT634_5104SLAVE_M_SP_TB_1;}
    else
    {temp_array[pdrv][2] = DLT634_5104SLAVE_M_DP_TB_1;}
    temp_array[pdrv][3] = sendnum;
    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_SPONT;
    temp_array[pdrv][5] = 0;
    temp_array[pdrv][6] = LOBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);
    temp_array[pdrv][7] = HIBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);

    memcpy(pbuf,temp_array[pdrv],temp_array[pdrv][0]);
}

/**
  * @brief : judg nva.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_SLAVE_H_NVA(uint8_t pdrv)//判断是否有NVA
{ 
    while(1)
    {
        if(g_NVADBIn != g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port])
        {
            if(g_NewAddTelemetry[g_NVADB[g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr] == 0)
            {
                if (++(g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]) >= NVA_MAX_NUM)
                {
                    g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port] = 0;
                }
                continue;
            }
            return(1);
        }
        break;
    }
    return(0); 
}

/**
  * @brief : read nva.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_R_NVA(uint8_t pdrv, uint8_t *pbuf)//读NVA
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/Array(InfoAddr_L/InfoAddr_M/InfoAddr_H/Value/QDS) 
    uint16_t i = 0;  
    uint8_t sendnum = 0;  
    uint16_t Property = 0xffff;
    struct NVA_Str nva_temp;
    int16_t tempu;
	uint32_t tempf;
    
    for(i=0;g_NVADBIn != g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port];i++)
    {
        if(Property == 0xffff)
        {
            if(g_NewAddTelemetry[g_NVADB[g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr] == 0)
            {
                if (++(g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]) >= NVA_MAX_NUM)
                {
                    g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port] = 0;
                }
                continue;//找到非0地址
            }
            else    
            {
                Property = ((g_NewPropertyTelemetry[g_NVADB[g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr]>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG)*2 + DLT634_5104SLAVE_M_ME_NA_1;
            }        
        }
        else
        {
			if(g_NewAddTelemetry[g_NVADB[g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr] == 0)
            {
                break;//地址=0
            } 
            if((((g_NewPropertyTelemetry[g_NVADB[g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr]>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG)*2 + DLT634_5104SLAVE_M_ME_NA_1) != (Property))
            {
                break;//与上一个属性不同
            }      
        }
        
        memcpy(&nva_temp,&(g_NVADB[g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]]),sizeof(struct NVA_Str)); 
        
        nva_temp.addr = g_NewAddTelemetry[(nva_temp.addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr)];
        
        switch(Property)
        {
            case DLT634_5104SLAVE_M_ME_NA_1:
                tempu = (int16_t)(nva_temp.value/(g_NewMultipleRateTelemetry[g_NVADB[g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr])*32768);
                temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + 8] = LOBYTE(nva_temp.addr);
                temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + 9] = HIBYTE(nva_temp.addr);
                temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + 10] = 0;
                temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + sizeof(nva_temp.addr)+ sizeof(int16_t)+1 + 8] = 0x00;//QDS                
                memcpy(&temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + 11],(uint8_t *)&tempu,sizeof(int16_t));
                break;
            case DLT634_5104SLAVE_M_ME_NB_1:
                tempu = (int16_t)(nva_temp.value*(g_NewMultipleRateTelemetry[g_NVADB[g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr]));            
                temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + 8] = LOBYTE(nva_temp.addr);
                temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + 9] = HIBYTE(nva_temp.addr);
                temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + 10] = 0;
                temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + sizeof(nva_temp.addr)+ sizeof(int16_t)+1 + 8] = 0x00;//QDS                
                memcpy(&temp_array[pdrv][sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1) + 11],(uint8_t *)&tempu,sizeof(int16_t));
                break;
            case DLT634_5104SLAVE_M_ME_NC_1:
				tempf = FloatToBin(nva_temp.value*(g_NewMultipleRateTelemetry[g_NVADB[g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]].addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr])); 
                temp_array[pdrv][sendnum*(sizeof(struct NVA_Str)+1+1) + 8] = LOBYTE(nva_temp.addr);
                temp_array[pdrv][sendnum*(sizeof(struct NVA_Str)+1+1) + 9] = HIBYTE(nva_temp.addr);
                temp_array[pdrv][sendnum*(sizeof(struct NVA_Str)+1+1) + 10] = 0;
                temp_array[pdrv][sendnum*(sizeof(struct NVA_Str)+1+1) + sizeof(struct NVA_Str)+1 + 8] = 0x00;//QDS                
                memcpy(&temp_array[pdrv][sendnum*(sizeof(struct NVA_Str)+1+1) + 11],(uint8_t *)&tempf,sizeof(float));  
                break;                    
        }       

        if (++(g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port]) >= NVA_MAX_NUM)
        {
            g_NVADBOut[DLT634_5104Slave_Pad[pdrv].Port] = 0;
        }        
        
        sendnum++;
        
        if(sendnum >= 10)
        {
            break;//大于每包数量
        }        
    }
    
        
    switch(Property)
    {
        case DLT634_5104SLAVE_M_ME_NA_1:
        case DLT634_5104SLAVE_M_ME_NB_1:
            temp_array[pdrv][0] = 8 + sendnum*(sizeof(nva_temp.addr)+ sizeof(int16_t)+1+1);
            break;
        case DLT634_5104SLAVE_M_ME_NC_1:
            temp_array[pdrv][0] = 8 + sendnum*(sizeof(struct NVA_Str)+1+1);  
            break;                    
    }
 
    temp_array[pdrv][2] = Property;
    temp_array[pdrv][3] = sendnum;
    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_SPONT;
    temp_array[pdrv][5] = 0;
    temp_array[pdrv][6] = LOBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);
    temp_array[pdrv][7] = HIBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);

    memcpy(pbuf,temp_array[pdrv],temp_array[pdrv][0]);
}

/**
  * @brief : judg fevent.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_SLAVE_H_FEvent(uint8_t pdrv)//判断是否有FEvent
{ 
    return(0);
}

/**
  * @brief : read fevent.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_R_FEvent(uint8_t pdrv, uint8_t *pbuf)//读FEvent
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/Array
}

/**
  * @brief : read yx.
  * @param : [pdrv]
  * @param : [addr]
  * @param : [num]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint16_t DLT634_5104_SLAVE_ReadYxData(uint8_t pdrv, uint16_t addr, uint16_t num, uint8_t *pbuf)//读YXDATA
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/Array(Value) 
    uint16_t i = 0,j = 0;  
    uint8_t sendnum = 0;  
    uint16_t Property = 0xffff;
    uint16_t start_add = 0;
    uint16_t temp1 = 0;
    uint8_t value,valuetemp;
    
    for(i=0,temp1=0;i<addr - DLT634_5104Slave_Pad[pdrv].YX_FirstAddr;i++)
    {
        temp1 += (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_NUM) + 1 + 1;
    }
    
    for(i=0;;i++,temp1 += (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_NUM) + 1 + 1)
    {
        if(Property == 0xffff)
        {
            if(((g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_NUM)&0xff) == 0)
            {
                continue;//找到非0地址
            }
            else    
            {
                Property = (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_PROPERTY)&NEWJUDG_PROPERTY;
                start_add = (g_NewToOldTelesignal[temp1]>>NEWONEYX_ADDR)&NEWJUDG_ADDR;
            }        
        }
        
        if(((g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_NUM)&0xff) == 0)
        {
            temp_array[pdrv][sendnum + 11] = 1;                 
        }
        else     
        {
            Property = (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_PROPERTY)&NEWJUDG_PROPERTY;
            for(j=0,value=0;j<(g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_NUM);j++)
            {
                valuetemp = *(TelesignalCfg[((g_NewToOldTelesignal[temp1 + 2 + j]>>NEWONEYX_ADDR)&NEWJUDG_ADDR) - DLT634_5104Slave_Pad[pdrv].YX_FirstAddr].pVal) - 1;
                if((g_NewToOldTelesignal[temp1 + 2 + j]>>NEWONEYX_CAL>>NEWCAL_NEG)&NEWPROPERTY_JUDG)
                {
                    valuetemp = (~valuetemp)&0x01;                
                }
                if(((g_NewToOldTelesignal[temp1 + 2 + j]>>NEWONEYX_CAL>>NEWCAL_AND)&NEWPROPERTY_JUDG) == NEWJUDG_AND)
                {
                    value &= valuetemp;                 
                }  
                else
                {
                    value |= valuetemp;  
                }   
            }
            
            value = value + 1;
            
            temp_array[pdrv][sendnum + 11] = value;
            
            if((Property>>NEWPROPERTY_NEG)&NEWPROPERTY_JUDG)//取反
            {
                temp_array[pdrv][sendnum + 11] = (~temp_array[pdrv][sendnum + 11])&0x03;        
            }       
        }  
        
        if(((Property>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG) == DLT634_5104SLAVE_M_SP_NA_1)//单点
        {
            temp_array[pdrv][sendnum + 11] = (temp_array[pdrv][sendnum + 11]>>1)&0x01;        
        }         
        
        sendnum++;
        
        if(sendnum >= num)
        {
            break;//大于需要个数
        }
        if((addr - DLT634_5104Slave_Pad[pdrv].YX_FirstAddr + i + 1) >= DLT634_5104Slave_Pad[pdrv].YX_AllNum)
        {
            break;//大于最大个数
        }
		if(((g_NewToOldTelesignal[temp1 + 1 + (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_NUM) + 1 + 1]>>NEWONEYX_NUM)&0xff) == 0)
        {
            continue;//下一个点号=0
        }
        if(((g_NewToOldTelesignal[temp1 + 1 + (g_NewToOldTelesignal[temp1 + 1]>>NEWONEYX_NUM) + 1 + 1]>>NEWONEYX_PROPERTY>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG) != ((Property>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG))
        {
            break;//下一个属性不同
        }
    }
    
    temp_array[pdrv][0] = 11 + sendnum;
    temp_array[pdrv][2] = ((Property>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG);
    temp_array[pdrv][3] = sendnum|0x80;
    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_INTROGEN;
    temp_array[pdrv][5] = 0;
    temp_array[pdrv][6] = LOBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);
    temp_array[pdrv][7] = HIBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);
    temp_array[pdrv][8] = LOBYTE(start_add);
    temp_array[pdrv][9] = HIBYTE(start_add);
    temp_array[pdrv][10] = 0x00;
    memcpy(pbuf,temp_array[pdrv],temp_array[pdrv][0]);
    
    return(start_add + sendnum);
}

/**
  * @brief : read yc.
  * @param : [pdrv]
  * @param : [addr]
  * @param : [num]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint16_t DLT634_5104_SLAVE_ReadYcData(uint8_t pdrv, uint16_t addr, uint16_t num, uint8_t *pbuf)//读YCDATA
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/Array(Value) 
    uint16_t i = 0;  
    uint8_t sendnum = 0;  
    uint16_t Property = 0xffff;
    uint16_t start_add = 0;
    uint32_t tempf;
    int16_t tempu;
    
    for(i=0;;i++)
    {
        if(Property == 0xffff)
        {
            if(g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i] == 0)
            {
                continue;//找到非0地址
            }
            else    
            {
                Property = ((g_NewPropertyTelemetry[g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i] - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr]>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG)*2 + DLT634_5104SLAVE_M_ME_NA_1;
                start_add = g_NewAddTelemetry[g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i] - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr];
            }        
        } 
        
        switch(Property)
        {
            case DLT634_5104SLAVE_M_ME_NA_1:
                tempu = (int16_t)(*TelemetryCfg[g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i] - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr].pVal/\
                        (g_NewMultipleRateTelemetry[g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i] - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr])*32768);
                temp_array[pdrv][sendnum*(1+sizeof(uint16_t)) + sizeof(uint16_t) + 11] = 0x00;//QDS
                memcpy(&temp_array[pdrv][sendnum*(1+sizeof(uint16_t)) + 11],&tempu,sizeof(uint16_t)); 
                break;
            case DLT634_5104SLAVE_M_ME_NB_1:
                tempu = (int16_t)(*TelemetryCfg[g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i] - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr].pVal*\
                        (g_NewMultipleRateTelemetry[g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i] - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr]));
                temp_array[pdrv][sendnum*(1+sizeof(uint16_t)) + sizeof(uint16_t) + 11] = 0x00;//QDS
                memcpy(&temp_array[pdrv][sendnum*(1+sizeof(uint16_t)) + 11],&tempu,sizeof(uint16_t)); 
                break;
            case DLT634_5104SLAVE_M_ME_NC_1:
                tempf = FloatToBin(*TelemetryCfg[g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i] - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr].pVal*\
                        (g_NewMultipleRateTelemetry[g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i] - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr]));
                temp_array[pdrv][sendnum*(1+sizeof(uint32_t)) + sizeof(uint32_t) + 11] = 0x00;//QDS
                memcpy(&temp_array[pdrv][sendnum*(1+sizeof(uint32_t)) + 11],&tempf,sizeof(uint32_t));   
                break;                    
        }   
        
        sendnum++;
        
        if(sendnum >= num)
        {
            break;//大于需要个数
        }
        if((addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i + 1) >= DLT634_5104Slave_Pad[pdrv].YC_AllNum)
        {
            break;//大于最大个数
        }
		if(g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i + 1] == 0)
        {
            break;//下一个点号等于0
        }
        if((((g_NewPropertyTelemetry[g_NewToOldTelemetry[addr - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr + i + 1] - DLT634_5104Slave_Pad[pdrv].YC_FirstAddr]>>NEWPROPERTY_TI)&NEWPROPERTY_JUDG)*2 + DLT634_5104SLAVE_M_ME_NA_1) != (Property))
        {
            break;//下一个属性不同
        }
    }
    
    switch(Property)
    {
        case DLT634_5104SLAVE_M_ME_NA_1:
        case DLT634_5104SLAVE_M_ME_NB_1:
            temp_array[pdrv][0] = 11 + sendnum*(1+sizeof(uint16_t));
            break;
        case DLT634_5104SLAVE_M_ME_NC_1:
            temp_array[pdrv][0] = 11 + sendnum*(1+sizeof(uint32_t));   
            break;                    
    } 

    temp_array[pdrv][2] = Property;
    temp_array[pdrv][3] = sendnum|0x80;
    temp_array[pdrv][4] = DLT634_5104SLAVE_COT_INTROGEN;
    temp_array[pdrv][5] = 0;
    temp_array[pdrv][6] = LOBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);
    temp_array[pdrv][7] = HIBYTE(DLT634_5104Slave_Pad[pdrv].PubAddress);
    temp_array[pdrv][8] = LOBYTE(start_add);
    temp_array[pdrv][9] = HIBYTE(start_add);
    temp_array[pdrv][10] = 0x00;
    memcpy(pbuf,temp_array[pdrv],temp_array[pdrv][0]);
    
    return(start_add + sendnum);
}

/**
  * @brief : updata command.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_SoftwareUpdate(uint8_t pdrv, uint8_t *pbuf)//升级操作
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/CTYPE 
    
	memcpy(temp_array[pdrv],pbuf,sizeof(temp_array[pdrv]));
	
    switch(pdrv)
    {
        case 0:
            switch(pbuf[4])
            {
                case 6:
                    temp_array[pdrv][4] = 7;
                    if(pbuf[11]&0x80)
                    {
                        //启动升级
                        rt_multi_common_write_update_state(SYS_HAVE_UPDATE);
                    }
                    else
                    {
                        //结束升级
                        rt_multi_common_data_fram_update_state_write(UPDATE_START);
                        g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKRESET;
                    }
                    break;
                case 8:
                    temp_array[pdrv][4] = 9;
                    rt_multi_common_write_update_state(SYS_UPDATE_FAILED);          
                    break;
                default:
                    temp_array[pdrv][4] = 47;
                    rt_multi_common_write_update_state(SYS_UPDATE_FAILED); 
                    break;
            }
            break;
        default:
            break;
    }

    DLT634_5104_SLAVE_Reply(pdrv,temp_array[pdrv]);
}

/**
  * @brief : file command.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_FileHandleProcess(uint8_t pdrv, uint8_t *pbuf)//文件操作
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/InfoAddr_L/InfoAddr_M/InfoAddr_H/sty/Array    
    if((g_CommunicatFlag[COM_FILE]&(1<<DLT634_5104Slave_Pad[pdrv].Port))||(!(g_CommunicatFlag[COM_FILE]&COMMUNICATLOCKUSERSTA)))
    {
        switch(pbuf[12])
        {
            //写文件激活
            case 7:
                if(pbuf[0] < 11)
                {return;}                
                memcpy(&pbuf[10],&pbuf[11],pbuf[0]-11);
                pbuf[0] -= 1;
                memcpy(temp_array[pdrv],pbuf,pbuf[0]);
                file_operate_WriteFileAct(DLT634_5104Slave_Pad[pdrv].Port,temp_array[pdrv]);
                memcpy(pbuf,temp_array[pdrv],temp_array[pdrv][0]);
                if(temp_array[pdrv][0] < 10)
                {return;}
                memcpy(&pbuf[11],&temp_array[pdrv][10],temp_array[pdrv][0]-10); 
                pbuf[10] = 0;
                pbuf[0] += 1;
                DLT634_5104_SLAVE_Reply(pdrv, pbuf);  
                g_CommunicatFlag[COM_FILE] |= (1<<DLT634_5104Slave_Pad[pdrv].Port); 
                g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKCLRCON;                 
                break;
            //写文件确认
            case 9:
                if(pbuf[0] < 11)
                {return;} 
                memcpy(&pbuf[10],&pbuf[11],pbuf[0]-11);
                pbuf[0] -= 1;
                memcpy(temp_array[pdrv],pbuf,pbuf[0]);
                file_operate_WriteFileAct(DLT634_5104Slave_Pad[pdrv].Port,temp_array[pdrv]);
                memcpy(pbuf,temp_array[pdrv],temp_array[pdrv][0]);
                if(temp_array[pdrv][0] < 10)
                {
                    g_CommunicatFlag[COM_FILE] |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
                    g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKCLRCON; 
                    return;
                }
                memcpy(&pbuf[11],&temp_array[pdrv][10],temp_array[pdrv][0]-10); 
                pbuf[10] = 0;
                pbuf[0] += 1;
                DLT634_5104_SLAVE_Reply(pdrv, pbuf);  
                g_CommunicatFlag[COM_FILE] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);                
                break;
            //读目录
            case 1:
            //读文件
            case 3:
            case 6:
                memcpy(file_array[pdrv],pbuf,pbuf[0]);
                g_CommunicatFlag[COM_FILE] |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
                g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKCLRCON;
                break;
            default:
                break;
        }
    }
    else
    {
        pbuf[4] = 47;
        DLT634_5104_SLAVE_Reply(pdrv, pbuf);
    }    
}

/**
  * @brief : send idle.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void DLT634_5104_SLAVE_IdleProcess(uint8_t pdrv, uint8_t *pbuf)//发送空闲回调函数
{//LENTH/Lock_ID/TypeID/VSQ/COT_L/COT_H/PubAddr_L/PubAddr_H/Array(InfoAddr_L/InfoAddr_M/InfoAddr_H/Value/QDS) 
    if(file_array[pdrv][0] != 0)
    {
        if(file_array[pdrv][0] < 11)
        {return;}
        memcpy(&file_array[pdrv][10],&file_array[pdrv][11],file_array[pdrv][0]-11);
        file_array[pdrv][0] -= 1;
        switch(file_array[pdrv][11])
        {
            //读目录
            case 1:
                if(file_operate_DirCall(DLT634_5104Slave_Pad[pdrv].Port,file_array[pdrv]))
                {
                    g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKREADDIR;
                    g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKCLRCON;
                    g_CommunicatFlag[COM_FILE] |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
                else
                {
                    g_CommunicatFlag[COM_FILE] &= ~COMMUNICATLOCKREADDIR;
                    g_CommunicatFlag[COM_FILE] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
                memcpy(pbuf,file_array[pdrv],file_array[pdrv][0]);
                break;
            //读文件激活
            case 3:
            //读文件确认
            case 6:
                if(file_operate_ReadFileAct(DLT634_5104Slave_Pad[pdrv].Port,file_array[pdrv]))
                {
                    g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKREADFILE;
                    g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKCLRCON;
                    g_CommunicatFlag[COM_FILE] |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
                else
                {
                    g_CommunicatFlag[COM_FILE] &= ~COMMUNICATLOCKREADFILE;
                    g_CommunicatFlag[COM_FILE] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
                }
                memcpy(pbuf,file_array[pdrv],file_array[pdrv][0]);
                break;
            default:
                break;
        }
        memcpy(temp_array[pdrv],pbuf,pbuf[0]);
        if(pbuf[0] < 10)
        {return;}
        memcpy(&pbuf[11],&temp_array[pdrv][10],temp_array[pdrv][0]-10);        
        pbuf[10] = 0;
        pbuf[0] += 1;
        memset(file_array[pdrv],0,file_array[pdrv][0]);
    }
    else
    {
        if((g_CommunicatFlag[COM_FILE]&COMMUNICATLOCKREADDIR)&&(g_CommunicatFlag[COM_FILE]&(1<<DLT634_5104Slave_Pad[pdrv].Port)))
        {
            if(file_operate_DirRead(DLT634_5104Slave_Pad[pdrv].Port,pbuf))
            {
                g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKREADDIR;
                g_CommunicatFlag[COM_FILE] |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
                g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKCLRCON;
            }
            else
            {
                g_CommunicatFlag[COM_FILE] &= ~COMMUNICATLOCKREADDIR;
                g_CommunicatFlag[COM_FILE] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
            }
            memcpy(temp_array[pdrv],pbuf,pbuf[0]);
            if(pbuf[0] < 10)
            {return;}
            memcpy(&pbuf[11],&temp_array[pdrv][10],temp_array[pdrv][0]-10);        
            pbuf[10] = 0;
            pbuf[0] += 1;
        }
        if((g_CommunicatFlag[COM_FILE]&COMMUNICATLOCKREADFILE)&&(g_CommunicatFlag[COM_FILE]&(1<<DLT634_5104Slave_Pad[pdrv].Port)))
        {
            if(file_operate_ReadFile(DLT634_5104Slave_Pad[pdrv].Port,pbuf))
            {
                g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKREADFILE;
                g_CommunicatFlag[COM_FILE] |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
                g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKCLRCON;
            }
            else
            {
                g_CommunicatFlag[COM_FILE] &= ~COMMUNICATLOCKREADFILE;
                g_CommunicatFlag[COM_FILE] &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
            }
            memcpy(temp_array[pdrv],pbuf,pbuf[0]);
            if(pbuf[0] < 10)
            {return;}
            memcpy(&pbuf[11],&temp_array[pdrv][10],temp_array[pdrv][0]-10);        
            pbuf[10] = 0;
            pbuf[0] += 1;
        }
    }
}


/**
  * @brief : other reply.
  * @param : [pdrv]
  * @param : [pbuf]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t DLT634_5104_SLAVE_C_REPLY(uint8_t drvid, uint8_t *pbuf)//其他设备回复
{ 
    return(0);
}

/**
  * @brief : init
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
int DLT634_5104_ParaInit(void)
{
    uint8_t pdrv;
    
    for(pdrv=0;pdrv<DLT634_5104SLAVE_VOLUMES;pdrv++)
    {
        switch(pdrv)
        {
            case DLT634_5104SLAVE_DISK0:
                memset(file_array[pdrv],0,sizeof(file_array[pdrv]));
                DLT634_5104Slave_Pad[pdrv].Port = SLAVE104_ID0; // 端口号

                DLT634_5104Slave_Pad[pdrv].LinkAddress = (uint16_t)g_Parameter[NET_SOURCEADDR]; // 链路地址
                DLT634_5104Slave_Pad[pdrv].PubAddress  = (uint16_t)g_Parameter[NET_ASDUADDR]; // 公共地址
                    
                DLT634_5104Slave_Pad[pdrv].CotSize = 2;        // 传输原因字节数
                DLT634_5104Slave_Pad[pdrv].InfoAddrSize = 3;   // 信息体地址字节数
                DLT634_5104Slave_Pad[pdrv].LinkAddrSize = 2;   // 链路层地址字节数
                DLT634_5104Slave_Pad[pdrv].PubAddrSize = 2;    // 公共地址字节数

                DLT634_5104Slave_Pad[pdrv].ClockTimers = 1;    // 非0
                DLT634_5104Slave_Pad[pdrv].YX_AllNum = g_NewMaxNumTelesignal;
                DLT634_5104Slave_Pad[pdrv].YX_FirstAddr = TELESIGNAL_START_ADDR;
                DLT634_5104Slave_Pad[pdrv].YX_FrameNum = 100;
                DLT634_5104Slave_Pad[pdrv].YC_AllNum = g_NewMaxNumTelemetry;
                DLT634_5104Slave_Pad[pdrv].YC_FirstAddr = TELEMETRY_START_ADDR;
                DLT634_5104Slave_Pad[pdrv].YC_FrameNum = 24;

                DLT634_5104Slave_Pad[pdrv].TickValue[0] = DLT634_5104SLAVE_T0/DLT634_5104Slave_Pad[pdrv].ClockTimers;  // 30s 连接建立的超时
                DLT634_5104Slave_Pad[pdrv].TickValue[1] = DLT634_5104SLAVE_T1/DLT634_5104Slave_Pad[pdrv].ClockTimers;  // 15s 发送或测试APDU的超时
                DLT634_5104Slave_Pad[pdrv].TickValue[2] = DLT634_5104SLAVE_T2/DLT634_5104Slave_Pad[pdrv].ClockTimers;  // 10s 无数据报文t2 t1状态下发送测试帧的超时
                DLT634_5104Slave_Pad[pdrv].TickValue[3] = DLT634_5104SLAVE_T3/DLT634_5104Slave_Pad[pdrv].ClockTimers;  // 20s 若在t3内未接收到任何报文，将向对方发送测试链路帧。
				DLT634_5104_SLAVE_AppInit(pdrv);
                break;
            case DLT634_5104SLAVE_DISK1:
                memset(file_array[pdrv],0,sizeof(file_array[pdrv]));
                DLT634_5104Slave_Pad[pdrv].Port = SLAVE104_ID1; // 端口号

                DLT634_5104Slave_Pad[pdrv].LinkAddress = (uint16_t)g_Parameter[NET_SOURCEADDR]; // 链路地址
                DLT634_5104Slave_Pad[pdrv].PubAddress  = (uint16_t)g_Parameter[NET_ASDUADDR]; // 公共地址
                    
                DLT634_5104Slave_Pad[pdrv].CotSize = 2;        // 传输原因字节数
                DLT634_5104Slave_Pad[pdrv].InfoAddrSize = 3;   // 信息体地址字节数
                DLT634_5104Slave_Pad[pdrv].LinkAddrSize = 2;   // 链路层地址字节数
                DLT634_5104Slave_Pad[pdrv].PubAddrSize = 2;    // 公共地址字节数

                DLT634_5104Slave_Pad[pdrv].ClockTimers = 1;    // 非0
                DLT634_5104Slave_Pad[pdrv].YX_AllNum = g_NewMaxNumTelesignal;
                DLT634_5104Slave_Pad[pdrv].YX_FirstAddr = TELESIGNAL_START_ADDR;
                DLT634_5104Slave_Pad[pdrv].YX_FrameNum = 100;
                DLT634_5104Slave_Pad[pdrv].YC_AllNum = g_NewMaxNumTelemetry;
                DLT634_5104Slave_Pad[pdrv].YC_FirstAddr = TELEMETRY_START_ADDR;
                DLT634_5104Slave_Pad[pdrv].YC_FrameNum = 24;

                DLT634_5104Slave_Pad[pdrv].TickValue[0] = DLT634_5104SLAVE_T0/DLT634_5104Slave_Pad[pdrv].ClockTimers;  // 30s 连接建立的超时
                DLT634_5104Slave_Pad[pdrv].TickValue[1] = DLT634_5104SLAVE_T1/DLT634_5104Slave_Pad[pdrv].ClockTimers;  // 15s 发送或测试APDU的超时
                DLT634_5104Slave_Pad[pdrv].TickValue[2] = DLT634_5104SLAVE_T2/DLT634_5104Slave_Pad[pdrv].ClockTimers;  // 10s 无数据报文t2 t1状态下发送测试帧的超时
                DLT634_5104Slave_Pad[pdrv].TickValue[3] = DLT634_5104SLAVE_T3/DLT634_5104Slave_Pad[pdrv].ClockTimers;  // 20s 若在t3内未接收到任何报文，将向对方发送测试链路帧。
                DLT634_5104_SLAVE_AppInit(pdrv);
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
void DLT634_5104_SlaveTask()
{
    uint8_t pdrv;
    
    for (pdrv=0; pdrv<DLT634_5104SLAVE_VOLUMES; pdrv++)
    {
        DLT634_5104_SLAVE_Clock(pdrv);
//        if(DLT634_5104_SLAVE_Clock(pdrv))
//        {
//            g_ComLed |= (1<<DLT634_5104Slave_Pad[pdrv].Port);
//        }
//        else
//        {
//            g_ComLed &= ~(1<<DLT634_5104Slave_Pad[pdrv].Port);
//        }
    }
} 

/* END OF FILE ---------------------------------------------------------------*/


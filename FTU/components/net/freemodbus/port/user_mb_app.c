/**
  *            Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file    : user_mb_app.c
  * @brief   : FreeModbus user app.
  * @version : V03.001
  * @author  : SUN
  * @date    : 2018-01-09
  * @update  : [2018-01-11][Sunxr][make the code cleanup]
  * @update  : [2018-02-28][Lexun][make code cleanup]
  */
	

/* INCLUDE FILES -------------------------------------------------------------*/
#include "string.h"
#include "drv_do.h"
#include "drv_fm25vxx.h"
#include "user_mb_app.h"
#include "common_data.h"
#include "file_operate.h"


/* Slave mode use these variables --------------------------------------------*/
//Slave mode:DiscreteInputs variables
static USHORT   usSDiscInStart                               = S_DISCRETE_INPUT_START;
#if S_DISCRETE_INPUT_NDISCRETES%8
static UCHAR    ucSDiscInBuf[S_DISCRETE_INPUT_NDISCRETES/8+1];
#else
static UCHAR    ucSDiscInBuf[S_DISCRETE_INPUT_NDISCRETES/8]  ;
#endif
//Slave mode:Coils variables
static USHORT   usSCoilStart                                 = S_COIL_START;
#if S_COIL_NCOILS%8
static UCHAR    ucSCoilBuf[S_COIL_NCOILS/8+1]                ;
#else
//static UCHAR    ucSCoilBuf[S_COIL_NCOILS/8]                  ;
#endif
//Slave mode:InputRegister variables
static USHORT   usSRegInStart                                = S_REG_INPUT_START;
static USHORT   usSRegInBuf[S_REG_INPUT_NREGS]               ;
//Slave mode:HoldingRegister variables
//static USHORT   usSRegHoldStart                              = S_REG_HOLDING_START;
//static USHORT   usSRegHoldBuf[S_REG_HOLDING_NREGS]           ;

struct TimeTelesignal *psCoilTimeTelesignal;
static float *pSCoilTelemetry;
static float *pSCoilFixedAndPara;
static struct Event_Str sCoilSOEData[5];
static struct M_FaultEvent sCoilFaultEventData[5];
static rt_device_t device_fram = RT_NULL; 


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : judge addr type
  * @param : [addr]-received addr
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
UCHAR eMBRegHoldingJudgeAddr(USHORT usAddress)
{
	UCHAR ucType = 0;
	 
    if (REG_TIME_TELESIGNAL(usAddress))
    {
        ucType = REG_TIME_TELESIGNAL;
    }
    else if (REG_TELEMETRY(usAddress))
    {
        ucType = REG_TELEMETRY;
    }
    else if (REG_TELECONTRL(usAddress))
    {
        ucType = REG_TELECONTRL;
    }
    else if (REG_INHERENT(usAddress))
	{
	    ucType = REG_INHERENT;
	}
	else if (REG_RUNPARAMETER(usAddress))
    {
        ucType = REG_RUN_PARA;
    }
    else if (REG_PUBLIC_FIXED(usAddress))
    {
        ucType = REG_PUBLIC_FIXED;
    }
    else if (REG_BREAK_FIXED(usAddress))
    {
        ucType = REG_BREAKER_FIXED;
    }
    else if (REG_LOAD_FIXED(usAddress))
    {
        ucType = REG_LOAD_FIXED;
    }
    else if (REG_SOE(usAddress))
    {
        ucType = REG_SOE;
    }
    else if (REG_OPERATE(usAddress))
    {
        ucType = REG_OPERATE;
    }
    else if (REG_EVENT(usAddress))
    {
        ucType = REG_EVENT;
    }

    return(ucType);
}

/**
  * @brief : Modbus slave get the num of record 
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
static void eMBRegGetRecordNum(void)
{
    uint16_t soenum = 0;
	uint16_t feventnum = 0;
    
    if (g_FlagDB.queue_soe.full == FULL)
    {
        soenum = SOE_MAX_NUM;
    }
    else
    {
        soenum = g_FlagDB.queue_soe.in;
    }
 
    if (g_FlagDB.queue_fevent.full == FULL)
    {
        feventnum = FEVENT_MAX_NUM;
    }
    else
    {
        feventnum = g_FlagDB.queue_fevent.in;
    }
	
	psCoilTimeTelesignal->feventNum = feventnum;
    psCoilTimeTelesignal->soePageNum = soenum % REG_SOE_FRAM_NUM ? soenum / REG_SOE_FRAM_NUM + 1 : soenum / REG_SOE_FRAM_NUM;
}

/**
  * @brief : Modbus slave record check 
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
static void eMBRegRecordCheck(void)
{
    static uint16_t soe_in = 0;
    static uint16_t fevent_in = 0;
	
    eMBRegGetRecordNum();    
    
	if (soe_in != g_FlagDB.queue_soe.in)
    {        
        psCoilTimeTelesignal->allFlag |= REG_HAVESOE;

        soe_in = g_FlagDB.queue_soe.in;
    }	
	
	if (fevent_in != g_FlagDB.queue_fevent.in)
    {        
        psCoilTimeTelesignal->allFlag |= REG_HAVEFEVENT;

        fevent_in = g_FlagDB.queue_fevent.in;
    }	
}

/**
  * @brief :  read time and YX
  * @param :  none
  * @return:  none
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
static void eMBRegHoldingReadTimeTelesignal(void)
{
    UCHAR i;
    ULONG loWord;
    ULONG hiWord;

    psCoilTimeTelesignal->telesignalState = 0x0000;

    psCoilTimeTelesignal->time.year = g_SystemTime.year;
    psCoilTimeTelesignal->time.month = g_SystemTime.month;
    psCoilTimeTelesignal->time.dayofWeek = g_SystemTime.day | g_SystemTime.week << 5;
    psCoilTimeTelesignal->time.hour = g_SystemTime.hour;
    psCoilTimeTelesignal->time.minute = g_SystemTime.minute;
    psCoilTimeTelesignal->time.msecondH = HIBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);
    psCoilTimeTelesignal->time.msecondL = LOBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);

    for (i = 0; i < TELESIGNAL_HMI_NUM; i++)
    {
        if (g_TelesignalDB.buf[i] == ON)
        {
            psCoilTimeTelesignal->telesignalState |= (1 << i);
        }
    }
    
    eMBRegRecordCheck();    

    hiWord = (HIWORD(psCoilTimeTelesignal->telesignalState) & 0xff) << 8 | (HIWORD(psCoilTimeTelesignal->telesignalState) & 0xff00) >> 8;
    loWord = (LOWORD(psCoilTimeTelesignal->telesignalState) & 0xff) << 8 | (LOWORD(psCoilTimeTelesignal->telesignalState) & 0xff00) >> 8;

    psCoilTimeTelesignal->telesignalState = hiWord << 16 | loWord;
}

/**
  * @brief : read fixed value and parameter
  * @param : [eDataType]-the type of fixed value and parameter
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
static rt_uint16_t eMBRegHoldingReadFixedAndPara(UCHAR ucDataType)
{
    USHORT usStartAddr;

    switch (ucDataType)
    {
//		case REG_RUN_PARA:
//			pSCoilFixedAndPara = g_ParameterDB.Data.runPara.buf;
//			usStartAddr = REG_RUNPARAMETER_START_ADDR;
//			break;
//		case REG_PUBLIC_FIXED:
//			pSCoilFixedAndPara = g_pFixedValue->Data.publicValue.buf;
//			usStartAddr = REG_PUBLIC_FIXED_START_ADDR;
//			break;
//		case REG_BREAKER_FIXED:
//			pSCoilFixedAndPara = g_pFixedValue->Data.breakerValue.buf;
//			usStartAddr = REG_BREAKER_FIXED_START_ADDR;
//			break;
//		case REG_LOAD_FIXED:
//			pSCoilFixedAndPara = g_pFixedValue->Data.loadSwitchValue.buf;
//			usStartAddr = REG_LOAD_FIXED_START_ADDR;
//			break;
		default:
			break;
    }

    return usStartAddr;
}

/**
  * @brief : read SOE
  * @param : [usN]-NO.n
  * @param : [usNum]-the num of soe
  * @param : [psCoilSOEData]-data pointer
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
static void eMBRegHoldingReadSOE(USHORT usN, USHORT usNum, struct Event_Str *psCoilSOEData)
{
    UCHAR  i;
    USHORT pos; 
    UCHAR  k;    

    for (i = 0; i < usNum; i++)
    {
		if ((usN + i) <= SOE_MAX_NUM)
		{
			pos = g_FlagDB.queue_soe.in >= (usN + i) ? g_FlagDB.queue_soe.in - (usN + i) : g_FlagDB.queue_soe.in + SOE_MAX_NUM - (usN + i);
			
			psCoilSOEData->addr_H  = HIBYTE(g_SOEDB[pos].addr);
			psCoilSOEData->addr_L = LOBYTE(g_SOEDB[pos].addr);
			psCoilSOEData->state = g_SOEDB[pos].value;
			psCoilSOEData->time.year = g_SOEDB[pos].time.year;
			psCoilSOEData->time.month = g_SOEDB[pos].time.month;
			psCoilSOEData->time.dayofWeek = g_SOEDB[pos].time.dayofWeek;
			psCoilSOEData->time.hour = g_SOEDB[pos].time.hour;
			psCoilSOEData->time.minute = g_SOEDB[pos].time.minute;
			psCoilSOEData->time.msecondH = g_SOEDB[pos].time.msecondH;
			psCoilSOEData->time.msecondL = g_SOEDB[pos].time.msecondL;		
		}
		else
		{
            memset(psCoilSOEData, 0, sizeof(struct Event_Str)); 			
		}

        psCoilSOEData++;        
    }   
}

/**
  * @brief : read fevent
  * @param : [usN]-NO.n
  * @param : [usNum]-the num of fevent
  * @param : [pFEventData]-data pointer
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
static void eMBRegHoldingReadFEvent(USHORT usN, USHORT usNum, struct M_FaultEvent *pFEventData)
{
    UCHAR i, j;
    UCHAR pos;

    for (i = 0; i < usNum; i++)
    {
		if ((usN + i) <= FEVENT_MAX_NUM)
		{
			pos = g_FlagDB.queue_fevent.in >= (usN + i) ? g_FlagDB.queue_fevent.in - (usN + i) : g_FlagDB.queue_fevent.in + FEVENT_MAX_NUM - (usN + i);
			
			pFEventData->yxNum = g_FeventDB[pos].yx_num;
			pFEventData->yxType = g_FeventDB[pos].yx_type;
			pFEventData->addrH = HIBYTE(g_FeventDB[pos].yx[0].addr);
			pFEventData->addrL = LOBYTE(g_FeventDB[pos].yx[0].addr);
			pFEventData->state = g_FeventDB[pos].yx[0].value;
			pFEventData->time.year = g_FeventDB[pos].yx[0].time.year;
			pFEventData->time.month = g_FeventDB[pos].yx[0].time.month;
			pFEventData->time.dayofWeek = g_FeventDB[pos].yx[0].time.dayofWeek;
			pFEventData->time.hour = g_FeventDB[pos].yx[0].time.hour;
			pFEventData->time.minute = g_FeventDB[pos].yx[0].time.minute;
			pFEventData->time.msecondH = g_FeventDB[pos].yx[0].time.msecondH;
			pFEventData->time.msecondL = g_FeventDB[pos].yx[0].time.msecondL;
			pFEventData->ycNum = g_FeventDB[pos].yc_num;
			pFEventData->ycType = g_FeventDB[pos].yc_type;
			
			for (j = 0; j < 10; j++)
			{
				pFEventData->yc[j].addr_H = HIBYTE(g_FeventDB[pos].yc[j].addr);
				pFEventData->yc[j].addr_L = LOBYTE(g_FeventDB[pos].yc[j].addr);
				pFEventData->yc[j].valueHH = HIBYTE(HIWORD(FloatToBin(g_FeventDB[pos].yc[j].value)));
				pFEventData->yc[j].valueHL = LOBYTE(HIWORD(FloatToBin(g_FeventDB[pos].yc[j].value)));
				pFEventData->yc[j].valueLH = HIBYTE(LOWORD(FloatToBin(g_FeventDB[pos].yc[j].value)));
				pFEventData->yc[j].valueLL = LOBYTE(LOWORD(FloatToBin(g_FeventDB[pos].yc[j].value)));
			}		
		}
		else
		{
		    memset(pFEventData, 0, sizeof(struct M_FaultEvent)); 
		}

        pFEventData++;
    }
}

/**
  * @brief : REG slave Read Coils function.
  * @param : [eDataType]-the type of Coils
  * @param : [pucRegBuffer]-the pointer of Coils
  * @param : [usAddress]-the address of Coils
  * @param : [usNCoils]-the num of Coils
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
static void eMBRegHoldingRead(UCHAR ucDataType, UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils)
{
	USHORT  usN;
	USHORT  i;
//    static USHORT  A;
	USHORT  usStartAddr;	
//	struct TimeTelesignal CoilTimeTelesignal;
    
	switch (ucDataType)
	{
		case REG_TIME_TELESIGNAL:
			eMBRegHoldingReadTimeTelesignal();

        	if ((psCoilTimeTelesignal->allFlag & 0x0C) == REG_UPDATE_FAILED)
			{
				psCoilTimeTelesignal->allFlag &= ~0x0C; // Update clear flag
			}        
			memcpy(pucRegBuffer, psCoilTimeTelesignal, usNCoils * 2);

			break;

		case REG_TELEMETRY:
			for (i = 0; i < (usNCoils/2); i++)
			{
				pucRegBuffer[0+4*i] = HIBYTE(HIWORD(FloatToBin(pSCoilTelemetry[(usAddress - REG_TELEMETRY_START_ADDR) / 2 + i])));
				pucRegBuffer[1+4*i] = LOBYTE(HIWORD(FloatToBin(pSCoilTelemetry[(usAddress - REG_TELEMETRY_START_ADDR) / 2 + i])));
				pucRegBuffer[2+4*i] = HIBYTE(LOWORD(FloatToBin(pSCoilTelemetry[(usAddress - REG_TELEMETRY_START_ADDR) / 2 + i])));
				pucRegBuffer[3+4*i] = LOBYTE(LOWORD(FloatToBin(pSCoilTelemetry[(usAddress - REG_TELEMETRY_START_ADDR) / 2 + i])));
			}
			break;  
		case REG_INHERENT:
			memcpy(pucRegBuffer, &g_InherentPara[24 * (usAddress - REG_INHERENT_START_ADDR)], usNCoils * 2); 
			break;
		case REG_RUN_PARA:
		case REG_PUBLIC_FIXED:
		case REG_BREAKER_FIXED:
		case REG_LOAD_FIXED:
			usStartAddr = eMBRegHoldingReadFixedAndPara(ucDataType);

			for (i = 0; i < (usNCoils/2); i++)
			{
				pucRegBuffer[0+4*i] = HIBYTE(HIWORD(FloatToBin(pSCoilFixedAndPara[(usAddress - usStartAddr) / 2 + i])));
				pucRegBuffer[1+4*i] = LOBYTE(HIWORD(FloatToBin(pSCoilFixedAndPara[(usAddress - usStartAddr) / 2 + i])));
				pucRegBuffer[2+4*i] = HIBYTE(LOWORD(FloatToBin(pSCoilFixedAndPara[(usAddress - usStartAddr) / 2 + i])));
				pucRegBuffer[3+4*i] = LOBYTE(LOWORD(FloatToBin(pSCoilFixedAndPara[(usAddress - usStartAddr) / 2 + i])));
			}
			break;

		case REG_SOE:
			psCoilTimeTelesignal->allFlag &= ~REG_HAVESOE;			
			usN = 2 * (usAddress - REG_SOE_START_ADDR)/ sizeof(struct Event_Str) + 1; 
			eMBRegHoldingReadSOE(usN, usNCoils / sizeof(struct Event_Str), sCoilSOEData);
			memcpy(pucRegBuffer, &sCoilSOEData, usNCoils);
			break;
		
		case REG_OPERATE:
			usN = 2 * (usAddress - REG_OPERATE_START_ADDR)/ sizeof(struct Event_Str) + 1;
//			eMBRegHoldingReadOperate(ucN, usNCoils / sizeof(struct Event_Str),       );
//			memcpy(pucRegBuffer, &OperateData, usNCoils);
	        break;
		case REG_EVENT:
			psCoilTimeTelesignal->allFlag &= ~REG_HAVEFEVENT;
			usN = 2 * (usAddress - REG_EVENT_START_ADDR)/ sizeof(struct M_FaultEvent) + 1;
			eMBRegHoldingReadFEvent(usN, usNCoils / sizeof(struct M_FaultEvent), sCoilFaultEventData);
			memcpy(pucRegBuffer, &sCoilFaultEventData, usNCoils);
			break;
		default :
			break;							
	}    
}

/**
  * @brief : operate handle
  * @param : [eDataType]-the type of operate
  * @param : [usAddress]-the address of operate
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
static void eMBRegCoilsWrite(USHORT usAddress)
{  
    switch (usAddress)
    {
		case REMOTE_OPEN:
			if (g_TelesignalDB.Str.remoteEarth == ON)
			{
				return;
			}
			rt_hw_do_operate(DO_OPEN, LOCAL);
			break;
		case REMOTE_CLOSE:
			if (g_TelesignalDB.Str.remoteEarth == ON)
			{
				return;
			}
			rt_hw_do_operate(DO_CLOSE, LOCAL);
			break;
		case REMOTE_REVERT:   // ??
			DBRevert(LOCAL);
			break;
		case PROTECT_PLATENT_EXIT:
			DBWriteSOE(ADDR_FUNCTION_HARDSTRAP, OFF);
			break;
		case PROTECT_PLATENT_INPUT:
			DBWriteSOE(ADDR_FUNCTION_HARDSTRAP, ON);
			break;
		case RECLOSE_PLATENT_EXIT:
			DBWriteSOE(ADDR_RECLOSEHARDSTRAP, OFF);
			break;
		case RECLOSE_PLATENT_INPUT:
			DBWriteSOE(ADDR_RECLOSEHARDSTRAP, ON);
			break;
		case DISTANT:
			if (g_TelesignalDB.Str.remoteEarth == ON)
			{
				return;
			}
			DBWriteSOE(ADDR_REMOTE_EARTH, ON);
			break;
		case LOCALLY:
			if (g_TelesignalDB.Str.remoteEarth == OFF)
			{
				return;
			}
			DBWriteSOE(ADDR_REMOTE_EARTH, OFF);
			break;
		case BREAK:
			if (g_TelesignalDB.Str.breakContact == OFF || g_Parameter[CONNECT_SWITCH_RECOGNIZE] == 1)
			{
				return;
			}
			DBWriteSOE(ADDR_BREAK_CONTACT, OFF);
			break;
		case CONNECT:
			if (g_TelesignalDB.Str.breakContact == ON || g_Parameter[CONNECT_SWITCH_RECOGNIZE] == 1)
			{
				return;
			}
			DBWriteSOE(ADDR_BREAK_CONTACT, ON);
			break;
		case CLAER_HISTORY:
            DBClear();
			DBWriteSOE(LOCAL_CLAER_HISTORY_ADDR, ON);
        break;    
    default:
        break;
    }	
}

/**
  * @brief : Modbus slave write Holding register function.
  * @param : [pucRegBuffer]-the pointer of Holding register
  * @param : [usAddress]-the address of Holding register
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
static void eMBRegHoldingWrite(UCHAR * pucRegBuffer, USHORT usAddress)
{
    LONG   temp;
    UCHAR  ucSn;
    float  value;
    float  *pValueTemp;   
	
    /* ÎÂ¶È */
    if (usAddress == REG_TEMP_START_ADDR)
    {
        temp = (int32_t)MAKEWORD(pucRegBuffer[3], pucRegBuffer[2]) << 16 | MAKEWORD(pucRegBuffer[1], pucRegBuffer[0]);
        g_TelemetryDB.Str.temprature = BinToFloat(temp);

        if (g_CalibrateFactor[CALIFACTOR_T] != 0)
		{
			g_TelemetryDB.Str.temprature *= g_CalibrateFactor[CALIFACTOR_T];
		}
    }
    else if (REG_RUNPARAMETER(usAddress) || REG_PUBLIC_FIXED(usAddress) || REG_BREAK_FIXED(usAddress) || REG_LOAD_FIXED(usAddress))
    {
        value = BinToFloat(MAKEWORD(pucRegBuffer[3], pucRegBuffer[2]) << 16 | MAKEWORD(pucRegBuffer[1], pucRegBuffer[0]));
        
        if (REG_RUNPARAMETER(usAddress))
        {
            ucSn = 0;
            usAddress = (usAddress - RUNPARAMETER_START_ADDR) / 2 + RUNPARAMETER_START_ADDR;
        } 
        else if (REG_PUBLIC_FIXED(usAddress))
        {
            ucSn = g_ValueParaOperateInfo.currentSN;
            usAddress = (usAddress - PUBLIC_VALUE_START_ADDR) / 2 + PUBLIC_VALUE_START_ADDR;
        }
        else if (REG_BREAK_FIXED(usAddress))
        { 
            ucSn = g_ValueParaOperateInfo.currentSN;
            usAddress = (usAddress - BREAKER_VALUE_START_ADDR) / 2 + BREAKER_VALUE_START_ADDR;
        }
        else if (REG_LOAD_FIXED(usAddress))
        {
            ucSn = g_ValueParaOperateInfo.currentSN;
            usAddress = (usAddress - LOAD_SWITCH_VALUE_START_ADDR) / 2 + LOAD_SWITCH_VALUE_START_ADDR;
        }            
            
        pValueTemp = GetValueArray(usAddress, ucSn);
        memcpy(pValueTemp, &value, sizeof(float)); //
        /* ¹Ì»¯FRAM */
        ParameterCheck(ZERODRIFT);    
        ParameterCheck(DEADZONE); 
        ParameterCheck(CALIBRATE_FACTOR);
        
        rt_multi_common_data_save_value_to_fram(ucSn);
    }    
}

/**
 * Modbus slave input register callback function.
 *
 * @param pucRegBuffer input register buffer
 * @param usAddress input register address
 * @param usNRegs input register number
 *
 * @return result
 */
eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegInputBuf;
    USHORT          REG_INPUT_START;
    USHORT          REG_INPUT_NREGS;
    USHORT          usRegInStart;

    pusRegInputBuf = usSRegInBuf;
    REG_INPUT_START = S_REG_INPUT_START;
    REG_INPUT_NREGS = S_REG_INPUT_NREGS;
    usRegInStart = usSRegInStart;

    /* it already plus one in modbus function method. */
    usAddress--;
	
    if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = usAddress - usRegInStart;
		
        while (usNRegs > 0)
        {
            *pucRegBuffer++ = (UCHAR) (pusRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ = (UCHAR) (pusRegInputBuf[iRegIndex] & 0xFF);
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/**
 * Modbus slave holding register callback function.
 *
 * @param pucRegBuffer holding register buffer
 * @param usAddress holding register address
 * @param usNRegs holding register number
 * @param eMode read or write
 *
 * @return result
 */
eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          REG_HOLDING_START;
	USHORT          REG_HOLDING_END;
	UCHAR           ucRegType;		

    REG_HOLDING_START = S_REG_HOLDING_START;
	REG_HOLDING_END   = S_REG_HOLDING_END;

    /* it already plus one in modbus function method. */
    usAddress--;

    ucRegType = eMBRegHoldingJudgeAddr(usAddress);	
	
    if ((usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_END))
    {
        //iRegIndex = usAddress - usRegHoldStart;
        switch (eMode)
        {
        /* read current register values from the protocol stack. */
        case MB_REG_READ:
       		eMBRegHoldingRead(ucRegType, pucRegBuffer, usAddress, usNRegs);		    	
            break;

        /* write current register values with new values from the protocol stack. */
        case MB_REG_WRITE:		
			eMBRegHoldingWrite(pucRegBuffer, usAddress);
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
 * Modbus slave coils callback function.
 *
 * @param pucRegBuffer coils buffer
 * @param usAddress coils address
 * @param usNCoils coils number
 * @param eMode read or write
 *
 * @return result
 */
eMBErrorCode eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex , iRegBitIndex , iNReg;
    UCHAR *         pucCoilBuf;
    USHORT          COIL_START;
	USHORT          COIL_END;
    USHORT          usCoilStart;

    COIL_START = S_COIL_START;
	COIL_END   = S_COIL_END;
    usCoilStart = usSCoilStart;

    /* it already plus one in modbus function method. */
    usAddress--;
    
	if( ( usAddress >= COIL_START ) && ( usAddress + usNCoils <= COIL_END ) )
    {
        iRegIndex = (USHORT) (usAddress - usCoilStart) / 8;
        iRegBitIndex = (USHORT) (usAddress - usCoilStart) % 8;
        switch ( eMode )
        {
        /* read current coil values from the protocol stack. */
        case MB_REG_READ:
            while (iNReg > 0)
            {
                *pucRegBuffer++ = xMBUtilGetBits(&pucCoilBuf[iRegIndex++],
                        iRegBitIndex, 8);
                iNReg--;
            }			
            pucRegBuffer--;
            /* last coils */
            usNCoils = usNCoils % 8;
            /* filling zero to high bit */
            *pucRegBuffer = *pucRegBuffer << (8 - usNCoils);
            *pucRegBuffer = *pucRegBuffer >> (8 - usNCoils);
            break;

            /* write current coil values with new values from the protocol stack. */
        case MB_REG_WRITE:
		    eMBRegCoilsWrite(usAddress);
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
 * Modbus slave discrete callback function.
 *
 * @param pucRegBuffer discrete buffer
 * @param usAddress discrete address
 * @param usNDiscrete discrete number
 *
 * @return result
 */
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex , iRegBitIndex , iNReg;
    UCHAR *         pucDiscreteInputBuf;
    USHORT          DISCRETE_INPUT_START;
    USHORT          DISCRETE_INPUT_NDISCRETES;
    USHORT          usDiscreteInputStart;
    iNReg =  usNDiscrete / 8 + 1;

    pucDiscreteInputBuf = ucSDiscInBuf;
    DISCRETE_INPUT_START = S_DISCRETE_INPUT_START;
    DISCRETE_INPUT_NDISCRETES = S_DISCRETE_INPUT_NDISCRETES;
    usDiscreteInputStart = usSDiscInStart;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= DISCRETE_INPUT_START)
            && (usAddress + usNDiscrete    <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES))
    {
        iRegIndex = (USHORT) (usAddress - usDiscreteInputStart) / 8;
        iRegBitIndex = (USHORT) (usAddress - usDiscreteInputStart) % 8;

        while (iNReg > 0)
        {
            *pucRegBuffer++ = xMBUtilGetBits(&pucDiscreteInputBuf[iRegIndex++],
                    iRegBitIndex, 8);
            iNReg--;
        }
        pucRegBuffer--;
        /* last discrete */
        usNDiscrete = usNDiscrete % 8;
        /* filling zero to high bit */
        *pucRegBuffer = *pucRegBuffer << (8 - usNDiscrete);
        *pucRegBuffer = *pucRegBuffer >> (8 - usNDiscrete);
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/**
  * @brief : Modbus slave app init function.
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-9][Sunxr][newly increased]
  */
void eUserMBAppInit(void)
{
    device_fram = rt_device_find(RT_SPI_FRAM_NAME);
    if (device_fram == NULL)
    {
        MB_PRINTF("HMI fram is not found! \r\n"); 		
    }
    
    psCoilTimeTelesignal = (struct TimeTelesignal *)rt_malloc(sizeof(struct TimeTelesignal));    
    pSCoilTelemetry = g_TelemetryDB.buf;
	
    memset(psCoilTimeTelesignal, 0, sizeof(struct TimeTelesignal));

    eMBRegGetRecordNum();    
}

/* END OF FILE ---------------------------------------------------------------*/

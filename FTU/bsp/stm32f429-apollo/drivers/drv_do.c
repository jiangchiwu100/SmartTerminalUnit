/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_do.c
  * @brief:     The driver of do.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017.09.05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  */
	
/* INCLUDES FILSE ------------------------------------------------------------*/
#include "drv_do.h"
#include "drv_gpio.h"
#include "common_data.h"
#include <arm_math.h>

/* PRIVATE VARIABLES ---------------------------------------------------------*/
static DoSrtuct DoStr;
static DoFlagBit DoFlag;
static rt_device_t rt_do_dev;


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : do GPIO initialize
  * @param : [none].
  * @return: [RT_EOK] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */  
static int rt_hw_do_init(void)
{
    rt_do_dev = rt_device_find(RT_PIN_NAME);	
    
    if (rt_do_dev == RT_NULL)
    {
        DO_PRINTF("do pin device is not found !!!"); 
    }
    else
    {
        DO_PRINTF("do pin device is found ~~~"); 
    }
		
    return (0);
}
INIT_DEVICE_EXPORT(rt_hw_do_init);

/**
  * @brief : switch close operate
  * @param : [none].
  * @return: [none] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */  
static rt_uint8_t rt_hw_close_operate(void)
{
    if (g_TelesignalDB.Str.switchOpen == ON && g_TelesignalDB.Str.switchClose == OFF && !CLOSING && !OPENING && g_TelesignalDB.Str.deviceFault == OFF)
    {
		if (g_pFixedValue->Data.publicValue.Str.closingLoopSwitch && g_TelemetryDB.Str.Uab >= g_pFixedValue->Data.loadSwitchValue.Str.voltageValue
			&& g_TelemetryDB.Str.UBC >= g_pFixedValue->Data.loadSwitchValue.Str.voltageValue)
		{
	        if (fabs(g_TelemetryDB.Str.AlphaU) > g_pFixedValue->Data.publicValue.Str.phaseAngleDifference 
				|| fabs(g_TelemetryDB.Str.Uab - g_TelemetryDB.Str.UBC) > g_pFixedValue->Data.publicValue.Str.voltageDifference) 
			{
				switch (DoStr.actSource)
				{
                    case REMOTE:
                        DBWriteSOE(LOCAL_REMOTE_ADDR, CLOSE_LOOP_EXECUTE_FAIL);
                        DoStr.actSource = 0;
                        break;
                    case LOCAL:
                        DBWriteSOE(LOCAL_OPERATION_ADDR, CLOSE_LOOP_EXECUTE_FAIL);
                        DoStr.actSource = 0;
                        break;
                    case DISTANCE:
                        DBWriteSOE(DISTANT_REMOTE_ADDR, CLOSE_LOOP_EXECUTE_FAIL);
                        DoStr.actSource = 0;
                        break;
                    case LOGIC_ACT:
                        DoStr.actSource = 0;
                        break;
                    default:
                        break;
                }

			    return FALSE;
			}
		}
      #ifdef LOGICLOCKINGMANUALREMOTECONTROL
		else
		{
			if (g_TelesignalDB.Str.closingLocked == ON)
			{
			    return FALSE;
			}			    
		}         
      #endif  		
		
        /* Permanent magnet mechanism */
        if (g_ParameterDB.Data.runPara.Str.operatingMechanism == MAGNET && g_TelesignalDB.Str.operatingMechanism == OFF)
        {
            return FALSE;
        }

        if (g_ParameterDB.Data.runPara.Str.outputOverturn == 0)
        { 
            pin_status[INDEX_SWITCH_OPEN_DO].status = DO_CLR;	
            pin_status[INDEX_SWITCH_CLOSE_DO].status = DO_SET;			
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));				
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));			
        }
        else
        {
            pin_status[INDEX_SWITCH_CLOSE_DO].status = DO_CLR;	
            pin_status[INDEX_SWITCH_OPEN_DO].status = DO_SET;			
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));				
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));					
        }            

        CLOSING = 1;
        DoStr.closingDelay = g_ParameterDB.Data.runPara.Str.closingPulseTime <= 10 ? PULSE_TIME : g_ParameterDB.Data.runPara.Str.closingPulseTime;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
  * @brief : switch close recovery
  * @param : [none].
  * @return: [rlt]:0xFF-Not return status; CLOSE_EXECUTE_SUCCESS;CLOSE_EXECUTE_FAIL
  * @updata: [2017-12-21][Sunxr][newly increased]
  */ 
static rt_uint8_t rt_hw_close_recovery(void)
{
    rt_uint8_t rlt = 0xff;
    static rt_uint32_t s_fault_counter;

    if (CLOSING)
    {
        pin_status[INDEX_SWITCH_OPEN_DO].status = DO_CLR;
        pin_status[INDEX_SWITCH_CLOSE_DO].status = DO_CLR;	
		
        if (g_TelesignalDB.Str.switchClose == ON) 
        {			
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));	
			
            CLOSING = 0;
            s_fault_counter = 0;
            DoStr.closingDelay = 0;
			
			if (g_pFixedValue->Data.publicValue.Str.closingLoopSwitch)
			{
			    rlt = CLOSE_LOOP_EXECUTE_SUCCESS;
			}
			else
			{
			    rlt = CLOSE_EXECUTE_SUCCESS; 
			}          
        }
        else if (DoStr.closingDelay <= 0)
        {		
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));	
        }
        else
        {
            DoStr.closingDelay--;
        }

        if (++s_fault_counter > DEVICE_FAULT_TIME)
        {
            CLOSING = 0;
            s_fault_counter = 0;
            
		    if (g_pFixedValue->Data.publicValue.Str.closingLoopSwitch && g_TelemetryDB.Str.Uab >= g_pFixedValue->Data.loadSwitchValue.Str.voltageValue
			    && g_TelemetryDB.Str.UBC >= g_pFixedValue->Data.loadSwitchValue.Str.voltageValue)
			{
			    rlt = CLOSE_LOOP_EXECUTE_FAIL;
			}
			else
			{
			    rlt = CLOSE_EXECUTE_FAIL;
			}	
			
            DBWriteSOE(ADDR_DEVICE_FAULT, ON);
        }
    }
    return rlt;
}

/**
  * @brief : switch open operate
  * @param : [none].
  * @return: [none].
  * @updata: [2017-12-21][Sunxr][newly increased]
  */ 
static rt_uint8_t rt_hw_open_operate(void)
{
  #ifdef LOGICLOCKINGMANUALREMOTECONTROL
    if (g_TelesignalDB.Str.switchClose == ON && g_TelesignalDB.Str.switchOpen == OFF && !CLOSING && !OPENING
        && g_TelesignalDB.Str.openingLockedEvent == OFF && g_TelesignalDB.Str.deviceFault == OFF) 				
  #else
    if (g_TelesignalDB.Str.switchClose == ON && g_TelesignalDB.Str.switchOpen == OFF && !CLOSING && !OPENING 
		&& g_TelesignalDB.Str.deviceFault == OFF)            
  #endif 	
    {
        /* Permanent magnet mechanism */
        if (g_ParameterDB.Data.runPara.Str.operatingMechanism == MAGNET && g_TelesignalDB.Str.operatingMechanism == OFF)
        {
            return FALSE;
        }

        if (g_ParameterDB.Data.runPara.Str.outputOverturn == 0)
        {
            pin_status[INDEX_SWITCH_CLOSE_DO].status = DO_CLR;	
            pin_status[INDEX_SWITCH_OPEN_DO].status = DO_SET;			
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));				
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));
        }
        else
        {
            pin_status[INDEX_SWITCH_OPEN_DO].status = DO_CLR;	
            pin_status[INDEX_SWITCH_CLOSE_DO].status = DO_SET;			
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));				
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));			
        } 
        
        OPENBUSY = 1;
        OPENING = 1;
        DoStr.openingDelay = g_ParameterDB.Data.runPara.Str.openingPulseTime <= 10 ? PULSE_TIME : g_ParameterDB.Data.runPara.Str.openingPulseTime;
        RELAY_OPEN = 1;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
  * @brief : switch open recovery
  * @param : [none].
  * @return: [rlt]:0xFF-Not return status; OPEN_EXECUTE_SUCCESS; OPEN_EXECUTE_FAIL
  * @updata: [2017-12-21][Sunxr][newly increased]
  */ 
static rt_uint8_t rt_hw_open_recovery(void)
{
    rt_uint8_t rlt = 0xFF;
    static uint32_t faultCounter;

    if (OPENING)
    {
        pin_status[INDEX_SWITCH_OPEN_DO].status = DO_CLR;
        pin_status[INDEX_SWITCH_CLOSE_DO].status = DO_CLR;	
		
        if (g_TelesignalDB.Str.switchOpen == ON) 
        {
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));	
			
            OPENING = 0;
            faultCounter = 0;
            DoStr.openingDelay = 0;
            rlt = OPEN_EXECUTE_SUCCESS;
        }
        else if (DoStr.openingDelay <= 0)
        {
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));	
        }
        else
        {
            DoStr.openingDelay--;
        }

        if (++faultCounter > DEVICE_FAULT_TIME)
        {
            OPENING = 0;
            faultCounter = 0;
            rlt = OPEN_EXECUTE_FAIL;
            DBWriteSOE(ADDR_DEVICE_FAULT, ON);
        }
    }
    return rlt;
}

/**
  * @brief : coil energy storage
  * @param : [none].
  * @return: [none].
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static void rt_hw_coil_energy_storage(void)
{
    static uint16_t s_counter;

    if (g_TelesignalDB.Str.deviceFault == OFF && g_TelesignalDB.Str.operatingMechanism == OFF)// && s_counter < ENERGY_STORAGE_TIME) // 开始储能
    {
        if (ENERGY_STORAGEING == 0)
        {
            pin_status[INDEX_ENERGY_STORAGE_DO].status = DO_SET;			
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ENERGY_STORAGE_DO], sizeof(struct rt_device_pin_status));			
            ENERGY_STORAGEING = 1;        
        }
        //s_counter++;		
    }
//    else if (g_TelesignalDB.Str.operatingMechanism == OFF && ENERGY_STORAGEING)// && (s_counter >= ENERGY_STORAGE_TIME))
//    {
//        pin_status[INDEX_ENERGY_STORAGE_DO].status = DO_CLR;			
//        rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ENERGY_STORAGE_DO], sizeof(struct rt_device_pin_status));	
//        s_counter = 0;
//        ENERGY_STORAGEING = 0;
//        //DBWriteSOE(ADDR_DEVICE_FAULT, ON);
//    }
    else if(g_TelesignalDB.Str.operatingMechanism == ON && ENERGY_STORAGEING == 1) // energy storage is completed
    {
        s_counter = 0;
        pin_status[INDEX_ENERGY_STORAGE_DO].status = DO_CLR;			
        rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ENERGY_STORAGE_DO], sizeof(struct rt_device_pin_status));	
        ENERGY_STORAGEING = 0;
    }
}

/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : do operate
  * @param : [operateType]-DO_CLOSE/DO_CLOSE_RECOVERY/DO_OPEN/DO_OPEN_RECOVERY/DO_COIL_ENERGY_STORAGE/DO_ALARM_LED/DO_BATTERY_ACTIVE/
                           DO_BATTERY_ACTIVE_RECOVERY/DO_BATTERY_ACTIVE_END/DO_BATTERY_ACTIVE_END_RECOVERY/DO_BATTERY_DROP_OUT/DO_BATTERY_DROP_OUT_RECOVERY
  * @param : [act]-REMOTE/LOCAL/ON/OFF.
  * @return: [none].
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
rt_uint8_t rt_hw_do_operate(rt_uint8_t operate_type, rt_uint8_t act)
{
    rt_uint8_t rtl = 0;

    switch (operate_type)
    {
    case DO_CLOSE:
        rtl = rt_hw_close_operate();
        DoStr.actSource = act;
        break;
		
    case DO_CLOSE_RECOVERY:
        rtl = rt_hw_close_recovery();
        if (rtl != 0xff)
        {
            switch (DoStr.actSource)
            {
            case REMOTE:
                DBWriteSOE(LOCAL_REMOTE_ADDR, rtl);
                DoStr.actSource = 0;
                break;
            case LOCAL:
                DBWriteSOE(LOCAL_OPERATION_ADDR, rtl);
                DoStr.actSource = 0;
                break;
            case DISTANCE:
                DBWriteSOE(DISTANT_REMOTE_ADDR, rtl);
                DoStr.actSource = 0;
                break;
            case LOGIC_ACT:
                DoStr.actSource = 0;
                break;
            default:
                break;
            }
        }
        break;
				
    case DO_OPEN:
        rtl = rt_hw_open_operate();
        DoStr.actSource = act;
        break;
		
    case DO_OPEN_RECOVERY:
        rtl = rt_hw_open_recovery();

        if (rtl != 0xff)
        {
            switch (DoStr.actSource)
            {
            case REMOTE:
                DBWriteSOE(LOCAL_REMOTE_ADDR, rtl);
                DoStr.actSource = 0;
                break;
            case LOCAL:
                DBWriteSOE(LOCAL_OPERATION_ADDR, rtl);
                DoStr.actSource = 0;
                break;
            case DISTANCE:
                DBWriteSOE(DISTANT_REMOTE_ADDR, rtl);
                DoStr.actSource = 0;
                break;
            case LOGIC_ACT:
                DoStr.actSource = 0;
                break;
            default:
                break;
            }
        }
        break;
				
    case DO_COIL_ENERGY_STORAGE:
        /* Permanent magnet mechanism */
        if (g_ParameterDB.Data.runPara.Str.operatingMechanism != MAGNET)
        {
            rt_hw_coil_energy_storage();
        }                
        break;
				
    case DO_BATTERY_ACTIVE:
        pin_status[INDEX_ACTIVATE_START_DO].status = DO_SET;			
        rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_START_DO], sizeof(struct rt_device_pin_status));	
        switch (act)
        {
        case DISTANCE:
            DBWriteSOE(DISTANT_ACTIVE_ADDR, ON);
            break;
//  			        case LOCAL:
//                    soe.addr = LOCAL_ACTIVE_ADDR;
//                    DBWriteSOE(&soe, 1);
//				            break;
        default:
            break;
        }
        break;
				
    case DO_BATTERY_ACTIVE_RECOVERY:
        pin_status[INDEX_ACTIVATE_START_DO].status = DO_CLR;			
        rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_START_DO], sizeof(struct rt_device_pin_status));		
        break;
		
    case DO_BATTERY_ACTIVE_END:
        pin_status[INDEX_ACTIVATE_STOP_DO].status = DO_SET;			
        rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_STOP_DO], sizeof(struct rt_device_pin_status));	    
        switch (act)
        {
        case DISTANCE:
            DBWriteSOE(DISTANT_ACTIVE_ADDR, OFF);
            break;
//  			        case LOCAL:
//                    soe.addr = LOCAL_ACTIVE_ADDR;
//                    DBWriteSOE(&soe, 1);
//				            break;
        default:
            break;
        }    
        break;
        
    case DO_BATTERY_ACTIVE_END_RECOVERY:
        pin_status[INDEX_ACTIVATE_STOP_DO].status = DO_CLR;			
        rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_STOP_DO], sizeof(struct rt_device_pin_status));	
        break;    
		
    default:
        break;
    }

    return rtl;
}

/**
  * @brief : battery_activation
  * @param : [clock]-call cycle.
  * @return: [none].
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
void rt_hw_battery_activation(rt_uint8_t clock)
{
    static rt_uint32_t s_cycle_counter;
    static rt_uint32_t s_act_counter;
    static rt_uint32_t s_fault_counter;
    static rt_uint32_t s_recover_counter;
    
    if (g_TelesignalDB.Str.batteryActivationStatus == OFF)
    {
        if (++s_recover_counter > 400)
        {
            s_recover_counter = 0;
            rt_hw_do_operate(DO_BATTERY_ACTIVE_END_RECOVERY, 0);
        }         
        
        if (g_pFixedValue->Data.publicValue.Str.batteryActiveSwitch)
        {                       
            if (s_cycle_counter >= g_pFixedValue->Data.publicValue.Str.batteryActiveCycle * 24 * 3600 * 1000 / clock) // 换算为小时
            {
                s_cycle_counter = 0;
                rt_hw_do_operate(DO_BATTERY_ACTIVE, 0);
            }
            else
            {
                s_cycle_counter++;
            }                
        }
    }
    else
    {
        s_cycle_counter = 0;
        s_recover_counter = 0;        
    }

    if (g_TelesignalDB.Str.batteryActivationStatus == ON)
    {
        if (s_fault_counter <= g_pFixedValue->Data.publicValue.Str.batteryActiveFaultTime + 1)
        {
            s_fault_counter++;
        }
        
        if (s_act_counter > 500)
        {
            rt_hw_do_operate(DO_BATTERY_ACTIVE_RECOVERY, 0);
        }

        if ((g_TelemetryDB.Str.Uab < g_pFixedValue->Data.publicValue.Str.downlimitVoltageU
            && g_TelemetryDB.Str.UBC < g_pFixedValue->Data.publicValue.Str.downlimitVoltageU))
        {
            /* AC disappeared, stop activation */
            rt_hw_do_operate(DO_BATTERY_ACTIVE_END, 0);                
        }
            
        if (g_pFixedValue->Data.publicValue.Str.batteryActiveSwitch && 
            s_act_counter >= g_pFixedValue->Data.publicValue.Str.batteryActiveTime * 60 * 1000)
        {
            s_act_counter = 0;

            /* stop activation */
            rt_hw_do_operate(DO_BATTERY_ACTIVE_END, 0);
        }
        else
        {
            s_act_counter++;
            
            /* Battery voltage during activation is lower than the fault voltage and reported SOE*/           
            if (s_fault_counter <= g_pFixedValue->Data.publicValue.Str.batteryActiveFaultTime)
            {                
                if (g_TelemetryDB.Str.DCVoltage < g_pFixedValue->Data.publicValue.Str.batteryActiveFaultVoltage)
                {
                    s_fault_counter = 0;
                    /* stop activation */
                    rt_hw_do_operate(DO_BATTERY_ACTIVE_END, 0);
                    DBWriteSOE(ADDR_BATTERY_FAULT_ALARM, ON); // battery fault
                }
            }           
        }
    }
    else
    {
        s_act_counter = 0;
        s_fault_counter = 0;
    }
}


/* END OF FILE ---------------------------------------------------------------*/

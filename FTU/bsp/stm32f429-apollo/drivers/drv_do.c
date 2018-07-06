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
    float uab,uCB;
    
    uab = g_TelemetryDB[g_TelemetryAddr.Uab];
    
    if(g_Parameter[CFG_PRO_VOL_N] == 0)
    {uCB = g_TelemetryDB[g_TelemetryAddr.UAB];}
    else
    {uCB = g_TelemetryDB[g_TelemetryAddr.UCB];}
        
  #ifdef LOGICLOCKINGMANUALREMOTECONTROL
    if ((g_TelesignalDB[g_TelesignalAddr.closingLocked] == OFF)&& g_TelesignalDB[g_TelesignalAddr.switchOpen] == ON && g_TelesignalDB[g_TelesignalAddr.switchClose] == OFF && 
        !CLOSING && !OPENING && g_TelesignalDB[g_TelesignalAddr.deviceFault] == OFF )//合环操作判断合闸闭锁
  #else
    if (g_TelesignalDB[g_TelesignalAddr.switchOpen] == ON && g_TelesignalDB[g_TelesignalAddr.switchClose] == OFF && !CLOSING && !OPENING && g_TelesignalDB[g_TelesignalAddr.deviceFault] == OFF)         
  #endif 	    
    {
		if (g_pFixedValue[CLOSING_LOOP_SWITCH])
		{
            // 合环条件不满足
	        if (uab < g_pFixedValue[VOLTAGE_VALUE] || uCB < g_pFixedValue[VOLTAGE_VALUE] || 
                ((fabs(g_TelemetryDB[g_TelemetryAddr.alphy_Ux_Ux]) > g_pFixedValue[PHASEANGLE_DIFFERENCE]) && (g_Parameter[CFG_PRO_VOL_N] == 0))||\
                ((fabs(g_TelemetryDB[g_TelemetryAddr.alphy_Ux_Ux] + 60) > g_pFixedValue[PHASEANGLE_DIFFERENCE])&& (g_Parameter[CFG_PRO_VOL_N] == 1))||\
                (fabs(uab - uCB) > g_pFixedValue[VOLTAGE_DIFFERENCE]) || (fabs(g_TelemetryDB[g_TelemetryAddr.F] - g_TelemetryDB[g_TelemetryAddr.F]) > g_pFixedValue[FREQUENCY_DIFFERENCE])) 
			{
				switch (DoStr.actSource)
				{
                    case ADDR_HANDHELD_OPER:
                        DBWriteCO(ADDR_HANDHELD_OPER, CLOSE_LOOP_EXECUTE_FAIL);
                        DoStr.actSource = 0;
                        break;
                    case ADDR_LOCAL_OPERATE:
                        DBWriteCO(ADDR_LOCAL_OPERATE, CLOSE_LOOP_EXECUTE_FAIL);
                        DoStr.actSource = 0;
                        break;
                    case ADDR_REMOTE_OPERATE:
                        DBWriteCO(ADDR_REMOTE_OPERATE, CLOSE_LOOP_EXECUTE_FAIL);
                        DoStr.actSource = 0;
                        break;
                    case ADDR_LOGIC_ACT:
                        DoStr.actSource = 0;
                        break;
                    default:
                        break;
                }

			    return FALSE;
			}
            else
            {
                 /* Permanent magnet mechanism */
                if (g_Parameter[OPERATING_MECHANISM] == MAGNET && g_TelesignalDB[g_TelesignalAddr.operatingMechanism] == OFF)
                {
                    return FALSE;
                }

                if (g_Parameter[OUTPUT_OVERTURN] == 0)
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
                DoStr.closingDelay = g_Parameter[CLOSING_PULSE_TIME] <= 10 ? PULSE_TIME : g_Parameter[CLOSING_PULSE_TIME];
                return TRUE;              
            }
		}
        else
        {
            /* Permanent magnet mechanism */
            if (g_Parameter[OPERATING_MECHANISM] == MAGNET && g_TelesignalDB[g_TelesignalAddr.operatingMechanism] == OFF)
            {
                return FALSE;
            }

            if (g_Parameter[OUTPUT_OVERTURN] == 0)
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
            DoStr.closingDelay = g_Parameter[CLOSING_PULSE_TIME] <= 10 ? PULSE_TIME : g_Parameter[CLOSING_PULSE_TIME];
            return TRUE;        
        }            
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
		
        if (g_TelesignalDB[g_TelesignalAddr.switchClose] == ON) 
        {			
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));
            rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));	
			
            CLOSING = 0;
            s_fault_counter = 0;
            DoStr.closingDelay = 0;
			
		    if (g_pFixedValue[CLOSING_LOOP_SWITCH])
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
            
		    if (g_pFixedValue[CLOSING_LOOP_SWITCH])
			{
                rlt = CLOSE_LOOP_EXECUTE_FAIL;  
			}
			else
			{
			    rlt = CLOSE_EXECUTE_FAIL;
			}	
			
            DBWriteSOE(g_TelesignalAddr.deviceFault, ON);
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
    if (g_TelesignalDB[g_TelesignalAddr.switchClose] == ON && g_TelesignalDB[g_TelesignalAddr.switchOpen] == OFF && !CLOSING && !OPENING
        && g_TelesignalDB[g_TelesignalAddr.openingLocked] == OFF && g_TelesignalDB[g_TelesignalAddr.deviceFault] == OFF) 				
  #else
    if (g_TelesignalDB[g_TelesignalAddr.switchClose] == ON && g_TelesignalDB[g_TelesignalAddr.switchOpen] == OFF && !CLOSING && !OPENING 
		&& g_TelesignalDB[g_TelesignalAddr.deviceFault] == OFF)            
  #endif 	
    {
        /* Permanent magnet mechanism */
        if (g_Parameter[OPERATING_MECHANISM] == MAGNET && g_TelesignalDB[g_TelesignalAddr.operatingMechanism] == OFF)
        {
            return FALSE;
        }

        if (g_Parameter[OUTPUT_OVERTURN] == 0)
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
        DoStr.openingDelay = g_Parameter[OPENING_PULSE_TIME] <= 10 ? PULSE_TIME : g_Parameter[OPENING_PULSE_TIME];
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
		
        if (g_TelesignalDB[g_TelesignalAddr.switchOpen] == ON) 
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
            DBWriteSOE(g_TelesignalAddr.deviceFault, ON);
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
//    static uint16_t s_counter;

    if (g_TelesignalDB[g_TelesignalAddr.deviceFault] == OFF && g_TelesignalDB[g_TelesignalAddr.operatingMechanism] == OFF)// && s_counter < ENERGY_STORAGE_TIME) // 开始储能
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
//        //DBWriteSOE(g_TelesignalAddr.deviceFault, ON);
//    }
    else if(g_TelesignalDB[g_TelesignalAddr.operatingMechanism] == ON && ENERGY_STORAGEING == 1) // energy storage is completed
    {
//        s_counter = 0;
        pin_status[INDEX_ENERGY_STORAGE_DO].status = DO_CLR;			
        rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ENERGY_STORAGE_DO], sizeof(struct rt_device_pin_status));	
        ENERGY_STORAGEING = 0;
    }
}

void rt_do_test(void)
{
//	pin_status[INDEX_SWITCH_OPEN_DO].status = DO_CLR;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));	
	
//	pin_status[INDEX_SWITCH_CLOSE_DO].status = DO_CLR;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));

//	rt_device_read(rt_do_dev, 0, &pin_status[INDEX_OPEN_CLOSE_DI], sizeof(struct rt_device_pin_status));

//	pin_status[INDEX_ENERGY_STORAGE_DO].status = DO_CLR;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ENERGY_STORAGE_DO], sizeof(struct rt_device_pin_status));

//	pin_status[INDEX_PRESET_DO].status = DO_CLR;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_PRESET_DO], sizeof(struct rt_device_pin_status));

//	pin_status[INDEX_ACTIVATE_START_DO].status = DO_CLR;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_START_DO], sizeof(struct rt_device_pin_status));	

//	pin_status[INDEX_ACTIVATE_STOP_DO].status = DO_CLR;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_STOP_DO], sizeof(struct rt_device_pin_status));	

//	pin_status[INDEX_BETTERY_DROP_OUT_DO].status = DO_CLR;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_BETTERY_DROP_OUT_DO], sizeof(struct rt_device_pin_status));	

//	pin_status[INDEX_BACKEUP_DO].status = DO_CLR;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_BACKEUP_DO], sizeof(struct rt_device_pin_status));	

//    rt_thread_delay(1000);  
//	pin_status[INDEX_SWITCH_OPEN_DO].status = DO_SET;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));	

//	pin_status[INDEX_SWITCH_CLOSE_DO].status = DO_SET;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));	
//    rt_device_read(rt_do_dev, 0, &pin_status[INDEX_OPEN_CLOSE_DI], sizeof(struct rt_device_pin_status));

//	pin_status[INDEX_ENERGY_STORAGE_DO].status = DO_SET;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ENERGY_STORAGE_DO], sizeof(struct rt_device_pin_status));

//	pin_status[INDEX_PRESET_DO].status = DO_SET;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_PRESET_DO], sizeof(struct rt_device_pin_status));

//	pin_status[INDEX_ACTIVATE_START_DO].status = DO_SET;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_START_DO], sizeof(struct rt_device_pin_status));

//	pin_status[INDEX_ACTIVATE_STOP_DO].status = DO_SET;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_STOP_DO], sizeof(struct rt_device_pin_status));	

//	pin_status[INDEX_BETTERY_DROP_OUT_DO].status = DO_SET;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_BETTERY_DROP_OUT_DO], sizeof(struct rt_device_pin_status));	

//	pin_status[INDEX_BACKEUP_DO].status = DO_SET;			
//	rt_device_write(rt_do_dev, 0, &pin_status[INDEX_BACKEUP_DO], sizeof(struct rt_device_pin_status));

//    rt_thread_delay(1000); 
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
rt_uint8_t rt_hw_do_operate(rt_uint16_t addr, rt_uint8_t operate_type)
{
    rt_uint8_t rtl = 0;

	switch (addr)
	{
		case ADDR_LOGIC_ACT:
            switch (operate_type)	
			{
				case DO_CLOSE: // 合闸
                    DoStr.actSource = addr;
					rtl = rt_hw_close_operate();					
					break;
					
				case DO_CLOSE_RECOVERY: // 合闸收回
					rtl = rt_hw_close_recovery();
					if (rtl != 0xff)
					{
						DBWriteCO(DoStr.actSource, rtl);
						DoStr.actSource = 0;
					}
					break;
							
				case DO_OPEN: // 分闸
					DoStr.actSource = addr;                    
					rtl = rt_hw_open_operate();
					break;
					
				case DO_OPEN_RECOVERY: // 分闸收回
					rtl = rt_hw_open_recovery();

					if (rtl != 0xff)
					{
						DBWriteCO(DoStr.actSource, rtl);
						DoStr.actSource = 0;
					}
					break;
				case DO_COIL_ENERGY_STORAGE: // 储能
					/* Permanent magnet mechanism */
					if (g_Parameter[OPERATING_MECHANISM] != MAGNET)
					{
						rt_hw_coil_energy_storage(); // 储能操作
					}
					break;					
			}				
		    break;
	    case ADDR_REMOTE_OPERATE:
		case ADDR_LOCAL_OPERATE:	
		case ADDR_HANDHELD_OPER:	
            switch (operate_type)	
			{
				case DO_CLOSE: // 合闸
                    DoStr.actSource = addr;
					rtl = rt_hw_close_operate();					
					break;
							
				case DO_OPEN: // 分闸
                    DoStr.actSource = addr;
					rtl = rt_hw_open_operate();					
					break;		
			}				
		    break;
	    case ADDR_REMOTE_ACTIVE:
            switch (operate_type)	
			{
				case DO_CLOSE: // 电池活化
					pin_status[INDEX_ACTIVATE_START_DO].status = DO_SET;			
					rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_START_DO], sizeof(struct rt_device_pin_status));	
					DBWriteCO(addr, ON);
					break;
							
				case DO_CLOSE_RECOVERY: // 活化收回
					pin_status[INDEX_ACTIVATE_START_DO].status = DO_CLR;			
					rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_START_DO], sizeof(struct rt_device_pin_status));		
					break;
					
				case DO_OPEN: // 活化结束
					pin_status[INDEX_ACTIVATE_STOP_DO].status = DO_SET;			
					rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_STOP_DO], sizeof(struct rt_device_pin_status));	    
					DBWriteCO(addr, ON);    
					break;
					
				case DO_OPEN_RECOVERY: // 活化结束收回
					pin_status[INDEX_ACTIVATE_STOP_DO].status = DO_CLR;			
					rt_device_write(rt_do_dev, 0, &pin_status[INDEX_ACTIVATE_STOP_DO], sizeof(struct rt_device_pin_status));	
					break;  		
			}						
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
    
    if (g_TelesignalDB[g_TelesignalAddr.batteryActivationStatus] == OFF)
    {
        if (++s_recover_counter > 400)
        {
            s_recover_counter = 0;
            rt_hw_do_operate(ADDR_REMOTE_ACTIVE, DO_OPEN_RECOVERY);
        }         
        
	 	if (g_pFixedValue[BATTERY_ACTIVE_SWITCH])
        {                       
            if (s_cycle_counter >= g_pFixedValue[BATTERY_ACTIVE_CYCLE] * 24 * 3600 * 1000 / clock) // 换算为小时
            {
                s_cycle_counter = 0;
                rt_hw_do_operate(ADDR_REMOTE_ACTIVE, DO_CLOSE);
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

    if (g_TelesignalDB[g_TelesignalAddr.batteryActivationStatus] == ON)
    {
        if (s_fault_counter <= g_pFixedValue[BATTERY_ACTIVE_FAULT_TIME] + 1)
        {
            s_fault_counter++;
        }
        
        if (s_act_counter > 500)
        {
            rt_hw_do_operate(ADDR_REMOTE_ACTIVE, DO_CLOSE_RECOVERY);
        }

        if ((g_TelemetryDB[g_TelemetryAddr.Uab] < g_pFixedValue[DOWNLIMIT_VOLTAGE_U] && g_TelemetryDB[g_TelemetryAddr.UCB] < g_pFixedValue[DOWNLIMIT_VOLTAGE_U]))
        {
            /* AC disappeared, stop activation */
            rt_hw_do_operate(ADDR_REMOTE_ACTIVE, DO_OPEN);                
        }
            
        if (g_pFixedValue[BATTERY_ACTIVE_SWITCH] && s_act_counter >= g_pFixedValue[BATTERY_ACTIVE_TIME] * 60 * 1000)
        {
            s_act_counter = 0;

            /* stop activation */
            rt_hw_do_operate(ADDR_REMOTE_ACTIVE, DO_OPEN);
        }
        else
        {
            s_act_counter++;
            
            /* Battery voltage during activation is lower than the fault voltage and reported SOE*/           
            if (s_fault_counter <= g_pFixedValue[BATTERY_ACTIVE_FAULT_TIME])
            {                
                if (g_TelesignalDB[g_TelesignalAddr.batteryUnderVoltageAlarm] == ON)
                {
                    s_fault_counter = 0;
                    /* stop activation */
                    rt_hw_do_operate(ADDR_REMOTE_ACTIVE, DO_OPEN);
                    DBWriteSOE(g_TelesignalAddr.batteryFaultAlarm, ON); // battery fault
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

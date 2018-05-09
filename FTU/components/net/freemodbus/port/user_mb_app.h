/**
  *            Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file    : user_mb_app.h
  * @brief   : FreeModbus user app.
  * @version : V03.001
  * @author  : SUN
  * @date    : 2018-01-09
  * @update  : [2018-01-11][Sunxr][make the code cleanup]
  */
  
#ifndef USER_APP
#define USER_APP
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"

/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START        0
#define S_DISCRETE_INPUT_NDISCRETES   16
#define S_COIL_START                  0x0001
#define S_COIL_END                    0xFBFF
#define S_COIL_NCOILS                 64
#define S_REG_INPUT_START             0
#define S_REG_INPUT_NREGS             100
#define S_REG_HOLDING_START           0x0001
#define S_REG_HOLDING_END             0xFBFF
#define S_REG_HOLDING_NREGS           100
/* salve mode: holding register's all address */
#define S_HD_RESERVE                  0
/* salve mode: input register's all address */
#define S_IN_RESERVE                  0
/* salve mode: coil's all address */
#define S_CO_RESERVE                  0
/* salve mode: discrete's all address */
#define S_DI_RESERVE                  0
/* slave address */
#define SLAVE_ADDR                    0x01 

/* -----------------------Master Defines -------------------------------------*/
#define M_DISCRETE_INPUT_START        0
#define M_DISCRETE_INPUT_NDISCRETES   16
#define M_COIL_START                  0
#define M_COIL_NCOILS                 64
#define M_REG_INPUT_START             0
#define M_REG_INPUT_NREGS             100
#define M_REG_HOLDING_START           0
#define M_REG_HOLDING_NREGS           100
/* master mode: holding register's all address */
#define M_HD_RESERVE                  0
/* master mode: input register's all address */
#define M_IN_RESERVE                  0
/* master mode: coil's all address */
#define M_CO_RESERVE                  0
/* master mode: discrete's all address */
#define M_DI_RESERVE                  0

/* DEFINE --------------------------------------------------------------------*/
/* remote command address */
#define REMOTE_OPEN                        0x6101 
#define REMOTE_CLOSE                       0x6102 
#define REMOTE_REVERT                      0x6103 
#define PROTECT_PLATENT_EXIT               0x6104 
#define PROTECT_PLATENT_INPUT              0x6105 
#define RECLOSE_PLATENT_EXIT               0x6106 
#define RECLOSE_PLATENT_INPUT              0x6107 
#define DISTANT                            0x6108 
#define LOCALLY                            0x6109 
#define BREAK                              0x610A 
#define CONNECT                            0x610B 
#define CLAER_HISTORY                      0x6201 

/* telesignal flag bit distribution */
#define REG_HAVESOE                        0x01   
#define REG_HAVEFEVENT                     0x02   
#define REG_HAVEUPDATE                     0x04   
#define REG_UPDATE_FAILED                  0x08   
#define REG_UPDATE_SUCCESS                 0x00   

/* register address ------------------------------------*/
/* time telesignal */
#define REG_TIME_TELESIGNAL_START_ADDR     0x0001   
#define REG_TIME_TELESIGNAL_END_ADDR       0x0007
#define REG_TIME_TELESIGNAL_TOTAL_NUM      (REG_TIME_TELESIGNAL_END_ADDR - REG_TIME_TELESIGNAL_START_ADDR + 1)*2

/* telemetry */
#define REG_TELEMETRY_START_ADDR           0x4001              
#define REG_TELEMETRY_END_ADDR             0x4100              
#define REG_TELEMETRY_TOTAL_NUM            (REG_TELEMETRY_END_ADDR - REG_TELEMETRY_START_ADDR + 1)*2
/* temperature */
#define REG_TEMP_START_ADDR                0x401B  

/* telecontrl */
#define REG_TELECONTRL_START_ADDR		   0x6101
#define REG_TELECONTRL_END_ADDR	           0x6205
#define REG_TELECONTRL_TOTAL_NUM           (REG_TELECONTRL_END_ADDR - REG_TELECONTRL_START_ADDR + 1) * 2

/* Inherent parameters */
#define REG_INHERENT_START_ADDR            0x8001 
#define REG_INHERENT_END_ADDR              0x8100

/* run parameters */
#define REG_RUNPARAMETER_START_ADDR        0x8101
#define REG_RUNPARAMETER_END_ADDR          0x8300
#define REG_RUNPARAMETER_TOTAL_NUM         (REG_RUNPARAMETER_END_ADDR - REG_RUNPARAMETER_START_ADDR + 1)*2

/* public value */
#define REG_PUBLIC_FIXED_START_ADDR        0x8301
#define REG_PUBLIC_FIXED_END_ADDR          0x8400
#define REG_PUBLIC_FIXED_TOTAL_NUM         (REG_PUBLIC_FIXED_END_ADDR - REG_PUBLIC_FIXED_START_ADDR + 1)*2
/* breaker value */
#define REG_BREAKER_FIXED_START_ADDR       0x8401
#define REG_BREAKER_FIXED_END_ADDR         0x8500
#define REG_BREAKER_FIXED_TOTAL_NUM        (REG_BREAKER_FIXED_END_ADDR - REG_BREAKER_FIXED_START_ADDR + 1) * 2
/* load switch value */
#define REG_LOAD_FIXED_START_ADDR          0x8501
#define REG_LOAD_FIXED_END_ADDR            0x8600
#define REG_LOAD_FIXED_TOTAL_NUM           (REG_LOAD_FIXED_END_ADDR - REG_LOAD_FIXED_START_ADDR + 1) * 2
/* SOE */
#define REG_SOE_START_ADDR                 0xF001
#define REG_SOE_END_ADDR                   0xF600
#define REG_SOE_TOTAL_NUM                  (REG_SOE_END_ADDR - REG_SOE_START_ADDR + 1) * 2
#define REG_SOE_FRAM_NUM                   5

/* operate event */
#define REG_OPERATE_START_ADDR		       0xF601
#define REG_OPERATE_END_ADDR	           0xF8FF
#define REG_OPERATE_TOTAL_NUM              (REG_OPERATE_END_ADDR - REG_OPERATE_START_ADDR + 1) * 2

/* fault event */
#define REG_EVENT_START_ADDR			   0xF901
#define REG_EVENT_END_ADDR	               0xFBFF
#define REG_EVENT_TOTAL_NUM                (REG_EVENT_END_ADDR - REG_EVENT_START_ADDR + 1) * 2

#define REG_TIME_TELESIGNAL(addr)          ((addr >= REG_TIME_TELESIGNAL_START_ADDR) && (addr <= REG_TIME_TELESIGNAL_END_ADDR))           
#define REG_TELEMETRY(addr)                ((addr >= REG_TELEMETRY_START_ADDR) && (addr <= REG_TELEMETRY_END_ADDR)) 
#define REG_TELECONTRL(addr)               ((addr >= REG_TELECONTRL_START_ADDR) && (addr <= REG_TELECONTRL_END_ADDR))
#define REG_INHERENT(addr)                 ((addr >= REG_INHERENT_START_ADDR) && (addr <= REG_INHERENT_END_ADDR))
#define REG_RUNPARAMETER(addr)             ((addr >= REG_RUNPARAMETER_START_ADDR) && (addr <= REG_RUNPARAMETER_END_ADDR))
#define REG_PUBLIC_FIXED(addr)             ((addr >= REG_PUBLIC_FIXED_START_ADDR) && (addr <= REG_PUBLIC_FIXED_END_ADDR))
#define REG_BREAK_FIXED(addr)              ((addr >= REG_BREAKER_FIXED_START_ADDR) && (addr <= REG_BREAKER_FIXED_END_ADDR))           
#define REG_LOAD_FIXED(addr)               ((addr >= REG_LOAD_FIXED_START_ADDR) && (addr <= REG_LOAD_FIXED_END_ADDR))           
#define REG_SOE(addr)                      ((addr >= REG_SOE_START_ADDR) && (addr <= REG_SOE_END_ADDR))           
#define REG_OPERATE(addr)                  ((addr >= REG_OPERATE_START_ADDR) && (addr <= REG_OPERATE_END_ADDR))           
#define REG_EVENT(addr)                    ((addr >= REG_EVENT_START_ADDR) && (addr <= REG_EVENT_END_ADDR))   

enum DATA_TYPE
{
    REG_TIME_TELESIGNAL = 0, 
    REG_TELEMETRY,
    REG_TELECONTRL,
	REG_INHERENT,
    REG_RUN_PARA,
    REG_PUBLIC_FIXED,
    REG_BREAKER_FIXED,
    REG_LOAD_FIXED,
    REG_SOE,
    REG_OPERATE,
    REG_EVENT,
    REG_TYPE_NUM
};

__packed struct CP56Time2a
{
	uint8_t msecondL;   
	uint8_t msecondH;  
	uint8_t minute;					
	uint8_t hour;						
	uint8_t dayofWeek;	// WEEK(D7-D5)day(D4-D0)
	uint8_t month;						
	uint8_t year;						
};

__packed struct TimeTelesignal
{
	struct CP56Time2a time;
	uint8_t allFlag;     // bit1-flag of have FEVENT;  bit0-flag of have SOE
	uint8_t feventNum;   // 
	uint8_t soePageNum;
    uint32_t telesignalState;	
};

struct Event_Str
{
    uint8_t addr_H;
	uint8_t addr_L;
	uint8_t state;
    struct CP56Time2a	time;  
};

struct M_YCFAULT
{	  
   	uint8_t addr_H; 
   	uint8_t addr_L; 
	uint8_t valueHH;
	uint8_t valueHL;
	uint8_t valueLH;
	uint8_t valueLL;
};

struct M_FaultEvent
{
    uint8_t yxNum; 
	uint8_t yxType; 	
	uint8_t addrH;
	uint8_t addrL;
	uint8_t state;
	struct CP56Time2a time;
	uint8_t ycNum;
	uint8_t ycType;
	struct M_YCFAULT yc[10]; 
};

extern struct TimeTelesignal *psCoilTimeTelesignal;

#endif

/* END OF FILE ---------------------------------------------------------------*/

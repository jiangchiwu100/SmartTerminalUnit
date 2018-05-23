/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      commmon_data.h
  * @brief:     The commmon data of the project.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
	
#ifndef __COMMON_DATA_H__
#define __COMMON_DATA_H__


/* INCLUDE FILES -------------------------------------------------------------*/	
#include "rtthread.h"
#include "stm32f4xx_hal.h"
#include "single_list.h"
#include "common_config.h"
#include "point_table_config.h"

/* COMMON --------------------------------------------------------------------*/
#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif /* END FALSE */

#ifndef  FULL
#define  FULL     1
#define  UNFULL      !FULL
#endif

#define ON                            0x02
#define OFF                           0x01

#define THREAD_NUM                    9

#define THREAD_RUN_SLAVE101           0x0001
#define THREAD_RUN_SLAVE104           0x0002
#define THREAD_RUN_HMI                0x0004
#define THREAD_RUN_WATCH              0x0008
#define THREAD_RUN_IDLE               0x0010
#define THREAD_RUN_DP83848            0x0020
#define THREAD_RUN_W5500              0x0040
#define THREAD_RUN_CAL                0x0080
#define THREAD_RUN_PROTECT            0x0100

#define SYS_HAVE_UPDATE               0x01   
#define SYS_UPDATE_FAILED             0x02   
#define SYS_UPDATE_SUCCESS            0x00  

#define SWITCH_ALARM  (2.0f)
#define SWITCH_ON  (1.0f)
#define SWITCH_OFF (0.0f)

#define SPRING                        0 // 弹簧机构
#define MAGNET                        1 // 永磁机构  

/* 模式枚举 */
typedef enum
{
    TYPE_BREAKER_NONE,     // 无
    TYPE_BREAKER_COMMON,   // 常规保护
    TYPE_BREAKER_VOLTIME,  // 电压时间型
    TYPE_BREAKER_VOLCUR,   // 电压电流型
    TYPE_BREAKER_CURCOUNT, // 电流计数型开关
    TYPE_BREAKER_NUM
} TYPE_BREAKER;

typedef enum
{
    TYPE_LOADSWTICH_NONE = TYPE_BREAKER_NUM,     // 无
    TYPE_LOADSWTICH_VOLTIME,  // 电压时间型
    TYPE_LOADSWTICH_VOLCUR,   // 电压电流型
    TYPE_LOADSWTICH_CURCOUNT, // 电流计数型开关
	TYPE_LOADSWTICH_DIVIDE,   // 分界负荷开关
    TYPE_LOADSWTICH_NUM
} TYPE_LOADSWTICH;

#define TYPE_BREAKER                  OFF          // 断路器
#define TYPE_LOADSWTICH               ON          // 负荷开关

#define RATIO_10k_220                      (10.0f / 0.22f)
#define RATIO_100_1                        (100.0f / 1.0f)

#define RATIO_200_5                        (200 / 5)
#define RATIO_400_5                        (400 / 5)
#define RATIO_600_5                        (600 / 5)

#define RATIO_20_1                         (20 / 1)
#define RATIO_600_1                        (600 / 1)


#define MAKEWORD(low, high)           ((uint16_t)((rt_uint8_t)(low) | (((uint16_t)((rt_uint8_t)(high))) << 8)))
#define LOBYTE(w)                     ((rt_uint8_t)(w))
#define HIBYTE(w)                     ((rt_uint8_t)((uint16_t)(w) >> 8))
#define LOWORD(l)                     ((uint16_t)(l))
#define HIWORD(l)                     ((uint16_t)((uint32_t)(l) >> 16))

#define DLT_101SLAVE_MS  10
#define DLT_104SLAVE_MS  10
#define WDG_MS           1800

/* 设备枚举 */
typedef enum
{
    FRAM_ID,
    FLASH_ID,
    SLAVE101_ID0,
    SLAVE101_ID1,
    SLAVE104_ID0,
    SLAVE104_ID1,
    DEV_MAX_NUM
} DEV_ID;


/* common数据操作类型 */
enum CommonOperateInfo
{
    DB_TELESIGNAL = 0,    // 遥信
    DB_TELEMETRY,         // 遥测
    DB_READVALUE,         // 读定值/参数
    DB_VALUE_PRESET,      // 预置
    DB_VALUE_SOLID,       // 固化
    DB_VALUE_REVOKE,	    // 撤销
    DB_SOE,               // SOE
    DB_NVA,               // 遥测
    DB_SN,                // 定值区
};

/* 变比 */
enum RatioSeclect
{
    RATIO_U,
	RATIO_U0,
	RATIO_I,
	RATIO_I0,
};

/* 系统时间结构体 */
__packed struct SD2405Time
{
    rt_uint16_t msecond;    // 毫秒
    rt_uint8_t second;     // 秒
    rt_uint8_t minute;     // 分
    rt_uint8_t hour;       // 时
    rt_uint8_t week;       // 周
    rt_uint8_t day;        // 日
    rt_uint8_t month;      // 月
    rt_uint8_t year;       // 年
};


/* CP56Time2a_t时标 */
__packed struct CP56Time2a_t
{
    rt_uint8_t msecondL;
    rt_uint8_t msecondH;
    rt_uint8_t minute;
    rt_uint8_t hour;
    rt_uint8_t dayofWeek;	// WEEK(D7-D5)day(D4-D0)
    rt_uint8_t month;
    rt_uint8_t year;
};

/* NUM ----------------------------------------------------------------------*/
#define TELESIGNAL_HMI_NUM            25     	  // 面板遥信总数
#define AVERAGE_TIMER                 50          // 系数校准取模拟量次数

#define COS_MAX_NUM                   64
#define SOE_MAX_NUM                   256
#define SOE_NEW_MAX_NUM               64
#define CO_MAX_NUM                    64
#define FEVENT_MAX_NUM                16
#define NVA_MAX_NUM                   64
#define ULOG_MAX_NUM                  32

#define TELESIGNAL_TOTAL_NUM        (sizeof(TelesignalDatabase) / sizeof(rt_uint8_t)) // 遥信总数
#define TELEMETRY_TOTAL_NUM         (sizeof(TelemetryDatabase) / sizeof(float))    // 遥测总数	
#define REMOTE_TOTAL_NUM             10    // 遥控总数	
#define INHERENT_PARAMETER_NUM      (sizeof(InherentPara) / sizeof(char) / 24)          // 终端固有参数数量
//#define RUN_PARAMETER_NUM     		(sizeof(RunParameter) / sizeof(float))         // 终端运行参数数量
///#define CALIBRATE_FACTOR_NUM        (sizeof(CalibrateFactor) / sizeof(float))      // 校准系数数量
//#define PUBLIC_VALUE_NUM            (sizeof(PublicValue) / sizeof(float))          // 公共定值数量
//#define BREAKER_VALUE_NUM           (sizeof(BreakerValue) / sizeof(float))         // 断路器定值数量
//#define LOAD_SWITCH_VALUE_NUM       (sizeof(LoadSwitchValue) / sizeof(float))      // 负荷开关定值数量
#define VALUE_PARAMETER_TOTAL_NUM   (RUN_PARAMETER_NUM+FIXED_VALUE_NUM)
//#define DB_SN0_SIZE                 (sizeof(g_Parameter))
//#define DB_SN1_SIZE                 (sizeof(PublicValue) + sizeof(BreakerValue) + sizeof(LoadSwitchValue))
//#define DB_SN2_SIZE                 (sizeof(PublicValue) + sizeof(BreakerValue) + sizeof(LoadSwitchValue))
    
/* SAMPLE ----------------------------------------------------------------------*/
#define ADC_SAMPLE_NUM                       48     // 采样半波点数
#define ADC_WAVE_SEMP_NUM                    25     // 采样全波总个数

typedef struct
{
    rt_int16_t ua[ADC_WAVE_SEMP_NUM * 2][48];
	rt_int16_t ub[ADC_WAVE_SEMP_NUM * 2][48];
	rt_int16_t uc[ADC_WAVE_SEMP_NUM * 2][48];
	rt_int16_t u0[ADC_WAVE_SEMP_NUM * 2][48];
    rt_int16_t ia[ADC_WAVE_SEMP_NUM * 2][48];
	rt_int16_t ib[ADC_WAVE_SEMP_NUM * 2][48];
	rt_int16_t ic[ADC_WAVE_SEMP_NUM * 2][48];
	rt_int16_t i0[ADC_WAVE_SEMP_NUM * 2][48];

    rt_int16_t uA[ADC_WAVE_SEMP_NUM * 2][48];
    rt_int16_t uC[ADC_WAVE_SEMP_NUM * 2][48];	
    
    rt_int16_t udc1[ADC_WAVE_SEMP_NUM * 2][48];
    rt_int16_t udc2[ADC_WAVE_SEMP_NUM * 2][48];	
	//rt_int8_t digtal[25][48];
}Queue;

/* UPDATE ----------------------------------------------------------------------*/
#define SDRAM_ADDR_UPDATE             0xC0000000         // 1M

/* LOGO ----------------------------------------------------------------------*/
#define COMMON_DATA_M_SP_NA_1              1            // 单点信息
#define COMMON_DATA_M_DP_NA_1              3            // 双点信息
#define COMMON_DATA_M_ME_NA_1              9            // 测量值，归一化值
#define COMMON_DATA_M_ME_NB_1              11           // 测量值，标度化值
#define COMMON_DATA_M_ME_NC_1              13           // 测量值，短浮点数
#define COMMON_DATA_M_SP_TB_1              30           // 带时标的单点信息
#define COMMON_DATA_M_DP_TB_1              31           // 带时标的双点信息
#define COMMON_DATA_M_FT_NA_1              42           // 故障事件信息
#define COMMON_DATA_M_IT_NB_1              206          // 累计量，短浮点数
#define COMMON_DATA_M_IT_TC_1              207          // 带时标累计量，短浮点数

/* ADDR ----------------------------------------------------------------------*/
#define TELESIGNAL_START_ADDR         0x0001      // 遥信起始地址
#define TELEMETRY_START_ADDR          0x4001      // 遥测起始地址
#if RT_USING_TELEMETRY_SET
#define TELEMETRY_SETENABLE_START_ADDR    0x4801      // 遥测置数使能起始地址
#define TELEMETRY_SETVALUE_START_ADDR     0x4881      // 遥测置数值起始地址
#endif
#define REMOTE_START_ADDR             0x6001      // 遥测起始地址
#define INTRIPARAME_START_ADDR        0x8001      // 终端固有参数起始地址
#define RUNPARAMETER_START_ADDR       0x8101      // 终端运行参数起始地址
#define CALIBRATE_FACTOR_START_ADDR   0x8201    	// 校准系数起始地址
#define CALIBRATE_VALUE_START_ADDR    0x8240    	// 校准值起始地址
#define FIXED_VALUE_START_ADDR         0x8301	    // 终端保护定值起始地址


#define DISTANT_REMOTE_ADDR           0x6001      // 远方操作
#define DISTANT_ACTIVE_ADDR           0x6002      // 活化操作
#define DISTANT_RESET_ADDR            0x6003      // 复归操作
#define DISTANT_CLAER_HISTORY_ADDR    0x6004      // 远程清除记录
#define LOCAL_OPERATION_ADDR          0x6005      // 本地操作
#define LOCAL_RESET_ADDR              0x6006      // 复归操作
#define LOCAL_CLAER_HISTORY_ADDR      0x6007      // 本地清除记录
#define LOCAL_REMOTE_ADDR             0x6008      // 本地遥控操作 (小白)


#define OPEN_SELECT_FAIL              0x01 | 0x20 // 遥控分闸选择失败  00000001
#define OPEN_SELECT_SUCCESS           0x05 | 0x20 // 遥控分闸选择成功  00000101
#define OPEN_EXECUTE_FAIL             0x09 | 0x20 // 遥控分闸执行失败  00001001						   
#define OPEN_EXECUTE_SUCCESS          0x0D | 0x20 // 遥控分闸执行成功  00001101						   							  							   
#define CLOSE_SELECT_FAIL             0x02 | 0x20 // 遥控合闸选择失败  00000010
#define CLOSE_SELECT_SUCCESS          0x06 | 0x20 // 遥控合闸选择成功  00000110
#define CLOSE_EXECUTE_FAIL            0x0A | 0x20 // 遥控合闸执行失败  00001010
#define CLOSE_EXECUTE_SUCCESS         0x0E | 0x20 // 遥控合闸执行成功  00001110
#define CLOSE_LOOP_SELECT_FAIL        0x03 | 0x20 // 合环选择失败      00000011
#define CLOSE_LOOP_SELECT_SUCCESS     0x07 | 0x20 // 合环选择成功      00000111
#define CLOSE_LOOP_EXECUTE_FAIL       0x0B | 0x20 // 合环执行失败      00001011
#define CLOSE_LOOP_EXECUTE_SUCCESS    0x0F | 0x20 // 合环执行成功      00001111
#define DISTANT_REMOTE_CANCEL         0x10        // 遥控取消

/* VALUE ----------------------------------------------------------------------*/
#define PULSE_TIME                    100.0f      // 分合闸脉宽时间
#define ENERGY_STORAGE_TIME           10000       // 储能时间
#define DEVICE_FAULT_TIME             10000       // 装置故障时间
#define LOOP_DISCONNECT_TIME          5000        // 控制回路断线时间

#define RATED_VALUE_U                 (100.0f)
#define RATED_VALUE_U0                (57.74f)
#define RATED_VALUE_I                 (5.0f)
#define RATED_VALUE_I0                (1.0f)
#define RATED_VALUE_P                 (0.5f)
#define RATED_VALUE_Q                 (0.5f)
#define RATED_VALUE_S                 (0.5f)
#define RATED_VALUE_PF                (1.0f)


#define DEADZONE_UP                   (100.0f)
#define DEADZONE_DOWN                 (1.0f)
#define DEADZONE_DEFAULT              (20.0f)

/* OPER ----------------------------------------------------------------------*/
/* 开出操作类型枚举 */
enum DoOperateType
{
    DO_CLOSE = 1,                      // 合闸
    DO_CLOSE_RECOVERY,                 // 合闸收回
    DO_OPEN,                           // 分闸
    DO_OPEN_RECOVERY,                  // 分闸收回
    DO_COIL_ENERGY_STORAGE,            // 线圈储能
    DO_ALARM_LED,                      // 告警灯
    DO_BATTERY_ACTIVE,                 // 电池活化启动
    DO_BATTERY_ACTIVE_RECOVERY,	       // 活化启动收回
    DO_BATTERY_ACTIVE_END,             // 电池活化结束
    DO_BATTERY_ACTIVE_END_RECOVERY,    // 活化结束收回
    DO_BATTERY_DROP_OUT,               // 电池退出
    DO_BATTERY_DROP_OUT_RECOVERY,	   // 电池退出收回
};
/* 遥控操作动作类型 */
enum ActReasion
{
    REMOTE = 1,                // 遥控
    LOCAL,                     // 本地操作
    DISTANCE,                  // 远方
    LOGIC_ACT,                 // 逻辑动作
    OTHER                      // 其他
};

/* FRAM----------------------------------------------------------------------*/
#define ADDR_FRAM_MONITOR               0x00000  // FRAM自检
#define ADDR_FRAM_UPDATE                0x00001  // 在线更新 2字节
#define ADDR_FRAM_START_NUM             0x00003  // 开机次数一个字节
#define ADDR_FRAM_CURRENT_SN            0x00004  // 当前定值区号 1个字节

#define ADDR_FRAM_CALI_FACTOR           0x00050  // 校准系数存储起始地址 0x50
#define ADDR_FRAM_AREA0                 0x00100	 // FRAM参数存储起始地址 运行参数0区     0x200
#define ADDR_FRAM_AREA1		            0x00300	 // FRAM参数存储起始地址	定值区一     0x300
#define ADDR_FRAM_AREA2		            0x00600	 // FRAM参数存储起始地址	定值区二     0x300
#define ADDR_FRAM_SOE                   0x00900  // FRAM参数存储起始地址 SOE  // 0x1400
#define ADDR_FRAM_SOE_NEW               0x01D00  // FRAM参数存储起始地址 SOE  // 0x1400
#define ADDR_FRAM_FEVENT                0x03100  // 故障事件起始地址 0x500
#define ADDR_FRAM_CO                    0x03600  // 操作记录起始地址 0x300
#define ADDR_FRAM_LOG                   0x03900  // 日志起始地址 0x1000
#define ADDR_FRAM_MEMORY                0x04900  // 日志起始地址 0x100
#define ADDR_FRAM_TELISIGNAL            0x05000  // 遥信起始地址 0x100

#define ADDR_FRAM_CONFIG                0x08000  // 配置起始地址   0x400


/* flag */                                        



/* FRAM区域枚举 */
enum FramArea
{
    VALUE_AREA0,                       // 定值区0
    VALUE_AREA1,                       // 定值区1
    VALUE_AREA2,                       // 定值区2
    SOE_RECODE,                        // SOE数据
    SOE_NEW_RECODE,                    // 新SOE数据
    FEVENT_RECODE,                     // FAULT_EVENT数据
    CO_RECODE,                         // 操作记录  
    LOG_RECODE,                        // 日志记录   
	MEMORY_FLAG,                       // 记录标志缓存
    CFG_RECODE,                        // 配置    
    TELESIGNAL,                        // 遥信数据
    CURRENT_SN,                        // 当前定值区号
    FRAM_AREA_NUM                      // 分区数量
};

enum UpdateState
{
    UPDATE_START,    // 升级启动
	UPDATE_COMPLATE, // 更新完成
	UPDATE_BACK,     // 回退
	RUN_NOMAL        // 运行正常
};

struct RecordStateType
{
	uint8_t flag;
	int32_t position;
	int32_t num;
};

/* DATABASE----------------------------------------------------------------------*/
/* database数据操作类型 */
enum TelesignalAddr
{
	ADDR_OPEN,	
	ADDR_CLOSE,
    ADDR_OPERATING_MECHANISM,                             // 电容欠压(储能开入)
    ADDR_LOW_PRESSURE,                                    // 低气压
    ADDR_POWERFAULTALARM,                                 // 电源故障告警
    ADDR_BATTERY_UNDERVOLTAGE_ALARM,                      // 电池欠压告警
    ADDR_BATTERYACTIVATIONSTATUS,                         // 电池活化状态
    ADDR_BATTERY_LOSS_ALARM,                              // 电池失电告警
    ADDR_BREAK_CONTACT,                                   // 分断/联络
    ADDR_RECLOSE_FA_STRAP,                                // 重合硬/FA压板
    ADDR_FUNCTION_HARDSTRAP,                              // 功能投退硬压板
    ADDR_REMOTE_EARTH,                                    // 远方/就地
    ADDR_SHORT_CIRCUIT_FAULT,                             // 短路故障/过流保护
    ADDR_EARTHING_FAULT,                                  // 接地故障/零序保护
    ADDR_PROTECTION_ACT,                                  // 保护动作
    ADDR_PROTECTION_LOCK,                                 // 保护闭锁
    ADDR_CLOSING_LOCK,                                    // 合闸闭锁
    ADDR_OPENING_LOCK,                                    // 分闸闭锁    
    ADDR_RECLOSE_ACT,                                     // 重合动作
    ADDR_RECLOSE_LOCK,                                    // 重合闸闭锁
    ADDR_DEVICE_FAULT,                                    // 装置故障
    ADDR_SELF_CHECK_ABNOMAL,                              // 自检异常
    ADDR_COMMUNICATION,                                   // 通讯 
    ADDR_SWTICHCLASS,                                     // 开关类型
    ADDR_BATTERY_FAULT_ALARM,                             // 电池故障告警
    ADDR_DOUBLE_SWITCH,                                   // 双点开关分合
    ADDR_CONTROL_LOOP_DISCONNECTION,                      // 控制回路断线
    ADDR_OVERCURRENT_IA_1,                                // A相过流一段保护
    ADDR_OVERCURRENT_IB_1,                                // B相过流一段保护
    ADDR_OVERCURRENT_IC_1,                                // C相过流一段保护
    ADDR_OVERCURRENT_IA_2,                                // A相过流二段保护
    ADDR_OVERCURRENT_IB_2,                                // A相过流二段保护
    ADDR_OVERCURRENT_IC_2,	                              // A相过流二段保护
    ADDR_OVERCURRENT_IA_3,                                // A相过流三段保护
    ADDR_OVERCURRENT_IB_3,                                // A相过流三段保护
    ADDR_OVERCURRENT_IC_3,	                              // A相过流三段保护
    ADDR_OVERCURRENT_I0_1,                                // 零序过流一段保护
    ADDR_OVERCURRENT_I0_2,	                              // 零序过流二段保护
    ADDR_OVERCURRENT_ACC_IA,                              // A相后加速过流
    ADDR_OVERCURRENT_ACC_IB,                              // B相后加速过流
    ADDR_OVERCURRENT_ACC_IC,                              // C相后加速过流
    ADDR_OVERCURRENT_INVERSE_IA,                          // A相反时限过流
    ADDR_OVERCURRENT_INVERSE_IB,                          // B相反时限过流
    ADDR_OVERCURRENT_INVERSE_IC,                          // C相反时限过流
    ADDR_RECLOSE_PROTECTION_1,                            // 重合闸一次保护
    ADDR_RECLOSE_PROTECTION_2,                            // 重合闸二次保护
    ADDR_RECLOSE_PROTECTION_3,                            // 重合闸三次保护
    ADDR_RECLOSE_PROTECTION_4,                            // 重合闸四次保护
    ADDR_RECLOSE_CLOCK_PROTECTION,                        // 重合闸闭锁保护
    ADDR_ZERO_RECLOSE_PROTECTION_1,                       // 零序重合闸一次保护
    ADDR_ZERO_RECLOSE_PROTECTION_2,                       // 零序重合闸二次保护
    ADDR_CLOCK_SECONDARY_CLOSE,                           // 闭锁二次重合闸
    ADDR_LOSS_TRIP,                                       // 失电跳闸
    ADDR_GET_CLOSSING_EVENT,                              // 得电合闸事件
    ADDR_CLOSING_FAULT,                                   // 合闸于故障
    ADDR_PROTECT_CLOCK,                                   // 保护闭锁
    ADDR_DOUBLE_PROHIBIT_CLOSSING,                        // 双侧有压禁止合闸
    ADDR_SINGLE_LOSS_CLOSING_EVENT,                       // 单侧失压延时合闸事件
    ADDR_REMAIN_CLOCK_CLOSSING,                           // 残压闭锁合闸
    ADDR_U0_EARTHING_FAULT_TRIP,                          // 零序电压接地故障跳闸
    ADDR_SMALL_SIGNAL_GROUND_LINE_TRIP,                   // 小信号接地选线跳闸
    ADDR_BREAKINGCURRENT,                                 // 遮断电流
    ADDR_OVERLOAD_EVENT,                                  // 过负荷事件
    ADDR_HAVEY_OVERLOAD_EVENT,                            // 重过载	
    ADDR_OVER_VOLTAGE_PROTECTION,                         // 过压保护
	ADDR_OVER_FREQUEBNCY_PROTECTION,                      // 过频
    ADDR_DOWN_VOLTAGE_PROTECTION,                         // 低压保护
	ADDR_DOWN_FREQUEBNCY_PROTECTION,                      // 低频		
    ADDR_OVERLIMIT_Uab_UP,                                // A相电压越限
    ADDR_OVERLIMIT_Uab_DOWN,                              // A相电压越限
    ADDR_OVERLIMIT_UBC_UP,                                // B相电压越限
    ADDR_OVERLIMIT_UBC_DOWN,                              // B相电压越限
    ADDR_OVERLIMIT_Uca_UP,                                // C相电压越限
    ADDR_OVERLIMIT_Uca_DOWN,                              // C相电压越限
    ADDR_OVERLIMIT_U0_UP,                                 // 零序电压越限
    ADDR_OVERLIMIT_U0_DOWN,                               // 零序电压越限
    ADDR_OVERLIMIT_IA_UP,                                 // A相电流越限
    ADDR_OVERLIMIT_IA_DOWN,                               // A相电流越限
    ADDR_OVERLIMIT_IB_UP,                                 // B相电流越限
    ADDR_OVERLIMIT_IB_DOWN,                               // B相电流越限
    ADDR_OVERLIMIT_IC_UP,                                 // C相电流越限
    ADDR_OVERLIMIT_IC_DOWN,                               // C相电流越限
    ADDR_OVERLIMIT_I0_UP,                                 // 零序电流越限
    ADDR_OVERLIMIT_I0_DOWN,                               // 零序电流越限
    ADDR_OVERLIMIT_DC_U_UP,                               // 直流电压越限
    ADDR_OVERLIMIT_DC_U_DOWN,                             // 直流电压越限
    ADDR_OVERLIMIT_DC_I_UP,                               // 直流电流越限
    ADDR_OVERLIMIT_DC_I_DOWN,                             // 直流电流越限
    ADDR_DEVICE_POWER_DOWN,                               // 装置掉电
	
	TELESIGNAL_NUM
};

///* 遥信数据 */
//typedef union TagTelesignalDatabase
//{
//    struct Telesignal
//    {
//        rt_uint8_t switchOpen;               // 开关分位
//        rt_uint8_t switchClose;              // 开关合位
//        rt_uint8_t operatingMechanism;       // 弹簧储能/永磁电容欠压
//        rt_uint8_t lowPressure;              // 低气压
//        rt_uint8_t powerFaultAlarm;          // 电源故障告警
//        rt_uint8_t batteryUnderVoltageAlarm; // 电池欠压告警
//        rt_uint8_t batteryActivationStatus;  // 电池活化状态
//        rt_uint8_t batteryLossAlarm;         // 电池失电告警
//        rt_uint8_t breakContact;             // 分断/联络	(模式)
//        union
//        {
//            rt_uint8_t recloseHardStrap;     // 重合闸硬压板
//            rt_uint8_t faHardStrap;          // FA功能投退硬压板
//        } FA;
//        rt_uint8_t functionHardStrap;        // 功能投退硬压板
//        rt_uint8_t remoteEarth;              // 远方/就地
//        rt_uint8_t shortCircuitFault;        // 短路故障/过流保护
//        rt_uint8_t earthingFault;            // 接地故障/零序保护
//        rt_uint8_t protectionAct;            // 保护动作
//        rt_uint8_t protectionLock;           // 保护闭锁
//        rt_uint8_t closingLocked;            // 合闸闭锁
//        rt_uint8_t openingLockedEvent;       // 分闸闭锁事件
//        rt_uint8_t recloseAct;               // 重合动作
//        rt_uint8_t recloseLock;  			  // 重合闭锁
//        rt_uint8_t deviceFault;              // 装置故障
//		rt_uint8_t selfCheckAbnomal;         // 自检异常
//		rt_uint8_t communication;            // 通讯
//        rt_uint8_t swtichclass;              // 开关类型		
//        rt_uint8_t batteryLowVoltageAlarm;   // 蓄电池低压告警(直流量)
//        rt_uint8_t doubleSwitch;             // 双点开关分合
//        rt_uint8_t controlLoopDisconnection; // 控制回路断线
//        rt_uint8_t overcurrentIa1;           // A相过流一段保护
//        rt_uint8_t overcurrentIb1;           // B相过流一段保护
//        rt_uint8_t overcurrentIc1;           // C相过流一段保护
//        rt_uint8_t overcurrentIa2;           // A相过流二段保护
//        rt_uint8_t overcurrentIb2;           // B相过流二段保护
//        rt_uint8_t overcurrentIc2;           // C相过流二段保护
//        rt_uint8_t overcurrentIa3;           // A相过流三段保护
//        rt_uint8_t overcurrentIb3;           // B相过流三段保护
//        rt_uint8_t overcurrentIc3;           // C相过流三段保护
//        rt_uint8_t overcurrentI01;           // 零序过流一段保护
//        rt_uint8_t overcurrentI02;           // 零序过流二段保护
//        rt_uint8_t overcurrentAccIa;		  // A相后加速过流
//        rt_uint8_t overcurrentAccIb;		  // B相后加速过流
//        rt_uint8_t overcurrentAccIc;		  // C相后加速过流
//        rt_uint8_t overcurrentInverseIa;	  // A相反时限过流
//        rt_uint8_t overcurrentInverseIb;	  // B相反时限过流
//        rt_uint8_t overcurrentInverseIc;	  // C相反时限过流
//        rt_uint8_t recloseProtection1;		  // 重合闸一次保护
//        rt_uint8_t recloseProtection2;		  // 重合闸二次保护
//        rt_uint8_t recloseProtection3;		  // 重合闸三次保护
//        rt_uint8_t recloseProtection4;		  // 重合闸四次保护
//        rt_uint8_t recloseClockProtection;	  // 重合闸闭锁保护
//        rt_uint8_t zeroRecloseProtection1;   // 零序重合闸一次保护
//        rt_uint8_t zeroRecloseProtection2;   // 零序重合闸二次保护
//        rt_uint8_t clockSecondaryClose;      // 闭锁二次重合闸
//        rt_uint8_t lossTrip;                 // 失电跳闸
//        rt_uint8_t getClossingEvent;		  // 得电合闸事件
//        rt_uint8_t closingFaultEvent;        // 合闸于故障
//        rt_uint8_t protectionLockedEvent;    // 保护闭锁事件
//        rt_uint8_t doubleProhibitClossing;   // 双侧有压禁止合闸
//        rt_uint8_t singleLossClosingEvent;   // 单侧失压延时合闸事件
//        rt_uint8_t remainClockClossing;      // 残压闭锁合闸
//        rt_uint8_t earthingFaultTripU0;      // 零序电压接地故障跳闸
//        rt_uint8_t smallSignalEarthingTrip;  // 小信号接地选线跳闸
//        rt_uint8_t breakingCurrent;          // 遮断电流
//        rt_uint8_t overloadEvent;            // 过负荷事件		
//        rt_uint8_t haveyOverload;            // 重过载
//        rt_uint8_t overVoltageProtection;    // 过压保护
//        rt_uint8_t overFrequencyProtection;  // 过频保护		
//        rt_uint8_t downVoltageProtection;    // 低压保护	
//        rt_uint8_t downFrequencyProtection;  // 低频保护
//        rt_uint8_t overLimitUaUp;            // A相电压越上限
//        rt_uint8_t overLimitUaDown;          // A相电压越下限
//        rt_uint8_t overLimitUbUp;            // B相电压越上限
//        rt_uint8_t overLimitUbDown;          // B相电压越下限
//        rt_uint8_t overLimitUcUp;            // C相电压越上限
//        rt_uint8_t overLimitUcDown;          // C相电压越下限
//        rt_uint8_t overLimitU0Up;            // 零序电压越上限
//        rt_uint8_t overLimitU0Down;          // 零序电压越下限
//        rt_uint8_t overLimitIaUp;            // A相电压越上限
//        rt_uint8_t overLimitIaDown;          // A相电压越下限
//        rt_uint8_t overLimitIbUp;            // B相电压越上限
//        rt_uint8_t overLimitIbDown;          // B相电压越下限
//        rt_uint8_t overLimitIcUp;            // C相电压越上限
//        rt_uint8_t overLimitIcDown;          // C相电压越下限
//        rt_uint8_t overLimitI0Up;            // 零序电压越上限
//        rt_uint8_t overLimitI0Down;          // 零序电压越下限
//        rt_uint8_t overLimitDC_U_Up;         // 直流电压越上限
//        rt_uint8_t overLimitDC_U_Down;       // 直流电压越下限
//        rt_uint8_t overLimitDC_I_Up;         // 直流电流越限
//        rt_uint8_t overLimitDC_I_Down;       // 直流电流越限
//        rt_uint8_t devicePowerDown;          // 装置掉电
//    }Str;

//    rt_uint8_t buf[sizeof(struct Telesignal)];

//}TelesignalDatabase;

enum TelemetryAddr
{
	ADDR_F,
    ADDR_IA,
    ADDR_IB,
    ADDR_IC,
    ADDR_I0,
    ADDR_Uab,
    ADDR_Ubc,
    ADDR_Uca,
    ADDR_U0,
    ADDR_UAB,
    ADDR_UBC,
	ADDR_P,
    ADDR_Q,
    ADDR_S,
    ADDR_PF,
    ADDR_DC1,
    ADDR_DC2,
    ADDR_T,
	ADDR_AIPHY_Ua_Ia,
	ADDR_AIPHY_Ub_Ib,
	ADDR_AIPHY_UC_Ic,
    ADDR_AIPHY_U0_I0,
    ADDR_ALPHY_Uab_UBC,		
	ADDR_Ia_ONCE,
	ADDR_Ib_ONCE,
	ADDR_Ic_ONCE,
	ADDR_I0_ONCE,	
    ADDR_Uab_ONCE,
    ADDR_Ubc_ONCE,
    ADDR_Uca_ONCE,
    ADDR_U0_ONCE,
    ADDR_UAB_ONCE,
    ADDR_UBC_ONCE,
	ADDR_P_ONCE,
    ADDR_Q_ONCE,
    ADDR_S_ONCE,
//    VOLTAGE_PASS_RATE,
	THIRDHARMONIC_Uab,
	THIRDHARMONIC_Ubc,
	THIRDHARMONIC_Uca,
	THIRDHARMONIC_U0,
//	THIRDHARMONIC_UAB,
//	THIRDHARMONIC_UBC,
	THIRDHARMONIC_Ia,
	THIRDHARMONIC_Ib,
	THIRDHARMONIC_Ic,
	THIRDHARMONIC_I0,
	FIFTHHARMONIC_Uab,
	FIFTHHARMONIC_Ubc,
	FIFTHHARMONIC_Uca,
	FIFTHHARMONIC_U0,
//	FIFTHHARMONIC_UAB,
//	FIFTHHARMONIC_UBC,
	FIFTHHARMONIC_Ia,
	FIFTHHARMONIC_Ib,
	FIFTHHARMONIC_Ic,
	FIFTHHARMONIC_I0,

    TELEMETRY_NUM
};
 
enum TelecontrlAddr
{
    ADDR_REMOTE_OPERATE = 0x6001, // 遥控操作
	ADDR_REMOTE_ACTIVE,           // 电池活化
	ADDR_REMOTE_RESET,            // 远方复位
	ADDR_REMOTE_CLEAR,            // 远方清除记录
    ADDR_LOCAL_OPERATE,           // 本地操作
	ADDR_LOCAL_RESET,             // 本地复位
	ADDR_LOCAL_CLEAR,             // 本地清除记录
	ADDR_HANDHELD_OPER,           // 手持操作
};
/* PARA ----------------------------------------------------------------------*/
#define RT_SYS_CONFIG_DEFAULT                           \
{                                                       \
    "F31                     ",                         \
    "RT-Thread               ",                         \
    "                        ",                         \
    "HV01.01                 ",                         \
    "SV03.021                ",                         \
    "CRC                     ",                         \
    "101 / 104               ",                         \
    "SJFD2000                ",                         \
    "F31xxxxxxxxx20171211xxxx",                         \
    "                        ",	                        \
    TERMINAL_PRODUCT_SERIAL_NUMBER                      \
}

#define SIZEOFINTRIPARAME             24          // 固有参数长度

#define SN_MIN                        0           // 最小定值区号
#define SN_MAX                        2           // 最大定值区号
#define DB_SN0                        0           // 0区
#define DB_SN1                        1           // 1区
#define DB_SN2                        2           // 2区
#define DB_CALI                       3           // 校准系数区

enum ParameterType
{
    ZERODRIFT,
    DEADZONE,
    CALIBRATE_FACTOR,
};

struct Inherent
{
    char terminalType[SIZEOFINTRIPARAME];                      // 终端类型
    char terminalOpreatSystem[SIZEOFINTRIPARAME];              // 终端操作系统
    char terminalManufacturer[SIZEOFINTRIPARAME];              // 终端制造商
    char terminalHardwareVersion[SIZEOFINTRIPARAME];           // 终端硬件版本
    char terminalSoftwareVersion[SIZEOFINTRIPARAME];           // 终端软件版本
    char terminalSoftwareVersionCheckCode[SIZEOFINTRIPARAME];  // 终端软件版本校验码
    char terminalCommunicationProtocolType[SIZEOFINTRIPARAME]; // 终端通信规约类型
    char terminalFactoryModel[SIZEOFINTRIPARAME];              // 终端出厂型号
    char terminalID[SIZEOFINTRIPARAME];                        // 终端ID号
    char terminalMAC[SIZEOFINTRIPARAME];                       // 终端网卡MAC地址
    char terminalProductSerialNumber[SIZEOFINTRIPARAME];                       // 终端产品序列号
};

		
typedef union TagInherentPara
{
    struct Inherent Str;
    char buf[sizeof(struct Inherent) / sizeof(char)];
} InherentPara;

enum AddrRunParameter
{
    OPERATING_MECHANISM,                  // 操作机构(0-弹簧/1-永磁)
	SWITCH_TYPE,						  // 开关类型(0-断路器/1-负荷开关)
	BREAK_WORK_MODE,                      // 断路器工作模式(0-无/1-常规保护/2-电压时间型/3-电压电流型/4-电流计数型)
	LOAD_WORK_MODE,                       // 负荷开关工作模式(0-无/1-电压时间型/2-电压电流型/3-电流计数型/4-分界负荷开关型)
	CONNECT_SWITCH_RECOGNIZE,             // 联络开关识别
	OUTPUT_OVERTURN,                      // 永磁输出翻转
	CAP_UNDERVOLTAGE_CLOSING,	          // 合闸电容欠压定值
	CAP_UNDERVOLTAGE_OPENING,             // 分闸电容欠压定值
	CAP_UNDERVOLTAGE_FACTOR,			  // 电容欠压返回系数
	RATIO_U_ONE_TURN,                     // 电压变比一次	
	RATIO_U_SECONDARY,                    // 电压变比二次
	RATIO_U0_ONE_TURN,                    // 零序电压变比一次
	RATIO_U0_SECONDARY,                   // 零序电压变比二次	
	RATIO_I_ONE_TURN,                     // 电流变比一次	
	RATIO_I_SECONDARY,                    // 电流变比二次
	RATIO_I0_ONE_TURN,                    // 零序电流一次变比
	RATIO_I0_SECONDARY,                   // 零序电流二次变比	
    DI_SHAKING_TIME,                      // 开入采集防抖时间 
    CLOSING_PULSE_TIME,                   // 合闸脉宽时间
    OPENING_PULSE_TIME,			          // 分闸脉宽时间
    REVERSE_TIME,                         // 反校时间
    ZERODRIFT_F,                          // 频率零漂
    ZERODRIFT_Ia,                         // A相电流零漂 
    ZERODRIFT_Ib,                         // B相电流零漂
    ZERODRIFT_Ic,                         // C相电流零漂
    ZERODRIFT_I0,                         // 零序电流零漂
    ZERODRIFT_Uab,                        // 线电压Uab零漂
    ZERODRIFT_Ubc,                        // 线电压Ubc零漂
    ZERODRIFT_Uca,                        // 线电压Uac零漂
    ZERODRIFT_U0,                         // 零序电压零漂
    ZERODRIFT_UAB,                        // 线电压Uab零漂
    ZERODRIFT_UBC,                        // 线电压Ubc零漂
    ZERODRIFT_P,                          // 有功功率零漂
    ZERODRIFT_Q,                          // 无功功率零漂
    ZERODRIFT_S,                          // 视在功率零漂
    ZERODRIFT_PF,                         // 功率因数零漂	
    ZERODRIFT_DC1,                        // 直流电压零漂	
    ZERODRIFT_DC2,                        // 直流电压2零漂
    ZERODRIFT_T,                          // 温度零漂	
    DEADZONE_F,                           // 频率死区
    DEADZONE_Ia,                          // A相电流死区
    DEADZONE_Ib,                          // B相电流死区
    DEADZONE_Ic,                          // C相电流死区
    DEADZONE_I0,                          // 零序电流死区
    DEADZONE_Uab,                         // 线电压Uab死区
    DEADZONE_Ubc,                         // 线电压Ubc死区
    DEADZONE_Uca,                         // 线电压Uac死区
    DEADZONE_U0,                          // 零序电压死区
    DEADZONE_UAB,                         // 线电压Ubc死区
    DEADZONE_UBC,                         // 线电压Uac死区
    DEADZONE_P,                           // 有功功率死区
    DEADZONE_Q,                           // 无功功率死区
    DEADZONE_S,                           // 视在功率死区
    DEADZONE_PF,                          // 功率因数死区
    DEADZONE_DC1,                         // 直流电压死区
    DEADZONE_DC2,                         // 直流量2死区
    DEADZONE_T,				              // 温度死区	
	
    RUN_PARAMETER_NUM,	                  // 运行参数数量
};

enum AddrCalibrateFactor
{
    CALIFACTOR_F,                        // 频率校准系数
	CALIFACTOR_Ia,                       // A相电流(Ia)校准系数
	CALIFACTOR_Ib,                       // B相电流(Ib)校准系数
    CALIFACTOR_Ic,                       // C相电流(Ic)校准系数
	CALIFACTOR_I0,                       // 零序电流(I0)校准系数
    CALIFACTOR_Uab,                      // 线电压(Uab)校准系数
	CALIFACTOR_Ubc,                      // 线电压(Ubc)校准系数
    CALIFACTOR_Uca,                      // 线电压(Uca)校准系数
	CALIFACTOR_U0,                       // 零序电压(U0)校准系数
    CALIFACTOR_UAB,                      // 线电压(Uab)校准系数
	CALIFACTOR_UBC,                      // 线电压(Ubc)校准系数
    CALIFACTOR_P,                        // 有功功率(p)校准系数（无效）
	CALIFACTOR_Q,                        // 无功功率(Q)校准系数（无效）
    CALIFACTOR_S,                        // 视在功率(S)校准系数（无效）
	CALIFACTOR_PF,                       // 功率因数(Pf)校准系数（无效） 		
    CALIFACTOR_DC1,                      // 直流电压(V)校准系数
	CALIFACTOR_DC2,                      // 直流电压(V)校准系数
    CALIFACTOR_T,			             // 温度(T)校准系数
	CALIFACTOR_ALPHA_UaIa,	             // 角UaIa校准系数
	CALIFACTOR_ALPHA_UbIb,	             // 角UbIb校准系数
	CALIFACTOR_ALPHA_UCIc,	             // 角UcIc校准系数
	CALIFACTOR_ALPHA_U0I0,	             // 角U0I0校准系数
	CALIFACTOR_ALPHA_UaUb,	             // 角UaUb校准系数		
	
	CALIFACTOR_NUM
};

enum AddrFixedValue
{
    OVER_CURRENT_SWITCH1,                // 过流1段投退 “1投入”、“0退出”
//    DIRE_CTIONLOCK_SWITCH1,              // 过流1段方向闭锁 “1投入”、“0退出”
	OVER_CURRENT_VALUE1,                 // 过流1段电流 流定值，整定范围：00.00A~99.99A
	OVER_CURRENT_TIME1,                  // 过流1段时间 时间定值，整定范围：00.00s~99.99s
    OVER_CURRENT_SWITCH2,                // 过流2段投退 “1投入”、“0退出”
//    DIRE_CTIONLOCK_SWITCH2,              // 过流2段方向闭锁 “1投入”、“0退出”
	OVER_CURRENT_VALUE2,                 // 过流2段电流 电流定值，整定范围：00.00A~99.99A
	OVER_CURRENT_TIME2,                  // 过流2段时间 时间定值，整定范围：00.00s~99.99s    
	OVER_CURRENT_SWITCH3,                // 过流3段投退 “1投入”、“0退出”
//    DIRE_CTIONLOCK_SWITCH3,              // 过流3段方向闭锁 “1投入”、“0退出”
	OVER_CURRENT_VALUE3,                 // 过流3段电流 电流定值，整定范围：00.00A~99.99A
	OVER_CURRENT_TIME3,                  // 过流3段时间 时间定值，整定范围：00.00s~99.99s	
    OVER_CURRENTI0_SWITCH1,              // 零序过流1段投退 “1投入”、“0退出”
	OVER_CURRENTI0_VALUE1,               // 零序过流1段电流 流定值，整定范围：00.00A~99.99A
	OVER_CURRENTI0_TIME1,                // 零序过流1段时间 时间定值，整定范围：00.00s~99.99s	
    OVER_CURRENTI0_SWITCH2,              // 零序过流2段投退 “1投入”、“0退出”
	OVER_CURRENTI0_VALUE2,               // 零序过流2段时间 时间定值，整定范围：00.00s~99.99s
	OVER_CURRENTI0_TIME2,                // 零序过流2段时间 时间定值，整定范围：00.00s~99.99s		
	OVERLIMIT_SWITCH,                    // 越限报警功能投退
	UPLIMIT_FACTOR,                      // 越限上限系数
	DOWNLIMIT_FACTOR,                    // 越限下限系数
    OVERLIMIT_TIME,                      // 越限延时
	OVERLIMIT_ALARM_SWITCH_Uab,          // Uab越限报警投退 
	OVERLIMIT_ALARM_SWITCH_UBC,          // Ubc越限报警投退
	OVERLIMIT_ALARM_SWITCH_Uca,          // Uac越限报警投退
	UPLIMIT_VOLTAGE_U,                   // 电压上限定值
	DOWNLIMIT_VOLTAGE_U,                 // 电压下限定值
	OVERLIMIT_ALARM_SWITCH_IA,           // Ia越限报警投退
	OVERLIMIT_ALARM_SWITCH_IB,           // Ib越限报警投退
	OVERLIMIT_ALARM_SWITCH_IC,           // Ic越限报警投退
	UPLIMIT_CURRENT_I,                   // 电流上限定值
	DOWNLIMIT_CURRENT_I,                 // 电流下限定值	
	OVERLIMIT_ALARM_SWITCH_U0,           // 零序电压越限投退	
	UPLIMIT_VOLTAGE_U0,                  // 零序电压上限定值
	DOWNLIMIT_VOLTAGE_U0,                // 零序电压下限定值
	OVERLIMIT_ALARM_SWITCH_I0,           // 零序电流越限投退	
	UPLIMIT_VOLTAGE_I0,                  // 零序电流上限定值
	DOWNLIMIT_VOLTAGE_I0,                // 零序电流下限定值	
	OVERLIMIT_ALARMSWITCH_DC_U,          // 直流电压越限投退
	UPLIMIT_DC_VOLTAGE_U,                // 直流电压上限定值
	DOWNLIMIT_DC_VOLTAGE_U,              // 直流电压下限定值
	OVERLIMIT_ALARMSWITCH_DC_I,          // 直流电流越限投退
	UPLIMIT_DC_CURRENT_I,                // 直流电流上限定值
	DOWNLIMIT_DC_CURRENT_I,              // 直流电流下限定值	
	HEAVY_OVERLOAD_SWITCH,               // 重过载保护投退
	HEAVY_OVERLOAD_VALUE,                // 重过载定值
	HEAVY_OVERLOAD_FACTOR,               // 重过载返回系数		
	HEAVY_OVERLOAD_TIME,                 // 重过载延时	
	OVERLOAD_SWITCH,                     // 过负荷保护投退
	OVERLOAD_VALUE,                      // 过负荷定值
	OVERLOAD_FACTOR,                     // 过负荷返回系数		
	OVERLOAD_TIME,	                     // 过负荷延时
    BATTERY_LOWVOLTAGE_ALARM_SWITCH,     // 蓄电池低压报警投退	
	BATTERY_LOWVOLTAGE_VALUE,            // 电池低压定值
	BATTERY_LOWVOLTAGE_FACTOR,           // 电池低压返回系数		
	BATTERY_LOWVOLTAGE_TIME,             // 电池低压延时	
    BATTERY_ACTIVE_SWITCH,               // 电池活化投退
    BATTERY_ACTIVE_CYCLE,                // 电池活化周期(天)
    BATTERY_ACTIVE_TIME,                 // 电池活化时间(h)
    BATTERY_ACTIVE_FAULT_VOLTAGE,        // 电池活化故障电压
    BATTERY_ACTIVE_FAULT_TIME,           // 电池活化故障延时
	AUTOMATIC_RESET_SWITCH,              // 自动复归使能
	AUTOMATIC_RESET_TIME,                // 自动复位时间
	OVERVOLTAGE_SWITCH,                  // 过压投退
	OVERVOLTAGE_VALUE,                   // 过压定值
    OVERVOLTAGE_TIME,                    // 过压时间
    OVERVOLTAGE_FACTOR,                  // 过压返回系数	
	OVERFREQUENCY_SWITCH,                // 过频投退
	OVERFREQUENCY_VALUE,                 // 过频定值
    OVERFREQUENCY_TIME,                  // 过频时间
    OVERFREQUENCY_FACTOR,                // 过频返回系数	
	DOWNVOLTAGE_SWITCH,                  // 低压投退
	DOWNVOLTAGE_VALUE,                   // 低压定值
    DOWNVOLTAGE_TIME,                    // 低压时间
    DOWNVOLTAGE_FACTOR,                  // 低压返回系数	
	DOWNFREQUENCY_SWITCH,                // 低频投退
	DOWNFREQUENCY_VALUE,                 // 低频定值
    DOWNFREQUENCY_TIME,                  // 低频时间
    DOWNFREQUENCY_FACTOR,                // 低频返回系数	
    CLOSING_LOOP_SWITCH,                 // 合环功能投退
    VOLTAGE_DIFFERENCE,                  // 两侧压差	
	PHASEANGLE_DIFFERENCE,				 // 相角差
    CONTROL_LOOP_ABNOMAL_ENABLE,         // 控制回路异常使能	
	INVERSE_SWITCH,                      // 反时限投退
	INVERSE_CURRENT_VALUE,               // 反时限保护电流值
	INVERSE_TIME,                        // 反时限保护延时
	IACC_SWITCH,                         // 后加速投退 软压板定值，可以选择“投入”、“退出"
	IACC_CURRENT_VALUE,                  // 后加速电流 电流定值，整定范围：00.00s~99.99A
	IACC_TIME,                           // 后加速时间 时间定值，整定范围：00.00s~99.99s	
	INRUSH_SWITCH,                       // 涌流抑制投退 软压板定值，可以选择“投入”、“退出”
	INRUSH_TIME,                         // 涌流抑制时间 时间定值，整定范围：0000~9999ms
	SECONDARY_RECLOSE_LOCK_SWITCH,       // 二次重合闸闭锁投退
	SECONDARY_RECLOSE_LOCK_TIME,         // 闭锁二次重合闸时间 时间定值，整定范围：00.00s~99.99
	RECLOSE_TIMES_I,                     // 重合闸次数 可以选择：0，1，2，3，4
	RECLOSE_TIMES_I0,                    // 零序重合闸次数 可以选择：0，1，2
	RECLOSE_TIMER1,                      // 1次重合闸时间 时间定值，整定范围：00.00s~99.99s
	RECLOSE_TIMER2,                      // 2次重合闸时间 时间定值，整定范围：00.00s~99.99s
	RECLOSE_TIMER3,                      // 3次重合闸时间 时间定值，整定范围：00.00s~99.99s
	RECLOSE_TIMER4,                      // 4次重合闸时间 时间定值，整定范围：00.00s~99.99s	
	RE_RESET_TIME,						 // 重合闸复位时间
	RE_CHARGE_TIME,						 // 重合闸充电时间
	CURRENT_VALUE,                       // 电流定值
	VOLTAGE_VALUE,                       // 电压定值
	FAULT_CHECK_Y_TIME,                  // 故障检测Y时间
	SHORT_LOCKED_OPENING_TIME,           // 短时闭锁分闸时间
	LOSS_ELECTRICITY_SWITCH,             // 失电分闸投退 “1投入”、“0退出”
	LOSS_OPENING_TIME,                   // 分闸时间
	GET_VOLTAGE_CLOSSING_SWITCH,         // 得电合闸投退
	GET_VOLTAGE_CLOSSING_X_TIME,         // 得电延时合闸X时间
	SINGLE_LOSS_VOLTAGE_SWITCH,          // 单侧失压延时合闸
	SINGLE_LOSS_VOLTAGE_XLTIME,            // 单侧失压延时XL时间
	DOUBLE_VOLTAGE_SWITCH,               // 双侧有压禁止合闸投退
	REMAIN_VOLTAGE_SWITCH,               // 残压脉冲投退
	REMAIN_VOLTAGE_VALUE,                // 残压定值
	VOLTAGE_U0_SWITCH,                   // 零序电压保护投退
	VOLTAGE_U0_VALUE,                    // 零序电压值
	VOLTAGE_U0_TIME,                     // 零序电压延时
	BREAKING_CURRENT_SWITCH,             // 非遮断电流投退
	BREAKING_CURRENT_VALUE,        		 // 非遮断电流
	
	
	FIXED_VALUE_NUM                      // 定值数量
};

/* common操作结构体 */
struct CommonInfo
{
    uint8_t operateType; // 操作类型 SOE/定值
    uint32_t num;        // 定值数量
    uint8_t sn;          // 定值区号
};

/* 定值/参数操作结构体 */
struct ValueParameterOperate
{
    uint8_t type;        // 操作类型 预置/固化/撤回
    uint32_t num;        // 定值数量
    uint32_t len;        // 预置字节数
    uint8_t operateSN;   // 操作定值区号
    uint8_t currentSN;   // 当前运行定值区号
    uint8_t calibratFlag;// 校准标志
};


typedef struct TagValueParameterStr
{
    uint16_t addr;
    uint8_t tag;
    uint8_t length;
} ValueParameterStr;

struct ValueParaPreset
{
    ValueParameterStr property[VALUE_PARAMETER_TOTAL_NUM];
    float value[VALUE_PARAMETER_TOTAL_NUM * 4 + 24];
};

/* RECORD ----------------------------------------------------------------------*/
/* COS数据结构体 */
__packed struct COS_Str
{
    rt_uint16_t addr;
    rt_uint8_t  value;
};

/* SOE数据结构体 */
__packed struct SOE_Str
{
    rt_uint16_t addr;
    rt_uint8_t  value;
    struct CP56Time2a_t time;
};

/* CO数据结构体 */
__packed struct CO_Str
{
    rt_uint16_t addr;
    rt_uint8_t  value;
    struct CP56Time2a_t time;
};

/* 变化遥测数据结构体 */
__packed struct NVA_Str
{
    rt_uint16_t addr;
    float value;
};

/* 故障事件数据结构体 */
__packed struct FEVENT_Str
{
    rt_uint8_t yx_num;
    rt_uint8_t yx_type;
    struct SOE_Str yx[1];
    rt_uint8_t yc_num;
    rt_uint8_t yc_type;
    struct NVA_Str yc[10];
};

/* ULOG数据结构体 */
__packed struct ULOG_Str
{
    rt_uint16_t addr;
    char description[24];
    rt_uint8_t  value;
    struct CP56Time2a_t time;
};

struct Queue_Flag_Str
{
    rt_uint16_t full;
    rt_uint16_t in;
    rt_uint16_t out[DEV_MAX_NUM];
};

struct Fatfs_Flag_Str
{
    rt_uint16_t fullnum;
    rt_uint16_t currentnum;
};

/* memory数据结构体 */
struct MEMORY_FLAG_Str
{
    struct Queue_Flag_Str queue_soe;
    struct Queue_Flag_Str queue_soe_new;
    struct Queue_Flag_Str queue_co;
    struct Queue_Flag_Str queue_fevent;
    struct Queue_Flag_Str queue_ulog;
    struct Fatfs_Flag_Str fatfs_soe;
    struct Fatfs_Flag_Str fatfs_co;
    struct Fatfs_Flag_Str fatfs_fevent;
    struct Fatfs_Flag_Str fatfs_ulog;
};

/* LOCK ----------------------------------------------------------------------*/
/* 通讯互锁定时 */
#define COMMUNICATLOCKFUC         0xFF000000 //功能
#define COMMUNICATLOCKCLRCON      0x01000000 //清零
#define COMMUNICATLOCKRESET       0x02000000 //复位标志
#define COMMUNICATLOCKRECORD      0x04000000 //有数据存贮
#define COMMUNICATLOCKCLEAR       0x08000000 //清除记录
#define COMMUNICATLOCKJSON        0x10000000 //Json填写

#define COMMUNICATLOCKUSERSTA     0x0000FFFF //正在有设备使用
#define COMMUNICATLOCKUSERCON     0x000000FF //需要超时处理设备
#define COMMUNICATLOCKREADDIR     0x00010000 //读目录标志
#define COMMUNICATLOCKREADFILE    0x00020000 //读文件标志
#define COMMUNICATLOCKWRITEFILE   0x00040000 //写文件标志
#define COMMUNICATLOCKUSERNOCON   0x0000FF00 //不需要超时处理设备
#define COMMUNICATLOCKCOMTRADE    0x00000100 //写comtrate文件
#define COMMUNICATLOCKRANDOM      0x00000200 //写SOECO文件
#define COMMUNICATLOCKMAKEFS      0x00000400 //格式化文件
#define COMMUNICATLOCKLOSEPOWER   0x00000800 //失电
#define COMMUNICATLOCKWRITEJSON   0x00001000 //Json填写

enum
{
    COM_YK=0,
    COM_PARA,
    COM_FILE,
    COM_MAX
};

/* CONFIGE ----------------------------------------------------------------------*/
/* lwip_dev */
struct lwip_dev
{
    unsigned char mac[6]; // MAC address
    unsigned char remoteip[4]; // default ip address
    unsigned char ip[4]; // ip address
    unsigned char netmask[4]; // net mask address
    unsigned char gateway[4]; // gate address
    unsigned char dns[4];  // DNS server IP Address
    volatile unsigned char dhcpstatus; 
};

struct ConfigurationSetDatabase
{
//串口配置
    uint16_t UartPort;      //左1，中2，右3
    uint16_t UartBaudRate;      //波特率 1200.2400,4800,9600,38400,115200
    uint16_t UartWordLength;    //数据位
    uint16_t UartStopBits;      //停止位
    uint16_t UartParity;        //无校验1，奇校验2，偶校验3
//串口设置
    uint16_t UartBalanMode;     //非平衡0，平衡1
    uint16_t UartSourceAddr;    //从站地址
    uint16_t UartLinkAddrSize;  //从站地址长度
    uint16_t UartASDUCotSize;   //传送原因字节数
    uint16_t UartASDUAddr;      //ASDU地址
    uint16_t UartASDUAddrSize;  //ASDU地址长度
//网口配置
    uint16_t Netip1[4]; //IP上
    uint16_t Netip2[4]; //IP下·
    uint16_t Netnetmask[4]; //子网掩码
    uint16_t Netgateway[4]; //网关
    uint16_t Netdns[4];//DNS
//网口设置
    uint16_t NetSourceAddr;//从站地址
    uint16_t NetASDUAddr;//ASDU地址
//YX设置
    uint16_t YXSetNum;//YX设置总数
    uint16_t YXSet[299];//YX设置
//YC设置
    uint16_t YCAddr[50];//值
    uint16_t YCProperty[50];//属性
//YK设置
    uint16_t YKAddr[10];//值
    uint16_t YKProperty[10];//属性
//ID设置
    uint16_t ID_Value[12];//值
};

/* 属性值 */
#define NEWJUDG_AND 0x2

/* 属性配置 */
#define NEWPROPERTY_JUDG    0x0003 //判断

#define NEWJUDG_PROPERTY    0x00ff //属性判断（大类）
//移位
#define NEWPROPERTY_TI      0 //类型标识
#define NEWPROPERTY_NEG     2 //取反
#define NEWPROPERTY_COE     2 //系数
#define NEWPROPERTY_SOE     4 //上送SOE
#define NEWPROPERTY_COS     6 //上送COS
#define NEWONEYX_PROPERTY   0 //属性
//判断位数
#define NEWJUDG_ADDR        0x0fff //地址判断
#define NEWJUDG_NUM         0x00ff //数量判断
#define NEWJUDG_VAULE       0x000f //值判断
#define NEWJUDG_CAL         0x000f //运算判断
//移位
#define NEWONEYX_VAULE      12 //值
#define NEWONEYX_NUM        8 //数量
#define NEWONEYX_ADDR       0 //地址
#define NEWCAL_AND          2 //与
#define NEWCAL_NEG          0 //非
#define NEWONEYX_CAL        12 //运算

/* PUBLIC VALUE ----------------------------------------------------------------------*/
/* 缓存后的数据，防止计算被打断 */
extern double g_IaSampleBuf[ADC_SAMPLE_NUM * 2]; 
extern double g_IbSampleBuf[ADC_SAMPLE_NUM * 2];
extern double g_IcSampleBuf[ADC_SAMPLE_NUM * 2];
extern double g_I0SampleBuf[ADC_SAMPLE_NUM * 2];
extern double g_UaSampleBuf[ADC_SAMPLE_NUM * 2];
extern double g_UbSampleBuf[ADC_SAMPLE_NUM * 2];
extern double g_UcSampleBuf[ADC_SAMPLE_NUM * 2];
extern double g_U0SampleBuf[ADC_SAMPLE_NUM * 2];

extern double g_UASampleBuf[ADC_SAMPLE_NUM * 2];
extern double g_UCSampleBuf[ADC_SAMPLE_NUM * 2];
extern double g_Udc1SampleBuf[ADC_SAMPLE_NUM * 2];
extern double g_Udc2SampleBuf[ADC_SAMPLE_NUM * 2];

/* 线程运行状态 */
extern uint16_t g_ThreadRunSta;
/* 采样队列 */
extern rt_int8_t g_SampleQueueIn;  // 入指针
extern Queue *g_SampleQueueBuf;// __attribute__((at(SDRAM_ADDR_WAVE)));    // 数据空间
extern uint16_t g_StartWave;

/* 系统配置 */
extern struct ConfigurationSetDatabase g_ConfigurationSetDB;

/* 系统时间 */
extern struct SD2405Time g_SystemTime;

/* 遥信缓存 */
extern uint8_t				g_TelesignalDB[TELESIGNAL_NUM];	
/* 新遥信点表映射 */
//extern List NewList_Telesignal[TELESIGNAL_TOTAL_NUM];
extern rt_uint16_t g_NewMaxNumTelesignal;
extern rt_uint16_t g_NewToOldTelesignal[];//新点表映射

/* 遥测缓存 */
extern float g_TelemetryDB[TELEMETRY_NUM];
extern float g_TelemetryLastDB[TELEMETRY_NUM];
extern float g_secondHarmonicIa, g_secondHarmonicIb, g_secondHarmonicIc;
#if RT_USING_TELEMETRY_SET
extern float    g_TelemetrySetEnable[TELEMETRY_NUM];
extern float    g_TelemetrySetValue[TELEMETRY_NUM];
#endif

/* 新遥测点表映射 */
extern rt_uint16_t g_NewPropertyTelemetry[TELEMETRY_NUM];//新点表属性
extern rt_uint16_t g_NewAddTelemetry[TELEMETRY_NUM];//写入对应新地址

extern rt_uint16_t g_NewMaxNumTelemetry;//新点表个数
extern rt_uint16_t g_NewToOldTelemetry[TELEMETRY_NUM];//新点表映射，填原点表数组下标

/* 新遥控点表映射 */
extern rt_uint16_t g_NewToOldRemote[REMOTE_TOTAL_NUM];//新点表映射，填原点表数组下标
extern rt_uint16_t g_NewToOldPropertyRemote[REMOTE_TOTAL_NUM];//新点表属性

/* 通讯互锁标志 */
extern rt_uint32_t g_CommunicatFlag[COM_MAX];

/* 定值和参数缓存 */

//extern FixedValue *g_pFixedValue;
extern struct Inherent  g_InherentPara;
extern float    *g_pFixedValue;
extern float     g_FixedValue1[FIXED_VALUE_NUM];
extern float     g_FixedValue2[FIXED_VALUE_NUM];

extern float     g_Parameter[RUN_PARAMETER_NUM];
extern float     g_CalibrateFactor[CALIFACTOR_NUM];
extern struct    tagValueParaCfg  *g_pFixedValueCfg;

/* 定值操作信息 */
extern struct ValueParameterOperate g_ValueParaOperateInfo;

/* 定值和参数预置缓存 */
extern struct ValueParaPreset g_ValueParaPresetDB;

/* 需存储记忆数据 */
extern struct MEMORY_FLAG_Str g_FlagDB;

/* COS缓存 */
extern struct COS_Str g_COSDB[COS_MAX_NUM];

/* SOE缓存 */
extern struct SOE_Str g_SOEDB[SOE_MAX_NUM];
extern struct SOE_Str g_SOENewDB[SOE_NEW_MAX_NUM];
/* CO缓存 */
extern struct CO_Str g_CoDB[CO_MAX_NUM];

/* FEVENT缓存 */
extern struct FEVENT_Str g_FeventDB[FEVENT_MAX_NUM];

/* ULOG缓存 */
extern struct ULOG_Str g_UlogDB[ULOG_MAX_NUM];

/* NVA缓存 */
extern struct NVA_Str g_NVADB[NVA_MAX_NUM];

/* COS缓存参数 */
extern rt_uint32_t g_COSDBIn;
extern rt_uint32_t g_COSDBOut[DEV_MAX_NUM];

/* NVA缓存参数 */
extern rt_uint32_t g_NVADBIn;
extern rt_uint32_t g_NVADBOut[DEV_MAX_NUM];

/* 通讯信息 */
extern struct lwip_dev g_EthW5500; // w5500 information
extern struct lwip_dev g_EthDP83848; // dp83848 

/* 通讯灯状态 */
extern rt_uint32_t g_ComLed;

/* PUBLIC FUNCTION  ----------------------------------------------------------*/
extern uint8_t rt_multi_common_read_update_state(void);
extern void rt_multi_common_write_update_state(const uint8_t sta);

extern void rt_multi_common_data_save_value_to_fram(uint8_t sn);
extern void rt_multi_common_data_get_value_from_fram(uint8_t sn);
extern void rt_common_data_save_value_default_to_fram(void);
extern rt_uint16_t rt_multi_common_data_fram_record_write(uint8_t type, uint8_t *pBuf, uint32_t len);
extern void rt_multi_common_data_fram_update_state_write(uint8_t type);
extern void rt_multi_common_data_fram_record_read(uint8_t type, uint8_t *pBuf);
extern void rt_multi_common_data_powerdown_storage(void);


extern int rt_multi_common_data_init(void);
extern float* GetValueArray(uint16_t addr, uint8_t sn);
extern void DBRevert(uint8_t act);
extern void DBClear(void);
extern void ParameterCheck(void);

extern uint8_t DBReadValueAreaSN(void);
extern uint8_t DBSwitchValueArea(uint8_t sn);

extern uint8_t DBWriteValue(uint8_t *pData, struct CommonInfo *pInfo);
extern uint8_t DBReadValue(uint16_t *pAddr, uint32_t num, uint8_t *pData, uint8_t sn, uint32_t *length);

extern void DBReadSystemTime(struct CP56Time2a_t *pTime);
extern void DBWriteSystemTime(struct CP56Time2a_t *pTime);

extern rt_uint8_t DBWriteSOE(uint16_t addr, rt_uint8_t state);
extern rt_uint8_t DBWriteCO(uint16_t addr, rt_uint8_t state);
extern rt_uint8_t DBWriteFEVENT(rt_uint16_t yx_addr, rt_uint16_t *yc_addr, rt_uint16_t yc_num);


extern uint8_t DB_NVA_Check(void);
extern void DBClearNVA(void);
extern float SelectRatio(rt_uint8_t ratioType, float ratioValue);  
extern void CalibrationFactorCal(uint8_t num);

extern int FloatToBin(float fl);
extern float BinToFloat(int bin);
extern uint8_t DecimalToBCD(uint8_t dec);
extern uint8_t BCDToDecimal(uint8_t bcd);
#endif 

/* END OF FILE ---------------------------------------------------------------*/

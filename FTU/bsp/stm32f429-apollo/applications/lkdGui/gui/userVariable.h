#ifndef  __USERVARIABLE_H
#define  __USERVARIABLE_H

#include "stdint.h"
#include "point_table_config.h"

/* 遥信显示结构体 */
typedef struct{
	uint8_t *pBuff;	/* 保存遥信索引号 */
	uint8_t Num;		/* 索引号总数 */
	struct tagTelesignalCfg *pRoot; /* 遥信实体指针 */
}YaoxinDisplayInfo;

extern YaoxinDisplayInfo yxInfo;

void YaoxinDisplayInit(void);



/* 遥信 */
struct YaoXinItem{
	uint16_t addr;
	uint8_t *name;
	uint8_t *status[2];
};
/* SOE */
struct SOEItem{
	uint32_t addr;
	uint8_t *name;
	uint8_t *status[2];
};
struct YaoCeItem{
	uint16_t addr;
	uint8_t	valueWide;
    uint8_t valueDecimal;
	uint8_t *name;
	uint8_t *unit[2];
};

struct DingZhi1Item{
	uint16_t addr;
	uint8_t *name;
	uint8_t	flag;//0 浮点数 1汉字
	uint8_t *max;
	uint8_t *min;
	uint8_t	valueWide;
    uint8_t valueDecimal;
	uint8_t *unit[2];
	uint8_t *pContent[8];//汉字
};

struct ConstParameter{
	uint16_t addr;
	uint8_t *name;
};

struct CmdSend{
	/* 遥控命令位于modbuscmd.c中，M_mapNum为相应结构体的item号 */
	uint8_t M_mapNum;
	uint8_t *name;
};

struct PassWordMessage{
	uint8_t passWord[8];
	uint8_t *name;
};

#if (USEING_TAB == USE_TAB_ONE)

/* 遥信 */
//#define YAOXIN_NUM 24
#define YAOXIN_START_ADDR 0x0001

/* 遥测 */
#define YAOCE1_NUM 20
#define YAOCE2_NUM 20
#define YAOCE_NUM  34//遥测总数
#define YAOCE_START_ADDR 0x4001
#define HARMONIC_NUM 16//谐波总数
#define HARMONIC_START_ADDR (YAOCE_START_ADDR+YAOCE_NUM)

/* 定值 */

/* 固有参数地址 */
#define ConstParameter_NUM 11
#define ConstParameter_START_ADDR	(0x8001)
/* 基本设置 */
#define BasicConfigure_NUM 21
#define BasicConfigure_START_ADDR	(0x8101)
/* 零漂设置 */
#define ZeroDrift_NUM 18
#define ZeroDrift_START_ADDR	(BasicConfigure_START_ADDR+BasicConfigure_NUM)
/* 死区设置 */
#define DeadZone_NUM 18
#define DeadZone_START_ADDR	(ZeroDrift_START_ADDR+ZeroDrift_NUM)
/* 相间过流 */
#define InterphaseOver_NUM 12
#define InterphaseOver_START_ADDR	0x8301
/* 零序过流 */
#define ZeroSequenceOver_NUM 6
#define ZeroSequenceOver_START_ADDR	(InterphaseOver_START_ADDR+InterphaseOver_NUM)
/* 越限报警 */
#define ThresholdAlarm_NUM 26
#define ThresholdAlarm_START_ADDR	(ZeroSequenceOver_START_ADDR+ZeroSequenceOver_NUM)
#define START_TO_ZERO_NUM 14//起始地址到零序定值的数目
#define Threshold_ZERO_START_ADDR (ThresholdAlarm_START_ADDR+START_TO_ZERO_NUM)
#define ZERO_TO_DC_NUM 6
#define Threshold_DC_START_ADDR (Threshold_ZERO_START_ADDR+ZERO_TO_DC_NUM)
/* 重过载 */
#define OverloadMuch_NUM 4
#define OverloadMuch_START_ADDR	(ThresholdAlarm_START_ADDR + ThresholdAlarm_NUM)
/* 过负荷 */
#define Overload_NUM 4
#define Overload_START_ADDR	(OverloadMuch_START_ADDR + OverloadMuch_NUM)
/* 电池设置 */
#define BatterySet_NUM 9
#define BatterySet_START_ADDR	(Overload_START_ADDR+Overload_NUM)
/* 自动复归 */
#define AutoReset_NUM 2
#define AutoReset_START_ADDR	(BatterySet_START_ADDR+BatterySet_NUM)
/* 过电压 */
#define OverVoltage_NUM 16
#define OverVoltage_START_ADDR	(AutoReset_START_ADDR+AutoReset_NUM)
/* 合环功能 */
#define RingUnite_NUM 3
#define RingUnite_START_ADDR	(OverVoltage_START_ADDR+OverVoltage_NUM)
/* 故障总 */
#define BreakDown_NUM 1
#define BreakDown_START_ADDR	(RingUnite_START_ADDR+RingUnite_NUM)
/* 常规保护 */
#define CutoutSwitch_NUM 18
#define CutoutSwitch_START_ADDR	0x8401
/* 逻辑功能 */
#define LoadSwitch_NUM 18
#define LoadSwitch_START_ADDR	0x8501
/*----------------SOE---------------------*/
#define SOE_NUM 113//SOE总数
#define SOE_START_ADDR YAOXIN_START_ADDR//SOE起始地址
#define START_TO_SHORTBD_NUM 12//开始地址到短路故障的数目
#define START_TO_SHORTBD_ADDR (SOE_START_ADDR+START_TO_SHORTBD_NUM)//SOE过流保护地址
#define START_TO_AOVERP_NUM 15//短路故障到A相过流一段保护的数目
#define START_TO_AOVERP_ADDR (START_TO_SHORTBD_ADDR+START_TO_AOVERP_NUM)//A相过流一段保护的地址
#define START_TO_POSTACCELE_NUM 11//相过流一段保护到A相后加速过流的数目
#define START_TO_POSTACCELE_ADDR (START_TO_AOVERP_ADDR+START_TO_POSTACCELE_NUM)//A相后加速过流
#define START_TO_TWOCLOSE_NUM 13//A相后加速过流到二次重合闸的数目
#define START_TO_TWOCLOSE_ADDR (START_TO_POSTACCELE_ADDR+START_TO_TWOCLOSE_NUM)//二次重合闸地址
#define START_TO_VLIMIT_A_NUM 17//二次重合闸到A相电压越线的数目
#define START_TO_VLIMIT_A_ADDR (START_TO_TWOCLOSE_ADDR+START_TO_VLIMIT_A_NUM)//二次重合闸地址

/*----------------遥控---------------------*/
#define CmdSend_NUM	4

#define CS_OPENSWITCH_NUM	0
#define CS_CLOSESWITCH_NUM	0
#define CS_RESETKEY_NUM		0
#define CS_SOECLEAR_MENUCMD_NUM	0

/*-------------密码管理--------------------*/
#define PassWord_NUM	1

/*=========================================*/
#elif (USEING_TAB == USE_TAB_TWO)

/* 遥信 */
#define YAOXIN_NUM 27
#define YAOXIN_START_ADDR 0x0001

/* 遥测 */
#define YAOCE1_NUM 19
#define YAOCE2_NUM 19
#define YAOCE_NUM  33//遥测总数
#define YAOCE_START_ADDR 0x4001
#define HARMONIC_NUM 16//谐波总数
#define HARMONIC_START_ADDR (YAOCE_START_ADDR+YAOCE_NUM)

/* 定值*/

/* 固有参数地址 */
#define ConstParameter_NUM 10
#define ConstParameter_START_ADDR	(0x8001)
/* 基本设置 */
#define BasicConfigure_NUM 15
#define BasicConfigure_START_ADDR	(0x8101)
/* 零漂设置 */
#define ZeroDrift_NUM 18
#define ZeroDrift_START_ADDR	(BasicConfigure_START_ADDR+BasicConfigure_NUM)
/* 死区设置 */
#define DeadZone_NUM 18
#define DeadZone_START_ADDR	(ZeroDrift_START_ADDR+ZeroDrift_NUM)
/* 相间过流 */
#define InterphaseOver_NUM 12
#define InterphaseOver_START_ADDR	0x8301
/* 零序过流 */
#define ZeroSequenceOver_NUM 6
#define ZeroSequenceOver_START_ADDR	(InterphaseOver_START_ADDR+InterphaseOver_NUM)
/* 越限报警 */
#define ThresholdAlarm_NUM 26
#define ThresholdAlarm_START_ADDR	(ZeroSequenceOver_START_ADDR+ZeroSequenceOver_NUM)
#define START_TO_ZERO_NUM 14//起始地址到零序定值的数目
#define Threshold_ZERO_START_ADDR (ThresholdAlarm_START_ADDR+START_TO_ZERO_NUM)
#define ZERO_TO_DC_NUM 6
#define Threshold_DC_START_ADDR (Threshold_ZERO_START_ADDR+ZERO_TO_DC_NUM)
/* 重过载 */
#define OverloadMuch_NUM 4
#define OverloadMuch_START_ADDR	(ThresholdAlarm_START_ADDR + ThresholdAlarm_NUM)
/* 过负荷 */
#define Overload_NUM 4
#define Overload_START_ADDR	(OverloadMuch_START_ADDR + OverloadMuch_NUM)
/* 电池设置 */
#define BatterySet_NUM 9
#define BatterySet_START_ADDR	(Overload_START_ADDR+Overload_NUM)
/* 自动复归 */
#define AutoReset_NUM 2
#define AutoReset_START_ADDR	(BatterySet_START_ADDR+BatterySet_NUM)
/* 过电压 */
#define OverVoltage_NUM 4
#define OverVoltage_START_ADDR	(AutoReset_START_ADDR+AutoReset_NUM)
/* 合环功能 */
#define RingUnite_NUM 3
#define RingUnite_START_ADDR	(OverVoltage_START_ADDR+OverVoltage_NUM)
/* 故障总 */
#define BreakDown_NUM 1
#define BreakDown_START_ADDR	(RingUnite_START_ADDR+RingUnite_NUM)
/* 常规保护 */
#define CutoutSwitch_NUM 16
#define CutoutSwitch_START_ADDR	0x8401
/* 逻辑功能 */
#define LoadSwitch_NUM 25
#define LoadSwitch_START_ADDR	0x8501
/*----------------SOE---------------------*/
#define SOE_NUM 110//SOE总数
#define SOE_START_ADDR YAOXIN_START_ADDR//SOE起始地址
#define START_TO_SHORTBD_NUM 12//开始地址到短路故障的数目
#define START_TO_SHORTBD_ADDR (SOE_START_ADDR+START_TO_SHORTBD_NUM)//SOE过流保护地址
#define START_TO_AOVERP_NUM 15//短路故障到A相过流一段保护的数目
#define START_TO_AOVERP_ADDR (START_TO_SHORTBD_ADDR+START_TO_AOVERP_NUM)//A相过流一段保护的地址
#define START_TO_POSTACCELE_NUM 11//相过流一段保护到A相后加速过流的数目
#define START_TO_POSTACCELE_ADDR (START_TO_AOVERP_ADDR+START_TO_POSTACCELE_NUM)//A相后加速过流
#define START_TO_TWOCLOSE_NUM 13//A相后加速过流到二次重合闸的数目
#define START_TO_TWOCLOSE_ADDR (START_TO_POSTACCELE_ADDR+START_TO_TWOCLOSE_NUM)//二次重合闸地址
#define START_TO_VLIMIT_A_NUM 14//二次重合闸到A相电压越线的数目
#define START_TO_VLIMIT_A_ADDR (START_TO_TWOCLOSE_ADDR+START_TO_VLIMIT_A_NUM)//二次重合闸地址

/*----------------遥控---------------------*/
#define CmdSend_NUM	4

#define CS_OPENSWITCH_NUM	OPENSWITCH_NUM
#define CS_CLOSESWITCH_NUM	CLOSESWITCH_NUM
#define CS_RESETKEY_NUM		RESETKEY_NUM
#define CS_SOECLEAR_MENUCMD_NUM	SOECLEAR_MENUCMD_NUM

/*-------------密码管理--------------------*/
#define PassWord_NUM	1

#endif

extern const struct YaoXinItem yaoXinItems[12];
extern const struct YaoCeItem yaoCe1Items[YAOCE1_NUM];
extern const struct YaoCeItem yaoCe2Items[YAOCE2_NUM];
extern const struct YaoCeItem yaoCe3Items[HARMONIC_NUM];

/* 负荷开关-逻辑功能 */
extern const struct DingZhi1Item LoadSwitchItems[LoadSwitch_NUM];
/* 越线报警 */
extern const struct DingZhi1Item ThresholdAlarmItems[ThresholdAlarm_NUM];
/* 重过载 */
extern const struct DingZhi1Item OverloadMuchItems[OverloadMuch_NUM];
/* 过负荷 */
extern const struct DingZhi1Item OverloadItems[Overload_NUM];
/* 过电压 */
extern const struct DingZhi1Item OverVoltageItems[OverVoltage_NUM];
/* 电池设置 */
extern const struct DingZhi1Item BatterySetItems[BatterySet_NUM];
/* 自动复归 */
extern const struct DingZhi1Item AutoResetItems[AutoReset_NUM];
/* 相间过流 */
extern const struct DingZhi1Item InterphaseOverItems[InterphaseOver_NUM - 3];
/* 零序过流 */
extern const struct DingZhi1Item ZeroSequenceOverItems[ZeroSequenceOver_NUM];
/*断路器设置 */
extern const struct DingZhi1Item CutoutSwitchItems[CutoutSwitch_NUM];
/* 死区设置 */
extern const struct DingZhi1Item DeadZoneItems[DeadZone_NUM];
/* 零漂设置 */
extern const struct DingZhi1Item ZeroDriftItems[ZeroDrift_NUM];
/* 基本设置 */
extern const struct DingZhi1Item BasicConfigureItems[BasicConfigure_NUM];
/* SOE */
extern const struct SOEItem SOEItems[SOE_NUM];
/* 固有参数 */
extern const struct ConstParameter ConstParItems[ConstParameter_NUM];
/* 命令下发 */
extern const struct CmdSend CmdSendItems[CmdSend_NUM];
/* 密码管理 */
extern const struct PassWordMessage PassWordItems[PassWord_NUM];
/* 合环 */
extern const struct DingZhi1Item RingUniteItems[RingUnite_NUM];
/* 故障总 */
extern const struct DingZhi1Item BreakDownItems[BreakDown_NUM];

extern const uint8_t *HMiVersion;
#endif

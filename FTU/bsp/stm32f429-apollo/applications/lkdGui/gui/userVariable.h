#ifndef  __USERVARIABLE_H
#define  __USERVARIABLE_H

#include "stdint.h"
#include "point_table_config.h"

/* 时间显示 */
typedef struct {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t s;
	uint16_t ms;
}SystemTimeDisplay;

/* 遥信显示结构体 */
typedef struct{
	uint8_t *pBuff;	/* 保存遥信索引号 */
	uint8_t Num;		/* 索引号总数 */
	struct tagTelesignalCfg *pRoot; /* 遥信实体指针 */
}YaoxinDisplayInfo;

/* 遥测显示结构体 */
typedef struct{
	uint8_t *pBuff;	/* 保存遥测索引号 */
	uint8_t num;		/* 索引号总数 */
	struct tagTelemetryCfg *pRoot; /* 遥测实体指针 */
}YaoceDisplayInfo;

/* 定值显示结构体 */
typedef struct{
	uint8_t *pBuff;	/* 保存定值索引号 */
	uint8_t num;		/* 索引号总数 */
	void(* SaveModify)(uint16_t addr);/* 定值修改保存指针函数 */
	struct tagValueParaCfg *pRoot; /* 定值实体指针 */
}DzhiDisplayInfo;

/* Soe显示结构体 */
typedef struct{
	uint8_t highlight;
	uint8_t *pName;
	uint8_t *pContent;
	SystemTimeDisplay time;
}SoeContent;

/* Soe显示结构体 */
typedef struct{
	uint16_t num;		/* soe总数 */
	uint16_t in;
	uint16_t (* GetNum)(void);
	uint8_t (* CheckUpdata)(void);
	uint8_t (* GetNoContent)(uint16_t soeNo,SoeContent *pSoe);
}SoeDisplayInfo;

/* 故障事件显示结构体 */
typedef struct{
	uint8_t *pName;
	uint8_t *pContent;
	uint8_t yaoceNum;
	uint8_t *pYaoceName[16];
	uint8_t *pYaoceUnit[16];
	float pYaoceValue[16];
	SystemTimeDisplay time;
}FeventContent;

/* 故障事件显示结构体 */
typedef struct{
	uint16_t num;		/* 故障事件总数 */
	uint16_t in;
}FeventDisplayInfo;

enum Dzhi0OffsetNumber{
	DZ0_CONFIG,/* 配置 */
	DZ0_ZERODRIFT,/* 零漂 */
	DZ0_DEADEZONE,/* 死区 */
	DZ0_SERIAL,		/* 串口 */
	DZ0_INTERNET,	/* 网口 */
	DZ0_ALLNUM,/* 定值总数 */
};
enum Dzhi1OffsetNumber{
	DZ1_INTERGHASE,		/* 相间过流 */
	DZ1_ZERO_SEQUE,		/* 零序过流 */
	DZ1_LIMITATION,		/* 越线告警 */
	DZ1_HEAVY_LOAD,		/* 重过载 */
	DZ1_OVERLOAD,			/* 过负荷 */
	DZ1_IBATTERY_SET,	/* 电池设置 */
	DZ1_AUTO_RESET,		/* 自动复归 */
	DZ1_LIMIT_V_F,		/* 越压越频 */
	DZ1_LOOP_CLOSE,		/* 合环 */
	DZ1_FAULT_SWITCH,	/* 故障投退 */
	DZ1_OTHER_PROTEC,	/* 其他保护 */
	DZ1_LOGICAL_FUN,	/* 逻辑功能 */
	DZ1_ALLNUM,				/* 定值总数 */
};

/* 命令下发内容 */
struct HmiCmdSend{
	uint8_t *name;
	uint8_t cmd;
};
/* 命令下发信息 */
typedef struct{
	uint8_t itemsNum;
	struct HmiCmdSend *pHmiCmd;
	void (* cmdfun)(uint8_t cmdIs);
}HmiCmdSendInfo;

/* 版本信息 */
typedef struct{
	uint8_t num;
	struct tagInherentParaCfg *pItems;
}VersionInfo;

/* 密码管理 */
struct PassWordMessage{
	uint8_t passWord[8];
	uint8_t *name;
};

/* 遥信信息结构 */
extern YaoxinDisplayInfo yxInfo;
/* 遥测信息结构 */
extern YaoceDisplayInfo yceInfo[3];
/* 定值信息结构 */
extern DzhiDisplayInfo dzhi0Info[DZ0_ALLNUM];
extern DzhiDisplayInfo dzhi1Info[DZ1_ALLNUM];
/* Soe显示信息 */
extern SoeDisplayInfo soeInfo;
/* Co显示信息 */
extern SoeDisplayInfo coInfo;
/* 命令下发结构体 */
extern HmiCmdSendInfo hcmdInfo;
/* 版本信息结构体 */
extern VersionInfo versInfo;
/* 密码管理 */
#define PassWord_NUM 1
extern const struct PassWordMessage PassWordItems[PassWord_NUM];

/* 获取显示时间 */
void GetDisplayTime(SystemTimeDisplay *tim);
/* 显示信息映射初始化 */
void userVariableDisplayInit(void);
/* 获取SOE内容 */
uint8_t GetSoeNoContent(uint16_t soeNo,SoeContent *pSoe);
/* 检测SOE有无更新 */
uint8_t CheckSoeUpdata(void);
/* 获取SOE数量 */
uint16_t GetSoeNum(void);
/* 获取CO数量 */
uint16_t GetCoNum(void);
/* 获取故障事件数量 */
uint16_t GetFeventNum(void);
/* 获取故障事件内容 */
uint8_t GetFeventNoContent(uint16_t feventNo,FeventContent *pEvent);
/* 检测故障事件有无更新 */
uint8_t CheckFeventUpdata(void);
/* 是否更新程序检测 */
uint8_t CheckUpdataProgram(void);

#endif

/* END */

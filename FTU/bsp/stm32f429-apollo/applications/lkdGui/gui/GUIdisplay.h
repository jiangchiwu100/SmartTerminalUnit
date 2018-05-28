/**
  * @file   GUIdisplay.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    03-06-2018
  * @brief   GUI显示
  */

#ifndef __GUIDISPLAY_H
#define __GUIDISPLAY_H

#include "lkdGuiMenu.h"
#include "gui_101_cmd.h"
#include "lkdGuiWindow.h"
#include "userVariable.h"

#define DZDISPLAYCOL 3		/* 定值显示列数 */
#define YaoCeDISPLAYCOL 3	/* 遥测显示列数 */
#define DISPLAYLISTROW 10	/* 可显示行数 */
#define SOEDISPLAYROW 5		/* SOE显示行数 */
#define EVENTDISPLAYROW 1	/* 操作事件显示行数 */
#define CONSTPARDISPLAYROW 5/* 固有参数显示行数 */

#define YAOCE_UPDATATIME 1000 /* 遥测更新显示时间 1s */

/* Lcd开关显示控制 */
#define LCDDISPLAYON	1
#define LCDDISPLAYOFF	0
#define LCDDISPLAYOFFTIME	(1000*60)/* ms */

/* 返回状态 */
enum ReturnState{
	ISFAILTURE,	/* 失败 */
	ISTURE,		/* 成功 */
	ISWAITING,	/* 等待 */
	ISOVERTIME	/* 超时 */
};

/* 设置定值方法 */
enum DingZhiSetOrSee{
	NULL_DINGZHI,	/* NULL */
	SET_DINGZHI,	/* 修改定值 */
	SEE_DINGZHI,	/* 浏览定值 */
};

/* 消息窗口消息 */
struct Message{
	uint8_t *str;	/* 提示语 */
	uint8_t *flag;	/* 返回标志 */
};

/* 密码窗口消息 */
struct PassWordPipe{
	uint8_t passWordType; 	/* 密码类型 */
	uint8_t *returnFlag;	/* 返回标志 */
};

/* SOE显示 */
struct SOEDisplay{
	uint16_t allNum;
	int16_t pRead;
	uint8_t itemNum[4];	/* 当前条数 */
	uint8_t time[24];	/* 时间 */
	SoeContent pSoe;
};

/* 故障事件显示 */
struct EventDisplay{
	uint8_t pRead;
	uint8_t allNum;
	uint8_t itemNum[4];	/* 当前条数 */
	uint8_t time[24];		/* 时间 */
	uint8_t yaoceValue[16];/* 遥测值转化为字符串 */
	FeventContent pFevent;
};

/* 定值修改 */
struct DZModfiy{
	const DzhiDisplayInfo *info;	/* 定值结构体 */
	uint8_t flag;	/* 标志 */
	uint8_t itemIs;	/* 当前行 */
	uint8_t *str;	/* 要修改的字符串 */
};

enum RunStepType{
	STEP_NORMAL,
	STEP_UPDATAPRO,
	STEP_PASSWORD,
	STEP_MESSAGE,
	STEP_DZHIMODFIY,
	STEP_HOMEWIN,
	STEP_MAINMENU,
	STEP_ALLNUM,
};
/* 首页界面窗口 */
static void HomeWindowFun(void);
/* 菜单窗口 */
static void MenuWindowFun(void);
/*M 主菜单 */
static void mianMenuFun(void);
/*M0 信息查询 */
static void MenuM0Fun(void);
	static void YaoxinFun(void);		//遥信
	static void MenuM0S1Fun(void);		//遥测
		static void YaoCe1Fun(void);	//一次
		static void YaoCe2Fun(void);	//二次
		static void HarmonicFun(void);	//谐波
	static void SOEFun(void);			//SOE
	static void FaultEventFun(void);	//故障事件
	static void MenuM0S4Fun(void);		//定值查询 
		static void MenuM0S4S0Fun(void);
	static void MenuM0S5Fun(void);		//配置查询
/* M1 定值设置  */
static void MenuM1Fun(void);
	static void MenuM1S0Fun(void);		//保护功能
		static void InterphaseOverFun(void);
		static void ZeroSequenceOverFun(void);
		static void CutoutSwitchFun(void);
	static void LogicalFunFun(void);	//逻辑功能
	static void OverLineWarnFun(void);	//越线报警
	static void OverLoadMuchFun(void);	//重过载
	static void OverLoadFun(void);		//过负荷
	static void OverVoltageFun(void);	//过电压
	static void BatterySetFun(void);	//电池设置
	static void AutoResetFun(void);		//自动复归
	static void MenuM1S8Fun(void);		//其他
		static void RingUniteFun(void); //合环
		static void BreakDownFun(void); //故障投退
/* M2 配置设置  */	
static void MenuM2Fun(void);	
	static void BasicSetFun(void);		//基本设置
	static void ZeroDriftFun(void);		//零漂设置
	static void DeadZoneFun(void);		//死区设置
/* 命令下发  */
static void CmdSendWinFun(void);
/* 版本信息 */	
static void VersionWinFun(void);
/* 定值修改窗口函数 */
void DZModfiyFun(void);
/* 消息窗口 */
void MessageFun(void);
/* 密码窗口 */
void PassWordFun(void);

/* 启动界面画图 */
void GUIStartInterface(void);
/* GUI主函数 */
void GUIDisplayMian(void);
/* GUI初始化 */
void GUIDisplayInit(void);

#endif /* __GUIDISPLAY_H */

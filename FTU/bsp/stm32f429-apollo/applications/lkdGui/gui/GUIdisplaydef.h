#ifndef  __GUIDISPLAYDEF_H
#define  __GUIDISPLAYDEF_H

#include "lkdGuiMenu.h"
#include "gui_101_cmd.h"
#include "lkdGuiWindow.h"

MENU mianMenu = {
	0,0,NULL,0,5,{1,1,1,0,0},//x,y,fatherMenu,currentItem,mun,ItemFlag
	{"信息查询","定值设置","配置设置","命令下发","版本信息",NULL,NULL,NULL,NULL,NULL},
	mianMenuFun/* MenuFunction */
};
MENU MenuM0 = {
	0,0,&mianMenu,0,6,{0,1,0,0,1,1},//x,y,fatherMenu,currentItem,mun
	{"遥信查询","遥测查询"," SOE查询","操作记录","故障事件","定值查询","配置查询",NULL,NULL,NULL},
	MenuM0Fun/* itemFunction */	
};
MENU MenuM1 = {
	0,0,&mianMenu,0,9,{1,0,0,0,0,0,0,0,1},//x,y,fatherMenu,currentItem,mun
	{"保护功能","逻辑功能","越限报警","重 过 载","过 负 荷","越压越频","电池设置","自动复归","其他设置",NULL},
	MenuM1Fun/* itemFunction */
};
MENU MenuM2 = {
	0,0,&mianMenu,0,5,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"基本设置","零漂设置","死区设置","串口设置","网口设置",NULL,NULL,NULL,NULL,NULL},
	MenuM2Fun/* itemFunction */
};
MENU MenuM0S1 = {
	0,0,&MenuM0,0,3,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"一次遥测","二次遥测","谐波分量",NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM0S1Fun/* itemFunction */
};
MENU MenuM0S4 = {
	0,0,&MenuM0,0,9,{1,0,0,0,0,0,0,0,1},//x,y,fatherMenu,currentItem,mun
	{"保护功能","逻辑功能","越限报警","重 过 载","过 负 荷","越压越频","电池设置","自动复归","其他查询",NULL},
	MenuM0S4Fun/* itemFunction */
};
MENU MenuM0S5 = {
	0,0,&MenuM0,0,5,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"基本查询","零漂查询","死区查询","串口查询","网口查询",NULL,NULL,NULL,NULL,NULL},
	MenuM0S5Fun/* itemFunction */
};
MENU MenuM0S4S0 = {
	0,0,&MenuM0S4,0,3,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"相间过流","零序过流","其他保护",NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM0S4S0Fun/* itemFunction */
};
MENU MenuM0S4S8 = {
	0,0,&MenuM0S4,0,2,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"合    环","故障投退",NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM1S8Fun/* itemFunction */
};
MENU MenuM1S0 = {
	0,0,&MenuM1,0,3,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"相间过流","零序过流","其他保护",NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM1S0Fun/* itemFunction */
};

MENU MenuM1S8 = {
	0,0,&MenuM1,0,2,{0,0,0},//x,y,fatherMenu,currentItem,mun
	{"合    环","故障投退",NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	MenuM1S8Fun/* itemFunction */
};

WINDOW TimeModfiyWin  = {0,0,160,160,NULL,"时间修改窗口",TimeModfiyFun};
WINDOW HomeWindow = {0,0,160,160,NULL,NULL,HomeWindowFun};
WINDOW MenuWindow = {0,0,160,160,NULL,"菜单管理窗口",MenuWindowFun};
WINDOW CmdSendWin = {0,0,160,160,NULL,"命令下发窗口",CmdSendWinFun};
WINDOW VersionWin = {0,0,160,160,NULL,"版本信息管理",VersionWinFun};

///* 信息查询菜单子窗口 */
WINDOW YaoxinWin     = {0,0,160,160,NULL,"遥信状态窗口",YaoxinFun};
	WINDOW YaoCe1Win   = {0,0,160,160,NULL,"一次遥测窗口",YaoCe1Fun};
	WINDOW YaoCe2Win   = {0,0,160,160,NULL,"二次遥测窗口",YaoCe2Fun};
	WINDOW HarmonicWin = {0,0,160,160,NULL,"谐波分量窗口",HarmonicFun};
WINDOW SOEWin   	 = {0,0,160,160,NULL,"SOE记录窗口",SOEFun};
WINDOW COWin   	 = {0,0,160,160,NULL,"操作记录窗口",COFun};
WINDOW FaultEventWin = {0,0,160,160,NULL,"故障事件窗口",FaultEventFun};

/* 信息查询->定值查询菜单子窗口 */

/* 定值设置菜单子窗口 */
	WINDOW InterphaseOverWin   = {0,0,160,160,NULL,"相间过流窗口",InterphaseOverFun};
	WINDOW ZeroSequenceOverWin = {0,0,160,160,NULL,"零序过流窗口",ZeroSequenceOverFun};
	WINDOW CutoutSwitchWin     = {0,0,160,160,NULL,"其它保护窗口",  CutoutSwitchFun};
WINDOW LogicalFunWin   = {0,0,160,160,NULL,"逻辑功能窗口",LogicalFunFun};
WINDOW OverLineWarnWin = {0,0,160,160,NULL,"越限报警窗口",OverLineWarnFun};
WINDOW OverLoadMuchWin = {0,0,160,160,NULL,"重过载窗口",OverLoadMuchFun};
WINDOW OverLoadWin     = {0,0,160,160,NULL,"过负荷窗口",OverLoadFun};
WINDOW OverVoltageWin  = {0,0,160,160,NULL,"过电压窗口",OverVoltageFun};
WINDOW BatterySetWin   = {0,0,160,160,NULL,"电池管理窗口",BatterySetFun};
WINDOW AutoResetWin    = {0,0,160,160,NULL,"自动复归窗口",AutoResetFun};
	WINDOW RingUniteWin        = {0,0,160,160,NULL,"合环设置窗口",RingUniteFun};
	WINDOW BreakDownWin        = {0,0,160,160,NULL,"故障投退窗口",BreakDownFun};
/* 配置设置 */
WINDOW BasicSetWin   = {0,0,160,160,NULL,"基本设置窗口",BasicSetFun};
WINDOW ZeroDriftWin  = {0,0,160,160,NULL,"零漂设置窗口",ZeroDriftFun};
WINDOW DeadZoneWin 	 = {0,0,160,160,NULL,"死区设置窗口",DeadZoneFun};
WINDOW SerialWin  = {0,0,160,160,NULL,"串口设置窗口",SerialFun};
WINDOW InternetWin 	 = {0,0,160,160,NULL,"网口设置窗口",InternetFun};

WINDOW DZModfiyWin = {14,19,130,134,NULL,"定值修改窗口",DZModfiyFun};
/* 公用窗口 */
WINDOW MessageWin = {23,40,110,75,NULL,"消息窗口",MessageFun};
WINDOW PassWordWin = {23,40,110,75,NULL,"密码窗口",PassWordFun};

#endif

/**
  * @file   hmiInout.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/18
  * @brief  hmi按键开关等处理
  */

#ifndef __HMIINOUT_H
#define	__HMIINOUT_H

#include "stdint.h"

/* 按键状态 */
#define USERKEY_DOWN	1
#define USERKEY_UP 		0

/* 按键映射 */
enum UserKeyNomberMap{
	/* 菜单按键映射 */
	MENUKEY_UP = 0,
	MENUKEY_LEFT = 1,
	MENUKEY_DOWN = 2,
	MENUKEY_RIGHT = 3,
	MENUKEY_ESC = 4,
	MENUKEY_OK = 5,
	/* 遥控按键映射 */
	YK_RESET = 6,
	YK_CLOSESWITCH = 7,
	YK_SHFITSWITCH = 8,
	YK_OPENSWITCH = 9,
	/* 开关映射 */
	SW_REMORE = 50,
	SW_LOCAL = 51,
	SW_RECLOSE = 52,
	SW_PROTECT = 53,
	SW_CONTACT = 54,
	SW_SECTION = 55,
};
/* 按键枚举 */
enum KeyStatus{
	NoKey,
	UpKey,
	DownKey,
	LeftKey,
	RightKey,
	OkKey,
	CancelKey
};
/* 菜单按键状态 */
struct MenuKeyValue{
	uint8_t up:1;
	uint8_t down:1;
	uint8_t left:1;
	uint8_t right:1;
	uint8_t ok:1;
	uint8_t esc:1;
	enum KeyStatus keyIs;
};
/* 遥控按键状态 */
struct YKKeyValue{
	uint8_t open:1;
	uint8_t close:1;
	uint8_t shift:1;
	uint8_t reset:1;
	enum KeyStatus keyIs;
};

/* 开出映射 */
enum UserOutputNoMap{
	/* LED 映射 */
	LED_BEGIN_NO = 2,
	LED_END_NO = 50,
};

/* Led使用定义 */
enum UserLedDefine{
	ULED_NOENERGY,		/* 未储能 */
	ULED_SWITCHOPEN,	/* 开关分位 */
	ULED_LowPa,				/* 低气压 */
	ULED_SWITCHCLOSE,	/* 开关合位 */
	ULED_CLOSELOCK,		/* 合闸闭锁 */
	ULED_OVERALARM,		/* 过流告警 */
	ULED_OPENLOCK,		/* 分闸闭锁 */
	ULED_ZEROALARM,		/* 零序告警 */
	ULED_DEVICEFAULT,	/* 装置故障 */
	ULED_SELFCHECK,		/* 自检异常 */
	ULED_COMMUN,			/* 通信 */
	ULED_RECLOSELOCK,	/* 重合闭锁 */
	ULED_LS,					/* 负荷开关 */
	ULED_CB,					/* 断路器 */
	ULED_ALLNUM
};
enum UserLedState{
	ULED_OFF,
	ULED_ON,
};

enum KeyStatus GetKeyStatus(void);
void SetKeyIsNoKey(void);
void KeyCmdResult(uint8_t keyNo, uint8_t state);
void LedChangeCheck(void);
void HmiInOutInit(void);

extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);
#endif /* __HMIINOUT_H */

/* END */

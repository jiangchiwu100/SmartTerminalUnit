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

/* 菜单按键映射 */
enum UserMenuKey{
	MENUKEY_UP = 0,
	MENUKEY_LEFT = 1,
	MENUKEY_DOWN = 2,
	MENUKEY_RIGHT = 3,
	MENUKEY_ESC = 4,
	MENUKEY_OK = 5,
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

enum KeyStatus GetKeyStatus(void);
void SetKeyIsNoKey(void);
void KeyCmdResult(uint8_t keyNo, uint8_t state);
#endif /* __HMIINOUT_H */

/* END */

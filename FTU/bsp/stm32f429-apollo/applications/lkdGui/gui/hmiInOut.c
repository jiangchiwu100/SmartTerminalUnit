/**
  * @file   hmiInout.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/18
  * @brief  hmi按键开关等处理
  */
#include "hmiInOut.h"

struct MenuKeyValue menuKey;
	
void MenuKeyResult(uint8_t keyNo, uint8_t state)
{
	if(state == USERKEY_DOWN){
		switch(keyNo){
			case MENUKEY_UP:
				menuKey.up = 1;menuKey.keyIs = UpKey;break;
			case MENUKEY_DOWN:
				menuKey.down = 1;menuKey.keyIs = DownKey;break;
			case MENUKEY_LEFT:
				menuKey.left = 1;menuKey.keyIs = LeftKey;break;
			case MENUKEY_RIGHT:
				menuKey.right = 1;menuKey.keyIs = RightKey;break;
			case MENUKEY_ESC:
				menuKey.esc = 1;menuKey.keyIs = CancelKey;break;
			case MENUKEY_OK:
				menuKey.ok = 1;menuKey.keyIs = OkKey;break;
			default:break;
		}
	}
	else{
		switch(keyNo){
			case MENUKEY_UP:
				menuKey.up = 0;break;
			case MENUKEY_DOWN:
				menuKey.down = 0;break;
			case MENUKEY_LEFT:
				menuKey.left = 0;;break;
			case MENUKEY_RIGHT:
				menuKey.right = 0;break;
			case MENUKEY_ESC:
				menuKey.esc = 0;break;
			case MENUKEY_OK:
				menuKey.ok = 0;break;
			default:break;
		}
	}
}
enum KeyStatus GetKeyStatus(void)
{
	return menuKey.keyIs;
}
void SetKeyIsNoKey(void)
{
	menuKey.keyIs = NoKey;
}

void KeyCmdResult(uint8_t keyNo, uint8_t state)
{
	switch(keyNo){
		case MENUKEY_UP:
		case MENUKEY_DOWN:
		case MENUKEY_LEFT:
		case MENUKEY_RIGHT:
		case MENUKEY_ESC:
		case MENUKEY_OK:MenuKeyResult(keyNo,state);break;
	}
}

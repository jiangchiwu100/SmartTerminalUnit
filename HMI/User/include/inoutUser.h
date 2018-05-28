/**
  * @file    inOutUser.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/17
  * @brief   
  */
  
#ifndef _INOUTUSER_H_
#define _INOUTUSER_H_

#include "stdint.h"

/* -- 按键映射定义 -- */

/* -- 开关映射定义 -- */

/* -- LED映射定义 -- */
#define USERLED_NUMALL 10	/* 用户使用LED总数 */
#define USERLED_NO0 0			/* LED映射 0~n 为实际led编码 */
#define USERLED_NO1 1
#define USERLED_NO2 2
#define USERLED_NO3 3
#define USERLED_NO4 4
#define USERLED_NO5 5
#define USERLED_NO6 6
#define USERLED_NO7 7
#define USERLED_NO8 8
#define USERLED_NO9 9
#define USERLED_NO10 10
#define USERLED_NO11 11
#define USERLED_NO12 12
#define USERLED_NO13 13
#define USERLED_NO14 14
#define USERLED_NO15 15
/* 用户按键状态 */
enum UserLedStatus{
	USERLED_OFF,
	USERLED_ON
};

#define USERSWITCH_NUMALL 6	/* 用户使用开关总数 */
struct SwitchQueue{
	#define SWITCHQUEUE_MAX 64
	uint8_t pIn;
	uint8_t pOut;
	uint8_t isfull;
	uint8_t number[SWITCHQUEUE_MAX];
	uint8_t state[SWITCHQUEUE_MAX];
};

#define USERKEY_NUMALL 10	/* 用户使用按键总数 */
struct KeyQueue{
	#define KEYQUEUE_MAX 64
	uint8_t pIn;
	uint8_t pOut;
	uint8_t isfull;
	uint8_t number[KEYQUEUE_MAX];
	uint8_t state[KEYQUEUE_MAX];
};

void SetUserLedStatus(uint8_t ledNo, uint8_t state);
void ContinuousOutResult(uint8_t beginNo,uint8_t num,uint8_t *pBuff);
struct KeyQueue *GetkeyQueueP(void);
void UserKeyChangeDeal(uint8_t keyNum, uint8_t state);
struct SwitchQueue *GetswitchQueueP(void);
void UserSwitchChangeDeal(uint8_t switchNum, uint8_t state);

#endif /* END _INOUTUSER_H_ */

/* END */

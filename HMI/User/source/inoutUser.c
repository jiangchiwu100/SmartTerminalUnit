/**
  * @file   inoutUser.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/17
  * @brief   开入开出用户配置程序
  */

#include "inoutUser.h"
#include "ledDriver.h"
#include "switchDriver.h"
#include "hmi101.h"

/* led映射表 */
const uint8_t userLedNoTab[USERLED_NUMALL] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
};
/* 开关映射表 */
const uint8_t userSwitchNoTab[USERSWITCH_NUMALL] = {
	0,1,2,3,4,5
};
/* 按键映射表 */
const uint8_t userKeyNoTab[USERKEY_NUMALL] = {
	0,1,2,3,4,5,6,7,8,9
};
/* 突变开关队列 */
struct SwitchQueue switchQueue;
/* 突变按键队列 */
struct KeyQueue keyQueue;

/**
  *@brief 设置userled的状态
  *@param  ledNo 灯号
  *@param  state led状态
  *@retval None
  */
void SetUserLedStatus(uint8_t ledNo, uint8_t state)
{
	if(ledNo < USERLED_NUMALL){
		switch(state){
			case USERLED_OFF:
				if(GetLedStatus(userLedNoTab[ledNo]) == LEDON){
					SetLedState(userLedNoTab[ledNo], LEDOFF);
				}break;
			case USERLED_ON:
				if(GetLedStatus(userLedNoTab[ledNo]) == LEDOFF){
					SetLedState(userLedNoTab[ledNo], LEDON);
				}break;
			default:break;
		}
	}
}
/**
  *@brief led连续处理
  *@param  beginNo 灯号
  *@param  num 数量
  *@param  pBuff 值数组
  *@retval None
  */
const uint8_t LedBitLook[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
void ContinuousOutResult(uint8_t beginNo,uint8_t num,uint8_t *pBuff)
{
	for(uint8_t i = 0; i < num; i++){
		if(pBuff[i>>3] & LedBitLook[i%8]){
			SetUserLedStatus(beginNo + i,USERLED_ON);
		}
		else{
			SetUserLedStatus(beginNo + i,USERLED_OFF);
		}
	}
}
/**
  *@brief 用户开关变位处理
  *@param  switchNum 开关号
  *@param  state 状态
  *@retval None
  */
void UserSwitchChangeDeal(uint8_t switchNum, uint8_t state)
{
	if(switchNum < USERSWITCH_NUMALL && userSwitchNoTab[switchNum] != 0xFF){
		switchQueue.number[switchQueue.pIn] = userSwitchNoTab[switchNum];
		switchQueue.state[switchQueue.pIn] = state;
		if(switchQueue.pIn == (uint8_t)(switchQueue.pOut - 1)){
			switchQueue.isfull = 1;
		}
		else{
			switchQueue.pIn ++;
		}
		if(switchQueue.pIn >= SWITCHQUEUE_MAX){
			switchQueue.pIn = 0;
		}
	}
}

/**
  *@brief 所有开关状态填充
  *@param  None
  *@retval 0 成功 1 失败
  */
uint8_t SwitchAllStateFill(void)
{
	uint8_t tSwitch[USERSWITCH_NUMALL];
	for(uint8_t i = 0; i < USERSWITCH_NUMALL; i++){
		tSwitch[i] = GetCurrentSwitchStatus(userSwitchNoTab[i]);
	}
	if(SwitchAllStateCmdSend(USERSWITCH_NUMALL,0,tSwitch) != 0){
		return 1;
	}
	return 0;
}

/**
  *@brief 返回开关队列指针
  *@param  None
  *@retval 开关队列指针
  */
struct SwitchQueue *GetswitchQueueP(void)
{
	return &switchQueue;
}

/**
  *@brief 开关队列初始化
  *@param  None
  *@retval None
  */
void SwitchQueueInit(void)
{
	switchQueue.isfull = 0;
	switchQueue.pIn = 0;
	switchQueue.pOut = 0;
}

/**
  *@brief 用户按键变位处理
  *@param  keyNum 按键号
  *@param  state 状态
  *@retval None
  */
void UserKeyChangeDeal(uint8_t keyNum, uint8_t state)
{
	if(keyNum < USERKEY_NUMALL && userKeyNoTab[keyNum] != 0xFF){
		keyQueue.number[keyQueue.pIn] = userKeyNoTab[keyNum];
		keyQueue.state[keyQueue.pIn] = state;
		if(keyQueue.pIn == (uint8_t)(keyQueue.pOut - 1)){
			keyQueue.isfull = 1;
		}
		else{
			keyQueue.pIn ++;
		}
		if(keyQueue.pIn >= KEYQUEUE_MAX){
			keyQueue.pIn = 0;
		}
	}
}

/**
  *@brief 返回按键队列指针
  *@param  None
  *@retval 按键队列指针
  */
struct KeyQueue *GetkeyQueueP(void)
{
	return &keyQueue;
}

/**
  *@brief 按键队列初始化
  *@param  None
  *@retval None
  */
void KeyQueueInit(void)
{
	keyQueue.isfull = 0;
	keyQueue.pIn = 0;
	keyQueue.pOut = 0;
}
/* END */

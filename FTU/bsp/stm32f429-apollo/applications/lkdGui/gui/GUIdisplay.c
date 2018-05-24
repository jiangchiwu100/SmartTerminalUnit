/**
  * @file   GUIdisplay.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    03-06-2018
  * @brief   GUI显示
  */

#include "gui_common.h"
#include "GUIdisplay.h"
#include "GUIdisplaydef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "userVariable.h"
#include "gui_101_cmd.h"
#include "hmiInOut.h"

const static uint8_t *modfiyKey[14] = {/* 软按键 */
	"0","1","2","3","4",".","esc","5","6","7","8","9","<-","ok"};
static enum KeyStatus keyStatus;	/* 按键值 */
static uint8_t userGUIBuff[1024*3]; /* 用于内存分配 */
static struct DZModfiy dZModfiy;	/* 定值修改 */
static struct Message MessageIs;	/* 消息管理 */
static struct PassWordPipe passWordPipe;		/* 密码管理 */
static enum DingZhiSetOrSee dingZhiSetOrSee;	/* 定值查询或者修改标志 */
static uint8_t stepTab[STEP_ALLNUM];
/**
  *@brief  获取时钟
  *@param  None
  *@retval 当前时钟滴答
  */	
static uint32_t getCurrentTick(void)
{
	return GetTimer1Tick();
}

/**
  *@brief  获取间隔时间
  *@param  开始时钟滴答
  *@retval 间隔时间
  */
static uint32_t GetIntervalTick(uint32_t beginTick)
{
	return GetTimer1IntervalTick(beginTick);
}

/**
  *@brief 修改定值设置或者浏览标志
  *@param  setOrSee 设置、浏览 枚举
  *@retval None
  */
static void SetDingZhiSetOrSee(enum DingZhiSetOrSee setOrSee)
{
	dingZhiSetOrSee = setOrSee;
}

/**
  *@brief 获取定值设置或者浏览标志
  *@param  None
  *@retval DingZhiSetOrSee 设置、浏览 枚举
  */
static enum DingZhiSetOrSee GetDingZhiSetOrSee(void)
{
	return dingZhiSetOrSee;
}

/**
  *@brief lcd显示开关
  *@param  key 按键状态
  *@retval lcd显示状态 LCDDISPLAYON LCDDISPLAYOFF
  */
static uint8_t GUIDisplayONOFF(enum KeyStatus *key)
{
	static uint32_t LcdDisPlayTick;//计时
	static uint8_t flag;
	static uint8_t lcdONOFF;
	if(flag == 0){
		LcdDisPlayTick = getCurrentTick();
		lcdONOFF = LCDDISPLAYON;
		flag = 1;
	}
	if(flag == 1){
		if(*key != NoKey){
			LcdDisPlayTick = getCurrentTick();
			if(lcdONOFF == LCDDISPLAYOFF){
				lcdONOFF = LCDDISPLAYON;
				*key = NoKey;
				OpenLcdDisplay();
			}
		}
		else{
			if(lcdONOFF == LCDDISPLAYON){
				if(GetIntervalTick(LcdDisPlayTick) > LCDDISPLAYOFFTIME){//时间
					lcdONOFF = LCDDISPLAYOFF;
					CloseLcdDisplay();
				}
			}
		}
	}
	return lcdONOFF;
}


/**
  *@brief GUI启动界面
  *@param  None
  *@retval None
  */
void GUIStartInterface(void)
{
	uint8_t i,j;
	lkdBitmap tImage;
	GuiSetbackcolor(0);
	GuiSetForecolor(1);
	tImage.number = 1;
	tImage.wide = 160;
	tImage.high = 30;
	tImage.beginx = 0;
	tImage.beginy = 0;
	tImage.bitmap = NULL;
	GuiAreaBitMap(&tImage,1, 25, 159, 55, 0);
	j = 110;
	for(i=1;i<24;i+=2){
		GuiHPointLine(1,i,j,2,forecolor);
		j -= 2;
	}
	for(i=25;i<55;i+=2){
		GuiHPointLine(1,i,159,2,forecolor);
	}
	j = 70;
	for(i=55;i<78;i+=2){
		GuiHPointLine(j,i,159,2,forecolor);
		j -= 2;
	}
	GuiHLine(1,78,158,forecolor);
	GuiFillRect(1,81,159,94, forecolor);
	GuiHLine(1,96,158,forecolor);
	GuiExchangeColor();
	GuiFont12Align(2,82,156,FONT_MID,"正在启动......");
	GuiExchangeColor();
	GuiUpdateDisplayAll();
}

/**
  *@brief GUI在线更新
  *@param  None
  *@retval None
  */
void GUIUpdataProgram(void)
{
	uint8_t i,j;
	lkdBitmap tImage;
	
	GuiFillRect(1,1,158,158, backcolor);
	tImage.number = 1;
	tImage.wide = 160;
	tImage.high = 30;
	tImage.beginx = 0;
	tImage.beginy = 0;
	tImage.bitmap = NULL;
	GuiAreaBitMap(&tImage,1, 25, 159, 55, 0);
	j = 110;
	for(i=1;i<24;i+=2){
		GuiHPointLine(1,i,j,2,forecolor);
		j -= 2;
	}
	for(i=25;i<55;i+=2){
		GuiHPointLine(1,i,159,2,forecolor);
	}
	j = 70;
	for(i=55;i<78;i+=2){
		GuiHPointLine(j,i,159,2,forecolor);
		j -= 2;
	}
	GuiHLine(1,78,158,forecolor);
	GuiFillRect(1,81,159,94, forecolor);
	GuiHLine(1,96,158,forecolor);
	GuiExchangeColor();
	GuiFont12Align(2,82,156,FONT_MID,"正在更新......");
	GuiExchangeColor();
	GuiUpdateDisplayAll();
}

/**
  *@brief GUI等待图标
  *@param  x,y 起始坐标
  *@retval None
  */
void GUIWiatMessage(uint8_t x,uint8_t y)
{
	GuiFillRect(x,y,x + 88,y + 16, backcolor);
	GuiHLine(x + 2,y + 16,x + 86,forecolor);
	GuiHLine(x + 3,y + 17,x + 87,forecolor);
	GuiRLine(x + 86,y + 2,y + 16,forecolor);
	GuiRLine(x + 87,y + 3,y + 16,forecolor);
	GuiFont12Align(x + 20,y + 2,70,FONT_LEFT,"加载中.....");
	GuiFillRect(x + 2,y + 2,x + 14,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiFillRect(x + 2,y + 2,32 + 12,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiUpdateDisplayAll();
}
/**
  *@brief GUI初始化，添加主窗口
  *@param  None
  *@retval None
  */
void GUIDisplayInit(void)
{
	
	MenuManageInit();
	WinManageInit();
	for(uint8_t i = 0; i < STEP_ALLNUM; i++){
		stepTab[i] = 0;
	}
	userGUIWindowAdd(&HomeWindow);
}

/**
  *@brief GUI主函数，用于主函数调用
  *@param  None
  *@retval None
  */
void GUIDisplayMian(void)
{
//	static uint8_t flag;
//	uint8_t upDataProgram;
//	//upDataProgram = GetDtatUpdateStatus();
	keyStatus = GetKeyStatus();//获取按键状态
	SetKeyIsNoKey();
//	if(flag == 0 && upDataProgram.bit.programUpdate != 0){
//		OpenLcdDisplay();
//		GUIUpdataProgram();//在线更新
//		flag = 1;
//	}
//	else if(flag == 1 && upDataProgram.bit.programUpdate == 0){
//		flag = 0;
//		//NVIC_SystemReset();//重启
//	}
//	else if(flag == 0  && GUIDisplayONOFF(&keyStatus) == LCDDISPLAYON){
//		
//	}
	userGUITopWindowDisplay();
}

/**
  *@brief 设置密码窗口相关值
  *@param  type 密码类型
  *@param  returnflag 返回状态指针
  *@retval None
  */
void SetPassWordWin(uint8_t type,uint8_t *returnflag)
{
	passWordPipe.passWordType = type;
	passWordPipe.returnFlag = returnflag;
}

/**
  *@brief 密码窗口
  *@param  None
  *@retval None
  */
void PassWordFun(void)
{	
	static uint8_t passWordStr[8];
	static int8_t currentNum;
	uint8_t tempStr[2];
	uint8_t i;
	if(stepTab[STEP_PASSWORD] == 0){
		currentNum = 0;
		for(i=0;i<7;i++){
			passWordStr[i] = '0';
		}
		passWordStr[7] = '\0';
		GuiFillRect(PassWordWin.x+1,PassWordWin.y+16,\
			PassWordWin.x + PassWordWin.wide - 2,PassWordWin.hight - 1, backcolor);
		GuiHLine(PassWordWin.x+1,PassWordWin.y+18,\
			PassWordWin.x + PassWordWin.wide - 1,forecolor);
		GuiFont12Align(PassWordWin.x+1,PassWordWin.y +20,PassWordWin.wide - 1,FONT_LEFT,\
			PassWordItems[passWordPipe.passWordType].name);//提示语
		GuiHPointLine(PassWordWin.x+1,PassWordWin.y +34,\
			PassWordWin.x + PassWordWin.wide - 1,2,forecolor);
		stepTab[STEP_PASSWORD] = 1;
	}
	if(stepTab[STEP_PASSWORD] == 1){
		for(i=0;i<6;i++){
			if(i == currentNum){
				tempStr[0] = passWordStr[currentNum];
				tempStr[1] = '\0';
				GuiFillRect(PassWordWin.x + 25 + i*10 - 1,PassWordWin.y +45,\
					PassWordWin.x + 25 + i*10 + 6,PassWordWin.y +57, forecolor);
				GuiExchangeColor();
				GuiFont12Align(PassWordWin.x + 25 + i*10,PassWordWin.y +45,12,FONT_LEFT,tempStr);
				GuiExchangeColor();
			}
			else{
				GuiFillRect(PassWordWin.x + 25 + i*10 - 1,PassWordWin.y +45,\
					PassWordWin.x + 25 + i*10 + 6,PassWordWin.y +57, backcolor);
				GuiFont12Align(PassWordWin.x + 25 + i*10,PassWordWin.y +45,12,FONT_LEFT,"*");
			}		
		}
		GuiUpdateDisplayAll();
		stepTab[STEP_PASSWORD] = 2;
	}
	switch(keyStatus){
	case LeftKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = 5;
		}
		stepTab[STEP_PASSWORD] = 1;
		break;
	case DownKey:
		passWordStr[currentNum] --;
		if(passWordStr[currentNum]<'0'){
			passWordStr[currentNum] = '9';
		}
		stepTab[STEP_PASSWORD] = 1;
		break;
	case RightKey:
		currentNum ++;
		if(currentNum >= 6){
			currentNum = 0;
		}
		stepTab[STEP_PASSWORD] = 1;
		break;
	case UpKey:
		passWordStr[currentNum] ++;
		if(passWordStr[currentNum] > '9'){
			passWordStr[currentNum] = '0';
		}
		stepTab[STEP_PASSWORD] = 1;
		break;
	case OkKey:
		if(strncmp((char *)passWordStr,\
			(char *)PassWordItems[passWordPipe.passWordType].passWord,6) == 0){
			*passWordPipe.returnFlag = 1;
		}
		else{
			*passWordPipe.returnFlag = 0;
		}//需要执行CancelKey程序，所以没有break
	case CancelKey:
		stepTab[STEP_PASSWORD] = 0; 
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}
/**
  *@brief 设置消息窗口相应变量
  *@param  x ,y 起始坐标
  *@param  pStr 说明字符串
  *@param  flag 返回标志 0 取消 1 确定
  *@retval None
  */
void SetMessageWin(int16_t x,int16_t y,uint8_t *pStr,uint8_t *flag)
{
	MessageWin.x = x;
	MessageWin.y = y;
	MessageIs.str = pStr;
	MessageIs.flag = flag;
}
/**
  *@brief 消息窗口处理函数
  *@param  None
  *@retval None
  */
void MessageFun(void)
{
	static int8_t keyFlag;
	uint8_t winWide;
	uint8_t winHight;
	if(stepTab[STEP_MESSAGE] == 0){
		keyFlag = 0;
		stepTab[STEP_MESSAGE] = 1;
	}
	if(stepTab[STEP_MESSAGE] == 1){
		GuiFillRect(MessageWin.x+1,MessageWin.y+16,\
			MessageWin.x + MessageWin.wide - 2,MessageWin.hight - 1, backcolor);
		GuiHLine(MessageWin.x+1,MessageWin.y+18,\
			MessageWin.x + MessageWin.wide - 1,forecolor);
		winHight =  (MessageWin.hight - 41 - 12)/2;
		GuiFont12Align(MessageWin.x+1,winHight + MessageWin.y +19,\
			MessageWin.wide - 1,FONT_MID,MessageIs.str);//提示语
		winHight = MessageWin.y + MessageWin.hight - 23;
		GuiHPointLine(MessageWin.x+1,winHight,\
			MessageWin.x + MessageWin.wide - 1,2,forecolor);
		if(keyFlag == 0){
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"确认",0);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"取消",1);
		}
		else{
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"确认",1);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"取消",0);
		}
		GuiUpdateDisplayAll();
		stepTab[STEP_MESSAGE] = 2;
	}
	if(stepTab[STEP_MESSAGE] == 2){
	}
	switch(keyStatus){
	case LeftKey:
		keyFlag--;
		if(keyFlag < 0){
			keyFlag = 1;
		}
		stepTab[STEP_MESSAGE] = 1;break;
	case UpKey:break;
	case RightKey:
		keyFlag++;
		if(keyFlag > 1){
			keyFlag = 0;
		}
		stepTab[STEP_MESSAGE] = 1;break;
	case DownKey:break;
	case OkKey:
		*MessageIs.flag = keyFlag;
		stepTab[STEP_MESSAGE] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	case CancelKey:
		*MessageIs.flag = 0;
		stepTab[STEP_MESSAGE] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	default:break;
	}
}

/**
  *@brief  设置定值修改相关变量
  *@param  info 定值结构体指针
  *@param  flag 标志字节，未使用
  *@param  itemIs 定值当前Item
  *@param  str 要修改字符串的指针
  *@retval None
  */
static void SetDingZhiModfiy(const DzhiDisplayInfo *info,uint8_t flag,uint8_t itemIs,uint8_t *str)
{
	dZModfiy.info = info;
	dZModfiy.flag = flag;
	dZModfiy.itemIs = itemIs;
	dZModfiy.str = str;	
}

/**
  *@brief 定值修改初始化
  *@param  None
  *@retval None
  */
void DZModfiyInit(void)
{
	int16_t x,y;
	float tRange;
	uint8_t tRangeStr[16];
	GuiRect(DZModfiyWin.x,DZModfiyWin.y,DZModfiyWin.x + DZModfiyWin.wide, \
		DZModfiyWin.y + DZModfiyWin.hight, forecolor);
	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 17,150,FONT_LEFT,\
		(uint8_t *)dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pName);
	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 30,\
		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + DZModfiyWin.hight - 16,\
		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);//状态栏
	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + DZModfiyWin.hight - 14,80,FONT_LEFT,"当前状态:");
	
	if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//汉字
		x = DZModfiyWin.x + DZModfiyWin.wide;
		y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
		GuiFillRect(DZModfiyWin.x+9,y,x - 10,y + 17, backcolor);
		GuiFillRect(DZModfiyWin.x+17,y + 2,x - 20,y + 15, forecolor);
		GuiFont12Align(DZModfiyWin.x+7,y+4,10,FONT_LEFT,"<");
		GuiFont12Align(x - 14,y+4,10,FONT_LEFT,">");
	}
	else{//float
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 32,40,FONT_LEFT,"最大值");
		tRange = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMax;
		sprintf((char *)tRangeStr,"%.3f",tRange);
		tRangeStr[15] = '\0';
		GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 32,80,FONT_MID,tRangeStr);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 45,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 47,40,FONT_LEFT,"最小值");
		tRange = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMin;
		sprintf((char *)tRangeStr,"%.3f",tRange);
		tRangeStr[15] = '\0';
		GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 47,80,FONT_MID,tRangeStr);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 60,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 62,40,FONT_LEFT,"当前值");
		GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 62,\
			DZModfiyWin.x + 128,DZModfiyWin.y + 74, backcolor);
		GuiFont12Align(DZModfiyWin.x + 50,\
			DZModfiyWin.y + 62,80,FONT_MID,dZModfiy.str);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 75,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 77,40,FONT_LEFT,"修改为");
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 90,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiRPointLine(DZModfiyWin.x + 47,DZModfiyWin.y + 30,\
			DZModfiyWin.y + 90,2,forecolor);	
	}
}

/**
  *@brief 画定值修改按键
  *@param  None
  *@retval None
  */
void DrawModfiyKey(uint8_t key)
{
	uint8_t i;
	int16_t x,y;
	for(i=0;i<14;i++){
		if(i < 7){
			x = DZModfiyWin.x + i*18;
			y = DZModfiyWin.y + 92;
		}
		else{
			x = DZModfiyWin.x + (i - 7)*18;
			y = DZModfiyWin.y + 104;
		}
		if(i == key){
			GuiFillRect(x,y,x + 18,y + 11, forecolor);
			GuiExchangeColor();
			GuiFont12Align(x+1,y,18,FONT_MID,(uint8_t *)modfiyKey[i]);
			GuiExchangeColor();
		}
		else{
			GuiFillRect(x,y,x + 18,y + 11, backcolor);
			GuiFont12Align(x+1,y,18,FONT_MID,(uint8_t *)modfiyKey[i]);
		}
		GuiRLine(x,y,y + 11,forecolor);
	}
	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 92,\
		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 104,\
		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 116,\
		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
	GuiUpdateDisplayAll();
}

/**
  *@brief 定值修改窗口函数
  *@param  None
  *@retval None
  */
void DZModfiyFun(void)
{
	static uint8_t modfiyStr[16];//修改暂存buff
	static uint8_t modfiyStrP = 0;
	static uint8_t keyIs = 0;//按键记录
	static uint32_t inputCursorTick;//光标闪烁记时
	int16_t x,y;
	float fValue;
	if(stepTab[STEP_DZHIMODFIY] == 0){//初始化
		DZModfiyInit();
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){
			keyIs = dZModfiy.str[0];
		}
		else{
			keyIs = 0;
		}
		modfiyStrP = 0;
		modfiyStr[0] = ' ';
		modfiyStr[1] = '\0';
		stepTab[STEP_DZHIMODFIY] = 1;
	}
	if(stepTab[STEP_DZHIMODFIY] == 1){//显示
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//汉字处理
			GuiExchangeColor();
			x = DZModfiyWin.x + DZModfiyWin.wide;
			y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
			GuiFillRect(DZModfiyWin.x+18,y + 3,x - 20,y + 15, backcolor);
			GuiFont12Align(DZModfiyWin.x+18,y + 3,x - DZModfiyWin.x - 40,FONT_MID,\
				(uint8_t *)dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pContent[keyIs]);
			GuiExchangeColor();
			stepTab[STEP_DZHIMODFIY] = 3;
		}
		else{//float处理
			DrawModfiyKey(keyIs);
			GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 77,\
				DZModfiyWin.x + 128,DZModfiyWin.y + 89, backcolor);
			GuiFont12Align(DZModfiyWin.x + 50,\
				DZModfiyWin.y + 77,78,FONT_MID,modfiyStr);
			stepTab[STEP_DZHIMODFIY] = 2;
		}
		GuiUpdateDisplayAll();
	}
	if(stepTab[STEP_DZHIMODFIY] == 2){//光标闪动处理
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType == 0){
			if(GetIntervalTick(inputCursorTick) < 500){
				if(modfiyStr[modfiyStrP] != '|'){
					modfiyStr[modfiyStrP] = '|';
					modfiyStr[modfiyStrP + 1] = '\0';
					stepTab[STEP_DZHIMODFIY] = 1;
				}
			}
			else if(GetIntervalTick(inputCursorTick) < 1000){
				if(modfiyStr[modfiyStrP] != ' '){
					modfiyStr[modfiyStrP] = ' ';
					modfiyStr[modfiyStrP + 1] = '\0';
					stepTab[STEP_DZHIMODFIY] = 1;
				}
			}
			else{
				inputCursorTick = getCurrentTick();
			}				
		}
	}
	if(stepTab[STEP_DZHIMODFIY] == 3){//等待
	}
	if(stepTab[STEP_DZHIMODFIY] == 4){//发送命令
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//汉字		
			dZModfiy.str[0] = modfiyStr[0];
			*(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pVal) = (float)modfiyStr[0];
			dZModfiy.info->SaveModify(0);
			stepTab[STEP_DZHIMODFIY] = 5;
		}
		else{
			modfiyStr[modfiyStrP] = '\0';
			fValue = atof((const char *)modfiyStr);
			if(fValue <= dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMax &&\
				fValue >= dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMin){
				*(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pVal) = fValue;
				strcpy((char *)dZModfiy.str,(char *)modfiyStr);
				dZModfiy.info->SaveModify(0);
				stepTab[STEP_DZHIMODFIY] = 5;
			}
			else{//超出范围
				GuiFillRect(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,\
					DZModfiyWin.x + DZModfiyWin.wide - 2,DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
				GuiFont12Align(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"超出范围");
				modfiyStr[modfiyStrP] = ' ';
				modfiyStr[modfiyStrP + 1] = '\0';
				stepTab[STEP_DZHIMODFIY] = 2;
			}
		}
	}
	if(stepTab[STEP_DZHIMODFIY] == 5){//发送完后等待回复
		GuiFillRect(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,\
			DZModfiyWin.x + DZModfiyWin.wide - 2,DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
		GuiFont12Align(DZModfiyWin.x + 57,\
			DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"修改成功");
		GuiUpdateDisplayAll();
		stepTab[STEP_DZHIMODFIY] = 6;
		inputCursorTick = getCurrentTick();
	}
	if(stepTab[STEP_DZHIMODFIY] == 6){
		if(GetIntervalTick(inputCursorTick) > 500){//延时退出
			stepTab[STEP_DZHIMODFIY] = 0;
			userGUITopWindowHide();
		}
	}
	if(keyStatus != CancelKey && stepTab[STEP_DZHIMODFIY] >= 4){
		keyStatus = NoKey;
	}
	switch(keyStatus){
	case UpKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType == 0){
			if(keyIs < 7)
				keyIs += 7;
			else
				keyIs -= 7;
		}
		stepTab[STEP_DZHIMODFIY] = 1;break;
	case DownKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType == 0){
			if(keyIs >= 7)
				keyIs -= 7;
			else
				keyIs += 7;
		}
		stepTab[STEP_DZHIMODFIY] = 1;break;
	case LeftKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){
			if(keyIs == 0)
				keyIs = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType - 1;
			else
				keyIs --; 
		}
		else{
			if(keyIs == 0 || keyIs == 7){	
				keyIs += 6;
			}
			else{
				keyIs --;
			}
		}
		stepTab[STEP_DZHIMODFIY] = 1;break;
	case RightKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){
			if(keyIs == (dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType - 1))
				keyIs = 0;
			else
				keyIs ++; 
		}
		else{
			if(keyIs == 6 || keyIs == 13)
				keyIs -= 6;
			else
				keyIs ++;
		}
		stepTab[STEP_DZHIMODFIY] = 1;break;
	case OkKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//命令发送
			modfiyStr[0] = keyIs;
			stepTab[STEP_DZHIMODFIY] = 4;
		}
		else{
			if(keyIs < 12 && keyIs != 6){
				if(modfiyStrP < 12){
					modfiyStr[modfiyStrP] = *modfiyKey[keyIs];
					modfiyStrP ++;
				}
			}
			else if(keyIs == 6){//esc
				modfiyStr[1] = '\0';
				modfiyStrP = 0;
				keyIs = 0;
				stepTab[STEP_DZHIMODFIY] = 0;
				userGUITopWindowHide();
			}
			else if(keyIs == 12){
				if(modfiyStrP > 0){
					modfiyStr[modfiyStrP] = '\0';
					modfiyStrP --;
				}			
			}
			else if(keyIs == 13){//ok
				stepTab[STEP_DZHIMODFIY] = 4;//命令发送
			}
		}break;
	case CancelKey:
		stepTab[STEP_DZHIMODFIY] = 0;
		userGUITopWindowHide();
		break;
	default: break;
	}
}

/**
  *@brief 通用定值显示函数
  *@param  Items 定值结构指针
  *@param  itemsNum 定值数
  *@param  cmd 命令
  *@param  flag 步骤标志
  *@retval None
  */
void DZModfiyDisplay(DzhiDisplayInfo *info,uint8_t *flag)
{
	const uint8_t listCol[2][DZDISPLAYCOL] = {{73,63,20},{FONT_LEFT,FONT_MID,FONT_MID}};
	static SCROLL *Scroll;//进度条
	static ListColSet colset;
	static LIST  *list;//列表控件
	static uint8_t **pText;//列表内容指针
	static uint8_t *col1Data;//显示内容指针
	uint8_t i,itemsNum;
	uint16_t j ;
	uint16_t memMall;//用于内存分配
	float tempFloat;
	if(*flag == 0){//初始化
		itemsNum = info->num;
		memMall = 0;
		list = (LIST  *)&userGUIBuff[memMall];
		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
		Scroll = (SCROLL *)&userGUIBuff[memMall];
		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
		pText = (uint8_t **)&userGUIBuff[memMall];
		memMall += itemsNum*DZDISPLAYCOL*4;
		col1Data = &userGUIBuff[memMall];
		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = itemsNum;
		Scroll->lump = 1;
		colset.colWide = (uint8_t *)listCol[0];
		colset.colFlag = (uint8_t *)listCol[1];
		list->x = 0;
		list->y = 18;
		list->wide = 156;
		list->hight = 141;
		list->row = itemsNum;
		list->col = DZDISPLAYCOL;
		list->drawRow = 0;
		list->currentRow = 0;
		list->flag = LIST_USEBORDER_H;
		list->content = (uint8_t **)pText;
		list->colSet = &colset;
		*flag = 1;
		for(j=0;j<itemsNum*16;j++){
			col1Data[j] = 0;
		}
	}
	if(*flag == 1){
		itemsNum = info->num;
		for(i=0;i<itemsNum;i++){
			*(pText + i*DZDISPLAYCOL + 0) = (uint8_t *)info->pRoot[info->pBuff[i]].pName;
			if(info->pRoot[info->pBuff[i]].dataType){//汉字
				tempFloat = *(info->pRoot[info->pBuff[i]].pVal);
				if((uint32_t)tempFloat >= info->pRoot[info->pBuff[i]].dataType){
					tempFloat = 0;
				}
				*(pText + i*DZDISPLAYCOL + 1) = (uint8_t *)info->pRoot[info->pBuff[i]].pContent[(uint8_t)tempFloat];
			}
			else{//float value
				tempFloat = *(info->pRoot[info->pBuff[i]].pVal);
				sprintf((char *)&col1Data[i*16],"%.3f",tempFloat);
				col1Data[i*16 + 15] = '\0';
				*(pText + i*DZDISPLAYCOL + 1) = &col1Data[i*16];
			}
			*(pText + i*DZDISPLAYCOL + 2) = (uint8_t *)info->pRoot[info->pBuff[i]].pUnit;
		}
		*flag = 2;
	}
	if(*flag == 2){//指定表格内容指针
		
		if((list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW + DISPLAYLISTROW <= list->row){
			list->drawRow = (list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW;
		}
		else{
			if(list->row < DISPLAYLISTROW){
				list->drawRow = 0;
			}
			else{
				list->drawRow = list->row - DISPLAYLISTROW;
			}	
		}
		DrawList(list);
		Scroll->lump = list->currentRow + 1;
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		*flag = 3;
	}
	if(keyStatus != CancelKey && *flag <= 1){
		keyStatus = NoKey;
	}
	switch(keyStatus){
	case UpKey:
		if(list->currentRow > 0){
			list->currentRow --;
		}
		*flag = 2;break;
	case DownKey:
		if(list->currentRow < list->row - 1){
			list->currentRow ++;
		}
		*flag = 2;break;
	case LeftKey:
		if(list->currentRow >= DISPLAYLISTROW){
			list->currentRow -= DISPLAYLISTROW;
		}
		else{
			list->currentRow =0;
		}
		*flag = 2;break;
	case RightKey:
		if(list->currentRow + DISPLAYLISTROW < list->row){
			list->currentRow += DISPLAYLISTROW;
		}
		else{
			list->currentRow = list->row - 1;
		}
		*flag = 2;break;
	case OkKey:
		if(GetDingZhiSetOrSee() == SET_DINGZHI){//定值修改
			SetDingZhiModfiy(info,1,list->currentRow,&col1Data[list->currentRow*16]);
			userGUIWindowAdd(&DZModfiyWin);
			*flag = 1;
		}break;
	case CancelKey:
		*flag = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}

/**
  *@brief HomeWindow deal with function
  *@param  None
  *@retval None
  */
void HomeWindowFun(void)
{
	static SystemTimeDisplay *disTime;
	static uint8_t *strTime;
	static uint32_t homeTick;
	static uint8_t tClose;
	uint8_t i,j;
	uint16_t memMall;
	if(stepTab[STEP_HOMEWIN] == 0){//绘制主页
		memMall = 0;
		strTime = &userGUIBuff[memMall];
		memMall += 32;
		disTime = (SystemTimeDisplay *)&userGUIBuff[memMall];
		
		lkdBitmap tImage;
		tImage.number = 1;
		tImage.wide = 160;
		tImage.high = 30;
		tImage.beginx = 0;
		tImage.beginy = 0;
		tImage.bitmap = NULL;
		GuiAreaBitMap(&tImage,HomeWindow.x+1,HomeWindow.y+25, 159, 55, 1);
		j = 110;
		for(i=1;i<24;i+=2){
			GuiHPointLine(HomeWindow.x+1,i,j,2,forecolor);
			j -= 2;
		}
		for(i=25;i<55;i+=2){
			GuiHPointLine(HomeWindow.x+1,i,159,2,forecolor);
		}
		j = 70;
		for(i=55;i<78;i+=2){
			GuiHPointLine(HomeWindow.x+j,i,159,2,forecolor);
			j -= 2;
		}
		GuiHLine(HomeWindow.x+1,78,158,forecolor);
		GuiFillRect(HomeWindow.x+1,81,159,94, forecolor);
		GuiHLine(HomeWindow.x+1,96,158,forecolor);
		
		GuiFont12Align(HomeWindow.x+9,102,16,FONT_LEFT,"信");
		GuiFont12Align(HomeWindow.x+9,121,16,FONT_LEFT,"息");
		GuiFont12Align(HomeWindow.x+9,140,16,FONT_LEFT,"栏");
		GuiRLine(HomeWindow.x+30,96,158,forecolor);
		
		for(i=0;i<4;i++){
			GuiFont12Align(HomeWindow.x+31,96+2+i*16,90,FONT_MID,"----");
			GuiRPointLine(HomeWindow.x+31+91,96,96+15+i*16,2,forecolor);
			GuiFont12Align(HomeWindow.x+31+93,96+2+i*16,34,FONT_MID,"--");
			GuiHPointLine(HomeWindow.x+30,96+15+i*16,159,2,forecolor);
		}
		
		GuiFont12Align(HomeWindow.x+31,96+34,90,FONT_MID,"开关状态");
		stepTab[STEP_HOMEWIN] = 1;
	}
	if(stepTab[STEP_HOMEWIN] == 1){
		GuiExchangeColor();
		GetDisplayTime(disTime);
		sprintf((char *)strTime,"20%02d-%02d-%02d   %02d:%02d:%02d",disTime->year,\
			disTime->month,disTime->day,disTime->hour,disTime->min,disTime->s);
		GuiFont12Align(HomeWindow.x+2,82,156,FONT_MID,strTime);
		GuiExchangeColor();
		tClose = *(yxInfo.pRoot[1].pVal);
		if(tClose == 2){
			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"合");
		}
		else{
			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"分");
		}
		GuiUpdateDisplayAll();
		stepTab[STEP_HOMEWIN] = 2;
		homeTick = getCurrentTick();
	}
	if(stepTab[STEP_HOMEWIN] == 2){
		if(GetIntervalTick(homeTick) > 1000 || tClose != *(yxInfo.pRoot[1].pVal)){
			stepTab[STEP_HOMEWIN] = 1;
		}
	}
	switch(keyStatus){
	case UpKey:
	case DownKey:
		stepTab[STEP_HOMEWIN] = 0;
		userGUIWindowAdd(&YaoCe2Win);
		break;
	case LeftKey:
	case RightKey:
		stepTab[STEP_HOMEWIN] = 0;
		userGUIWindowAdd(&YaoxinWin);
		break;
	case OkKey:
		stepTab[STEP_HOMEWIN] = 0;
		userGUIWindowAdd(&MenuWindow);
		break;
	case CancelKey:break;
	default: break;
	}
}

/**
  *@brief Menu Window deal with function
  *@param  None
  *@retval None
  */
static void MenuWindowFun(void)
{
	if(GetMenuManagepMenu() == 0){
		userGUIMenuAdd(&mianMenu);
	}
	userGUIMenuDisplay();
}

/**
  *@brief mianMenu deal with function 主菜单
  *@param  None
  *@retval None
  */
static void mianMenuFun(void)
{
	static uint8_t passWordState;
	
	if(stepTab[STEP_MAINMENU] == 0){
		passWordState = 0;
	}
	if(stepTab[STEP_MAINMENU] == 1){
		switch(mianMenu.currentItem){
		case 0://信息查询
		case 4://版本信息
			stepTab[STEP_MAINMENU] = 3;
			break;
		case 1://定值设置
		case 2://配置设置
		case 3://命令下发
			SetPassWordWin(0,&passWordState);
			userGUIWindowAdd(&PassWordWin);
			stepTab[STEP_MAINMENU] = 2;
			break;
		default:break;
		}			
	}
	else if(stepTab[STEP_MAINMENU] == 2){
		if(passWordState == 1){
			stepTab[STEP_MAINMENU] = 3;
		}
		else{
			stepTab[STEP_MAINMENU] = 0;
		}
	}
	if(stepTab[STEP_MAINMENU] == 3){
		switch(mianMenu.currentItem){
		case 0:userGUIMenuAdd(&MenuM0);break;//信息查询
		case 1:userGUIMenuAdd(&MenuM1);break;//定值设置
		case 2:userGUIMenuAdd(&MenuM2);break;//配置设置
		case 3:userGUIWindowAdd(&CmdSendWin);break;//命令下发
		case 4:userGUIWindowAdd(&VersionWin);break;//版本信息
		default:break;
		}
	}
	switch(keyStatus){
	case UpKey:
		userGUIMenuIremModfiy(0);
		break;
	case DownKey:
		userGUIMenuIremModfiy(1);
		break;
	case LeftKey:
	case CancelKey:
		stepTab[STEP_MAINMENU] = 0;
		userGUIMenuHide();
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	case RightKey:
	case OkKey:
		if(stepTab[STEP_MAINMENU] == 0){
			stepTab[STEP_MAINMENU] = 1;
		}
		break;
	}
	if(stepTab[STEP_MAINMENU] == 3){
		stepTab[STEP_MAINMENU] = 0;
	}
}

/**
  *@brief MenuM0 deal with function 信息查询
  *@param  None
  *@retval None
  */
static void MenuM0Fun(void)
{
	switch(keyStatus){
	case UpKey:userGUIMenuIremModfiy(0);break;	
	case DownKey:userGUIMenuIremModfiy(1);break;	
	case LeftKey:
	case CancelKey:
		SetDingZhiSetOrSee(NULL_DINGZHI);//修改定值设置标志为空
		userGUIMenuHide();break;
	case RightKey:
	case OkKey:
		SetDingZhiSetOrSee(SEE_DINGZHI);//修改定值设置标志为查询
		switch(MenuM0.currentItem){
		case 0:userGUIWindowAdd(&YaoxinWin);break;//遥信查询
		case 1:userGUIMenuAdd(&MenuM0S1);break;//遥测查询
		case 2:userGUIWindowAdd(&SOEWin);break;//SOE查询
		case 3:userGUIWindowAdd(&FaultEventWin);break;//故障事件
		case 4:userGUIMenuAdd(&MenuM0S4);break;//定值查询
		case 5:userGUIMenuAdd(&MenuM0S5);break;//配置查询
		default:break;
		}
	}
}

/**
  *@brief MenuM1 deal with function 定值设置
  *@param  None
  *@retval None
  */
static void MenuM1Fun(void)
{
	switch(keyStatus){
	case UpKey:
		userGUIMenuIremModfiy(0);
		break;
	case DownKey:
		userGUIMenuIremModfiy(1);
		break;
	case LeftKey:
	case CancelKey:
		SetDingZhiSetOrSee(NULL_DINGZHI);//修改定值设置标志为空
		userGUIMenuHide();
		break;
	case RightKey:
	case OkKey:
		SetDingZhiSetOrSee(SET_DINGZHI);//修改定值设置标志为设置
		switch(MenuM1.currentItem){
		case 0:userGUIMenuAdd(&MenuM1S0);break;//保护功能
		case 1:userGUIWindowAdd(&LogicalFunWin);break;//逻辑功能
		case 2:userGUIWindowAdd(&OverLineWarnWin);break;//越线报警
		case 3:userGUIWindowAdd(&OverLoadMuchWin);break;//重过载
		case 4:userGUIWindowAdd(&OverLoadWin);break;//过负荷
		case 5:userGUIWindowAdd(&OverVoltageWin);break;//过电压
		case 6:userGUIWindowAdd(&BatterySetWin);break;//电池设置
		case 7:userGUIWindowAdd(&AutoResetWin);break;//自动复归
		case 8:userGUIMenuAdd(&MenuM1S8);break;//其他设置
		default:break;
		}
	}
}

/**
  *@brief  MenuM2 deal with function 配置设置
  *@param  None
  *@retval None
  */
static void MenuM2Fun(void)
{
	switch(keyStatus){
	case UpKey:
		userGUIMenuIremModfiy(0);
		break;
	case DownKey:
		userGUIMenuIremModfiy(1);
		break;
	case LeftKey:
	case CancelKey:
		SetDingZhiSetOrSee(NULL_DINGZHI);//修改定值设置标志为空
		userGUIMenuHide();
		break;
	case RightKey:
	case OkKey:
		SetDingZhiSetOrSee(SET_DINGZHI);//修改定值设置标志为设置
		switch(MenuM2.currentItem){
		case 0:userGUIWindowAdd(&BasicSetWin);break;
		case 1:userGUIWindowAdd(&ZeroDriftWin);break;
		case 2:userGUIWindowAdd(&DeadZoneWin);break;
		default:break;
		}
	}
}

/**
  *@brief yaoce deal with function
  *@param  None
  *@retval None
  */
static void MenuM0S1Fun(void)
{
	switch(keyStatus){
	case UpKey:userGUIMenuIremModfiy(0);break;
	case DownKey:userGUIMenuIremModfiy(1);break;
	case LeftKey:
	case CancelKey:
		userGUIMenuHide();break;
	case RightKey:
	case OkKey:
		switch(MenuM0S1.currentItem){
		case 0:userGUIWindowAdd(&YaoCe1Win);break;//一次遥测
		case 1:userGUIWindowAdd(&YaoCe2Win);break;//二次遥测
		case 2:userGUIWindowAdd(&HarmonicWin);break;//谐波
		default:break;
		}
	}
}

/**
  *@brief MenuM0S4 deal with function 定值查询
  *@param  None
  *@retval None
  */
static void MenuM0S4Fun(void)
{
	switch(keyStatus){
	case UpKey:
		userGUIMenuIremModfiy(0);
		break;
	case DownKey:
		userGUIMenuIremModfiy(1);
		break;
	case LeftKey:
	case CancelKey:
		userGUIMenuHide();
		break;
	case RightKey:
	case OkKey:
		switch(MenuM0S4.currentItem){
		case 0:userGUIMenuAdd(&MenuM0S4S0);break;//保护功能
		case 1:userGUIWindowAdd(&LogicalFunWin);break;//逻辑功能
		case 2:userGUIWindowAdd(&OverLineWarnWin);break;//越线报警
		case 3:userGUIWindowAdd(&OverLoadMuchWin);break;//重过载
		case 4:userGUIWindowAdd(&OverLoadWin);break;//过负荷
		case 5:userGUIWindowAdd(&OverVoltageWin);break;//过电压
		case 6:userGUIWindowAdd(&BatterySetWin);break;//电池设置
		case 7:userGUIWindowAdd(&AutoResetWin);break;//自动复归
		case 8:userGUIMenuAdd(&MenuM0S4S8);break;//其他设置
		default:break;
		}
	}
}

/**
  *@brief MenuM0S4 deal with function 配置查询
  *@param  None
  *@retval None
  */
static void MenuM0S5Fun(void)
{
	switch(keyStatus){
	case UpKey:
		userGUIMenuIremModfiy(0);
		break;
	case DownKey:
		userGUIMenuIremModfiy(1);
		break;
	case LeftKey:
	case CancelKey:
		userGUIMenuHide();
		break;
	case RightKey:
	case OkKey:
		switch(MenuM0S5.currentItem){
		case 0:userGUIWindowAdd(&BasicSetWin);break;
		case 1:userGUIWindowAdd(&ZeroDriftWin);break;
		case 2:userGUIWindowAdd(&DeadZoneWin);break;
		default:break;
		}
	}
}

/**
  *@brief MenuM0S4S0 deal with function 保护功能
  *@param  None
  *@retval None
  */
static void MenuM0S4S0Fun(void)
{
	switch(keyStatus){
	case UpKey:userGUIMenuIremModfiy(0);break;	
	case DownKey:userGUIMenuIremModfiy(1);break;	
	case LeftKey:
	case CancelKey:
		userGUIMenuHide();break;
	case RightKey:
	case OkKey:
		switch( MenuM0S4S0.currentItem){
		case 0:userGUIWindowAdd(&InterphaseOverWin);break;//相间过流
		case 1:userGUIWindowAdd(&ZeroSequenceOverWin);break;//零序过流
		case 2:userGUIWindowAdd(&CutoutSwitchWin);break;//断路器
		default:break;
		}
	}
}
/**
  *@brief MenuM1S0 deal with function 保护功能
  *@param  None
  *@retval None
  */
static void MenuM1S0Fun(void)
{
	switch(keyStatus){
	case UpKey:userGUIMenuIremModfiy(0);break;	
	case DownKey:userGUIMenuIremModfiy(1);break;	
	case LeftKey:
	case CancelKey:
		userGUIMenuHide();break;
	case RightKey:
	case OkKey:
		switch( MenuM1S0.currentItem){
		case 0:userGUIWindowAdd(&InterphaseOverWin);break;//相间过流
		case 1:userGUIWindowAdd(&ZeroSequenceOverWin);break;//零序过流
		case 2:userGUIWindowAdd(&CutoutSwitchWin);break;//断路器
		default:break;
		}
	}
}

/**
  *@brief MenuM1S8 deal with function 其他
  *@param  None
  *@retval None
  */
static void MenuM1S8Fun(void)
{
	switch(keyStatus){
	case UpKey:userGUIMenuIremModfiy(0);break;	
	case DownKey:userGUIMenuIremModfiy(1);break;	
	case LeftKey:
	case CancelKey:
		userGUIMenuHide();break;
	case RightKey:
	case OkKey:
		switch( MenuM1S8.currentItem){
		case 0:userGUIWindowAdd(&RingUniteWin);break;//合环
		case 1:userGUIWindowAdd(&BreakDownWin);break;//故障投退
		default:break;
		}
	}
}

/**
  *@brief CmdSend deal with function 命令下发
  *@param  None
  *@retval None
  */
static void CmdSendWinFun(void)
{
	const  uint8_t listCol[2][1] = {{156},{FONT_MID}};
	static SCROLL *Scroll;//进度条
	static ListColSet colset;
	static LIST  *list;//列表控件
	static uint8_t **pText;//列表内容指针
	static uint8_t messageFlag = 0;//步骤标志
	static int8_t currentNum;
	uint16_t memMall;
	uint8_t i;
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
		memMall = 0;	
		list = (LIST  *)&userGUIBuff[memMall];
		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
		Scroll = (SCROLL *)&userGUIBuff[memMall];
		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
		pText = (uint8_t **)&userGUIBuff[memMall];
		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = hcmdInfo.itemsNum;
		Scroll->lump = 1;
		colset.colWide = (uint8_t *)listCol[0];
		colset.colFlag = (uint8_t *)listCol[1];
		list->x = 0;
		list->y = 18;
		list->wide = 156;
		list->hight = 141;
		list->row = hcmdInfo.itemsNum;
		list->col = 1;
		list->drawRow = 0;
		list->currentRow = 0;
		list->flag = LIST_USEBORDER_H;
		list->content = (uint8_t **)pText;
		list->colSet = &colset;	
		currentNum = 0;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){//获取相应数据	
		for(i = 0;i < hcmdInfo.itemsNum;i++){
			*(pText + i) = hcmdInfo.pHmiCmd[i].name;
		}
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//显示
		list->currentRow = currentNum;
		Scroll->lump = currentNum + 1;
		if((list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW + DISPLAYLISTROW <= list->row){
			list->drawRow = (list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW;
		}
		else{
			if(list->row < DISPLAYLISTROW){
				list->drawRow = 0;
			}
			else{
				list->drawRow = list->row - DISPLAYLISTROW;
			}	
		}
		DrawList(list);
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
	}
	if(stepTab[STEP_NORMAL] == 4){
		if(messageFlag == 1){
			hcmdInfo.cmdfun(hcmdInfo.pHmiCmd[currentNum].cmd);
		}
		stepTab[STEP_NORMAL] = 2;
	}
	switch(keyStatus){
	case LeftKey:
	case UpKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = Scroll->max - 1;
		}
		stepTab[STEP_NORMAL] = 2;
		break;
	case RightKey:
	case DownKey:
		currentNum ++;
		if(currentNum > Scroll->max - 1){
			currentNum = 0;
		}
		stepTab[STEP_NORMAL] = 2;
		break;
	case OkKey:
		stepTab[STEP_NORMAL] = 4;
		messageFlag = 0;
		SetMessageWin(23,40,"确定执行?",&messageFlag);
		userGUIWindowAdd(&MessageWin);
		break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}

/**
  *@brief  版本信息数据处理
  *@param  None
  *@retval None
  */
static void ConstParDataResult(const uint8_t *pData,uint8_t *outData,uint8_t len)
{
	int8_t j;
	strncpy((char *)outData,(char *)pData,len);
	for(j = len - 1; j >= 0 ; j--){
		if(outData[j] == ' '){
			outData[j] = '\0';
		}
		else{
			outData[j + 1] = '\0';
			break;
		}
	}
}
/**
  *@brief  版本信息
  *@param  None
  *@retval None
  */
static void VersionWinFun(void)
{
	static SCROLL *Scroll;//进度条
	static uint8_t *col1Data;//显示内容指针		
	static int8_t currentNum;
	uint16_t memMall;
	uint8_t i,y;
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
		memMall = 0;
		col1Data = &userGUIBuff[memMall];
		memMall += 28;
		Scroll = (SCROLL *)&userGUIBuff[memMall];	
		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = versInfo.num / CONSTPARDISPLAYROW +\
			(versInfo.num % CONSTPARDISPLAYROW > 0 ? 1 : 0);
		Scroll->lump = 1;
		currentNum = 0;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){//获取相应数据
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//显示
		y = VersionWin.y + 18;
		GuiFillRect(VersionWin.x+1,y,155,158, backcolor);
		for(i = 0; i < CONSTPARDISPLAYROW; i++){
			if((i + currentNum * CONSTPARDISPLAYROW) >= versInfo.num){
				GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//水平线
				break;
			}
			GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//水平线		
			GuiFillRect(VersionWin.x+1,y + i*28+1,155,y + i*28+14, forecolor);
			GuiExchangeColor();
			GuiFont12Align(VersionWin.x + 2,y + i*28+2,133,FONT_LEFT,
				(uint8_t *)versInfo.pItems[i + currentNum * CONSTPARDISPLAYROW].pName);
			GuiExchangeColor();
			ConstParDataResult((uint8_t *)versInfo.pItems[i + currentNum * CONSTPARDISPLAYROW].pVal,col1Data,24);
			GuiFont12Align(VersionWin.x+2,y + i*28+15,153,FONT_MID,col1Data);
		}
		Scroll->lump = currentNum + 1;
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
	}
	if(keyStatus != CancelKey && stepTab[STEP_NORMAL] <= 1){
		keyStatus = NoKey;
	}
	switch(keyStatus){
	case LeftKey:
	case UpKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = Scroll->max - 1;
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case RightKey:
	case DownKey:
		currentNum ++;
		if(currentNum > Scroll->max - 1){
			currentNum = 0;
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case OkKey:break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}

/**
  *@brief  相间过流
  *@param  None
  *@retval None
  */
static void InterphaseOverFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_INTERGHASE],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 零序过流
  *@param  None
  *@retval None
  */
static void ZeroSequenceOverFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_ZERO_SEQUE],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 断路器
  *@param  None
  *@retval None
  */
static void CutoutSwitchFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_OTHER_PROTEC],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 逻辑功能
  *@param  None
  *@retval None
  */
static void LogicalFunFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_LOGICAL_FUN],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 越线报警
  *@param  None
  *@retval None
  */
static void OverLineWarnFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_LIMITATION],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 重过载
  *@param  None
  *@retval None
  */
static void OverLoadMuchFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_HEAVY_LOAD],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 过负荷
  *@param  None
  *@retval None
  */
static void OverLoadFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_OVERLOAD],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 过电压
  *@param  None
  *@retval None
  */
static void OverVoltageFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_LIMIT_V_F],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 电池设置
  *@param  None
  *@retval None
  */
static void BatterySetFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_IBATTERY_SET],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 自动复归
  *@param  None
  *@retval None
  */
static void AutoResetFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_AUTO_RESET],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 合环
  *@param  None
  *@retval None
  */
static void RingUniteFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_LOOP_CLOSE],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 故障投退
  *@param  None
  *@retval None
  */
static void BreakDownFun(void)
{
	DZModfiyDisplay(&dzhi1Info[DZ1_FAULT_SWITCH],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 遥信处理函数
  *@param  None
  *@retval None
  */
static void YaoxinFun(void)
{
	const  uint8_t listCol[2][2] = {{90,66},{FONT_LEFT,FONT_MID}};
	static SCROLL *Scroll;//进度条
	static ListColSet colset;
	static LIST  *list;//列表控件
	static uint8_t **pText;//列表内容指针
	static uint8_t *col1Data;//显示内容指针	
	static uint32_t YaoXinTick;
	
	uint16_t memMall;
	uint8_t i;
	uint8_t itemsNum;
	
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
		itemsNum = yxInfo.Num;
		
		memMall = 0;
		list = (LIST  *)&userGUIBuff[memMall];
		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
		Scroll = (SCROLL *)&userGUIBuff[memMall];
		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
		pText = (uint8_t **)&userGUIBuff[memMall];
		memMall += itemsNum*2*4;
		col1Data = &userGUIBuff[memMall];

		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = itemsNum/DISPLAYLISTROW+(itemsNum%DISPLAYLISTROW >0 ?1:0);
		Scroll->lump = 1;
		colset.colWide = (uint8_t *)listCol[0];
		colset.colFlag = (uint8_t *)listCol[1];
		list->x = 0;
		list->y = 18;
		list->wide = 156;
		list->hight = 141;
		list->row = itemsNum;
		list->col = 2;
		list->drawRow = 0;
		list->currentRow = 255;
		list->flag = LIST_USEBORDER_H;
		list->content = (uint8_t **)pText;
		list->colSet = &colset;	
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){//获取相应数据
		itemsNum = yxInfo.Num;
		for(i = 0;i < itemsNum;i++){
			col1Data[i] = (uint8_t )*(yxInfo.pRoot[yxInfo.pBuff[i]].pVal);
		}	
		for(i = 0;i < itemsNum;i++){
			*(pText + i*2 + 0) = (uint8_t *)yxInfo.pRoot[i].pName;
			if(col1Data[i] == 1)
				*(pText + i*2 + 1) = (uint8_t *)yxInfo.pRoot[yxInfo.pBuff[i]].pContentYx[0];
			else
				*(pText + i*2 + 1) = (uint8_t *)yxInfo.pRoot[yxInfo.pBuff[i]].pContentYx[1];
		}
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//显示
		if(list->drawRow == list->row - DISPLAYLISTROW){
			Scroll->lump = Scroll->max;
		}
		else{
			Scroll->lump = list->drawRow/DISPLAYLISTROW + 1;
		}
		DrawList(list);
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		YaoXinTick = getCurrentTick();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){//检测遥信变化
		if(GetIntervalTick(YaoXinTick) > 1000){
			stepTab[STEP_NORMAL] = 1;
		}
	}

	switch(keyStatus){
	case UpKey:
		if(list->drawRow > DISPLAYLISTROW)
			list->drawRow -= DISPLAYLISTROW;
		else
			list->drawRow = 0;
		stepTab[STEP_NORMAL] = 2;
		break;	
	case DownKey:	
		list->drawRow += DISPLAYLISTROW;
		if(list->drawRow + DISPLAYLISTROW > list->row){
			list->drawRow = list->row - DISPLAYLISTROW;
		}
		stepTab[STEP_NORMAL] = 2;
		break;
	case LeftKey:break;
	case RightKey:break;
	case OkKey:break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}

/**
  *@brief 遥测处理显示
  *@param  items 遥测显示项
  *@param  itemsNum 遥测数量
  *@param  items modbus命令
  *@retval None
  */
void yaoCeDisplay(YaoceDisplayInfo *info)
{
	const uint8_t listCol[2][YaoCeDISPLAYCOL] = {//表格列修饰
		{50,75,31},{FONT_LEFT,FONT_MID,FONT_MID}};
	static SCROLL *Scroll;//进度条
	static ListColSet colset;
	static LIST  *list;//列表控件
	static uint8_t **pText;//列表内容指针
	static uint8_t *col1Data;//显示内容指针		
	static uint32_t YaoCeTick;//循环计时
	
	uint16_t memMall;
	uint8_t i;
	uint8_t itemsNum = info->num;/* 可显示条目 */
	float tempFloat;	
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
		memMall = 0;	
		list = (LIST  *)&userGUIBuff[memMall];
		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
		Scroll = (SCROLL *)&userGUIBuff[memMall];
		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
		pText = (uint8_t **)&userGUIBuff[memMall];
		memMall += itemsNum*YaoCeDISPLAYCOL*4;
		col1Data = &userGUIBuff[memMall];
		
		Scroll->x = 156;
		Scroll->y = 18;
		Scroll->hight = 141;
		Scroll->max = itemsNum / DISPLAYLISTROW + (itemsNum % DISPLAYLISTROW > 0 ? 1 : 0);
		Scroll->lump = 1;
		colset.colWide = (uint8_t *)listCol[0];
		colset.colFlag = (uint8_t *)listCol[1];
		list->x = 0;
		list->y = 18;
		list->wide = 156;
		list->hight = 141;
		list->row = itemsNum;
		list->col = YaoCeDISPLAYCOL;
		list->drawRow = 0;
		list->currentRow = 255;
		list->flag = LIST_USEBORDER_H;
		list->content = (uint8_t **)pText;
		list->colSet = &colset;
		//获取数据中
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){
		for(i = 0;i < itemsNum;i++){
			tempFloat = *(info->pRoot[info->pBuff[i]].pVal);
			if(((uint8_t)isnan(tempFloat)) != 0){
				sprintf((char *)&col1Data[i*16],"%s","NaN");
			}
			else{
				sprintf((char *)&col1Data[i*16],"%.3f",tempFloat);
			}
			col1Data[i*16 + 15] = '\0';
			*(pText + i*YaoCeDISPLAYCOL + 0) = (uint8_t *)info->pRoot[info->pBuff[i]].pName;
			*(pText + i*YaoCeDISPLAYCOL + 1) = &col1Data[i*16];
			*(pText + i*YaoCeDISPLAYCOL + 2) = (uint8_t *)info->pRoot[info->pBuff[i]].pUnit;
		}
		stepTab[STEP_NORMAL] = 2;
	}
	if(stepTab[STEP_NORMAL] == 2){//显示	
		if(list->drawRow == list->row - DISPLAYLISTROW){
			Scroll->lump = Scroll->max;
		}
		else{
			Scroll->lump = list->drawRow/DISPLAYLISTROW +\
				1 + (list->drawRow % DISPLAYLISTROW > 0 ? 1 : 0);
		}
		DrawList(list);
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		YaoCeTick = getCurrentTick();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
		if(GetIntervalTick(YaoCeTick) > YAOCE_UPDATATIME){//检测更新
			stepTab[STEP_NORMAL] = 1;
		}
	}
	switch(keyStatus){
	case UpKey:
		if(stepTab[STEP_NORMAL] > 1){
			if(list->drawRow > DISPLAYLISTROW)
				list->drawRow -= DISPLAYLISTROW;
			else
				list->drawRow = 0;
			stepTab[STEP_NORMAL] = 2;
			break;
		}	
	case DownKey:
		if(stepTab[STEP_NORMAL] > 1){
			list->drawRow += DISPLAYLISTROW;
			if(list->drawRow + DISPLAYLISTROW > list->row){
				list->drawRow = list->row - DISPLAYLISTROW;
			}
			stepTab[STEP_NORMAL] = 2;
			break;
		}
	case LeftKey:break;
	case RightKey:break;
	case OkKey:break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}

/**
  *@brief 一次遥测处理函数
  *@param  None
  *@retval None
  */
static void YaoCe1Fun(void)
{
	yaoCeDisplay(&yceInfo[0]);
}

/**
  *@brief 二次遥测处理函数
  *@param  None
  *@retval None
  */
static void YaoCe2Fun(void)
{
	yaoCeDisplay(&yceInfo[1]);
}
/**
  *@brief 谐波处理函数
  *@param  None
  *@retval None
  */
static void HarmonicFun(void)
{
	yaoCeDisplay(&yceInfo[2]);
}


/**
  *@brief SOE处理函数
  *@param  None
  *@retval None
  */
static void SOEFun(void)
{
	static struct SOEDisplay *soeStr;
	static SCROLL Scroll ={156,18,141,2,1};//进度条
	static uint8_t currentNum;
	SystemTimeDisplay *tTim;
	uint8_t tRes;
	uint8_t i;
	uint16_t y;
	
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
		soeStr = (struct SOEDisplay *)&userGUIBuff[0];
		soeStr->pRead = 0;
		soeStr->allNum = GetSoeNum();
		currentNum = 1;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){
		if(soeStr->allNum == 0){//没有SOE
			GuiFont12Align(SOEWin.x+2,SOEWin.y + 30,SOEWin.x+SOEWin.wide-4,FONT_MID,"当前没有SOE");
			GuiUpdateDisplayAll();
			stepTab[STEP_NORMAL] = 3;
		}
		else{
			if(soeStr->allNum % SOEDISPLAYROW){
				Scroll.max = soeStr->allNum / SOEDISPLAYROW + 1;
			}
			else{
				Scroll.max = soeStr->allNum / SOEDISPLAYROW;
			}
			stepTab[STEP_NORMAL] = 2;
		}
	}
	if(stepTab[STEP_NORMAL] == 2){
		soeStr->pRead = (currentNum - 1) * SOEDISPLAYROW;
		if(soeStr->pRead >= soeStr->allNum){//SOE最后一条
			stepTab[STEP_NORMAL] = 3;
		}
		y = SOEWin.y + 18;
		GuiFillRect(SOEWin.x+1,y,155,158, backcolor);
		for(i = 0; i < SOEDISPLAYROW; i ++){
			if(soeStr->pRead >= soeStr->allNum){//SOE最后一条
				GuiHLine(SOEWin.x,y+i*28,155,forecolor);
				break;
			}
			tRes = GetSoeNoContent(soeStr->pRead,&soeStr->pSoe);
			if(tRes != 0){//获取SOE有错
				GuiHLine(SOEWin.x,y+i*28,155,forecolor);
				break;
			}
			tTim = &soeStr->pSoe.time;
			sprintf((char *)soeStr->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
				tTim->year,tTim->month,tTim->day,tTim->hour,tTim->min,tTim->s,tTim->ms);
			soeStr->time[23] = '\0';
			sprintf((char *)soeStr->itemNum,"%d",soeStr->pRead + 1);
			soeStr->itemNum[3] = '\0';
			GuiHLine(SOEWin.x,y+i*28+0,155,forecolor);//水平线
			GuiFillRect(SOEWin.x+1,y+i*28,SOEWin.x+20,y+i*28+13, forecolor);
			GuiExchangeColor();
			GuiFont12Align(SOEWin.x+1,y + i*28+1,19,FONT_MID,soeStr->itemNum);//序号
			GuiExchangeColor();	
			GuiRPointLine(SOEWin.x+20,y+i*28+1,y+i*28+13,2,forecolor);//垂直线
			GuiFont12Align(SOEWin.x + 21,y + i*28+2,133,FONT_RIGHT,soeStr->time);
			GuiHPointLine(SOEWin.x,y+i*28+13,155,2,forecolor);
			GuiFont12Align(SOEWin.x+2,y + i*28+15,72,FONT_MID,soeStr->pSoe.pName);
			GuiRPointLine(SOEWin.x+10+72,y+i*28+15,y+i*28+27,2,forecolor);
			if(soeStr->pSoe.highlight){//高亮显示
				GuiFillRect(SOEWin.x+10+73,y+i*28+14,158,y+i*28+28, forecolor);
				GuiExchangeColor();
				GuiFont12Align(SOEWin.x+10+73,y+i*28+15,70,FONT_RIGHT,soeStr->pSoe.pContent);
				GuiExchangeColor();
			}
			else{
				GuiFont12Align(SOEWin.x+10+73,y+i*28+15,70,FONT_RIGHT,soeStr->pSoe.pContent);
			}
			soeStr->pRead ++;
		}
		Scroll.lump = currentNum;
		GuiVScroll(&Scroll);
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
		if(CheckSoeUpdata()){//SOE有更新
			soeStr->allNum = GetSoeNum();
			soeStr->pRead = 0;
			currentNum = 1;
			stepTab[STEP_NORMAL] = 1;
		}
	}
	switch(keyStatus){
	case UpKey:
	case LeftKey:
		if(stepTab[STEP_NORMAL] > 1){
			currentNum --;
			if(currentNum < 1){
				currentNum = Scroll.max;
			}
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case DownKey:
	case RightKey:
		if(stepTab[STEP_NORMAL] > 1){
			currentNum ++;
			if(currentNum > Scroll.max){
				currentNum = 1;
			}
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case OkKey:break;
	case CancelKey:
		currentNum = 0;
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}

/**
  *@brief 故障事件处理函数
  *@param  None
  *@retval None
  */
static void FaultEventFun(void)
{
	static struct EventDisplay *pEventStr;
	static SCROLL Scroll ={156,18,141,2,1};//进度条
	static SCROLL yaoCeScroll ={153,47,112,2,1};//进度条
	static int16_t currentNum;
	static int8_t yaoCeNum;
	uint8_t i;
	uint16_t y;
	
	if(stepTab[STEP_NORMAL] == 0){//初始化，分配内存
		pEventStr = (struct EventDisplay *)&userGUIBuff[0];
		pEventStr->pRead = 0;
		pEventStr->allNum = GetFeventNum();
		currentNum = 1;
		yaoCeNum = 1;
		stepTab[STEP_NORMAL] = 1;
	}
	if(stepTab[STEP_NORMAL] == 1){
		if(pEventStr->allNum == 0){//没有故障事件
			GuiFont12Align(FaultEventWin.x+2,FaultEventWin.y + 50,\
			FaultEventWin.x+FaultEventWin.wide-4,FONT_MID,"当前没有故障事件");
			GuiUpdateDisplayAll();
			stepTab[STEP_NORMAL] = 4;
		}
		else{
			pEventStr->pRead = currentNum - 1;
			GetFeventNoContent(pEventStr->pRead,&pEventStr->pFevent);
			Scroll.max = pEventStr->allNum;
			if(pEventStr->pFevent.yaoceNum <= 8){
				yaoCeScroll.max = 1;
			}
			else{
				yaoCeScroll.max = pEventStr->pFevent.yaoceNum - 8 + 1;
			}
			stepTab[STEP_NORMAL] = 2;
		}
	}
	if(stepTab[STEP_NORMAL] == 2){//画图
		y = FaultEventWin.y + 18;
		GuiFillRect(FaultEventWin.x+1,y,155,158, backcolor);
		GuiHLine(FaultEventWin.x,y,155,forecolor);//水平线	
		GuiFillRect(FaultEventWin.x+1,y,FaultEventWin.x+20,y+13, forecolor);
		SystemTimeDisplay *tTim;
		tTim = &pEventStr->pFevent.time;
		sprintf((char *)pEventStr->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
			tTim->year,tTim->month,tTim->day,tTim->hour,tTim->min,tTim->s,tTim->ms);
		pEventStr->time[23] = '\0';
		sprintf((char *)pEventStr->itemNum,"%d",pEventStr->pRead + 1);
		pEventStr->itemNum[3] = '\0';
		GuiExchangeColor();
		GuiFont12Align(FaultEventWin.x+1,y+2,20,FONT_MID,pEventStr->itemNum);//序号
		GuiExchangeColor();	
		GuiRPointLine(FaultEventWin.x+20,y+1,y+13,2,forecolor);//垂直线
		GuiFont12Align(FaultEventWin.x + 21,y+2,133,FONT_RIGHT,pEventStr->time);
		GuiHPointLine(FaultEventWin.x,y+13,155,2,forecolor);
		GuiFont12Align(FaultEventWin.x+2,y+15,72,FONT_LEFT,pEventStr->pFevent.pName);
		GuiRPointLine(FaultEventWin.x+10+72,y+15,y+27,2,forecolor);
		GuiFont12Align(FaultEventWin.x+10+73,y+15,70,FONT_RIGHT,pEventStr->pFevent.pContent);	
		Scroll.lump = currentNum;
		GuiVScroll(&Scroll);
		GuiHLine(FaultEventWin.x,y+28,155,forecolor);//水平线	
		stepTab[STEP_NORMAL] = 3;
	}
	if(stepTab[STEP_NORMAL] == 3){
		y = FaultEventWin.y + 18 + 29;
		yaoCeScroll.lump = yaoCeNum;
		GuiVScroll(&yaoCeScroll);
		GuiFillRect(FaultEventWin.x+1,y,151,158, backcolor);
		for(i = 0;i < 8;i ++){
			GuiFont12Align(FaultEventWin.x+2,y+1+i*14,40,FONT_LEFT,\
				pEventStr->pFevent.pYaoceName[i + yaoCeNum - 1]);			
			GuiRPointLine(FaultEventWin.x+43,y+i*14,y+13+i*14,2,forecolor);
			sprintf((char *)pEventStr->yaoceValue,"%.3f",pEventStr->pFevent.pYaoceValue[i + yaoCeNum - 1]);
			pEventStr->yaoceValue[15] = '\0';
			GuiFont12Align(FaultEventWin.x+44,y+1+i*14,70,FONT_MID,pEventStr->yaoceValue);
			GuiRPointLine(FaultEventWin.x+115,y+i*14,y+13+i*14,2,forecolor);
			GuiFont12Align(FaultEventWin.x+116,y+1+i*14,35,FONT_RIGHT,\
				pEventStr->pFevent.pYaoceUnit[i + yaoCeNum - 1]);		
			GuiHPointLine(FaultEventWin.x,y+13+i*14,151,2,forecolor);
		}
		GuiUpdateDisplayAll();
		stepTab[STEP_NORMAL] = 4;
	}
	if(stepTab[STEP_NORMAL] == 4){
		if(CheckFeventUpdata()){//SOE有更新
			currentNum = 1;
			stepTab[STEP_NORMAL] = 1;
		}
	}
	
	switch(keyStatus){
	case UpKey:
		if(stepTab[STEP_NORMAL] > 3){
			yaoCeNum --;
			if(yaoCeNum < 1){
				yaoCeNum = yaoCeScroll.max;
			}
		}
		stepTab[STEP_NORMAL] = 3;
		break;
	case LeftKey:
		if(stepTab[STEP_NORMAL] > 1){
			currentNum --;
			if(currentNum < 1){
				currentNum = Scroll.max;
			}
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case DownKey:
		if(stepTab[STEP_NORMAL] > 3){
			yaoCeNum ++;
			if(yaoCeNum > yaoCeScroll.max){
				yaoCeNum = 1;
			}
		}
		stepTab[STEP_NORMAL] = 3;
		break;
	case RightKey:
		if(stepTab[STEP_NORMAL] > 1){
			currentNum ++;
			if(currentNum > Scroll.max){
				currentNum = 1;
			}
		}
		stepTab[STEP_NORMAL] = 1;
		break;
	case OkKey:break;
	case CancelKey:
		stepTab[STEP_NORMAL] = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}

/**
  *@brief 基本设置处理函数
  *@param  None
  *@retval None
  */
static void BasicSetFun(void)
{
	DZModfiyDisplay(&dzhi0Info[DZ0_CONFIG],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 零漂设置处理函数
  *@param  None
  *@retval None
  */
static void ZeroDriftFun(void)
{
	DZModfiyDisplay(&dzhi0Info[DZ0_ZERODRIFT],&stepTab[STEP_NORMAL]);
}

/**
  *@brief 死区设置处理函数
  *@param  None
  *@retval None
  */
static void DeadZoneFun(void)
{
	DZModfiyDisplay(&dzhi0Info[DZ0_DEADEZONE],&stepTab[STEP_NORMAL]);
}

/* END */

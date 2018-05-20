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
	userGUIWindowAdd(&HomeWindow);
	YaoxinDisplayInit();
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
//	keyStatus = GetKeyStatus();//获取按键状态
//	SetKeyIsNoKey();
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
//		userGUITopWindowDisplay();
//	}
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
	static uint8_t flag;
	static uint8_t passWordStr[8];
	static int8_t currentNum;
	uint8_t tempStr[2];
	uint8_t i;
	if(flag == 0){
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
		flag = 1;
	}
	if(flag == 1){
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
		flag = 2;
	}
	switch(keyStatus){
	case LeftKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = 5;
		}
		flag = 1;
		break;
	case DownKey:
		passWordStr[currentNum] --;
		if(passWordStr[currentNum]<'0'){
			passWordStr[currentNum] = '9';
		}
		flag = 1;
		break;
	case RightKey:
		currentNum ++;
		if(currentNum >= 6){
			currentNum = 0;
		}
		flag = 1;
		break;
	case UpKey:
		passWordStr[currentNum] ++;
		if(passWordStr[currentNum] > '9'){
			passWordStr[currentNum] = '0';
		}
		flag = 1;
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
		flag = 0; 
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
	static uint8_t flag;
	static int8_t keyFlag;
	uint8_t winWide;
	uint8_t winHight;
	if(flag == 0){
		keyFlag = 0;
		flag = 1;
	}
	if(flag == 1){
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
	}
	switch(keyStatus){
	case LeftKey:
		keyFlag--;
		if(keyFlag < 0){
			keyFlag = 1;
		}break;
	case UpKey:break;
	case RightKey:
		keyFlag++;
		if(keyFlag > 1){
			keyFlag = 0;
		}break;
	case DownKey:break;
	case OkKey:
		*MessageIs.flag = keyFlag;
		flag = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	case CancelKey:
		*MessageIs.flag = 0;
		flag = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	default:break;
	}
}

/**
  *@brief  设置定值修改相关变量
  *@param  pDingZhiItems 定值结构体指针
  *@param  pDingZhiItems modbus指令
  *@param  flag 标志字节，未使用
  *@param  itemIs 定值当前Item
  *@param  str 要修改字符串的指针
  *@retval None
  */
//static void SetDingZhiModfiy(const struct DingZhi1Item *pDingZhiItems,\
//	const struct ModbusCmd_ *cmd,uint8_t flag,uint8_t itemIs,uint8_t *str)
//{
//	dZModfiy.Items = pDingZhiItems;
//	dZModfiy.cmd = cmd;
//	dZModfiy.flag = flag;
//	dZModfiy.itemIs = itemIs;
//	dZModfiy.str = str;	
//}

///**
//  *@brief 定值修改初始化
//  *@param  None
//  *@retval None
//  */
//void DZModfiyInit(void)
//{
//	int16_t x,y;
//	GuiRect(DZModfiyWin.x,DZModfiyWin.y,DZModfiyWin.x + DZModfiyWin.wide, \
//		DZModfiyWin.y + DZModfiyWin.hight, forecolor);
//	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 17,150,FONT_LEFT,\
//		dZModfiy.Items[dZModfiy.itemIs].name);
//	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 30,\
//		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + DZModfiyWin.hight - 16,\
//		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);//状态栏
//	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + DZModfiyWin.hight - 14,80,FONT_LEFT,"当前状态:");
//	if(dZModfiy.Items[dZModfiy.itemIs].flag){//汉字
//		x = DZModfiyWin.x + DZModfiyWin.wide;
//		y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
//		GuiFillRect(DZModfiyWin.x+9,y,x - 10,y + 17, backcolor);
//		GuiFillRect(DZModfiyWin.x+17,y + 2,x - 20,y + 15, forecolor);
//		GuiFont12Align(DZModfiyWin.x+7,y+4,10,FONT_LEFT,"<");
//		GuiFont12Align(x - 14,y+4,10,FONT_LEFT,">");
//	}
//	else{//float
//		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 32,40,FONT_LEFT,"最大值");
//		GuiFont12Align(DZModfiyWin.x + 50,\
//			DZModfiyWin.y + 32,80,FONT_MID,dZModfiy.Items[dZModfiy.itemIs].max);
//		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 45,\
//			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 47,40,FONT_LEFT,"最小值");
//		GuiFont12Align(DZModfiyWin.x + 50,\
//			DZModfiyWin.y + 47,80,FONT_MID,dZModfiy.Items[dZModfiy.itemIs].min);
//		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 60,\
//			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 62,40,FONT_LEFT,"当前值");
//		
//		GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 62,\
//			DZModfiyWin.x + 128,DZModfiyWin.y + 74, backcolor);
//		GuiFont12Align(DZModfiyWin.x + 50,\
//			DZModfiyWin.y + 62,80,FONT_MID,dZModfiy.str);
//		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 75,\
//			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 77,40,FONT_LEFT,"修改为");
//		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 90,\
//			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
//		GuiRPointLine(DZModfiyWin.x + 47,DZModfiyWin.y + 30,\
//			DZModfiyWin.y + 90,2,forecolor);	
//	}
//}

///**
//  *@brief 画定值修改按键
//  *@param  None
//  *@retval None
//  */
//void DrawModfiyKey(uint8_t key)
//{
//	uint8_t i;
//	int16_t x,y;
//	for(i=0;i<14;i++){
//		if(i < 7){
//			x = DZModfiyWin.x + i*18;
//			y = DZModfiyWin.y + 93;
//		}
//		else{
//			x = DZModfiyWin.x + (i - 7)*18;
//			y = DZModfiyWin.y + 105;
//		}
//		if(i == key){
//			GuiFillRect(x,y,x + 18,y + 11, forecolor);
//			GuiExchangeColor();
//			GuiFont12Align(x+1,y,18,FONT_MID,(uint8_t *)modfiyKey[i]);
//			GuiExchangeColor();
//		}
//		else{
//			GuiFillRect(x,y,x + 18,y + 11, backcolor);
//			GuiFont12Align(x+1,y,18,FONT_MID,(uint8_t *)modfiyKey[i]);
//		}
//		GuiRLine(x,y,y + 10,forecolor);
//	}
//	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 92,\
//		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
//	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 104,\
//		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
//	GuiHLine(DZModfiyWin.x,DZModfiyWin.y + 116,\
//		DZModfiyWin.x + DZModfiyWin.wide,forecolor);
//}

///**
//  *@brief 定值修改窗口函数
//  *@param  None
//  *@retval None
//  */
//void DZModfiyFun(void)
//{
//	static uint8_t flag = 0;//步骤
//	static uint8_t modfiyStr[16];//修改暂存buff
//	static uint8_t modfiyStrP = 0;
//	static uint8_t keyIs = 0;//按键记录
//	static uint32_t inputCursorTick;//光标闪烁记时
//	static uint8_t getDataFlag;
//	int16_t x,y;
//	uint8_t tempRetrun;
//	static union{
//		float f32;
//		uint16_t u16[2];
//	}ftrans16;
//	float fValue;
//	if(flag == 0){//初始化
//		DZModfiyInit();
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){
//			keyIs = dZModfiy.str[0];
//		}
//		flag = 1;
//		getDataFlag = 0;
//	}
//	if(flag == 1){//显示
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){//汉字处理
//			GuiExchangeColor();
//			x = DZModfiyWin.x + DZModfiyWin.wide;
//			y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
//			GuiFillRect(DZModfiyWin.x+18,y + 3,\
//				x - 20,y + 15, backcolor);
//			GuiFont12Align(DZModfiyWin.x+18,y + 3,x - DZModfiyWin.x - 40,\
//				FONT_MID,dZModfiy.Items[dZModfiy.itemIs].pContent[keyIs]);
//			GuiExchangeColor();
//			flag = 3;
//		}
//		else{//float处理
//			DrawModfiyKey(keyIs);
//			GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 77,\
//				DZModfiyWin.x + 128,DZModfiyWin.y + 89, backcolor);
//			GuiFont12Align(DZModfiyWin.x + 50,\
//				DZModfiyWin.y + 77,78,FONT_MID,modfiyStr);
//			flag = 2;
//		}
//		GuiUpdateDisplayAll();
//	}
//	if(flag == 2){//光标闪动处理
//		if(dZModfiy.Items[dZModfiy.itemIs].flag == 0){
//			if(GetIntervalTick(inputCursorTick) < 500){
//				if(modfiyStr[modfiyStrP] != '|'){
//					modfiyStr[modfiyStrP] = '|';
//					modfiyStr[modfiyStrP + 1] = '\0';
//					flag = 1;
//				}
//			}
//			else if(GetIntervalTick(inputCursorTick) < 1000){
//				if(modfiyStr[modfiyStrP] != ' '){
//					modfiyStr[modfiyStrP] = ' ';
//					modfiyStr[modfiyStrP + 1] = '\0';
//					flag = 1;
//				}
//			}
//			else{
//				inputCursorTick = getCurrentTick();
//			}				
//		}
//	}
//	if(flag == 3){//等待
//	}
//	if(flag == 4){//发送命令
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){//汉字		
//			ftrans16.f32 = (float)modfiyStr[0];
//			GuiFillRect(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,\
//				DZModfiyWin.x + DZModfiyWin.wide - 2,\
//				DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//			GuiFont12Align(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"修改中...");
//			flag = 5;
//		}
//		else{
//			modfiyStr[modfiyStrP] = '\0';
//			fValue = atof((const char *)modfiyStr);
//			if(fValue <= atof((const char *)dZModfiy.Items[dZModfiy.itemIs].max) &&\
//				fValue >= atof((const char *)dZModfiy.Items[dZModfiy.itemIs].min)){
//				ftrans16.f32 = fValue;
//				GuiFillRect(DZModfiyWin.x + 57,\
//					DZModfiyWin.y + DZModfiyWin.hight - 14,\
//					DZModfiyWin.x + DZModfiyWin.wide - 2,\
//					DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//				GuiFont12Align(DZModfiyWin.x + 57,\
//					DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"修改中...");
//				flag = 5;
//			}
//			else{//超出范围
//				GuiFillRect(DZModfiyWin.x + 57,\
//					DZModfiyWin.y + DZModfiyWin.hight - 14,\
//					DZModfiyWin.x + DZModfiyWin.wide - 2,\
//					DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//				GuiFont12Align(DZModfiyWin.x + 57,\
//					DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"超出范围");
//				flag = 1;
//			}
//		}
//	}
//	if(flag == 5){//发送完后等待回复
//		tempRetrun = SetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//			dZModfiy.cmd->addr.addr16 +(dZModfiy.Items[dZModfiy.itemIs].addr -\
//			dZModfiy.cmd->mapAddr.addr16)*2,2,&ftrans16.u16[0]);
//		if(tempRetrun == ISTURE){//修改成功
//			if(dZModfiy.Items[dZModfiy.itemIs].flag){
//				dZModfiy.str[0] = modfiyStr[0];
//			}
//			else{
//				strcpy((char *)dZModfiy.str,(char *)modfiyStr);
//			}
//			GuiFillRect(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,\
//				DZModfiyWin.x + DZModfiyWin.wide - 2,\
//				DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//			GuiFont12Align(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"修改成功");
//			flag = 0;
//		}
//		else if(tempRetrun == ISFAILTURE){//修改失败
//			getDataFlag = 0;
//			GuiFillRect(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,\
//				DZModfiyWin.x + DZModfiyWin.wide - 2,\
//				DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
//			GuiFont12Align(DZModfiyWin.x + 57,\
//				DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"修改失败");
//			flag = 1;
//		}
//	}
//	switch(keyStatus){
//	case UpKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag == 0){
//			if(keyIs < 7)
//				keyIs += 7;
//			else
//				keyIs -= 7;
//		}
//		flag = 1;break;
//	case DownKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag == 0){
//			if(keyIs >= 7)
//				keyIs -= 7;
//			else
//				keyIs += 7;
//		}
//		flag = 1;break;
//	case LeftKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){
//			if(keyIs == 0)
//				keyIs = dZModfiy.Items[dZModfiy.itemIs].flag - 1;
//			else
//				keyIs --; 
//		}
//		else{
//			if(keyIs == 0 || keyIs == 7){	
//				keyIs += 6;
//			}
//			else{
//				keyIs --;
//			}
//		}
//		flag = 1;break;
//	case RightKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){
//			if(keyIs == (dZModfiy.Items[dZModfiy.itemIs].flag - 1))
//				keyIs = 0;
//			else
//				keyIs ++; 
//		}
//		else{
//			if(keyIs == 6 || keyIs == 13)
//				keyIs -= 6;
//			else
//				keyIs ++;
//		}
//		flag = 1;break;
//	case OkKey:
//		if(dZModfiy.Items[dZModfiy.itemIs].flag){//命令发送
//			modfiyStr[0] = keyIs;
//			flag = 4;
//		}
//		else{
//			if(keyIs < 12 && keyIs != 6){
//				if(modfiyStrP < 12){
//					modfiyStr[modfiyStrP] = *modfiyKey[keyIs];
//					modfiyStrP ++;
//				}
//			}
//			else if(keyIs == 6){//esc
//				modfiyStr[1] = '\0';
//				modfiyStrP = 0;
//				keyIs = 0;
//				flag = 0;
//				userGUITopWindowHide();
//			}
//			else if(keyIs == 12){
//				if(modfiyStrP > 0){
//					modfiyStr[modfiyStrP] = '\0';
//					modfiyStrP --;
//				}			
//			}
//			else if(keyIs == 13){//ok
//				flag = 4;//命令发送
//			}
//		}break;
//	case CancelKey:
//		modfiyStr[1] = '\0';
//		modfiyStrP = 0;
//		keyIs = 0;
//		flag = 0;
//		userGUITopWindowHide();
//		break;
//	default: break;
//	}
//}


///**
//  *@brief 定值数据处理
//  *@param  None
//  *@retval None
//  */
//void DingZhiDataResult(uint8_t *pData,uint8_t *outData,const struct DingZhi1Item *items,\
//	uint8_t itemsNum,const struct ModbusCmd_ *modbusCmd)
//{
//	union{
//		float f32;
//		uint8_t u8[4];
//	}tempFloat;
//	
//	uint16_t mapAddr;
//	uint8_t i;
//	
//	for(i=0;i<itemsNum;i++){
//		mapAddr = items[i].addr - modbusCmd->mapAddr.addr16;
//		tempFloat.u8[3] = pData[4*mapAddr];
//		tempFloat.u8[2] = pData[4*mapAddr + 1];
//		tempFloat.u8[1] = pData[4*mapAddr + 2];
//		tempFloat.u8[0] = pData[4*mapAddr + 3];
//		if(items[i].flag){//汉字
//			if((uint32_t)tempFloat.f32 < items[i].flag){
//				outData[i*16] = (uint32_t)tempFloat.f32;
//			}
//			else{
//				outData[i*16] = 0;
//			}
//		}
//		else{
//			sprintf((char *)&outData[i*16],"%.3f",tempFloat.f32);
//		}
//	}
//}

///**
//  *@brief 通用定值显示函数
//  *@param  Items 定值结构指针
//  *@param  itemsNum 定值数
//  *@param  cmd 命令
//  *@param  flag 步骤标志
//  *@retval None
//  */
//void DZModfiyDisplay(const struct DingZhi1Item *Items,\
//	uint8_t itemsNum,const struct ModbusCmd_ *modbusCmd,uint8_t *flag)
//{
//	const uint8_t listCol[2][DZDISPLAYCOL] = {//表格列修饰
//		{73,63,20},{COL_LEFT_DISPLAY,COL_MID_DISPLAY,COL_MID_DISPLAY}
//	};
//	static SCROLL *Scroll;//进度条
//	static ListColSet colset;
//	static LIST  *list;//列表控件
//	static uint8_t **pText;//列表内容指针
//	static uint8_t *col1Data;//显示内容指针
//	static uint8_t getDataFlag;
//	uint8_t tempRetrun;
//	uint8_t i;
//	uint16_t j ;
//	uint16_t memMall;//用于内存分配
//	uint32_t pData;
//	if(*flag == 0){//初始化
//		memMall = 0;
//		list = (LIST  *)&userGUIBuff[memMall];
//		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
//		Scroll = (SCROLL *)&userGUIBuff[memMall];
//		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
//		pText = (uint8_t **)&userGUIBuff[memMall];
//		memMall += itemsNum*DZDISPLAYCOL*4;
//		col1Data = &userGUIBuff[memMall];
//		Scroll->x = 156;
//		Scroll->y = 18;
//		Scroll->hight = 141;
//		Scroll->max = itemsNum;
//		Scroll->lump = 1;
//		colset.colWide = (uint8_t *)listCol[0];
//		colset.colFlag = (uint8_t *)listCol[1];
//		list->x = 0;
//		list->y = 18;
//		list->wide = 156;
//		list->hight = 141;
//		list->row = itemsNum;
//		list->col = DZDISPLAYCOL;
//		list->drawRow = 0;
//		list->currentRow = 0;
//		list->flag = LIST_USEBORDER_H;
//		list->content = (uint8_t **)pText;
//		list->colSet = &colset;
//		*flag = 1;
//		for(j=0;j<itemsNum*16;j++){
//			col1Data[j] = 0;
//		}
//		getDataFlag = 0;
//	}
//	if(*flag == 1){
//		tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//			modbusCmd->addr.addr16,modbusCmd->length.len16,&pData);
//		if(tempRetrun == ISTURE){//处理接收数据		
//			DingZhiDataResult((uint8_t *)pData,col1Data,Items,itemsNum,modbusCmd);
//			*flag = 2;
//		}
//		else if(tempRetrun == ISFAILTURE){
//			getDataFlag = 0;
//		}
//		else{
//			GUIWiatMessage(30,65);
//		}
//	}
//	if(*flag == 2){//指定表格内容指针
//		for(i=0;i<itemsNum;i++){
//			*(pText + i*DZDISPLAYCOL + 0) = Items[i].name;
//			if(Items[i].flag){//汉字
//				*(pText + i*DZDISPLAYCOL + 1) = \
//					Items[i].pContent[col1Data[i*16]];	
//			}
//			else{//float value
//				*(pText + i*DZDISPLAYCOL + 1) = &col1Data[i*16];
//			}
//			*(pText + i*DZDISPLAYCOL + 2) = Items[i].unit[0];
//		}
//		if((list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW + DISPLAYLISTROW <= list->row){
//			list->drawRow = (list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW;
//		}
//		else{
//			if(list->row < DISPLAYLISTROW){
//				list->drawRow = 0;
//			}
//			else{
//				list->drawRow = list->row - DISPLAYLISTROW;
//			}	
//		}
//		DrawList(list);
//		Scroll->lump = list->currentRow + 1;
//		DrawVScroll(Scroll);
//		GuiUpdateDisplayAll();
//		*flag = 3;
//	}
//	if(keyStatus != CancelKey && *flag <= 1){
//		keyStatus = NoKey;
//	}
//	switch(keyStatus){
//	case UpKey:
//		if(list->currentRow > 0){
//			list->currentRow --;
//		}
//		*flag = 2;break;
//	case DownKey:
//		if(list->currentRow < list->row - 1){
//			list->currentRow ++;
//		}
//		*flag = 2;break;
//	case LeftKey:
//		if(list->currentRow >= DISPLAYLISTROW){
//			list->currentRow -= DISPLAYLISTROW;
//		}
//		else{
//			list->currentRow =0;
//		}
//		*flag = 2;break;
//	case RightKey:
//		if(list->currentRow + DISPLAYLISTROW < list->row){
//			list->currentRow += DISPLAYLISTROW;
//		}
//		else{
//			list->currentRow = list->row - 1;
//		}
//		*flag = 2;break;
//	case OkKey:
//		if(GetDingZhiSetOrSee() == SET_DINGZHI){//定值修改
//			SetDingZhiModfiy(Items,(void *)modbusCmd,1,list->currentRow,\
//				&col1Data[list->currentRow*16]);
//			userGUIWindowAdd(&DZModfiyWin);
//			*flag = 2;
//		}break;
//	case CancelKey:
//		*flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

/**
  *@brief HomeWindow deal with function
  *@param  None
  *@retval None
  */
void HomeWindowFun(void)
{
	static uint8_t flag = 0;
	static uint8_t currentTime[24];
	//struct YaoXinData_  tempyaoXin;
	uint8_t i,j;
	if(flag == 0){//绘制主页
		lkdBitmap tImage;
	
		tImage.number = 1;
		tImage.wide = 160;
		tImage.high = 30;
		tImage.beginx = 0;
		tImage.beginy = 0;
		tImage.bitmap = NULL;
		GuiAreaBitMap(&tImage,HomeWindow.x+1,HomeWindow.y+25, 159, 55, 0);
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
		GuiFont12Align(HomeWindow.x+5,1,152,FONT_RIGHT,(uint8_t *)HMiVersion);
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
		flag = 1;
	}
	if(flag == 1){
		//TimeToString(currentTime);
		GuiExchangeColor();
		//GuiFont12Align(HomeWindow.x+2,82,156,FONT_MID,currentTime);
		GuiExchangeColor();
//		tempyaoXin = GetYaoxinStatus();
//		if(tempyaoXin.YaoXinData1.Bit.switchClose_h == 1){
//			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"合");
//		}
//		else{
//			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"分");
//		}
		GuiUpdateDisplayAll();
	}
	switch(keyStatus){
	case UpKey:
	case DownKey:
//		userGUIWindowAdd(&YaoCe2Win);
		flag = 0;
		break;
	case LeftKey:
	case RightKey:
//		userGUIWindowAdd(&YaoxinWin);
		flag = 0;
		break;
	case OkKey:
		flag = 0;
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
	static uint8_t flag;
	static uint8_t passWordState;
	
	if(flag == 0){
		passWordState = 0;
	}
	if(flag == 1){
		switch(mianMenu.currentItem){
		case 0://信息查询
		case 4://版本信息
			flag = 3;
			break;
		case 1://定值设置
		case 2://配置设置
		case 3://命令下发
			SetPassWordWin(0,&passWordState);
			userGUIWindowAdd(&PassWordWin);
			flag = 2;
			break;
		default:break;
		}			
	}
	else if(flag == 2){
		if(passWordState == 1){
			flag = 3;
		}
		else{
			flag = 0;
		}
	}
	if(flag == 3){
		switch(mianMenu.currentItem){
		case 0:userGUIMenuAdd(&MenuM0);break;//信息查询
		case 1:userGUIMenuAdd(&MenuM1);break;//定值设置
		case 2:userGUIMenuAdd(&MenuM2);break;//配置设置
//		case 3:userGUIWindowAdd(&CmdSendWin);break;//命令下发
//		case 4:userGUIWindowAdd(&VersionWin);break;//版本信息
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
		flag = 0;
		userGUIMenuHide();
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		break;
	case RightKey:
	case OkKey:
		if(flag == 0){
			flag = 1;
		}
		break;
	}
	if(flag == 3){
		flag = 0;
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
//		case 0:userGUIWindowAdd(&YaoxinWin);break;//遥信查询
		case 1:userGUIMenuAdd(&MenuM0S1);break;//遥测查询
//		case 2:userGUIWindowAdd(&SOEWin);break;//SOE查询
//		case 3:userGUIWindowAdd(&FaultEventWin);break;//故障事件
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
//		case 0:userGUIMenuAdd(&MenuM1S0);break;//保护功能
//		case 1:userGUIWindowAdd(&LogicalFunWin);break;//逻辑功能
//		case 2:userGUIWindowAdd(&OverLineWarnWin);break;//越线报警
//		case 3:userGUIWindowAdd(&OverLoadMuchWin);break;//重过载
//		case 4:userGUIWindowAdd(&OverLoadWin);break;//过负荷
//		case 5:userGUIWindowAdd(&OverVoltageWin);break;//过电压
//		case 6:userGUIWindowAdd(&BatterySetWin);break;//电池设置
//		case 7:userGUIWindowAdd(&AutoResetWin);break;//自动复归
//		case 8:userGUIMenuAdd(&MenuM1S8);break;//其他设置
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
//		case 0:userGUIWindowAdd(&BasicSetWin);break;
//		case 1:userGUIWindowAdd(&ZeroDriftWin);break;
//		case 2:userGUIWindowAdd(&DeadZoneWin);break;
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
//		case 0:userGUIWindowAdd(&YaoCe1Win);break;//一次遥测
//		case 1:userGUIWindowAdd(&YaoCe2Win);break;//二次遥测
//		case 2:userGUIWindowAdd(&HarmonicWin);break;//谐波
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
//		case 0:userGUIMenuAdd(&MenuM0S4S0);break;//保护功能
//		case 1:userGUIWindowAdd(&LogicalFunWin);break;//逻辑功能
//		case 2:userGUIWindowAdd(&OverLineWarnWin);break;//越线报警
//		case 3:userGUIWindowAdd(&OverLoadMuchWin);break;//重过载
//		case 4:userGUIWindowAdd(&OverLoadWin);break;//过负荷
//		case 5:userGUIWindowAdd(&OverVoltageWin);break;//过电压
//		case 6:userGUIWindowAdd(&BatterySetWin);break;//电池设置
//		case 7:userGUIWindowAdd(&AutoResetWin);break;//自动复归
//		case 8:userGUIMenuAdd(&MenuM0S4S8);break;//其他设置
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
//		case 0:userGUIWindowAdd(&BasicSetWin);break;
//		case 1:userGUIWindowAdd(&ZeroDriftWin);break;
//		case 2:userGUIWindowAdd(&DeadZoneWin);break;
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
//		case 0:userGUIWindowAdd(&InterphaseOverWin);break;//相间过流
//		case 1:userGUIWindowAdd(&ZeroSequenceOverWin);break;//零序过流
//		case 2:userGUIWindowAdd(&CutoutSwitchWin);break;//断路器
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
//		case 0:userGUIWindowAdd(&InterphaseOverWin);break;//相间过流
//		case 1:userGUIWindowAdd(&ZeroSequenceOverWin);break;//零序过流
//		case 2:userGUIWindowAdd(&CutoutSwitchWin);break;//断路器
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
//		case 0:userGUIWindowAdd(&RingUniteWin);break;//合环
//		case 1:userGUIWindowAdd(&BreakDownWin);break;//故障投退
		default:break;
		}
	}
}

///**
//  *@brief CmdSend deal with function 命令下发
//  *@param  None
//  *@retval None
//  */
//static void CmdSendWinFun(void)
//{
//	const  uint8_t listCol[2][1] = {{156},{COL_MID_DISPLAY}};
//	static uint8_t flag = 0;//步骤标志
//	static SCROLL *Scroll;//进度条
//	static ListColSet colset;
//	static LIST  *list;//列表控件
//	static uint8_t **pText;//列表内容指针
//	
//	static uint8_t messageFlag = 0;//步骤标志
//	uint16_t memMall;
//	static int8_t currentNum;
//	uint8_t i;
//	if(flag == 0){//初始化，分配内存
//		memMall = 0;	
//		list = (LIST  *)&userGUIBuff[memMall];
//		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
//		Scroll = (SCROLL *)&userGUIBuff[memMall];
//		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
//		pText = (uint8_t **)&userGUIBuff[memMall];
//		Scroll->x = 156;
//		Scroll->y = 18;
//		Scroll->hight = 141;
//		Scroll->max = CmdSend_NUM;
//		Scroll->lump = 1;
//		colset.colWide = (uint8_t *)listCol[0];
//		colset.colFlag = (uint8_t *)listCol[1];
//		list->x = 0;
//		list->y = 18;
//		list->wide = 156;
//		list->hight = 141;
//		list->row = CmdSend_NUM;
//		list->col = 1;
//		list->drawRow = 0;
//		list->currentRow = 0;
//		list->flag = LIST_USEBORDER_H;
//		list->content = (uint8_t **)pText;
//		list->colSet = &colset;	
//		currentNum = 0;
//		flag = 1;
//	}
//	if(flag == 1){//获取相应数据	
//		for(i = 0;i < CmdSend_NUM;i++){
//			*(pText + i) = CmdSendItems[i].name;
//		}
//		flag = 2;
//	}
//	if(flag == 2){//显示
//		list->currentRow = currentNum;
//		Scroll->lump = currentNum + 1;
//		if((list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW + DISPLAYLISTROW <= list->row){
//			list->drawRow = (list->currentRow /DISPLAYLISTROW)*DISPLAYLISTROW;
//		}
//		else{
//			if(list->row < DISPLAYLISTROW){
//				list->drawRow = 0;
//			}
//			else{
//				list->drawRow = list->row - DISPLAYLISTROW;
//			}	
//		}
//		DrawList(list);
//		DrawVScroll(Scroll);
//		GuiUpdateDisplayAll();
//		flag = 3;
//	}
//	if(flag == 3){
//	}
//	if(flag == 4){
//		if(messageFlag == 1){
//			FillYaoKongFIFO(ModBusSlaveAddr1,\
//				&ModbuscmdYaoKong[CmdSendItems[currentNum].M_mapNum]);
//		}
//		flag = 2;
//	}
//	switch(keyStatus){
//	case LeftKey:
//	case UpKey:
//		currentNum --;
//		if(currentNum < 0){
//			currentNum = Scroll->max - 1;
//		}
//		flag = 2;
//		break;
//	case RightKey:
//	case DownKey:
//		currentNum ++;
//		if(currentNum > Scroll->max - 1){
//			currentNum = 0;
//		}
//		flag = 2;
//		break;
//	case OkKey:
//		flag = 4;
//		messageFlag = 0;
//		SetMessageWin(23,40,"确定执行?",&messageFlag);
//		userGUIWindowAdd(&MessageWin);
//		break;
//	case CancelKey:
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief  固有参数数据处理
//  *@param  pData 待处理数据
//  *@param  outData 处理完成数据
//  *@param  eachSize 单个大小
//  *@retval None
//  */
//static void ConstParDataResult(uint8_t *pData,uint8_t *outData,uint8_t eachSize)
//{
//	uint8_t i;
//	int8_t j;
//	for(i=0;i<CONSTPARDISPLAYROW;i++){
//		strncpy((char *)&outData[i*(eachSize+1)],(char *)&pData[i*eachSize],eachSize);
//		for(j = 23;j >= 0;j--){
//			if(outData[i*eachSize + j] == ' '){
//				outData[i*eachSize + j] = '\0';
//			}
//			else{
//				outData[i*(eachSize+1) + 24] = '\0';
//				break;
//			}	
//		}
//	}
//}

///**
//  *@brief  版本信息
//  *@param  None
//  *@retval None
//  */
//static void VersionWinFun(void)
//{
//	static uint8_t flag = 0;//步骤标志
//	static SCROLL *Scroll;//进度条
//	static uint8_t *col1Data;//显示内容指针		
//	static uint8_t getDataFlag;
//	static int8_t currentNum;
//	uint32_t pData;
//	uint8_t tempRetrun;
//	uint8_t len;
//	uint16_t memMall;
//	uint8_t i,y;
//	if(flag == 0){//初始化，分配内存
//		memMall = 0;
//		Scroll = (SCROLL *)&userGUIBuff[memMall];
//		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
//		col1Data = &userGUIBuff[memMall];
//		
//		Scroll->x = 156;
//		Scroll->y = 18;
//		Scroll->hight = 141;
//		Scroll->max = ConstParameter_NUM/CONSTPARDISPLAYROW +\
//			(ConstParameter_NUM%CONSTPARDISPLAYROW >0 ?1:0);
//		Scroll->lump = 1;
//		currentNum = 0;
//		getDataFlag = 0;
//		flag = 1;
//	}
//	if(flag == 1){//获取相应数据
//		len = ModbuscmdDingZhi[ConstParametereNum].length.len16*CONSTPARDISPLAYROW;
//		tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//			ModbuscmdDingZhi[ConstParametereNum].addr.addr16 +\
//			currentNum*CONSTPARDISPLAYROW,len,&pData);//此处24字节一个地址
//		if(tempRetrun == ISTURE){//处理接收数据
//			ConstParDataResult((uint8_t *)pData,col1Data,\
//				ModbuscmdDingZhi[ConstParametereNum].length.len16*2);
//			flag = 2;
//		}
//		else if(tempRetrun == ISFAILTURE){
//			getDataFlag = 0;
//		}
//		else{
//			GUIWiatMessage(30,65);
//		}
//	}
//	if(flag == 2){//显示
//		y = VersionWin.y + 18;
//		GuiFillRect(VersionWin.x+1,y,155,158, backcolor);
//		for(i=0;i<CONSTPARDISPLAYROW;i++){
//			if((i + currentNum*CONSTPARDISPLAYROW) >= ConstParameter_NUM){
//				GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//水平线
//				break;
//			}
//			GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//水平线		
//			GuiFillRect(VersionWin.x+1,y + i*28+1,155,y + i*28+14, forecolor);
//			GuiExchangeColor();
//			GuiFont12Align(VersionWin.x + 2,y + i*28+2,\
//				133,FONT_LEFT,ConstParItems[i+currentNum*CONSTPARDISPLAYROW].name);
//			GuiExchangeColor();
//			GuiFont12Align(VersionWin.x+2,y + i*28+15,153,\
//				FONT_MID,&col1Data[i*25]);
//		}
//		Scroll->lump = currentNum + 1;
//		DrawVScroll(Scroll);
//		GuiUpdateDisplayAll();
//		flag = 3;
//	}
//	if(flag == 3){
//	}
//	if(keyStatus != CancelKey && flag <= 1){
//		keyStatus = NoKey;
//	}
//	switch(keyStatus){
//	case LeftKey:
//	case UpKey:
//		currentNum --;
//		if(currentNum < 0){
//			currentNum = Scroll->max - 1;
//		}
//		flag = 1;
//		break;
//	case RightKey:
//	case DownKey:
//		currentNum ++;
//		if(currentNum > Scroll->max - 1){
//			currentNum = 0;
//		}
//		flag = 1;
//		break;
//	case OkKey:break;
//	case CancelKey:
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief  相间过流
//  *@param  None
//  *@retval None
//  */
//static void InterphaseOverFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(InterphaseOverItems,InterphaseOver_NUM - 3,\
//		&ModbuscmdDingZhi[InterphaseOverNum],&flag);
//}

///**
//  *@brief 零序过流
//  *@param  None
//  *@retval None
//  */
//static void ZeroSequenceOverFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(ZeroSequenceOverItems,ZeroSequenceOver_NUM,\
//		&ModbuscmdDingZhi[ZeroSequenceOverNum],&flag);
//}

///**
//  *@brief 断路器
//  *@param  None
//  *@retval None
//  */
//static void CutoutSwitchFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(CutoutSwitchItems,CutoutSwitch_NUM,\
//		&ModbuscmdDingZhi[CutoutSwitchNum],&flag);
//}

///**
//  *@brief 逻辑功能
//  *@param  None
//  *@retval None
//  */
//static void LogicalFunFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(LoadSwitchItems,LoadSwitch_NUM,\
//		&ModbuscmdDingZhi[LoadSwitchNum],&flag);
//}

///**
//  *@brief 越线报警
//  *@param  None
//  *@retval None
//  */
//static void OverLineWarnFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(ThresholdAlarmItems,ThresholdAlarm_NUM,\
//		&ModbuscmdDingZhi[ThresholdAlarmNum],&flag);
//}

///**
//  *@brief 重过载
//  *@param  None
//  *@retval None
//  */
//static void OverLoadMuchFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(OverloadMuchItems,OverloadMuch_NUM,\
//		&ModbuscmdDingZhi[OverloadNum],&flag);
//}

///**
//  *@brief 过负荷
//  *@param  None
//  *@retval None
//  */
//static void OverLoadFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(OverloadItems,Overload_NUM,\
//		&ModbuscmdDingZhi[Overload2Num],&flag);
//}

///**
//  *@brief 过电压
//  *@param  None
//  *@retval None
//  */
//static void OverVoltageFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(OverVoltageItems,OverVoltage_NUM,\
//		&ModbuscmdDingZhi[OverVoltageNum],&flag);
//}

///**
//  *@brief 电池设置
//  *@param  None
//  *@retval None
//  */
//static void BatterySetFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(BatterySetItems,BatterySet_NUM,\
//		&ModbuscmdDingZhi[BatteryActivationNum],&flag);
//}

///**
//  *@brief 自动复归
//  *@param  None
//  *@retval None
//  */
//static void AutoResetFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(AutoResetItems,AutoReset_NUM,\
//		&ModbuscmdDingZhi[AutoResetNum],&flag);
//}

///**
//  *@brief 合环
//  *@param  None
//  *@retval None
//  */
//static void RingUniteFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(RingUniteItems,RingUnite_NUM,\
//		&ModbuscmdDingZhi[RingUniteNum],&flag);
//}

///**
//  *@brief 故障投退
//  *@param  None
//  *@retval None
//  */
//static void BreakDownFun(void)
//{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(BreakDownItems,BreakDown_NUM,\
//		&ModbuscmdDingZhi[BreakDownNum],&flag);
//}

/**
  *@brief 遥信处理函数
  *@param  None
  *@retval None
  */
//static void YaoxinFun(void)
void YaoxinFun(void)
{
	const  uint8_t listCol[2][2] = {{90,66},{FONT_LEFT,FONT_MID}};
	static uint8_t flag = 0;//步骤标志
	static SCROLL *Scroll;//进度条
	static ListColSet colset;
	static LIST  *list;//列表控件
	static uint8_t **pText;//列表内容指针
	static uint8_t *col1Data;//显示内容指针	
	static uint32_t YaoXinTick;
	
	uint16_t memMall;
	uint8_t i;
	uint8_t itemsNum;
	
	keyStatus = GetKeyStatus();//获取按键状态
	SetKeyIsNoKey();
	
	if(flag == 0){//初始化，分配内存
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
		flag = 1;
	}
	if(flag == 1){//获取相应数据
		itemsNum = yxInfo.Num;
		for(i = 0;i < itemsNum;i++){
			col1Data[i] = (uint8_t )*(yxInfo.pRoot[yxInfo.pBuff[i]].pVal);
		}	
		for(i = 0;i < itemsNum;i++){
			*(pText + i*2 + 0) = (uint8_t *)yxInfo.pRoot[i].pName;
			if(col1Data[i] == 0)
				*(pText + i*2 + 1) = (uint8_t *)yxInfo.pRoot[yxInfo.pBuff[i]].pContentYx[0];
			else
				*(pText + i*2 + 1) = (uint8_t *)yxInfo.pRoot[yxInfo.pBuff[i]].pContentYx[1];
		}
		flag = 2;
	}
	if(flag == 2){//显示
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
		flag = 3;
	}
	if(flag == 3){//检测遥信变化
		if(GetIntervalTick(YaoXinTick) > 2000){
			//flag = 1;
		}
	}

	switch(keyStatus){
	case UpKey:
		if(list->drawRow > DISPLAYLISTROW)
			list->drawRow -= DISPLAYLISTROW;
		else
			list->drawRow = 0;
		flag = 2;
		break;	
	case DownKey:	
		list->drawRow += DISPLAYLISTROW;
		if(list->drawRow + DISPLAYLISTROW > list->row){
			list->drawRow = list->row - DISPLAYLISTROW;
		}
		flag = 2;
		break;
	case LeftKey:break;
	case RightKey:break;
	case OkKey:break;
	case CancelKey:
		flag = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}

/**
  *@brief 遥测数据处理
  *@param  pData 待处理数据
  *@param  outData 处理输出数据
  *@param  items 遥测显示项
  *@param  itemsNum 遥测数量
  *@param  items modbus命令
  *@retval None
  */
//void yaoCeDataResult(uint8_t *pData,uint8_t *outData,const struct YaoCeItem *items,\
//	uint8_t itemsNum,const struct ModbusCmd_ *modbusCmd)
//{
//	union{
//		float f32;
//		uint8_t u8[4];
//	}tempFloat;
//	
//	uint16_t mapAddr;
//	uint8_t i;
//	
//	for(i=0;i<itemsNum;i++){
//		mapAddr = items[i].addr - modbusCmd->mapAddr.addr16;
//		tempFloat.u8[3] = pData[4*mapAddr];
//		tempFloat.u8[2] = pData[4*mapAddr + 1];
//		tempFloat.u8[1] = pData[4*mapAddr + 2];
//		tempFloat.u8[0] = pData[4*mapAddr + 3];
//		if(((uint8_t)isnan(tempFloat.f32)) != 0){
//			sprintf((char *)&outData[i*16],"%s","NaN");
//		}
//		else{
//			sprintf((char *)&outData[i*16],"%.3f",tempFloat.f32);
//		}
//		
//	}
//}

///**
//  *@brief 遥测处理显示
//  *@param  items 遥测显示项
//  *@param  itemsNum 遥测数量
//  *@param  items modbus命令
//  *@retval None
//  */
//void yaoCeDisplay(const struct YaoCeItem *items,uint8_t itemsNum,\
//	const struct ModbusCmd_ *modbusCmd)
//{
//	const uint8_t listCol[2][YaoCeDISPLAYCOL] = {//表格列修饰
//		{50,75,31},{COL_LEFT_DISPLAY,COL_MID_DISPLAY,COL_MID_DISPLAY}};
//	static uint8_t flag = 0;//步骤标志
//	static SCROLL *Scroll;//进度条
//	static ListColSet colset;
//	static LIST  *list;//列表控件
//	static uint8_t **pText;//列表内容指针
//	static uint8_t *col1Data;//显示内容指针		
//	static uint32_t YaoCeTick;//循环计时
//	static uint8_t getDataFlag;
//	
//	uint16_t memMall;
//	uint8_t tempRetrun;
//	uint8_t i;
//	uint32_t pData;
//		
//	if(flag == 0){//初始化，分配内存
//		memMall = 0;	
//		list = (LIST  *)&userGUIBuff[memMall];
//		memMall += sizeof(LIST) + (4 - sizeof(LIST)%4);
//		Scroll = (SCROLL *)&userGUIBuff[memMall];
//		memMall += sizeof(SCROLL) + (4 - sizeof(SCROLL)%4);
//		pText = (uint8_t **)&userGUIBuff[memMall];
//		memMall += itemsNum*YaoCeDISPLAYCOL*4;
//		col1Data = &userGUIBuff[memMall];
//		
//		Scroll->x = 156;
//		Scroll->y = 18;
//		Scroll->hight = 141;
//		Scroll->max = itemsNum/DISPLAYLISTROW+(itemsNum%DISPLAYLISTROW >0 ?1:0);
//		Scroll->lump = 1;
//		colset.colWide = (uint8_t *)listCol[0];
//		colset.colFlag = (uint8_t *)listCol[1];
//		list->x = 0;
//		list->y = 18;
//		list->wide = 156;
//		list->hight = 141;
//		list->row = itemsNum;
//		list->col = YaoCeDISPLAYCOL;
//		list->drawRow = 0;
//		list->currentRow = 255;
//		list->flag = LIST_USEBORDER_H;
//		list->content = (uint8_t **)pText;
//		list->colSet = &colset;
//		//获取数据中
//		flag = 1;
//		getDataFlag = 0;
//	}
//	if(flag == 1){
//		tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//			modbusCmd->addr.addr16,\
//			modbusCmd->length.len16,&pData);
//		if(tempRetrun == ISTURE){//处理接收数据		
//			yaoCeDataResult((uint8_t *)pData,col1Data,items,itemsNum,modbusCmd);
//			flag = 2;
//		}
//		else if(tempRetrun == ISFAILTURE){
//			getDataFlag = 0;
//		}
//		else{
//				GUIWiatMessage(30,65);
//		}
//		for(i = 0;i < itemsNum;i++){
//			*(pText + i*YaoCeDISPLAYCOL + 0) =items[i].name;
//			*(pText + i*YaoCeDISPLAYCOL + 1) = &col1Data[i*16];
//			*(pText + i*YaoCeDISPLAYCOL + 2) = items[i].unit[0];
//		}
//	}
//	if(flag == 2){//显示	
//		if(list->drawRow == list->row - DISPLAYLISTROW){
//			Scroll->lump = Scroll->max;
//		}
//		else{
//			Scroll->lump = list->drawRow/DISPLAYLISTROW + 1;
//		}
//		DrawList(list);
//		DrawVScroll(Scroll);
//		GuiUpdateDisplayAll();
//		flag = 3;
//	}
//	if(flag == 3){
//		if(GetIntervalTick(YaoCeTick) > YAOCE_UPDATATIME){//检测更新
//			tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//				modbusCmd->addr.addr16,\
//				modbusCmd->length.len16,&pData);
//			if(tempRetrun == ISTURE){//处理接收数据
//				yaoCeDataResult((uint8_t *)pData,col1Data,items,itemsNum,modbusCmd);
//				YaoCeTick = getCurrentTick();
//				flag = 2;
//			}
//			else if(tempRetrun == ISFAILTURE){
//				getDataFlag = 0;
//			}
//		}
//	}
//	switch(keyStatus){
//	case UpKey:
//		if(flag > 1){
//			if(list->drawRow > DISPLAYLISTROW)
//				list->drawRow -= DISPLAYLISTROW;
//			else
//				list->drawRow = 0;
//			flag = 2;
//			break;
//		}	
//	case DownKey:
//		if(flag > 1){
//			list->drawRow += DISPLAYLISTROW;
//			if(list->drawRow + DISPLAYLISTROW > list->row){
//				list->drawRow = list->row - DISPLAYLISTROW;
//			}
//			flag = 2;
//			break;
//		}
//	case LeftKey:break;
//	case RightKey:break;
//	case OkKey:break;
//	case CancelKey:
//		getDataFlag = 0;
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief 一次遥测处理函数
//  *@param  None
//  *@retval None
//  */
//static void YaoCe1Fun(void)
//{
//	yaoCeDisplay(yaoCe1Items,YAOCE1_NUM,&ModbuscmdYaoCe[0]);
//}

///**
//  *@brief 二次遥测处理函数
//  *@param  None
//  *@retval None
//  */
//static void YaoCe2Fun(void)
//{
//	yaoCeDisplay(yaoCe2Items,YAOCE2_NUM,&ModbuscmdYaoCe[0]);
//}
///**
//  *@brief 谐波处理函数
//  *@param  None
//  *@retval None
//  */
//static void HarmonicFun(void)
//{
//	yaoCeDisplay(yaoCe3Items,HARMONIC_NUM,&ModbuscmdYaoCe[1]);
//}

///**
//  *@brief SOE数据处理
//  *@param  pData 要处理的数据
//  *@param  eachSoeSize 单位大小
//  *@param  *SOE[] 处理存放
//  *@retval None
//  */
//static void SOEDataResult(uint8_t *pData,uint8_t eachSoeSize,struct SOEDisplay *SOE[])
//{
//	uint32_t tempaddr,tempaddr2;
//	uint8_t i,j;
//	
//	for(j=0;j<SOEDISPLAYROW;j++){
//		tempaddr = (pData[j*eachSoeSize+0]<<8)+pData[j*eachSoeSize+1];
//		if(pData[j*eachSoeSize+2]>=0x20){//状态位含义较多
//			if((pData[j*eachSoeSize+2]&0x10) == 1){
//				tempaddr2 = 0x00060000|tempaddr;//遥控取消
//			}
//			else{
//				switch(pData[j*eachSoeSize+2]&0x03){
//				case 0x01://分闸
//					switch(pData[j*eachSoeSize+2]&0x08){
//					case 0x00://选择
//						tempaddr2 = 0x00000000|tempaddr;
//						break;
//					case 0x08://执行
//						tempaddr2 = 0x00010000|tempaddr;
//						break;;
//					}break;
//				case 0x02://合闸
//					switch(pData[j*eachSoeSize+2]&0x08){
//					case 0x00://选择
//						tempaddr2 = 0x00020000|tempaddr;
//						break;
//					case 0x08://执行
//						tempaddr2 = 0x00030000|tempaddr;
//						break;
//					}break;
//				case 0x03://合环
//					switch(pData[j*eachSoeSize+2]&0x08){
//					case 0x00://选择
//						tempaddr2 = 0x00040000|tempaddr;
//						break;
//					case 0x08://执行
//						tempaddr2 = 0x00050000|tempaddr;
//						break;
//					}break;
//				}
//			}
//			for(i = 0;i<SOE_NUM;i++){
//				if(tempaddr2 == SOEItems[i].addr){
//					SOE[j]->SOEItem = i;
//					SOE[j]->itemValue = (pData[j*eachSoeSize+2]&0x04)?1:0;
//					break;
//				}
//			}
//			if(i == SOE_NUM){
//				SOE[j]->SOEItem = 255;//255表示无效
//			}
//		}
//		else{
//			tempaddr2 = (uint32_t)(tempaddr&0x0000FFFF);
//			for(i = 0;i<SOE_NUM;i++){
//				if(tempaddr2 == SOEItems[i].addr){
//					SOE[j]->SOEItem = i;
//					SOE[j]->itemValue = pData[j*eachSoeSize+2] - 1;//将 1，2 两种状态转换成 0 1 方便数组调用
//					if(SOE[j]->itemValue > 2){
//						SOE[j]->itemValue = 0;
//					}
//					break;
//				}
//			}
//			if(i == SOE_NUM){
//				SOE[j]->SOEItem = 255;//255表示无效
//			}
//		}
//		sprintf((char *)SOE[j]->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
//			(pData[j*eachSoeSize+9]%100),(pData[j*eachSoeSize+8]&0x3F),\
//			pData[j*eachSoeSize+7]&0x1F,(pData[j*eachSoeSize+6]&0x3F),\
//			(pData[j*eachSoeSize+5]&0x3F),\
//			((pData[j*eachSoeSize+4]<<8) + pData[j*eachSoeSize+3])/1000,
//			((pData[j*eachSoeSize+4]<<8) + pData[j*eachSoeSize+3])%1000);
//	}
//}

///**
//  *@brief SOE处理函数
//  *@param  None
//  *@retval None
//  */
//static void SOEFun(void)
//{
//	static struct SOEDisplay *SOE[SOEDISPLAYROW];
//	static uint8_t flag = 0;
//	static SCROLL Scroll ={156,18,141,2,1};//进度条
//	static uint8_t getDataFlag = 0;
//	static uint16_t offestAddr = 0;//SOE偏移地址
//	static int16_t currentNum;
//	
//	uint32_t pData;//存放接收数据指针
//	union yaoXinTimeflag dataUpdateFlag;
//	uint8_t tempRetrun;
//	uint16_t offest;
//	uint8_t i;
//	uint16_t y;
//	Scroll.max = GetSoeAllNum();
//	if(Scroll.max == 0){
//		GuiFont12Align(SOEWin.x+2,SOEWin.y + 30,\
//			SOEWin.x+SOEWin.wide-4,FONT_MID,"当前没有SOE");
//		GuiUpdateDisplayAll();
//	}
//	else{
//		if(flag == 0){//初始化，分配内存
//			for(i=0;i<SOEDISPLAYROW;i++){//给SOE分配显示缓冲区
//				SOE[i] = (struct SOEDisplay *)&userGUIBuff[i*sizeof(struct SOEDisplay)];
//			}
//			flag = 1;
//			getDataFlag = 0;
//		}
//		if(flag == 1){//发送命令
//			offestAddr = currentNum*(ModbuscmdFIFO[FIFO_SOE_NUM].length.len16/2)*SOEDISPLAYROW;
//			tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//				ModbuscmdFIFO[FIFO_SOE_NUM].addr.addr16+offestAddr,\
//				ModbuscmdFIFO[FIFO_SOE_NUM].length.len16*SOEDISPLAYROW,&pData);
//			if(tempRetrun == ISTURE){
//				SOEDataResult((uint8_t *)pData,ModbuscmdFIFO[FIFO_SOE_NUM].length.len16,SOE);
//				offest = offestAddr/(ModbuscmdFIFO[FIFO_SOE_NUM].length.len16/2) + 1;
//				for(i=0;i<SOEDISPLAYROW;i++){
//					sprintf((char *)SOE[i]->itemNum,"%d",offest + i);
//				}
//				flag = 2;
//			}
//			else if(tempRetrun == ISFAILTURE){
//				getDataFlag = 0;
//			}
//			else{
//				GUIWiatMessage(30,65);
//			}
//		}
//		if(flag == 2){
//			y = SOEWin.y + 18;
//			GuiFillRect(SOEWin.x+1,y,155,158, backcolor);
//			for(i=0;i<SOEDISPLAYROW;i++){
//				if(SOE[i]->SOEItem == 255){//检测到无效SOE,结束
//					GuiHLine(SOEWin.x,y+i*28,155,forecolor);
//					break;
//				}
//				GuiHLine(SOEWin.x,y+i*28+0,155,forecolor);//水平线
//				GuiFillRect(SOEWin.x+1,y+i*28,SOEWin.x+20,y+i*28+13, forecolor);
//				GuiExchangeColor();
//				GuiFont12Align(SOEWin.x+1,y + i*28+2,19,\
//					FONT_MID,SOE[i]->itemNum);//序号
//				GuiExchangeColor();	
//				GUI_RPointLine(SOEWin.x+20,y+i*28+1,y+i*28+13,2,forecolor);//垂直线
//				GuiFont12Align(SOEWin.x + 21,y + i*28+2,133,FONT_RIGHT,SOE[i]->time);
//				GuiHPointLine(SOEWin.x,y+i*28+13,155,2,forecolor);
//				GuiFont12Align(SOEWin.x+2,y + i*28+15,72,\
//					FONT_MID,SOEItems[SOE[i]->SOEItem].name);
//				GUI_RPointLine(SOEWin.x+10+72,y+i*28+15,y+i*28+27,2,forecolor);
//				if(SOE[i]->itemValue){
//					GuiFillRect(SOEWin.x+10+73,y+i*28+14,158,y+i*28+28, forecolor);
//					GuiExchangeColor();
//					GuiFont12Align(SOEWin.x+10+73,y+i*28+15,70,FONT_RIGHT,\
//						SOEItems[SOE[i]->SOEItem].status[SOE[i]->itemValue]);
//					GuiExchangeColor();
//				}
//				else{
//					GuiFont12Align(SOEWin.x+10+73,y+i*28+15,70,FONT_RIGHT,\
//						SOEItems[SOE[i]->SOEItem].status[SOE[i]->itemValue]);
//				}
//					
//			}
//			Scroll.lump = currentNum + 1;
//			DrawVScroll(&Scroll);
//			GuiUpdateDisplayAll();
//			flag = 3;
//		}
//		if(flag == 3){
//			 dataUpdateFlag = GetDtatUpdateStatus();
//			if(dataUpdateFlag.bit.newSoe == 1){//SOE有更新
//				ClearDtatUpdateNewSoe();//清除更新标志
//				currentNum = 0;
//				flag = 1;
//			}
//		}
//	}
//	
//	switch(keyStatus){
//	case UpKey:
//	case LeftKey:
//		if(flag > 1){
//			currentNum --;
//			if(currentNum < 0){
//				currentNum = Scroll.max - 1;
//			}
//		}
//		flag = 1;
//		break;
//	case DownKey:
//	case RightKey:
//		if(flag > 1){
//			currentNum ++;
//			if(currentNum > Scroll.max - 1){
//				currentNum = 0;
//			}
//		}
//		flag = 1;
//		break;
//	case OkKey:break;
//	case CancelKey:
//		currentNum = 0;
//		offestAddr = 0;
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

///**
//  *@brief 故障事件数据处理
//  *@param  pData 待处理数据
//  *@param  eachSize 每个事件数据的大小
//  *@param  pEvent 故障事件显示指针
//  *@retval None
//  */
//static void EventDataResult(uint8_t *pData,uint8_t eachSize,struct EventDisplay *pEvent)
//{
//	uint32_t tempaddr,tempaddr2;
//	uint8_t i,j;
//	union{
//		float f32;
//		uint8_t u8[4];
//	}charToFloat;
//	tempaddr = (pData[2]<<8)+pData[3];
//	if(pData[4]>=0x20){//状态位含义较多
//		if((pData[4]&0x10) == 1){
//			tempaddr2 = 0x00060000|tempaddr;//遥控取消
//		}
//		else{
//			switch(pData[4]&0x03){
//			case 0x01://分闸
//				switch(pData[4]&0x08){
//				case 0x00://选择
//					tempaddr2 = 0x00000000|tempaddr;
//					break;
//				case 0x08://执行
//					tempaddr2 = 0x00010000|tempaddr;
//					break;
//				default: break;
//				}break;
//			case 0x02://合闸
//				switch(pData[4]&0x08){
//				case 0x00://选择
//					tempaddr2 = 0x00020000|tempaddr;
//					break;
//				case 0x08://执行
//					tempaddr2 = 0x00030000|tempaddr;
//					break;
//				default: break;
//				}break;
//			case 0x03://合环
//				switch(pData[4]&0x08){
//				case 0x00://选择
//					tempaddr2 = 0x00040000|tempaddr;
//					break;
//				case 0x08://执行
//					tempaddr2 = 0x00050000|tempaddr;
//					break;
//				default: break;
//				}break;
//			}
//		}
//		for(i = 0;i<SOE_NUM;i++){
//			if(tempaddr2 == SOEItems[i].addr){
//				pEvent->SOEItem = i;
//				pEvent->itemValue = (pData[4]&0x04)?1:0;
//				break;
//			}
//		}
//		if(i == SOE_NUM){
//			pEvent->SOEItem = 255;//255表示无效
//		}
//	}
//	else{
//		tempaddr2 = (uint32_t)(tempaddr&0x0000FFFF);
//		for(i = 0;i<SOE_NUM;i++){
//			if(tempaddr2 == SOEItems[i].addr){
//				pEvent->SOEItem = i;
//				pEvent->itemValue = pData[4] - 1;//将 1，2 两种状态转换成 0 1 方便数组调用
//				if(pEvent->itemValue > 2){
//					pEvent->itemValue = 0;
//				}
//				break;
//			}
//		}
//		if(i == SOE_NUM){
//			pEvent->SOEItem = 255;//255表示无效
//		}
//	}
//	if(pEvent->SOEItem < 255){
//		pEvent->yaoceNum = pData[12];
//		if(pEvent->yaoceNum > 12){
//			pEvent->yaoceNum = 12;
//		}
//		for(i=0;i<pEvent->yaoceNum;i++){
//			tempaddr = (pData[14 + i*6]<<8) + pData[15 + i*6];
//			for(j=0;j<YAOCE2_NUM;j++){
//				if(tempaddr == yaoCe2Items[j].addr){
//					pEvent->yaoceItem[i] = j;
//					charToFloat.u8[3] = pData[i*6+16];
//					charToFloat.u8[2] = pData[i*6+17];
//					charToFloat.u8[1] = pData[i*6+18];
//					charToFloat.u8[0] = pData[i*6+19];
//					if(((uint8_t)isnan(charToFloat.f32)) != 0){
//						sprintf((char *)&(pEvent->yaoceValue[i*16]),"%s","NaN");
//					}
//					else{
//						sprintf((char *)&(pEvent->yaoceValue[i*16]),"%.3f",charToFloat.f32);
//					}
//					break;
//				}
//			}
//			if(j == YAOCE2_NUM){
//				pEvent->yaoceItem[i] = 255;
//				break;
//			}
//		}
//	}
//	sprintf((char *)pEvent->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
//		(pData[11]%100),(pData[10]&0x3F),pData[9]&0x1F,\
//		(pData[8]&0x3F),(pData[7]&0x3F),\
//		((pData[6]<<8) + pData[5])/1000,((pData[6]<<8) + pData[5])%1000);
//}

///**
//  *@brief 故障事件处理函数
//  *@param  None
//  *@retval None
//  */
//static void FaultEventFun(void)
//{
//	static uint8_t flag = 0;
//	static struct EventDisplay *pEvent;
//	static SCROLL Scroll ={156,18,141,2,1};//进度条
//	static SCROLL yaoCeScroll ={153,47,112,2,1};//进度条
//	static uint8_t getDataFlag = 0;
//	static uint16_t offestAddr = 0;//SOE偏移地址
//	static int16_t currentNum;
//	static int8_t yaoCeNum;
//	uint32_t pData;//存放接收数据指针
//	union yaoXinTimeflag dataUpdateFlag;
//	uint8_t tempRetrun;
//	uint8_t i;
//	uint16_t y;
//	Scroll.max = GetEventAllNum();
//	if(Scroll.max == 0){
//		GuiFont12Align(FaultEventWin.x+2,FaultEventWin.y + 50,\
//			FaultEventWin.x+FaultEventWin.wide-4,FONT_MID,"当前没有故障事件");
//		GuiUpdateDisplayAll();
//	}
//	else{
//		if(flag == 0){//初始化，分配内存
//			pEvent = (struct EventDisplay *)userGUIBuff;
//			flag = 1;
//			getDataFlag = 0;
//		}
//		if(flag == 1){//发送命令
//			offestAddr = currentNum*(ModbuscmdFIFO[FIFO_EVENT_NUM].length.len16/2);
//			tempRetrun = GetModbusData(&getDataFlag,ModBusSlaveAddr1,\
//				ModbuscmdFIFO[FIFO_EVENT_NUM].addr.addr16+offestAddr,\
//				ModbuscmdFIFO[FIFO_EVENT_NUM].length.len16,&pData);
//			if(tempRetrun == ISTURE){
//				EventDataResult((uint8_t *)pData,ModbuscmdFIFO[FIFO_EVENT_NUM].length.len16,pEvent);
//				sprintf((char *)pEvent->itemNum,"%d",\
//					offestAddr/(ModbuscmdFIFO[FIFO_EVENT_NUM].length.len16/2) + 1);
//				if(pEvent->yaoceNum <= 8){
//					yaoCeScroll.max = 1;
//				}
//				else{
//					yaoCeScroll.max = pEvent->yaoceNum - 8 + 1;
//				}
//				yaoCeNum = 0;
//				flag = 2;
//				if(pEvent->SOEItem == 255){
//					flag = 4;
//				}
//			}
//			else if(tempRetrun == ISFAILTURE){
//				getDataFlag = 0;
//			}
//			else{//等待画面
//				GUIWiatMessage(30,65);
//			}
//		}
//		if(flag == 2){//画图
//			y = FaultEventWin.y + 18;
//			GuiFillRect(FaultEventWin.x+1,y,155,158, backcolor);
//			GuiHLine(FaultEventWin.x,y,155,forecolor);//水平线	
//			GuiFillRect(FaultEventWin.x+1,y,FaultEventWin.x+20,y+13, forecolor);
//			GuiExchangeColor();
//			GuiFont12Align(FaultEventWin.x+1,y+2,20,FONT_MID,pEvent->itemNum);//序号
//			GuiExchangeColor();	
//			GUI_RPointLine(FaultEventWin.x+20,y+1,y+13,2,forecolor);//垂直线
//			GuiFont12Align(FaultEventWin.x + 21,y+2,133,FONT_RIGHT,pEvent->time);
//			GuiHPointLine(FaultEventWin.x,y+13,155,2,forecolor);
//			GuiFont12Align(FaultEventWin.x+2,y+15,72,FONT_LEFT,SOEItems[pEvent->SOEItem].name);
//			GUI_RPointLine(FaultEventWin.x+10+72,y+15,y+27,2,forecolor);
//			GuiFont12Align(FaultEventWin.x+10+73,y+15,70,FONT_RIGHT,\
//				SOEItems[pEvent->SOEItem].status[pEvent->itemValue]);	
//			Scroll.lump = currentNum + 1;
//			DrawVScroll(&Scroll);
//			GuiHLine(FaultEventWin.x,y+28,155,forecolor);//水平线	
//			flag = 3;
//		}
//		if(flag == 3){
//			y = FaultEventWin.y + 18 + 29;
//			yaoCeScroll.lump = yaoCeNum + 1;
//			DrawVScroll(&yaoCeScroll);
//			GuiFillRect(FaultEventWin.x+1,y,151,158, backcolor);
//			for(i=0;i<8;i++){
//				GuiFont12Align(FaultEventWin.x+2,y+1+i*14,40,FONT_LEFT,\
//					yaoCe2Items[pEvent->yaoceItem[yaoCeNum + i]].name);			
//				GUI_RPointLine(FaultEventWin.x+43,y+i*14,y+13+i*14,2,forecolor);
//				GuiFont12Align(FaultEventWin.x+44,y+1+i*14,70,FONT_MID,&pEvent->yaoceValue[(i+yaoCeNum)*16]);
//				GUI_RPointLine(FaultEventWin.x+115,y+i*14,y+13+i*14,2,forecolor);
//				GuiFont12Align(FaultEventWin.x+116,y+1+i*14,35,FONT_RIGHT,\
//					yaoCe2Items[pEvent->yaoceItem[yaoCeNum+i]].unit[0]);		
//				GuiHPointLine(FaultEventWin.x,y+13+i*14,151,2,forecolor);
//			}
//			GuiUpdateDisplayAll();
//			flag = 4;
//		}
//		if(flag == 4){
//			 dataUpdateFlag = GetDtatUpdateStatus();
//			if(dataUpdateFlag.bit.newFaultEvent == 1){//SOE有更新
//				ClearnewFaultEvent();//清除更新标志
//				currentNum = 0;
//				flag = 1;
//			}
//		}
//	}
//	
//	switch(keyStatus){
//	case UpKey:
//		if(flag > 3){
//			yaoCeNum --;
//			if(yaoCeNum < 0){
//				yaoCeNum = yaoCeScroll.max - 1;
//			}
//		}
//		flag = 3;
//		break;
//	case LeftKey:
//		if(flag > 1){
//			currentNum --;
//			if(currentNum < 0){
//				currentNum = Scroll.max - 1;
//			}
//		}
//		flag = 1;
//		break;
//	case DownKey:
//		if(flag > 3){
//			yaoCeNum ++;
//			if(yaoCeNum > yaoCeScroll.max - 1){
//				yaoCeNum = 0;
//			}
//		}
//		flag = 3;
//		break;
//	case RightKey:
//		if(flag > 1){
//			currentNum ++;
//			if(currentNum > Scroll.max - 1){
//				currentNum = 0;
//			}
//		}
//		flag = 1;
//		break;
//	case OkKey:break;
//	case CancelKey:
//		currentNum = 0;
//		offestAddr = 0;
//		flag = 0;
//		userGUITopWindowHide();
//		userGUITopWindowRedraw();
//		userGUIMenuRedraw();
//		break;
//	default:break;
//	}
//}

/**
  *@brief 基本设置处理函数
  *@param  None
  *@retval None
  */
static void BasicSetFun(void)
{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(BasicConfigureItems,BasicConfigure_NUM,\
//		&ModbuscmdDingZhi[BasicConfigureNum],&flag);
}

/**
  *@brief 零漂设置处理函数
  *@param  None
  *@retval None
  */
static void ZeroDriftFun(void)
{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(ZeroDriftItems,ZeroDrift_NUM,\
//		&ModbuscmdDingZhi[ZeroDriftNum],&flag);
}

/**
  *@brief 死区设置处理函数
  *@param  None
  *@retval None
  */
static void DeadZoneFun(void)
{
//	static uint8_t flag = 0;
//	DZModfiyDisplay(DeadZoneItems,DeadZone_NUM,\
//		&ModbuscmdDingZhi[DeadZoneNum],&flag);
}

/* END */

/**
  * @file   GUIdisplay.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    03-06-2018
  * @brief   GUI��ʾ
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

const static uint8_t *modfiyKey[14] = {/* ������ */
	"0","1","2","3","4",".","esc","5","6","7","8","9","<-","ok"};
static enum KeyStatus keyStatus;	/* ����ֵ */
static uint8_t userGUIBuff[1024*3]; /* �����ڴ���� */
static struct DZModfiy dZModfiy;	/* ��ֵ�޸� */
static struct Message MessageIs;	/* ��Ϣ���� */
static struct PassWordPipe passWordPipe;		/* ������� */
static enum DingZhiSetOrSee dingZhiSetOrSee;	/* ��ֵ��ѯ�����޸ı�־ */

/**
  *@brief  ��ȡʱ��
  *@param  None
  *@retval ��ǰʱ�ӵδ�
  */	
static uint32_t getCurrentTick(void)
{
	return GetTimer1Tick();
}

/**
  *@brief  ��ȡ���ʱ��
  *@param  ��ʼʱ�ӵδ�
  *@retval ���ʱ��
  */
static uint32_t GetIntervalTick(uint32_t beginTick)
{
	return GetTimer1IntervalTick(beginTick);
}

/**
  *@brief �޸Ķ�ֵ���û��������־
  *@param  setOrSee ���á���� ö��
  *@retval None
  */
static void SetDingZhiSetOrSee(enum DingZhiSetOrSee setOrSee)
{
	dingZhiSetOrSee = setOrSee;
}

/**
  *@brief ��ȡ��ֵ���û��������־
  *@param  None
  *@retval DingZhiSetOrSee ���á���� ö��
  */
static enum DingZhiSetOrSee GetDingZhiSetOrSee(void)
{
	return dingZhiSetOrSee;
}

/**
  *@brief lcd��ʾ����
  *@param  key ����״̬
  *@retval lcd��ʾ״̬ LCDDISPLAYON LCDDISPLAYOFF
  */
static uint8_t GUIDisplayONOFF(enum KeyStatus *key)
{
	static uint32_t LcdDisPlayTick;//��ʱ
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
				if(GetIntervalTick(LcdDisPlayTick) > LCDDISPLAYOFFTIME){//ʱ��
					lcdONOFF = LCDDISPLAYOFF;
					CloseLcdDisplay();
				}
			}
		}
	}
	return lcdONOFF;
}


/**
  *@brief GUI��������
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
	GuiFont12Align(2,82,156,FONT_MID,"��������......");
	GuiExchangeColor();
	GuiUpdateDisplayAll();
}

/**
  *@brief GUI���߸���
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
	GuiFont12Align(2,82,156,FONT_MID,"���ڸ���......");
	GuiExchangeColor();
	GuiUpdateDisplayAll();
}

/**
  *@brief GUI�ȴ�ͼ��
  *@param  x,y ��ʼ����
  *@retval None
  */
void GUIWiatMessage(uint8_t x,uint8_t y)
{
	GuiFillRect(x,y,x + 88,y + 16, backcolor);
	GuiHLine(x + 2,y + 16,x + 86,forecolor);
	GuiHLine(x + 3,y + 17,x + 87,forecolor);
	GuiRLine(x + 86,y + 2,y + 16,forecolor);
	GuiRLine(x + 87,y + 3,y + 16,forecolor);
	GuiFont12Align(x + 20,y + 2,70,FONT_LEFT,"������.....");
	GuiFillRect(x + 2,y + 2,x + 14,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiFillRect(x + 2,y + 2,32 + 12,y + 14, forecolor);
	GuiHLine(x + 2,y + 8,x + 14,backcolor);
	GuiRLine(x + 8,y + 2,y + 14,backcolor);
	GuiUpdateDisplayAll();
}
/**
  *@brief GUI��ʼ��������������
  *@param  None
  *@retval None
  */
void GUIDisplayInit(void)
{
	MenuManageInit();
	WinManageInit();
	userGUIWindowAdd(&HomeWindow);
}

/**
  *@brief GUI����������������������
  *@param  None
  *@retval None
  */
void GUIDisplayMian(void)
{
//	static uint8_t flag;
//	uint8_t upDataProgram;
//	//upDataProgram = GetDtatUpdateStatus();
	keyStatus = GetKeyStatus();//��ȡ����״̬
	SetKeyIsNoKey();
//	if(flag == 0 && upDataProgram.bit.programUpdate != 0){
//		OpenLcdDisplay();
//		GUIUpdataProgram();//���߸���
//		flag = 1;
//	}
//	else if(flag == 1 && upDataProgram.bit.programUpdate == 0){
//		flag = 0;
//		//NVIC_SystemReset();//����
//	}
//	else if(flag == 0  && GUIDisplayONOFF(&keyStatus) == LCDDISPLAYON){
//		
//	}
	userGUITopWindowDisplay();
}

/**
  *@brief �������봰�����ֵ
  *@param  type ��������
  *@param  returnflag ����״ָ̬��
  *@retval None
  */
void SetPassWordWin(uint8_t type,uint8_t *returnflag)
{
	passWordPipe.passWordType = type;
	passWordPipe.returnFlag = returnflag;
}

/**
  *@brief ���봰��
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
			PassWordItems[passWordPipe.passWordType].name);//��ʾ��
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
		}//��Ҫִ��CancelKey��������û��break
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
  *@brief ������Ϣ������Ӧ����
  *@param  x ,y ��ʼ����
  *@param  pStr ˵���ַ���
  *@param  flag ���ر�־ 0 ȡ�� 1 ȷ��
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
  *@brief ��Ϣ���ڴ�������
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
			MessageWin.wide - 1,FONT_MID,MessageIs.str);//��ʾ��
		winHight = MessageWin.y + MessageWin.hight - 23;
		GuiHPointLine(MessageWin.x+1,winHight,\
			MessageWin.x + MessageWin.wide - 1,2,forecolor);
		if(keyFlag == 0){
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"ȷ��",0);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"ȡ��",1);
		}
		else{
			winWide =  MessageWin.x + (MessageWin.wide/2 - 45)/2;
			GuiButton(winWide,winHight + 3,"ȷ��",1);
			winWide +=  MessageWin.wide/2;
			GuiButton(winWide,winHight + 3,"ȡ��",0);
		}
		GuiUpdateDisplayAll();
		flag = 2;
	}
	if(flag == 2){
	}
	switch(keyStatus){
	case LeftKey:
		keyFlag--;
		if(keyFlag < 0){
			keyFlag = 1;
		}
		flag = 1;break;
	case UpKey:break;
	case RightKey:
		keyFlag++;
		if(keyFlag > 1){
			keyFlag = 0;
		}
		flag = 1;break;
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
  *@brief  ���ö�ֵ�޸���ر���
  *@param  info ��ֵ�ṹ��ָ��
  *@param  flag ��־�ֽڣ�δʹ��
  *@param  itemIs ��ֵ��ǰItem
  *@param  str Ҫ�޸��ַ�����ָ��
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
  *@brief ��ֵ�޸ĳ�ʼ��
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
		dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pName);
	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 30,\
		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
	GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + DZModfiyWin.hight - 16,\
		DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);//״̬��
	GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + DZModfiyWin.hight - 14,80,FONT_LEFT,"��ǰ״̬:");
	
	if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//����
		x = DZModfiyWin.x + DZModfiyWin.wide;
		y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
		GuiFillRect(DZModfiyWin.x+9,y,x - 10,y + 17, backcolor);
		GuiFillRect(DZModfiyWin.x+17,y + 2,x - 20,y + 15, forecolor);
		GuiFont12Align(DZModfiyWin.x+7,y+4,10,FONT_LEFT,"<");
		GuiFont12Align(x - 14,y+4,10,FONT_LEFT,">");
	}
	else{//float
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 32,40,FONT_LEFT,"���ֵ");
		tRange = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMax;
		sprintf(tRangeStr,"%.3f",tRange);
		tRangeStr[15] = '\0';
		GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 32,80,FONT_MID,tRangeStr);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 45,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 47,40,FONT_LEFT,"��Сֵ");
		tRange = dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMin;
		sprintf(tRangeStr,"%.3f",tRange);
		tRangeStr[15] = '\0';
		GuiFont12Align(DZModfiyWin.x + 50,DZModfiyWin.y + 47,80,FONT_MID,tRangeStr);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 60,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 62,40,FONT_LEFT,"��ǰֵ");
		GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 62,\
			DZModfiyWin.x + 128,DZModfiyWin.y + 74, backcolor);
		GuiFont12Align(DZModfiyWin.x + 50,\
			DZModfiyWin.y + 62,80,FONT_MID,dZModfiy.str);
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 75,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiFont12Align(DZModfiyWin.x + 2,DZModfiyWin.y + 77,40,FONT_LEFT,"�޸�Ϊ");
		GuiHPointLine(DZModfiyWin.x,DZModfiyWin.y + 90,\
			DZModfiyWin.x + DZModfiyWin.wide,2,forecolor);
		GuiRPointLine(DZModfiyWin.x + 47,DZModfiyWin.y + 30,\
			DZModfiyWin.y + 90,2,forecolor);	
	}
}

/**
  *@brief ����ֵ�޸İ���
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
  *@brief ��ֵ�޸Ĵ��ں���
  *@param  None
  *@retval None
  */
void DZModfiyFun(void)
{
	static uint8_t flag = 0;//����
	static uint8_t modfiyStr[16];//�޸��ݴ�buff
	static uint8_t modfiyStrP = 0;
	static uint8_t keyIs = 0;//������¼
	static uint32_t inputCursorTick;//�����˸��ʱ
	int16_t x,y;
	static union{
		float f32;
		uint16_t u16[2];
	}ftrans16;
	float fValue;
	if(flag == 0){//��ʼ��
		DZModfiyInit();
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){
			keyIs = dZModfiy.str[0];
		}
		else{
			keyIs = 0;
		}
		modfiyStrP = 0;
		modfiyStr[0] = ' ';
		modfiyStr[0] = '\0';
		flag = 1;
	}
	if(flag == 1){//��ʾ
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//���ִ���
			GuiExchangeColor();
			x = DZModfiyWin.x + DZModfiyWin.wide;
			y = DZModfiyWin.y + 25 + (DZModfiyWin.hight - 43)/2 - 8;
			GuiFillRect(DZModfiyWin.x+18,y + 3,x - 20,y + 15, backcolor);
			GuiFont12Align(DZModfiyWin.x+18,y + 3,x - DZModfiyWin.x - 40,FONT_MID,\
				dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pContent[keyIs]);
			GuiExchangeColor();
			flag = 3;
		}
		else{//float����
			DrawModfiyKey(keyIs);
			GuiFillRect(DZModfiyWin.x + 50,DZModfiyWin.y + 77,\
				DZModfiyWin.x + 128,DZModfiyWin.y + 89, backcolor);
			GuiFont12Align(DZModfiyWin.x + 50,\
				DZModfiyWin.y + 77,78,FONT_MID,modfiyStr);
			flag = 2;
		}
		GuiUpdateDisplayAll();
	}
	if(flag == 2){//�����������
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType == 0){
			if(GetIntervalTick(inputCursorTick) < 500){
				if(modfiyStr[modfiyStrP] != '|'){
					modfiyStr[modfiyStrP] = '|';
					modfiyStr[modfiyStrP + 1] = '\0';
					flag = 1;
				}
			}
			else if(GetIntervalTick(inputCursorTick) < 1000){
				if(modfiyStr[modfiyStrP] != ' '){
					modfiyStr[modfiyStrP] = ' ';
					modfiyStr[modfiyStrP + 1] = '\0';
					flag = 1;
				}
			}
			else{
				inputCursorTick = getCurrentTick();
			}				
		}
	}
	if(flag == 3){//�ȴ�
	}
	if(flag == 4){//��������
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//����		
			dZModfiy.str[0] = modfiyStr[0];
			*(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pVal) = (float)modfiyStr[0];
			dZModfiy.info->SaveModify(0);
			flag = 5;
		}
		else{
			modfiyStr[modfiyStrP] = '\0';
			fValue = atof((const char *)modfiyStr);
			if(fValue <= dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMax &&\
				fValue >= dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].valMin){
				*(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].pVal) = fValue;
				strcpy((char *)dZModfiy.str,(char *)modfiyStr);
				dZModfiy.info->SaveModify(0);
				flag = 5;
			}
			else{//������Χ
				GuiFillRect(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,\
					DZModfiyWin.x + DZModfiyWin.wide - 2,DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
				GuiFont12Align(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"������Χ");
				modfiyStr[modfiyStrP] = ' ';
				modfiyStr[modfiyStrP + 1] = '\0';
				flag = 2;
			}
		}
	}
	if(flag == 5){//�������ȴ��ظ�
		GuiFillRect(DZModfiyWin.x + 57,DZModfiyWin.y + DZModfiyWin.hight - 14,\
			DZModfiyWin.x + DZModfiyWin.wide - 2,DZModfiyWin.y + DZModfiyWin.hight - 2, backcolor);
		GuiFont12Align(DZModfiyWin.x + 57,\
			DZModfiyWin.y + DZModfiyWin.hight - 14,70,FONT_RIGHT,"�޸ĳɹ�");
		GuiUpdateDisplayAll();
		flag = 6;
		inputCursorTick = getCurrentTick();
	}
	if(flag == 6){
		if(GetIntervalTick(inputCursorTick) > 500){//��ʱ�˳�
			flag = 0;
			userGUITopWindowHide();
		}
	}
	if(keyStatus != CancelKey && flag >= 4){
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
		flag = 1;break;
	case DownKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType == 0){
			if(keyIs >= 7)
				keyIs -= 7;
			else
				keyIs += 7;
		}
		flag = 1;break;
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
		flag = 1;break;
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
		flag = 1;break;
	case OkKey:
		if(dZModfiy.info->pRoot[dZModfiy.info->pBuff[dZModfiy.itemIs]].dataType){//�����
			modfiyStr[0] = keyIs;
			flag = 4;
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
				flag = 0;
				userGUITopWindowHide();
			}
			else if(keyIs == 12){
				if(modfiyStrP > 0){
					modfiyStr[modfiyStrP] = '\0';
					modfiyStrP --;
				}			
			}
			else if(keyIs == 13){//ok
				flag = 4;//�����
			}
		}break;
	case CancelKey:
		flag = 0;
		userGUITopWindowHide();
		break;
	default: break;
	}
}

/**
  *@brief ͨ�ö�ֵ��ʾ����
  *@param  Items ��ֵ�ṹָ��
  *@param  itemsNum ��ֵ��
  *@param  cmd ����
  *@param  flag �����־
  *@retval None
  */
void DZModfiyDisplay(DzhiDisplayInfo *info,uint8_t *flag)
{
	const uint8_t listCol[2][DZDISPLAYCOL] = {{73,63,20},{FONT_LEFT,FONT_MID,FONT_MID}};
	static SCROLL *Scroll;//������
	static ListColSet colset;
	static LIST  *list;//�б��ؼ�
	static uint8_t **pText;//�б�����ָ��
	static uint8_t *col1Data;//��ʾ����ָ��
	uint8_t i,itemsNum;
	uint16_t j ;
	uint16_t memMall;//�����ڴ����
	float tempFloat;
	if(*flag == 0){//��ʼ��
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
			*(pText + i*DZDISPLAYCOL + 0) = info->pRoot[info->pBuff[i]].pName;
			if(info->pRoot[info->pBuff[i]].dataType){//����
				tempFloat = *(info->pRoot[info->pBuff[i]].pVal);
				if((uint32_t)tempFloat >= info->pRoot[info->pBuff[i]].dataType){
					tempFloat = 0;
				}
				*(pText + i*DZDISPLAYCOL + 1) = info->pRoot[info->pBuff[i]].pContent[(uint8_t)tempFloat];
			}
			else{//float value
				tempFloat = *(info->pRoot[info->pBuff[i]].pVal);
				sprintf((char *)&col1Data[i*16],"%.3f",tempFloat);
				col1Data[i*16 + 15] = '\0';
				*(pText + i*DZDISPLAYCOL + 1) = &col1Data[i*16];
			}
			*(pText + i*DZDISPLAYCOL + 2) = info->pRoot[info->pBuff[i]].pUnit;
		}
		*flag = 2;
	}
	if(*flag == 2){//ָ����������ָ��
		
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
		if(GetDingZhiSetOrSee() == SET_DINGZHI){//��ֵ�޸�
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
	static uint8_t flag = 0;
	SystemTimeDisplay *disTime,DisTime;
	uint8_t strTime[24];
	uint8_t i,j;
	if(flag == 0){//������ҳ
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
		GuiHLine(HomeWindow.x+1,78,158,forecolor);
		GuiFillRect(HomeWindow.x+1,81,159,94, forecolor);
		GuiHLine(HomeWindow.x+1,96,158,forecolor);
		
		GuiFont12Align(HomeWindow.x+9,102,16,FONT_LEFT,"��");
		GuiFont12Align(HomeWindow.x+9,121,16,FONT_LEFT,"Ϣ");
		GuiFont12Align(HomeWindow.x+9,140,16,FONT_LEFT,"��");
		GuiRLine(HomeWindow.x+30,96,158,forecolor);
		
		for(i=0;i<4;i++){
			GuiFont12Align(HomeWindow.x+31,96+2+i*16,90,FONT_MID,"----");
			GuiRPointLine(HomeWindow.x+31+91,96,96+15+i*16,2,forecolor);
			GuiFont12Align(HomeWindow.x+31+93,96+2+i*16,34,FONT_MID,"--");
			GuiHPointLine(HomeWindow.x+30,96+15+i*16,159,2,forecolor);
		}
		
		GuiFont12Align(HomeWindow.x+31,96+34,90,FONT_MID,"����״̬");
		flag = 1;
	}
	if(flag == 1){
		disTime = &DisTime;
		GuiExchangeColor();
		GetDisplayTime(disTime);
		sprintf(strTime,"20%02d-%02d-%02d   %02d:%02d:%02d",disTime->year,\
			disTime->month,disTime->day,disTime->hour,disTime->min,disTime->s);
		GuiFont12Align(HomeWindow.x+2,82,156,FONT_MID,strTime);
		GuiExchangeColor();
		if(*(yxInfo.pRoot[1].pVal) == 2){
			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"��");
		}
		else{
			GuiFont12Align(HomeWindow.x+31+93,96+34,34,FONT_MID,"��");
		}
		GuiUpdateDisplayAll();
		flag = 2;
	}
	switch(keyStatus){
	case UpKey:
	case DownKey:
		userGUIWindowAdd(&YaoCe2Win);
		flag = 0;
		break;
	case LeftKey:
	case RightKey:
		userGUIWindowAdd(&YaoxinWin);
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
  *@brief mianMenu deal with function ���˵�
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
		case 0://��Ϣ��ѯ
		case 4://�汾��Ϣ
			flag = 3;
			break;
		case 1://��ֵ����
		case 2://��������
		case 3://�����·�
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
		case 0:userGUIMenuAdd(&MenuM0);break;//��Ϣ��ѯ
		case 1:userGUIMenuAdd(&MenuM1);break;//��ֵ����
		case 2:userGUIMenuAdd(&MenuM2);break;//��������
		case 3:userGUIWindowAdd(&CmdSendWin);break;//�����·�
		case 4:userGUIWindowAdd(&VersionWin);break;//�汾��Ϣ
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
  *@brief MenuM0 deal with function ��Ϣ��ѯ
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
		SetDingZhiSetOrSee(NULL_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ��
		userGUIMenuHide();break;
	case RightKey:
	case OkKey:
		SetDingZhiSetOrSee(SEE_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ��ѯ
		switch(MenuM0.currentItem){
		case 0:userGUIWindowAdd(&YaoxinWin);break;//ң�Ų�ѯ
		case 1:userGUIMenuAdd(&MenuM0S1);break;//ң���ѯ
		case 2:userGUIWindowAdd(&SOEWin);break;//SOE��ѯ
		case 3:userGUIWindowAdd(&FaultEventWin);break;//�����¼�
		case 4:userGUIMenuAdd(&MenuM0S4);break;//��ֵ��ѯ
		case 5:userGUIMenuAdd(&MenuM0S5);break;//���ò�ѯ
		default:break;
		}
	}
}

/**
  *@brief MenuM1 deal with function ��ֵ����
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
		SetDingZhiSetOrSee(NULL_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ��
		userGUIMenuHide();
		break;
	case RightKey:
	case OkKey:
		SetDingZhiSetOrSee(SET_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ����
		switch(MenuM1.currentItem){
		case 0:userGUIMenuAdd(&MenuM1S0);break;//��������
		case 1:userGUIWindowAdd(&LogicalFunWin);break;//�߼�����
		case 2:userGUIWindowAdd(&OverLineWarnWin);break;//Խ�߱���
		case 3:userGUIWindowAdd(&OverLoadMuchWin);break;//�ع���
		case 4:userGUIWindowAdd(&OverLoadWin);break;//������
		case 5:userGUIWindowAdd(&OverVoltageWin);break;//����ѹ
		case 6:userGUIWindowAdd(&BatterySetWin);break;//�������
		case 7:userGUIWindowAdd(&AutoResetWin);break;//�Զ�����
		case 8:userGUIMenuAdd(&MenuM1S8);break;//��������
		default:break;
		}
	}
}

/**
  *@brief  MenuM2 deal with function ��������
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
		SetDingZhiSetOrSee(NULL_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ��
		userGUIMenuHide();
		break;
	case RightKey:
	case OkKey:
		SetDingZhiSetOrSee(SET_DINGZHI);//�޸Ķ�ֵ���ñ�־Ϊ����
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
		case 0:userGUIWindowAdd(&YaoCe1Win);break;//һ��ң��
		case 1:userGUIWindowAdd(&YaoCe2Win);break;//����ң��
		case 2:userGUIWindowAdd(&HarmonicWin);break;//г��
		default:break;
		}
	}
}

/**
  *@brief MenuM0S4 deal with function ��ֵ��ѯ
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
		case 0:userGUIMenuAdd(&MenuM0S4S0);break;//��������
		case 1:userGUIWindowAdd(&LogicalFunWin);break;//�߼�����
		case 2:userGUIWindowAdd(&OverLineWarnWin);break;//Խ�߱���
		case 3:userGUIWindowAdd(&OverLoadMuchWin);break;//�ع���
		case 4:userGUIWindowAdd(&OverLoadWin);break;//������
		case 5:userGUIWindowAdd(&OverVoltageWin);break;//����ѹ
		case 6:userGUIWindowAdd(&BatterySetWin);break;//�������
		case 7:userGUIWindowAdd(&AutoResetWin);break;//�Զ�����
		case 8:userGUIMenuAdd(&MenuM0S4S8);break;//��������
		default:break;
		}
	}
}

/**
  *@brief MenuM0S4 deal with function ���ò�ѯ
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
  *@brief MenuM0S4S0 deal with function ��������
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
		case 0:userGUIWindowAdd(&InterphaseOverWin);break;//������
		case 1:userGUIWindowAdd(&ZeroSequenceOverWin);break;//�������
		case 2:userGUIWindowAdd(&CutoutSwitchWin);break;//��·��
		default:break;
		}
	}
}
/**
  *@brief MenuM1S0 deal with function ��������
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
		case 0:userGUIWindowAdd(&InterphaseOverWin);break;//������
		case 1:userGUIWindowAdd(&ZeroSequenceOverWin);break;//�������
		case 2:userGUIWindowAdd(&CutoutSwitchWin);break;//��·��
		default:break;
		}
	}
}

/**
  *@brief MenuM1S8 deal with function ����
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
		case 0:userGUIWindowAdd(&RingUniteWin);break;//�ϻ�
		case 1:userGUIWindowAdd(&BreakDownWin);break;//����Ͷ��
		default:break;
		}
	}
}

/**
  *@brief CmdSend deal with function �����·�
  *@param  None
  *@retval None
  */
static void CmdSendWinFun(void)
{
	const  uint8_t listCol[2][1] = {{156},{FONT_MID}};
	static uint8_t flag = 0;//�����־
	static SCROLL *Scroll;//������
	static ListColSet colset;
	static LIST  *list;//�б��ؼ�
	static uint8_t **pText;//�б�����ָ��
	static uint8_t messageFlag = 0;//�����־
	uint16_t memMall;
	static int8_t currentNum;
	uint8_t i;
	if(flag == 0){//��ʼ���������ڴ�
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
		flag = 1;
	}
	if(flag == 1){//��ȡ��Ӧ����	
		for(i = 0;i < hcmdInfo.itemsNum;i++){
			*(pText + i) = hcmdInfo.pHmiCmd[i].name;
		}
		flag = 2;
	}
	if(flag == 2){//��ʾ
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
		flag = 3;
	}
	if(flag == 3){
	}
	if(flag == 4){
		if(messageFlag == 1){
			hcmdInfo.cmdfun(hcmdInfo.pHmiCmd[currentNum].cmd);
		}
		flag = 2;
	}
	switch(keyStatus){
	case LeftKey:
	case UpKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = Scroll->max - 1;
		}
		flag = 2;
		break;
	case RightKey:
	case DownKey:
		currentNum ++;
		if(currentNum > Scroll->max - 1){
			currentNum = 0;
		}
		flag = 2;
		break;
	case OkKey:
		flag = 4;
		messageFlag = 0;
		SetMessageWin(23,40,"ȷ��ִ��?",&messageFlag);
		userGUIWindowAdd(&MessageWin);
		break;
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
  *@brief  �汾��Ϣ���ݴ���
  *@param  None
  *@retval None
  */
static void ConstParDataResult(const uint8_t *pData,uint8_t *outData,uint8_t len)
{
	uint8_t j;
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
  *@brief  �汾��Ϣ
  *@param  None
  *@retval None
  */
static void VersionWinFun(void)
{
	static uint8_t flag = 0;//�����־
	static SCROLL *Scroll;//������
	static uint8_t *col1Data;//��ʾ����ָ��		
	static int8_t currentNum;
	uint16_t memMall;
	uint8_t i,y;
	if(flag == 0){//��ʼ���������ڴ�
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
		flag = 1;
	}
	if(flag == 1){//��ȡ��Ӧ����
		flag = 2;
	}
	if(flag == 2){//��ʾ
		y = VersionWin.y + 18;
		GuiFillRect(VersionWin.x+1,y,155,158, backcolor);
		for(i = 0; i < CONSTPARDISPLAYROW; i++){
			if((i + currentNum * CONSTPARDISPLAYROW) >= versInfo.num){
				GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//ˮƽ��
				break;
			}
			GuiHLine(VersionWin.x,y+i*28+0,155,forecolor);//ˮƽ��		
			GuiFillRect(VersionWin.x+1,y + i*28+1,155,y + i*28+14, forecolor);
			GuiExchangeColor();
			GuiFont12Align(VersionWin.x + 2,y + i*28+2,133,FONT_LEFT,
				versInfo.pItems[i + currentNum * CONSTPARDISPLAYROW].pName);
			GuiExchangeColor();
			ConstParDataResult(versInfo.pItems[i + currentNum * CONSTPARDISPLAYROW].pVal,col1Data,24);
			GuiFont12Align(VersionWin.x+2,y + i*28+15,153,FONT_MID,col1Data);
		}
		Scroll->lump = currentNum + 1;
		GuiVScroll(Scroll);
		GuiUpdateDisplayAll();
		flag = 3;
	}
	if(flag == 3){
	}
	if(keyStatus != CancelKey && flag <= 1){
		keyStatus = NoKey;
	}
	switch(keyStatus){
	case LeftKey:
	case UpKey:
		currentNum --;
		if(currentNum < 0){
			currentNum = Scroll->max - 1;
		}
		flag = 1;
		break;
	case RightKey:
	case DownKey:
		currentNum ++;
		if(currentNum > Scroll->max - 1){
			currentNum = 0;
		}
		flag = 1;
		break;
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
  *@brief  ������
  *@param  None
  *@retval None
  */
static void InterphaseOverFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_INTERGHASE],&flag);
}

/**
  *@brief �������
  *@param  None
  *@retval None
  */
static void ZeroSequenceOverFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_ZERO_SEQUE],&flag);
}

/**
  *@brief ��·��
  *@param  None
  *@retval None
  */
static void CutoutSwitchFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_OTHER_PROTEC],&flag);
}

/**
  *@brief �߼�����
  *@param  None
  *@retval None
  */
static void LogicalFunFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_LOGICAL_FUN],&flag);
}

/**
  *@brief Խ�߱���
  *@param  None
  *@retval None
  */
static void OverLineWarnFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_LIMITATION],&flag);
}

/**
  *@brief �ع���
  *@param  None
  *@retval None
  */
static void OverLoadMuchFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_HEAVY_LOAD],&flag);
}

/**
  *@brief ������
  *@param  None
  *@retval None
  */
static void OverLoadFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_OVERLOAD],&flag);
}

/**
  *@brief ����ѹ
  *@param  None
  *@retval None
  */
static void OverVoltageFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_LIMIT_V_F],&flag);
}

/**
  *@brief �������
  *@param  None
  *@retval None
  */
static void BatterySetFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_IBATTERY_SET],&flag);
}

/**
  *@brief �Զ�����
  *@param  None
  *@retval None
  */
static void AutoResetFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_AUTO_RESET],&flag);
}

/**
  *@brief �ϻ�
  *@param  None
  *@retval None
  */
static void RingUniteFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_LOOP_CLOSE],&flag);
}

/**
  *@brief ����Ͷ��
  *@param  None
  *@retval None
  */
static void BreakDownFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi1Info[DZ1_FAULT_SWITCH],&flag);
}

/**
  *@brief ң�Ŵ�������
  *@param  None
  *@retval None
  */
static void YaoxinFun(void)
{
	const  uint8_t listCol[2][2] = {{90,66},{FONT_LEFT,FONT_MID}};
	static uint8_t flag = 0;//�����־
	static SCROLL *Scroll;//������
	static ListColSet colset;
	static LIST  *list;//�б��ؼ�
	static uint8_t **pText;//�б�����ָ��
	static uint8_t *col1Data;//��ʾ����ָ��	
	static uint32_t YaoXinTick;
	
	uint16_t memMall;
	uint8_t i;
	uint8_t itemsNum;
	
	if(flag == 0){//��ʼ���������ڴ�
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
	if(flag == 1){//��ȡ��Ӧ����
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
		flag = 2;
	}
	if(flag == 2){//��ʾ
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
	if(flag == 3){//���ң�ű仯
		if(GetIntervalTick(YaoXinTick) > 1000){
			flag = 1;
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
  *@brief ң�⴦����ʾ
  *@param  items ң����ʾ��
  *@param  itemsNum ң������
  *@param  items modbus����
  *@retval None
  */
void yaoCeDisplay(YaoceDisplayInfo *info)
{
	const uint8_t listCol[2][YaoCeDISPLAYCOL] = {//����������
		{50,75,31},{FONT_LEFT,FONT_MID,FONT_MID}};
	static uint8_t flag = 0;//�����־
	static SCROLL *Scroll;//������
	static ListColSet colset;
	static LIST  *list;//�б��ؼ�
	static uint8_t **pText;//�б�����ָ��
	static uint8_t *col1Data;//��ʾ����ָ��		
	static uint32_t YaoCeTick;//ѭ����ʱ
	
	uint16_t memMall;
	uint8_t i;
	uint8_t itemsNum = info->num;/* ����ʾ��Ŀ */
	float tempFloat;	
	if(flag == 0){//��ʼ���������ڴ�
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
		//��ȡ������
		flag = 1;
	}
	if(flag == 1){
		for(i = 0;i < itemsNum;i++){
			tempFloat = *(info->pRoot[info->pBuff[i]].pVal);
			if(((uint8_t)isnan(tempFloat)) != 0){
				sprintf((char *)&col1Data[i*16],"%s","NaN");
			}
			else{
				sprintf((char *)&col1Data[i*16],"%.3f",tempFloat);
			}
			col1Data[i*16 + 15] = '\0';
			*(pText + i*YaoCeDISPLAYCOL + 0) = info->pRoot[info->pBuff[i]].pName;
			*(pText + i*YaoCeDISPLAYCOL + 1) = &col1Data[i*16];
			*(pText + i*YaoCeDISPLAYCOL + 2) = info->pRoot[info->pBuff[i]].pUnit;
		}
		flag = 2;
	}
	if(flag == 2){//��ʾ	
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
		flag = 3;
	}
	if(flag == 3){
		if(GetIntervalTick(YaoCeTick) > YAOCE_UPDATATIME){//������
			flag = 1;
		}
	}
	switch(keyStatus){
	case UpKey:
		if(flag > 1){
			if(list->drawRow > DISPLAYLISTROW)
				list->drawRow -= DISPLAYLISTROW;
			else
				list->drawRow = 0;
			flag = 2;
			break;
		}	
	case DownKey:
		if(flag > 1){
			list->drawRow += DISPLAYLISTROW;
			if(list->drawRow + DISPLAYLISTROW > list->row){
				list->drawRow = list->row - DISPLAYLISTROW;
			}
			flag = 2;
			break;
		}
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
  *@brief һ��ң�⴦������
  *@param  None
  *@retval None
  */
static void YaoCe1Fun(void)
{
	yaoCeDisplay(&yceInfo[0]);
}

/**
  *@brief ����ң�⴦������
  *@param  None
  *@retval None
  */
static void YaoCe2Fun(void)
{
	yaoCeDisplay(&yceInfo[1]);
}
/**
  *@brief г����������
  *@param  None
  *@retval None
  */
static void HarmonicFun(void)
{
	yaoCeDisplay(&yceInfo[2]);
}


/**
  *@brief SOE��������
  *@param  None
  *@retval None
  */
static void SOEFun(void)
{
	static struct SOEDisplay *soeStr;
	static SCROLL Scroll ={156,18,141,2,1};//������
	static uint8_t currentNum;
	static uint8_t flag = 0;
	SystemTimeDisplay *tTim;
	uint8_t tRes;
	uint8_t i;
	uint16_t y;
	
	if(flag == 0){//��ʼ���������ڴ�
		soeStr = (struct SOEDisplay *)&userGUIBuff[0];
		soeStr->pRead = 0;
		soeStr->allNum = GetSoeNum();
		currentNum = 1;
		flag = 1;
	}
	if(flag == 1){
		if(soeStr->allNum == 0){//û��SOE
			GuiFont12Align(SOEWin.x+2,SOEWin.y + 30,SOEWin.x+SOEWin.wide-4,FONT_MID,"��ǰû��SOE");
			GuiUpdateDisplayAll();
			flag = 3;
		}
		else{
			if(soeStr->allNum % SOEDISPLAYROW){
				Scroll.max = soeStr->allNum / SOEDISPLAYROW + 1;
			}
			else{
				Scroll.max = soeStr->allNum / SOEDISPLAYROW;
			}
			flag = 2;
		}
	}
	if(flag == 2){
		soeStr->pRead = (currentNum - 1) * SOEDISPLAYROW;
		if(soeStr->pRead >= soeStr->allNum){//SOE���һ��
			flag = 3;
		}
		y = SOEWin.y + 18;
		GuiFillRect(SOEWin.x+1,y,155,158, backcolor);
		for(i = 0; i < SOEDISPLAYROW; i ++){
			if(soeStr->pRead >= soeStr->allNum){//SOE���һ��
				GuiHLine(SOEWin.x,y+i*28,155,forecolor);
				break;
			}
			tRes = GetSoeNoContent(soeStr->pRead,&soeStr->pSoe);
			if(tRes != 0){//��ȡSOE�д�
				GuiHLine(SOEWin.x,y+i*28,155,forecolor);
				break;
			}
			tTim = &soeStr->pSoe.time;
			sprintf((char *)soeStr->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
				tTim->year,tTim->month,tTim->day,tTim->hour,tTim->min,tTim->s,tTim->ms);
			soeStr->time[23] = '\0';
			sprintf((char *)soeStr->itemNum,"%d",soeStr->pRead + 1);
			soeStr->itemNum[3] = '\0';
			GuiHLine(SOEWin.x,y+i*28+0,155,forecolor);//ˮƽ��
			GuiFillRect(SOEWin.x+1,y+i*28,SOEWin.x+20,y+i*28+13, forecolor);
			GuiExchangeColor();
			GuiFont12Align(SOEWin.x+1,y + i*28+1,19,FONT_MID,soeStr->itemNum);//���
			GuiExchangeColor();	
			GuiRPointLine(SOEWin.x+20,y+i*28+1,y+i*28+13,2,forecolor);//��ֱ��
			GuiFont12Align(SOEWin.x + 21,y + i*28+2,133,FONT_RIGHT,soeStr->time);
			GuiHPointLine(SOEWin.x,y+i*28+13,155,2,forecolor);
			GuiFont12Align(SOEWin.x+2,y + i*28+15,72,FONT_MID,soeStr->pSoe.pName);
			GuiRPointLine(SOEWin.x+10+72,y+i*28+15,y+i*28+27,2,forecolor);
			if(soeStr->pSoe.highlight){//������ʾ
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
		flag = 3;
	}
	if(flag == 3){
		if(CheckSoeUpdata()){//SOE�и���
			soeStr->allNum = GetSoeNum();
			soeStr->pRead = 0;
			currentNum = 1;
			flag = 1;
		}
	}
	switch(keyStatus){
	case UpKey:
	case LeftKey:
		if(flag > 1){
			currentNum --;
			if(currentNum < 1){
				currentNum = Scroll.max;
			}
		}
		flag = 1;
		break;
	case DownKey:
	case RightKey:
		if(flag > 1){
			currentNum ++;
			if(currentNum > Scroll.max){
				currentNum = 1;
			}
		}
		flag = 1;
		break;
	case OkKey:break;
	case CancelKey:
		currentNum = 0;
		flag = 0;
		userGUITopWindowHide();
		userGUITopWindowRedraw();
		userGUIMenuRedraw();
		break;
	default:break;
	}
}

/**
  *@brief �����¼���������
  *@param  None
  *@retval None
  */
static void FaultEventFun(void)
{
	static uint8_t flag = 0;
	static struct EventDisplay *pEventStr;
	static SCROLL Scroll ={156,18,141,2,1};//������
	static SCROLL yaoCeScroll ={153,47,112,2,1};//������
	static int16_t currentNum;
	static int8_t yaoCeNum;
	uint8_t i;
	uint16_t y;
	
	if(flag == 0){//��ʼ���������ڴ�
		pEventStr = (struct EventDisplay *)&userGUIBuff[0];
		pEventStr->pRead = 0;
		pEventStr->allNum = GetFeventNum();
		currentNum = 1;
		yaoCeNum = 1;
		flag = 1;
	}
	if(flag == 1){
		if(pEventStr->allNum == 0){//û�й����¼�
			GuiFont12Align(FaultEventWin.x+2,FaultEventWin.y + 50,\
			FaultEventWin.x+FaultEventWin.wide-4,FONT_MID,"��ǰû�й����¼�");
			GuiUpdateDisplayAll();
			flag = 4;
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
			flag = 2;
		}
	}
	if(flag == 2){//��ͼ
		y = FaultEventWin.y + 18;
		GuiFillRect(FaultEventWin.x+1,y,155,158, backcolor);
		GuiHLine(FaultEventWin.x,y,155,forecolor);//ˮƽ��	
		GuiFillRect(FaultEventWin.x+1,y,FaultEventWin.x+20,y+13, forecolor);
		SystemTimeDisplay *tTim;
		tTim = &pEventStr->pFevent.time;
		sprintf((char *)pEventStr->time,"%02d/%02d/%02d-%02d:%02d:%02d.%03d",\
			tTim->year,tTim->month,tTim->day,tTim->hour,tTim->min,tTim->s,tTim->ms);
		pEventStr->time[23] = '\0';
		sprintf((char *)pEventStr->itemNum,"%d",pEventStr->pRead + 1);
		pEventStr->itemNum[3] = '\0';
		GuiExchangeColor();
		GuiFont12Align(FaultEventWin.x+1,y+2,20,FONT_MID,pEventStr->itemNum);//���
		GuiExchangeColor();	
		GuiRPointLine(FaultEventWin.x+20,y+1,y+13,2,forecolor);//��ֱ��
		GuiFont12Align(FaultEventWin.x + 21,y+2,133,FONT_RIGHT,pEventStr->time);
		GuiHPointLine(FaultEventWin.x,y+13,155,2,forecolor);
		GuiFont12Align(FaultEventWin.x+2,y+15,72,FONT_LEFT,pEventStr->pFevent.pName);
		GuiRPointLine(FaultEventWin.x+10+72,y+15,y+27,2,forecolor);
		GuiFont12Align(FaultEventWin.x+10+73,y+15,70,FONT_RIGHT,pEventStr->pFevent.pContent);	
		Scroll.lump = currentNum;
		GuiVScroll(&Scroll);
		GuiHLine(FaultEventWin.x,y+28,155,forecolor);//ˮƽ��	
		flag = 3;
	}
	if(flag == 3){
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
		flag = 4;
	}
	if(flag == 4){
		if(CheckFeventUpdata()){//SOE�и���
			currentNum = 1;
			flag = 1;
		}
	}
	
	switch(keyStatus){
	case UpKey:
		if(flag > 3){
			yaoCeNum --;
			if(yaoCeNum < 1){
				yaoCeNum = yaoCeScroll.max;
			}
		}
		flag = 3;
		break;
	case LeftKey:
		if(flag > 1){
			currentNum --;
			if(currentNum < 1){
				currentNum = Scroll.max;
			}
		}
		flag = 1;
		break;
	case DownKey:
		if(flag > 3){
			yaoCeNum ++;
			if(yaoCeNum > yaoCeScroll.max){
				yaoCeNum = 1;
			}
		}
		flag = 3;
		break;
	case RightKey:
		if(flag > 1){
			currentNum ++;
			if(currentNum > Scroll.max){
				currentNum = 1;
			}
		}
		flag = 1;
		break;
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
  *@brief �������ô�������
  *@param  None
  *@retval None
  */
static void BasicSetFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi0Info[DZ0_CONFIG],&flag);
}

/**
  *@brief ��Ư���ô�������
  *@param  None
  *@retval None
  */
static void ZeroDriftFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi0Info[DZ0_ZERODRIFT],&flag);
}

/**
  *@brief �������ô�������
  *@param  None
  *@retval None
  */
static void DeadZoneFun(void)
{
	static uint8_t flag = 0;
	DZModfiyDisplay(&dzhi0Info[DZ0_DEADEZONE],&flag);
}

/* END */
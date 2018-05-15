
#ifndef __GUI101CMD_H
#define	__GUI101CMD_H

#include "stdint.h"

/* 总行数y 总列数 x */
#define  GUI_LCM_XMAX	160
#define  GUI_LCM_YMAX	160
#define  GUIXMAX	GUI_LCM_XMAX
#define  GUIYMAX	GUI_LCM_YMAX

/* 定义坐标类型 */
typedef int32_t lkdCoord;
/* 定义颜色类型 */
typedef uint8_t lkdColour;

/* 字体对齐方式 */
typedef enum{
	FONT_LEFT,	/* 左对齐 */
	FONT_MID,		/* 居中 */
	FONT_RIGHT,	/* 右对齐 */
}FontFlag;

/* 进度条 */
typedef struct{
	int16_t x;
	int16_t y;
	uint16_t hight;
	uint16_t max;
	uint16_t lump;
}SCROLL;

/* 定义位图结构体 */
typedef struct{
	uint8_t number;
	uint8_t *bitmap;/* 位图数组 */
	uint16_t wide;/* 位图宽 */
	uint16_t high;/* 位图高 */
	uint16_t beginx;/* 位图开始坐标 */
	uint16_t beginy;/* 位图开始坐标 */
}lkdBitmap;

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


#define PACKBUFFMAX 1024
typedef struct{
	uint8_t state;
	uint8_t packBuff[PACKBUFFMAX];
	uint16_t pIn;
	uint8_t cmdNum;
}Gui101CmdControl;

/* 声明前景色/背景色 */
extern lkdColour forecolor,backcolor;
/* 声明cmd101发送事件 */
extern struct rt_event Cmd101SendEvent;

void GuiBiasLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdCoord y1, lkdColour color);
void GuiHPointLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, uint8_t interval, lkdColour color);
void GuiRPointLine(lkdCoord x0, lkdCoord y0, lkdCoord y1,uint8_t interval, lkdColour color);
void GuiHLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdColour color);
void GuiRLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, lkdColour color);
void GuiRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
void GuiFillRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
void GuiFont12(lkdCoord x, lkdCoord y, uint8_t *str);
void GuiFont12Align(lkdCoord x, lkdCoord y,uint16_t wide, FontFlag flag,uint8_t *str);
void GuiUpdateDisplayAll(void);
void GuiSetbackcolor(lkdColour colour);
void GuiSetForecolor(lkdColour colour);
void GuiExchangeColor(void);
void GuiAreaBitMap(lkdBitmap *bitMap,\
	lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag);
void OpenLcdDisplay(void);
void CloseLcdDisplay(void);
void GuiButton(int16_t x,int16_t y,uint8_t *keyName,uint8_t flag);
void GuiVScroll(SCROLL *pScroll);

/* 按键处理函数 */
enum KeyStatus GetKeyStatus(void);
void SetKeyStatus(enum KeyStatus keyIs);
void SetKeyIsNoKey(void);




void TestMain(void);

extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);
extern uint8_t DLT634_HMI_SLAVE_C_REPLY(uint8_t drvid,uint8_t *pbuf);

#endif /* __GUI101CMD_H */

/* END */

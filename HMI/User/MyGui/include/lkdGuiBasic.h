#ifndef __LKDGUIBASIC_H
#define	__LKDGUIBASIC_H

#include "lkdGuiColour.h"

/* 总行数y 总列数 x */
#define  GUI_LCM_XMAX	160
#define  GUI_LCM_YMAX	160
#define  GUIXMAX	GUI_LCM_XMAX
#define  GUIYMAX	GUI_LCM_YMAX

/* 定义坐标类型 */
typedef int32_t lkdCoord;

/* 定义位图结构体 */
typedef struct{
	uint8_t *bitmap;/* 位图数组 */
	uint16_t wide;/* 位图宽 */
	uint16_t high;/* 位图高 */
	uint16_t beginx;/* 位图开始坐标 */
	uint16_t beginy;/* 位图开始坐标 */
}lkdBitmap;

/* 画点 */
void GuiPoint(lkdCoord x, lkdCoord y, lkdColour color);
/* 画斜线 */
void GuiBiasLine(lkdCoord x0, lkdCoord y0,lkdCoord x1, lkdCoord y1, lkdColour color);
/* 画水平线 */
void GuiHLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdColour color);
/* 画垂直线 */
void GuiRLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, lkdColour color);
/* 画水平点线 */
void GuiHPointLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, uint8_t interval, lkdColour color);
/* 画垂直点线 */
void GuiRPointLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, uint8_t interval, lkdColour color);
/* 画矩形 */
void GuiRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
/* 画填充矩形 */
void GuiFillRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color);
/* 画位图 */
void GuiBitMap(uint8_t *bitMap,lkdCoord x0, lkdCoord y0,uint32_t xlen,uint32_t ylen,uint8_t flag);

void GuiAreaBitMap(lkdBitmap *bitMap,lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag);


/* ---由外部驱动提供--- */

/* 底层打点函数 */
extern void GuiDrawPoint(lkdCoord x, lkdCoord y, lkdColour color);
/* 底层读点函数 */
extern void GuiReadPoint(lkdCoord x, lkdCoord y, lkdColour *pColor);

#endif /* __GWGUIBASIC_H */

/* END */

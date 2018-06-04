#ifndef __LKDGUIFONT_H
#define	__LKDGUIFONT_H

#include "lkdGuiDriver.h"

typedef enum{
	FONT_LEFT,	/* 左对齐 */
	FONT_MID,		/* 居中 */
	FONT_RIGHT,	/* 右对齐 */
}FontFlag;

typedef struct{
	uint8_t wide;
	uint8_t high;
	uint8_t rWide;
	uint8_t rhigh;
	uint8_t (*getFont)(uint8_t firstCode, uint8_t secondCode, uint8_t *pBuff);
}lkdFont;

/* 12x12字体函数 */
void GuiFont12(lkdCoord x, lkdCoord y, uint8_t *str);
/* 12x12字体对齐显示函数 */
void GuiFont12Align(lkdCoord x, lkdCoord y,uint16_t wide,	FontFlag flag,uint8_t *str);
#endif /* __LKDGUIFONT_H */

/* END */

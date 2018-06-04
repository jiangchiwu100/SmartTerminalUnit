#ifndef __LKDGUICONTROL_H
#define	__LKDGUICONTROL_H

#include "lkdGuiBasic.h"

typedef struct{
	int16_t x;
	int16_t y;
	uint16_t hight;
	uint16_t max;
	uint16_t lump;
}SCROLL;

void GuiVScroll(SCROLL *pScroll);
void GuiButton(int16_t x,int16_t y,uint8_t *keyName,uint8_t flag);
#endif /* __LKDGUICONTROL_H */

/* END */

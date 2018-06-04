#ifndef __LKDGUIDRIVER_H
#define	__LKDGUIDRIVER_H

#include "lkdGuiBasic.h"

/* 更新 */
void GuiUpdateDisplayAll(void);
/* 画点 */
void GuiDrawPoint(lkdCoord x, lkdCoord y, lkdColour color);
/* 读点 */
void  GuiReadPoint(lkdCoord x, lkdCoord y, lkdColour *pColor);

#endif /* __LKDGUIDRIVER_H */

/* END */

#ifndef __GWGUIMENU_H
#define	__GWGUIMENU_H

#include "gwGuiDriver.h"

#undef NULL
#define NULL 0
#define MENU_MAXDEEP 16		/* 菜单最大深度 */

struct DrawMenuInfo{
	uint16_t beginX;		/* 开始坐标 */
	uint16_t beginY;
	uint16_t endX;			/* 结束坐标 */
	uint16_t endY;
	uint16_t hight;			/* 高度 */
	uint16_t currentItem;	/* 当前Item */
	uint16_t beginItem;		/* 开始绘制Item */
};
typedef struct DrawMenuInfo MenuInfo;
struct ItemInfo{
	uint8_t flag1;/*  */
	uint8_t *name;/* 名称 */
	void(*ItemFunction)(void);/* Item对应的服务程序 */ 
};
struct GuiMenuTree{
	struct ItemInfo thisInfo;/* 此节点信息 */
	struct GuiMenuTree *firstChild;/* 子节点 */
	struct GuiMenuTree *nextSibling;/* 兄弟节点 */	
};
typedef struct GuiMenuTree MenuItem;

struct GuiMenuManage{
	uint8_t topMenu;
	uint16_t currentItem[MENU_MAXDEEP];
	uint16_t beginItem[MENU_MAXDEEP];
	MenuItem *userMenu[MENU_MAXDEEP];
};
typedef struct GuiMenuManage MenuManage;

struct GuiMenuHandle{
	MenuManage menuStack;
	MenuInfo drawinfo;
	MenuItem *rootNode;
};
typedef struct GuiMenuHandle MenuHandle;
#endif /* __GWGUIMENU_H */

/* END */

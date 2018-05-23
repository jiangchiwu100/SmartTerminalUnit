
#ifndef __LKDGUIMENU_H
#define	__LKDGUIMENU_H

#include "stdint.h"

#undef  NULL
#define NULL 0

#define MAXMENUITEM  10		//菜单最多可容纳的菜单项条数
#define MENU_BEGINHIGH 15 	//菜单开始坐标偏移
#define MENU_BEGINWIDE 0 	//菜单开始坐标偏移
#define MENUITEM_HIGH	15		//菜单项的高度
#define MENUITEM_WIDE	12*6	//菜单项的宽度
#define MAX_NEST_MENU 2

/* 菜单定义 */
struct MENU_
{
	int16_t x;							// 起始坐标
	int16_t y;							// 起始坐标
	struct MENU_  *fatherMenu;			// 所属窗口
	uint8_t  currentItem;				// 当前菜单项
	uint8_t  num;						// 菜单项个数
	uint8_t Itemflag[MAXMENUITEM];		// 菜单项有无子项
	uint8_t  *itemName[MAXMENUITEM];	// 菜单项名称
	void(*MenuFunction)(void);	// 菜单项对应的服务程序
};
typedef  struct MENU_ MENU;

/* 菜单管理 */
struct MenuManage{
	uint8_t pMenu;
	MENU *userMenu[12];
};


void GUIMenuDraw(MENU *pMenu);

/* 隐藏菜单 */
void userGUIMenuHide(void);
/* 添加菜单 */
void userGUIMenuAdd(MENU *pMenu);
/* 菜单对应函数调用 */
void userGUIMenuDisplay(void);
/* 重绘菜单 */
void userGUIMenuRedraw(void);
/* 菜单选中行修改 */
void userGUIMenuIremModfiy(uint8_t flag);
/* 获取pMenu */
uint8_t GetMenuManagepMenu(void);

void MenuManageInit(void);
#endif /* __LKDGUIMENU_H */

/* END */

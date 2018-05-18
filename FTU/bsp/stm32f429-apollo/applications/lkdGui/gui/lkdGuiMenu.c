/**
  * @file   lkdGuiMenu.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/14
  * @brief   lkdGui菜单管理
  */

#include "lkdGuiMenu.h"
#include "gui_101_cmd.h"

struct MenuManage menuManage;
/**
  *@brief menu draw
  *@param  menu handle
  *@retval None
  */
static void MenuDrawXY(MENU *pMenu)
{
	int16_t x1,y1,x2,y2;
	uint8_t item;
	x1 = pMenu->x;
	y1 = pMenu->y;
	x2 = x1 + MENUITEM_WIDE + 1;
	if(y1 + pMenu->num*MENUITEM_HIGH >= GUIYMAX){
		y2 = GUIYMAX - 1;
	}
	else{
		y2 = y1 + pMenu->num*MENUITEM_HIGH +1;
	}
	if( x1 < MENU_BEGINWIDE || y1 < MENU_BEGINHIGH ||\
		x2 >= GUIXMAX || y2 >= GUIYMAX){
		return;
	}
	GuiFillRect(x1,y1,x2,y2, backcolor);
	GuiRect(x1,y1,x2,y2, forecolor);
	GuiRLine(x2-1,y1,y2-1, forecolor);
  GuiHLine(x1,y2-1,x2-1, forecolor);
	
	for(item = 0;item < pMenu->num;item++){
		if(item == pMenu->currentItem){
			GuiFillRect(x1+1, y1+1, x1+MENUITEM_WIDE-1, y1+MENUITEM_HIGH, forecolor);
			GuiExchangeColor();
			GuiFont12Align(x1+2, y1+2, 100, FONT_LEFT,pMenu->itemName[item]);
			if(pMenu->Itemflag[item] != 0){
				GuiFont12Align(x1+MENUITEM_WIDE-8, y1+2, 10, FONT_LEFT,">");
			}
			GuiExchangeColor();
			GuiHLine(x1+1,y1+MENUITEM_HIGH,x1+MENUITEM_WIDE-1, forecolor);
		}
		else{	
			GuiFillRect(x1+1, y1+1, x1+MENUITEM_WIDE-1, y1+MENUITEM_HIGH, backcolor);
			GuiFont12Align(x1+2, y1+2, 100, FONT_LEFT, pMenu->itemName[item]);
			if(pMenu->Itemflag[item] != 0){
				GuiFont12Align(x1+MENUITEM_WIDE-8, y1+2, 10, FONT_LEFT,">");
			}
			GuiHLine(x1+1,y1+MENUITEM_HIGH,x1+MENUITEM_WIDE-1, forecolor);	
		}
		y1 += MENUITEM_HIGH;
		if(y1 >= GUIYMAX){
			break;
		}
	}	
}

/**
  *@brief menu draw (include all Menu deal with)
  *@param  menu handle
  *@retval None
  */
void GUIMenuDraw(MENU *pMenu)
{
	MENU *pTempMenu;
	MENU *pfMenu[MAX_NEST_MENU];
	int8_t fCount = 0;
	int16_t x,y;
	
	/* Calculate y coordinate */
	if(pMenu->fatherMenu != NULL){
		y = pMenu->fatherMenu->currentItem * MENUITEM_HIGH \
					+ pMenu->fatherMenu->y;
	}
	else{
		y = MENU_BEGINHIGH;
	}
	while(1){
		if(y + pMenu->num * MENUITEM_HIGH > GUIYMAX){
			y -= MENUITEM_HIGH;
		}
		else if(y <= MENU_BEGINHIGH){
			break;
		}
		else{
			break;
		}
	}	
	/* Calculate x coordinate */
	pTempMenu = pMenu;
	if(pTempMenu->fatherMenu != NULL){
		x = pTempMenu->fatherMenu->x + MENUITEM_WIDE;
	}
	else{
		x = MENU_BEGINWIDE;
	}
	
	pTempMenu = pMenu;
	pTempMenu->x = x;
	pTempMenu->y = y;
	
	
	if(x + MENUITEM_WIDE > GUIXMAX){
		x -= MENUITEM_WIDE;
		while(1){
			if(x < MENU_BEGINWIDE){
				break;
			}
			pfMenu[fCount] = pTempMenu;
			pTempMenu = pTempMenu->fatherMenu;
			fCount ++;
			x -= MENUITEM_WIDE;
		}
		GuiFillRect(MENU_BEGINWIDE+1,MENU_BEGINHIGH+1,\
			GUIXMAX - 2, GUIYMAX - 2, backcolor);
		x = MENU_BEGINWIDE;
		while(fCount > 0){
			pfMenu[fCount-1]->x = x;
			MenuDrawXY(pfMenu[fCount-1]);
			x += MENUITEM_WIDE;
			fCount --;
		}		
	}	
	else{
		MenuDrawXY(pTempMenu);
	}
}

/**
  *@brief  菜单隐藏
  *@param  菜单指针
  *@retval None
  */
void GUIMenuHide(MENU *pMenu)
{
	MENU *pTempMenu;
	MENU *pfMenu[MAX_NEST_MENU];
	int8_t fCount = 0;
	int16_t x;
	if(pMenu == NULL){
		return;
	}
	pTempMenu = pMenu;
	x = MENU_BEGINWIDE;
	while(1){
		if(pTempMenu->fatherMenu != NULL){
			if(x + MENUITEM_WIDE > GUIXMAX){
				break;
			}
			pfMenu[fCount] = pTempMenu->fatherMenu;
			fCount ++;
			pTempMenu = pTempMenu->fatherMenu;
			x += MENUITEM_WIDE;
		}
		else{
			break;
		}
	}
	GuiFillRect(MENU_BEGINWIDE+1,MENU_BEGINHIGH+1,\
			GUIXMAX - 2, GUIYMAX - 2, backcolor);
	x = MENU_BEGINWIDE;
	while(fCount > 0){
		pfMenu[fCount - 1]->x = x;
		MenuDrawXY(pfMenu[fCount - 1]);
		x += MENUITEM_WIDE;
		fCount --;
	}
}

/**
  *@brief 菜单当前item控制
  *@param  菜单指针
  *@param  flag 0 current item -- ,1 current item ++
  *@retval None
  */
void GUIMenuCurrentItemModfiy(MENU *pMenu,uint8_t flag)
{
	uint8_t tempCurrentItem;
	int16_t y,ty;
	if(flag > 0){//item add
		if(pMenu->currentItem + 1 >= pMenu->num){
			tempCurrentItem = 0;
		}
		else{
			tempCurrentItem = pMenu->currentItem + 1;
		}
	}
	else{
		if(pMenu->currentItem == 0){
			tempCurrentItem = pMenu->num - 1;
		}
		else{
			tempCurrentItem = pMenu->currentItem - 1;
		}
	}
	
	ty = pMenu->y + tempCurrentItem*MENUITEM_HIGH;
	y = pMenu->y + pMenu->currentItem*MENUITEM_HIGH;
	
	GuiFillRect(pMenu->x + 1, y+1, pMenu->x + MENUITEM_WIDE-1, y+MENUITEM_HIGH-1,backcolor);
	GuiFont12Align(pMenu->x + 2, y+2,100,FONT_LEFT,pMenu->itemName[pMenu->currentItem]);
	if(pMenu->Itemflag[pMenu->currentItem] != 0){
		GuiFont12Align(pMenu->x + MENUITEM_WIDE-8, y+2,10,FONT_LEFT,">");
	}
	
	GuiFillRect(pMenu->x + 1, ty+1, pMenu->x + MENUITEM_WIDE-1, ty+MENUITEM_HIGH-1,forecolor);
	GuiExchangeColor();
	GuiFont12Align(pMenu->x + 2, ty+2,100,FONT_LEFT,pMenu->itemName[tempCurrentItem]);
	if(pMenu->Itemflag[tempCurrentItem] != 0){
		GuiFont12Align(pMenu->x + MENUITEM_WIDE-8, ty+2,10,FONT_LEFT,">");
	}
	GuiExchangeColor();
	pMenu->currentItem = tempCurrentItem;
}

/**
  *@brief 获取菜单管理当前菜单号
  *@param  None
  *@retval 菜单号
  */
uint8_t GetMenuManagepMenu(void)
{
	return menuManage.pMenu;
}

/**
  *@brief top menu current item contol
  *@param  pWindow window handle
  *@param  flag 0 current item -- ,1 current item ++
  *@retval None
  */
void userGUIMenuIremModfiy(uint8_t flag)
{
	if(menuManage.pMenu > 0){
		GUIMenuCurrentItemModfiy(\
			menuManage.userMenu[menuManage.pMenu - 1],flag);
	}
}

/**
  *@brief menu redraw (Used to return to the upper menu display.)
  *@param  None
  *@retval None
  */
void userGUIMenuRedraw(void)
{
	int8_t fcount = 0;
	int8_t x;
	if(menuManage.pMenu < 1){
		return;
	}
	x = menuManage.userMenu[menuManage.pMenu - 1]->x;
	while(1){
		if(x < MENU_BEGINWIDE){
			break;
		}
		x -= MENUITEM_WIDE;
		fcount ++;
	}
	while(fcount){
		if(menuManage.pMenu >= fcount){
			GUIMenuDraw(menuManage.userMenu[menuManage.pMenu - fcount]);
		}
		fcount --;
	}
}

/**
  *@brief menu redraw (Used to return to the upper menu display.)
  *@param  None
  *@retval None
  */
void userGUIMenuDisplay(void)
{
	if(menuManage.pMenu > 0){
		menuManage.userMenu[menuManage.pMenu - 1]->MenuFunction();
	}
}

/**
  *@brief Register the menu to display. 
  *@param  pMenu menu handle
  *@retval None
  */
void userGUIMenuAdd(MENU *pMenu)
{
	GUIMenuDraw(pMenu);
	menuManage.pMenu ++;
	menuManage.userMenu[menuManage.pMenu - 1] = pMenu;
}

/**
  *@brief hide top window
  *@param  None
  *@retval None
  */
void userGUIMenuHide(void)
{
	if(menuManage.pMenu < 1){
		return;
	}
	GUIMenuHide(menuManage.userMenu[menuManage.pMenu - 1]);
	menuManage.userMenu[menuManage.pMenu - 1] = NULL;
	menuManage.pMenu --;
}

/**
  *@brief 菜单管理初始化
  *@param  None
  *@retval None
  */
void MenuManageInit(void)
{
	menuManage.pMenu = 0;
}

/* END */

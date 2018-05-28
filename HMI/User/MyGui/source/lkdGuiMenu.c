/**
  * @file   gwGuiMenu.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/04/02
  * @brief  gwGui菜单文件
  */
#include "gwGuiMenu.h"
#include "gwGuiBasic.h"
#include "gwGuiFont.h"

/**
  *@brief 画基础菜单
  *@param  info 画基础菜单参数
  *@param  node 父节点
  *@retval None
  */
void GuiDrawMenu(MenuInfo *info, MenuItem *node)
{
	uint16_t getBeginNode = 0;
	uint8_t forecolor ,backcolor ;
	MenuItem *ptempNode;
	uint16_t tempX,tempY,tempWide,tempItem;
	forecolor = GuiGetForecolor();
	backcolor = GuiGetbackcolor();
	tempX = info->beginX + 1;
	tempY = info->beginY;
	tempWide = info->endX - info->beginX - 1;
	ptempNode = node->firstChild;
	tempItem = info->beginItem;
	
	while(1){//获取开始绘制节点
		if(getBeginNode == tempItem){
			break;
		}
		ptempNode = ptempNode->nextSibling;
		getBeginNode ++;
	}
	GuiFillRect(info->beginX, info->beginY, info->endX,info->endY,backcolor);
	while(1){
		if(ptempNode == NULL || tempY + info->hight > info->endY){
			break;
		}
		if(tempItem == info->currentItem){
			GuiFillRect(tempX, tempY + 1, info->endX - 1,tempY + info->hight,forecolor);
			GuiFont12Align(tempX, tempY + 2, tempWide, COL_MID_DISPLAY, ptempNode->thisInfo.name);
		}
		else{
			GuiFont12Align(tempX, tempY + 2, tempWide, COL_MID_DISPLAY, ptempNode->thisInfo.name);
		}
		GuiHLine(tempX, tempY + info->hight, info->endX - 1, forecolor);
		ptempNode = ptempNode->nextSibling;
		tempItem ++;
		tempY = tempY + info->hight;
	}
}

/**
  *@brief 添加子节点菜单
  *@param  hd 菜单句柄
  *@param  pMenu 要画的节点
  *@retval 0 成功 1 失败
  */
//uint8_t GuiMenuNodeAdd(MenuHandle *hd, MenuItem *pMenu)
//{
//	if(hd->MenuStack.pMenu >= MENU_MAXDEEP){
//		return 1;/* 达到最大堆栈深度，返回失败 */
//	}
//	hd->menuStack.beginItem[hd->menuStack.pMenu - 1] = hd->drawinfo.beginItem;
//	hd->menuStack.currentItem[hd->menuStack.pMenu - 1] = hd->drawinfo.currentItem;
//	hd->menuStack.userMenu[MenuStack.pMenu] = pMenu;
//	hd->MenuStack.pMenu ++;
//	
//	return 0;
//}

///**
//  *@brief 销毁最顶层子节点菜单
//  *@param  hd 菜单句柄
//  *@param  pMenu 要画的节点
//  *@retval 0 成功 1 失败
//  */
//uint8_t GuiMenuTopNodeHide(MenuHandle *hd, MenuItem *pMenu)
//{
//	if(hd->MenuStack.pMenu <= 0){
//		return 1;/* 没有可销毁的，返回失败 */
//	}
//	hd->menuStack.userMenu[MenuStack.pMenu - 1] = NULL;
//	hd->MenuStack.pMenu --;
//	
//	return 0;
//}

//void GuiMenuItem

/**
  * @file   lkdGuiWindow.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/15
  * @brief   lkdGui窗口管理
  */

#include "lkdGuiWindow.h"
#include "gui_101_cmd.h"

/* 窗口管理 */
static struct WindowManage winManage;

/**
  *@brief draw window
  *@param  pWindow window handle
  *@retval None
  */
void GUIWindowDraw(WINDOW *pWindow)
{
	if( pWindow->x + pWindow->wide > GUI_LCM_XMAX ||\
			pWindow->y + pWindow->hight> GUI_LCM_YMAX || \
	    pWindow->x < 0 || pWindow->y < 0){
		   return;
	}
	GuiFillRect(pWindow->x, pWindow->y, \
		pWindow->x + pWindow->wide - 1,\
		pWindow->y + pWindow->hight - 1, backcolor);
	GuiRect(pWindow->x, pWindow->y,pWindow->x + pWindow->wide - 1,\
		pWindow->y + pWindow->hight - 1, forecolor);
	if(pWindow->title != NULL){
		GuiHLine(pWindow->x, pWindow->y + 15,pWindow->x + pWindow->wide - 1, forecolor);
		GuiBiasLine(pWindow->x, pWindow->y, pWindow->x + 15, pWindow->y + 15, forecolor);
		GuiBiasLine(pWindow->x + 15, pWindow->y, pWindow->x, pWindow->y + 15, forecolor);
		GuiRLine(pWindow->x + 15, pWindow->y, pWindow->y + 15, forecolor);
		GuiFont12Align(pWindow->x + 18, pWindow->y +2,130,FONT_LEFT,pWindow->title);
	}
}

/**
  *@brief hide  window
  *@param  pWindow window handle
  *@retval None
  */
void GUIWindowHide(WINDOW *pWindow)
{
	if(pWindow != NULL){
		GuiFillRect(pWindow->x, pWindow->y, pWindow->x + pWindow->wide - 1,\
			pWindow->y + pWindow->hight - 1, backcolor);
	}
}

/**
  *@brief hide top window
  *@param  None
  *@retval None
  */
void userGUITopWindowHide(void)
{
	if(winManage.pWin <= 0){
		return;
	}
	GUIWindowHide(winManage.userWindow[winManage.pWin - 1]);
	winManage.userWindow[winManage.pWin - 1] = NULL;
	winManage.pWin --;
}

/**
  *@brief Register the window to display. 
  *@param  pWindow window handle
  *@retval None
  */
void userGUIWindowAdd(WINDOW *pWindow)
{
	GUIWindowDraw(pWindow);
	winManage.pWin ++;
	winManage.userWindow[winManage.pWin - 1] = pWindow;
}
/**
  *@brief top window display (execute top window's function)
  *@param  None
  *@retval None
  */
void userGUITopWindowDisplay(void)
{
	if(winManage.pWin < 1){
		return;
	}
	winManage.userWindow[winManage.pWin - 1]->WindowFunction();
}
/**
  *@brief window redraw (Used to return to the upper window.)
  *@param  None
  *@retval None
  */
void userGUITopWindowRedraw(void)
{
	if(winManage.pWin < 1){
		return;
	}
	GUIWindowDraw(winManage.userWindow[winManage.pWin - 1]);
}

/**
  *@brief  窗口管理初始化
  *@param  None
  *@retval None
  */
void WinManageInit(void)
{
	winManage.pWin = 0;
}

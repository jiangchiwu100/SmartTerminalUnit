
#ifndef __LKDGUIWINDOW_H
#define	__LKDGUIWINDOW_H

#include "stdint.h"

#undef  NULL
#define NULL 0

/* 窗口 */
struct WINDOW_
{
	int16_t x;							// 起始坐标
	int16_t y;							// 起始坐标
	int16_t wide;						// 宽度
	int16_t hight;						// 高度
	struct WINDOW_  *fatherMenu;			// 所属窗口
	uint8_t   *title;		// 定义标题栏指针(NULL为不显示)
	void(*WindowFunction)(void);	// 窗口对应的服务程序
} ;
typedef  struct WINDOW_ WINDOW;


/* 窗口管理 */
struct WindowManage{
	uint8_t pWin;
	WINDOW *userWindow[12];	
};

void userGUITopWindowHide(void);
void userGUIWindowAdd(WINDOW *pWindow);
void userGUITopWindowDisplay(void);
void userGUITopWindowRedraw(void);
void WinManageInit(void);

#endif /* __LKDGUIMENU_H */

/* END */

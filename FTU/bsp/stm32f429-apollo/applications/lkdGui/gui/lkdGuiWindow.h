
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

#define LIST_CONTENT_ROWHIGHT 14//内容行高度
#define MAXLIST_COL 4
#define LIST_USESCROLL_H 	0x01
#define LIST_USESCROLL_V 	0x02
#define LIST_USEBORDER_H 	0x04
struct ColSet_{
	uint8_t *colWide;
	uint8_t *colFlag;	
};
union DrawFlag_{
	uint8_t all;
	struct{
		uint8_t useHScroll:1;//水平滚动条
		uint8_t useVScroll:1;//垂直滚动条
		uint8_t useBorder:1;//使用边框
		uint8_t useHighLight:1;//当前行高亮
	}bit;
};

struct LIST_{
	int16_t x;
	int16_t y;
	uint16_t wide;
	uint16_t hight;
	uint8_t row;
	uint8_t col;//列
	uint8_t drawRow;
	uint8_t currentRow;
	uint8_t flag;
	uint8_t **content;
	struct ColSet_ *colSet;
};
typedef struct ColSet_ ListColSet;
typedef struct LIST_ LIST;


void userGUITopWindowHide(void);
void userGUIWindowAdd(WINDOW *pWindow);
void userGUITopWindowDisplay(void);
void userGUITopWindowRedraw(void);
void WinManageInit(void);


void DrawList(LIST *plist);

#endif /* __LKDGUIMENU_H */

/* END */

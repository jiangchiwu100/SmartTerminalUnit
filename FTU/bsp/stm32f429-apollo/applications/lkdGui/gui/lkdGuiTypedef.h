
#ifndef __LKDGUITYPEDEF_H
#define	__LKDGUITYPEDEF_H


/* 窗口定义 */
struct WINDOW_
{
	int16_t x;							// 起始坐标
	int16_t y;							// 起始坐标
	int16_t wide;						// 宽度
	int16_t hight;						// 高度
	struct WINDOW_  *fatherMenu;			// 所属窗口
	uint8_t   *title;		// 定义标题栏指针(NULL为不显示)
	void(*WindowFunction)(void);	// 窗口对应的服务程序
};
typedef  struct WINDOW_ WINDOW;

/* 进度条定义 */
typedef struct{
	int16_t x;
	int16_t y;
	uint16_t hight;
	uint16_t max;
	uint16_t lump;
}SCROLL;

#endif /* __LKDGUITYPEDEF_H */

/* END */
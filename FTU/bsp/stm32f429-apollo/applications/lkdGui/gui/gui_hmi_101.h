/**
  * @file    hmi101.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/10
  * @brief   
  */

#ifndef __HMI101_H
#define	__HMI101_H

#include "stdint.h"

#define HMIREVBUFF_MAX 1024
struct Hmi101Rev{
	uint8_t revBuff[HMIREVBUFF_MAX];
	uint16_t pIn;
	uint16_t pout;
};

/* Hmi101帧命令 */
enum Hmi101Frame{
	H101CMD_LEN_L,
	H101CMD_LEN_H,
	H101CMD_CMDNUM,
	H101CMD_CMD,
	/* 后面跟所有具体命令 */
};

/* 开入命令 */
enum HmiInputCmd_001{
	CMD001_LEN,		/* 命令长度 */
	CMD001_CMD,		/* 命令标识 */
	CMD001_TYPE,	/* 0 非连续 1 连续 */
	CMD001_NUM,		/* 开入数量 */
	CMD001_NUMBER,/* 开入号 */
	CMD001_VALUE,	/* 对应值 */
	/* 如果是连续 全部为值 */
	/* 如果非连续  CMD100_NUMBER0 CMD100_VALUE0 CMD100_NUMBER1 CMD100_VALUE1 ... */
};
enum Cmd001Type{
	C001TYPE_DISCRETE,	/* 非连续 */
	C001TYPE_CONTINUOUS,/* 连续 */
};
/* 开出命令 */
enum HmiInputCmd_002{
	CMD002_LEN,		/* 命令长度 */
	CMD002_CMD,		/* 命令标识 */
	CMD002_TYPE,	/* 0 非连续 1 连续 */
	CMD002_NUM,		/* 开出数量 */
	CMD002_NUMBER,/* 开出号 */
	CMD002_VALUE,	/* 对应值 */
	/* 如果是连续 全部为值 */
	/* 如果非连续  CMD002_NUMBER0 CMD002_VALUE0 CMD002_NUMBER1 CMD002_VALUE1 ... */
};
enum Cmd002Type{
	C002TYPE_DISCRETE,	/* 非连续 */
	C002TYPE_CONTINUOUS,/* 连续 */
};

/* 模拟量命令 */
enum HmiAnalogCmd_003{
	CMD003_LEN,		/* 命令长度 */
	CMD003_CMD,		/* 命令标识 */
	CMD003_TYPE,	/* 0 非连续 1 连续 */
	CMD003_NUM,		/* 模拟量数量 */
	CMD003_NUMBER,/* 模拟量号 */
	CMD003_VALUE_LL,	/* 对应值 */
	CMD003_VALUE_LH,
	CMD003_VALUE_HL,
	CMD003_VALUE_HH,
	/* 如果是连续 全部为值 */
	/* 如果非连续  CMD002_NUMBER0 CMD002_VALUE0 CMD002_NUMBER1 CMD002_VALUE1 ... */
};
enum Cmd003Type{
	C003TYPE_DISCRETE,	/* 非连续 */
	C003TYPE_CONTINUOUS,/* 连续 */
};

/* 画线命令 */
enum HmiGuiCmd_100{
	CMD100_LEN,		/* 命令长度 */
	CMD100_CMD,		/* 命令标识 */
	CMD100_TYPE,	/* 画线类型 0水平线 1垂直线 2 斜线 */
	CMD100_BX,		/* 起始坐标x */
	CMD100_BY,		/* 起始坐标y */
	CMD100_EX,		/* 结束坐标x */
	CMD100_EY,		/* 结束坐标y */
	CMD100_POINT,	/* 点线间隔 */
	CMD100_COLOUR,/* 颜色 */
};
enum Cmd100Type{
	C100TYPE_HLINE,	/* 水平线 */
	C100TYPE_RLINE,	/* 垂直线 */
	C100TYPE_BLINE,	/* 斜线 */
};
/* 画矩形命令 */
enum HmiGuiCmd_101{
	CMD101_LEN,		/* 命令长度 */
	CMD101_CMD,		/* 命令标识 */
	CMD101_TYPE,	/* 矩形类型 0 矩形 1 填充矩形 */
	CMD101_BX,		/* 起始坐标x */
	CMD101_BY,		/* 起始坐标y */
	CMD101_EX,		/* 结束坐标x */
	CMD101_EY,		/* 结束坐标y */
	CMD101_COLOUR,/* 颜色 */
};
enum Cmd101Type{
	C101TYPE_RECT,			/* 矩形 */
	C101TYPE_FILLRECT,	/* 填充矩形 */
};
/* 画窗口命令 */
enum HmiGuiCmd_102{
	CMD102_LEN,		/* 命令长度 */
	CMD102_CMD,		/* 命令标识 */
	CMD102_TYPE,	/* 窗口类型 0 自定义窗口 1 密码窗口 2 消息窗口  */
	CMD102_BX,		/* 起始坐标x */
	CMD102_BY,		/* 起始坐标y */
	CMD102_EX,		/* 结束坐标x */
	CMD102_EY,		/* 结束坐标y */
	CMD102_CONTENT,/* 窗口名称->字符串,\0结束 */
};
enum Cmd102Type{
	C102TYPE_DEFAULT,		/* 自定义窗口 */
	C102TYPE_PASSWORD,	/* 密码窗口 */
	C102TYPE_MESSAGE,		/* 消息窗口 */
};
/* 刷新LCD命令 */
enum HmiGuiCmd_103{
	CMD103_LEN,		/* 命令长度 */
	CMD103_CMD,		/* 命令标识 */
};

/* 控制LCD命令 */
enum HmiGuiCmd_104{
	CMD104_LEN,		/* 命令长度 */
	CMD104_CMD,		/* 命令标识 */
	CMD104_TYPE,	/* 控制类型 0 关背光 1 开背光 2 关显示 3 开显示 */
};
enum Cmd104Type{
	C104TYPE_OFFBLA,		/* 关背光 */
	C104TYPE_ONBLA,			/* 开背光 */
	C104TYPE_OFFSCREEN,	/* 关显示 */
	C104TYPE_ONSCREEN,	/* 开显示 */
};
/* 画进度条命令 */
enum HmiGuiCmd_105{
	CMD105_LEN,		/* 命令长度 */
	CMD105_CMD,		/* 命令标识 */
	CMD105_TYPE,	/* 进度条类型 0水平进度条 1垂直进度条 */
	CMD105_BX,		/* 起始坐标x */
	CMD105_BY,		/* 起始坐标y */
	CMD105_SLEN,	/* 进度条长度 */
	CMD105_SMAX,	/* 进度条最大值 */
	CMD105_SLUMP,	/* 当前值 */
};
enum Cmd105Type{
	C105TYPE_HSCROLL,	/* 水平进度条 */
	C105TYPE_RSCROLL,	/* 垂直进度条 */
};
/* 画按钮命令 */
enum HmiGuiCmd_106{
	CMD106_LEN,		/* 命令长度 */
	CMD106_CMD,		/* 命令标识 */
	CMD106_TYPE,	/* 按钮类型  0 自定义 1 确认 2 取消 */
	CMD106_BX,		/* 起始坐标x */
	CMD106_BY,		/* 起始坐标y */
	CMD106_FlAG,	/* 按钮状态  0 非选中 1 选中 */
	CMD106_CONTENT,/* 按钮名称->字符串,\0结束 */
};
enum Cmd106Type{
	C106TYPE_DEFAULT,	/* 自定义 */
	C106TYPE_OK,			/* 确认 */
	C106TYPE_ESC,			/* 取消 */
};
enum Cmd106Flag{
	C106FLAG_NOTHIS,	/* 非选中 */
	C106FLAG_ISTHIS,	/* 选中 */
};
/* 加载图片命令 */
enum HmiGuiCmd_107{
	CMD107_LEN,		/* 命令长度 */
	CMD107_CMD,		/* 命令标识 */
	CMD107_TYPE,	/* 图片类型  0 自定义 1 双杰图标 2 加载中图片 */
	CMD107_BX,		/* 起始坐标x */
	CMD107_BY,		/* 起始坐标y */
	CMD107_EX,		/* 结束坐标x */
	CMD107_EY,		/* 结束坐标y */
	CMD107_MXLEN,	/* 宽像素大小 */
	CMD107_MYLEN,	/* 高像素大小 */
	CMD107_FlAG,	/* 图片标志  0 正常显示 1 反显 */
	CMD107_CONTENT,/* 图片内容 */
};
enum Cmd107Type{
	C107TYPE_DEFAULT,/* 自定义 */
	C107TYPE_SOJO,/* SOJO */
	C107TYPE_WAIT,/* 加载中 */
};
enum Cmd107Flag{
	C107FLAG_NORMAL,	/* 正常显示 */
	C107FLAG_INVERSE,	/* 反显 */
};

/* 写字命令 */
enum HmiGuiCmd_108{
	CMD108_LEN,		/* 命令长度 */
	CMD108_CMD,		/* 命令标识 */
	CMD108_TYPE,	/* 字体类型 0 默认字体 */
	CMD108_BX,		/* 起始坐标x */
	CMD108_BY,		/* 起始坐标y */
	CMD108_WIDE,	/* 宽度 */
	CMD108_HIGH,	/* 高度 */
	CMD108_FlAG,	/* 对齐标志  0 左对齐 1 居中 2 右对齐 3 可换行写 */
	CMD108_CONTENT,/* 字体内容 GB2312 \0 end*/
};
enum Cmd108Type{
	C108TYPE_DEFAULT,/* 默认字体 */
};
enum Cmd108Flag{
	C108FLAG_LEFT,			/* 左对齐 */
	C108FLAG_MID,				/* 居中 */
	C108FLAG_RIGHT,			/* 右对齐 */
	C108FLAG_FEEDLINE,	/* 可换行写 */
};

/* 设置颜色命令 */
enum HmiGuiCmd_109{
	CMD109_LEN,		/* 命令长度 */
	CMD109_CMD,		/* 命令标识 */
	CMD109_TYPE,	/* 设置类型 0 前景色 1背景色 2 交换颜色 */
	CMD109_COLOUR,/* 颜色 */
};
enum Cmd109Type{
	C109TYPE_FORE,		/* 前景色 */
	C109TYPE_BACK,		/* 背景色 */
	C109TYPE_EXCHANGE,/* 交换颜色 */
};

enum HmiGuiCmdTab{
	HmiCmd002 = 2,
	HmiCmd003 = 3,
	GuiCmd100 = 100,
	GuiCmd101,
	GuiCmd102,
	GuiCmd103,
	GuiCmd104,
	GuiCmd105,
	GuiCmd106,
	GuiCmd107,
	GuiCmd108,
	GuiCmd109,
	GuiCmd110,
};


extern uint32_t GetTimer1Tick(void);
extern uint32_t GetTimer1IntervalTick(uint32_t beginTick);

#endif /* END __HMI101_H */

/**
  * @file   Gui101Cmd.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/11
  * @brief  Gui101CmdMaster
  */

#include "gui_101_cmd.h"
#include "gui_hmi_101.h"
#include "string.h"
#include "stdio.h"
#include "lkdGuiMenu.h"
#include "lkdGuiWindow.h"
#include <rtthread.h>


/* cmd101发送事件 */
struct rt_event Cmd101SendEvent;


Gui101CmdControl cmd101;

/* 定义前景色/背景色 */
lkdColour forecolor = 1,backcolor;
/* 定义按键状态 */
enum KeyStatus guikeyIs;

/**
  *@brief  结束101压栈
  *@param  None
  *@retval None
  */
void EndCmd101Down(void)
{
	union{
		uint16_t len16;
		uint8_t len8[2];
	}frameLen;	
	frameLen.len16 = cmd101.pIn;
	cmd101.state = 2;
	cmd101.packBuff[0] = frameLen.len8[0];
	cmd101.packBuff[1] = frameLen.len8[1];
	cmd101.packBuff[2] = cmd101.cmdNum;
	DLT634_HMI_SLAVE_C_REPLY(0,cmd101.packBuff);
}

/**
  *@brief cmd101压栈控制函数
  *@param  x,y 坐标
  *@param  color 颜色
  *@retval None
  */
static void Cmd101DownControl(uint16_t len)
{
	if(cmd101.state == 0){//栈空,可以压栈
		cmd101.cmdNum = 0;
		cmd101.pIn = 3;
		cmd101.state = 1;
	}
	if(cmd101.state == 2){//正在发送,阻塞等待
		rt_event_recv(&Cmd101SendEvent, (1 << 0), RT_EVENT_FLAG_OR |\
			RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);
		cmd101.cmdNum = 0;
		cmd101.pIn = 3;
		cmd101.state = 1;
	}
	if(cmd101.state == 1){//压栈中
		if(cmd101.pIn + len >= PACKBUFFMAX - 20){
			EndCmd101Down();
			rt_event_recv(&Cmd101SendEvent, (1 << 0), RT_EVENT_FLAG_OR |\
				RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);
			cmd101.cmdNum = 0;
			cmd101.pIn = 3;
			cmd101.state = 1;
		}
	}
}

/**
  *@brief cmd101发送完成函数
  *@param  x,y 坐标
  *@param  color 颜色
  *@retval None
  */
void Cmd101SendFinish(void)
{
	//发送完成事件
	rt_event_send(&Cmd101SendEvent, (1 << 0));
}


void BeginCmd101Down(void)
{
	if(cmd101.state != 0){
	}
	else{
		cmd101.pIn = 3;
	}
	cmd101.cmdNum = 0;
	cmd101.pIn = 3;
}

/**
  *@brief 画斜线
  *@param  x,y 坐标
  *@param  color 颜色
  *@retval None
  */
void GuiBiasLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord x1, lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_BLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = 1;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}
/**
  *@brief 画水平点线
  *@param  x0,y0 起始坐标
  *@param  x1 结束坐标
  *@param  interval 每隔N点画一个点
  *@param  color 颜色
  *@retval None
  */
void GuiHPointLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord x1, uint8_t interval, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_HLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = interval;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}

/**
  *@brief 画垂直点线
  *@param  x0,y0 起始坐标
  *@param  y1 结束坐标
  *@param  interval 每隔N点画一个点
  *@param  color 颜色
  *@retval None
  */
void  GuiRPointLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord y1,uint8_t interval, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_RLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = y1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = interval;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}
/**
  *@brief 画水平线
  *@param  x0,y0 起始坐标
  *@param  x1结束坐标
  *@param  color 颜色
  *@retval None
  */
void GuiHLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_HLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = 1;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}

/**
  *@brief 画垂直线
  *@param  x0,y0 起始坐标
  *@param  y1 结束坐标
  *@param  color 颜色
  *@retval None
  */
void  GuiRLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(9);
	cmd101.packBuff[cmd101.pIn + CMD100_CMD] = 100;
	cmd101.packBuff[cmd101.pIn + CMD100_TYPE] = C100TYPE_RLINE;
	cmd101.packBuff[cmd101.pIn + CMD100_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD100_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD100_EX] = 0;
	cmd101.packBuff[cmd101.pIn + CMD100_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD100_POINT] = 1;
	cmd101.packBuff[cmd101.pIn + CMD100_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD100_LEN] = 9;
	cmd101.pIn += 9;
	cmd101.cmdNum += 1;
}

/**
  *@brief 画矩形
  *@param  x0,y0 起始坐标
  *@param  x1,y1 结束坐标
  *@param  color 颜色
  *@retval None
  */
void GuiRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(8);
	cmd101.packBuff[cmd101.pIn + CMD101_CMD] = 101;
	cmd101.packBuff[cmd101.pIn + CMD101_TYPE] = C101TYPE_RECT;
	cmd101.packBuff[cmd101.pIn + CMD101_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD101_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD101_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD101_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD101_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD101_LEN] = 8;
	cmd101.pIn += 8;
	cmd101.cmdNum += 1;
}

/**
  *@brief 画填充矩形
  *@param  x0,y0 起始坐标
  *@param  x1,y1 结束坐标
  *@param  color 颜色
  *@retval None
  */
void GuiFillRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color)
{
	Cmd101DownControl(8);
	cmd101.packBuff[cmd101.pIn + CMD101_CMD] = 101;
	cmd101.packBuff[cmd101.pIn + CMD101_TYPE] = C101TYPE_FILLRECT;
	cmd101.packBuff[cmd101.pIn + CMD101_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD101_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD101_EX] = x1;
	cmd101.packBuff[cmd101.pIn + CMD101_EY] = y1;
	cmd101.packBuff[cmd101.pIn + CMD101_COLOUR] = color;
	cmd101.packBuff[cmd101.pIn + CMD101_LEN] = 8;
	cmd101.pIn += 8;
	cmd101.cmdNum += 1;
}

/**
  *@brief 12x12字体函数
  *@param  x,y 起始坐标
  *@param  str 字符串
  *@retval None
  */
void GuiFont12(lkdCoord x, lkdCoord y, uint8_t *str)
{
	uint8_t tstrlen;
	
	tstrlen = strlen((char *)str) + 1;
	Cmd101DownControl(tstrlen + 7);
	cmd101.packBuff[cmd101.pIn + CMD108_CMD] = 108;
	cmd101.packBuff[cmd101.pIn + CMD108_TYPE] = C108TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD108_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD108_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD108_WIDE] = 160;
	cmd101.packBuff[cmd101.pIn + CMD108_HIGH] = 160;
	cmd101.packBuff[cmd101.pIn + CMD108_FlAG] = C108FLAG_FEEDLINE;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD108_CONTENT],str,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD108_LEN] = tstrlen + 7;
	cmd101.pIn += tstrlen + 7;
	cmd101.cmdNum += 1;
}

/**
  *@brief 12x12字体对齐显示函数
  *@param  x,y 起始坐标
  *@param  wide 宽度
  *@param  flag 对齐标志
  *@param  str 字符串
  *@retval None
  */
void GuiFont12Align(lkdCoord x, lkdCoord y,uint16_t wide, FontFlag flag,uint8_t *str)
{
	uint8_t tstrlen;
	tstrlen = strlen((char *)str) + 1;
	Cmd101DownControl(tstrlen + 7);
	cmd101.packBuff[cmd101.pIn + CMD108_CMD] = 108;
	cmd101.packBuff[cmd101.pIn + CMD108_TYPE] = C108TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD108_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD108_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD108_WIDE] = wide;
	cmd101.packBuff[cmd101.pIn + CMD108_HIGH] = 14;
	cmd101.packBuff[cmd101.pIn + CMD108_FlAG] = flag;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD108_CONTENT],str,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD108_LEN] = tstrlen + CMD108_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD108_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief 垂直滚动条
  *@param  pScroll Scroll handle
  *@retval None
  */
void GuiVScroll(SCROLL *pScroll)
{
	Cmd101DownControl(8);
	cmd101.packBuff[cmd101.pIn + CMD105_CMD] = 105;
	cmd101.packBuff[cmd101.pIn + CMD105_TYPE] = C105TYPE_RSCROLL;
	cmd101.packBuff[cmd101.pIn + CMD105_BX] = pScroll->x;
	cmd101.packBuff[cmd101.pIn + CMD105_BY] = pScroll->y;
	cmd101.packBuff[cmd101.pIn + CMD105_SLEN] = pScroll->hight;
	cmd101.packBuff[cmd101.pIn + CMD105_SMAX] = pScroll->max;
	cmd101.packBuff[cmd101.pIn + CMD105_SLUMP] = pScroll->lump;
	cmd101.packBuff[cmd101.pIn + CMD105_LEN] = 8;
	cmd101.pIn += 8;
	cmd101.cmdNum += 1;
}

/**
  *@brief 按键
  *@param  x，y 起始坐标
  *@param  keyName 按键名称
  *@param  flag 0非选中 1选中
  *@retval None
  */
void GuiButton(int16_t x,int16_t y,uint8_t *keyName,uint8_t flag)
{
	uint8_t tstrlen;
	tstrlen = strlen((char *)keyName) + 1;
	Cmd101DownControl(tstrlen + 7);
	cmd101.packBuff[cmd101.pIn + CMD106_CMD] = 106;
	cmd101.packBuff[cmd101.pIn + CMD106_TYPE] = C106TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD106_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD106_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD106_FlAG] = flag;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD106_CONTENT],keyName,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD106_LEN] = tstrlen + CMD106_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD106_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief 关显示
  *@param  None
  *@retval None
  */
void CloseLcdDisplay(void)
{
	Cmd101DownControl(3);
	cmd101.packBuff[cmd101.pIn + CMD104_CMD] = 104;
	cmd101.packBuff[cmd101.pIn + CMD104_TYPE] = C104TYPE_OFFSCREEN;
	cmd101.packBuff[cmd101.pIn + CMD104_LEN] = 3;
	cmd101.pIn += 3;
	cmd101.cmdNum += 1;
}

/**
  *@brief 开显示
  *@param  None
  *@retval None
  */
void OpenLcdDisplay(void)
{
	Cmd101DownControl(3);
	cmd101.packBuff[cmd101.pIn + CMD104_CMD] = 104;
	cmd101.packBuff[cmd101.pIn + CMD104_TYPE] = C104TYPE_ONSCREEN;
	cmd101.packBuff[cmd101.pIn + CMD104_LEN] = 3;
	cmd101.pIn += 3;
	cmd101.cmdNum += 1;
}

/**
  *@brief 画指定范围的位图
  *@param  bitMap 位图数组
  *@param  x0,y0 起始坐标
  *@param  endx,endy 终点坐标
  *@param  flag 0 正常显示 1反显
  *@retval None
  */
void GuiAreaBitMap(lkdBitmap *bitMap,\
	lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag)
{
	uint16_t tstrlen;
	
	if(bitMap->number < 1){
		if((bitMap->wide % 8) == 0){
			tstrlen = bitMap->high * (bitMap->wide / 8);
		}
		else{
			tstrlen = bitMap->high * (bitMap->wide / 8 + 1);
		}
		Cmd101DownControl(tstrlen + CMD107_CONTENT);
		memcpy(&cmd101.packBuff[cmd101.pIn + CMD107_CONTENT],bitMap->bitmap,tstrlen);
	}
	else{
		tstrlen = 1;
		Cmd101DownControl(tstrlen + CMD107_CONTENT);
		cmd101.packBuff[cmd101.pIn + CMD107_CONTENT] = '\0';
	}
	cmd101.packBuff[cmd101.pIn + CMD107_CMD] = 107;
	cmd101.packBuff[cmd101.pIn + CMD107_TYPE] = bitMap->number;
	cmd101.packBuff[cmd101.pIn + CMD107_BX] = x0;
	cmd101.packBuff[cmd101.pIn + CMD107_BY] = y0;
	cmd101.packBuff[cmd101.pIn + CMD107_EX] = endx;
	cmd101.packBuff[cmd101.pIn + CMD107_EY] = endy;
	cmd101.packBuff[cmd101.pIn + CMD107_MXLEN] = bitMap->wide;
	cmd101.packBuff[cmd101.pIn + CMD107_MYLEN] = bitMap->high;
	cmd101.packBuff[cmd101.pIn + CMD107_FlAG] = flag;
	
	cmd101.packBuff[cmd101.pIn + CMD107_LEN] = tstrlen + CMD107_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD107_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief 更新显示
  *@param  None
  *@retval None
  */
void GuiUpdateDisplayAll(void)
{
	Cmd101DownControl(2);
	cmd101.packBuff[cmd101.pIn + CMD103_CMD] = 103;
	cmd101.packBuff[cmd101.pIn + CMD103_LEN] = 2;
	cmd101.pIn += 2;
	cmd101.cmdNum += 1;
	EndCmd101Down();
}

/**
  *@brief 获取前景色
  *@param  None
  *@retval 返回前景色
  */
lkdColour GuiGetForecolor(void)
{
	return forecolor;
}

/**
  *@brief 设置前景色
  *@param  colour 前景色
  *@retval None
  */
void GuiSetForecolor(lkdColour colour)
{
	forecolor = colour;
	cmd101.packBuff[cmd101.pIn + CMD109_CMD] = 109;
	cmd101.packBuff[cmd101.pIn + CMD109_TYPE] = C109TYPE_FORE;
	cmd101.packBuff[cmd101.pIn + CMD109_COLOUR] = forecolor;
	cmd101.packBuff[cmd101.pIn + CMD109_LEN] = 4;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD109_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief 获取背景色
  *@param  None
  *@retval 返回背景色
  */
lkdColour GuiGetbackcolor(void)
{
	return backcolor;
}
/**
  *@brief 设置背景色
  *@param  colour 背景色
  *@retval None
  */
void GuiSetbackcolor(lkdColour colour)
{
	backcolor = colour;
	cmd101.packBuff[cmd101.pIn + CMD109_CMD] = 109;
	cmd101.packBuff[cmd101.pIn + CMD109_TYPE] = C109TYPE_BACK;
	cmd101.packBuff[cmd101.pIn + CMD109_COLOUR] = backcolor;
	cmd101.packBuff[cmd101.pIn + CMD109_LEN] = 4;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD109_LEN];
	cmd101.cmdNum += 1;
}

/**
  *@brief 交换前景色和背景色
  *@param  None
  *@retval None
  */
void GuiExchangeColor(void)
{
	lkdColour tColour;
	tColour = backcolor;
	backcolor = forecolor;
	forecolor = tColour;
	cmd101.packBuff[cmd101.pIn + CMD109_CMD] = 109;
	cmd101.packBuff[cmd101.pIn + CMD109_TYPE] = C109TYPE_EXCHANGE;
	cmd101.packBuff[cmd101.pIn + CMD109_COLOUR] = forecolor;
	cmd101.packBuff[cmd101.pIn + CMD109_LEN] = 4;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD109_LEN];
	cmd101.cmdNum += 1;
}

enum KeyStatus GetKeyStatus(void)
{
	return guikeyIs;
}
void SetKeyStatus(enum KeyStatus keyIs)
{
	guikeyIs = keyIs;
}
void SetKeyIsNoKey(void)
{
	guikeyIs = NoKey;
}


uint8_t *testStr[] = {
	"开关分位0","开关合位1","过流一段2","过流二段3","重合闸闭锁4",
	"开关分位5","开关合位6","过流一段7","过流二段8","重合闸闭锁9",
	"开关分位10","开关合位11","过流一段12","过流二段13","重合闸闭锁14",
	"灯15","SOE16","保护投17","保护退18","掉电19",
	"灯20","SOE21","保护投22","保护退23","掉电24",
};


void TestHmiMain(uint8_t *flag)
{
	uint8_t i;
	static  uint32_t TestTick;
	uint8_t floatStr[16];
	if(*flag == 1){
		BeginCmd101Down();
		GuiFillRect(0, 0, 159, 159, 0);
		GuiFillRect(10, 20, 149, 36, 1);
		GuiFont12Align(12, 22, 130, FONT_MID,"sojo新架构测试例子");
		for(i = 0; i < 6; i++){
			GuiHLine(0, 38 + i * 14, 159, 1);
		}
		GuiRLine(0, 38, 102, 1);
		GuiRLine(52, 38, 102, 1);
		GuiRLine(104, 38, 102, 1);
		GuiRLine(159, 38, 102, 1);
		GuiRect(20, 110, 140,130, 1);
		GuiUpdateDisplayAll();
	}
	if(*flag == 2){
		BeginCmd101Down();
		GuiFillRect(0, 0, 159, 159, 0);
		GuiRect(0, 0, 159, 159, 1);
		GuiHLine(0, 16, 159, 1);
		GuiHLine(0, 18, 159, 1);
		GuiFont12Align(0, 2, 159, FONT_MID,"不刷新窗口");
		for(i = 1; i < 9; i++){
			GuiHPointLine(0, 18 + i * 15, 159, 2, 1);
		}
		for(i = 0; i < 9; i++){
			GuiFont12Align(2, 20 + i * 15, 78, FONT_LEFT,testStr[i]);
		}
		for(i = 0; i < 9; i++){
			GuiFont12Align(82, 20 + i * 15, 77, FONT_LEFT,testStr[i + 9]);
		}
		GuiUpdateDisplayAll();
	}
	if(*flag == 3){
		if(GetTimer1IntervalTick(TestTick) > 1000){
			TestTick = GetTimer1Tick();
			BeginCmd101Down();
			GuiFillRect(0, 0, 159, 159, 0);
			GuiRect(0, 0, 159, 159, 1);
			GuiHLine(0, 16, 159, 1);
			GuiHLine(0, 18, 159, 1);
			GuiFont12Align(0, 2, 159, FONT_MID,"HMI窗口");
			for(i = 1; i < 9; i++){
				GuiHPointLine(0, 18 + i * 15, 159, 2, 1);
			}
			sprintf((char *)floatStr,"%d",(uint8_t)TestTick);
			for(i = 1; i < 9; i++){
				GuiFont12Align(2, 20 + i * 15, 78, FONT_LEFT,floatStr);
			}
			for(i = 1; i < 9; i++){
				GuiFont12Align(82, 20 + i * 15, 77, FONT_LEFT,floatStr);
			}
			GuiUpdateDisplayAll();
		}
	}
	if(*flag == 4){
		BeginCmd101Down();
		//GUIMenuDraw(&mianMenu);
		GuiUpdateDisplayAll();
	}
}

//MENU mianMenu = {
//	0,0,NULL,0,5,{1,1,1,0,0},//x,y,fatherMenu,currentItem,mun,ItemFlag
//	{"信息查询","定值设置","配置设置","命令下发","版本信息",NULL,NULL,NULL,NULL,NULL},
//	NULL/* MenuFunction */
//};
//MENU MenuM1 = {
//	0,0,&mianMenu,0,9,{1,0,0,0,0,0,0,0,1},//x,y,fatherMenu,currentItem,mun
//	{"保护功能","逻辑功能","越限报警","重 过 载","过 负 荷","越压越频","电池设置","自动复归","其他设置",NULL},
//	NULL/* itemFunction */
//};

//void MenuTest(void)
//{
//	static uint8_t menustep,ItemIs,itemflag;
//	static uint32_t menuTick;
//	BeginCmd101Down();
//	if(menustep == 0){
//		BeginCmd101Down();
//		userGUIMenuAdd(&mianMenu);
//		userGUIMenuAdd(&MenuM1);
//		GuiUpdateDisplayAll();
//		menuTick = GetTimer1Tick();
//		menustep = 1;
//	}
//	if(menustep == 1 && GetTimer1IntervalTick(menuTick) > 1000){
//		menuTick = GetTimer1Tick();
//		if(ItemIs < 9 && itemflag == 0){
//			userGUIMenuIremModfiy(1);
//			ItemIs ++;
//			if(ItemIs >= 9){
//				itemflag = 1;
//			}
//		}
//		else{
//			if(ItemIs < 1){
//				itemflag = 0;
//			}
//			ItemIs --;
//			userGUIMenuIremModfiy(0);	
//		}
//		GuiUpdateDisplayAll();
//	}
//}
//WINDOW VersionWin = {0,0,160,160,NULL,"版本信息管理",NULL};
//void WindowTest(void)
//{
//	static uint8_t winstep;
//	static uint32_t winTick;
//	if(winstep == 0){
//		BeginCmd101Down();
//		userGUIWindowAdd(&VersionWin);
//		GuiUpdateDisplayAll();
//		winTick = GetTimer1Tick();
//		winstep = 1;
//	}
//	if(winstep == 1 && GetTimer1IntervalTick(winTick) > 1000){
//		winstep = 0;
//		BeginCmd101Down();
//		userGUITopWindowHide();
//		GuiUpdateDisplayAll();
//	}
//}
void TestMain(void)
{
	static uint8_t flag;
	static uint32_t runTick,HmiTestTick;
	if(flag == 0){
		runTick = GetTimer1Tick();
		flag = 1;
	}
	else if(flag == 1 && GetTimer1IntervalTick(runTick) > 5000){
		runTick = GetTimer1Tick();
		flag = 2;
	}
	else if(flag == 2 && GetTimer1IntervalTick(runTick) > 5000){
		runTick = GetTimer1Tick();
		flag = 3;
	}
	else if(flag == 3 && GetTimer1IntervalTick(runTick) > 10000){
		runTick = GetTimer1Tick();
		flag = 4;
	}
	else if(flag == 4 && GetTimer1IntervalTick(runTick) > 10000){
		runTick = GetTimer1Tick();
		flag = 1;
	}
	if(GetTimer1IntervalTick(HmiTestTick) > 50){
		HmiTestTick = GetTimer1Tick();
		//TestHmiMain(&flag);
		//MenuTest();
		//WindowTest();
	}
	
}


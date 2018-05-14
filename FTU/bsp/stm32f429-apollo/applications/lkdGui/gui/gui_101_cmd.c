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

Gui101CmdControl cmd101;

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

void EndCmd101Down(void)
{
	union{
		uint16_t len16;
		uint8_t len8[2];
	}frameLen;	
	frameLen.len16 = cmd101.pIn;
	cmd101.state = 0;
	cmd101.packBuff[0] = frameLen.len8[0];
	cmd101.packBuff[1] = frameLen.len8[1];
	cmd101.packBuff[2] = cmd101.cmdNum;
	DLT634_HMI_SLAVE_C_REPLY(0,cmd101.packBuff);
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
	cmd101.packBuff[cmd101.pIn + CMD108_CMD] = 108;
	cmd101.packBuff[cmd101.pIn + CMD108_TYPE] = C108TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD108_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD108_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD108_WIDE] = 160;
	cmd101.packBuff[cmd101.pIn + CMD108_HIGH] = 160;
	cmd101.packBuff[cmd101.pIn + CMD108_FlAG] = C108FLAG_FEEDLINE;
	tstrlen = strlen((char *)str) + 1;
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
	cmd101.packBuff[cmd101.pIn + CMD108_CMD] = 108;
	cmd101.packBuff[cmd101.pIn + CMD108_TYPE] = C108TYPE_DEFAULT;
	cmd101.packBuff[cmd101.pIn + CMD108_BX] = x;
	cmd101.packBuff[cmd101.pIn + CMD108_BY] = y;
	cmd101.packBuff[cmd101.pIn + CMD108_WIDE] = wide;
	cmd101.packBuff[cmd101.pIn + CMD108_HIGH] = 14;
	cmd101.packBuff[cmd101.pIn + CMD108_FlAG] = flag;
	tstrlen = strlen((char *)str) + 1;
	memcpy(&cmd101.packBuff[cmd101.pIn + CMD108_CONTENT],str,tstrlen);
	cmd101.packBuff[cmd101.pIn + CMD108_LEN] = tstrlen + CMD108_CONTENT;
	cmd101.pIn += cmd101.packBuff[cmd101.pIn + CMD108_LEN];
	cmd101.cmdNum += 1;
}

void GuiUpdateDisplayAll(void)
{
	cmd101.packBuff[cmd101.pIn + CMD103_CMD] = 103;
	cmd101.packBuff[cmd101.pIn + CMD103_LEN] = 2;
	cmd101.pIn += 2;
	cmd101.cmdNum += 1;
	EndCmd101Down();
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
}

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
		flag = 1;
	}
	if(GetTimer1IntervalTick(HmiTestTick) > 50){
		HmiTestTick = GetTimer1Tick();
		TestHmiMain(&flag);
	}
	
}

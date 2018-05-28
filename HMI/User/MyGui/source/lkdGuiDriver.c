/**
  * @file   lkdGuiDriver.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/18
  * @brief  lkdGui驱动
  */

#include "lkdGuiDriver.h"
/* lcd驱动文件引用 */
#include "UC1698Driver.h"

/* Gui显示缓冲区 */
static uint8_t  lkdGuiBuff[GUIYMAX][GUIXMAX/8 + GUIXMAX%8];
/* 行位获取数组 */
const static uint8_t colBit[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

/**
  *@brief 将Gui缓冲区数据显示到lcd上
  *@param  None
  *@retval None
  */
void GuiUpdateDisplayAll(void)
{
	uint32_t x,y;
	LCD_CS_LOW();
	SetLcdCol(0x25);
	SetLcdRow(0);
//	WriteLcdCommand(0x60);	//row address LSB     
//	WriteLcdCommand(0x70);	//row address MSB	
//	WriteLcdCommand(0x05);	//Column address LSB 
//	WriteLcdCommand(0x12);	//Column address MSB
	for(y = 0; y < GUIYMAX; y++){
		for(x = 0;x < 20; x++){
			Write8DotsUC1698U(lkdGuiBuff[y][x]);
		}
		WriteLcdData(0x00);
	}
	LCD_CS_HIGH();
}

/**
  *@brief 画点
  *@param  x,y 坐标
  *@param  color 颜色 <目前只有黑白>
  *@retval None
  */
void GuiDrawPoint(lkdCoord x, lkdCoord y, lkdColour color)
{
	if(color == CWHITLE){//白色
		lkdGuiBuff[y][x >> 3] &= ~colBit[x & 0x07];
	}
	else{//黑色
		lkdGuiBuff[y][x >> 3] |= colBit[x & 0x07];
	}
}

/**
  *@brief 读点
  *@param  x,y 坐标
  *@param  color 颜色 <目前只有黑白>
  *@retval None
  */
void  GuiReadPoint(lkdCoord x, lkdCoord y, lkdColour *pColor)
{
	
}

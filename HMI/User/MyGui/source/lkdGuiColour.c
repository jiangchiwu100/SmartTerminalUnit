/**
  * @file   lkdGuiColour.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/18
  * @brief  lkdGui驱动
  */

#include "lkdGuiColour.h"

/* 定义前景色/背景色 */
lkdColour forecolor = 1,backcolor;

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
}

/* END */

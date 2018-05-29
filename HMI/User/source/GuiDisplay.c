/**
  * @file   GuiDisplay.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/28
  * @brief  用户Gui
  */
#include "hmi101.h"
#include "string.h"
#include "lkdGuiUse.h"
#include "imageArray.h"
#include "GuiDisplay.h"

/**
  *@brief GUI启动界面
  *@param  None
  *@retval None
  */
void GUIStartInterface(void)
{
	uint8_t i,j;
	lkdBitmap tImage;
	GuiSetbackcolor(0);
	GuiSetForecolor(1);
	tImage.wide = 160;
	tImage.high = 30;
	tImage.beginx = 0;
	tImage.beginy = 0;
	tImage.bitmap = (uint8_t *)imageTab[0].image;
	GuiAreaBitMap(&tImage,1, 25, 159, 55, 0);
	j = 110;
	for(i=1;i<24;i+=2){
		GuiHPointLine(1,i,j,2,forecolor);
		j -= 2;
	}
	for(i=25;i<55;i+=2){
		GuiHPointLine(1,i,159,2,forecolor);
	}
	j = 70;
	for(i=55;i<78;i+=2){
		GuiHPointLine(j,i,159,2,forecolor);
		j -= 2;
	}
	GuiHLine(1,78,158,forecolor);
	GuiFillRect(1,81,159,94, forecolor);
	GuiHLine(1,96,158,forecolor);
	GuiExchangeColor();
	GuiFont12Align(2,82,156,FONT_MID,"正在启动......");
	GuiExchangeColor();
	GuiHLine(1,143,158,forecolor);
	GuiFont12Align(2,145,156,FONT_MID,VERSIONNUMBER);
	GuiHLine(1,159,158,forecolor);
	GuiUpdateDisplayAll();
}

/* END */

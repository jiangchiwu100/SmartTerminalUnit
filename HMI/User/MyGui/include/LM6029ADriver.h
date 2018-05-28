#ifndef __LM6029ADriver_H
#define	__LM6029ADriver_H

#include "stm32f10x.h"

/* 片选 */
#define LCD_CS_HIGH() {GPIOC->BSRR = GPIO_Pin_4;}
#define LCD_CS_LOW() {GPIOC->BRR = GPIO_Pin_4;}
/* 复位 */
#define LCD_RES_HIGH() {GPIOB->BSRR = GPIO_Pin_11;}
#define LCD_RES_LOW() {GPIOB->BRR = GPIO_Pin_11;}
/* /RD */
#define LCD_RD_HIGH() {GPIOC->BSRR = GPIO_Pin_2;}
#define LCD_RD_LOW() {GPIOC->BRR = GPIO_Pin_2;}
/* /WR */
#define LCD_WR_HIGH() {GPIOC->BSRR = GPIO_Pin_3;}
#define LCD_WR_LOW() {GPIOC->BRR = GPIO_Pin_3;}
/* BLA */
#define LCD_BLA_ON() {GPIOC->BRR = GPIO_Pin_5;}
#define LCD_BLA_OFF() {GPIOC->BSRR = GPIO_Pin_5;}
/* RS */
#define LCD_RS_HIGH() {GPIOB->BSRR = GPIO_Pin_10;}
#define LCD_RS_LOW() {GPIOB->BRR = GPIO_Pin_10;}
/* 数据线 */
#define  LCD_DB0_HIGH()            	 {GPIOE->BSRR = GPIO_Pin_0;}//DB0
#define  LCD_DB0_LOW()               {GPIOE->BRR = GPIO_Pin_0;}
#define  LCD_DB1_HIGH()            	 {GPIOE->BSRR = GPIO_Pin_1;}//DB1
#define  LCD_DB1_LOW()               {GPIOE->BRR = GPIO_Pin_1;}
#define  LCD_DB2_HIGH()            	 {GPIOE->BSRR = GPIO_Pin_2;}//DB2
#define  LCD_DB2_LOW()               {GPIOE->BRR = GPIO_Pin_2;}
#define  LCD_DB3_HIGH()            	 {GPIOE->BSRR = GPIO_Pin_3;}//DB3
#define  LCD_DB3_LOW()               {GPIOE->BRR = GPIO_Pin_3;}
#define  LCD_DB4_HIGH()            	 {GPIOE->BSRR = GPIO_Pin_4;}//DB4
#define  LCD_DB4_LOW()               {GPIOE->BRR = GPIO_Pin_4;}
#define  LCD_DB5_HIGH()            	 {GPIOE->BSRR = GPIO_Pin_5;}//DB5
#define  LCD_DB5_LOW()               {GPIOE->BRR = GPIO_Pin_5;}
#define  LCD_DB6_HIGH()              {GPIOE->BSRR = GPIO_Pin_6;}//DB6
#define  LCD_DB6_LOW()               {GPIOE->BRR = GPIO_Pin_6;}
#define  LCD_DB7_HIGH()            	 {GPIOE->BSRR = GPIO_Pin_7;}//DB7
#define  LCD_DB7_LOW()               {GPIOE->BRR = GPIO_Pin_7;}
/* 数据总线 */
#define LCD_DATABUS(TxData)  \
  	if(TxData&0x01){LCD_DB0_HIGH();}else{LCD_DB0_LOW();}\
	if(TxData&0x02){LCD_DB1_HIGH();}else{LCD_DB1_LOW();}\
	if(TxData&0x04){LCD_DB2_HIGH();}else{LCD_DB2_LOW();}\
	if(TxData&0x08){LCD_DB3_HIGH();}else{LCD_DB3_LOW();}\
	if(TxData&0x10){LCD_DB4_HIGH();}else{LCD_DB4_LOW();}\
	if(TxData&0x20){LCD_DB5_HIGH();}else{LCD_DB5_LOW();}\
	if(TxData&0x40){LCD_DB6_HIGH();}else{LCD_DB6_LOW();}\
	if(TxData&0x80){LCD_DB7_HIGH();}else{LCD_DB7_LOW();}

#define OPEN_LCDDISPLAY 	0xAF/* 开显示 */
#define CLOSE_LCDDISPLAY	0xAE/* 关显示 */

/* lcd初始化 */	
extern void LcdInit(void);
/* 设置页地址 */	
extern void SetLcdPage(uint8_t page);
/* 设置列地址 */
extern void SetLcdCol(uint8_t col);
/* 写数据 */
extern void WriteLcdData(uint8_t data);
/* 写命令 */
extern void WriteLcdCommand(uint8_t cmd);
/* 开显示 */
extern void OpenLcdDisplay(void);
/* 关显示 */
extern void CloseLcdDisplay(void);

#endif /* __LM6029ADriver_H */

/* END */

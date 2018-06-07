/**
  * @file   ds18B20.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   温度获取文件
  */
 
#ifndef __DS18B20_H
#define	__DS18B20_H

#include "stm32f10x.h"

#define CYCTRANSFORMVAULE 1000 /* 循环转换时间*/

#define HIGH  1
#define LOW   0

#define DS18B20_CLK     RCC_APB2Periph_GPIOE
#define DS18B20_PIN     GPIO_Pin_2                  
#define DS18B20_PORT	GPIOE 

/* 输出高电平或低电平 */
#define DS18B20_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(DS18B20_PORT,DS18B20_PIN);\
					else		\
					GPIO_ResetBits(DS18B20_PORT,DS18B20_PIN)
/* 读取引脚的电平 */
#define  DS18B20_DATA_IN()	GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN)


/* 获取温度 */
float GetTemperature(void);
/* 主函数 */
void Ds18B20Main(void);
/* Ds18B20初始化 */
void DS18B20_Init(void);
					
#endif /* __DS18B20_H */

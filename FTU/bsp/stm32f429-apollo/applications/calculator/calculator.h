/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      calculator.h
  * @brief:     calculator function.
  * @version:   ver 1.1 
  * @author:    Mr.Sun
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__


/* INCLUDES ------------------------------------------------------------------*/	
#include <rtthread.h>
#include "stm32f4xx_hal.h"


/* DEFINE --------------------------------------------------------------------*/	
#define FFT_LENGTH		                       32		   
#define CALC_TIMES                           (ADC_SAMPLE_NUM * 2/FFT_LENGTH) // FFT计算次数
#define POWER_RADIO                          1/(FFT_LENGTH*FFT_LENGTH/2)     // FFT功率系数
#define CURRENT_RADIO                        1/(FFT_LENGTH/2*sqrt(2))        // FFT电流系数   
#define VOLTAGE_RADIO	                       1/(FFT_LENGTH/2*sqrt(2))        // FFT电压系数
#define VOLDC_RADIO	                         1                               // FFT直流电压系数
#define FACTOR_ANGLE_TO_RAD                  3.1415926f / 180.0f  
#define FACTOR_RAD_TO_ANGLE                  180.0f / 3.1415926f

#define UDC_SMOOTH_TIMES                           10 // 平滑次数

enum
{
    ALPHA_UabIa,
    ALPHA_UcbIc,
    ALPHA_U0I0,
    ALPHA_UxUx,
    
    ALPHA_NUM,
};

/* PUBLIC VARIABLES ----------------------------------------------------------*/
extern float g_Alpha[ALPHA_NUM];

/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
void CalcultateTask(void);


#endif /* __CALCULATOR_H__ */


/* END OF FILE ---------------------------------------------------------------*/


/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      miscellaneous.c
  * @brief:     用于不好分类的其他项信息
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018-07-26 
  * @update:    
  */

#include "miscellaneous.h"

#include "stm32f429xx.h" 
#include "ll_driver.h"
#include <stdbool.h>
/**
* @brief :系统复位
* @param ： void
* @return: bool
* @update: [2018-07-26][张宇飞][创建]
*/
void SystemReset(void )
{
    
    __set_FAULTMASK(1);
    NVIC_SystemReset();

}


static uint32_t TimeCn;
static bool StopWatchRun = false;
/**
* @brief :秒表初始化，
* @param ：void
* @return: void
* @update: [2018-08-06][张宇飞][使用Timer10]
*[2018-08-16][张宇飞][添加秒表运行状态，只有在停止时，才能开始新的即时。]
*/
void StopWatchInit(void)
{

    if (StopWatchRun)
    {
        return;
    }
  LL_TIM_InitTypeDef TIM_InitStruct;

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM10);

  TIM_InitStruct.Prescaler = 179;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM10, &TIM_InitStruct);
  
    
}

/**
* @brief :秒表开始
* @param ：void
* @return: void
* @update: [2018-08-06][张宇飞][]
*/
void StopWatchStart(void)
{    
    TimeCn = 0;
    LL_TIM_SetCounter(TIM10, 0);
    LL_TIM_EnableCounter(TIM10);
    StopWatchRun = true;
}
/**
* @brief :秒表停止
* @param ：void
* @return: void
* @update: [2018-08-06][张宇飞][]
*/
uint32_t StopWatchStop(void)
{
    TimeCn = LL_TIM_GetCounter(TIM10);
    LL_TIM_DisableCounter(TIM10);
    
    //rt_kprintf("\nduty time: %d\r\n", TimeCn);
    StopWatchRun = false;
    return TimeCn;
}

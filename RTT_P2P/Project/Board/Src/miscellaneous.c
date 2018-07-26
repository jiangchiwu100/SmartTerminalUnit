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


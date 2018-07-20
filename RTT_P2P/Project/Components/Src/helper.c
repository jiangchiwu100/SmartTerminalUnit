/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      help.c
  * @brief:     
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-05-25
  * @update:    
  */
  
#include "helper.h" 

/**
  * @brief :累加和计算
  * @param  array        带计算数组
  * @param  start        开始索引
  * @param  len         长度
  * @return: 计算的累加和
  * @update: [2018-05-25][张宇飞][不验证参数]
  */
uint16_t SumCheck(const uint8_t array[], uint16_t start, uint16_t len)
{
	uint16_t sum = 0;
	for (uint8_t i = 0; i < len; i++)
	{
		sum += array[i + start];
	}
    return sum;
}

/**
  * @brief :计算全1掩码，如n=1，返回1； n=3，返回7，最多32bit
  * @param  n --位数
  * @param  len         长度
  * @return: 全1掩码
  * @update: [2018-06-07][张宇飞][不验证参数]
  */
uint32_t CalMaskAllOne(uint8_t n)
{
    if (n == 0 || n > 32)
    {
        return 0;
    }
    if (n == 32)
    {
        return 0xFFFFFFFF;
    }
    uint32_t data = 1;
    for(uint8_t i = 0 ; i < n; i++)
    {
        data += ((uint32_t)1) << i;
    }
    return data - 1;
    
}


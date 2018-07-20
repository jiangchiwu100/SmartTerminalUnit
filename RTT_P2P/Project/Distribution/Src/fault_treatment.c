/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fault_treatment.c
  * @brief:     故障处理，故障判据，故障隔离
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-05
  * @update:    
  */
#include "fault_treatment.h"    
#include "distribution.h"

//函数替换,根据实验环境
#define MALLOC rt_malloc
#define FREE rt_free
#define MEMSET rt_memset



/**
 * @brief  : 故障判别, 无向故障判别, 只对已经收集完成的进行判别
 * @param  : DistributionArea* distributionArea 配电区域
 * @param  ：uint8_t* faultIndex
 * @param  ：uint8_t flag
 * @return: 0-正常，非空--未找到或异常 
 * @update: [2018-05-26][张宇飞][]
 *[2018-06-25][张宇飞][修改输入类型，修改返回类型为ErroCode,进行重构]
 *[2018-07-07][张宇飞][是否覆盖已有故障， 将无故障改写成有故障]
 */
ErrorCode JudgeFaultArea(DistributionStation* distribution, bool isCover)
{
    uint8_t calFlag = 0;
    uint8_t size = 0;
    SwitchProperty** switchNode;
    CHECK_POINT_RETURN(distribution, NULL, ERROR_NULL_PTR);
    
    DistributionPowerArea* powerArea = distribution->powerArea;
    
    //对于有过流且过流数量为1，标记为故障区域。
    for (uint8_t i=0; i < distribution->areaCount; i++,  powerArea++)
    {   
        if (! powerArea->isUpadeted)
        {
            continue;
        }
		
        size = powerArea->switchNum;
        switchNode = powerArea->areaSwitch;
        calFlag = 0;
        for (uint8_t k = 0; k < size; k++)
        {
            if ((*switchNode)->fault.state == FAULT_YES)
            {
                calFlag++;
            }
            switchNode++;
        }
        
        if (1 == calFlag)
        {            
            powerArea->isFaultArea = true;
        }
        else
        {
			if (isCover)
			{
				powerArea->isFaultArea = false;
			}
           
        }       
    }
	return ERROR_OK_NULL; //未找到
}



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
#include "extern_interface.h"
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


/**
* @brief :获取配置文件名
* @param ：void
* @return: void
* @update: [2018-08-06][张宇飞][创建]
*/
uint8_t* System_getConfigName(uint8_t id)
{
    static uint8_t name[32] = {0};
    sprintf(name, "stu%d.cfg", id);    
    return name;
  
}
/**
* @brief :获取goose配置件名
* @param ：void
* @return: void
* @update: [2018-08-06][张宇飞][创建]
*/
uint8_t* System_getGooseConfigName(uint8_t id)
{
    static uint8_t name[32] = {0};
    sprintf(name, "stu%d-goose.txt", id);   
    return name;
  
}
/**
* @brief :获取goose配置件名,全名,包含路径
* @param ：void
* @return: void
* @update: [2018-08-06][张宇飞][创建]
*/
uint8_t* System_getGooseConfigFullName(uint8_t id)
{
    static uint8_t name[32] = {0};
    sprintf(name, "//sojo//%s", System_getGooseConfigName(id));   
    return name;
  
}

/**
* @brief :获取goose配置件名,全名,包含路径
* @param ：void
* @return: void
* @update: [2018-08-06][张宇飞][创建]
*/
uint8_t* System_getConfigFullName(uint8_t id)
{
    static uint8_t name[32] = {0};
    sprintf(name, "//sojo//%s", System_getConfigName(id));   
    return name;
  
}

extern void tftp_get(const char* host, const char* dir, const char* filename);
#include "dfs_posix.h"
/**
* @brief :获取配置件名
* @param ：void
* @return: void
* @update: [2018-08-06][张宇飞][创建]
*/
bool System_getConfigFile(uint8_t id)
{
    struct stat st;
    MEMSET(&st, 0, sizeof(struct stat));
    uint8_t name[64] = {0};
    MEMSET(name, 0, sizeof(name));
    uint8_t* pName = System_getConfigName(id);    
    uint8_t* pFull = System_getConfigFullName(id);
    sprintf(name, "%s.bk", pFull);
    unlink(name);
    rename(pFull, name);    
    printf("tftp_get %s\n", pName);       
    tftp_get("192.168.10.111", "//sojo", pName); 
    
    int re = stat(pFull, &st);
    if (re < 0)
    {
        printf("tftp_get failure.%s\n", pName);
        unlink(pFull);
        rename(name, pFull);  //恢复  
        
    }
    else
    {
        if (st.st_size < 100)
        {
            unlink(pFull);
            rename(name, pFull);  //恢复  
        }        
    }
    
    
    MEMSET(name, 0, sizeof(name));
    MEMSET(&st, 0, sizeof(struct stat));
    pFull = System_getGooseConfigFullName(id);
    pName = System_getGooseConfigName(id);
    sprintf(name, "%s.bk", pFull);
    unlink(name);
    rename(pFull, name);        
    printf("tftp_get %s\n", pName);
    tftp_get("192.168.10.111", "//sojo", pName);
    re = stat(pFull, &st);
    if (re < 0)
    {
        printf("tftp_get failure.%s\n", pName);
        unlink(pFull);
        rename(name, pFull);  //恢复  
        
    }
    else
    {
        if (st.st_size < 100)
        {
            unlink(pFull);
            rename(name, pFull);  //恢复  
        }        
    }
    return true;
}
/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      timer.h
  * @brief:     The driver of timer.
  * @version:   V1.0.1
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @update:    [2017-09-05][Lexun][make the code cleanup]
  */

#ifndef __DRV_TIMER_H__
#define __DRV_TIMER_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <board.h>
#include <stm32f4xx.h>


/* DEFINE --------------------------------------------------------------------*/
#ifdef  DEBUG
#define TIMER_PRINTF            rt_kprintf
#else
#define TIMER_PRINTF(...)
#endif /* DEBUG */


/* 频率采集 */
#define FRE_GATHER_NUM          8
#define FRISTTIME               0
#define PREPARE                 1
#define VALID                   2

#define TIMER_1MS               1
#define TIMER_2MS               2
#define TIMER_8MS               8
#define TIMER_16MS              16
#define TIMER_32MS              32
#define TIMER_64MS              64
#define TIMER_512MS             512


/* STRUCT --------------------------------------------------------------------*/
/* 频率采集结构体 */
struct TagFreGather
{
    uint8_t sta;   // 采集状态
    uint8_t num;   // 数组中有效数据数量
    uint8_t count; // 数组数据指针

    uint32_t data[FRE_GATHER_NUM];
    uint32_t curValue;  // 本次测量值
    uint32_t lastValue; // 上次测量值
    float freValue;	// 计算频率值
	float freValueProtect;	// 计算频率保护
};

typedef struct TagSystemCounter
{
    uint8_t __1Ms;
    uint8_t __2Ms;
    uint8_t __8Ms;
    uint8_t __16Ms;
    uint8_t __32Ms;	
    uint8_t __64Ms;		
    uint8_t __512Ms;			
    uint32_t  heartBeat;
}SystemCounter;


/* ENUM ----------------------------------------------------------------------*/
enum OS_TMR
{
    TMR_1MS,
    TMR_2MS,
    TMR_5MS,
    TMR_10MS,
    TMR_20MS,
};


/* PUBLIC VARIABLES ----------------------------------------------------------*/
extern struct TagFreGather g_FreGather; // 频率采集

/* PUBLIC FUNCTION  ----------------------------------------------------------*/
int rt_hw_tim1ms_init(void);
void TIM6_Init(rt_uint16_t arr,rt_uint16_t psc);
void TIM7_Init(rt_uint16_t arr,rt_uint16_t psc);
void TIM5_PWM_Init(rt_uint16_t arr,rt_uint16_t psc);
//void TIM3_CH2_Cap_Init(rt_uint32_t arr,rt_uint16_t psc);

void GetFrequency(void);
void FreGatherHandle(void);

void rt_ostimer_init(rt_uint8_t timer);


#endif /* __DRV_TIMER_H__ */

/* END OF FILE ---------------------------------------------------------------*/

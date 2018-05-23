#include "gui_common.h"
#include ".\MultiThread\multi_thread.h"

/* 1ms周期定时器，用来其他函数计时使用 */
static struct rt_timer Timer1;
/* 1ms周期定时器计数 */
static volatile uint32_t timer1Count = 0;

/**
  *@brief 定时器1中断处理函数
  *@param  parameter 
  *@retval None
  */
static void Timer1Timeout(void	*parameter)
{
	timer1Count ++;
}

/**
  *@brief 定时器1初始化函数
  *@param  None 
  *@retval None
  */
void time_static_init(void)
{
	/* 1ms中断一次 */
	rt_timer_init(&Timer1,"gui_time",Timer1Timeout,\
				   RT_NULL,1,RT_TIMER_FLAG_PERIODIC);	
	rt_timer_start(&Timer1);
}

/**
  *@brief 获取开始计时数
  *@param  None
  *@retval beginTick  计时开始数，
  */
inline uint32_t GetTimer1Tick(void)
{
	return timer1Count;
}

/**
  *@brief 获取间隔周期数
  *@param  beginTick  计时开始数，由GetTimer1Tick()函数获得
  *@retval 间隔周期数
  */
inline uint32_t GetTimer1IntervalTick(uint32_t beginTick)
{
	uint32_t temptick;
	temptick = GetTimer1Tick();
	if(beginTick <= temptick){
		return (temptick - beginTick);
	}
	else{
		return ((0xFFFFFFFF - beginTick)+temptick);
	}
}

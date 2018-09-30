/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      quick_break_protest.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    tianxiaoliang
  * @date:      2018.08.03
  * @update:    [YYYY-MM-DD][更改人姓名][变更描述]
  */

#include "quick_break_protect.h"
#include "extern_interface.h"
#include "drv_timer.h"
#include "output.h"
#include "input.h"


extern volatile rt_uint8_t rt_interrupt_nest;
//在board.c文件的rt_hw_board_init()里面将其置为1
uint8_t OSRunning=0;

#define delay_osrunning		  OSRunning			       //OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	RT_TICK_PER_SECOND	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	rt_interrupt_nest		//中断嵌套级别,即中断嵌套次数

static ComProSts g_ComProSts[BRE_DEVMAXNUM];
static CurQuickBreakSts g_Overcur[BRE_DEVMAXNUM];
static float  g_num;
/**
  * @Description: 电流速断
  * @param:  Ki-电流互感器变比
  * @return: Ikmax-线路末端最大短路电流
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
//void CurrentValue(CurQuickBreakSts *curquickbreakSts, float Ki, float Ikmax)
//{
//	float Krel = 1.3;             //保护装置的可靠系数
//	float Kw = 1;		      //接线系数		
//						  
//	*(curquickbreakSts->parastr.pValue) = ((Krel * Kw * Ikmax) / (2 * Ki));		//过流1段动作电流
//}
/**
  * @Description: 电流速断
  * @param:  
  * @return: 
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void CurQuickBreak_ctrl(ComProSts *comProSts,CurQuickBreakSts *curQuickBreakSts)
{										
	if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
	{
		if(curQuickBreakSts->valstr.flag&OVERCURSTA2)
		{
			curQuickBreakSts->valstr.flag = 0;
			*(curQuickBreakSts->valstr.gTime) = 0;					//1段定时器清零
		}
		*(curQuickBreakSts->parastr.pValue) = 0.3;
		
		rt_kprintf("value             =              %d\r\n",    (int32_t)*(curQuickBreakSts->parastr.pValue));

		if((*(comProSts->yc.Ia)>*(curQuickBreakSts->parastr.pValue))||(*(comProSts->yc.Ib)>*(curQuickBreakSts->parastr.pValue)))	    //检测过流
		{
//			if(!(*(curQuickBreakSts->valstr.gTime)&MAINPRO_ENTIMERS))
//            {
//                *(curQuickBreakSts->valstr.gTime) = MAINPRO_ENTIMERS;
//            }
//			if((*(curQuickBreakSts->valstr.gTime)&MAINPRO_ENTIMERS)>=(uint32_t)(*(curQuickBreakSts->parastr.pTime)*1000))
//			{
				curQuickBreakSts->valstr.flag |= OVERCUR1|OVERCURSTA1;
//				rt_hw_output_operate(ADDR_LOGIC_ACT,DO_OPEN);					//分闸
				OpeningclosingOperate(TMR_50MS_OPEN);
                curQuickBreakSts->valstr.flag |= RESETFLAG;
//			}
		}
		else
		{
			*(curQuickBreakSts->valstr.gTime) = 0;
		}
	}
	else if(curQuickBreakSts->valstr.flag&OVERCURSTA1)
	{
		curQuickBreakSts->valstr.flag &= (~OVERCURSTA1);
		curQuickBreakSts->valstr.flag |= OVERCURSTA2;
	}
}

/**
  * @Description: 零序过流
  * @param:  
  * @return: 
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */

void CurQuickBreakI0_ctrl(ComProSts *comProSts,CurQuickBreakI0Sts *curquickbreakSts)
{
	
}



void CurProtectCtrlClock(void)
{
	uint8_t pdrv = 0;
	
	g_ComProSts[pdrv].yc.Ia = &g_TelemetryDB[g_TelemetryAddr.Ia];
	g_ComProSts[pdrv].yc.Ib = &g_TelemetryDB[g_TelemetryAddr.Ib];
	g_ComProSts[pdrv].yc.Ic = &g_TelemetryDB[g_TelemetryAddr.Ic];
	
	g_Overcur[pdrv].parastr.pValue = &g_num;
//	g_Overcur[pdrv].parastr.pTime = &g_FixedValueP[OVER_CURRENT_TIME1];
	g_ComProSts[pdrv].yx.switchOpen.value = &g_TelesignalDB[g_TelesignalAddr.switchOpen];
    g_ComProSts[pdrv].yx.switchClose.value = &g_TelesignalDB[g_TelesignalAddr.switchClose];

	CurQuickBreak_ctrl(&g_ComProSts[pdrv], &g_Overcur[pdrv]);	
}
static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;



void delay_init(uint8_t SYSCLK)
{
	uint32_t reload;

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
	fac_us=SYSCLK;						//不论是否使用OS,fac_us都需要使用
						//如果需要支持OS.
	reload=SYSCLK;						//每秒钟的计数次数 单位为M	   
	reload*=1000000/delay_ostickspersec;	//根据delay_ostickspersec设定溢出时间
											//reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右	
	fac_ms=1000/delay_ostickspersec;		//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/delay_ostickspersec秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	//开启SYSTICK      
}	

void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt = 0;
	uint32_t reload = SysTick->LOAD;

	ticks = nus * fac_us;
	rt_enter_critical();
	told = SysTick->VAL;
	while (1)
	{
		tnow = SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)
			{
				tcnt+=told-tnow;
			}
			else
			{
				tcnt+=reload-tnow+told;
			}
			told = tnow;
			if(tcnt>=ticks)
			{
				break;
			}
		}
	}
	rt_exit_critical();
}

void delay_ostimedly(uint32_t ticks)
{
	rt_thread_delay(ticks);
}

void delay_ms(uint16_t nms)
{	
	if(delay_osrunning && delay_osintnesting==0)//如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)	    
	{		 
		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			delay_ostimedly(nms/fac_ms);	//OS延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((uint32_t)(nms*1000));				//普通方式延时
}

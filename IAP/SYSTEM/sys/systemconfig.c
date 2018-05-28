/**
  ******************************************************************************
  * @file    systemconfig.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    09-10-2017
  * @brief   M3核Systick定时计数器作为整个文件 计数
  ******************************************************************************
  */
#include "systemconfig.h"

/* 1us时间的节拍数 */
static uint32_t Fac_us;	
static uint32_t timeCount;

void Stm32_Clock_Init(uint32_t plln,uint32_t pllm,uint32_t pllp,uint32_t pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    __HAL_RCC_PWR_CLK_ENABLE(); //使能PWR时钟
    
    //下面这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
    //时使性能与功耗实现平衡，此功能只有STM32F42xx和STM32F43xx器件有，
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//设置调压器输出电压级别1
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //打开HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;//打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM=pllm; //主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
    RCC_OscInitStructure.PLL.PLLN=plln; //主PLL倍频系数(PLL倍频),取值范围:64~432.  
    RCC_OscInitStructure.PLL.PLLP=pllp; //系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
	
    if(ret!=HAL_OK) while(1);
    
    ret=HAL_PWREx_EnableOverDrive(); //开启Over-Driver功能
    if(ret!=HAL_OK) while(1);
    
    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; //APB1分频系数为4
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; //APB2分频系数为2
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//同时设置FLASH延时周期为5WS，也就是6个CPU周期。
	
    if(ret!=HAL_OK) while(1);
}


/**
  *@brief 系统优先级配置
  *@param  None
  *@retval None
  */
void NVIC_Configuration(void)
{  
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);		
}

void SysTick_Init(int16_t clock_div)
{
	/* SystemCoreClock / 1000   1ms */
   if (SysTick_Config(SystemCoreClock / clock_div)){ 
		/* Capture error */ 
		while (1);
	}
    Fac_us = SystemCoreClock/1000000;/* 1us时间的节拍数,用于延时函数 */
	OPen_SysTick();
}

/**
  *@brief 精确微秒延时
  *@param  nus  要延时的微秒数
  *@retval None
  */
void Delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				/* LOAD的值	 */    	 
	ticks=nus*Fac_us; 						/* 需要的节拍数	  */ 		 
	tcnt=0;
	told=SysTick->VAL;        				/* 刚进入时的计数器值 */
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)
				tcnt+=told-tnow;			/* 这里注意一下SYSTICK是一个递减的计数器就可以了. */
			else 
				tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)
				break;						/* 时间超过/等于要延迟的时间,则退出. */
		}  
	}							    
}

/**
  *@brief 精确ms延时
  *@param  nms 要延时的毫秒数
  *@retval None
  */
void Delay_ms(uint16_t nms)
{	    		  
	Delay_us((uint32_t)(nms*1000));
}

void SystemconfigInit(void)
{
	
	NVIC_Configuration();	
	SysTick_Init((int16_t)1000);/* 1ms */
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	timeCount ++;
	LedRun();
}

/**
  *@brief 获取开始计时数
  *@param  None
  *@retval beginTick  计时开始数，
  */
uint32_t GetTimer1Tick(void)
{
	return timeCount;
}

/**
  *@brief 获取间隔周期数
  *@param  beginTick  计时开始数，由GetTimer1Tick()函数获得
  *@retval 间隔周期数
  */
uint32_t GetTimer1IntervalTick(uint32_t beginTick)
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

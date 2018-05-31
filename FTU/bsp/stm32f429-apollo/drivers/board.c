 /**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      board.c
  * @brief:     This file is part of RT-Thread RTOS.
  * @version:   V03.001(first implementation)
  * @author:    Bernard
  * @date:      2009-01-05
  * @updata:    [2018-01-23][Lexun][make the code clean up]
  */
	

/* INCLUDE FILES -------------------------------------------------------------*/
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

#include "stm32f4xx.h"
#include "board.h"
#include "drv_usart.h"
#include "stm32f4xx_hal.h"
#include "drv_sdram.h"
#include "drv_fm25vxx.h"
#include "drv_nand.h"
#include "drv_mpu.h"
#include "common_data.h"


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : The initialization function.
  * @param : None.
  * @return: None. 
  */  
void _init(void)
{
}

/**
  * @brief : This function is executed in case of error occurrence.
  * @param : None.
  * @return: None. 
  */  
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
    }
    /* USER CODE END Error_Handler */
}

/**
  * @brief : System Clock Configuration.
  * @param : None.
  * @return: None. 
  */  
void rt_hw_system_clock_init(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 8;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 260;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / RT_TICK_PER_SECOND);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, (INT_SYSTICK_PRIO >> 4) & 0x0F, INT_SYSTICK_PRIO & 0x0F);
}

/**
  * @brief : This is the timer interrupt service routine.
  * @param : None.
  * @return: None. 
  */  
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
  * @brief : re-implement tick interface for STM32 HAL.
  * @param : [TickPriority] the priority of tick timer.
  * @return: [HAL_OK] success. 
  */  
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    /* Return function status */
    return(HAL_OK);
}

/**
  * @brief : get the tick.
  * @param : None.
  * @return: return the number of tick. 
  */  
uint32_t HAL_GetTick(void)
{
    return(rt_tick_get() * 1000 / RT_TICK_PER_SECOND);
}

/**
  * @brief : suspend the tick.
  * @param : None.
  * @return: None. 
  */  
void HAL_SuspendTick(void)
{
}

/**
  * @brief : resume the tick.
  * @param : None.
  * @return: None. 
  */  
void HAL_ResumeTick(void)
{
}

/**
  * @brief : resume the tick.
  * @param : [Delay] the tick delay.
  * @return: None. 
  */  
void HAL_Delay(__IO uint32_t Delay)
{
}

/**
  * @brief : This function will initial STM32 board.
  * @param : None.
  * @return: None. 
  */  
void rt_hw_board_init(void)
{
    /* NVIC Configuration */
#define NVIC_VTOR_MASK              0x3FFFFF80
	
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x10000000 */
    SCB->VTOR  = (0x10000000 & NVIC_VTOR_MASK);
#else /* VECT_TAB_RAM */
    /* Set the Vector Table base location at 0x08000000 */
    SCB->VTOR  = (0x08040000 & NVIC_VTOR_MASK);
#endif /* VECT_TAB_RAM */
	
    HAL_Init();

    rt_hw_system_clock_init();
    rt_hw_usart_init();
	
#ifdef RT_USING_CONSOLE
    rt_console_set_device(CONSOLE_DEVICE);
#endif /* RT_USING_CONSOLE */
    
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#else
    SDRAM_Init();
    rt_hw_mpu_init();
#endif /* RT_USING_COMPONENTS_INIT */
}


/* END OF FILE ---------------------------------------------------------------*/


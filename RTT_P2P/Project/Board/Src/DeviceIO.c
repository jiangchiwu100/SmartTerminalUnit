#include "DeviceIO.h"

#include "ll_driver.h"
#include "basicModule.h"
#include "device.h"
//#define KEY1        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12)  //KEY1按键PD12
//#define KEY2        HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_7)  //KEY2按键PG7
//#define KEY3        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8) //KEY3按键PC8
//#define KEY4        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9) //KEY4按键PC9
//#define KEY5        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) //KEY5按键PA8
//#define KEY6        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_13) //KEY6按键PH13
//#define KEY7        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_14) //KEY7按键PH14
//#define KEY8        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_15) //KEY8按键PH15
static void MX_TIM5_Init(void);

__STATIC_INLINE void SetInput(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    LL_GPIO_SetPinMode(GPIOx, Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinSpeed(GPIOx, Pin,  LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOx, Pin,  LL_GPIO_PULL_NO);
}

__STATIC_INLINE void SetOutput(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    LL_GPIO_SetPinMode(GPIOx, Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOx, Pin,  LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOx, Pin,  LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOx, Pin,  LL_GPIO_PULL_NO);
}


__STATIC_INLINE void KeyInit(void)
{

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);

    SetInput(GPIOD, LL_GPIO_PIN_12);//KEY1 PD12
    SetInput(GPIOG, LL_GPIO_PIN_7);//KEY2 PD7
    SetInput(GPIOC, LL_GPIO_PIN_8);//KEY3 PC8
    SetInput(GPIOC, LL_GPIO_PIN_9);//KEY4 PC9
    SetInput(GPIOA, LL_GPIO_PIN_8);//KEY5 PA8
    SetInput(GPIOH, LL_GPIO_PIN_13);//KEY6 PH13
    SetInput(GPIOH, LL_GPIO_PIN_14);//KEY7 PH14
    SetInput(GPIOH, LL_GPIO_PIN_15);//KEY8 PH15


}

//#define LED1 PDout(3)   //LED1
//#define LED2 PGout(10)  //LED2
//#define LED3 PDout(6)   //LED3
//#define LED4 PDout(7)   //LED4
//#define LED5 PBout(5)   //LED6
//#define LED6 PGout(12)  //LED5
//#define LED7 PBout(6)   //LED7
//#define LED8 PIout(10)   //LED8
//#define LED9 PIout(11)   //LED9
//#define LED10 PHout(7)  //LED10
//#define LED11 PHout(8)  //LED11

__STATIC_INLINE void LedInit(void)
{
    //LED 灯
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);


    //指示灯
    SetOutput(GPIOB, LL_GPIO_PIN_0);


//    SetOutput(GPIOD, LL_GPIO_PIN_3); //LED1
//    SetOutput(GPIOG, LL_GPIO_PIN_10); //LED2
//    SetOutput(GPIOD, LL_GPIO_PIN_6); //LED3
//    SetOutput(GPIOD, LL_GPIO_PIN_7); //LED4
//    SetOutput(GPIOB, LL_GPIO_PIN_5); //LED5
//    SetOutput(GPIOG, LL_GPIO_PIN_12); //LED6
//    SetOutput(GPIOB, LL_GPIO_PIN_6); //LED7
//    SetOutput(GPIOI, LL_GPIO_PIN_10); //LED8
//    SetOutput(GPIOI, LL_GPIO_PIN_11); //LED9
//    SetOutput(GPIOH, LL_GPIO_PIN_7); //LED10
//    SetOutput(GPIOH, LL_GPIO_PIN_8); //LED11

//    SetOutput(GPIOD, LL_GPIO_PIN_11); //LED12 磁保持继电器 动作
//    SetOutput(GPIOD, LL_GPIO_PIN_13); //LED12 磁保持继电器 复归
//    
//    
//    for (uint8_t i = LED1; i < LED8; i++)
//    {
//        SetLedState(i, LED_OFF);
//    }
}




/**
  * @brief : 初始化IO
  * @param : None.
  * @return: None.
  */
void InitDeviceIO(void)
{
    
    //KeyInit();
    LedInit();

}
/**
  * @brief : 获取指定输入，低电平非0，高电平0
  * @param : None.
  * @return: None.
  */
uint32_t ReadInputPin(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    uint32_t value = LL_GPIO_ReadInputPort(GPIOx);
    return (value & Pin) ^ Pin ; //低电平非0，高电平0
}

/**
  * @brief : 获取指定按键，有效非0，无效为0
  * @param : None.
  * @return: None.
  */
uint32_t value = 0;
uint32_t GetKeyValue(uint8_t key)
{

    value = 0;
    switch (key)
    {
    case KEY1:
    {
        value = ReadInputPin(GPIOD, LL_GPIO_PIN_12);
        break;
    }
    case KEY2:
    {
        value = ReadInputPin(GPIOG, LL_GPIO_PIN_7);
        break;
    }
    case KEY3:
    {
        value = ReadInputPin(GPIOC, LL_GPIO_PIN_8);
        break;
    }
    case KEY4:
    {
        value = ReadInputPin(GPIOC, LL_GPIO_PIN_9);
        break;
    }
    case KEY5:
    {
        value = ReadInputPin(GPIOH, LL_GPIO_PIN_13);
        break;
    }
    case KEY6:
    {
        value = ReadInputPin(GPIOA, LL_GPIO_PIN_8);
        
        break;
    }
    case KEY7:
    {
        value = ReadInputPin(GPIOH, LL_GPIO_PIN_14);
        break;
    }
    case KEY8:
    {
        value = ReadInputPin(GPIOH, LL_GPIO_PIN_15);
        break;
    }
    }
    return value;
}

__STATIC_INLINE void LedSet(GPIO_TypeDef *GPIOx, uint32_t Pin, uint8_t state)
{
    switch(state)
    {
    case LED_ON:
    {
        LL_GPIO_SetOutputPin(GPIOx, Pin);

        break;
    }
    case LED_OFF:
    {
        LL_GPIO_ResetOutputPin(GPIOx, Pin);
        break;
    }
    case LED_TOGGLE:
    {
        LL_GPIO_TogglePin(GPIOx, Pin);
        break;
    }
    }
}




void SetLedState(uint8_t led, uint8_t state)
{
    switch(led)
    {
    case LED1:
    {
        LedSet(GPIOD, LL_GPIO_PIN_3, state); //LED1
        break;
    }
    case LED2:
    {
        LedSet(GPIOG, LL_GPIO_PIN_10, state); //LED2
        break;
    }
    case LED3:
    {
        LedSet(GPIOD, LL_GPIO_PIN_6, state); //LED3
        break;
    }
    case LED4:
    {
        LedSet(GPIOD, LL_GPIO_PIN_7, state); //LED4
        break;
    }
    case LED5:
    {
        LedSet(GPIOG, LL_GPIO_PIN_12, state); //LED6
        break;
    }
    case LED6:
    {
        LedSet(GPIOB, LL_GPIO_PIN_5, state); //LED5
        
        break;
    }
    case LED7:
    {
        LedSet(GPIOB, LL_GPIO_PIN_6, state); //LED7
        break;
    }
    case LED8:
    {
        LedSet(GPIOI, LL_GPIO_PIN_10, state); //LED8
        break;
    }
    case LED9:
    {
        LedSet(GPIOI, LL_GPIO_PIN_11, state); //LED9
        break;
    }
    case LED10:
    {
        LedSet(GPIOH, LL_GPIO_PIN_7, state); //LED10
        break;
    }
    case LED11:
    {
        LedSet(GPIOH, LL_GPIO_PIN_8, state); //LED11
        break;
    }
    case LED12:
    {
        switch(state)
        {
        case LED_ON:
        {
            LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_11);
            delay_ms_soft(10);
            LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_11);
            break;
        }
        case LED_OFF:
        {
            LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_13);
            delay_ms_soft(10);
            LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_13);
            break;
        }
        case LED_TOGGLE:
        {
            // LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_11);
            break;
        }
        }
    }

    }
}



static void MX_TIM5_Init(void)
{

    LL_TIM_InitTypeDef TIM_InitStruct;
    LL_TIM_OC_InitTypeDef TIM_OC_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);

    TIM_InitStruct.Prescaler = 179;
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload = 0;
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    LL_TIM_Init(TIM5, &TIM_InitStruct);

    LL_TIM_SetClockSource(TIM5, LL_TIM_CLOCKSOURCE_INTERNAL);

    LL_TIM_OC_EnablePreload(TIM5, LL_TIM_CHANNEL_CH1);

    TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
    TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
    TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
    TIM_OC_InitStruct.CompareValue = 100;
    TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
    LL_TIM_OC_Init(TIM5, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);

    LL_TIM_OC_DisableFast(TIM5, LL_TIM_CHANNEL_CH1);

    LL_TIM_SetTriggerOutput(TIM5, LL_TIM_TRGO_RESET);

    LL_TIM_DisableMasterSlaveMode(TIM5);

    /**TIM5 GPIO Configuration
    PH10   ------> TIM5_CH1
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
    LL_GPIO_Init(GPIOH, &GPIO_InitStruct);

}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
//void SystemClock_Config(void)
//{
//    /* Enable HSE oscillator */
//    LL_RCC_HSE_Enable();
//    while(LL_RCC_HSE_IsReady() != 1)
//    {
//    };

//    /* Set FLASH latency */
//    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

//    /* Enable PWR clock */
//    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

//    /* Activation OverDrive Mode */
//    LL_PWR_EnableOverDriveMode();
//    while(LL_PWR_IsActiveFlag_OD() != 1)
//    {
//    };

//    /* Activation OverDrive Switching */
//    LL_PWR_EnableOverDriveSwitching();
//    while(LL_PWR_IsActiveFlag_ODSW() != 1)
//    {
//    };

//    /* Main PLL configuration and activation */
//    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 360, LL_RCC_PLLP_DIV_2);
//    LL_RCC_PLL_Enable();
//    while(LL_RCC_PLL_IsReady() != 1)
//    {
//    };

//    /* Sysclk activation on the main PLL */
//    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
//    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
//    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
//    {
//    };

//    /* Set APB1 & APB2 prescaler */
//    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
//    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);

//    

//    
//}



//void delay_ms_soft(unsigned int cn)
//{
//	for ( ; cn-- > 0; )
//		for ( int n = 13850; n-- > 0; );
//
//}

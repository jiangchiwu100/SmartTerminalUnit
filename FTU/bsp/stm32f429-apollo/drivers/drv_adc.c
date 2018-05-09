/** ----------------------------------------------------------------------------
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_adc.c
  * @brief:     The driver of AD7607,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    Lexun
  * @date:      2009-01-05
  * @update:    [2017-11-22][Lexun][The STM32F7 version(V1.0.0)]
 ** --------------------------------------------------------------------------*/

#if(0)
/* INCLUDE FILES -------------------------------------------------------------*/
#include <board.h>
#include "drv_adc.h"
#include "bsp_cfg.h"
#include "gpio_dev.h"


/* PUBIC VARIABLES -----------------------------------------------------------*/


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct ad7606_device _ad7606_dev;
static struct rt_device Ad7606Device;
static TIM_IC_InitTypeDef sICConfig;
static rt_uint8_t AdcBuffer[8][AD_BUFF_MAX_COUNT]; // ad Channel = 8, count of the one wave = 1024(max), count of wave = 8
static rt_uint8_t RwTempBuffer[AD_BUFF_MAX_COUNT];
static SRAM_HandleTypeDef sramHandle;
static FMC_NORSRAM_TimingTypeDef Timing;


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : Initiliaztion the timer of AD7606.
  * @param : [ad] the pointer of ad7606 device struct.
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
static void _ad7606_timer_init(struct ad7606_device *ad)
{
    rt_uint32_t prescalcer = (rt_uint32_t) ((SystemCoreClock /2) / ad->config.sample_scal) - 1;
  
    /* 1 - Set TIMx instance */
    ad->ad_timer.Instance = TIM2;
    ad->ad_timer.Init.Period = prescalcer;
    ad->ad_timer.Init.Prescaler = 0;
    ad->ad_timer.Init.ClockDivision = 0;
    ad->ad_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&ad->ad_timer);
	
    /* 2 - Start the TIM Base generation in interrupt mode */
    HAL_TIM_Base_Start_IT(&ad->ad_timer); /* Start Channel1 */
}

/* -----------------------------------------------------------------------------
** Function name    : _ad7606_gpio_init
** Descriptions     : The GPIO of AD7606 initialized.
** Parameter        : None
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static void _ad7606_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __GPIOI_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);
  
    GPIO_InitStructure.Pin = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
  
    GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;

    GPIO_InitStructure.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11, GPIO_PIN_SET);
		
    rt_thread_delay(10);

    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_4, GPIO_PIN_RESET);
		
    rt_thread_delay(1);
		
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_4, GPIO_PIN_SET);
}

/* -----------------------------------------------------------------------------
** Function name    : _ad7606_start
** Descriptions     : The device of AD7606 started.
** Parameter        : None
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static void _ad7606_start(void)
{
    rt_uint32_t k = 0;

    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_4, GPIO_PIN_RESET);
	
    for (k = 0; k < 2; k++);
	
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_4, GPIO_PIN_SET);
}

/* -----------------------------------------------------------------------------
** Function name    : _ad7606_complete_sample
** Descriptions     : Release the semaphore when the AD7606 complete sample.
** Parameter        : None
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static void _ad7606_complete_sample(void)
{
    if (_ad7606_dev.context.sample_complete_sem != RT_NULL)
    {
        rt_sem_release(_ad7606_dev.context.sample_complete_sem);
    }
}

/* -----------------------------------------------------------------------------
** Function name    : _ad7606_wait_sample
** Descriptions     : Wait for releasing the semaphore when the AD7606 complete sample.
** Parameter        : None
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static void _ad7606_wait_sample(void)
{
    if (_ad7606_dev.context.sample_complete_sem != RT_NULL)
    {
        rt_sem_take(_ad7606_dev.context.sample_complete_sem, RT_WAITING_FOREVER);
    }
}

/* -----------------------------------------------------------------------------
** Function name    : _frq_timer_init
** Descriptions     : The timer of AD7606 initialized.
** Parameter        : <ad> The AD7606 device.
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static void _frq_timer_init(struct ad7606_device *ad)
{
    ad->frq_timer.Instance = TIM3;
 
    ad->frq_timer.Init.Period        = 0xFFFF;
    ad->frq_timer.Init.Prescaler     = 44;
    ad->frq_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    ad->frq_timer.Init.CounterMode   = TIM_COUNTERMODE_UP;  
    HAL_TIM_IC_Init(&ad->frq_timer);

    sICConfig.ICPolarity  = TIM_ICPOLARITY_RISING;
    sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sICConfig.ICPrescaler = TIM_ICPSC_DIV1;
    sICConfig.ICFilter    = 0;   
    HAL_TIM_IC_ConfigChannel(&ad->frq_timer, &sICConfig, TIM_CHANNEL_1);
  
    HAL_TIM_IC_Start_IT(&ad->frq_timer, TIM_CHANNEL_1);
}

/* -----------------------------------------------------------------------------
** Function name    : SRAM_MspInit
** Descriptions     : The SRAM msp initialized.
** Parameter        : None
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static void SRAM_MspInit(void)
{
    GPIO_InitTypeDef GPIO_Init_Structure;
//    SRAM_HandleTypeDef *hsram = &sramHandle;

    /* Enable FMC clock */
    __FMC_CLK_ENABLE();

    /* Enable chosen DMAx clock */

    /* Enable GPIOs clock */
    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();

    /* Common GPIO configuration */
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_PULLUP;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
    GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;

    /* GPIOD configuration */
    GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

    /* GPIOE configuration */  
    GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

    /* GPIOF configuration */  
    GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

    /* GPIOG configuration */  
    GPIO_Init_Structure.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure); 
}

/* -----------------------------------------------------------------------------
** Function name    : _ad7606_sram_init
** Descriptions     : The SRAM for AD7606 initialized.
** Parameter        : None
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static rt_err_t _ad7606_sram_init(void)
{
    sramHandle.Instance = FMC_NORSRAM_DEVICE;
    sramHandle.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  
    /* SRAM device configuration */  
    Timing.AddressSetupTime      = 2;
    Timing.AddressHoldTime       = 1;
    Timing.DataSetupTime         = 2;
    Timing.BusTurnAroundDuration = 1;
    Timing.CLKDivision           = 2;
    Timing.DataLatency           = 2;
    Timing.AccessMode            = FMC_ACCESS_MODE_A;
  
    sramHandle.Init.NSBank             = FMC_NORSRAM_BANK3;
    sramHandle.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
    sramHandle.Init.MemoryType         = FMC_MEMORY_TYPE_SRAM;
    sramHandle.Init.MemoryDataWidth    = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    sramHandle.Init.BurstAccessMode    = FMC_NORSRAM_FLASH_ACCESS_DISABLE;
    sramHandle.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    sramHandle.Init.WrapMode           = FMC_WRAP_MODE_DISABLE;
    sramHandle.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
    sramHandle.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
    sramHandle.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;
    sramHandle.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
    sramHandle.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    sramHandle.Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;
    sramHandle.Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    
    /* SRAM controller initialization */
    SRAM_MspInit();
    HAL_SRAM_Init(&sramHandle, &Timing, &Timing);
		
    return (RT_EOK);
}

/* -----------------------------------------------------------------------------
** Function name    : _ad_init
** Descriptions     : The device of AD7606 initialized.
** Parameter        : <dev> The device.
** Return           : <RT_EOK> Success.
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static rt_err_t _ad_init(rt_device_t dev)
{
    _ad7606_sram_init(); 
    _ad7606_gpio_init();

    return (RT_EOK);
}

/* -----------------------------------------------------------------------------
** Function name    : _ad_open
** Descriptions     : The device of AD7606 initialized.
** Parameter        : <dev> The device.
**                  : <oflag> The flag.
** Return           : <RT_EOK> Success.
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static rt_err_t _ad_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct ad7606_device *ad = (struct ad7606_device*)dev->user_data; 
	
    _frq_timer_init(ad);
    _ad7606_timer_init(ad);
    _ad7606_start();  
	
    return (RT_EOK);
}

/* -----------------------------------------------------------------------------
** Function name    : _ad_close
** Descriptions     : Close the device of AD7606.
** Parameter        : <dev> The device.
** Return           : <RT_EOK> Success.
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static rt_err_t _ad_close(rt_device_t dev)
{
    return (RT_EOK);
}

/* -----------------------------------------------------------------------------
** Function name    : _ad_read
** Descriptions     : Read the device of AD7606.
** Parameter        : <dev> The device.
**                  : <pos> The position.
**                  : <buffer> The buffer.
**                  : <size> The size of buffer.
** Return           : <r_size> The size of AD7606 buffer.
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static rt_size_t _ad_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_uint32_t r_size = 0;
    struct ad7606_device *ad = (struct ad7606_device*)dev->user_data;
	
    if (pos < 8)
		{
        r_size = rt_ringbuffer_get(&ad->config.rw_buffer[pos], buffer, size);
		}
    else
		{
        r_size = rt_ringbuffer_get(&ad->config.pos_buffer, buffer, size);
		}
		
    return (r_size);
}

/* -----------------------------------------------------------------------------
** Function name    : _ad_write
** Descriptions     : Write the device of AD7606.
** Parameter        : <dev> The device.
**                  : <pos> The position.
**                  : <buffer> The buffer.
**                  : <size> The size of buffer.
** Return           : <r_size> The size of AD7606 buffer.
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
static rt_size_t _ad_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    return (0);
}

/* -----------------------------------------------------------------------------
** Function name    : _ad_control
** Descriptions     : Control the device of AD7606.
** Parameter        : <dev> The device.
**                  : <cmd> The command.
**                  : <args> The parameter.
** Return           : <RT_EOK> Success.
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
//static rt_err_t _ad_control(rt_device_t dev, rt_uint8_t cmd, void *args)
static rt_err_t _ad_control(rt_device_t dev, int cmd, void *args)
{
    struct ad7606_device *ad = (struct ad7606_device*)dev->user_data;
	
    switch (cmd)
    {
        case AD_CMD_READ_FREQ:
            *((rt_uint32_t *)args)=ad->context.freq;
            break;
        case AD_CMD_START_SAMPLE:
            if (ad->config.single_sample_enable == 1)
						{
                HAL_TIM_Base_Start_IT(&ad->ad_timer);
						}
            break;
						
        case AD_CMD_WAIT_SAM:
            _ad7606_wait_sample();
            break;
				
        case AD_CMD_GET_PCOUNTS:
            *((rt_uint32_t *)args) = ad->config.sample_point_count;
            break;
    }
		
    return (RT_EOK);
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/* -----------------------------------------------------------------------------
** Function name    : TIM2_IRQHandler
** Descriptions     : The interrupt Service function for TIM2.
** Parameter        : None
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
void TIM2_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_TIM_IRQHandler(&_ad7606_dev.ad_timer);
    rt_interrupt_leave();
}

/* -----------------------------------------------------------------------------
** Function name    : TIM3_IRQHandler
** Descriptions     : The interrupt Service function for TIM3.
** Parameter        : None
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
void TIM3_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_TIM_IRQHandler(&_ad7606_dev.frq_timer);
    rt_interrupt_leave();
}

/* -----------------------------------------------------------------------------
** Function name    : HAL_TIM_PeriodElapsedCallback
** Descriptions     : The callback function of TIM.
** Parameter        : <htim> The TIM pointer.
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    rt_uint8_t k = 0;
    rt_uint16_t temp = 0;
    static rt_uint32_t single_flag = 0;

    if (htim->Instance == TIM5)
    {
        return;
    }

    for (k = 0; k < 8; k++)
    {
        temp = *((rt_int16_t *)_ad7606_dev.config.ad_addr);
        if (_ad7606_dev.config.single_sample_enable == 1 && _ad7606_dev.context.sample_counter== 0)
        {
            if (single_flag == 0)
						{
                continue;
						}
        }

        rt_ringbuffer_put_force(&_ad7606_dev.config.rw_buffer[k], (rt_uint8_t*)&temp, sizeof(rt_uint16_t));    
    }

    temp = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == IN_NORMAL) ? 1 : 0; // 直接读取IO,节省时间合信号
    rt_ringbuffer_put_force(&_ad7606_dev.config.pos_buffer, (rt_uint8_t*)&temp, sizeof(rt_uint16_t));

    if (_ad7606_dev.config.single_sample_enable == 1 && single_flag == 0)
    {
        single_flag = 1;
    }
    else
    {
        _ad7606_dev.context.sample_counter++;
    }

    if (_ad7606_dev.context.sample_counter >= _ad7606_dev.config.sample_point_count)
    {
        if (_ad7606_dev.config.single_sample_enable == 1)
        {
            single_flag = 0;
            HAL_TIM_Base_Stop_IT(&_ad7606_dev.ad_timer);
        }
        _ad7606_dev.context.sample_counter = 0;
        _ad7606_complete_sample();
    }
    _ad7606_start();
}

/* -----------------------------------------------------------------------------
** Function name    : HAL_TIM_IC_CaptureCallback
** Descriptions     : The callback function of TIM_IC.
** Parameter        : <htim> The TIM_HandleTypeDef pointer.
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    static rt_uint32_t uhCaptureIndex = 0;
    static rt_uint32_t uwIC2Value1 = 0;
    static rt_uint32_t uwIC2Value2 = 0;
    static rt_uint32_t uwDiffCapture = 0;
    static rt_uint32_t lastFreq = 50;
    static rt_int32_t freqChangedCounter = 0;

    if (htim->Instance == TIM3)
    {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if (uhCaptureIndex == 0)
            {
                /* Get the 1st Input Capture value */
                uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
                uhCaptureIndex = 1;
            }
            else if (uhCaptureIndex == 1)
            {
                /* Get the 2nd Input Capture value */
                uwIC2Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

                /* Capture computation */
                if (uwIC2Value2 > uwIC2Value1)
                {
                    uwDiffCapture = (uwIC2Value2 - uwIC2Value1);
                }
                else if (uwIC2Value2 < uwIC2Value1)
                {
                    uwDiffCapture = ((0xFFFF - uwIC2Value1) + uwIC2Value2);
                }
                else
                {
                    uwDiffCapture = 0;
                }
                _ad7606_dev.context.freq = (((HAL_RCC_GetPCLK1Freq() / 45 * 200) / uwDiffCapture));
								
                if (_ad7606_dev.context.freq < 4400 || _ad7606_dev.context.freq > 5600)
                {
                    _ad7606_dev.context.freq = 5000;
                }
                else
                {
                    if (((lastFreq > _ad7606_dev.context.freq) && ((lastFreq - _ad7606_dev.context.freq) >= 100))
                        || ((lastFreq < _ad7606_dev.context.freq) && ((_ad7606_dev.context.freq - lastFreq) >= 100)))
                    {
                        lastFreq = _ad7606_dev.context.freq;
                        freqChangedCounter = 5;
                    }

                    if (freqChangedCounter > 0)
                    {
                        HAL_TIM_Base_Stop_IT(&_ad7606_dev.ad_timer);
                        _ad7606_dev.config.sample_scal=_ad7606_dev.context.freq*_ad7606_dev.config.sample_point_count / 100;
                        _ad7606_timer_init(&_ad7606_dev);
                        freqChangedCounter--;
                    }
                }

                uhCaptureIndex = 0;
            }
        }
    }
}

/* -----------------------------------------------------------------------------
** Function name    : HAL_TIM_IC_MspInit
** Descriptions     : TIM_IC of device of AD7606 initialized.
** Parameter        : <htim> The pointer.
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_InitStruct;
 
    /* 1- Enable peripherals and GPIO Clocks */
    /* TIMx Peripheral clock enable */
    __TIM3_CLK_ENABLE();

    /* Enable GPIO channels Clock */
    __GPIOA_CLK_ENABLE();

    /* Configure  (TIMx_Channel) in Alternate function, push-pull and 100MHz speed */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* 2- Configure the NVIC for TIMx */
    HAL_NVIC_SetPriority(TIM3_IRQn, (INT_TIM3_PRIO >> 4) & 0x0F, INT_TIM3_PRIO & 0x0F);
  
    /* Enable the TIM3 global Interrupt */
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

/* -----------------------------------------------------------------------------
** Function name    : HAL_TIM_Base_MspInit
** Descriptions     : TIM_Base of device of AD7606 initialized.
** Parameter        : <htim> The pointer.
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        /* 1- Enable peripherals and GPIO Clocks */
        /* TIMx Peripheral clock enable */
			
        __TIM2_CLK_ENABLE();
        /* 2- Configure the NVIC for TIMx */
        /* Set the TIMx priority */
        HAL_NVIC_SetPriority(TIM2_IRQn, (INT_TIM2_PRIO >> 4) & 0x0F, INT_TIM2_PRIO & 0x0F);

        /* Enable the TIMx global Interrupt */
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
    else if (htim->Instance == TIM5)
    {
        __TIM5_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM5_IRQn, (INT_TIM5_PRIO >> 4) & 0x0F, INT_TIM5_PRIO & 0x0F);

        /* Enable the TIMx global Interrupt */
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
    }
}

/* -----------------------------------------------------------------------------
** Function name    : rt_hw_ad7606_init
** Descriptions     : The hardware of AD7606 device initialized.
** Parameter        : <sample_count> The sample count.
**                  : <single_sample> The single sample.
** Return           : None
** Global parameter : None
** Module           : None
** Create by        : Mr.Lee
** Create date      : 2017.07.12
** -----------------------------------------------------------------------------
** Modified by      :             
** Modified date    :           
** Version          :                 
** Descriptions     :            
** ---------------------------------------------------------------------------*/
//void rt_hw_ad7606_init(rt_uint32_t sample_count, rt_uint32_t single_sample)
void rt_hw_adc_init(rt_uint32_t sample_count, rt_uint32_t single_sample)
{
    rt_uint32_t i = 0;
    rt_device_t device = 0; 
	
    device = RT_DEVICE(&Ad7606Device);
	
    RT_ASSERT(device != RT_NULL);
	
    rt_memset(&_ad7606_dev, 0, sizeof(struct ad7606_device));

    _ad7606_dev.config.ad_addr = AD_ADDR_BASE;
    _ad7606_dev.config.sample_point_count = sample_count;
    _ad7606_dev.config.sample_scal = 50 * sample_count; // default 50 hz
    _ad7606_dev.config.single_sample_enable = single_sample;

    for (i = 0; i < 8;i++)
    {
        rt_ringbuffer_init(&_ad7606_dev.config.rw_buffer[i], AdcBuffer[i], AD_BUFF_MAX_COUNT);
    }

    rt_ringbuffer_init(&_ad7606_dev.config.pos_buffer, RwTempBuffer, AD_BUFF_MAX_COUNT);

    _ad7606_dev.context.sample_complete_sem = rt_sem_create("adcomp", 0, RT_IPC_FLAG_FIFO);

    /* set device class and generic device interface */
    device->type        = RT_Device_Class_Unknown;
    device->init        = _ad_init;
    device->open        = _ad_open;
    device->read        = _ad_read;
    device->write       = _ad_write;
    device->close       = _ad_close;
    device->control     = _ad_control;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;
    device->user_data   = &_ad7606_dev;
		
    /* register to RT-Thread device system */
    rt_device_register(device, ADC_DEVICE_NAME, RT_DEVICE_FLAG_RDONLY | RT_DEVICE_FLAG_STANDALONE);
}
#endif

/* END OF FILE ---------------------------------------------------------------*/


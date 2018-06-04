/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      ad7616_fsmc.c
  * @brief:     The driver of AD7616_FSMC.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @updata  : [2018-02-27][Lexun][make code cleanup]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <math.h>
#include "rtthread.h"
#include "rtdevice.h"
#include "drv_gpio.h"
#include "drv_timer.h"
#include "drv_ad7616.h"
#include "other_protect.h"
#include "common_data.h"
#include ".\MultiThread\multi_thread.h"
#include "wave_recording.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static SRAM_HandleTypeDef hsram2;


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @description: ad7616 delay function.
  * @param[in]  : [nus] us delay.
  * @return     : none
  */
static void AD7616_delay_ns(rt_uint32_t nus)
{
    while (nus--)
    {
        __NOP();
    }
}

/**
  * @description: FSMC Init.
  * @param[in]  : none
  * @return     : none
  * @updata     : [YYYY-MM-DD] [Change person name][change content]
  */
static void AD7616_FSMC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    FMC_NORSRAM_TimingTypeDef Timing;

    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    hsram2.Instance = FMC_NORSRAM_DEVICE;
    hsram2.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram2.Init */
    hsram2.Init.NSBank = FMC_NORSRAM_BANK2;
    hsram2.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_ENABLE;
    hsram2.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;//´æ´¢Æ÷ÀàÐÍ
    hsram2.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;//Êý¾Ý¿í¶È
    hsram2.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
    hsram2.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram2.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
    hsram2.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
    hsram2.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;//Ð´Ê¹ÄÜ
    hsram2.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
    hsram2.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;//À©Õ¹Ä£Ê½Ê¹ÄÜÎ»
    hsram2.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram2.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
    hsram2.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    hsram2.Init.PageSize = FMC_PAGE_SIZE_NONE;

    Timing.AddressSetupTime = 3;
    Timing.AddressHoldTime = 0;
    Timing.DataSetupTime = 5;
    Timing.BusTurnAroundDuration = 1;
    Timing.CLKDivision = 0;
    Timing.DataLatency = 0;
    Timing.AccessMode = FMC_ACCESS_MODE_A;

    HAL_SRAM_Init(&hsram2, &Timing, NULL);
}

/**
  * @Description: Read the sampled data of AD7616
  * @param[in]  : none
  * @return     : none
  * @updata     : [YYYY-MM-DD] [Change person name][change content]
  */
static void read_ad7616_data(void)
{
    static uint16_t s_ADC_Count;
    static short temp;
 
//0 
    g_SampleQueueBuf->udc1[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));
    temp = (*((volatile  short *)AD7616_DATA_ADDR));
//    AD7616_delay_ns(1);
 
//1 
    g_SampleQueueBuf->udc2[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));
    temp = (*((volatile  short *)AD7616_DATA_ADDR));
//    AD7616_delay_ns(1);

//2
    if(g_Parameter[CFG_PRO_VOL_N] == 0)
    {g_SampleQueueBuf->uC[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));}
    else
    {g_SampleQueueBuf->uA[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));}
    temp = (*((volatile  short *)AD7616_DATA_ADDR));
//    AD7616_delay_ns(1);
 
//3
    g_SampleQueueBuf->uc[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));
    temp = (*((volatile  short *)AD7616_DATA_ADDR));
//    AD7616_delay_ns(1);

//4
    g_SampleQueueBuf->ua[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));
    g_SampleQueueBuf->i0[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));

//5
    if(g_Parameter[CFG_PRO_VOL_N] == 0)
    {g_SampleQueueBuf->uA[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));}
    else
    {g_SampleQueueBuf->uC[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));}
    g_SampleQueueBuf->ic[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));

//6
    g_SampleQueueBuf->ub[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));
    g_SampleQueueBuf->ib[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));
 
//7 
    g_SampleQueueBuf->u0[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));
    g_SampleQueueBuf->ia[g_SampleQueueIn][s_ADC_Count] = (*((volatile  short *)AD7616_DATA_ADDR));

//6
    g_SampleQueueBuf->ub[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->ub[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;
    g_SampleQueueBuf->ib[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->ib[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;

//5
    if(g_Parameter[CFG_PRO_VOL_N] == 0)
    {g_SampleQueueBuf->uA[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->uA[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;}
    else
    {g_SampleQueueBuf->uC[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->uC[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;}
    g_SampleQueueBuf->ic[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->ic[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2; 

//4
    g_SampleQueueBuf->ua[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->ua[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;
    g_SampleQueueBuf->i0[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->i0[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;
 
//3 
    g_SampleQueueBuf->uc[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->uc[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;
    temp = (*((volatile  short *)AD7616_DATA_ADDR));
//    AD7616_delay_ns(1);
    
//2
    if(g_Parameter[CFG_PRO_VOL_N] == 0)
    {g_SampleQueueBuf->uC[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->uC[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;}
    else
    {g_SampleQueueBuf->uA[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->uA[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;}
    temp = (*((volatile  short *)AD7616_DATA_ADDR));
//    AD7616_delay_ns(1);
 
//1 
    g_SampleQueueBuf->udc2[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->udc2[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;
    temp = (*((volatile  short *)AD7616_DATA_ADDR));
//    AD7616_delay_ns(1);
 
//0 
    g_SampleQueueBuf->udc1[g_SampleQueueIn][s_ADC_Count] = (g_SampleQueueBuf->udc1[g_SampleQueueIn][s_ADC_Count] + (*((volatile  short *)AD7616_DATA_ADDR)))/2;
    temp = (*((volatile  short *)AD7616_DATA_ADDR));
//    AD7616_delay_ns(1);  
    
    s_ADC_Count++;

    if (s_ADC_Count == 48)
    {  
        s_ADC_Count = 0;
        
        rt_event_send(&calculate_event, EVENT_RUN);// ²ÉÑùÍê³É
                    
        if (++g_SampleQueueIn >= (ADC_WAVE_SEMP_NUM * 2))
        {
            g_SampleQueueIn = 0;
        }
    } 

    if (g_StartWave)    
    {
		g_StartWave = 0;
        EnQueueRecord();
        temp = temp;
    }
	
    QueueRecordToSDRAM();
}

static void rt_hw_ad7616_irq_service(void *args)
{
    read_ad7616_data();  
}

/**
  * @description: AD7616 GPIO Init.
  * @param[in]  : none
  * @return     : none
  * @updata     : [YYYY-MM-DD] [Change person name][change content]
  */
static void AD7616_GPIO_Init(void)
{
    static struct rt_device_pin *ad7616_pin;                

    ad7616_pin = (struct rt_device_pin *)rt_device_find(RT_PIN_NAME);
	
    if (ad7616_pin == RT_NULL)
    {
        AD7616_PRINTF("ad7616 pin device is not found!!! "); 
    }
    else
    {    
        /* AD7616 BUSY */
        pin_status[INDEX_AD7616_RESET].status = GPIO_PIN_RESET;			
        rt_device_write(&ad7616_pin->parent, 0, &pin_status[INDEX_AD7616_RESET], sizeof(struct rt_device_pin_status));	   
        AD7616_delay_ns(3000);
        
        ad7616_pin->ops->pin_attach_irq(&ad7616_pin->parent, 89, PIN_IRQ_MODE_FALLING, rt_hw_ad7616_irq_service, RT_NULL);  
        ad7616_pin->ops->pin_irq_enable(&ad7616_pin->parent, 89, PIN_IRQ_ENABLE, INT_EXTI15_10_PRIO); 
        
        pin_status[INDEX_AD7616_RESET].status = GPIO_PIN_SET;			
        rt_device_write(&ad7616_pin->parent, 0, &pin_status[INDEX_AD7616_RESET], sizeof(struct rt_device_pin_status));
        AD7616_delay_ns(10);
    }
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @description: AD7616 Init.
  * @param[in]  : none
  * @return     : none
  * @updata     : [YYYY-MM-DD] [Change person name][change content]
  */
int rt_hw_adc_init(void)
{
    AD7616_FSMC_Init();    
    AD7616_GPIO_Init();
    
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0002&0x003f)<<9) | (0x0060&0x01ff);//Ð´ÅäÖÃ¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0004&0x003f)<<9) | (0x0000&0x01ff);//Ð´ÊäÈë·¶Î§¼Ä´æÆ÷A1
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0005&0x003f)<<9) | (0x0000&0x01ff);//Ð´ÊäÈë·¶Î§¼Ä´æÆ÷A2
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0006&0x003f)<<9) | (0x0000&0x01ff);//Ð´ÊäÈë·¶Î§¼Ä´æÆ÷B1
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0007&0x003f)<<9) | (0x0000&0x01ff);//Ð´ÊäÈë·¶Î§¼Ä´æÆ÷B2
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0020&0x003f)<<9) | (0x0000&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0021&0x003f)<<9) | (0x0011&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0022&0x003f)<<9) | (0x0022&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0023&0x003f)<<9) | (0x0033&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0024&0x003f)<<9) | (0x0044&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0025&0x003f)<<9) | (0x0055&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0026&0x003f)<<9) | (0x0066&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0027&0x003f)<<9) | (0x0077&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0028&0x003f)<<9) | (0x0066&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0029&0x003f)<<9) | (0x0055&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002A&0x003f)<<9) | (0x0044&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002B&0x003f)<<9) | (0x0033&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002C&0x003f)<<9) | (0x0022&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002D&0x003f)<<9) | (0x0011&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002E&0x003f)<<9) | (0x0100&0x01ff);//ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    
  #if RT_USING_HWTIMER
    TIM5_PWM_Init(6250-1, 3-1); // ¼ÆÊ±1875*10 / 180Mhz=20ms/96=208.3us  37500
  #endif /* RT_USING_HWTIMER */
    
    return(0);
}
INIT_ENV_EXPORT(rt_hw_adc_init);


/* END OF FILE ---------------------------------------------------------------*/



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
static uint8_t s_Mapping[16];

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
    hsram2.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;//存储器类型
    hsram2.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;//数据宽度
    hsram2.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
    hsram2.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram2.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
    hsram2.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
    hsram2.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;//写使能
    hsram2.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
    hsram2.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;//扩展模式使能位
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
  * @updata     : [2018-9-18] [田晓亮][重新赋值]
  */
static void read_ad7616_data(void)
{
    static uint16_t s_ADC_Count;
    static short temp[16];
 
//0 
    temp[0] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
    temp[8] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
//1 
    temp[1] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
    temp[9] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
//2 
    temp[2] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
    temp[10] = (*((volatile  short *)AD7616_DATA_ADDR)); 
    //AD7616_delay_ns(1);  
//3 
    temp[3] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
    temp[11] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
//4 
    temp[4] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
    temp[12] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
//5 
    temp[5] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
    temp[13] = (*((volatile  short *)AD7616_DATA_ADDR)); 
    //AD7616_delay_ns(1);
//6 
    temp[6] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
    temp[14] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
//7 
    temp[7] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
    temp[15] = (*((volatile  short *)AD7616_DATA_ADDR));
    //AD7616_delay_ns(1);
//6 
    temp[6] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[6])/2;
    //AD7616_delay_ns(1);
    temp[14] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[14])/2; 
    //AD7616_delay_ns(1);     
    
//5 
    temp[5] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[5])/2;
    AD7616_delay_ns(1);
    temp[13] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[13])/2;
    AD7616_delay_ns(1); 
    
//4 
    temp[4] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[4])/2;
    //AD7616_delay_ns(1);
    temp[12] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[12])/2;
    //AD7616_delay_ns(1);
    
//3 
    temp[3] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[3])/2;
    //AD7616_delay_ns(1);
    temp[11] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[11])/2;
    //AD7616_delay_ns(1);
    
//2 
    temp[2] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[2])/2;
    //AD7616_delay_ns(1);
    temp[10] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[10])/2;
    //AD7616_delay_ns(1);
    
//1 
    temp[1] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[1])/2;
    //AD7616_delay_ns(1);
    temp[9] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[9])/2;
    //AD7616_delay_ns(1);
    
//0 
    temp[0] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[0])/2;
    //AD7616_delay_ns(1);
    temp[8] = ((*((volatile  short *)AD7616_DATA_ADDR)) + temp[8])/2;
    //AD7616_delay_ns(1);  
    
	g_SampleQueueBuf->ua[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[0]];
    g_SampleQueueBuf->ub[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[1]];
    g_SampleQueueBuf->uc[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[2]];
	
//    g_SampleQueueBuf->UA2[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[3]];		//更新与D03C-603模拟板接口对应
//    g_SampleQueueBuf->UB2[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[4]];		//更新与D03C-603模拟板接口对应
//    g_SampleQueueBuf->UC2[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[5]];		//更新与D03C-603模拟板接口对应
	
    g_SampleQueueBuf->u0[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[3]];
    g_SampleQueueBuf->uA[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[4]];
    g_SampleQueueBuf->uC[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[5]];
    g_SampleQueueBuf->udc1[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[6]];
    g_SampleQueueBuf->udc2[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[7]];
    
	g_SampleQueueBuf->i0[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[8]];		//更改新通道	
    g_SampleQueueBuf->ia[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[9]];
    g_SampleQueueBuf->ib[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[10]];
   	g_SampleQueueBuf->ic[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[11]];		//更改新通道
	
//    g_SampleQueueBuf->ic[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[12]];		//与外部接口不符
//    g_SampleQueueBuf->i0[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[13]];		//与外部接口不符

//    g_SampleQueueBuf->ia[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[12]];
//    g_SampleQueueBuf->ib[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[13]];
//    g_SampleQueueBuf->ic[g_SampleQueueIn][s_ADC_Count] = temp[s_Mapping[14]];		//更改新通道

    s_ADC_Count++;

    if (s_ADC_Count == 48)
    {  
        s_ADC_Count = 0;
                
        if(temp[15] == 0x5555)
        {rt_event_send(&calculate_event, EVENT_RUN);}// 采样完成
		
        if (++g_SampleQueueIn >= (ADC_WAVE_SEMP_NUM * 2))
        {
            g_SampleQueueIn = 0;
        }
    } 

    if (g_StartWave)    
    {
		g_StartWave = 0;
        EnQueueRecord();
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

/**
  * @description: AD7616 GPIO Init.
  * @param[in]  : none
  * @return     : none
  * @updata     : [2018-9-18] [田晓亮][添加模拟通道索引]
  */
static void AD7616_Mapping_Init(void)
{
    uint8_t i;
    
    for(i=0;i<g_tagzkAnalogInputCfg_Len;i++)
    {
        s_Mapping[zkAnalogInputCfg[i].pin - 1] = i;
    }
    
}
/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @description: AD7616 Init.
  * @param[in]  : none
  * @return     : none
  * @updata     : [2018-9-18] [田晓亮][改变配置序列堆栈寄存器]
  */
int rt_hw_adc_init(void)
{
    AD7616_FSMC_Init();    
    AD7616_GPIO_Init();
    AD7616_Mapping_Init();
	
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0002&0x003f)<<9) | (0x0060&0x01ff);//写配置寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0004&0x003f)<<9) | (0x0000&0x01ff);//写输入范围寄存器A1
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0005&0x003f)<<9) | (0x0000&0x01ff);//写输入范围寄存器A2
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0006&0x003f)<<9) | (0x0000&0x01ff);//写输入范围寄存器B1
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0007&0x003f)<<9) | (0x0000&0x01ff);//写输入范围寄存器B2
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0020&0x003f)<<9) | (0x0070&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0021&0x003f)<<9) | (0x0061&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0022&0x003f)<<9) | (0x0052&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0023&0x003f)<<9) | (0x0043&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0024&0x003f)<<9) | (0x0034&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0025&0x003f)<<9) | (0x0025&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0026&0x003f)<<9) | (0x0016&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0027&0x003f)<<9) | (0x00B7&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0028&0x003f)<<9) | (0x0016&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x0029&0x003f)<<9) | (0x0025&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002A&0x003f)<<9) | (0x0034&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002B&0x003f)<<9) | (0x0043&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002C&0x003f)<<9) | (0x0052&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002D&0x003f)<<9) | (0x0061&0x01ff);//配置序列器堆栈寄存器
    *((volatile  short *)AD7616_DATA_ADDR) = 0x8000 | ((0x002E&0x003f)<<9) | (0x0170&0x01ff);//配置序列器堆栈寄存器
    
  #if RT_USING_HWTIMER
    TIM5_PWM_Init(6250-1, 3-1); // 计时1875*10 / 180Mhz=20ms/96=208.3us  37500
  #endif /* RT_USING_HWTIMER */
    
    return(0);
}
INIT_ENV_EXPORT(rt_hw_adc_init);


/* END OF FILE ---------------------------------------------------------------*/



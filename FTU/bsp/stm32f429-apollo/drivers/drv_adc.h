/*
** ------------------------------- Copyright (c) -------------------------------                                     
**
**                           http://www.sojoline.com
**
** note:    Copyright SOJO. ALL RIGHTS RESERVED.
**          This software is provided under license and contains proprietary and
**          confidential material which is the property of Company Name tech. 
**    
** -------------------------------- FILE INFO ----------------------------------
** File name:               drv_adc.h
** Descriptions:            The driver of A/D device.
**
** -----------------------------------------------------------------------------
** Author:                  Mr.Lee
** Created date:            2017.07.12
** System:                  The new FTU system.
** Version:                 V1.01
** Descriptions:            The original version
**
** -----------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
** ---------------------------------------------------------------------------*/
#ifndef _DRV_ADC_H_
#define _DRV_ADC_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "rtconfig.h"
#include "rtthread.h"
#include "rtdevice.h"
#include "stm32f4xx_hal.h"


/* DEFINES -------------------------------------------------------------------*/
#define AD_ADDR_BASE              (0x68000000)

#define AD_STATE_READ             (0x1)
#define AD_STATE_INIT             (0x0)

#define AD_CMD_READ_FREQ          (0x67)
#define AD_CMD_START_SAMPLE       (0x68)
#define AD_CMD_WAIT_SAM           (0x69)
#define AD_CMD_GET_PCOUNTS        (0x70)

#define AD_BUFF_MAX_COUNT         4096


/* STRUCTS -------------------------------------------------------------------*/
struct ad7606_config
{
    rt_uint32_t ad_addr;
    rt_uint32_t sample_scal;
    rt_uint32_t single_sample_enable;
    rt_uint32_t sample_point_count;
    struct rt_ringbuffer rw_buffer[8];
    struct rt_ringbuffer pos_buffer;
};

struct ad7606_context
{
    rt_uint32_t state;
    rt_uint32_t freq;
    rt_sem_t sample_complete_sem;
    rt_uint32_t sample_counter;
};

struct ad7606_device
{
    struct ad7606_config config;
    struct ad7606_context context;
    TIM_HandleTypeDef ad_timer;
    TIM_HandleTypeDef frq_timer;
};


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
extern void rt_hw_adc_init(rt_uint32_t sample_count,rt_uint32_t single_sample);


#endif /* END _DRV_ADC_H_ */


/* END OF FILE ---------------------------------------------------------------*/


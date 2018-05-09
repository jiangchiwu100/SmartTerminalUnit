/** 
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_gpio.h
  * @brief:     The driver of GPIO,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    Lexun
  * @date:      2009-01-05
  * @update:    [2017-11-22][Lexun][The STM32F7 version(V1.0.0)]
  */
#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rthw.h>
#include <rtdevice.h>
#include <board.h>


/* DEFINE --------------------------------------------------------------------*/
#define PIN_USERDATA_END {-1,0}
#define __STM32_PIN(index, gpio, gpio_index) {index, GPIO##gpio##_CLK_ENABLE, GPIO##gpio, GPIO_PIN_##gpio_index}
#define __STM32_PIN_DEFAULT {-1, 0, 0, 0}
#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])

/* LED */
#define BOARD_RUN_LED              56
#define BOTTOM_RUN_LED             85
#define BOTTOM_POWER_LED           84
/* DO */
#define SWITCH_OPEN_DO             151
#define SWITCH_CLOSE_DO            150
#define ENERGY_STORAGE_DO          155
#define PRESET_DO                  153
#define ACTIVATE_START_DO          163
#define ACTIVATE_STOP_DO           164
#define BETTERY_DROP_OUT_DO        12
#define BACKEUP_DO                 13
/* DI */
#define SWITCH_OPEN_DI             111
#define SWITCH_CLOSE_DI            100
#define ENERGY_STORAGE_DI          117
#define LOW_PRESSURE_DI            118
#define BATTERY_LOSS_ALARM_DI      119
#define BATTERYA_CTIVATE_DI        128
#define BETTERY_UNDERVOLTAGE_DI    129
#define POWER_FAILURE_ALARM_DI     130
#define MCU_POWER_ALARM_DI         145

#define HANDHELD_OPEN_PRESET       173
#define HANDHELD_OPEN_EXECUTE      174
#define HANDHELD_CLOSE_PRESET      175
#define HANDHELD_CLOSE_EXECUTE     176

#define AD7616_RESET               88
#define WDG_FEED                   110
#define W5500_CS                   131
#define W5500_RESET                7
#define SC1161Y_CS                 24


/* ENUM ----------------------------------------------------------------------*/
enum PIN_INDEX
{
    INDEX_BOARD_RUN_LED = 0,
    INDEX_BOTTOM_RUN_LED,
    INDEX_BOTTOM_POWER_LED,
	
    INDEX_SWITCH_OPEN_DO,
    INDEX_SWITCH_CLOSE_DO,
    INDEX_ENERGY_STORAGE_DO,
    INDEX_PRESET_DO,
    INDEX_ACTIVATE_START_DO,
    INDEX_ACTIVATE_STOP_DO,
    INDEX_BETTERY_DROP_OUT_DO,
    INDEX_BACKEUP_DO,
	
    INDEX_SWITCH_CLOSE_DI,
    INDEX_SWITCH_OPEN_DI,	
    INDEX_ENERGY_STORAGE_DI,
    INDEX_LOW_PRESSURE_DI,
    INDEX_POWER_FAILURE_ALARM_DI,
    INDEX_BETTERY_UNDERVOLTAGE_DI,
    INDEX_BATTERYA_CTIVATE_DI,	
    INDEX_BATTERY_LOSS_ALARM_DI,
    INDEX_MCU_POWER_ALARM_DI,
	
    INDEX_HANDHELD_OPEN_PRESET,
    INDEX_HANDHELD_OPEN_EXECUTE,
    INDEX_HANDHELD_CLOSE_PRESET,	
    INDEX_HANDHELD_CLOSE_EXECUTE,
    INDEX_AD7616_RESET,
    INDEX_WDG_FEED,
    INDEX_W5500_CS,
    INDEX_W5500_RESET,	
    INDEX_SC1161Y_CS,
};


/* STRUCT --------------------------------------------------------------------*/
struct stm32_hw_pin_userdata
{
    int pin;
    uint32_t mode;
};

/* STM32 GPIO driver */
struct pin_index
{
    int index;
    void (*rcc)(void);
    GPIO_TypeDef *gpio;
    uint32_t pin;
};

struct pin_irq_map
{
    rt_uint16_t pinbit;
    IRQn_Type irqno;
};


/* PUBLIC VARIABLES ----------------------------------------------------------*/
extern struct rt_device_pin_mode pin_mode[];
extern struct rt_device_pin_status pin_status[];


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
int rt_hw_pin_init(void);


#endif /* __DRV_GPIO_H__ */


/* END OF FILE ---------------------------------------------------------------*/


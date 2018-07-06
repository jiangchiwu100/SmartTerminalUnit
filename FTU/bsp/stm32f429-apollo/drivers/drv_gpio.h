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

/* DO */
#define SWITCH_OPEN_DO            175
#define SWITCH_CLOSE_DO           111
#define ENERGY_STORAGE_DO         119
#define PRESET_DO                 117
#define ACTIVATE_START_DO         118
#define ACTIVATE_STOP_DO          45
#define BETTERY_DROP_OUT_DO       122
#define BACKEUP_DO                128

#define KI_CS1          57
#define KI_CS2          155
#define KI_CS3          83
#define KI_CS4          84

/* DI */
#define DI1                        145
#define DI2                        46
#define DI3                        47
#define DI4                        151
#define DI5                        50
#define DI6                        51
#define DI7                        150
#define DI8                        52

//“‘œ¬”≤±‡¬Î
#define OPEN_CLOSE_DI              176
#define ENERGY_STORAGE_DI          110
#define MCU_POWER_ALARM_DI         107
#define AD7616_RESET               88
#define WDG_FEED                   130
#define W5500_CS                   131
#define W5500_RESET                7
#define SC1161Y_CS                 24


/* ENUM ----------------------------------------------------------------------*/
enum PIN_INDEX
{
    INDEX_BOARD_RUN_LED = 0,
	
    INDEX_SWITCH_OPEN_DO,
    INDEX_SWITCH_CLOSE_DO,
    INDEX_ENERGY_STORAGE_DO,
    INDEX_PRESET_DO,
    INDEX_ACTIVATE_START_DO,
    INDEX_ACTIVATE_STOP_DO,
    INDEX_BETTERY_DROP_OUT_DO,
    INDEX_BACKEUP_DO,

    INDEX_KI_CS1,
    INDEX_KI_CS2,
    INDEX_KI_CS3,	
    INDEX_KI_CS4,
	
    INDEX_DI1,
    INDEX_DI2,	
    INDEX_DI3,
    INDEX_DI4,
    INDEX_DI5,
    INDEX_DI6,
    INDEX_DI7,	
    INDEX_DI8,
    INDEX_OPEN_CLOSE_DI,	
    INDEX_ENERGY_STORAGE_DI,

    INDEX_MCU_POWER_ALARM_DI,
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
void do_test();

#endif /* __DRV_GPIO_H__ */


/* END OF FILE ---------------------------------------------------------------*/


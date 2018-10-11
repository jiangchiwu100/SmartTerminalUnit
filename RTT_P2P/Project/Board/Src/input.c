/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      input.c
  * @brief:     The driver of input.
  * @version:   V03.001
  * @author:    tianxiaoliang
  * @date:      2018-09-17
  * @updata:   
  */

  
#include "input.h"
#include "drv_gpio.h"
#include "string.h"
#include "common_data.h"
#include "point_table_config.h"


static rt_device_t rt_input_dev;
SwitchProperty curStation;

/**
  * @brief : input 初始化
  * @param : [none].
  * @return: [0] 
  * @updata: [2018-09-17][tianxiaoliang][创建]
  */  
int rt_hw_input_init(void)
{
    rt_input_dev = rt_device_find(RT_PIN_NAME);	
    
    if (rt_input_dev == RT_NULL)
    {
        INPUT_PRINTF("input pin device is not found !!!"); 
    }
    else
    {
        INPUT_PRINTF("input pin device is found ~~~"); 
    }
		
    return (0);
}
INIT_DEVICE_EXPORT(rt_hw_input_init);

/**
  * @brief : 分合位状态
  * @param : [none].
  * @return: [0] 
  * @updata: [2018-09-17][tianxiaoliang][创建]
  */ 
void OpeningClosing(void)
{
	if(g_TelesignalDB[g_TelesignalAddr.switchOpen]==ON  &&  g_TelesignalDB[g_TelesignalAddr.switchClose]==OFF)
	{
		curStation.state = SWITCH_OPEN;
	}
	else if(g_TelesignalDB[g_TelesignalAddr.switchOpen]==OFF && g_TelesignalDB[g_TelesignalAddr.switchClose]==ON)
	{
		curStation.state = SWITCH_CLOSE;
	}
	else
	{
        curStation.state = 0;
	}
}

/**
  * @brief : 开入信息读取
  * @param : [none].
  * @return: [0] 
  * @updata: [2018-09-17][tianxiaoliang][创建]
  */ 
void rt_hw_input_check_task(rt_uint8_t clock)
{
	uint8_t i = 0;
	static float s_shaking_time = 0;
//	float *tmp = &s_shaking_time;

	s_shaking_time = (uint32_t)g_Parameter[DI_SHAKING_TIME] / clock;

	pin_status[INDEX_KI_CS1].status = GPIO_PIN_RESET;
	rt_device_write(rt_input_dev, 0, &pin_status[INDEX_KI_CS1], sizeof(struct rt_device_pin_status));//使能片选1
	
	for (i = 0; i < INPUT_NUM; i++)
    {
        rt_device_read(rt_input_dev, 0, &pin_status[INDEX_DI1 + i], sizeof(struct rt_device_pin_status));	
		if(zkDigitalInputCfg[i].isNegated)
		{
			pin_status[INDEX_DI1 + i].status = (~pin_status[INDEX_DI1 + i].status)&0x00000001;
		}
    }
	
	pin_status[INDEX_KI_CS1].status = GPIO_PIN_SET;			
	rt_device_write(rt_input_dev, 0, &pin_status[INDEX_KI_CS1], sizeof(struct rt_device_pin_status));//失能片选1
	
	for (i = 0; i < INPUT_NUM; i++)
    {
//        if (pin_status[INDEX_DI1 + i].status == zkDigitalInputCfg[i].lastVal)//判断最后一次赋值
//        {
//            zkDigitalInputCfg[i].count = 0;
//        }
//        else if(zkDigitalInputCfg[i].enable)
//        {
            zkDigitalInputCfg[i].count++;

            if (zkDigitalInputCfg[i].count >= s_shaking_time)				//开入防抖时间
            {
                zkDigitalInputCfg[i].count = 0;			
				
                g_TelesignalDB[*(zkDigitalInputCfg[i].pAddr)]=(pin_status[INDEX_DI1 + i].status ? OFF : ON);//赋值开入状态
                
                zkDigitalInputCfg[i].lastVal = pin_status[INDEX_DI1 + i].status;
            }
//        }
    }

/*---------------------------------------第2片---------------------------------------------*/
	pin_status[INDEX_KI_CS2].status = GPIO_PIN_RESET;			
	rt_device_write(rt_input_dev, 0, &pin_status[INDEX_KI_CS2], sizeof(struct rt_device_pin_status));//使能片选2		

    for (i = 0; i < INPUT_NUM; i++)
    {
        rt_device_read(rt_input_dev, 0, &pin_status[INDEX_DI1 + i], sizeof(struct rt_device_pin_status));	
		if(zkDigitalInputCfg[i + INPUT_NUM].isNegated)
		{
			pin_status[INDEX_DI1 + i].status = (~pin_status[INDEX_DI1 + i].status)&0x00000001;
		}
    }

	pin_status[INDEX_KI_CS2].status = GPIO_PIN_SET;			
	rt_device_write(rt_input_dev, 0, &pin_status[INDEX_KI_CS2], sizeof(struct rt_device_pin_status));//失能片选2	

    for (i = 0; i < INPUT_NUM; i++)
    {
        if (pin_status[INDEX_DI1 + i].status == zkDigitalInputCfg[i + INPUT_NUM].lastVal)
        {
            zkDigitalInputCfg[i + INPUT_NUM].count = 0;
        }
        else if(zkDigitalInputCfg[i + INPUT_NUM].enable)
        {
            zkDigitalInputCfg[i + INPUT_NUM].count++;

            if (zkDigitalInputCfg[i + INPUT_NUM].count >= s_shaking_time)
            {
                zkDigitalInputCfg[i + INPUT_NUM].count = 0;			
				
                g_TelesignalDB[*(zkDigitalInputCfg[i + INPUT_NUM].pAddr)]=(pin_status[INDEX_DI1 + i].status ? OFF : ON);
                
                zkDigitalInputCfg[i + INPUT_NUM].lastVal = pin_status[INDEX_DI1 + i].status;
            }
        }
    }	
    
    rt_device_read(rt_input_dev, 0, &pin_status[INDEX_MCU_POWER_ALARM_DI], sizeof(struct rt_device_pin_status));
//    DBWriteSOE(g_TelesignalAddr.devicePowerDown, (pin_status[INDEX_MCU_POWER_ALARM_DI].status) ? ON : OFF);
	
}



/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      sc1161y.h
  * @brief:     The driver of sc1161y.
  * @version:   V03.001 
  * @author:    Lexun
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
#ifndef  __DRV_SCL161Y_H__
#define  __DRV_SCL161Y_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <stdint.h>
#include <rtthread.h>


/* DEFINE --------------------------------------------------------------------*/
#ifdef DEBUG
#define SCL161Y_PRINTF(...)    rt_kprintf(__VA__ARGS__)
#else
#define SCL161Y_PRINTF(...)
#endif /* DEBUG */


/* STRUCT --------------------------------------------------------------------*/	
struct spi_sc1161y_device
{
    struct rt_device                sc1161y_device;
    struct rt_spi_device *          rt_spi_device;
    struct rt_mutex                 lock;
    void *                          user_data;
};

typedef struct spi_sc1161y_device *rt_spi_sc1161y_device_t;


/* PUBLIC FUNCTION -----------------------------------------------------------*/
extern void sc1161y_ReadWrite(unsigned char *TxData , unsigned char *RxData);


#endif /* END __DRV_W5500_H__ */ 


/* END OF FILE ---------------------------------------------------------------*/


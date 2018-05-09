/**
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_sdram.h
  * @brief:     The driver of SDRAM,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    Lexun
  * @date:      2009-01-05
  * @update:    [2017-11-22][Lexun][make the code clean up]
  */
#ifndef __DRV_SDRAM_H__
#define __DRV_SDRAM_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include <board.h>


/* DEFINES -------------------------------------------------------------------*/
#define SDRAM_BANK_ADDR                          ((uint32_t)0XC0600000)

/* #define SDRAM_MEMORY_WIDTH                    FMC_SDRAM_MEM_BUS_WIDTH_8 */
#define SDRAM_MEMORY_WIDTH                       FMC_SDRAM_MEM_BUS_WIDTH_16

#define SDCLOCK_PERIOD                           FMC_SDRAM_CLOCK_PERIOD_2 
/* #define SDCLOCK_PERIOD                        FMC_SDRAM_CLOCK_PERIOD_3 */

#define SDRAM_TIMEOUT                            ((uint32_t)0xFFFF)

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

#define BUFFER_SIZE                              ((uint32_t)0x0100)
#define WRITE_READ_ADDR                          ((uint32_t)0x0800)
#define REFRESH_COUNT                            ((uint32_t)0x02AB)   // SDRAM refresh counter (90MHz SD clock)


/* PUBLIC FUNCTION -----------------------------------------------------------*/
int rt_hw_sdram_init(void);


#ifdef __cplusplus
}
#endif

#endif /* __DRV_SDRAM_H__ */


/* END OF FILE ---------------------------------------------------------------*/


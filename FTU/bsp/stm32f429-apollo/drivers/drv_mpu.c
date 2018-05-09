/**
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_mpu.c
  * @brief:     The driver of MPU,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    Lexun
  * @date:      2009-01-05
  * @update:    [2018-02-02][Lexun][make the code clean up]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include "drv_mpu.h"
#include "stm32f4xx_hal.h"


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : the mpu hardware inialization.
  * @param : None.
	* @return: None. 
  */  
int rt_hw_mpu_init(void)
{
    /* Disable MPU */
    MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;

    /* - Region 0: 0x00000000 - 0x0007FFFF --- on-chip non-volatile memory
    *      + Size: 512kB
    *      + Acess permission: full access
    */
    MPU->RNR = 0;//indicate MPU region 0
    MPU->RBAR = 0x00000000; // update the base address for the region 0
    MPU->RASR = MPU_RASR_ACCESS_PERMISSION(MPU_FULL_ACCESS)     //full access
                | MPU_RASR_REGION_SIZE(MPU_REGION_SIZE_1MB)     //512Kb size
                | MPU_REGION_ENABLE;                            //region enable

    /* - Region 1:0x20000000 - 0x20007FFF --- on chip SRAM
    *      + Size: 32kB
    *      + Access permission: full access
    */
    MPU->RNR = 1;
    MPU->RBAR = 0x20000000; // update the base address for the region 5
    MPU->RASR = MPU_RASR_ACCESS_PERMISSION(MPU_FULL_ACCESS)
                | MPU_RASR_REGION_SIZE(MPU_REGION_SIZE_256KB)
                | MPU_REGION_ENABLE;

    /* - Region 2: 0x40000000 - 0x400FFFFF --- APB peripheral
    *      + Size: 1MB
    *      + Access permission: full access
    */
    MPU->RNR = 2;
    MPU->RBAR = 0x40000000; // update the base address for the region 2
    MPU->RASR = MPU_RASR_ACCESS_PERMISSION(MPU_FULL_ACCESS)
                | MPU_RASR_REGION_SIZE(MPU_REGION_SIZE_128KB)
                | MPU_REGION_ENABLE;

    /* - Region 3: 0x20080000 - 0x200BFFFF --- AHB1 peripheral
    *      + Size: 256KB
    *      + AP=b011: full access
    */
    MPU->RNR = 3;
    MPU->RBAR = 0x40020000; // update the base address for the region 3
    MPU->RASR = MPU_RASR_ACCESS_PERMISSION(MPU_FULL_ACCESS)
                | MPU_RASR_REGION_SIZE(MPU_REGION_SIZE_512KB)
                | MPU_REGION_ENABLE;

    /* - Region 4: 0xE0000000 - 0xE00FFFFF --- System control
    *      + Size: 1MB
    *      + Access permission: full access
    */
    MPU->RNR = 4;
    MPU->RBAR = 0xE0000000; // update the base address for the region 4
    MPU->RASR = MPU_RASR_ACCESS_PERMISSION(MPU_FULL_ACCESS)
                | MPU_RASR_REGION_SIZE(MPU_REGION_SIZE_1MB)
                | MPU_REGION_ENABLE;

    /* - Region 5:0xA0000000 - 0xA2000000 --- external SDRAM
    *      + Size: 32MB
    *      + Access permission: full access
    */
    MPU->RNR = 6;
    MPU->RBAR = 0xC0000000; // update the base address for the region 6
    MPU->RASR = MPU_RASR_ACCESS_PERMISSION(MPU_FULL_ACCESS)
                | MPU_RASR_REGION_SIZE(MPU_REGION_SIZE_32MB)
                | MPU_REGION_ENABLE;

    /* Enable the memory fault exception */
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    /* Enable MPU */
    MPU->CTRL |= MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_ENABLE_Msk;
	
    return(0);
}
INIT_BOARD_EXPORT(rt_hw_mpu_init);


/* END OF FILE ---------------------------------------------------------------*/



/**
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_mpu.h
  * @brief:     The driver of MPU,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    Lexun
  * @date:      2009-01-05
  * @update:    [2018-02-02][Lexun][make the code clean up]
  */

#ifndef __DRV_MPU_H__
#define __DRV_MPU_H__


/* DEFINES -------------------------------------------------------------------*/

/* Access permission definitions */
#define MPU_NO_ACCESS                           0x00
#define MPU_PRIVILEGED_ACESS_USER_NO_ACCESS     0x01
#define MPU_PRIVILEGED_RW_USER_READ_ONLY        0x02
#define MPU_FULL_ACCESS                         0x03
#define MPU_UNPREDICTABLE                       0x04
#define MPU_PRIVILEGED_READ_ONLY_USER_NO_ACCESS 0x05
#define MPU_READ_ONLY                           0x06

/* RASR bit definitions */
#define MPU_RASR_REGION_SIZE(n)                 ((uint32_t)(n<<1))
#define MPU_RASR_ACCESS_PERMISSION(n)           ((uint32_t)(n<<24))


/* PUBLIC FUNCTION -----------------------------------------------------------*/
int rt_hw_mpu_init(void); // Initialize Cortex M4 MPU


#endif /* __DRV_MPU_H__ */


/* END OF FILE ---------------------------------------------------------------*/


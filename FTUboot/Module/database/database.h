/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      databse.c
  * @brief:     
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018-07-25 
  * @update:    
  */
#ifndef __DATABASE_H
#define __DATABASE_H

#define ADDR_FRAM_DISTRIBUTION 0x0C000  //注意与common_data.h 地址不要重复
#define ADDR_FRAM_DISTRIBUTION_SIZE 0x01000  //4K

#include  <stdbool.h>

extern bool StationMessageRead(void);
#endif


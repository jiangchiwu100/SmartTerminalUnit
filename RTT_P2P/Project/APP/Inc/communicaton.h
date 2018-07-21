/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      communicaton.h
  * @brief:     
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-05-27
  * @update:    
  */
#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

extern uint8_t CommunicationServerInitSingle(void);
extern uint8_t CommunicationServerSingle(void);
extern uint8_t CommunicationServeice(void);
extern void Monitor(void);

#ifdef	__cplusplus
}
#endif

#endif

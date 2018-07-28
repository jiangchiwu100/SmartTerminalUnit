/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      config
  * @brief:     
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-07-21
  * @update:    
  */
#ifndef __DISTRIBUTION_CONFIG_H__
#define __DISTRIBUTION_CONFIG_H__

  //仿真开关
#define THREAD_SIMSWITCH_NAME                                              "SIMSW"
#define THREAD_SIMSWITCH_PRIORITY                                          (3)
#define THREAD_SIMSWITCH_STACK_SIZE                                        (1024)
#define THREAD_SIMSWITCH_TIMESLICE                                         (20)

//分布式保护逻辑
#define THREAD_DISTRIBUTION_NAME                                              "distpro"
#define THREAD_DISTRIBUTION_PRIORITY                                          (4)
#define THREAD_DISTRIBUTION_STACK_SIZE                                        (1024)
#define THREAD_DISTRIBUTION_TIMESLICE                                         (20)

//UDPserver
#define THREAD_UDPSERVER_NAME                                              "udpser"
#define THREAD_UDPSERVER_PRIORITY                                          (5)
#define THREAD_UDPSERVER_STACK_SIZE                                        (2048)
#define THREAD_UDPSERVER_TIMESLICE                                         (20)

//路由通信
#define THREAD_MUTAL_NAME                                              "mutal"
#define THREAD_MUTAL_PRIORITY                                          (13)
#define THREAD_MUTAL_STACK_SIZE                                        (2048)
#define THREAD_MUTAL_TIMESLICE                                         (20)


//联络相关逻辑
#define THREAD_CONNECT_NAME                                              "connect"
#define THREAD_CONNECT_PRIORITY                                          (15)
#define THREAD_CONNECT_STACK_SIZE                                        (1024)
#define THREAD_CONNECT_TIMESLICE                                         (20)



//单个站点
#define SINGLE_POINT  (1)
#define UDP_SEND  (1)

//广播地址，后两字段
#define BROADCAST_ADDRESS 0x0AFF //根据需要修改


#define  MONITOR_CHECK_TIME  1000// 监控发送时间
#define  MONITOR_LIFT_TIME 5000 //数据有效时间

#ifdef	__cplusplus
}
#endif

#endif

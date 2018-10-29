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
#define THREAD_SIMSWITCH_PRIORITY                                          (2)
#define THREAD_SIMSWITCH_STACK_SIZE                                        (256)
#define THREAD_SIMSWITCH_TIMESLICE                                         (20)


//分布式保护逻辑
#define THREAD_DISTRIBUTION_NAME                                              "distpro"
#define THREAD_DISTRIBUTION_PRIORITY                                          (4)
#define THREAD_DISTRIBUTION_STACK_SIZE                                        (512)
#define THREAD_DISTRIBUTION_TIMESLICE                                         (20)

  //goose接收
#define THREAD_GOOSERE_NAME                                                "goRe"
#define THREAD_GOOSERE_PRIORITY                                          (5)
#define THREAD_GOOSERE_STACK_SIZE                                        (512)
#define THREAD_GOOSERE_TIMESLICE                                         (20)

//UDPserver
#define THREAD_UDPSERVER_NAME                                              "udpser"
#define THREAD_UDPSERVER_PRIORITY                                          (2)
#define THREAD_UDPSERVER_STACK_SIZE                                        (1024*2)
#define THREAD_UDPSERVER_TIMESLICE                                         (20)

//路由通信
#define THREAD_MUTAL_NAME                                              "mutal"
#define THREAD_MUTAL_PRIORITY                                          (13)
#define THREAD_MUTAL_STACK_SIZE                                        (128)
#define THREAD_MUTAL_TIMESLICE                                         (20)


//联络相关逻辑
#define THREAD_CONNECT_NAME                                              "connect"
#define THREAD_CONNECT_PRIORITY                                          (15)
#define THREAD_CONNECT_STACK_SIZE                                        (1024)
#define THREAD_CONNECT_TIMESLICE                                         (20)

//61850
#define THREAD_61850_NAME                                              "61850"
#define THREAD_61850_PRIORITY                                          (14)
#define THREAD_61850_STACK_SIZE                                        (1024*16)
#define THREAD_61850_TIMESLICE                                         (20)





#define NET_FINSH_THREAD_NAME                                           "net_fn"
#define NET_FINSH_THREAD_PRIORITY                                       (19)
#define NET_FINSH_THREAD_STACK_SIZE                                     (4096)
#define NET_FINSH_THREAD_TIMESLICE                                      (20)

#define UDP_SERVE_THREAD_NAME                                           "udp_ser"
#define UDP_SERVE_THREAD_PRIORITY                                       (25)
#define UDP_SERVE_THREAD_STACK_SIZE                                     (1024*16)
#define UDP_SERVE_THREAD_TIMESLICE                                      (20)

#define MAINTENANCE_SERVE_THREAD_NAME                                   "m_ser"
#define MAINTENANCE_SERVE_THREAD_PRIORITY                               (22)
#define MAINTENANCE_SERVE_THREAD_STACK_SIZE                             (1024*32)
#define MAINTENANCE_SERVE_THREAD_TIMESLICE                              (20)



//单个站点
#define SINGLE_POINT  (1)
#define UDP_SEND  (1)

//广播地址，后两字段
#define BROADCAST_ADDRESS 0x0AFF //根据需要修改


#define  MONITOR_CHECK_TIME  1000// 监控发送时间
#define  MONITOR_LIFT_TIME 5000 //数据有效时间
#define  ONLINE_LIFT_TIME 3000 //数据有效时间


#define ENABLE_GOOSE   1


#define RT_USING_NET_FINSH                                             (1) // UDP实现通讯
#define RT_USING_UDP_SERVE                                             (1) // UDP server


#ifdef	__cplusplus
}
#endif

#endif

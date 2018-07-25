/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distribution_app.c
  * @brief:     分布式应用总入口
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018/7/21 
  * @update:    
  */

#include "distribution.h"
#include "distribution_app.h"
#include "extern_interface.h"
#include "distribution_config.h"
#include "database.h"



extern  void DistributionLogicalAppInit(void);
extern  void DistributionMutalAppInit(void);
extern  void LogicalSimulationAppInit(void);
extern  void UdpServerAppInit(void);

/**
* @brief  : 分布式初始化
* @param  : void
* @update: [2018-07-][张宇飞][]
*/
void DistributionAppInit(void)
{

	LogInit(&g_Loghandle);
	
	StationMangerInit(&g_StationManger);
    //先读取数据
    bool state = StationMessageRead(&g_StationManger);

    UdpServerAppInit();
    //等待udp初始化成功
    while (!g_StationManger.isMaintanceRun)
    {
        rt_thread_delay(100);
    }
     //再判断
    if (state && g_StationManger.pWorkPoint)
    {
        PrintTopologyMessage(g_StationManger.pWorkPoint->topology.localTopology);
        g_StationManger.firstRun = true;
        StartSinglePointNormalThread();
    }
   
	rt_kprintf("\n DATA:%s,%s, %s, %d\n", __DATE__, __TIME__, __FUNCTION__, __LINE__);
	// TestListPrevCase();
	ErrorCode error = RouterDatagram_NewTransferNode(LOCAL_ADDRESS, 100, &g_VirtualNode);      
	if (error)
	{
		perror("RouterDatagram_NewTransferNode:0x%x", error);
	}
    
	// RingQueueTest();
	LogAddException(ERROR_OK_NULL, 0);

#if !SINGLE_POINT
    
    LogicalSimulationAppInit();
	DistributionMutalAppInit();
	DistributionLogicalAppInit();
#endif
    
   
}

/**
* @brief  : 开始其它任务
* @param  : 队列句柄
* @param  ：RingQueue* ring 环形队列
* @param  ：PointUint8* pPacket
* @return: ErrorCode
* @update: [2018-07-23][张宇飞][]
*/
void StartSinglePointNormalThread(void)
{
    LogicalSimulationAppInit();
	
	DistributionLogicalAppInit();
}

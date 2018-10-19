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
#include "w5500_server.h"
#include "NetFinshThread.h"

extern  void DistributionLogicalAppInit(void);
extern  void DistributionMutalAppInit(void);
extern  void LogicalSimulationAppInit(void);


/**
* @brief  : 分布式数据部分初始化
* @param  : void
* @update: [2018-07-31][张宇飞][]
*/
void DistributionDatabaseInit(void)
{
    LogInit(&g_Loghandle);
	
	StationMangerInit(&g_StationManger);
    //先读取数据
    bool state = StationMessageRead(&g_StationManger);
     //再判断
   
}

/**
  * @brief :等待进入条件满足
  * @param : void
  * @return: 0--
  * @update: [2018-08-15][张宇飞][创建]
  */
void WaitEnterCodition(void)
{
	//等待工作点初始化
    extern StationManger g_StationManger;
	do
	{
		if (g_StationManger.pWorkPoint)
		{
			break;
		}
		rt_thread_delay(5000);
		printf("61850 waitting: g_StationManger.pWorkPoint is null\r\n");
	}
	while(true);

	do
	{
		if (g_StationManger.pWorkPoint->topology.areaID.isGainComplted)
		{
			break;
		}
		rt_thread_delay(3000);
		printf("61850 waitting: isGainComplted false\r\n");
	}
	while(true);
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
   // LogicalSimulationAppInit();
	
	DistributionLogicalAppInit();
}



/**
* @brief  : 分布式初始化
* @param  : void
* @update: [2018-07-][张宇飞][]
*/
void DistributionAppInit(void)
{

	 
    DistributionDatabaseInit();
    EmmedNetInit();
    
    
    rt_kprintf("\n\n\n DATA:%s, %s, %s, %d\n\n ", __DATE__, __TIME__, __FUNCTION__, __LINE__);
   
    // UdpServerAppInit();
	// TestListPrevCase();
    DP83848_MaintenanceServiceInit(NULL);
    rt_UDP_CommunicateServe_thread_start();
	ErrorCode error = RouterDatagram_NewTransferNode(LOCAL_ADDRESS, 100, &g_VirtualNode);      
	if (error)
	{
		perror("RouterDatagram_NewTransferNode:0x%x", error);
	}
    
	// RingQueueTest();
	LogAddException(ERROR_OK_NULL, 0);

    if ( g_StationManger.pWorkPoint)
    {
        PrintTopologyMessage(g_StationManger.pWorkPoint->topology.localTopology);
        g_StationManger.firstRun = true;
        StartSinglePointNormalThread();
    }
    
#if !SINGLE_POINT
    
    LogicalSimulationAppInit();
	DistributionMutalAppInit();
	DistributionLogicalAppInit();
#endif
    
   
}


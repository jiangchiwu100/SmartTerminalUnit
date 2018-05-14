/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_HMISLAVE_config.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */
#ifndef	_HMI_101_CONFIG_H_
#define _HMI_101_CONFIG_H_

/* SPECIAL VARIABLE -----------------------------------------------------------*/
#define _DLT634_HMISLAVE_E5  0   //针对无数据确认回复E5

/* PUBLIC VARIABLE -----------------------------------------------------------*/
enum
{
    DLT634_HMISLAVE_DISK0 = 0,
    _DLT634_HMISLAVE_VOLUMES
};
/* Number of volumes (logical drives) to be used.*/
#define _DLT634_HMISLAVE_LPDUSIZE  1024

#define  _DLT634_HMISLAVE_NUMOF_MAXRETRY                0x03 // 默认DL/T634.HMI_2002规约链路建立最多连接次数

//LINK_FLAG
#define  _DLT634_HMISLAVE_REQSTATUS                  0x00000001
#define  _DLT634_HMISLAVE_RECONFIRM                  0x00000002
#define  _DLT634_HMISLAVE_ASKSTATUS                  0x00000004
#define  _DLT634_HMISLAVE_ASKRELINK                  0x00000008

#define  _DLT634_HMISLAVE_SENDABLE                  0x00100000// 平衡

#define  _DLT634_HMISLAVE_FIRST_RlaConCode         	0x40000000
#define  _DLT634_HMISLAVE_INITEND                  	0x80000000

//1级数据状?
#define  _DLT634_HMISLAVE_CALLALLDATA                0x00000010
#define  _DLT634_HMISLAVE_HAVECOS                    0x00000040

/*************************************************************************************************************************************/
//DL/T634.HMI_2002规约

/* 链路层帧修饰词***************************************************************/
#define  _DLT634_HMISLAVE_STARTCODE10	                     0x10	// 起始码
#define  _DLT634_HMISLAVE_STARTCODE68	                     0x68	// 起始码
#define  _DLT634_HMISLAVE_ENDCODE                           0x16	// 终止码

/* 链路层控制域定义************************************************************/
//前4位
#define  _DLT634_HMISLAVE_FUNCODE                       0x0F // 功能码
#define  _DLT634_HMISLAVE_FCV                           0x10 // 帧计数有效位 
#define  _DLT634_HMISLAVE_DFC                           0x10 // 数据流控制
#define  _DLT634_HMISLAVE_FCB                           0x20 // 帧计数位
#define  _DLT634_HMISLAVE_ACD                           0x20 // 要求访问位 
#define  _DLT634_HMISLAVE_PRM                           0x40 // 启动报文位
#define  _DLT634_HMISLAVE_DIR                           0x80 // 传输方向位

//链路层主站向子站传输功能码定义
#define   _DLT634_HMISLAVE_M_FUN0             	0           // 复位远方链路
#define   _DLT634_HMISLAVE_M_FUN2              2           // 测试链路功能 
#define   _DLT634_HMISLAVE_M_FUN3              3	          // 发送确认命令
#define   _DLT634_HMISLAVE_M_FUN4             	4	          // 发送不确认命令
#define   _DLT634_HMISLAVE_M_FUN8              8	          // 请求响应确定访问状态
#define   _DLT634_HMISLAVE_M_FUN9             	9	          // 召唤链路状态
#define   _DLT634_HMISLAVE_M_FUN10             10	        // 召唤1级用户数据 
#define   _DLT634_HMISLAVE_M_FUN11             11          // 召唤2级用户数据

//*链路层子站向主站传输功能码定义 
#define   _DLT634_HMISLAVE_S_FUN0              0            // 确认
#define   _DLT634_HMISLAVE_S_FUN1              1            // 确认链路忙未接收报文
#define   _DLT634_HMISLAVE_S_FUN8              8            // 以数据响应请求帧
#define   _DLT634_HMISLAVE_S_FUN9              9            // 无所召唤的数据
#define   _DLT634_HMISLAVE_S_FUN11             11           // 响应链路状态或回答请求帧
 
/* TI类型标识 ********************************************************************************/ 
//监视方向过程信息
#define   _DLT634_HMISLAVE_M_SP_NA_1              1            // 灯单点信息
#define   _DLT634_HMISLAVE_M_DP_NA_1              3            // 按键单点信息
#define   _DLT634_HMISLAVE_M_SP_TB_1              30           // 灯带变位的单点信息
#define   _DLT634_HMISLAVE_M_DP_TB_1              31           // 按键带变位的单点信息
#define   _DLT634_HMISLAVE_M_HMI_NA_1             42           // HMI信息

/* COT传送原因 ********************************************************************************/ 
#define   _DLT634_HMISLAVE_COT_CYC              1            // 周期循环
#define   _DLT634_HMISLAVE_COT_BACK             2            // 背景扫描
#define   _DLT634_HMISLAVE_COT_SPONT            3            // 突发
#define   _DLT634_HMISLAVE_COT_INIT             4            // 初始化
#define   _DLT634_HMISLAVE_COT_REQ              5            // 请求或被请求
#define   _DLT634_HMISLAVE_COT_ACT              6            // 激活
#define   _DLT634_HMISLAVE_COT_ACTCON           7            // 激活确认
#define   _DLT634_HMISLAVE_COT_DEACT            8            // 停止激活
#define   _DLT634_HMISLAVE_COT_DEACTCON         9            // 停止激活确认
#define   _DLT634_HMISLAVE_COT_ACCTTERM         10           // 激活终止
#define   _DLT634_HMISLAVE_COT_FILE             13           // 文件传输
#define   _DLT634_HMISLAVE_COT_INTROGEN         20           // 响应站召唤
#define   _DLT634_HMISLAVE_COT_REQCOGEN         37           // 响应电能召唤
#define   _DLT634_HMISLAVE_COT_ERR              47           // 错误

#endif /* END _HMI_101_CONFIG_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/

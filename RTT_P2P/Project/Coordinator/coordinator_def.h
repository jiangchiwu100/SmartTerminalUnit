/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Coordinator_def.h
* @brief:     定义文件
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-08-22
* @update:    
*/
#ifndef __CORDINATOR_DEF_H
#define __CORDINATOR_DEF_H

#include "common_def.h"
#include "iec61850_model.h"
#include "iec61850_server.h"
#include "goose_receiver.h"
#include "GooseParser.h"


typedef struct TagGooseCheck
{
	uint32_t (*testData)[3];
	uint32_t lastSqNum;
	uint32_t lastStNum;
	uint32_t sqNum;
	uint32_t stNum ;
	uint16_t index;

    uint32_t appId;
}GooseCheck;

typedef struct TagSequenceInformation
{
    uint32_t stNum;
	uint32_t sqNum;
    uint32_t time;
}SequenceInformation;

typedef struct TagDeviceIndicate
{
	uint16_t count;
	DataAttribute** daCollect;//数据属性合集
	uint16_t* indexTrans;//数据集对应的索引
	uint32_t id;//
	uint16_t appId;
	char* goCbRef;

	RingQueue ringCheck;
	uint32_t lastSq;
	uint32_t lastSt;
}DeviceIndicate;


/**
 * 数据订阅结构
 */
typedef struct TagDatasetSubscriber
{
	uint8_t count;
	DeviceIndicate* indicateCollect;//指示属性集关联到数据集

}DatasetSubscriber;

typedef struct TagServerModelManager{

	IedModel* model;// 模型
	GooseReceiver receiver; //订阅者
	DatasetSubscriber* dsSubscriber; //数据集订阅信息
	DeviceIndicate* localPulicDataset;  //本地发布
	IedServer server; //服务
    GooseTxMessage gooseTxMessage;  //goose发送信息
    GooseRxMessage gooseRxMessage;  //goose接收信息
}ServerModelManager;

typedef enum TagDeviceStatusCode
{
  DEVICE_IED_FAULT      =     0,  //是否为故障
  DEVICE_IED_FAULT_FORWARE =     1,   //是否正向故障
  DEVICE_IED_FAULT_REVERSE =     2,  //是否为反向故障
  DEVICE_IED_COMMUNICATION_CORRECT = 3,//是否通讯正常
  DEVICE_IED_BILATERAL_VOLTAGE = 4,//是否为双侧有压
  DEVICE_IED_ONSESIDE_VOLTAGE=     5,//是否是单侧有压
  DEVICE_IED_DISTRIBUTION_RESET = 6,//分布式是否复归
  DEVICE_IED_DISTRIBUTION_RUN = 7,//分布式是否投入
  DEVICE_IED_SWITCH_POS 			= 8,//开关位置
  DEVICE_IED_REMOVE_SUCESS = 9,//切除成功
  DEVICE_IED_REMOVE_FAILURE = 10,//切除失败
  DEVICE_IED_INSULATE_FAILURE = 11,//隔离失败
  DEVICE_IED_INSULATE_SUCESS = 12,//隔离失败
  DEVICE_IED_ACTION =      13,//动作
  DEVICE_IED_REJECT_ACTION =      14,//动作

  DEVICE_IED_ALM1_RESET =   16,//分布式复归
  DEVICE_IED_ALM2_SET   =  17,//分布式投入
  DEVICE_IED_ALM3 =     18,//是否是告警3
}DeviceStatusCode;

#endif


/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Coordinator_def.h
* @brief:     定义文件
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-08-22
* @update:    
*/


typedef struct TagDeviceIndicate
{
	uint8_t count;
	DataAttribute** daCollect;//数据属性合集
	uint8_t* indexTrans;//数据集对应的索引
}DeviceIndicate;



typedef struct TagNeighborCollect
{
	uint8_t count;
	DeviceIndicate* indicateCollect;//指示属性集
}NeighborCollect;

typedef enum TagDeviceStatusCode
{
  DEVICE_IED_FAULT      =     0,  //是否为故障
  DEVICE_IED_FAULT_FORWARE =     1,   //是否正向故障
  DEVICE_IED_FAULT_REVERSE =     2,  //是否为反向故障
  DEVICE_IED_COMMUNICATION_CORRECT = 3,//是否通讯正常
  DEVICE_IED_BILATERAL_VOLTAGE = 4,//是否为双侧有压
  DEVICE_IED_ONSESIDE_VOLTAGE=     5,//是否是单侧有压
  DEVICE_IED_DISTRIBUTION_EXIT = 6,//分布式是否投入
  DEVICE_IED_SWITCH_POS 			= 7,//开关位置
  DEVICE_IED_REMOVE_SUCESS = 8,//切除成功
  DEVICE_IED_INSULATE_SUCESS = 9,//隔离成功
  DEVICE_IED_ACTION =      10,//动作

  DEVICE_IED_ALM1 =   16,//是否是告警1
  DEVICE_IED_ALM2=     17,//是否是告警2
  DEVICE_IED_ALM3 =     18,//是否是告警3
}DeviceStatusCode;




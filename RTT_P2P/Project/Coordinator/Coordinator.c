/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Coordinator.c
* @brief:     协调器文件，本文件主要用于补充分布式的集中控制怿
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-08-22
* @update:    
*/

#include "common_def.h"
#include "distribution_def.h"
#include "iec61850_model.h"
#include "goose_subscriber.h"

#include "lib_memory.h"
#include "Coordinator.h"
#include "goose_receiver.h"
#include "iec61850_server.h"

/**
* @brief :初始化数据指示空闿
* @param : uint8_t neighbourCount 邻居数量
* @param : uint8_t daCount 数据集数釿
* @return: DatasetSubscriber* 分配 好的空间
* @update: [2018-08-22][张宇飞][创建]
*/
bool DeviceIndicate_init(DeviceIndicate* ic , uint16_t daCount)
{
	if (!ic || daCount==0)
	{
		return false;
	}
	ic->count = daCount;
	ic->daCollect = (DataAttribute**)GLOBAL_CALLOC(sizeof(DataAttribute*), daCount);
	ic->indexTrans = (uint16_t*)GLOBAL_CALLOC(sizeof(uint8_t), daCount);//转换索引数组
	if ((!ic->daCollect)
		|| (!ic->indexTrans))
	{
		return false;
	}
	for (uint8_t k = 0; k < daCount; k++)
	{
		ic->indexTrans[k] = k;
	}
	return true;
}

/**
* @brief : 创建设备指示，分配内存空闿
* @param : DeviceIndicate*  self 开关属性指钿
* @return: 指针
* @update: [2018-08-23][张宇飞][创建]
*/
DeviceIndicate* DeviceIndicate_crate(uint16_t daCount)
{
	DeviceIndicate* ic = (DeviceIndicate*)GLOBAL_CALLOC(sizeof(DeviceIndicate), 1);
	if (!ic)
	{
		return NULL;
	}
	bool result = DeviceIndicate_init(ic , daCount);
	if (result)
	{
		return ic;
	}
	else
	{
		return NULL;
	}
}

/**
* @brief :创建邻居合集
* @param : uint8_t neighbourCount 邻居数量
* @param : uint8_t daCount 数据集数釿
* @return: DatasetSubscriber* 分配 好的空间
* @update: [2018-08-21][张宇飞][创建]
*[2018-08-22][张宇飞][引入中间变量]
*/
bool DeviceIndicate_initValue(DeviceIndicate* di, LogicalDevice* ld, char** ref, uint16_t* index, uint16_t daCount)
{

	if ((!di) || (!ld) || (!ref) || (!daCount))
	{
		printf("Error Input, (!ld) || (!ref) || (!daCount)\n");
		return false;
	}
	DataAttribute* da;
	for(uint16_t i = 0; i < daCount; i++ )
	{
		da =  (DataAttribute*)ModelNode_getChild((ModelNode*)ld, ref[i]);
		if(!da)
		{
			printf("Error Unfind. ref: %s\n", *ref);
			continue;
		}
		di->daCollect[i] = da;
		di->indexTrans[i] = index[i];
	}
	return true;
}
/**
* @brief :创建邻居合集
* @param : uint8_t count 订阅集合数量
* @param : uint8_t daCount 每个集合订阅的数量
* @return: DatasetSubscriber* 分配 好的空间
* @update: [2018-08-21][张宇飞][创建]
*[2018-08-22][张宇飞][引入中间变量]
*/
DatasetSubscriber* DatasetSubscriber_create(uint8_t count, uint16_t* daCount)
{
	DatasetSubscriber* nc = (DatasetSubscriber*)GLOBAL_CALLOC(sizeof(DatasetSubscriber), 1);
	if (!nc)
	{
		return NULL;
	}
	nc->count = count;

	nc->indicateCollect = (DeviceIndicate*)GLOBAL_CALLOC(sizeof(DeviceIndicate), count);
	if (!nc->indicateCollect)
	{
		return NULL;
	}
	DeviceIndicate* ic;
	for (uint16_t i = 0; i< count; i++)
	{
		ic = &(nc->indicateCollect[i]);
		bool result = DeviceIndicate_init(ic, daCount[i]);
		if (!result)
		{
			return NULL;
		}
	}

	return nc;
}


/**
* @brief :MMS数据集转换为指定的属性集
* @param : const MmsValue* self
* @param : DeviceIndicate* deviceIndicat 设备指示
* @return: void
* @update: [2018-08-22][张宇飞][创建]
*/
void MmsDatasetToDataAtrributeSet(const MmsValue* self, DeviceIndicate* deviceIndicate)
{
	if (!self || !deviceIndicate)
	{
		printf("!self || !deviceIndicate)\n");
		return;
	}

	char buffer[1024];
	switch (MmsValue_getType(self))
	{
	case MMS_STRUCTURE:
	case MMS_ARRAY:
	{
		int arraySize = MmsValue_getArraySize(self);

		for (uint8_t k = 0; k < deviceIndicate->count; k++)
		{
			MmsValue* value = (MmsValue*)MmsValue_getElement(self, deviceIndicate->indexTrans[k]);
			DataAttribute* da = deviceIndicate->daCollect[k];
			if (MmsValue_getType(da->mmsValue) == MmsValue_getType(value))
			{
				//da->mmsValue = MmsValue_clone(value);
				//MmsValue_delete(value);
				MmsValue_setBoolean(da->mmsValue, MmsValue_getBoolean(value));


				//MmsValue_printToBuffer(da->mmsValue, buffer, 1024);
				//printf(Ref2[i]);
				//printf(":%s\n", buffer);
			}
			else
			{
				printf("type error!\n");
			}
		}
		//更新对应数据
		GooseSubscriberUpdateSwitchStatus(deviceIndicate);
		break;
	}
	}

}




/**
* @brief : 获取设备状态
* @param : DeviceIndicate*  self 开关属性指钿
* @return: 是否有故障
* @update: [2018-08-22][张宇飞][创建]
*/
bool DeviceIndicate_getBooleanStatus(DeviceIndicate* self, DeviceStatusCode code)
{
	return MmsValue_getBoolean(self->daCollect[self->indexTrans[code]]->mmsValue);
}
/**
* @brief : 设置设备状态
* @param : DeviceIndicate*  self 开关属性指钿
* @return: 是否有故障
* @update: [2018-08-23][张宇飞][创建]
*/
void DeviceIndicate_setBooleanStatus(DeviceIndicate* self, DeviceStatusCode code, bool state)
{
	 MmsValue_setBoolean(self->daCollect[self->indexTrans[code]]->mmsValue, state);
}



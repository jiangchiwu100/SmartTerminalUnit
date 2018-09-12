/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      databse.c
  * @brief:     
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018-07-25 用于数据存储使用
  * @update:    
  */
  
#include "database.h"

#include "drv_fm25vxx.h"
#include "systemconfig.h"


#include "station.pb.h"
#include "pb_decode.h"
#include "extern_interface.h"
#include "config.h"
/*
FRAM区域规划
1）功能索引区域
(1)序号(1byte) + 偏移地址(2bytes)+ 长度 (2bytes)
......最多16组

2）实际数据区域
(1) 

【以上实际比较复杂，很不灵活！】
*/

/**
简单处理
(1)标识符 0xA55A(2bytes)
(2) 长度  (2bytes)
(3)有效数据区域
*/
#define SAVE_FLAG_TAG1  0xA5
#define SAVE_FLAG_TAG2  0x5A


 static void W5500_SetDefaultNetInfo(StationMessage* message,  CONFIG_MSG*  config);
/**
* @brief :从FRAM读取数据
* @param  uint8_t* pdata
* @param  输入的最多数据长度，如读取成功改写为实际长度
* @return: 0--正
* @update: [2018-07-30][张宇飞][创建]
*/
static uint8_t rt_device_read(uint8_t dev,
                         uint32_t    pos,
                         void       *buffer,
                         uint32_t   size)
{
    FramReadDate(pos, size, buffer);
    return 0;
}





/**
* @brief :配置值读取,拓扑信息
* @param  uint8_t* pdata
* @param  输入的最多数据长度，如读取成功改写为实际长度
* @return: 0--正
* @update: [2018-07-30][张宇飞][创建]
*/
bool FramRead(uint8_t* pdata, uint16_t* pDataLen)
{
   uint8_t deviceFram = NULL; 

//    rt_err_t resultOpen = rt_device_open(deviceFram, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);
//    if (resultOpen != RT_EOK)
//    {
//        perror("deviceFram open failure.\n");
//        return false;
//    }
    uint32_t addr = 0;
    uint8_t data[2] = {0};
    
    //读标志
    data[0] = 0;
    data[1] = 0;
    addr = ADDR_FRAM_DISTRIBUTION;
    uint8_t result = rt_device_read(deviceFram, addr, data, 2);
    if (0 != result)
    {
        perror("result = rt_device_read(), ERROR:%d\n", result);
        return false;
    }
    if ((data[0] != SAVE_FLAG_TAG1) || (data[1] != SAVE_FLAG_TAG2))
    {
         perror("(data[0] != SAVE_FLAG_TAG1) || (data[1] != SAVE_FLAG_TAG2)\n");
        return false;
    }
    //读长度
   
    addr += 2;   
  
    
   
    data[0] = 0;
    data[1] = 0;
    result = rt_device_read(deviceFram, addr, data, 2);
    if (0 != result)
    {
        perror("result = rt_device_read(), ERROR:%d\n", result);
        return false;
    }
    uint16_t dataLen = COMBINE_UINT16(data[1], data[0]);
    
    if ((dataLen > *pDataLen) || (dataLen > ADDR_FRAM_DISTRIBUTION_SIZE))
    {
        perror("Over Limit Max Length!\n");
        return false;
    }
    *pDataLen = dataLen;
    
    addr += 2;
    //读有效数据
    result = rt_device_read(deviceFram, addr, pdata, dataLen);
    if (0 != result)
    {
        perror("result = rt_device_read(), ERROR:%d\n", result);
        return false;
    }
    

    return true;
}



/**
* @brief :解码所有站点信息
* @param ：StationMessage message = StationMessage_init_zero; 初始化后的StationMessage
* @param ：uint8_t* data
* @param ：uint16_t len 长度
* @update: [2018-07-25][张宇飞][]
*/
static ErrorCode PacketDecodeStationMessage_ALL(StationMessage* pMessage, uint8_t* data, uint16_t len)
{

	uint16_t nanolen = COMBINE_UINT16(data[2], data[1]);
	if (nanolen + 2 > len)
	{
		return ERROR_OVER_LIMIT;
	}
	
	pb_istream_t instreamLog = pb_istream_from_buffer(data + 3, nanolen);
	bool status = pb_decode(&instreamLog, StationMessage_fields, pMessage);
	if (!status)
	{
		return ERROR_ENCODE;
	}

	
	return ERROR_OK_NULL;
}

/**
* @brief :站点信息读取
* @param ： const TopologyMessage* topology 拓扑信息
* @return: bool
* @update: [2018-07-30][张宇飞][创建]
*/
bool StationMessageRead(void)
{
    extern CONFIG_MSG  ConfigMsg;
    PointUint8 packet;
    
    packet.len = 512;
    uint8_t data[512];
    packet.pData = data;
    if (packet.pData == NULL)
    {
        perror("CALLOC Failure\n");
        return false;
    }
    
    bool state = FramRead(packet.pData, &packet.len);
    if ((!state) || (packet.len == 0) )
    {
       perror("Read Failure.\n");
       return false;      
    }
    StationMessage message = StationMessage_init_zero;
	//反序列化生成拓扑信息
	ErrorCode error = PacketDecodeStationMessage_ALL(&message, packet.pData + 1, packet.len - 1);
   
    
	if (error != ERROR_OK_NULL)
	{
		perror("PacketDecodeStationMessage_ALL ERROR : 0x%X\n", error);
		return false;
	}
    else
    {
        W5500_SetDefaultNetInfo(&message, &ConfigMsg);
        return true;
    }
   
    
    
}

#include "config.h"
/**
* @brief :w5500 配置默认值
* @param ： const TopologyMessage* topology 拓扑信息
* @return: bool
* @update: [2018-07-30][张宇飞][创建]
*/
static void W5500_SetDefaultNetInfo(StationMessage* message,  CONFIG_MSG*  config)
{

    
    
    if (message->node.id != 0)
    {
        config->lip[0] =  GET_N_BYTE( message->node.id , 3);
        config->lip[1] =  GET_N_BYTE( message->node.id , 2);
        config->lip[2] =  GET_N_BYTE( message->node.id , 1);
        config->lip[3] =  GET_N_BYTE( message->node.id , 0);
    
    }
    else
    {
        config->lip[0] =  192;
        config->lip[1] =  168;
        config->lip[2] =  10;
        config->lip[3] =  249; //默认IP
    }
    

	
    config->mac[0] = 0x00;
    config->mac[1] = 0x80;
    config->mac[2] = 0xE1;
    config->mac[3] = *(uint8_t*)(UID_BASE + 0);
    config->mac[4] = *(uint8_t*)(UID_BASE + 2);
    config->mac[5] = *(uint8_t*)(UID_BASE + 4);
    

    config->sub[0] = 255;
    config->sub[1] = 255;
    config->sub[2] = 255;
    config->sub[3] = 0;
    config->gw[0] = 192;
    config->gw[1] = 168;
    config->gw[2] = 10;
    config->gw[3] = 254;
    config->dns[0] = 114;
    config->dns[1] = 114;
    config->dns[2] = 114;
    config->dns[3] = 114;
    
   
   ConfigMsg.dhcp=0;
    ConfigMsg.debug=1;
    ConfigMsg.fw_len=0;

    ConfigMsg.state=NORMAL_STATE;
    ConfigMsg.sw_ver[0]=FW_VER_HIGH;
    ConfigMsg.sw_ver[1]=FW_VER_LOW;

}

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
#include "distribution.h"
#include "distribution_config.h"
#include "distribution_app.h"
#include "common_data.h"
#include "stm32f429xx.h"
#include "station.h"

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


static bool CheckIp(uint8_t ip);
static bool ConfigNetMessage(StationMessage* pMessage);
static bool FramWriteAndReadCheck(PointUint8* pPacket);
/**
* @brief :配置值保存保存
* @param  void
* @return: 0--正常
* @update: [2018-07-25][张宇飞][创建]
*/
bool FramSave(const uint8_t* pdata, uint16_t dataLen)
{    
    rt_device_t deviceFram = RT_NULL; 
   
    deviceFram = rt_device_find(RT_SPI_FRAM_NAME);
    if(deviceFram == RT_NULL)
    {
        perror("deviceFram == RT_NULL\n");
        return false;
    }
   // rt_err_t resultOpen = rt_device_open(deviceFram, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);
//    if (resultOpen != RT_EOK)
//    {
//        perror("deviceFram open failure.\n");
//        return false;
//    }
    
    uint32_t addr = 0;
    uint8_t data[2] = {0};
    
    //写标志
    data[0] = SAVE_FLAG_TAG1;
    data[1] = SAVE_FLAG_TAG2;
    addr = ADDR_FRAM_DISTRIBUTION;
    uint8_t result = rt_device_write(deviceFram, addr, data, 2);
    if (0 != result)
    {
        perror("result = rt_device_write(), ERROR:%d\n", result);
        return false;
    }
    addr += 2;
    
    if (dataLen > ADDR_FRAM_DISTRIBUTION_SIZE)
    {
         perror("Over Limit!\n", result);
        return false;
    }
    
    //写数据长度
    data[0] = GET_N_BYTE(dataLen, 0);
    data[1] = GET_N_BYTE(dataLen, 1);
    result = rt_device_write(deviceFram, addr, data, 2);
    if (0 != result)
    {
        perror("result = rt_device_write(), ERROR:%d\n", result);
        return false;
    }
    addr += 2;
    //写有效数据
    result = rt_device_write(deviceFram, addr, pdata, dataLen);
    if (0 != result)
    {
        perror("result = rt_device_write(), ERROR:%d\n", result);
        return false;
    }
    
   // rt_device_close(deviceFram);
    return true;
}

/**
* @brief :配置值读取,拓扑信息
* @param  uint8_t* pdata
* @param  输入的最多数据长度，如读取成功改写为实际长度
* @return: 0--正
* @update: [2018-07-25][张宇飞][创建]
*/
bool FramRead(uint8_t* pdata, uint16_t* pDataLen)
{
   rt_device_t deviceFram = RT_NULL; 
   //  rt_device_close(deviceFram);
    deviceFram = rt_device_find(RT_SPI_FRAM_NAME);
    if(deviceFram == RT_NULL)
    {
        perror("deviceFram == RT_NULL\n");
        return false;
    }
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
    
   // rt_device_close(deviceFram);   
    return true;
}

/**
* @brief :FRAM写后并进行读取校验
* @param ：PointUint8* pPacket
* @return: bool
* @update: [2018-07-25][张宇飞][创建]
*/
static bool FramWriteAndReadCheck(PointUint8* pPacket)
{
    bool state = FramSave(pPacket->pData, pPacket->len);
    if (!state)
    {
        perror("Save Failure.\n");
        return false;
    }
   
    uint8_t* readData;
    uint16_t readLen = pPacket->len;

    readData = (uint8_t*)CALLOC(sizeof(uint8_t), readLen);
    if (readData== NULL)
    {
        perror("CALLOC Failure\n");
        return false;
    }
    
    
     //读取检测
    state = FramRead(readData, &readLen);
    if ((!state) || (readLen != pPacket->len) )
    {
        FREE(readData);
       perror("Read Failure.\n");
       return false;      
    }
    
    for (uint16_t i = 0; i < readLen; i++)
    {
        if (readData[i] != pPacket->pData[i])
        {
            FREE(readData);
            perror("Check Failure.\n");                      
            return false;
        }
    }
    FREE(readData);
    return true;
}

/**
* @brief :拓扑信息保存,拓扑信息
* @param ： const TopologyMessage* topology 拓扑信息
* @return: bool
* @update: [2018-07-25][张宇飞][创建]
*/
bool TopologyMessageSave(const TopologyMessage* topology)
{
    PointUint8 packet;
    if (topology == NULL)
    {
        perror("topology == NULL\n");
        return false;
    }
    
    ErrorCode error = SerializeCollectTopology(topology, &packet, 0, 0);
    if (error)
    {
        perror("SerializeCollectTopology:0x%x\n", error);
        return false;
    }
    bool  state = FramWriteAndReadCheck(&packet);
    if (state)
    {
        perror("FramWriteAndReadCheck Failure\n");
        return false;
    }
    FREE(packet.pData);
    
    return true;
}

/**
* @brief :拓扑信息读取,拓扑信息
* @param ： const TopologyMessage* topology 拓扑信息
* @return: bool
* @update: [2018-07-25][张宇飞][创建]
*/
bool TopologyMessageRead(StationManger* manger)
{
    PointUint8 packet;
  
    
    if (manger == NULL)
    {
        perror("manger == NULL\n");
        return false;
    }
    uint8_t sizeBefore = list_size(&(manger->stationServer.stationPointList));
    
    packet.len = 512;
    packet.pData = (uint8_t*)CALLOC(sizeof(uint8_t), packet.len);
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
    
    ManagerAddStation(packet.pData, packet.len, manger);
    
    FREE(packet.pData);
    uint8_t sizeAfter = list_size(&(manger->stationServer.stationPointList));
    if (sizeAfter > sizeBefore)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
* @brief :拓扑信息保存,拓扑信息
* @param ： const TopologyMessage* topology 拓扑信息
* @return: bool
* @update: [2018-07-25][张宇飞][创建]
*/
bool StationMessageSave(const StationPoint* const point)
{
    PointUint8 packet;
    if (point == NULL)
    {
        perror("point == NULL\n");
        return false;
    }
    ErrorCode error = PacketEncodeStationMessage_All(point, &packet, 1, 0);
    if (error)
    {
        perror("PacketEncodeStationMessage_All:0x%x\n", error);
        return false;
    }
    
    bool  state = FramWriteAndReadCheck(&packet);
    if (!state)
    {
        perror("FramWriteAndReadCheck Failure\n");
        return false;
    }
    FREE(packet.pData);
    
    return true;
}

/**
* @brief :站点信息读取
* @param ： const TopologyMessage* topology 拓扑信息
* @return: bool
* @update: [2018-07-25][张宇飞][创建]
*/
bool StationMessageRead(StationManger* manger)
{
    PointUint8 packet;
  
    
    if (manger == NULL)
    {
        perror("manger == NULL\n");
        return false;
    }
    uint8_t sizeBefore = list_size(&(manger->stationServer.stationPointList));
    
    packet.len = 512;
    packet.pData = (uint8_t*)CALLOC(sizeof(uint8_t), packet.len);
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
    ManagerAddStationByStationMessage(packet.pData + 1, packet.len - 1, manger);
    
    
    FREE(packet.pData);
    uint8_t sizeAfter = list_size(&(manger->stationServer.stationPointList));
    if (sizeAfter > sizeBefore)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**
* @brief :站点信息读取
* @param : void (*ConfigDeal)(StationMessage* pMessage) 配置处理信息
* @return: bool
* @update: [2018-08-29][张宇飞][创建]
*/
bool StationMessageConfigRead(bool (*ConfigDeal)(StationMessage* pMessage))
{
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
    	return ConfigDeal(&message);
       
    }



}
/**
* @brief :检测ip是否符合要求，需要位于[1,253]
* @param : uint8_t ip
* @return: bool
* @update: [2018-08-29][张宇飞][创建]
*/
static bool CheckIp(uint8_t ip)
{
	return (ip >= 1) && (ip <= 253);
}

/**
 * @brief :配置网络信息的默认值
 * @param : StationMessage* pMessage
 * @return: bool
 * @update: [2018-08-29][张宇飞][创建]
 *			[2018-10-10][李  磊][改为w5500的ip地址增加0x80,dp83848的从1开始]
 */
static bool ConfigNetMessage(StationMessage* pMessage)
{

	
	if (!pMessage)
	{
		perror("Error:!pMessage\n");
		return false;
	}

	uint32_t ipAddress = pMessage->id_own;
	g_EthDP83848.ip[0] =  GET_N_BYTE( ipAddress, 3);
	g_EthDP83848.ip[1] =  GET_N_BYTE( ipAddress, 2);
	g_EthDP83848.ip[2] =  GET_N_BYTE( ipAddress, 1) ;
	g_EthDP83848.ip[3] =  GET_N_BYTE( ipAddress, 0) ;
    
	g_EthW5500.ip[0] =  GET_N_BYTE( ipAddress, 3);
	g_EthW5500.ip[1] =  GET_N_BYTE( ipAddress, 2);
	g_EthW5500.ip[2] =  GET_N_BYTE( ipAddress, 1);
	g_EthW5500.ip[3] =  GET_N_BYTE( ipAddress, 0);
	if (g_EthW5500.ip[3] < 0x80)
    {
        g_EthW5500.ip[3] += 0x80;
    }
    else
    {
        perror("Error:g_EthW5500.ip[3] < 0x80, ip:0x%x\n", g_EthW5500.ip[3]);
        return false;
    }
	for (uint8_t i = 0; i < 4; i++)
	{
		if(!CheckIp(g_EthW5500.ip[i]))
		{
			perror("Error:g_EthW5500.ip, ip:0x%x\n", g_EthW5500.ip[i]);
			return false;
		}
	}

	g_EthDP83848.mac[0] = 0x01;
	g_EthDP83848.mac[1] = 0x80;
	g_EthDP83848.mac[2] = 0xC2;
	g_EthDP83848.mac[3] = *(rt_uint8_t*)(UID_BASE + 4); //驱动内另有设置
	g_EthDP83848.mac[4] = *(rt_uint8_t*)(UID_BASE + 2);
	g_EthDP83848.mac[5] = *(rt_uint8_t*)(UID_BASE + 0);

	g_EthW5500.mac[0] = 0x00;
	g_EthW5500.mac[1] = 0x80;
	g_EthW5500.mac[2] = 0xE1;
	g_EthW5500.mac[3] = *(rt_uint8_t*)(UID_BASE + 0);
	g_EthW5500.mac[4] = *(rt_uint8_t*)(UID_BASE + 2);
	g_EthW5500.mac[5] = *(rt_uint8_t*)(UID_BASE + 4);

	//相同
	g_EthDP83848.netmask[0] = 255;
	g_EthDP83848.netmask[1] = 255;
	g_EthDP83848.netmask[2] = 255;
	g_EthDP83848.netmask[3] = 0;
	g_EthDP83848.gateway[0] = 0;
	g_EthDP83848.gateway[1] = 0;
	g_EthDP83848.gateway[2] = 0;
	g_EthDP83848.gateway[3] = 0;


	g_EthDP83848.dns[0] = 114;
	g_EthDP83848.dns[1] = 114;
	g_EthDP83848.dns[2] = 114;
	g_EthDP83848.dns[3] = 114;
	g_EthDP83848.remoteip[0] = 192;
	g_EthDP83848.remoteip[1] = 168;
	g_EthDP83848.remoteip[2] = 10;
	g_EthDP83848.remoteip[3] = 111;
	g_EthDP83848.dhcpstatus = 0;

	g_EthW5500.netmask[0] = 255;
	g_EthW5500.netmask[1] = 255;
	g_EthW5500.netmask[2] = 255;
	g_EthW5500.netmask[3] = 0;
	g_EthW5500.gateway[0] = 0;
	g_EthW5500.gateway[1] = 0;
	g_EthW5500.gateway[2] = 0;
	g_EthW5500.gateway[3] = 0;
	g_EthW5500.dns[0] = 114;
	g_EthW5500.dns[1] = 114;
	g_EthW5500.dns[2] = 114;
	g_EthW5500.dns[3] = 114;

	g_EthW5500.remoteip[0] = 192;
	g_EthW5500.remoteip[1] = 168;
	g_EthW5500.remoteip[2] = 10;
	g_EthW5500.remoteip[3] = 111;
	g_EthW5500.dhcpstatus = 0;
	return true;
}

/**
* @brief :读取然后，配置网络信息的默认值
* @param : StationMessage* pMessage
* @return: bool
* @update: [2018-08-29][张宇飞][创建]
*/
bool ReadAndConfigNetMessage(void)
{	
	return StationMessageConfigRead(ConfigNetMessage);	
}

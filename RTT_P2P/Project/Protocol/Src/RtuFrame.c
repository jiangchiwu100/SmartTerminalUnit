/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      RtuFrame.c
  * @brief:     
  * @version:   V0.1.0 
  * @author:    Zhang Yufei
  * @date:      2018-05-27
  * @update:    由dispic 永磁控制器移植而来
  */
#include "RtuFrame.h"
#include "helper.h"
#include "CRC16.h"
#include "log.h"
#include <string.h>



#include "station_manager.h"
#include "topology.h"
#include "extern_interface.h"
#include "distribution_config.h"
/******************************************
//针对此种情况出队与入队可能发生冲突。
//改在出队时，禁止串口接收中断。 2015/10/15
//增大缓冲帧容量由16改为64
//每次进入帧处理队列后刷新接收使能位
//增加  ReciveErrorFlag 接收错误标志 及接收满后的重新清空初始化
********************************************/




/**
  * @brief : 自定义数据解析处理
  * @param : ProtocolAnylast* anylast 解析处理器 
  * @return: none
  * @update: [2018-06-12][张宇飞][创建并进行基本测试]
  * [2018-06-20][张宇飞][添加广播地址0xFFFF识别接收]
  * [2018-06-21][张宇飞][结构中增加源地址]
  *[2018-07-28][张宇飞][修改广播地址BROADCAST_ADDRESS]
  */
uint8_t DataProtocolAnylast(ProtocolAnylast* anylast )
{
    uint8_t data = 0;
    uint8_t result = 0;
    FrameRtu* pReciveFrame =   &(anylast->recvRtu);
    FifoHandle* handle = anylast->fifohanlde; 
    if (handle == 0)
    {
        return ERROR_NULL_PTR;
    }
  
    if (handle->fifo.count > 0)
    {
        switch(anylast->step)
        {
            //判断启动部分 与 地址
            case 0:
            {               
                pReciveFrame->completeFlag = false;
                if ( handle->fifo.count < FRAME_FUNCODE_INDEX)
                {
                    return handle->fifo.count;
                } 
                //检视正确与否
                result = handle->Peek(handle, anylast->reviceFrame, FRAME_FUNCODE_INDEX);  //TODO:注意同步与互斥
                if (!result ) 
                {
                    handle->Dequeue(handle, &data);//丢弃一个
                    break;                    
                }
                uint16_t dataL = anylast->reviceFrame[FRAME_SOURCE_INDEX];
                uint16_t dataH = anylast->reviceFrame[FRAME_SOURCE_INDEX + 1];
                uint16_t adress = (dataH << 8 | dataL);
                pReciveFrame->address = adress; //源地址
                dataL = anylast->reviceFrame[FRAME_DEST_INDEX];
                dataH = anylast->reviceFrame[FRAME_DEST_INDEX + 1];
                adress = (dataH << 8 | dataL);//目的地址
                pReciveFrame->destAddress = adress;
                //判定启动功能码 与 地址, 地址对于相同，广播地址，本地地址为全0，接收
                if ((anylast->reviceFrame[0] == START_CHAR1) && 
                    (anylast->reviceFrame[1] == START_CHAR2) && 
                    (anylast->reviceFrame[4] == START_CHAR3) && 
                    (anylast->reviceFrame[5] == START_CHAR4) &&
                     ((anylast->address) == adress || (BROADCAST_ADDRESS == adress) || (anylast->address == 0)))
                {                 
                    
                    //出队并丢弃
                    for(uint8_t i = 0; i < FRAME_FUNCODE_INDEX; i++)
                    {
                        handle->Dequeue(handle, &data);
                    }
                    dataL = anylast->reviceFrame[2];
                    dataH = anylast->reviceFrame[3];
                    pReciveFrame->datalen = (dataH<<8  | dataL);
                    //检查最大接收帧限制
                    if (pReciveFrame->datalen <= RECIVE_FRAME_MAX)
                    {
                        anylast->step  = 1; //转入到下一步
                    }                 
                    
                }
                else
                {
                     handle->Dequeue(handle, &data);//丢弃一个                    
                }
                break;
            }
            //等待收取完整一帧，校验结束符号和CRC
            case 1:
            {
                 if ( handle->fifo.count >=  pReciveFrame->datalen + FRAME_END_LEN)
                 {
                        result = handle->Peek(handle, anylast->reviceFrame + FRAME_FUNCODE_INDEX, pReciveFrame->datalen + FRAME_END_LEN);  //TODO:注意同步与互斥
                        //判断结束符
                        if ((!result) ||  (END_CHAR !=  anylast->reviceFrame[pReciveFrame->datalen + FRAME_MIN_LEN -1]))
                        {
                            handle->Dequeue(handle, &data);//丢弃一个
                                                      
                        }                        
                        else
                        {
                            uint16_t crc =  CRC16(anylast->reviceFrame, pReciveFrame->datalen + FRAME_CRC_OFFSET);
                            uint8_t  crcL = anylast->reviceFrame[pReciveFrame->datalen + FRAME_CRC_OFFSET];
                            uint8_t  crcH = anylast->reviceFrame[pReciveFrame->datalen + FRAME_CRC_OFFSET + 1];
                                                     
                            
                            if (crc == ((uint16_t)crcH<<8  | crcL))
                            {
                                for (uint8_t k = 0; k <  pReciveFrame->datalen + FRAME_END_LEN; k++)
                                {
                                     handle->Dequeue(handle, &data);
                                }  
                                pReciveFrame->pData = anylast->reviceFrame;
                                pReciveFrame->pValidData = anylast->reviceFrame + FRAME_VALID_INDEX;
                                pReciveFrame->funcode =  anylast->reviceFrame[FRAME_FUNCODE_INDEX];// 功能码                               
                                pReciveFrame->completeFlag = true;
                            }
                            else
                            {
                                handle->Dequeue(handle, &data);//丢弃一个
                            }
                        }
                        anylast->step = 0;   
                 }
                
                
                 
                 break;
            }
            //获取数据字节长度
         
            default:
            {
                 anylast->step = 0;
                 anylast->reciveIndex = 0;
                 break;
            }
        }
    }
    return handle->fifo.count; //返回剩余长度
}

/**
* @brief : 将生成的数据内容进行打包。从状态位到校验和之前为有效位
* @param  ：PointUint8* packet 包信息
* @param  ：uint16_t addres  地址
* @return: 0-正常 非0错误
* @update: [2018-06-20[张宇飞][创建]
* [2018-06-21[张宇飞][修正数据长度错误，校验和赋值错误,添加形参多一个源地址]
*/
ErrorCode MakePacketMessage(PointUint8* packet, uint16_t addres , uint16_t sourceAddr)
{
    CHECK_POINT_RETURN(packet, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(packet->pData, NULL, ERROR_NULL_PTR);
    uint16_t index = 0;
	uint8_t* pData = packet->pData;
    pData[index++] = START_CHAR1;
    pData[index++] = START_CHAR2;
    pData[index++] = GET_N_BYTE(packet->len - FRAME_MIN_LEN, 0);
    pData[index++] = GET_N_BYTE(packet->len - FRAME_MIN_LEN, 1);
    pData[index++] = START_CHAR3;
    pData[index++] = START_CHAR4;
    pData[index++] = GET_N_BYTE(sourceAddr, 0);
    pData[index++] = GET_N_BYTE(sourceAddr, 1);
    pData[index++] = GET_N_BYTE(addres, 0);
    pData[index++] = GET_N_BYTE(addres, 1);

    uint16_t crc = CRC16(packet->pData, packet->len - 3);
    index = packet->len - 3;
    pData[index++] = GET_N_BYTE(crc, 0);
    pData[index++] = GET_N_BYTE(crc, 1);
    pData[index++] = END_CHAR;
    return ERROR_OK_NULL;
}



/**
  * @brief : Kill Timer.
  * @param : [pdrv]
  * @param : [No]
  * @return: none
  * @update: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t AddDataToBuffer(ProtocolAnylast* anylast, uint8_t* pFrame, uint16_t len)
{
    if (anylast->sendFifohanlde == 0 || pFrame== 0)
    {
        return ERROR_NULL_PTR;
    }
    FifoHandle* handle = anylast->sendFifohanlde;
    if (handle->fifo.capacity - handle->fifo.count < len)
    {
          return ERROR_VOLUM;
    }
    
    for (uint8_t i = 0; i < len; i++)
    {
        handle->Enqueue(handle, pFrame[i]);
    }      
    
    return 0;
}
  
/**
 * 数据帧处理
 * <p>
 *
 * @param  ProtocolAnylast* anylast 分析器数据
 * @param  FrameRtu* pReciveFrame 接收帧数据
 * @return 索引剩余数据 
 * @update: [2018-06-12[张宇飞][完善赋值]
 * [2018-06-19[张宇飞][修改形参为16bit]
 */
ErrorCode ProtocolAnylastInit(ProtocolAnylast* anylast, uint16_t address)
{      
    anylast->step = 0;
    anylast->reciveIndex = 3;
    anylast->frameCapcity = RECIVE_FRAME_MAX;
    
    anylast->address = address;
    anylast->ProtocolAnylastDeal = DataProtocolAnylast;
    anylast->SendData = AddDataToBuffer;
    anylast->sendRtu.address = address;
    return ERROR_OK_NULL;
}

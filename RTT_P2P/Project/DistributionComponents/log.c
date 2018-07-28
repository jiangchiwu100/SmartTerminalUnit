/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      log.c
* @brief:     记录日志信息等
* @version:   V1.0.0
* @author:    Zhang Yufei
* @date:      2018-06-27
* @update 2018/7/14: 增加状态信息内容
*/

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

#include "log.h"
#include "distribution_enum.h"
#include "helper.h"
#include "station.pb.h"

#include <stdint.h>

#include "RingQueue.h"

#include "topology.h"


#include "distribution.h"
#include "extern_interface.h"

//全局日志记录
LogHandle g_Loghandle =
{
    LogRecord_init_zero

};

//日志存储总数量
#define LOG_TOTAL_COUNT   (sizeof(g_Loghandle.log.exception) / sizeof(ExceptionRecord))
#define LOG_FUCNTION_LEN   (sizeof(g_Loghandle.log.exception->functionName)/ sizeof(char))

static ErrorCode AddExceptionLog(LogHandle* handle, uint16_t code,
    uint32_t id, uint16_t line, const char* name);



/**
* @brief :出事化日志记录系统
* @param ：
* @update: [2018-06-30][张宇飞][]
*/
ErrorCode LogInit(LogHandle* handle)
{
    CHECK_POINT_RETURN(handle, NULL, ERROR_NULL_PTR);
    handle->current = 0;
    handle->AddException = AddExceptionLog;

    return ERROR_OK_NULL;
}

/**
* @brief :添加日志信息
* @param ：
* @update: [2018-06-30][张宇飞][]
*/
static ErrorCode AddExceptionLog(LogHandle* handle, uint16_t code,
    uint32_t id, uint16_t line, const char* name )
{
    CHECK_POINT_RETURN(handle, NULL, ERROR_NULL_PTR);
    ExceptionRecord* record = handle->log.exception + handle->current;
    record->code = code;
    record->has_code = true;
    record->time = GetTime();
    record->has_time = true;
    record->id = id;
    record->has_id = true;
    uint8_t len = strlen(name);
    if (len <= LOG_FUCNTION_LEN)
    {
        memcpy(record->functionName, name, len);
    }
    else
    {
        memcpy(record->functionName, name, LOG_FUCNTION_LEN);
    }
    record->has_functionName = true;

    record->line = line;
    record->has_line = true;

  
    (handle->log.exception_count > LOG_TOTAL_COUNT) ?
        (handle->log.exception_count = LOG_TOTAL_COUNT) : (handle->log.exception_count++);
    
    handle->current = (handle->current + 1) % LOG_TOTAL_COUNT;


    return ERROR_OK_NULL;
}


/**
* @brief :编码日志消息,默认长度1024
* @param ：
* @update: [2018-06-30][张宇飞][]
*/
ErrorCode PacketEncodeLogMessage(LogRecord* log, PointUint8* packet, uint16_t addLen, uint16_t offset)
{
    uint16_t defaultLen = 1024;
    if (offset > defaultLen)
    {
        return ERROR_OVER_LIMIT;
    }
   
    CHECK_POINT_RETURN(log, NULL, ERROR_OK_NULL);
    CHECK_POINT_RETURN(packet, NULL, ERROR_OK_NULL);
    uint16_t len = defaultLen + addLen + offset;
    packet->pData = (uint8_t*)CALLOC(len, sizeof(uint8_t));
    CHECK_POINT_RETURN(packet->pData, NULL, ERROR_OK_NULL);
    
    
    pb_ostream_t stream = pb_ostream_from_buffer(packet->pData + offset + 4, defaultLen);
    bool status = pb_encode(&stream, LogRecord_fields, log);
    if (!status)
    {
        SafeFree(packet->pData);        
        return ERROR_ENCODE;
    }
    uint16_t realLen = stream.bytes_written + addLen + offset;
    if (realLen != len)
    {
        REALLOC(packet->pData, realLen);//调小到实际长度
    }
    packet->pData[offset++] = NANOPB_TYPE;
    packet->pData[offset++] = LOG_EXCEPTION;
    packet->pData[offset++] = GET_N_BYTE(stream.bytes_written, 0);
    packet->pData[offset++] = GET_N_BYTE(stream.bytes_written, 1);
    packet->len = realLen;
    return ERROR_OK_NULL;
}


/**
* @brief :解码日志信息
* @param ：
* @update: [2018-06-30][张宇飞][]
*/
ErrorCode PacketDecodeLogMessage(LogRecord* log, uint8_t* data, uint16_t len)
{  
    
    CHECK_POINT_RETURN(log, NULL, ERROR_OK_NULL);
    CHECK_POINT_RETURN(data, NULL, ERROR_OK_NULL);
   
    uint16_t nanolen = COMBINE_UINT16(data[2], data[1]);
    if (nanolen + 2 > len)
    {      
        return ERROR_OVER_LIMIT;
    }

    pb_istream_t instreamLog = pb_istream_from_buffer(data + 3, nanolen);
    bool status = pb_decode(&instreamLog, LogRecord_fields, log);
    if (!status)
    {             
        return ERROR_ENCODE;
    }    
    return ERROR_OK_NULL;
}



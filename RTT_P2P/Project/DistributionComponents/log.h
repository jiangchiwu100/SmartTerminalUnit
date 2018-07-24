/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      log.h
  * @brief:     
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-27
  * @update:    
  */
#ifndef __LOGLOG_H__
#define __LOGLOG_H__

#include "distribution_def.h"
#include "common_def.h"
#include <stdint.h>
#include "log.pb.h"



//ExceptionRecord
typedef struct  TagLogHandle
{
    LogRecord log;//日志
    uint8_t current; //存储 循环存储

    ErrorCode (*AddException)(struct  TagLogHandle* handle, uint16_t code,
        uint32_t id, uint16_t line, const char* name);
}LogHandle;

#define LogAddException(code, id) {g_Loghandle.AddException(&g_Loghandle, code, id, __LINE__, __FUNCTION__);}

#ifdef	__cplusplus
extern "C" {
#endif


ErrorCode LogInit(LogHandle* handle);
ErrorCode PacketEncodeLogMessage(LogRecord* log, PointUint8* packet, uint16_t addLen, uint16_t offset);
ErrorCode PacketDecodeLogMessage(LogRecord* log, uint8_t* data, uint16_t len);

extern LogHandle g_Loghandle;



#ifdef	__cplusplus
}
#endif

#endif

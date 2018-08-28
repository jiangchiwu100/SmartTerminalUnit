/**
 * @file GooseParser.h
 * @brief 解析goose的ini配置文件的线程文件
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.08.27
 *
 * @author Lee
 * @version ver 1.0
 **/


#ifndef _PARSER_THREAD_H_
#define _PARSER_THREAD_H_

#include <stdint.h>
#define RT_USING_INIPARSER   1

#define INIPARSER_THREAD_NAME                                          "iniparser"
#define INIPARSER_THREAD_PRIORITY                                      (25)
#define INIPARSER_THREAD_STACK_SIZE                                    (1024*10)
#define INIPARSER_THREAD_TIMESLICE                                     (20)


uint8_t rt_goose_parser_thread_start(void *param);

#endif
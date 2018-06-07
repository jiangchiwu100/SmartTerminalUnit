/**
 * @file md5.h
 * @brief md5算法
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2015.05.22
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _MD5_H
#define _MD5_H
 
#include <stdint.h>
  
  
int getStringMD5(const char* src, char* md5);  //获取字符串的MD5校验码
int getFileMD5(const char* fileName, uint8_t* md5Bytes);
//int getFileMD5(const char* fileName, char* md5);   //获取文件的MD5校验码

#endif



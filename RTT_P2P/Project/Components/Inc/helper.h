/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      topology.c
  * @brief:     
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-05-25
  * @update:    
  */
#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdint.h>

/**
*将两个8bit融合成16bit
*/
#define COMBINE_UINT16(byteH, byteL) (uint16_t)(((uint16_t)(uint8_t)(byteH)<<8) | ((uint8_t)(byteL)))
/**
*将4个8bit融合成32bit
*/
#define COMBINE_UINT32(byte4, byte3, byte2, byte1) ((uint32_t)((((uint32_t)(byte4 & 0xFF))<<24) | \
            (((uint32_t)(byte3 & 0xFF))<<16) | (((uint32_t)(byte2 & 0xFF))<<8) |  ((uint8_t)(byte1))))


/**
*获取N字节,n=0,1,2,3
*/
#define GET_N_BYTE(data, n) ((uint8_t)( (uint32_t)(data>>(n<<3))))

/**
*获取低16bit
*/
#define GET_UINT16(data)  ((uint16_t)((data) & (0xFFFF)))
/**
*置相应数据位
*/
#define SET_N_BIT(data, n) {(data) |= ( (uint32_t)1 << n);}
/**
*清除相应数据位
*/
#define RESET_N_BIT(n) (data & (~( (uint32_t)1 << n)))

/**
*检测相等 return  result
*/
#define CHECK_EQUAL_RETURN(ptr, value, result) do { if ((ptr) == (value)) return (result); }while(0);
#define CHECK_EQUAL_RETURN_LOG(ptr, value, result, id) do { if ((ptr) == (value)) { LogAddException(result, id);  return (result);} }while(0);
/**
*检测不相等相等 return  result
*/
#define CHECK_UNEQUAL_RETURN(ptr, value, result) do { if ((ptr) != (value)) return (result); }while(0);

#define CHECK_UNEQUAL_RETURN_LOG(ptr, value, result, id) do { if ((ptr) != (value)) { LogAddException(result, id);  return (result);} }while(0);
/**
*检测指针是否满足条件，返回return  result LogAddException(ERROR_ENCODE, 0);
*/
#define CHECK_POINT_RETURN(ptr, value, result) do { if ((ptr) == (value)) return (result); }while(0);
#define CHECK_NULL_RETURN(ptr, result) do { if ((ptr) == (0)) return (result); }while(0);

#define CHECK_POINT_RETURN_LOG(ptr, value, result, id) do { if ((ptr) == (value)) { LogAddException(result, id); return (result);} }while(0);


/**
*遍历链表开始——与下方配对使用
*/
#define FOR_EARCH_LIST_START(list)  { ListElment* m_foreach = list_head((list));\
                           uint8_t size_for = list_size((list));   \
                            for (uint8_t iii = 0; iii < size_for; iii++, m_foreach = m_foreach->next)  \
                            {
/**
*遍历链表结束——与上方配对使用
*/
#define FOR_EARCH_LIST_END()       }}

/**
*遍历链表开始——与下方配对使用
*/
#define FOR_EARCH_LIST_TAIL_START(list)  { ListElment* m_foreach = list_tail((list));\
                           uint8_t size_for = list_size((list));   \
                            for (uint8_t kkk = 0; kkk < size_for; kkk++, m_foreach = m_foreach->prev; )  \
                            {
/**
*遍历链表结束——与上方配对使用
*/
#define FOR_EARCH_LIST_TAIL_END()       }}




#ifdef	__cplusplus
extern "C" {
#endif


uint16_t SumCheck(const uint8_t array[], uint16_t start, uint16_t len);


uint32_t CalMaskAllOne(uint8_t n);

#ifdef	__cplusplus
}
#endif

#endif

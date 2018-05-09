/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      queue.h
  * @brief:     This file is part of systerm.
  * @version:   V1.0.1 
  * @author:    Lex
  * @date:      2015.02.08
  * @update:    [2017-12-06][Lex][make code cleanup]
  */
#ifndef __QUEUE_H__
#define __QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif /* END __cplusplus */
	
	
/* CONFIG --------------------------------------------------------------------*/
#ifndef  EN_QUEUE_WRITE
#define  EN_QUEUE_WRITE             1 // 禁止(0)或允许(1)FIFO发送数据
#endif /* END EN_QUEUE_WRITE */
	
#ifndef  EN_QUEUE_WRITE_FRONT
#define  EN_QUEUE_WRITE_FRONT       0 // 禁止(0)或允许(1)LIFO发送数据
#endif /* END EN_QUEUE_WRITE_FRONT */

#ifndef  EN_QUEUE_FULL
#define  EN_QUEUE_FULL              0
#endif /* END EN_QUEUE_FULL */

#ifndef  EN_QUEUE_FLUSH
#define  EN_QUEUE_FLUSH             0 // 禁止(0)或允许(1)清空队列
#endif /* END EN_QUEUE_FLUSH */

#ifndef  EN_QUEUE_NDATA
#define  EN_QUEUE_NDATA             0 // 禁止(0)或允许(1)取得队列数据数目
#endif /* END EN_QUEUE_NDATA */

#ifndef  EN_QUEUE_SIZE
#define  EN_QUEUE_SIZE              1 // 禁止(0)或允许(1)取得队列数据总容量
#endif /* END EN_QUEUE_SIZE */



/* DEFINE --------------------------------------------------------------------*/
#ifdef   QUEUE_GLOBALS
#define  QUEUE_EXT
#else
#define  QUEUE_EXT                  extern
#endif /* END QUEUE_GLOBALS */

#ifndef  QUEUE_ERR
#define  QUEUE_ERR                  0xFF // 参数错误
#endif /* END QUEUE_ERR */

#define  QUEUE_FULL                 8 // 队列满
#define  QUEUE_EMPTY                4 // 无数据
#define  QUEUE_OK                   1 // 操作成功

#define  Q_WRITE_MODE               1 // 操作成功
#define  Q_WRITE_FRONT_MODE         2 // 操作成功

/* TYPEDEF -------------------------------------------------------------------*/
#ifndef  QUEUE_U8
typedef  unsigned char              QUEUE_U8;
typedef  unsigned short             QUEUE_U16;
typedef  unsigned int               QUEUE_U32;
#endif /* END QUEUE_U8 */

typedef  struct _DataQueue          DataQueue;

/* STRUCT --------------------------------------------------------------------*/
struct _DataQueue
{
    QUEUE_U8  *Out;                                             // 指向数据输出位置        
    QUEUE_U8  *In;                                              // 指向数据输入位置         
    QUEUE_U8  *End;                                             // 指向Buf的结束位置       
    QUEUE_U8  *Buf;                                             // 存储数据的空间                  
    QUEUE_U16 MaxData;                                          // 队列中允许存储的数据个数 
    QUEUE_U8  (* ReadEmpty)();                                  // 读空处理函数             
    QUEUE_U8  (* WriteFull)(void*, QUEUE_U8, QUEUE_U8);         // 写满处理函数
};


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
QUEUE_EXT QUEUE_U8 QueueCreate(void *BufPCB, void *Buf, QUEUE_U32 SizeOfBuf, QUEUE_U8 (* ReadEmpty)(), QUEUE_U8 (* WriteFull)(void*, QUEUE_U8, QUEUE_U8));
QUEUE_EXT QUEUE_U8 QueueRead(QUEUE_U8 *Ret, void *Buf);
QUEUE_EXT QUEUE_U8 QueueWrite(void *Buf, QUEUE_U8 Data);
QUEUE_EXT QUEUE_U8 QueueWriteFront(void *Buf, QUEUE_U8 Data);
QUEUE_EXT QUEUE_U8 QueueWriteBlock(void *Buf, QUEUE_U8 *pData, QUEUE_U16 len);
QUEUE_EXT unsigned short QueueNData(void *Buf);
QUEUE_EXT unsigned short QueueSize(void *Buf);
QUEUE_EXT void QueueFlush(void *Buf);


#ifdef __cplusplus
}
#endif /* END __cplusplus */

#endif /* END _QUEUE_H_ */


/* END OF FILE ---------------------------------------------------------------*/


#ifndef  __COMMON_DEF_H
#define  __COMMON_DEF_H
#include <stdint.h> 
#include <stdbool.h>

typedef float           float32_t;
typedef long double     float64_t;


//#define true true
//#define false false
//#define bool bool

//#define bool uint8_t
////typedef uint8_t bool;
//
//
//#undef true
//#define true ((uint8_t)0xff)
//
//#undef false
//#define false ((uint8_t)0)

//typedef enum  TagBOOL
//{
//	true = 0xff,
//	false = 0,
//}bool;

/**
* 错误代码定义
*/
typedef enum TagErrorCode
{
	ERROR_OK_NULL = 0,
	ERROR_NULL_PTR = 0x11,
	ERROR_PARAMTER = 0x12,
	ERROR_MALLOC = 0x13,
	ERROR_OVER_LIMIT = 0x14, //超过范围
	ERROR_UNFIND = 0x15,
	ERROR_CHECK = 0x16, //校验错误
	ERROR_LEN = 0x17,  //长度错误
	ERROR_INSERT_LIST = 0x18,  //插入校验
	ERROR_VOLUM = 0x19,  //容量不足
	ERROR_EXIST = 0x1A,  //已存在   
	EROR_INIT = 0x1B, //初始化
	ERROR_ENCODE = 0x1C, //编码错误
	ERROR_DECODE = 0x1D, //解码错误

	ERROR_UNKONOW = 0x30,
}ErrorCode;

typedef struct TagPointUint8
{
	uint8_t* pData; //数据指针
	uint16_t len; //指向数据长度
}PointUint8;

typedef union TagPointFloat
{
	float32_t fdata;
	uint8_t uData[4];
}PointFloat;

typedef struct TagFIFO
{
    uint16_t capacity;//FIFO总容量
    uint16_t count;//运算个数
    uint16_t head;//队尾索引，入队索引
    uint16_t end;  //队头索引，出队索引
    uint8_t* pData;
} FifoInformation;

typedef struct TagFifoHandle
{
    FifoInformation fifo;
    uint8_t(*Enqueue)(struct TagFifoHandle *const fifohandle, uint8_t const indata);
    uint8_t(*Dequeue)(struct TagFifoHandle *const fifohandle, uint8_t*  dedata);
    uint8_t(*Peek)(struct TagFifoHandle *const fifohandle, uint8_t  *peekdata, uint8_t len);
} FifoHandle;


/*
*如何实现同步互斥机制
*/
typedef struct TagRingQueue
{  

    uint16_t capacity;//FIFO总容量
    uint16_t count;//总总数
    uint16_t head;//队尾索引，入队索引dashu
    uint16_t tail;  //队头索引，出队索引
    void** arrayPtr;//只想有效数据类型 

    bool (*Write)(struct TagRingQueue* ring,  void*  indata);
    bool (*Read)(struct TagRingQueue* ring, void** indata);
    bool (*Peek)(struct TagRingQueue *const ring, void  **peekdata, uint16_t len);
    bool  (*Destory)(struct TagRingQueue* ring);
    
} RingQueue;


/**
* @brief : 自定义打包数据，适用于UDP包等情况
*/
typedef struct  TagDatagramFrame
{
	uint32_t sourceAddress; //源地址
	uint32_t destAddress; //目的地址
	uint8_t* pData; //数据部分，指向的属于数据有效缓冲区
	uint16_t size; //数据长度

}DatagramFrame;

#endif


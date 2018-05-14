/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_HMISLAVE_app.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */
#ifndef	_HMI_101_APP_H_
#define _HMI_101_APP_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "hmi_101_config.h"


#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif

/* PUBLIC VARIABLE -----------------------------------------------------------*/
#pragma pack(push,1)
typedef struct 
{
    uint8_t Milliseconds_L;   // D7-D0
    uint8_t Milliseconds_H;   // D7-D0
    uint8_t Minutes;					// D5-D0
    uint8_t Hours;						// D4-D0
    uint8_t DayofWeekMonth;		// WEEK(D7-D5)MONTH(D4-D0)
    uint8_t Months;						// D3-D0
    uint8_t Years;						// D6-D0
}_DLT634_HMISLAVE_CP56Time2a_t;

typedef struct 
{
    struct STATUS
    {
        uint16_t Length;
    }status;

    uint8_t	buff[_DLT634_HMISLAVE_LPDUSIZE-sizeof(struct STATUS)];
}DLT634_HMISLAVE_PASDU;

typedef struct 
{
//link	
	enum
	{
        RXDSTART = 0,	        // 寻找起始标志状态
        RXDHEAD,	            // 寻找报文头
        RXDCONTINUE             // 接收后续报文
	}RxdStatus;           		// 接收状态
	
	uint16_t TxdHead;	      				// 发送缓冲区头指针
	uint16_t TxdTail;	     				// 发送缓冲区尾指针
	uint8_t TxdBuf[_DLT634_HMISLAVE_LPDUSIZE];     	// 发送缓冲区
	
	uint16_t RxdHead;	      				// 接收数据区头指针
    uint16_t RxdTail;       				// 接收数据区尾指针
	uint16_t RxdLength;     				// 接收一帧数据长度
	uint8_t RxdBuf[_DLT634_HMISLAVE_LPDUSIZE];			// 接收缓冲区
	
	uint16_t TxdTail_Pri;
	uint8_t TxdBuf_Pri[_DLT634_HMISLAVE_LPDUSIZE]; // 发送缓冲区，暂存发送的数据，重发时用
	
//app
	uint8_t StoreDATABuf[_DLT634_HMISLAVE_LPDUSIZE];     	// 数据待发送缓冲区
	
	DLT634_HMISLAVE_PASDU TX_Temp;              //发送数据处理
	DLT634_HMISLAVE_PASDU RX_Temp;              //接受数据处理
	
	uint32_t LinkFlag;              //回复链路数据标志
	uint32_t DataFlag;				//回复1级数据标志
	
	uint32_t AllDataFlag;			//总召状态标志
    uint16_t AllData_yx_sendaddr;   //总召遥信待发送首地址
    uint16_t AllData_yc_sendaddr;   //总召遥测待发送首地址
	
	uint32_t ClockCounter;  			//程序运行周期控制
	uint32_t TimeOutTick_Pri; 			//超时节拍计时器
	uint32_t RetryCount;				//超时记数器
    
    uint32_t TimeOutTick_AllData;
	
	uint32_t TimeOutTick_AskSta;       //链路重发启动链路间隔
    uint32_t TimeOutTick_AskCount;     //链路重发启动链路次数
	
	uint8_t RlaConCode;	        // 接收的控制码FCB位记录
	uint8_t FCBNoTurnNum;       // FCB未翻转计数
}DLT634_HMISLAVE_APPINFO;

typedef struct 
{
	uint8_t IEC_DIR;         // 终端0x80,主站0x00
	
	uint16_t SourceAddr;	 // 源地址，即本机地址	
	uint8_t FixFrmLength;	 //固定帧长度
	
	uint32_t ClockTimers;		//调用超出次数，程序执行一次，控制运行周期
	uint32_t TimeOutValue;		//超时重发间隔
	uint32_t AskStaOutValue;    //链路请求间隔
    uint32_t AllDataOutValue;    //数据发送间隔
}DLT634_HMISLAVE_PAD;
#pragma pack(pop)

extern DLT634_HMISLAVE_PAD      DLT634_HMISLAVE_Pad[_DLT634_HMISLAVE_VOLUMES];

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern uint8_t DLT634_HMI_SLAVE_REPLY(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_HMI_SLAVE_AppInit(uint8_t pdrv);
extern uint8_t DLT634_HMI_SLAVE_Clock(uint8_t pdrv);

#endif /* END _DLT634_HMI_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/



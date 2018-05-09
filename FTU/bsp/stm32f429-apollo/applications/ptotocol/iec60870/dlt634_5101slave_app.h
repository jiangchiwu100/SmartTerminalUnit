/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      dlt634_5101slave_app.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.12.19
  * @update:    
  */
#ifndef	_DLT634_5101SLAVE_APP_H_
#define _DLT634_5101SLAVE_APP_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "dlt634_5101slave_config.h"


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
}_DLT634_5101SLAVE_CP56Time2a_t;

typedef struct 
{
    struct STATUS
    {
        uint8_t Length;
        union
        {
            uint8_t priority;	
            uint8_t Lock_ID;
        }symbol;
    }status;

    struct HEAD
    {
        uint8_t TypeID;     
        uint8_t VSQ;  
        uint8_t COT_L;
        uint8_t COT_H;
        uint8_t PubAddr_L;
        uint8_t PubAddr_H;
    }Head;

    union
    {
        uint8_t	buff[256-sizeof(struct STATUS)-sizeof(struct HEAD)];
        struct
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_H;
            uint8_t QOI; 
        }C_100;

        struct
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_H;	
            _DLT634_5101SLAVE_CP56Time2a_t CP56Time2a;
        }C_103;

        struct
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_H;	
            uint8_t QRP; 
        }C_105;

        struct
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_H;	
            uint8_t COI; 
        }C_70;

        struct
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_H;
            uint8_t FBP_L;
            uint8_t FBP_H;				
        }C_104;

        union
        {
            struct
            {
                struct
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_H;	
                    uint8_t Value; 
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/3];
            }SQ0;

            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;
                struct
                {
                    uint8_t Value; 
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-2)/1];
            }SQ1;
        }C_1;

        union
        {
            struct
            {
                struct
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_H;	
                    uint8_t Value; 
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/3];
            }SQ0;

            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;	
                struct
                {
                    uint8_t Value; 
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-2)/1];
            }SQ1;
        }C_3;

        union
        {
            struct
            {
                struct
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_H;
                    uint8_t Value; 
                    _DLT634_5101SLAVE_CP56Time2a_t CP56Time2a;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/10];
            }SQ0;

            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;	
                struct
                {
                    uint8_t Value; 
                    _DLT634_5101SLAVE_CP56Time2a_t CP56Time2a;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-2)/8];
            }SQ1;
        }C_30;

        union
        {
            struct
            {
                struct
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_H;	
                    uint8_t Value; 
                    _DLT634_5101SLAVE_CP56Time2a_t CP56Time2a;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/10];
            }SQ0;

            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;	
                struct
                {
                    uint8_t Value; 
                    _DLT634_5101SLAVE_CP56Time2a_t CP56Time2a;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-2)/8];
            }SQ1;
        }C_31;

        union
        {
            struct
            {
                struct
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_H;
                    uint32_t Value;
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/7];
            }SQ0;

            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;	
                struct
                {
                    uint32_t Value;									
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-2)/5];
            }SQ1;
        }C_9;

        union
        {
            struct
            {
                struct
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_H;
                    uint32_t Value;
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/7];
            }SQ0;

            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;
                struct
                {
                    uint32_t Value_L;
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-2)/5];
            }SQ1;
        }C_11;

        union
        {
            struct
            {
                struct
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_H;
                    uint32_t Value;	
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))/7];
            }SQ0;

            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;
                struct
                {
                    uint32_t Value;	
                    uint8_t QDS;
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-2)/5];
            }SQ1;
        }C_13;

        struct
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_H;
            uint8_t SCO_L;
            uint8_t SCO_H;
        }C_45;	

        struct
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_H;	
            uint8_t DCO_L;
            uint8_t DCO_H;
        }C_46;	

        struct
        {
            uint8_t Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD))]; 
        }C_42;

        struct
        {
            uint8_t InfoAddr_L;
            uint8_t InfoAddr_H;	
            uint8_t SN_L;
            uint8_t SN_H;
        }C_200;

        union
        {
            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;
            }Down;

            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;	
                uint8_t SN1_L;
                uint8_t SN1_H;
                uint8_t SN2_L;
                uint8_t SN2_H;
                uint8_t SN3_L;
                uint8_t SN3_H;
            }Up;
        }C_201;

        union
        {
            struct
            {
                uint8_t SN1_L;
                uint8_t SN1_H;
                uint32_t Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-2)/4];		
            }Down;

            struct
            {
                uint8_t SN_L;
                uint8_t SN_H;	
                uint8_t PI_L;
                uint8_t PI_H;
                struct
                {
                    uint8_t InfoAddr_L;
                    uint8_t InfoAddr_H;
                    uint8_t Tag;
                    uint8_t Len;
                    uint32_t Value;	
                }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-4)/8];
            }Up;
        }C_202;

        union
        {
            uint8_t SN_L;
            uint8_t SN_H;	
            uint8_t PI_L;
            uint8_t PI_H;
            struct
            {
                uint8_t InfoAddr_L;
                uint8_t InfoAddr_H;
                uint8_t Tag;
                uint8_t Len;
                uint32_t Value;	
            }Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-4)/8];
        }C_203;

        struct
        {
            uint8_t Array[(256-sizeof(struct STATUS)-sizeof(struct HEAD)-4)/1];
        }C_210;
    }Data;
}DLT634_5101SLAVE_PASDU;

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
	uint8_t TxdBuf[_DLT634_5101SLAVE_LPDUSIZE];     	// 发送缓冲区
	
	uint16_t RxdHead;	      				// 接收数据区头指针
    uint16_t RxdTail;       				// 接收数据区尾指针
	uint16_t RxdLength;     				// 接收一帧数据长度
	uint8_t RxdBuf[_DLT634_5101SLAVE_LPDUSIZE];			// 接收缓冲区
	
	uint16_t TxdTail_Pri;
	uint8_t TxdBuf_Pri[_DLT634_5101SLAVE_LPDUSIZE]; // 发送缓冲区，暂存发送的数据，重发时用
	
	uint16_t TxdTail_Special;
	uint8_t TxdBuf_Special[_DLT634_5101SLAVE_LPDUSIZE]; // 发送缓冲区，暂存发送的数据，特殊数据链路重建时发送
	
//app
	DLT634_5101SLAVE_PASDU *StoreDATA1IN;              //待发送缓冲区1级数据入指针
	DLT634_5101SLAVE_PASDU *StoreDATA1OUT;             //待发送缓冲区1级数据出指针
	DLT634_5101SLAVE_PASDU StoreDATA1Buf[_DLT634_5101SLAVE_STOREDATA1NUM];     	// 1级数据待发送缓冲区
	DLT634_5101SLAVE_PASDU *StoreDATA2IN;              //待发送缓冲区2级数据入指针
	DLT634_5101SLAVE_PASDU *StoreDATA2OUT;             //待发送缓冲区2级数据出指针
	DLT634_5101SLAVE_PASDU StoreDATA2Buf[_DLT634_5101SLAVE_STOREDATA2NUM];     	// 2级数据待发送缓冲区
	DLT634_5101SLAVE_PASDU ST_Temp;              //存储数据处理
	
	DLT634_5101SLAVE_PASDU TX_Temp;              //发送数据处理
	DLT634_5101SLAVE_PASDU RX_Temp;              //接受数据处理
	
	uint32_t LinkFlag;              //回复链路数据标志
	uint32_t Data1Flag;				//回复1级数据标志
	uint32_t Data2Flag;				//回复2级数据标志
	
	uint32_t AllDataFlag;			//总召状态标志
	
	uint32_t ClockCounter;  			//程序运行周期控制
	uint32_t TimeOutTick_Pri; 			//超时节拍计时器
	uint32_t RetryCount;				//超时记数器
	
	uint32_t TimeOutTick_AskSta;       //链路重发启动链路间隔
    uint32_t TimeOutTick_AskCount;     //链路重发启动链路次数
    
    uint32_t TimeOutLink;       //链路无数据交互
	
	uint8_t RlaConCode;	        // 接收的控制码FCB位记录
	uint8_t FCBNoTurnNum;       // FCB未翻转计数
	
	uint16_t AllData_yx_sendaddr; // 
    uint16_t AllData_yc_sendaddr; // 
}DLT634_5101SLAVE_APPINFO;

typedef struct 
{
	uint8_t Port;            //端口
    uint8_t Encrypt;	     // 加密
	uint8_t BalanMode;	     // 1平衡，0非平衡
	uint8_t IEC_DIR;         // 终端0x80,主站0x00
	
	uint16_t SourceAddr;	 // 源地址，即本机地址
	
	uint8_t LinkAddrSize;   //链路地址长度
	uint8_t ASDUCotSize;    //传送原因长度
	
	uint16_t ASDUAddr;       //ASDU地址
	uint8_t ASDUAddrSize;    //ASDU地址长度
	uint8_t FixFrmLength;	 //固定帧长度
	
	uint32_t ClockTimers;		//调用超出次数，程序执行一次，控制运行周期
	uint32_t TimeOutValue;		//超时重发间隔
	uint32_t AskStaOutValue;    //链路请求间隔
    uint32_t TimeOutLink;		//链路超时
	
	uint16_t YX_AllNum;       //遥信总数
	uint16_t YX_FirstAddr;    //遥信首地址
	uint16_t YX_FrameNum;	  //遥信每组数量
	uint16_t YC_AllNum;		  //遥测总数
	uint16_t YC_FirstAddr;    //遥信首地址
	uint16_t YC_FrameNum;	  //遥测每组数量

}DLT634_5101SLAVE_PAD;
#pragma pack(pop)

extern DLT634_5101SLAVE_PAD      DLT634_5101Slave_Pad[_DLT634_5101SLAVE_VOLUMES];

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern uint8_t DLT634_5101_SLAVE_REPLY(uint8_t pdrv, uint8_t *pbuf);
extern uint8_t DLT634_5101_SLAVE_AppInit(uint8_t pdrv);
extern uint8_t DLT634_5101_SLAVE_Clock(uint8_t pdrv);

#endif /* END _DLT634_5101_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/



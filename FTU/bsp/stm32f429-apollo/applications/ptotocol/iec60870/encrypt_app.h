/*
** ------------------------------- Copyright (c) -------------------------------                                     
**
**                           http://www.sojoline.com
**
** note:    Copyright SOJO. ALL RIGHTS RESERVED.
**          This software is provided under license and contains proprietary and
**          confidential material which is the property of Company Name tech. 
**    
** -------------------------------- File Info ----------------------------------
** File name:               encrypt_app.h
** Descriptions:            The application layer of encryp
**
** -----------------------------------------------------------------------------
** Created by:              Mr.J
** Created date:            2015.10.25
** System:                  Protocol Software System
** Version:                 V3.01
** Descriptions:            The original version
**
** -----------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
** ---------------------------------------------------------------------------*/
#ifndef	_ENCRYPT_APP_H_
#define _ENCRYPT_APP_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "encrypt_config.h"
#include "integer.h"

#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif

/* PUBLIC VARIABLE -----------------------------------------------------------*/
#pragma pack(push,1)
__packed typedef struct 
{
    __packed struct STATUS//ENCRYPT_PASDU-2
    {
        BYTE Length_H;
        BYTE Length_L;
    }status;
		
    __packed struct HEAD//报文类型
    {
        BYTE MesType_H;
        BYTE MesType_L;
    }Head;
		
    __packed struct//封装数据域
    {
        BYTE Type;
        BYTE buff[_ENCRYPT_LPDUSIZE-sizeof(struct STATUS)-sizeof(struct HEAD)-1];
    }Data;
}ENCRYPT_PASDU;

typedef struct 
{
//link	

	enum
	{
        RXDSTART = 0,	          // 寻找起始标志状态
        RXDHEAD,	              // 寻找报文头
        RXDCONTINUE             // 接收后续报文
	}RxdStatus;           			// 接收状态
	
	WORD TxdHead;	      				// 发送缓冲区头指针
	WORD TxdTail;	     					// 发送缓冲区尾指针
	BYTE TxdBuf[_ENCRYPT_LPDUSIZE];     	// 发送缓冲区
	
	WORD RxdHead;	      				// 接收数据区头指针
	WORD RxdTail;       				// 接收数据区尾指针
	WORD RxdLength;     				// 接收一帧数据长度
	BYTE RxdBuf[_ENCRYPT_LPDUSIZE*2];			// 接收缓冲区
	
//app
	ENCRYPT_PASDU RxTemp;     	// 接受数据处理
    ENCRYPT_PASDU TxTemp;     	// 接受数据处理
	
    ENCRYPT_PASDU StoreDATA1Temp;             //组包
	ENCRYPT_PASDU *StoreDATA1IN;              //待发送缓冲区1级数据入指针
	ENCRYPT_PASDU *StoreDATA1OUT;             //待发送缓冲区1级数据出指针
	ENCRYPT_PASDU StoreDATA1Buf[_ENCRYPT_STOREDATA1NUM];     	// 1级数据待发送缓冲区
	
	DWORD LinkFlag;              //回复链路数据标志
	
	WORD IECLength;     				// IEC数据长度
	BYTE IECBuf[_ENCRYPT_LPDUSIZE];			// IEC接收缓冲区

	BYTE IVData[16];   //前8位R1,后8位R1按位取反
	
	BYTE Object;       //链接对象
	BYTE M_ID[8];       //证书管理工具ID
    BYTE R_NUM[8];       //随机数
	
	BYTE CerContent[_ENCRYPT_LPDUSIZE*2]; // 证书内容
    
    BYTE Enc_txbuff[_ENCRYPT_LPDUSIZE*2];
    BYTE Enc_rxbuff[_ENCRYPT_LPDUSIZE*2];
	
}ENCRYPT_APPINFO;
#pragma pack(pop)

/* PUBLIC FUNCTION DECLARATION -----------------------------------------------*/
extern WORD Encrypt_ReadMISIData(BYTE *pbuf, BYTE pdrv);
extern BYTE Encrypt_IECSendData(BYTE *pbuf, WORD count, BYTE type, BYTE pdrv);
extern BYTE Encrypt_ENCSendData(BYTE pdrv);
extern BYTE Encrypt_AppInit(BYTE pdrv);

#endif /* END _ENCRYPT_APP_H_ */
    

/* END OF FILE ---------------------------------------------------------------*/



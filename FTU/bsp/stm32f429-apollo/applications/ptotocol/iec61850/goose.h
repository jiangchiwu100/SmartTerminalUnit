/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      goose.h
  * @brief:     The protocol of goose.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2018.03.28
  * @updata  : [2018-03-28][Sunxr][modify the driver]
  */
  
#ifndef	__GOOSE_H__
#define __GOOSE_H__

/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include "stm32f4xx_hal.h"

/* DEFINE --------------------------------------------------------------------*/
#define GOOSE_T0                        (2000)      // 5000ms
#define GOOSE_T1                        (2)         // 2ms     
#define GOOSE_T2                        (2 * GOOSE_T1)      
#define GOOSE_T3                        (2 * GOOSE_T2)  

#define GOOSE_LINK_MAX_LEN              1500
#define GOOSE_BUF_MAX_SIZE              4500

/* Ethernet type */
#define ETH_TYPE_GOOSE                  0x88B8
#define ETH_TYPE_SV                     0x88BA

/* APPID highest 2 bit  00-GOOSE 01-SV */
#define APPID_TYPE                      0xC000
/* GOOSE 0x0000~0x3FFF */
#define APPID_TYPE_GOOSE                0x0000
/* SV 0x4000~0x7FFF */
#define APPID_TYPE_SV                   0x4000

/* MAC ADDR */
#define MAC_FOURTH_BYTE_GOOSE           0x01
#define MAC_FOURTH_BYTE_SV              0x04

/* TPID */
#define TPID_GOOSE                      0x8100

/* VLAN ID */
#define VID_DEFAULT                     0

/* User Priority */
#define USER_PRIORITY                   4

/* PDU Tag */
#define PDU_TAG_GOCBREF                 0x80            
#define PDU_TAG_TIME_ALLOW_TO_LIVE      0x81            
#define PDU_TAG_DATASET                 0x82            
#define PDU_TAG_GOID                    0x83           
#define PDU_TAG_T                       0x84          
#define PDU_TAG_STNUM                   0x85          
#define PDU_TAG_SQNUM                   0x86      
#define PDU_TAG_TEST                    0x87            
#define PDU_TAG_CONFREV                 0x88            
#define PDU_TAG_NDSCOM                  0x89            
#define PDU_TAG_NUM_DATASET_ENTIIES     0x8A           
#define PDU_TAG_ALL_DATA                0xAB      

/* Data type in dataset */
#define DATASET_TAG_ARRAY               0x81            // 数组
#define DATASET_TAG_STRCTURE            0x82            // 结构体
#define DATASET_TAG_BOOLEAN             0x83            // 布尔
#define DATASET_TAG_BIT_STRING          0x84            // 位串
#define DATASET_TAG_INTEGER             0x85            // 整形
#define DATASET_TAG_UNSIGNED            0x86            // 无符号整形
#define DATASET_TAG_FLOATING_POINT      0x87            // 浮点型
#define DATASET_TAG_OCTET_STRING        0x89            // 8位位组
#define DATASET_TAG_VISIBLE_STRING      0x8A            // 可视字符串
#define DATASET_TAG_GENERALIZED_TIME    0x8B            // 广义时间
#define DATASET_TAG_BINARY_TIME         0x8C            // 时间timeofday
#define DATASET_TAG_BCD                 0x8D            // BCD
#define DATASET_TAG_BOOLEAN_ARRAY       0x8E            // 布尔数组
#define DATASET_TAG_OBJID               0x8F            // Object标识
#define DATASET_TAG_MMS_STRING          0x90            // MMS串
#define DATASET_TAG_UTC_TIME            0x91            // 世界协调时间                 

/* PUBLIC VARIABLE -----------------------------------------------------------*/

/* STRUCT --------------------------------------------------------------------*/
#pragma pack(push,1)
/* MAC GOOSE : 01-0C-CD-01-00~01-0C-CD-01-FF; SV : 01-0C-CD-04-00~01-0C-CD-04-FF*/
struct TagGooseMAC
{
    uint8_t macDst[6];        // Destination MAC
    uint8_t macSrc[6];	      // Source MAC
};

struct TagTCI
{
    uint16_t userPrioity : 3;
	uint16_t CFI : 1;
	uint16_t VID : 12;        // VLAN ID 
};

/* GOOSE TAG */
struct TagGooseLinkTAG
{
    uint16_t tpid;
    struct TagTCI tci;
};

struct TagSubscriberInfo
{
    uint8_t membersNum;   
};

struct TagReceiverInfo
{
	uint8_t ip[4];
    uint32_t stNum;
    uint32_t sqNum;  
    uint32_t data[8];	
};

struct TagPublisherInfo
{
	uint8_t haveChange;	
    uint32_t linkTimer; 
    uint32_t stNum;
    uint32_t sqNum;   
    uint32_t *dataset; 	
};

struct TagGooseAPDU
{
    uint32_t timeAllowedToLive;		
	uint32_t goID;             
    uint32_t stNum;
    uint32_t sqNum;
    uint8_t test;
	uint32_t confRev;
	uint8_t ndsCom;     // bool    needsCommission
	uint32_t data[8];
};

struct TagGooseLink
{
    uint8_t macDst[6];        // Destination MAC
    uint8_t macSrc[6];	      // Source MAC    
	struct TagGooseLinkTAG tag;
	uint16_t ethType;         // Ethenet Type
	uint16_t appid;
	uint16_t pduLength;       // APPID+pduLength+reserved1+reserved2+APDU
	uint16_t reserved1;
	uint16_t reserved2;			
	struct TagGooseAPDU apdu;
	uint8_t cks;
};	
	
struct TagCommParameters 
{
    uint8_t macDst[6];	
    uint8_t macSrc[6];	      // Source MAC	
    uint8_t vlanPriority;
    uint16_t vlanId;
    uint16_t appId;
    //uint16_t minTime;
    //uint16_t maxTime;	
}; 

#pragma pack(pop)

uint8_t goose_receiver_processe(uint8_t *pbuf, uint8_t *src_ip);
uint16_t goose_publisher_process(uint16_t clock, struct TagGooseLink *publisher, uint8_t haveChange);
	
#endif /* END _GOOSE_H_ */

/* END OF FILE ---------------------------------------------------------------*/



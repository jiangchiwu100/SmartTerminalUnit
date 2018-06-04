/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      goose.c
  * @brief:     The protocol of goose.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2018.03.28
  * @updata  : [2018-03-28][Sunxr][modify the driver]
  */
 
/* INCLUDE FILES -------------------------------------------------------------*/ 
#include "goose.h"
#include "string.h"
#include "distributed_interface.h"

static struct TagCommParameters GoosePara;
static struct TagPublisherInfo PublisherInfo;
static struct TagReceiverInfo ReceiverInfo; 
//uint8_t g_GooseRxBuf[GOOSE_BUF_MAX_SIZE];
//uint8_t g_GooseTxBuf[GOOSE_BUF_MAX_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};


/**
  * @brief : sum of CKS.
  * @param : [pdrv]
  * @param : [pBuf]
  * @return: sum
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static uint8_t goose_cks(uint8_t *pBuf)
{
    uint8_t sum = 0;
    uint8_t *p = 0;
    uint8_t i = 0;

    p = pBuf + 18;
    i = *(p + 2);

    while (i--)
    {
        sum += (*p++);
    }

    return (uint8_t)(sum&0xff);
}

/**
  * @brief : publisher process.
  * @param : [clock]
  * @param : [publisher]
  * @return: [none]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint16_t goose_publisher_process(uint16_t clock, struct TagGooseLink *publisher, uint8_t haveChange)
{
    static uint8_t s_sta = 0;
	static uint32_t s_period = 0;
	static uint32_t s_timer = 0;
	
	if ((++s_period >= s_timer) || haveChange)
	{
		s_period = 0;	
		
        if (haveChange)
		{
            s_sta = 1;	
			
			PublisherInfo.sqNum = 0;
			PublisherInfo.stNum++;
			
			if (PublisherInfo.stNum == 0)
			{
				PublisherInfo.stNum = 1;
			}			
		}
		
		memcpy(publisher->macDst, GoosePara.macDst, 6);
		memcpy(publisher->macSrc, GoosePara.macSrc, 6);
		publisher->tag.tpid = TPID_GOOSE;
		publisher->tag.tci.userPrioity = USER_PRIORITY; 
//				publisher->tag.tci.VID = VID_DEFAULT; 
//				publisher->tag.tci.CFI = 
		publisher->ethType = ETH_TYPE_GOOSE;
		publisher->appid = APPID_TYPE_GOOSE;
		publisher->pduLength = sizeof (struct TagGooseLink) - 19;
		publisher->apdu.timeAllowedToLive = GOOSE_T0 * 2;
		publisher->apdu.goID = 1; 
		publisher->apdu.stNum = PublisherInfo.stNum;
		publisher->apdu.sqNum = PublisherInfo.sqNum;
		publisher->apdu.test = 0;
		publisher->apdu.confRev = 0;
		publisher->apdu.ndsCom = 0;
//        publisher->apdu.data[0] = g_SelfSts[0].variableState;
		memcpy(publisher->apdu.data, PublisherInfo.dataset, sizeof(publisher->apdu.data));

		PublisherInfo.sqNum++;
		
		publisher->cks = goose_cks((uint8_t *)publisher);				
		
		switch (s_sta)
		{
		    case 0:
				//s_sta = 1;
			    s_timer = GOOSE_T0;
                break;  
		    case 1:
				s_sta = 2;
			    s_timer = GOOSE_T1;
                break;  
		    case 2:
				s_sta = 3;
			    s_timer = GOOSE_T1;
                break;  
		    case 3:
				s_sta = 4;
			    s_timer = GOOSE_T2;
                break;  
		    case 4:
				s_sta = 0;
			    s_timer = GOOSE_T3;
                break;  			
		}
	}

    return (sizeof (struct TagGooseLink));
}

void goose_link_on_timer(void)
{
    
}
/**
  * @brief : receiver processe
  * @param : [none]
  * @return: [none]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
uint8_t goose_receiver_processe(uint8_t *pbuf, uint8_t *src_ip)
{
//	uint8_t *appdata;
	struct TagGooseLink *rec = (struct TagGooseLink *)pbuf;
		
    if (pbuf[0] == 0x01 && pbuf[1] == 0x0C && pbuf[2] == 0xCD)
	{		
		if (rec->cks != goose_cks(pbuf))
		{
			return 1;
		}
		
//		if (rec->apdu.sqNum != ReceiverInfo.sqNum + 1)
//		{
//			return 1;
//		}
		
		ReceiverInfo.sqNum = rec->apdu.sqNum;
		
		//appdata = (uint8_t *)rec->apdu.data;  
		
		distributUpdataQueue(0, src_ip[3], rec->apdu.data);
	}
	else
	{
	    return 1;
	}
	
	return 0;
}
	
/**
  * @brief : goose_init
  * @param : [none]
  * @return: [none]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
int goose_init(void)
{
    GoosePara.macDst[0] = 0x01;
	GoosePara.macDst[1] = 0x0C;
	GoosePara.macDst[2] = 0xCD;
	GoosePara.macDst[3] = 11;
	GoosePara.macDst[4] = 11;
	GoosePara.macDst[5] = 11;

    GoosePara.macSrc[0] = 0x01;
	GoosePara.macSrc[1] = 0x0C;
	GoosePara.macSrc[2] = 0xCD;
	GoosePara.macSrc[3] = 22;
	GoosePara.macSrc[4] = 22;
	GoosePara.macSrc[5] = 22;

//    PublisherInfo.haveChange = 0;
    PublisherInfo.sqNum = 1;
    PublisherInfo.stNum = 1;  
	ReceiverInfo.sqNum = PublisherInfo.sqNum;
    PublisherInfo.dataset = (uint32_t *)&g_SelfSts[0].comstr;	
}

INIT_APP_EXPORT(goose_init)

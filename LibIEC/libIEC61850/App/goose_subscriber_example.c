/*
 * goose_subscriber_example.c
 *
 * This is an example for a standalone GOOSE subscriber
 *
 * Has to be started as root in Linux.
 */

#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "hal_thread.h"

//#include <stdlib.h>
//#include <stdio.h>
#include "extern_interface.h"
#include "miscellaneous.h"

#include "RingQueue.h"
/**
* @brief :根据StNum和SqNum进行有效性预处理，对于新数据予以更新，心跳予以确认
* @param void
* @return: true--继续下面处理，false--不再进行处理
* @update: [2018-08-03][张宇飞][创建]
*/
static bool reciveValidPredealTest(uint32_t appId, uint32_t lastSt, uint32_t lastSq, uint32_t stNum, uint32_t sqNum )
{

   
	//序列号错误设置为无效状态
	 if (lastSt == stNum) {
		if (lastSq >= sqNum) {

			
			printf("\n\nAppID: 0x%x sqNum Loss Error Sequence, last:%d , current: %d!\n\n" ,
                   appId, lastSq, sqNum);
            //return false;
            printf("Restart Recive!!!!!!!!\n");
			return true;
		}
		else
		{

			if ((lastSq + 1) != sqNum){
				 //if (DEBUG_GOOSE_SUBSCRIBER)
					 printf("AppID: 0x%x, SqNum Loss, last:%d , current: %d!\n",  
                            appId,  lastSq, sqNum);
			}
			//更新状态
			 
			 lastSt = stNum;
			 lastSq = sqNum;
			 
             
             printf("AppID: 0x%x, jump, st:%d , sq: %d!\n",  
                           appId,  stNum,  sqNum);
             
               
			 return false;
		}
	}
	else if (lastSt < stNum) {//新数据
		 if ((lastSt + 1) != stNum){
			 //if (DEBUG_GOOSE_SUBSCRIBER)
			  printf("\n\nAppID: 0x%x, stNum Loss, last:%d , current: %d!\n",
                     appId, lastSt, stNum);
			  return true;
		 }
	 }
	else
	{
		
		//if (DEBUG_GOOSE_SUBSCRIBER)
		printf("\n\nAppID: 0x%x, stNum Loss Error Sequence, last:%d , current: %d!\n" ,
               appId,  lastSt, stNum);
		//return false;
        printf("Restart Recive!!!!!!!!\n");
	    return true;
	}
     printf("AppID: 0x%x, valid, st:%d , sq: %d!\n",  
                           appId,  stNum,  sqNum);
	 return true;

}

typedef struct TagGooseCheck
{
	uint32_t (*testData)[3];
	uint32_t lastSqNum;
	uint32_t lastStNum;
	uint32_t sqNum;
	uint32_t stNum ;
	uint16_t index;
    
    uint32_t appId;
}GooseCheck;
typedef struct TagSequenceInformation
{	
    uint32_t stNum;
	uint32_t sqNum;
    uint32_t time;
}SequenceInformation;

#define GOOSE_CHECK_COUNT   10

GooseCheck GooseCheck1000;
GooseCheck GooseCheck1001;
GooseCheck GooseCheck1003;
GooseCheck GooseCheck1004;

static SequenceInformation*  SequenceInformation_create()
{
    SequenceInformation* inf = (SequenceInformation* )rt_calloc(sizeof(SequenceInformation), 1);
    if (! inf)
    {
    	perror(" (SequenceInformation* )rt_calloc(sizeof(SequenceInformation), 1)\n");
    	return NULL;
    }
    return inf;
}
static void  SequenceInformation_destory(SequenceInformation* pInfo)
{
    if (pInfo)
    {
        rt_free(pInfo);
    }
}


RingQueue GooseCheckRing;
RingQueue Ring1000;
RingQueue Ring1001;
RingQueue Ring1003;
RingQueue Ring1004;




static void GooseCheck_init(GooseCheck* check,uint32_t appId)
{
    check->testData = (uint32_t (*)[3])rt_calloc(sizeof(uint32_t), 1000*3);
    
    if (!check->testData)
    {
    	perror(" (uint32_t *)rt_calloc(sizeof(uint32_t), 1000*3)\n");
    	return;
    }


	check->lastSqNum = 0;
	check->lastSqNum = 0;
	check->index = 0;
	check->sqNum = 0;
	check->stNum = 0;
    check->appId = appId;
}

void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
   
    SequenceInformation* pInfo = SequenceInformation_create();
    if (pInfo)
    {
        RingQueue* ring = (RingQueue*)parameter;
        pInfo->stNum = GooseSubscriber_getStNum(subscriber);
        pInfo->sqNum = GooseSubscriber_getSqNum(subscriber);
        pInfo->time = StopWatchStop();
        ring->Write(ring, pInfo);
    }
    
    
}
//void
//gooseListener(GooseSubscriber subscriber, void* parameter)
//{
//	GooseCheck* check = (GooseCheck*)parameter;
//    
//    check->testData[check->index][2] = StopWatchStop();
//   // rt_kprintf("index: %d\n ", index);
//    check->testData[check->index][0] = GooseSubscriber_getStNum(subscriber);
//    check->testData[check->index][1] =  GooseSubscriber_getSqNum(subscriber);
//    check->index++;
//    if (check->index == 1000)
//    {
//        
//        check->lastStNum = check->stNum = check->testData[0][0];
//        check->lastSqNum = check->sqNum = check->testData[0][1];
//        for (uint16_t  i= 0 ; i < 1000; i++)
//        {
//            check->stNum = check->testData[i][0];
//            check->sqNum = check->testData[i][1];
//            reciveValidPredealTest(check->appId, check->lastStNum, check->lastSqNum, check->stNum,   check->sqNum);
//            //rt_kprintf("Time: %d\n ", check->testData[i][2]);
//            check->lastStNum = check->stNum;
//            check->lastSqNum =  check->sqNum;
//        }
//        check->index = 0;
//        
//    }        
//}


static void CheckSequence(RingQueue* pRing, uint32_t appId,  uint32_t* lastStNum, uint32_t* lastSqNum)
{
    //每500一组输出
    if (pRing->count > 1000)
    {
        for(uint16_t i = 0; i< 1000; i++)
        {
            SequenceInformation* pInfo;
            pRing->Read(pRing, (void**)(&pInfo));
            
            reciveValidPredealTest(appId, *lastStNum, *lastSqNum, pInfo->stNum,   pInfo->sqNum);
            *lastStNum =  pInfo->stNum;
            *lastSqNum = pInfo->sqNum;
            SequenceInformation_destory(pInfo);
        }
    }
    
}

void FtuIdleHook(void)
{
    static uint32_t lastSq1000 = 0;
    static uint32_t lastSt1000 = 0;
    static uint32_t lastSq1001 = 0;
    static uint32_t lastSt1001 = 0;
    static uint32_t lastSq1003 = 0;
    static uint32_t lastSt1003 = 0;
    static uint32_t lastSq1004 = 0;
    static uint32_t lastSt1004 = 0;   
    
    CheckSequence(&Ring1000,0x1000, &lastSt1000, &lastSq1000);
    CheckSequence(&Ring1001,0x1001, &lastSt1001, &lastSq1001);
    CheckSequence(&Ring1003,0x1003, &lastSt1003, &lastSq1003);
    CheckSequence(&Ring1004,0x1004, &lastSt1004, &lastSq1004);
    
    
    
}

 

int
subscriber_example(void)
{
    GooseReceiver receiver = GooseReceiver_create();

     
	printf("Using interface eth0\n");
	GooseReceiver_setInterfaceId(receiver, "eth0");
   



    GooseSubscriber subscriberRemote = GooseSubscriber_create("STU1LD0/LLN0$GO$gcbRemote", NULL);
	GooseSubscriber_setAppId(subscriberRemote, 0x1000);

	GooseSubscriber subscriberMeasure = GooseSubscriber_create("STU1MEAS/LLN0$GO$gcbMeasureValues", NULL);
	GooseSubscriber_setAppId(subscriberMeasure, 0x1001);
	
	GooseSubscriber subscriberIndicate = GooseSubscriber_create("STU1PROT/LLN0$GO$gcbDistriIndicate", NULL);
	GooseSubscriber_setAppId(subscriberIndicate, 0x1003);
	
	GooseSubscriber subscriberDeal = GooseSubscriber_create("STU1PROT/LLN0$GO$gcbDistriDeal", NULL);
	GooseSubscriber_setAppId(subscriberDeal, 0x1004);

    
//	GooseCheck_init(&GooseCheck1000, 0x1000);
//	GooseCheck_init(&GooseCheck1001, 0x1001);
//	GooseCheck_init(&GooseCheck1003, 0x1003);
//	GooseCheck_init(&GooseCheck1004, 0x1004);
    RingQueueInit(&Ring1000, 10000);
    RingQueueInit(&Ring1001, 10000);
    RingQueueInit(&Ring1003, 10000);
    RingQueueInit(&Ring1004, 10000);
    GooseSubscriber_setListener(subscriberRemote, gooseListener, &Ring1000);
	GooseSubscriber_setListener(subscriberMeasure, gooseListener, &Ring1001);
	GooseSubscriber_setListener(subscriberIndicate, gooseListener, &Ring1003);
	GooseSubscriber_setListener(subscriberDeal, gooseListener, &Ring1004);
	

    GooseReceiver_addSubscriber(receiver, subscriberRemote);
	GooseReceiver_addSubscriber(receiver, subscriberMeasure);
	GooseReceiver_addSubscriber(receiver, subscriberIndicate);
	GooseReceiver_addSubscriber(receiver, subscriberDeal);

    GooseReceiver_start(receiver);

    

//    while (1) {
//        Thread_sleep(100);
//    }

//    GooseReceiver_stop(receiver);

//    GooseReceiver_destroy(receiver);
    return 0;
}

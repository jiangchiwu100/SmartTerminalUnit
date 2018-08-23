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

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "iec61850_server.h"

#include "ied_data_ref.h"
const char  Ref1[][24] = {
	{ "GOINGGIO1.Ind1.stVal" },
	{ "GOINGGIO1.Ind2.stVal" },
	{ "GOINGGIO1.Ind3.stVal" },
	{ "GOINGGIO1.Ind4.stVal" },
	{ "GOINGGIO1.Ind5.stVal" },
	{ "GOINGGIO1.Ind6.stVal" },
	{ "GOINGGIO1.Ind7.stVal" },
	{ "GOINGGIO1.Ind8.stVal" },
	{ "GOINGGIO1.Ind9.stVal" },
	{ "GOINGGIO1.Ind10.stVal" },
	{ "GOINGGIO1.Ind11.stVal" },
	{ "GOINGGIO1.Ind12.stVal" },
	{ "GOINGGIO1.Ind13.stVal" },
	{ "GOINGGIO1.Ind14.stVal" },
	{ "GOINGGIO1.Ind15.stVal" },
	{ "GOINGGIO1.Ind16.stVal" },
	{ "GOINGGIO1.Alm1.stVal" },
	{ "GOINGGIO1.Alm2.stVal" },
	{ "GOINGGIO1.Alm3.stVal" },
	{ "GOINGGIO1.Alm4.stVal" },
	{ "GOINGGIO1.Alm5.stVal" },
	{ "GOINGGIO1.Alm6.stVal" },
	{ "GOINGGIO1.Alm7.stVal" },
	{ "GOINGGIO1.Alm8.stVal" },

};
const char Ref2[][24] = {
	{ "GOINGGIO2.Ind1.stVal"},
	{ "GOINGGIO2.Ind2.stVal" },
	{ "GOINGGIO2.Ind3.stVal" },
	{ "GOINGGIO2.Ind4.stVal" },
	{ "GOINGGIO2.Ind5.stVal" },
	{ "GOINGGIO2.Ind6.stVal" },
	{ "GOINGGIO2.Ind7.stVal" },
	{ "GOINGGIO2.Ind8.stVal" },
	{ "GOINGGIO2.Ind9.stVal" },
	{ "GOINGGIO2.Ind10.stVal" },
	{ "GOINGGIO2.Ind11.stVal" },
	{ "GOINGGIO2.Ind12.stVal" },
	{ "GOINGGIO2.Ind13.stVal" },
	{ "GOINGGIO2.Ind14.stVal" },
	{ "GOINGGIO2.Ind15.stVal" },
	{ "GOINGGIO2.Ind16.stVal" },
	{ "GOINGGIO2.Alm1.stVal" },
	{ "GOINGGIO2.Alm2.stVal" },
	{ "GOINGGIO2.Alm3.stVal" },
	{ "GOINGGIO2.Alm4.stVal" },
	{ "GOINGGIO2.Alm5.stVal" },
	{ "GOINGGIO2.Alm6.stVal" },
	{ "GOINGGIO2.Alm7.stVal" },
	{ "GOINGGIO2.Alm8.stVal" },

};
const char Ref3[][24] = {
	{ "GOINGGIO3.Ind1.stVal" },
	{ "GOINGGIO3.Ind2.stVal" },
	{ "GOINGGIO3.Ind3.stVal" },
	{ "GOINGGIO3.Ind4.stVal" },
	{ "GOINGGIO3.Ind5.stVal" },
	{ "GOINGGIO3.Ind6.stVal" },
	{ "GOINGGIO3.Ind7.stVal" },
	{ "GOINGGIO3.Ind8.stVal" },
	{ "GOINGGIO3.Ind9.stVal" },
	{ "GOINGGIO3.Ind10.stVal" },
	{ "GOINGGIO3.Ind11.stVal" },
	{ "GOINGGIO3.Ind12.stVal" },
	{ "GOINGGIO3.Ind13.stVal" },
	{ "GOINGGIO3.Ind14.stVal" },
	{ "GOINGGIO3.Ind15.stVal" },
	{ "GOINGGIO3.Ind16.stVal" },
	{ "GOINGGIO3.Alm1.stVal" },
	{ "GOINGGIO3.Alm2.stVal" },
	{ "GOINGGIO3.Alm3.stVal" },
	{ "GOINGGIO3.Alm4.stVal" },
	{ "GOINGGIO3.Alm5.stVal" },
	{ "GOINGGIO3.Alm6.stVal" },
	{ "GOINGGIO3.Alm7.stVal" },
	{ "GOINGGIO3.Alm8.stVal" },

};
const char Ref4[][24] = {
	{ "GOINGGIO4.Ind1.stVal" },
	{ "GOINGGIO4.Ind2.stVal" },
	{ "GOINGGIO4.Ind3.stVal" },
	{ "GOINGGIO4.Ind4.stVal" },
	{ "GOINGGIO4.Ind5.stVal" },
	{ "GOINGGIO4.Ind6.stVal" },
	{ "GOINGGIO4.Ind7.stVal" },
	{ "GOINGGIO4.Ind8.stVal" },
	{ "GOINGGIO4.Ind9.stVal" },
	{ "GOINGGIO4.Ind10.stVal" },
	{ "GOINGGIO4.Ind11.stVal" },
	{ "GOINGGIO4.Ind12.stVal" },
	{ "GOINGGIO4.Ind13.stVal" },
	{ "GOINGGIO4.Ind14.stVal" },
	{ "GOINGGIO4.Ind15.stVal" },
	{ "GOINGGIO4.Ind16.stVal" },
	{ "GOINGGIO4.Alm1.stVal" },
	{ "GOINGGIO4.Alm2.stVal" },
	{ "GOINGGIO4.Alm3.stVal" },
	{ "GOINGGIO4.Alm4.stVal" },
	{ "GOINGGIO4.Alm5.stVal" },
	{ "GOINGGIO4.Alm6.stVal" },
	{ "GOINGGIO4.Alm7.stVal" },
	{ "GOINGGIO4.Alm8.stVal" },

};










/*

void
gooseListenerMeasure(GooseSubscriber subscriber, void* parameter)
{
	printf("GOOSE event:\n");
	printf("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
		GooseSubscriber_getSqNum(subscriber));
	printf("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));

	uint64_t timestamp = GooseSubscriber_getTimestamp(subscriber);

	printf("  timestamp: %u.%u\n", (uint32_t)(timestamp / 1000), (uint32_t)(timestamp % 1000));

	MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);

	char buffer[1024];

	MmsValue_printToBuffer(values, buffer, 1024);

	printf("%s\n", buffer);
}
int
GooseSubscriberInstance(GooseReceiver receiver)
{
	
    

	printf("Using interface eth0\n");
	GooseReceiver_setInterfaceId(receiver, "eth0");
	GooseSubscriber subscriberRemote = GooseSubscriber_create("STU1LD0/LLN0$GO$gocb0", NULL);
	GooseSubscriber_setAppId(subscriberRemote, 0x1000);

	GooseSubscriber subscriberMeasure = GooseSubscriber_create("STU1MEAS/LLN0$GO$gocb1", NULL);
	GooseSubscriber_setAppId(subscriberMeasure, 0x1001);
	
	

    GooseSubscriber_setListener(subscriberRemote, gooseListenerRemote, NULL);
	GooseSubscriber_setListener(subscriberMeasure, gooseListenerMeasure, NULL);

	

    GooseReceiver_addSubscriber(receiver, subscriberRemote);
	GooseReceiver_addSubscriber(receiver, subscriberMeasure);


    GooseReceiver_start(receiver);

    

   // GooseReceiver_stop(receiver);

    //GooseReceiver_destroy(receiver);
	return 0;
}
*/
#include "extern_interface.h"
#include "miscellaneous.h"

#include "RingQueue.h"
/**
* @brief :??StNum?SqNum????????,?????????,??????
* @param void
* @return: true--??????,false--??????
* @update: [2018-08-03][???][??]
*/
static bool reciveValidPredealTest(uint32_t appId, uint32_t lastSt, uint32_t lastSq, uint32_t stNum, uint32_t sqNum )
{

   
	//????????????
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
			//????
			 
			 lastSt = stNum;
			 lastSq = sqNum;
			 
             
             printf("AppID: 0x%x, jump, st:%d , sq: %d!\n",  
                           appId,  stNum,  sqNum);
             
               
			 return false;
		}
	}
	else if (lastSt < stNum) {//???
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


void
gooseParese(GooseSubscriber subscriber, void* parameter)
{
    printf("GOOSE event:\n");
    printf("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
            GooseSubscriber_getSqNum(subscriber));
    printf("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));

    uint64_t timestamp = GooseSubscriber_getTimestamp(subscriber);

    printf("  timestamp: %u.%u\n", (uint32_t) (timestamp / 1000), (uint32_t) (timestamp % 1000));

    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);

    char buffer[1024];

    MmsValue_printToBuffer(values, buffer, 1024);

    printf("%s\n", buffer);
}

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
    gooseParese(subscriber, NULL);
    
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
    //?500????
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
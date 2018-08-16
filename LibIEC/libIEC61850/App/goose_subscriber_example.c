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
     printf("AppID: 0x%x, vlaid, st:%d , sq: %d!\n",  
                           appId,  stNum,  sqNum);
	 return true;

}
uint32_t TestData[1000][3] = {0};

uint32_t LastSqNum = 0;
uint32_t LastStNum = 0;
uint32_t SqNum = 0;
uint32_t StNum = 0;
uint16_t NumIndex = 0;
void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    
    TestData[NumIndex][2] = StopWatchStop();
   // rt_kprintf("index: %d\n ", NumIndex);
    TestData[NumIndex][0] = GooseSubscriber_getStNum(subscriber);
    TestData[NumIndex][1] =  GooseSubscriber_getSqNum(subscriber);
    NumIndex++;
    if (NumIndex == 1000)
    {
        LastStNum = StNum = TestData[0][0];
        LastSqNum = SqNum = TestData[0][1];        
        for (uint16_t  i= 0 ; i < 1000; i++)
        {
            StNum = TestData[i][0];
            SqNum = TestData[i][1];  
            reciveValidPredealTest(0x1001, LastStNum, LastSqNum, StNum,  SqNum);
            rt_kprintf("Time: %d\n ", TestData[i][2]);
            LastStNum = StNum;
            LastSqNum = SqNum;  
        }
        NumIndex = 0;
        
    }
        
    return;
    reciveValidPredealTest(0x1001, LastStNum, LastSqNum, GooseSubscriber_getStNum(subscriber), GooseSubscriber_getSqNum(subscriber));
    LastStNum = GooseSubscriber_getStNum(subscriber);
    LastSqNum =  GooseSubscriber_getSqNum(subscriber);
     return;
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

    GooseSubscriber_setListener(subscriberRemote, gooseListener, NULL);
	GooseSubscriber_setListener(subscriberMeasure, gooseListener, NULL);
	GooseSubscriber_setListener(subscriberIndicate, gooseListener, NULL);
	GooseSubscriber_setListener(subscriberDeal, gooseListener, NULL);
	

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

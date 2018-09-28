/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      goose_publisher_beat.c
* @brief:     goose发布节拍机制
* @version:   V0.0.1
* @author:    Zhang Yufei
* @date:      2018/8/4
* @update:
*/
#include "goose_publisher_beat.h"
#include <string.h>
#include <stdbool.h>
//#include <stdio.h>

#include "mms_value.h"
#include "goose_publisher.h"
#include "hal_thread.h"

#include "extern_interface.h"



static void BeatExecuteTask(void* parameter);




/**
* @brief  : goose 节拍周期处理
* @param  : void
* @update: [2018-08-04][张宇飞][]
*/
static void BeatExecuteTask(void* parameter)
{
	GooseBeat* beat = (GooseBeat*)parameter;
	//执行发送任务

	beat->Execute(beat);
	
	uint32_t nextTime = beat->beat[beat->next];
	//更新下一节拍
	if ((beat->next < beat->count) && (nextTime != 0))
	{
//        rt_timer_stop(&(beat->timer));
		rt_timer_control(&(beat->timer), RT_TIMER_CTRL_SET_TIME, &nextTime);
		rt_timer_start(&(beat->timer));
		if (beat->next + 1 < beat->count)
		{
			beat->next++;
		}
	}
	else
	{
		perror("BeatExecuteTask next\n");
	}

}

/**
* @brief  : 根据参数创建，首次按照min， min，2min，4min，max节拍
* @param  : void
* @update: [2018-08-24][张宇飞][创建，开始值从1开始]
*/
GooseBeat* GooseBeat_create(char* name, uint32_t min, uint32_t max,
		uint8_t (*Execute)(struct TagGooseBeat* handle) ,
		uint8_t (*FirstExecute)(struct TagGooseBeat* handle))
{
	uint8_t index = 0;
    uint32_t time = 0;
	if (min > max)
	{
		perror("Error parameter.\n");
		return NULL;
	}
	GooseBeat* pBeat = (GooseBeat*)rt_calloc(sizeof(GooseBeat), 1);
	if (!pBeat)
	{
		perror("rt_calloc\n");
		return NULL;
	}
	pBeat->beat[index++] = min;// min;
	pBeat->beat[index++] = min;//min
    time = min << 1;
    if (time > max)
    {
        time = max;
    }
	pBeat->beat[index++] = time;//time
    time = min << 2;
    if (time > max)
    {
        time = max;
    }
	pBeat->beat[index++] = time;//time
    pBeat->beat[index++] = max;		
	pBeat->count = index;
	pBeat->next = 0;
	pBeat->Execute = Execute;
	pBeat->FirstExecute = FirstExecute;
	rt_timer_init(&(pBeat->timer), name, BeatExecuteTask, pBeat, pBeat->beat[pBeat->next],
		RT_TIMER_FLAG_ONE_SHOT| RT_TIMER_FLAG_SOFT_TIMER);

    return pBeat;
}

/**
* @brief  : 用于开始，或者重新从0开始goose beat,
* @param  : void
* @update: [2018-08-24][张宇飞][]
*/
void GooseBeat_reTrigger(GooseBeat* pbeat)
{
	if (!pbeat)
	{
		perror("pbeat is NUll \n");
		return;
	}
    rt_timer_t ptimer = &(pbeat->timer);
	rt_timer_stop(ptimer);
	pbeat->next = 0;
    uint32_t nextTime = pbeat->beat[pbeat->next];
    pbeat->next = 1;
    rt_timer_control(ptimer, RT_TIMER_CTRL_SET_TIME, &nextTime);
    rt_timer_start(ptimer);
	pbeat->FirstExecute(pbeat);
	
}
/**
* @brief  : 删除
* @param  : void
* @update: [2018-08-24][张宇飞][]
*/
void GooseBeat_destory(GooseBeat* pbeat)
{
	if (!pbeat)
	{
		perror("pbeat is NUll \n");
		return;
	}
	rt_timer_stop(&(pbeat->timer));
	rt_timer_delete(&(pbeat->timer));
	rt_free(pbeat);
}


//uint8_t publisher_test(GooseBeat* beat)
//{
//	LinkedList dataSetValues = LinkedList_create();
//
//	LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(1234));
//	LinkedList_add(dataSetValues, MmsValue_newBinaryTime(false));
//	LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));
//
//	CommParameters gooseCommParameters;
//
//	gooseCommParameters.appId = 1000;
//	gooseCommParameters.dstAddress[0] = 0xFF;
//	gooseCommParameters.dstAddress[1] = 0xFF;
//	gooseCommParameters.dstAddress[2] = 0xFF;
//	gooseCommParameters.dstAddress[3] = 0xFF;
//	gooseCommParameters.dstAddress[4] = 0xFF;
//	gooseCommParameters.dstAddress[5] = 0xFF;
//	gooseCommParameters.vlanId = 0;
//	gooseCommParameters.vlanPriority = 4;
//
//	GoosePublisher publisher = GoosePublisher_create(&gooseCommParameters, "en0");
//
//	GoosePublisher_setGoCbRef(publisher, "simpleIOGenericIO/LLN0$GO$gcbAnalogValues");
//	GoosePublisher_setConfRev(publisher, 1);
//	GoosePublisher_setDataSetRef(publisher, "simpleIOGenericIO/LLN0$AnalogValues");
//
//	if (GoosePublisher_publish(publisher, dataSetValues) == -1)
//	{
//		printf("Error sending message!\n");
//	}
//
//	GoosePublisher_destroy(publisher);
//	return 0;
//}

//static GooseBeat Beat;

//extern int server61850_test(void) ;
/*
* @brief  : goose 测试
* @param  : void
* @update: [2018-08-04][张宇飞][]
*/
//void TestGooseBeat(void)
//{
//	uint8_t index = 0;

//	rt_memset(&Beat, 0, sizeof(GooseBeat));
//	Beat.beat[index++] = 2;
//	Beat.beat[index++] = 2;
//	Beat.beat[index++] = 4;
//	Beat.beat[index++] = 8;
//	Beat.beat[index++] = 500;
//	Beat.count = index;
//	Beat.next = 0;
//	Beat.Execute = publisher_test;
//    GooseBeatInit("goose1", &Beat);
//    rt_thread_delay(1000);
   // Beat.Execute(&Beat);
  //  rt_timer_start(&Beat.timer);
    
    //开始订阅
    //subscriber_main();
  //  server61850_test();
//    do
//    {
//        rt_thread_delay(5000);
//        rt_timer_stop(&Beat.timer);
//        Beat.next = 0;
//        Beat.Execute(&Beat);
//        rt_timer_start(&Beat.timer);
//    }while(1);
    
//}



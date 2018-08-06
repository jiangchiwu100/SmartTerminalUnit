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








/**
* @brief  : goose 节拍周期处理
* @param  : void
* @update: [2018-08-04][张宇飞][]
*/
void BeatExecuteTask(void* parameter)
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
* @brief  : goose 节拍发布机制初始化
* @param  : void
* @update: [2018-08-04][张宇飞][]
*/
void GooseBeatInit(const char* name, GooseBeat* beat)
{
	rt_timer_init(&(beat->timer), name, BeatExecuteTask, beat, beat->beat[beat->next], 
		RT_TIMER_FLAG_ONE_SHOT| RT_TIMER_FLAG_SOFT_TIMER);

}



uint8_t publisher_test(GooseBeat* beat)
{
	LinkedList dataSetValues = LinkedList_create();

	LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(1234));
	LinkedList_add(dataSetValues, MmsValue_newBinaryTime(false));
	LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));

	CommParameters gooseCommParameters;

	gooseCommParameters.appId = 1000;
	gooseCommParameters.dstAddress[0] = 0xFF;
	gooseCommParameters.dstAddress[1] = 0xFF;
	gooseCommParameters.dstAddress[2] = 0xFF;
	gooseCommParameters.dstAddress[3] = 0xFF;
	gooseCommParameters.dstAddress[4] = 0xFF;
	gooseCommParameters.dstAddress[5] = 0xFF;
	gooseCommParameters.vlanId = 0;
	gooseCommParameters.vlanPriority = 4;

	GoosePublisher publisher = GoosePublisher_create(&gooseCommParameters, "en0");

	GoosePublisher_setGoCbRef(publisher, "simpleIOGenericIO/LLN0$GO$gcbAnalogValues");
	GoosePublisher_setConfRev(publisher, 1);
	GoosePublisher_setDataSetRef(publisher, "simpleIOGenericIO/LLN0$AnalogValues");

	if (GoosePublisher_publish(publisher, dataSetValues) == -1) 
	{
		printf("Error sending message!\n");
	}

	GoosePublisher_destroy(publisher);
	return 0;
}

static GooseBeat Beat;
/**
* @brief  : goose 测试
* @param  : void
* @update: [2018-08-04][张宇飞][]
*/
void TestGooseBeat(void)
{
	uint8_t index = 0;

	rt_memset(&Beat, 0, sizeof(GooseBeat));
	Beat.beat[index++] = 2;
	Beat.beat[index++] = 2;
	Beat.beat[index++] = 4;
	Beat.beat[index++] = 8; 
	Beat.beat[index++] = 500;
	Beat.count = index;
	Beat.next = 0;
	Beat.Execute = publisher_test;
    GooseBeatInit("goose1", &Beat);
    rt_thread_delay(1000);
   // Beat.Execute(&Beat);
  //  rt_timer_start(&Beat.timer);
    
    //开始订阅
    subscriber_main();
//    do
//    {
//        rt_thread_delay(5000);
//        rt_timer_stop(&Beat.timer);
//        Beat.next = 0;
//        Beat.Execute(&Beat);
//        rt_timer_start(&Beat.timer);
//    }while(1);
    
}



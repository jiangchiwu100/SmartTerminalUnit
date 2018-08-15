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


void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    
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

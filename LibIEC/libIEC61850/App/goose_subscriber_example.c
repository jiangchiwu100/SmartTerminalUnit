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

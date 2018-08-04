/*
 * goose_publisher_example.c
 */

#include <stdint.h>
//#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include <stdio.h>

#include "mms_value.h"
#include "goose_publisher.h"
#include "hal_thread.h"

#include "extern_interface.h"

// has to be executed as root!
int
publisher_main(void)
{
    char* interface = "eth0";

 

    printf("Using interface %s\n", interface);

        MX_TIM10_Init();
        StartTimer();
	LinkedList dataSetValues = LinkedList_create();

	LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(1234));
	LinkedList_add(dataSetValues, MmsValue_newBinaryTime(false));
	LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));

	CommParameters gooseCommParameters;

	gooseCommParameters.appId = 1000;
	gooseCommParameters.dstAddress[0] = 0x10;
	gooseCommParameters.dstAddress[1] = 0xC3;
	gooseCommParameters.dstAddress[2] = 0x7B;
	gooseCommParameters.dstAddress[3] = 0x8F;
	gooseCommParameters.dstAddress[4] = 0xCd;
	gooseCommParameters.dstAddress[5] = 0x2C;
	gooseCommParameters.vlanId = 0;
	gooseCommParameters.vlanPriority = 4;

	/*
	 * Create a new GOOSE publisher instance. As the second parameter the interface
	 * name can be provided (e.g. "eth0" on a Linux system). If the second parameter
	 * is NULL the interface name as defined with CONFIG_ETHERNET_INTERFACE_ID in
	 * stack_config.h is used.
	 */
	GoosePublisher publisher = GoosePublisher_create(&gooseCommParameters, interface);

	GoosePublisher_setGoCbRef(publisher, "simpleIOGenericIO/LLN0$GO$gcbAnalogValues");
	GoosePublisher_setConfRev(publisher, 1);
	GoosePublisher_setDataSetRef(publisher, "simpleIOGenericIO/LLN0$AnalogValues");

	int i = 0;

	for (i = 0; i < 3; i++) {
		

		if (GoosePublisher_publish(publisher, dataSetValues) == -1) {
			printf("Error sending message!\n");
		}
        Thread_sleep(1000);
	}

	GoosePublisher_destroy(publisher);
    return 0;
}





/*
 *  server_example_goose.c
 *
 *  This example demonstrates how to use GOOSE publishing, Reporting and the
 *  control model.
 *
 */

#include "iec61850_server.h"
#include "hal_thread.h" /* for Thread_sleep() */
#include "miscellaneous.h"
//#include <stdlib.h>
//#include <stdio.h>
#include <extern_interface.h>

#include "static_model.h"

/* import IEC 61850 device model created from SCL-File */
extern IedModel iedModel;

static int running = 0;
static IedServer iedServer = NULL;

void sigint_handler(int signalId)
{
	running = 0;
}

void
controlHandlerForBinaryOutput(void* parameter, MmsValue* value)
{
    uint64_t timestamp = Hal_getTimeInMs();

    /*if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO1) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_t, timestamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO2) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_t, timestamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO3) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_t, timestamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3_stVal, value);
    }

    if (parameter == IEDMODEL_GenericIO_GGIO1_SPCSO4) {
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_t, timestamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4_stVal, value);
    }*/
}

int server61850_test(void) 
{

    
	iedServer = IedServer_create(&iedModel);
//	
//		if (argc > 1) 
//		{
//			char* ethernetIfcID = argv[1];
//		
//			printf("Using GOOSE interface: %s\n", ethernetIfcID);
//			IedServer_setGooseInterfaceId(iedServer, ethernetIfcID);
//		}

	/* MMS server will be instructed to start listening to client connections. */
	IedServer_start(iedServer, 102);

	/*IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO1, (ControlHandler) controlHandlerForBinaryOutput,
			IEDMODEL_GenericIO_GGIO1_SPCSO1);

	IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO2, (ControlHandler) controlHandlerForBinaryOutput,
				IEDMODEL_GenericIO_GGIO1_SPCSO2);

	IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO3, (ControlHandler) controlHandlerForBinaryOutput,
				IEDMODEL_GenericIO_GGIO1_SPCSO3);

	IedServer_setControlHandler(iedServer, IEDMODEL_GenericIO_GGIO1_SPCSO4, (ControlHandler) controlHandlerForBinaryOutput,
				IEDMODEL_GenericIO_GGIO1_SPCSO4);*/

	if (!IedServer_isRunning(iedServer)) {
		printf("Starting server failed! Exit.\n");
		IedServer_destroy(iedServer);
		return -1;
	}

	/* Start GOOSE publishing */
	IedServer_enableGoosePublishing(iedServer);

	running = 1; 

	

	float anIn1 = 0.f;

	uint32_t cn = 0;
	while (running) {

        
	    IedServer_lockDataModel(iedServer);
        
		//IedServer_updateBooleanAttributeValue(iedServer, IEDMODEL_LD0_GGIO1_Ind3_stVal, true);
		

     //   IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_t, Hal_getTimeInMs());
	  //  IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_GenericIO_GGIO1_AnIn1_mag_f, anIn1);

	    //IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsA_t, Hal_getTimeInMs());
	    //IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsA_cVal_mag_f, anIn1);
	
		//IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_Hz_mag, );
		//更新频率相关
		//MmsValue_setFloat(IEDMODEL_MEAS_MMXU1_Hz_mag_f->mmsValue , 50.01 + 0.001*cn);
		MmsValue_setBitStringFromInteger(IEDMODEL_MEAS_MMXU1_Hz_q->mmsValue, (uint32_t)QUALITY_VALIDITY_GOOD);
	    MmsValue_setUtcTimeMs(IEDMODEL_MEAS_MMXU1_Hz_t->mmsValue, Hal_getTimeInMs());
		MmsValue_setFloat(IEDMODEL_MEAS_MMXU1_Hz_mag_f->mmsValue, 50.01 + 0.001*cn);
		//IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_Hz_mag_f, 50.01 + 0.001*cn);




		//IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_Hz_mag_f, 50.01 + 0.001*cn);
		//IedServer_updateQuality(iedServer, IEDMODEL_MEAS_MMXU1_Hz_q, QUALITY_VALIDITY_GOOD);
		//IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_Hz_t, Hal_getTimeInMs());
		////三相 电压
		//IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsA_cVal_mag_f, 100 + 1*cn);
		//IedServer_updateQuality(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsA_q, QUALITY_VALIDITY_GOOD);
		//IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsA_t, Hal_getTimeInMs());

		MmsValue_setBitStringFromInteger(IEDMODEL_MEAS_MMXU1_PhV_phsA_q->mmsValue, (uint32_t)QUALITY_VALIDITY_GOOD);
		MmsValue_setUtcTimeMs(IEDMODEL_MEAS_MMXU1_PhV_phsA_t->mmsValue, Hal_getTimeInMs());
		MmsValue_setFloat(IEDMODEL_MEAS_MMXU1_PhV_phsA_cVal_mag_f->mmsValue, 100 + 1 * cn);
		//IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsA_cVal_mag_f, 100 + 1 * cn);

		//IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsB_cVal_mag_f, 110 + 1 * cn);
		//IedServer_updateQuality(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsB_q, QUALITY_VALIDITY_GOOD);
		//IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsB_t, Hal_getTimeInMs());
		MmsValue_setBitStringFromInteger(IEDMODEL_MEAS_MMXU1_PhV_phsB_q->mmsValue, (uint32_t)QUALITY_VALIDITY_GOOD);
		MmsValue_setUtcTimeMs(IEDMODEL_MEAS_MMXU1_PhV_phsB_t->mmsValue, Hal_getTimeInMs());
		MmsValue_setFloat(IEDMODEL_MEAS_MMXU1_PhV_phsB_cVal_mag_f->mmsValue, 110 + 1 * cn);


		//IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsC_cVal_mag_f, 120 + 1 * cn);
		//IedServer_updateQuality(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsC_q, QUALITY_VALIDITY_GOOD);
		//IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_PhV_phsC_t, Hal_getTimeInMs());
		MmsValue_setBitStringFromInteger(IEDMODEL_MEAS_MMXU1_PhV_phsC_q->mmsValue, (uint32_t)QUALITY_VALIDITY_GOOD);
		MmsValue_setUtcTimeMs(IEDMODEL_MEAS_MMXU1_PhV_phsC_t->mmsValue, Hal_getTimeInMs());
		MmsValue_setFloat(IEDMODEL_MEAS_MMXU1_PhV_phsC_cVal_mag_f->mmsValue, 130 + 1 * cn);
		////三相 电流
		//IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_A_phsA_cVal_mag_f, 1100 + 10 * cn);
		//IedServer_updateQuality(iedServer, IEDMODEL_MEAS_MMXU1_A_phsA_q, QUALITY_VALIDITY_GOOD);
		//IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_A_phsA_t, Hal_getTimeInMs());
		MmsValue_setBitStringFromInteger(IEDMODEL_MEAS_MMXU1_A_phsB_q->mmsValue, (uint32_t)QUALITY_VALIDITY_GOOD);
		MmsValue_setUtcTimeMs(IEDMODEL_MEAS_MMXU1_A_phsB_t->mmsValue, Hal_getTimeInMs());
		MmsValue_setFloat(IEDMODEL_MEAS_MMXU1_A_phsB_cVal_mag_f->mmsValue, 1000 + 10 * cn);

		//IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_A_phsB_cVal_mag_f, 1200 + 10 * cn);
		//IedServer_updateQuality(iedServer, IEDMODEL_MEAS_MMXU1_A_phsB_q, QUALITY_VALIDITY_GOOD);
		//IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_A_phsB_t, Hal_getTimeInMs());
		MmsValue_setBitStringFromInteger(IEDMODEL_MEAS_MMXU1_A_phsB_q->mmsValue, (uint32_t)QUALITY_VALIDITY_GOOD);
		MmsValue_setUtcTimeMs(IEDMODEL_MEAS_MMXU1_A_phsB_t->mmsValue, Hal_getTimeInMs());
		MmsValue_setFloat(IEDMODEL_MEAS_MMXU1_A_phsB_cVal_mag_f->mmsValue, 2000 + 10 * cn);

		//IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_A_phsC_cVal_mag_f, 1300 + 10 * cn);
		//IedServer_updateQuality(iedServer, IEDMODEL_MEAS_MMXU1_A_phsC_q, QUALITY_VALIDITY_GOOD);
		//IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_A_phsC_t, Hal_getTimeInMs());
		MmsValue_setBitStringFromInteger(IEDMODEL_MEAS_MMXU1_A_phsB_q->mmsValue, (uint32_t)QUALITY_VALIDITY_GOOD);
		MmsValue_setUtcTimeMs(IEDMODEL_MEAS_MMXU1_A_phsB_t->mmsValue, Hal_getTimeInMs());
		//MmsValue_setFloat(IEDMODEL_MEAS_MMXU1_A_phsB_cVal_mag_f->mmsValue, 3000 + 10 * cn);
        
        
        IedServer_updateFloatAttributeValue(iedServer, IEDMODEL_MEAS_MMXU1_A_phsC_cVal_mag_f, 3000 + 10 * cn);
                   
        
        
	    IedServer_unlockDataModel(iedServer);
        
	    anIn1 += 1;
		cn++;
		Thread_sleep(500);
	}

	/* stop MMS server - close TCP server socket and all client sockets */
	IedServer_stop(iedServer);

	/* Cleanup - free all resources */
	IedServer_destroy(iedServer);
} /* main() */

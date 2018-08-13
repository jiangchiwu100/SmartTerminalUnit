/*
 *  server_example_config_file.c
 *
 *  This example shows how to use dynamic server data model with a configuration file.
 *
 *  - How to open and parse the model configuration file
 *  - How to access data attributes by object reference strings
 *  - How to access data attributes by short addresses
 *
 *  Note: If building with cmake the vmd-filestore folder containing the configuration file
 *  (model.cfg) has to be copied to the folder where the example is executed!
 *  The configuration file can be created from the SCL(ICD) file with the Java tool genconfig.jar
 *  that is included in the source distribution of libiec61580.
 *
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <stdlib.h>
#include <stdio.h>

#include "hal_filesystem.h"
#include "iec61850_config_file_parser.h"

static int running = 0;



int config_flie_test(void)
{
    int tcpPort = 102;


    
    /* open configuration file */
    FileHandle configFile = FileSystem_openFile("//sojo//test_goose.cfg", false);

    if (configFile == NULL) {
        printf("Error opening config file!\n");

        return 1;
    }

    /* parse the configuration file and create the data model */
    IedModel* model = ConfigFileParser_createModelFromConfigFile(configFile);

    FileSystem_closeFile(configFile);

    if (model == NULL) {
        printf("Error parsing config file!\n");

        return 1;
    }

	IedServer iedServer = IedServer_create(model);

	/* Access to data attributes by object reference */
	DataAttribute* ggio_ind1_stvalue = (DataAttribute*)
		IedModel_getModelNodeByObjectReference(model, "STU1LD0/GGIO1.Ind1.stVal");
	
	if (ggio_ind1_stvalue == NULL)
		printf("Error getting STU1LD0/GGIO1.Ind1.stVal data attribute!\n");

    DataAttribute* hz_mag_f = (DataAttribute*)
            IedModel_getModelNodeByObjectReference(model, "STU1MEAS/MMXU1.Hz.mag.f");

    if (hz_mag_f == NULL)
        printf("Error getting STU1MEAS/MMXU1.Hz.mag.f data attribute!\n");


	//DataAttribute* anIn2_mag_f = (DataAttribute*)ModelNode_getChild((ModelNode*)anIn2_mag, "f"); */

    /* Access to data attributes by short address */
	/*DataAttribute* ggio_ind2_stvalue = (DataAttribute*)
			IedModel_getModelNodeByShortAddress(model, 101);

	DataAttribute* anIn2_t = (DataAttribute*)
			IedModel_getModelNodeByShortAddress(model, 102);*/

			/*DataAttribute* anIn2_mag_f = NULL;

			if (anIn2_mag == NULL)
				printf("Error getting AnIn2.mag data attribute!\n");
			else
				anIn2_mag_f = (DataAttribute*)ModelNode_getChild((ModelNode*)anIn2_mag, "f");*/

	IedServer_start(iedServer, tcpPort);

	if (!IedServer_isRunning(iedServer)) {
		printf("Starting server failed! Exit.\n");
		IedServer_destroy(iedServer);
		exit(-1);
	}

	/* Start GOOSE publishing */
	IedServer_enableGoosePublishing(iedServer);

	running = 1;



	float val = 0.f;

	MmsValue* floatValue = MmsValue_newFloat(val);
	bool flag = false;
	while (running) {

        StopWatchInit();
        StopWatchStart();
	    if (ggio_ind1_stvalue != NULL) {
			
            IedServer_lockDataModel(iedServer);            
			IedServer_updateBooleanAttributeValue(iedServer, ggio_ind1_stvalue, flag);
			flag = !flag;
           

            IedServer_unlockDataModel(iedServer);
	    }

	    if (hz_mag_f != NULL) {
	        MmsValue_setFloat(floatValue, 0.f - val);

	        IedServer_lockDataModel(iedServer);

            IedServer_updateAttributeValue(iedServer, hz_mag_f, floatValue);

            IedServer_unlockDataModel(iedServer);
	    }
        StopWatchStop();
	    val += 0.1f;

		Thread_sleep(1000);
	}

	MmsValue_delete(floatValue);

	IedServer_stop(iedServer);

	IedServer_destroy(iedServer);

	IedModel_destroy(model);
	
	

} /* main() */

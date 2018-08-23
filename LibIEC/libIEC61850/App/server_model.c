/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      server_model.c
  * @brief:     61850数据映射
  * @version:   V0.0.0
  * @author:    Zhang Yufei
  * @date:      2018-08-23
  * @update:
  */
#include "goose_receiver.h"
#include "iec61850_server.h"
#include "hal_thread.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hal_filesystem.h"
#include "iec61850_config_file_parser.h"
//#include "extern_interface.h"
#include "ied_data_ref.h"

#include "server_datamapping.h"




#include "Coordinator_def.h"

char* LPHD1_NEIGHBOURCOUNT_N = "LPHD1.NeighbourCountN.stVal";
char* LPHD1_NEIGHBOURCOUNT_M = "LPHD1.NeighbourCountM.stVal";

#define MODEL_CONFIG_PATH  "//sojo//test_goose.cfg"
//#define MODEL_CONFIG_PATH "H:\\CodeResourceLib\\Net\\IEC61850\\libIEC61850\\libiec61850-1.2.2-V2\\vs-2015\\examples\\server_example_config_file\\Debug\\stu_v0.01.cfg"
IedServer CurrentIedServer;



extern  char Ref1[][24];
void GetNeighbourCount(void)
{
	DataAttribute* da = (DataAttribute*)ModelNode_getChild((ModelNode*)IED_LD0, LPHD1_NEIGHBOURCOUNT_N);
	uint16_t countN = MmsValue_toInt32(da->mmsValue);
	 da = (DataAttribute*)ModelNode_getChild((ModelNode*)IED_LD0, LPHD1_NEIGHBOURCOUNT_M);
	uint16_t countM = MmsValue_toInt32(da->mmsValue);
	
	
//	NeighborCollect* nc =   NeighborCollect_create(3, 24);

//	for (uint8_t i = 0; i < 6; i++)
//	{
//		for (uint8_t k = 0; k < 24; k++)
//		{
//			nc->indicateCollect[i].daCollect[k] = (DataAttribute*)ModelNode_getChild((ModelNode*)IED_LD0, Ref1[k]);
//		}
//		
//	}


}


/**
  * @brief :iec61850App
  * @param  Svoid
  * @return: 0--
  * @update: [2018-08-15][创建]
  */
int Iec61850Server(void)
{
    int tcpPort = 102;


    IedModel* model = CreateIedModeFromConfig( MODEL_CONFIG_PATH);
    if (!model)
    {
        printf("CreateIedModeFromConfig is null\n");
        return -1;
    }
	GooseReceiver receiver;
    LogicalDeviceDataRefInit(model);
	receiver = GooseReceiver_create();
	//GooseSubscriberInstance(receiver);

	char buffer[1024];
	/*MmsValue* values = IED_LD0_LGOS1_GoCBRef_setSrcRef->mmsValue;
	MmsValue_printToBuffer(values, buffer, 1024);
	puts(buffer);
	 values = IED_LD0_LGOS2_GoCBRef_setSrcRef->mmsValue;
	MmsValue_printToBuffer(values, buffer, 1024);
	puts(buffer);*/

	
	

	CurrentIedServer = IedServer_create(model);
	IedServer_start(CurrentIedServer, tcpPort);

	if (!IedServer_isRunning(CurrentIedServer)) {
		printf("Starting server failed! Exit.\n");
		IedServer_destroy(CurrentIedServer);
		return -1;
	}

	DataSet* dsGoose = IedModel_lookupDataSet(model, "STU1LD0/LLN0$dsGoose");

	/* Start GOOSE publishing */
	IedServer_enableGoosePublishing(CurrentIedServer);

	GetNeighbourCount();
	int32_t cn = 0;
	float i = 100;
	float v = 1000;
	float f = 50;
	float angle = 1;
	float va = 10000;
	bool state = false;
	float ratio = 0;
	while (true) {

		ratio = 1- (float)cn*0.01;

		i = 100  * ratio;
		v = 1000  * ratio;
		f = 50  * ratio;
		angle = 1  * ratio;
		va = 10000  * ratio;
		state = !state;


		(cn++ > 10) ? (cn = -10) : (cn++);

		//统一更新
		IedServer_lockDataModel(CurrentIedServer);


		SetMeasure_TotaVA(va);
		/*SetMeasure_Hz(f);
		SetMeasure_PhV_A(v,  angle);
		SetMeasure_PhV_B(v,  angle);
		SetMeasure_PhV_C(v,  angle);
		SetMeasure_A_A(i,  angle);
		SetMeasure_A_B(i,  angle);
		SetMeasure_A_C(i,  angle);*/

		SetRemote_Ind1(state);
		SetRemote_Ind2(!state);
		SetRemote_Ind3(state);
		SetRemote_Ind4(!state);
		SetRemote_Ind5(state);
		SetRemote_Ind6(!state);
		SetRemote_Ind7(state);
		SetRemote_Ind8(!state);

		/*SetRemote_XCBR_Pos(DBPOS_ON);

		SetRemote_PTOC1(state, !state);
		SetRemote_SCPI1(state, !state, state, !state);
		SetRemote_SVPI1(state, !state, state, !state);
		SetRemote_SFPI1(state, !state, state, !state);
		SetRemote_AFSL1(state, !state);
		SetRemote_AFSI1(state, !state);*/


		//以此触发




		IedServer_unlockDataModel(CurrentIedServer);

		Thread_sleep(5000);



	}



	IedServer_stop(CurrentIedServer);

	IedServer_destroy(CurrentIedServer);

	IedModel_destroy(model);
	


}



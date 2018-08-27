/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      iec61850_datamapping.c
  * @brief:     61850数据映射
  * @version:   V0.0.0
  * @author:    Zhang Yufei
  * @date:      2018-08-14
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
#include "Coordinator.h"
#include "extern_interface.h"
#include "station_manager.h"

extern ServerModelManager g_ServerModelManager;
/**
  * @brief :创建IED Mode从配置文件
  * @param   char* path 文件路径 //sojo//test_goose.cfg
  * @return: 指向模型的指针
  * @update: [2018-08-14][创建]
  */
IedModel* CreateIedModeFromConfig( char* path)
{
	 /* open configuration file */
	    FileHandle configFile = FileSystem_openFile(path, false);

	    if (configFile == NULL) {
	        perror("Error opening config file!\n");
	        return NULL;
	    }

	    /* parse the configuration file and create the data model */
	    IedModel* model = ConfigFileParser_createModelFromConfigFile(configFile);

	    FileSystem_closeFile(configFile);

	    if (model == NULL) {
	        printf("Error parsing config file!\n");
	        return NULL;
	    }
	    return model;
}


#if IED_DATA_ENABLE_OTHER
/**
  * @brief :更新测量值
  * @param  float fva 视在功率
  * @return: 0--
  * @update: [2018-08-15][创建]
  */
void SetMeasure_TotaVA(float fva)
{
	MmsValue_setBitStringFromInteger(IED_MEAS_MMXU1_TotVA_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_MEAS_MMXU1_TotVA_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateFloatAttributeValue(g_ServerModelManager.server, IED_MEAS_MMXU1_TotVA_mag_f, fva);
}
/**
  * @brief :更新频率
  * @param  float fva 视在功率
  * @return: 0--
  * @update: [2018-08-15][创建]
  */
void SetMeasure_Hz(float fhz)
{
	MmsValue_setBitStringFromInteger(IED_MEAS_MMXU1_Hz_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_MEAS_MMXU1_Hz_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateFloatAttributeValue(g_ServerModelManager.server, IED_MEAS_MMXU1_Hz_mag_f, fhz);
}
/**
  * @brief :更新电压
  * @param  float f, 幅值
  * @param  float angle 角度
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetMeasure_PhV_A(float f, float angle)
{
	MmsValue_setBitStringFromInteger(IED_MEAS_MMXU1_PhV_phsA_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_MEAS_MMXU1_PhV_phsA_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setFloat(IED_MEAS_MMXU1_PhV_phsA_cVal_mag_f->mmsValue, f);
	IedServer_updateFloatAttributeValue(g_ServerModelManager.server, IED_MEAS_MMXU1_PhV_phsA_cVal_ang_f, angle);
}
/**
  * @brief :更新电压
  * @param  float f, 幅值
  * @param  float angle 角度
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetMeasure_PhV_B(float f, float angle)
{
	MmsValue_setBitStringFromInteger(IED_MEAS_MMXU1_PhV_phsB_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_MEAS_MMXU1_PhV_phsB_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setFloat(IED_MEAS_MMXU1_PhV_phsB_cVal_mag_f->mmsValue, f);
	IedServer_updateFloatAttributeValue(g_ServerModelManager.server, IED_MEAS_MMXU1_PhV_phsB_cVal_ang_f, angle);
}
/**
  * @brief :更新电压
  * @param  float f, 幅值
  * @param  float angle 角度
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetMeasure_PhV_C(float f, float angle)
{
	MmsValue_setBitStringFromInteger(IED_MEAS_MMXU1_PhV_phsC_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_MEAS_MMXU1_PhV_phsC_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setFloat(IED_MEAS_MMXU1_PhV_phsC_cVal_mag_f->mmsValue, f);
	IedServer_updateFloatAttributeValue(g_ServerModelManager.server, IED_MEAS_MMXU1_PhV_phsC_cVal_ang_f, angle);
}
/**
  * @brief :更新电流
  * @param  float f, 幅值
  * @param  float angle 角度
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetMeasure_A_A(float f, float angle)
{
	MmsValue_setBitStringFromInteger(IED_MEAS_MMXU1_A_phsA_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_MEAS_MMXU1_A_phsA_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setFloat(IED_MEAS_MMXU1_A_phsA_cVal_mag_f->mmsValue, f);
	IedServer_updateFloatAttributeValue(g_ServerModelManager.server, IED_MEAS_MMXU1_A_phsA_cVal_ang_f, angle);
}
/**
  * @brief :更新电流
  * @param  float f, 幅值
  * @param  float angle 角度
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetMeasure_A_B(float f, float angle)
{
	MmsValue_setBitStringFromInteger(IED_MEAS_MMXU1_A_phsB_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_MEAS_MMXU1_A_phsB_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setFloat(IED_MEAS_MMXU1_A_phsB_cVal_mag_f->mmsValue, f);
	IedServer_updateFloatAttributeValue(g_ServerModelManager.server, IED_MEAS_MMXU1_A_phsB_cVal_ang_f, angle);
}
/**
  * @brief :更新电流
  * @param  float f, 幅值
  * @param  float angle 角度
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetMeasure_A_C(float f, float angle)
{
	MmsValue_setBitStringFromInteger(IED_MEAS_MMXU1_A_phsC_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_MEAS_MMXU1_A_phsC_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setFloat(IED_MEAS_MMXU1_A_phsC_cVal_mag_f->mmsValue, f);
	IedServer_updateFloatAttributeValue(g_ServerModelManager.server, IED_MEAS_MMXU1_A_phsC_cVal_ang_f, angle);
}



/**
  * @brief :设置数字输入
  * @param  bool state
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_Ind1(bool state)
{
	MmsValue_setBitStringFromInteger(IED_LD0_GGIO1_Ind1_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_LD0_GGIO1_Ind1_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_LD0_GGIO1_Ind1_stVal, state);
}
/**
  * @brief :设置数字输入
  * @param  bool state
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_Ind2(bool state)
{
	MmsValue_setBitStringFromInteger(IED_LD0_GGIO1_Ind2_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_LD0_GGIO1_Ind2_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_LD0_GGIO1_Ind2_stVal, state);
}
/**
  * @brief :设置数字输入
  * @param  bool state
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_Ind3(bool state)
{
	MmsValue_setBitStringFromInteger(IED_LD0_GGIO1_Ind3_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_LD0_GGIO1_Ind3_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_LD0_GGIO1_Ind3_stVal, state);
}
/**
  * @brief :设置数字输入
  * @param  bool state
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_Ind4(bool state)
{
	MmsValue_setBitStringFromInteger(IED_LD0_GGIO1_Ind4_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_LD0_GGIO1_Ind4_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_LD0_GGIO1_Ind4_stVal, state);
}
/**
  * @brief :设置数字输入
  * @param  bool state
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_Ind5(bool state)
{
	MmsValue_setBitStringFromInteger(IED_LD0_GGIO1_Ind5_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_LD0_GGIO1_Ind5_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_LD0_GGIO1_Ind5_stVal, state);
}
/**
  * @brief :设置数字输入
  * @param  bool state
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_Ind6(bool state)
{
	MmsValue_setBitStringFromInteger(IED_LD0_GGIO1_Ind6_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_LD0_GGIO1_Ind6_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_LD0_GGIO1_Ind6_stVal, state);
}
/**
  * @brief :设置数字输入
  * @param  bool state
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_Ind7(bool state)
{
	MmsValue_setBitStringFromInteger(IED_LD0_GGIO1_Ind7_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_LD0_GGIO1_Ind7_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_LD0_GGIO1_Ind7_stVal, state);
}
/**
  * @brief :设置数字输入
  * @param  bool state
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_Ind8(bool state)
{
	MmsValue_setBitStringFromInteger(IED_LD0_GGIO1_Ind8_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_LD0_GGIO1_Ind8_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_LD0_GGIO1_Ind8_stVal, state);
}

/**
  * @brief :更新开关状态
  * @param   Dbpos dbpos
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_XCBR_Pos( Dbpos dbpos)
{
	MmsValue_setBitStringFromInteger(IED_CTRL_XCBR1_Pos_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_CTRL_XCBR1_Pos_t->mmsValue, Hal_getTimeInMs());
	//MmsValue_setBitStringFromIntegerBigEndian(IED_CTRL_XCBR1_Pos_stVal->mmsValue, dbpos);
	IedServer_udpateDbposValue(g_ServerModelManager.server, IED_CTRL_XCBR1_Pos_stVal, dbpos);
}
/**
  * @brief :设置过流保护状态
  * @param  bool str
  * @param  bool op
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_PTOC1(bool str, bool op)
{
	MmsValue_setBitStringFromInteger(IED_PROT_PTOC1_Str_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_PTOC1_Str_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setBoolean(IED_PROT_PTOC1_Str_general->mmsValue, str);
	MmsValue_setInt16(IED_PROT_PTOC1_Str_dirGeneral->mmsValue, (uint32_t)0); //forward

	MmsValue_setBitStringFromInteger(IED_PROT_PTOC1_Op_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_PTOC1_Op_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_PROT_PTOC1_Op_general, op);

}
/**
  * @brief :设置欠压
  * @param  bool str
  * @param  bool op
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_PTUV1(bool str, bool op)
{
   //没有此逻辑节点

}

/**
  * @brief :设置电流指示
  * @param  bool str
  * @param  bool op
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_SCPI1(bool a,bool b, bool c,  bool neut)
{
	MmsValue_setBitStringFromInteger(IED_PROT_SCPI1_Abc_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_SCPI1_Abc_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setBoolean(IED_PROT_SCPI1_Abc_phsA->mmsValue, a);
	MmsValue_setBoolean(IED_PROT_SCPI1_Abc_phsB->mmsValue, b);
	MmsValue_setBoolean(IED_PROT_SCPI1_Abc_phsC->mmsValue, c);
	MmsValue_setBoolean(IED_PROT_SCPI1_Abc_general->mmsValue, a && b && c);
	MmsValue_setBoolean(IED_PROT_SCPI1_Abc_neut->mmsValue, neut);


	//方向性
	MmsValue_setBitStringFromInteger(IED_PROT_SCPI1_DirPrs_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_SCPI1_DirPrs_t->mmsValue, Hal_getTimeInMs());

	MmsValue_setBoolean(IED_PROT_SCPI1_DirPrs_phsA->mmsValue, a);
	MmsValue_setInt16(IED_PROT_SCPI1_DirPrs_dirPhsA->mmsValue , (uint32_t)0);//unknown|forward
	MmsValue_setBoolean(IED_PROT_SCPI1_DirPrs_phsB->mmsValue, b);
	MmsValue_setInt16(IED_PROT_SCPI1_DirPrs_dirPhsB->mmsValue , (uint32_t)0);//unknown|forward
	MmsValue_setBoolean(IED_PROT_SCPI1_DirPrs_phsC->mmsValue, c);
	MmsValue_setInt16(IED_PROT_SCPI1_DirPrs_dirPhsC->mmsValue , (uint32_t)0);//unknown|forward

	MmsValue_setBoolean(IED_PROT_SCPI1_DirPrs_general->mmsValue, a && b && c);
	MmsValue_setInt16(IED_PROT_SCPI1_DirPrs_dirGeneral->mmsValue , (uint32_t)0);//unknown|forward


	MmsValue_setInt16(IED_PROT_SCPI1_DirPrs_dirNeut->mmsValue , (uint32_t)0);//unknown|forward
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server,IED_PROT_SCPI1_DirPrs_neut, neut);
}
/**
  * @brief :设置电压指示
  * @param  bool str
  * @param  bool op
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_SVPI1(bool a,bool b, bool c,  bool neut)
{
	MmsValue_setBitStringFromInteger(IED_PROT_SVPI1_Abc_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_SVPI1_Abc_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setBoolean(IED_PROT_SVPI1_Abc_phsA->mmsValue, a);
	MmsValue_setBoolean(IED_PROT_SVPI1_Abc_phsB->mmsValue, b);
	MmsValue_setBoolean(IED_PROT_SVPI1_Abc_phsC->mmsValue, c);
	MmsValue_setBoolean(IED_PROT_SVPI1_Abc_general->mmsValue, a && b && c);
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_PROT_SVPI1_Abc_neut, neut);

}
/**
  * @brief :设置故障指示
  * @param  bool str
  * @param  bool op
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_SFPI1(bool a,bool b, bool c,  bool neut)
{
	//方向性
	MmsValue_setBitStringFromInteger(IED_PROT_SFPI1_FltInd_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_SFPI1_FltInd_t->mmsValue, Hal_getTimeInMs());

	MmsValue_setBoolean(IED_PROT_SFPI1_FltInd_phsA->mmsValue, a);
	MmsValue_setInt16(IED_PROT_SFPI1_FltInd_dirPhsA->mmsValue , (uint32_t)0);//unknown|forward
	MmsValue_setBoolean(IED_PROT_SFPI1_FltInd_phsB->mmsValue, b);
	MmsValue_setInt16(IED_PROT_SFPI1_FltInd_dirPhsB->mmsValue , (uint32_t)0);//unknown|forward
	MmsValue_setBoolean(IED_PROT_SFPI1_FltInd_phsC->mmsValue, c);
	MmsValue_setInt16(IED_PROT_SFPI1_FltInd_dirPhsC->mmsValue , (uint32_t)0);//unknown|forward

	MmsValue_setBoolean(IED_PROT_SFPI1_FltInd_general->mmsValue, a && b && c);
	MmsValue_setInt16(IED_PROT_SFPI1_FltInd_dirGeneral->mmsValue , (uint32_t)0);//unknown|forward


	MmsValue_setInt16(IED_PROT_SFPI1_FltInd_dirNeut->mmsValue , (uint32_t)0);//unknown|forward
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_PROT_SFPI1_FltInd_neut, neut);
}
/**
  * @brief :设置故障定位
  * @param  bool str
  * @param  bool op
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_AFSL1(bool locInd, bool locStr)
{
	MmsValue_setBitStringFromInteger(IED_PROT_AFSL1_FltLocInd_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_AFSL1_FltLocInd_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setBoolean(IED_PROT_AFSL1_FltLocInd_general->mmsValue, locInd);

	MmsValue_setBitStringFromInteger(IED_PROT_AFSL1_FltLocStr_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_AFSL1_FltLocStr_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_PROT_AFSL1_FltLocStr_stVal, locStr);
}
/**
  * @brief :设置故障隔离
  * @param  bool str
  * @param  bool op
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_AFSI1(bool isoInd, bool isoStr)
{
	MmsValue_setBitStringFromInteger(IED_PROT_AFSI1_FltIsoInd_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_AFSI1_FltIsoInd_t->mmsValue, Hal_getTimeInMs());
	MmsValue_setBoolean(IED_PROT_AFSI1_FltIsoInd_stVal->mmsValue, isoInd);

	MmsValue_setBitStringFromInteger(IED_PROT_AFSI1_FltIsoStr_q->mmsValue , (uint32_t)QUALITY_VALIDITY_GOOD);
	MmsValue_setUtcTimeMs(IED_PROT_AFSI1_FltIsoStr_t->mmsValue, Hal_getTimeInMs());
	IedServer_updateBooleanAttributeValue(g_ServerModelManager.server, IED_PROT_AFSI1_FltIsoStr_stVal, isoStr);

}
/**
  * @brief :设置故障恢复
  * @param  bool str
  * @param  bool op
  * @return: void
  * @update: [2018-08-15][创建]
  */
void SetRemote_ASRC1(void)
{


}

#endif


/**
  * @brief :绑定本地开关状态
  * @param
  * @return:
  * @update: [2018-08-25][创建]
  */
void BindLocalSwitchStatus(void)
{
    if(g_StationManger.pWorkPoint)
    {
    	DeviceIndicate* di = DeviceIndicate_crate(24);
    	if (!di)
    	{
    		perror("Error : DeviceIndicate_crate\n");
    		return;
    	}
    	g_ServerModelManager.localPulicDataset = di;
    	di->id = g_StationManger.pWorkPoint->id;
    	uint16_t index = 0;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind1_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind2_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind3_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind4_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind5_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind6_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind7_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind8_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind9_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind10_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind11_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind12_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind13_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind14_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind15_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Ind16_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Alm1_stVal;    	
    	di->daCollect[index++] = IED_LD0_GGIO17_Alm2_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Alm3_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Alm4_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Alm5_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Alm6_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Alm7_stVal;
    	di->daCollect[index++] = IED_LD0_GGIO17_Alm8_stVal;
    	//check
        if (index != 24)
        {
            perror("Error: Num, req:%i, all: %d\n", 24, index);
        }
    	for(uint16_t i = 0; i < index; i++ )
    	{
    		if (!di->daCollect[i++])
    		{
    			perror("Error: Assigen, seq:%i, all: %d\n", i, index);
    		}
    	}
    	//赋值给本地
    	g_StationManger.pWorkPoint->topology.localSwitch->pDeviceIndicate = di;
    }
    else
    {
    	perror("BindLocalSwitchStatus. ERROR\n");
    }
}

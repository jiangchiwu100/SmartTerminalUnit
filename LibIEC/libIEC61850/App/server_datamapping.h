/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      iec61850_datamapping.h
  * @brief:     61850����ӳ��
  * @version:   V0.0.0
  * @author:    Zhang Yufei
  * @date:      2018-08-15
  * @update:
  */

#ifndef SERVER_DARAMAPPING_H_
#define SERVER_DARAMAPPING_H_


#include "iec61850_model.h"


IedModel* CreateIedModeFromConfig( char* path);

void SetMeasure_TotaVA(float fva);
void SetMeasure_Hz(float fhz);
void SetMeasure_PhV_A(float f, float angle);
void SetMeasure_PhV_B(float f, float angle);
void SetMeasure_PhV_C(float f, float angle);
void SetMeasure_A_A(float f, float angle);
void SetMeasure_A_B(float f, float angle);
void SetMeasure_A_C(float f, float angle);

void SetRemote_Ind1(bool state);
void SetRemote_Ind2(bool state);
void SetRemote_Ind3(bool state);
void SetRemote_Ind4(bool state);
void SetRemote_Ind5(bool state);
void SetRemote_Ind6(bool state);
void SetRemote_Ind7(bool state);
void SetRemote_Ind8(bool state);

void SetRemote_XCBR_Pos( Dbpos dbpos);
void SetRemote_PTOC1(bool str, bool op);
void SetRemote_SCPI1(bool a,bool b, bool c,  bool neut);
void SetRemote_SVPI1(bool a,bool b, bool c,  bool neut);
void SetRemote_SFPI1(bool a,bool b, bool c,  bool neut);
void SetRemote_AFSL1(bool locInd, bool locStr);
void SetRemote_AFSI1(bool isoInd, bool isoStr);
void SetRemote_ASRC1(void);

void BindLocalSwitchStatus(void);

#endif 

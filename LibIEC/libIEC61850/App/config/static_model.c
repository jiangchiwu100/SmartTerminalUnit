/*
 * static_model.c
 *
 * automatically generated from test_goose.icd
 */
#include "static_model.h"

static void initializeValues();

extern DataSet iedModelds_LD0_LLN0_RmoteSignal;
extern DataSet iedModelds_MEAS_LLN0_MeasureValues;
extern DataSet iedModelds_PROT_LLN0_DistriIndicate;
extern DataSet iedModelds_PROT_LLN0_DistriDeal;


extern DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda0;
extern DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda1;
extern DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda2;
extern DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda3;
extern DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda4;
extern DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda5;
extern DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda6;
extern DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda7;

DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda0 = {
  "LD0",
  false,
  "GGIO1$ST$Ind1$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_RmoteSignal_fcda1
};

DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda1 = {
  "LD0",
  false,
  "GGIO1$ST$Ind2$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_RmoteSignal_fcda2
};

DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda2 = {
  "LD0",
  false,
  "GGIO1$ST$Ind3$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_RmoteSignal_fcda3
};

DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda3 = {
  "LD0",
  false,
  "GGIO1$ST$Ind4$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_RmoteSignal_fcda4
};

DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda4 = {
  "LD0",
  false,
  "GGIO1$ST$Ind5$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_RmoteSignal_fcda5
};

DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda5 = {
  "LD0",
  false,
  "GGIO1$ST$Ind6$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_RmoteSignal_fcda6
};

DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda6 = {
  "LD0",
  false,
  "GGIO1$ST$Ind7$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_LD0_LLN0_RmoteSignal_fcda7
};

DataSetEntry iedModelds_LD0_LLN0_RmoteSignal_fcda7 = {
  "LD0",
  false,
  "GGIO1$ST$Ind8$stVal",
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_LD0_LLN0_RmoteSignal = {
  "LD0",
  "LLN0$RmoteSignal",
  8,
  &iedModelds_LD0_LLN0_RmoteSignal_fcda0,
  &iedModelds_MEAS_LLN0_MeasureValues
};

extern DataSetEntry iedModelds_MEAS_LLN0_MeasureValues_fcda0;
extern DataSetEntry iedModelds_MEAS_LLN0_MeasureValues_fcda1;
extern DataSetEntry iedModelds_MEAS_LLN0_MeasureValues_fcda2;

DataSetEntry iedModelds_MEAS_LLN0_MeasureValues_fcda0 = {
  "MEAS",
  false,
  "MMXU1$MX$TotVA",
  -1,
  NULL,
  NULL,
  &iedModelds_MEAS_LLN0_MeasureValues_fcda1
};

DataSetEntry iedModelds_MEAS_LLN0_MeasureValues_fcda1 = {
  "MEAS",
  false,
  "MMXU1$MX$PhV",
  -1,
  NULL,
  NULL,
  &iedModelds_MEAS_LLN0_MeasureValues_fcda2
};

DataSetEntry iedModelds_MEAS_LLN0_MeasureValues_fcda2 = {
  "MEAS",
  false,
  "MMXU1$MX$A",
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_MEAS_LLN0_MeasureValues = {
  "MEAS",
  "LLN0$MeasureValues",
  3,
  &iedModelds_MEAS_LLN0_MeasureValues_fcda0,
  &iedModelds_PROT_LLN0_DistriIndicate
};

extern DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda0;
extern DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda1;
extern DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda2;
extern DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda3;
extern DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda4;

DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda0 = {
  "CTRL",
  false,
  "XCBR1$ST$Pos$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_PROT_LLN0_DistriIndicate_fcda1
};

DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda1 = {
  "PROT",
  false,
  "PTOC1$ST$Str$general",
  -1,
  NULL,
  NULL,
  &iedModelds_PROT_LLN0_DistriIndicate_fcda2
};

DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda2 = {
  "PROT",
  false,
  "SCPI1$ST$Abc",
  -1,
  NULL,
  NULL,
  &iedModelds_PROT_LLN0_DistriIndicate_fcda3
};

DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda3 = {
  "PROT",
  false,
  "SVPI1$ST$Abc",
  -1,
  NULL,
  NULL,
  &iedModelds_PROT_LLN0_DistriIndicate_fcda4
};

DataSetEntry iedModelds_PROT_LLN0_DistriIndicate_fcda4 = {
  "PROT",
  false,
  "SFPI1$ST$FltInd",
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_PROT_LLN0_DistriIndicate = {
  "PROT",
  "LLN0$DistriIndicate",
  5,
  &iedModelds_PROT_LLN0_DistriIndicate_fcda0,
  &iedModelds_PROT_LLN0_DistriDeal
};

extern DataSetEntry iedModelds_PROT_LLN0_DistriDeal_fcda0;
extern DataSetEntry iedModelds_PROT_LLN0_DistriDeal_fcda1;
extern DataSetEntry iedModelds_PROT_LLN0_DistriDeal_fcda2;
extern DataSetEntry iedModelds_PROT_LLN0_DistriDeal_fcda3;

DataSetEntry iedModelds_PROT_LLN0_DistriDeal_fcda0 = {
  "PROT",
  false,
  "AFSL1$ST$FltLocInd$general",
  -1,
  NULL,
  NULL,
  &iedModelds_PROT_LLN0_DistriDeal_fcda1
};

DataSetEntry iedModelds_PROT_LLN0_DistriDeal_fcda1 = {
  "PROT",
  false,
  "AFSL1$ST$FltLocStr$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_PROT_LLN0_DistriDeal_fcda2
};

DataSetEntry iedModelds_PROT_LLN0_DistriDeal_fcda2 = {
  "PROT",
  false,
  "AFSI1$ST$FltIsoInd$stVal",
  -1,
  NULL,
  NULL,
  &iedModelds_PROT_LLN0_DistriDeal_fcda3
};

DataSetEntry iedModelds_PROT_LLN0_DistriDeal_fcda3 = {
  "PROT",
  false,
  "AFSI1$ST$FltIsoStr$stVal",
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_PROT_LLN0_DistriDeal = {
  "PROT",
  "LLN0$DistriDeal",
  4,
  &iedModelds_PROT_LLN0_DistriDeal_fcda0,
  NULL
};

LogicalDevice iedModel_LD0 = {
    LogicalDeviceModelType,
    "LD0",
    (ModelNode*) &iedModel,
    (ModelNode*) &iedModel_MEAS,
    (ModelNode*) &iedModel_LD0_LLN0
};

LogicalNode iedModel_LD0_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_LPHD1,
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
};

DataObject iedModel_LD0_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_LD0_LLN0,
    (ModelNode*) &iedModel_LD0_LLN0_Beh,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_stVal,
    0
};

DataAttribute iedModel_LD0_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    (ModelNode*) &iedModel_LD0_LLN0_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    106};

DataObject iedModel_LD0_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_LLN0,
    (ModelNode*) &iedModel_LD0_LLN0_Health,
    (ModelNode*) &iedModel_LD0_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LLN0_Beh,
    (ModelNode*) &iedModel_LD0_LLN0_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN0_Beh,
    (ModelNode*) &iedModel_LD0_LLN0_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN0_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_LD0_LLN0,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_LD0_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LLN0_Health,
    (ModelNode*) &iedModel_LD0_LLN0_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN0_Health,
    (ModelNode*) &iedModel_LD0_LLN0_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN0_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_LD0_LLN0,
    NULL,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_vendor,
    0
};

DataAttribute iedModel_LD0_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt_ldNs,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_LD0_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
};

DataObject iedModel_LD0_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_LD0_LPHD1,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_LD0_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_hwRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyNam_hwRev = {
    DataAttributeModelType,
    "hwRev",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyNam_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_serNum,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyNam_serNum = {
    DataAttributeModelType,
    "serNum",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_model,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyNam_model = {
    DataAttributeModelType,
    "model",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_location,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyNam_location = {
    DataAttributeModelType,
    "location",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam_mRID,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyNam_mRID = {
    DataAttributeModelType,
    "mRID",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_LD0_LPHD1,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth_stVal,
    0
};

DataAttribute iedModel_LD0_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_LD0_LPHD1,
    NULL,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_stVal,
    0
};

DataAttribute iedModel_LD0_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LPHD1_Proxy_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_GGIO1 = {
    LogicalNodeModelType,
    "GGIO1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_LGOS1,
    (ModelNode*) &iedModel_LD0_GGIO1_Mod,
};

DataObject iedModel_LD0_GGIO1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Beh,
    (ModelNode*) &iedModel_LD0_GGIO1_Mod_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Mod,
    (ModelNode*) &iedModel_LD0_GGIO1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Mod,
    (ModelNode*) &iedModel_LD0_GGIO1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Mod,
    (ModelNode*) &iedModel_LD0_GGIO1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_LD0_GGIO1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_LD0_GGIO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Health,
    (ModelNode*) &iedModel_LD0_GGIO1_Beh_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Beh,
    (ModelNode*) &iedModel_LD0_GGIO1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Beh,
    (ModelNode*) &iedModel_LD0_GGIO1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD0_GGIO1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_NamPlt,
    (ModelNode*) &iedModel_LD0_GGIO1_Health_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Health,
    (ModelNode*) &iedModel_LD0_GGIO1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Health,
    (ModelNode*) &iedModel_LD0_GGIO1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD0_GGIO1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_GGIO1_NamPlt_vendor,
    0
};

DataAttribute iedModel_LD0_GGIO1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_LD0_GGIO1_NamPlt,
    (ModelNode*) &iedModel_LD0_GGIO1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_LD0_GGIO1_NamPlt,
    (ModelNode*) &iedModel_LD0_GGIO1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_LD0_GGIO1_NamPlt,
    (ModelNode*) &iedModel_LD0_GGIO1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_GGIO1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_GGIO1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_GGIO1_Ind1 = {
    DataObjectModelType,
    "Ind1",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Ind1_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    1101};

DataAttribute iedModel_LD0_GGIO1_Ind1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    1102};

DataAttribute iedModel_LD0_GGIO1_Ind1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    1103};

DataAttribute iedModel_LD0_GGIO1_Ind1_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1104};

DataAttribute iedModel_LD0_GGIO1_Ind1_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1105};

DataAttribute iedModel_LD0_GGIO1_Ind1_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    1106};

DataAttribute iedModel_LD0_GGIO1_Ind1_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    1107};

DataAttribute iedModel_LD0_GGIO1_Ind1_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind1,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    1108};

DataObject iedModel_LD0_GGIO1_Ind2 = {
    DataObjectModelType,
    "Ind2",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Ind2_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    1201};

DataAttribute iedModel_LD0_GGIO1_Ind2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    1202};

DataAttribute iedModel_LD0_GGIO1_Ind2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    1203};

DataAttribute iedModel_LD0_GGIO1_Ind2_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1204};

DataAttribute iedModel_LD0_GGIO1_Ind2_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1205};

DataAttribute iedModel_LD0_GGIO1_Ind2_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    1206};

DataAttribute iedModel_LD0_GGIO1_Ind2_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    1207};

DataAttribute iedModel_LD0_GGIO1_Ind2_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind2,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    1208};

DataObject iedModel_LD0_GGIO1_Ind3 = {
    DataObjectModelType,
    "Ind3",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Ind3_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    1301};

DataAttribute iedModel_LD0_GGIO1_Ind3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    1302};

DataAttribute iedModel_LD0_GGIO1_Ind3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    1303};

DataAttribute iedModel_LD0_GGIO1_Ind3_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1304};

DataAttribute iedModel_LD0_GGIO1_Ind3_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1305};

DataAttribute iedModel_LD0_GGIO1_Ind3_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    1306};

DataAttribute iedModel_LD0_GGIO1_Ind3_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    1307};

DataAttribute iedModel_LD0_GGIO1_Ind3_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind3,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    1308};

DataObject iedModel_LD0_GGIO1_Ind4 = {
    DataObjectModelType,
    "Ind4",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Ind4_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    1401};

DataAttribute iedModel_LD0_GGIO1_Ind4_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    1402};

DataAttribute iedModel_LD0_GGIO1_Ind4_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    1403};

DataAttribute iedModel_LD0_GGIO1_Ind4_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1404};

DataAttribute iedModel_LD0_GGIO1_Ind4_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1405};

DataAttribute iedModel_LD0_GGIO1_Ind4_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    1406};

DataAttribute iedModel_LD0_GGIO1_Ind4_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    1407};

DataAttribute iedModel_LD0_GGIO1_Ind4_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind4,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    1408};

DataObject iedModel_LD0_GGIO1_Ind5 = {
    DataObjectModelType,
    "Ind5",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Ind5_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    1501};

DataAttribute iedModel_LD0_GGIO1_Ind5_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    1502};

DataAttribute iedModel_LD0_GGIO1_Ind5_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    1503};

DataAttribute iedModel_LD0_GGIO1_Ind5_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1504};

DataAttribute iedModel_LD0_GGIO1_Ind5_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1505};

DataAttribute iedModel_LD0_GGIO1_Ind5_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    1506};

DataAttribute iedModel_LD0_GGIO1_Ind5_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    1507};

DataAttribute iedModel_LD0_GGIO1_Ind5_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind5,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    1508};

DataObject iedModel_LD0_GGIO1_Ind6 = {
    DataObjectModelType,
    "Ind6",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Ind6_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    1601};

DataAttribute iedModel_LD0_GGIO1_Ind6_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    1602};

DataAttribute iedModel_LD0_GGIO1_Ind6_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    1603};

DataAttribute iedModel_LD0_GGIO1_Ind6_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1604};

DataAttribute iedModel_LD0_GGIO1_Ind6_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1605};

DataAttribute iedModel_LD0_GGIO1_Ind6_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    1606};

DataAttribute iedModel_LD0_GGIO1_Ind6_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    1607};

DataAttribute iedModel_LD0_GGIO1_Ind6_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind6,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    1608};

DataObject iedModel_LD0_GGIO1_Ind7 = {
    DataObjectModelType,
    "Ind7",
    (ModelNode*) &iedModel_LD0_GGIO1,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Ind7_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    1701};

DataAttribute iedModel_LD0_GGIO1_Ind7_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    1702};

DataAttribute iedModel_LD0_GGIO1_Ind7_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    1703};

DataAttribute iedModel_LD0_GGIO1_Ind7_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1704};

DataAttribute iedModel_LD0_GGIO1_Ind7_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1705};

DataAttribute iedModel_LD0_GGIO1_Ind7_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    1706};

DataAttribute iedModel_LD0_GGIO1_Ind7_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    1707};

DataAttribute iedModel_LD0_GGIO1_Ind7_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind7,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    1708};

DataObject iedModel_LD0_GGIO1_Ind8 = {
    DataObjectModelType,
    "Ind8",
    (ModelNode*) &iedModel_LD0_GGIO1,
    NULL,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8_stVal,
    0
};

DataAttribute iedModel_LD0_GGIO1_Ind8_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    1801};

DataAttribute iedModel_LD0_GGIO1_Ind8_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    1802};

DataAttribute iedModel_LD0_GGIO1_Ind8_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    1803};

DataAttribute iedModel_LD0_GGIO1_Ind8_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1804};

DataAttribute iedModel_LD0_GGIO1_Ind8_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    1805};

DataAttribute iedModel_LD0_GGIO1_Ind8_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    1806};

DataAttribute iedModel_LD0_GGIO1_Ind8_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8,
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    1807};

DataAttribute iedModel_LD0_GGIO1_Ind8_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_GGIO1_Ind8,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    1808};

LogicalNode iedModel_LD0_LGOS1 = {
    LogicalNodeModelType,
    "LGOS1",
    (ModelNode*) &iedModel_LD0,
    (ModelNode*) &iedModel_LD0_LGOS2,
    (ModelNode*) &iedModel_LD0_LGOS1_St,
};

DataObject iedModel_LD0_LGOS1_St = {
    DataObjectModelType,
    "St",
    (ModelNode*) &iedModel_LD0_LGOS1,
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS1_St_stVal,
    0
};

DataAttribute iedModel_LD0_LGOS1_St_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LGOS1_St,
    (ModelNode*) &iedModel_LD0_LGOS1_St_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_St_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LGOS1_St,
    (ModelNode*) &iedModel_LD0_LGOS1_St_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_St_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LGOS1_St,
    (ModelNode*) &iedModel_LD0_LGOS1_St_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_St_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_LGOS1_St,
    (ModelNode*) &iedModel_LD0_LGOS1_St_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_St_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_LGOS1_St,
    (ModelNode*) &iedModel_LD0_LGOS1_St_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_St_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_LGOS1_St,
    (ModelNode*) &iedModel_LD0_LGOS1_St_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_St_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_LGOS1_St,
    (ModelNode*) &iedModel_LD0_LGOS1_St_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_St_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LGOS1_St,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LGOS1_LastStNum = {
    DataObjectModelType,
    "LastStNum",
    (ModelNode*) &iedModel_LD0_LGOS1,
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum_stVal,
    0
};

DataAttribute iedModel_LD0_LGOS1_LastStNum_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_LastStNum_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_LastStNum_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_LastStNum_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_LastStNum_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_LastStNum_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_LastStNum_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_LastStNum_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LGOS1_LastStNum,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LGOS1_GoCBRef = {
    DataObjectModelType,
    "GoCBRef",
    (ModelNode*) &iedModel_LD0_LGOS1,
    NULL,
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef_setSrcRef,
    0
};

DataAttribute iedModel_LD0_LGOS1_GoCBRef_setSrcRef = {
    DataAttributeModelType,
    "setSrcRef",
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef_setTstRef,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    2101};

DataAttribute iedModel_LD0_LGOS1_GoCBRef_setTstRef = {
    DataAttributeModelType,
    "setTstRef",
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef_setSrcCB,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_GoCBRef_setSrcCB = {
    DataAttributeModelType,
    "setSrcCB",
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef_setTstCB,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    2102};

DataAttribute iedModel_LD0_LGOS1_GoCBRef_setTstCB = {
    DataAttributeModelType,
    "setTstCB",
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef_intAddr,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_GoCBRef_intAddr = {
    DataAttributeModelType,
    "intAddr",
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef_tstEna,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_255,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS1_GoCBRef_tstEna = {
    DataAttributeModelType,
    "tstEna",
    (ModelNode*) &iedModel_LD0_LGOS1_GoCBRef,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

LogicalNode iedModel_LD0_LGOS2 = {
    LogicalNodeModelType,
    "LGOS2",
    (ModelNode*) &iedModel_LD0,
    NULL,
    (ModelNode*) &iedModel_LD0_LGOS2_St,
};

DataObject iedModel_LD0_LGOS2_St = {
    DataObjectModelType,
    "St",
    (ModelNode*) &iedModel_LD0_LGOS2,
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS2_St_stVal,
    0
};

DataAttribute iedModel_LD0_LGOS2_St_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LGOS2_St,
    (ModelNode*) &iedModel_LD0_LGOS2_St_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_St_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LGOS2_St,
    (ModelNode*) &iedModel_LD0_LGOS2_St_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_St_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LGOS2_St,
    (ModelNode*) &iedModel_LD0_LGOS2_St_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_St_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_LGOS2_St,
    (ModelNode*) &iedModel_LD0_LGOS2_St_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_St_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_LGOS2_St,
    (ModelNode*) &iedModel_LD0_LGOS2_St_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_St_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_LGOS2_St,
    (ModelNode*) &iedModel_LD0_LGOS2_St_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_St_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_LGOS2_St,
    (ModelNode*) &iedModel_LD0_LGOS2_St_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_St_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LGOS2_St,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LGOS2_LastStNum = {
    DataObjectModelType,
    "LastStNum",
    (ModelNode*) &iedModel_LD0_LGOS2,
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum_stVal,
    0
};

DataAttribute iedModel_LD0_LGOS2_LastStNum_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_INT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_LastStNum_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_LastStNum_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_LastStNum_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_LastStNum_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_LastStNum_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_LastStNum_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum,
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_LastStNum_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_LD0_LGOS2_LastStNum,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LGOS2_GoCBRef = {
    DataObjectModelType,
    "GoCBRef",
    (ModelNode*) &iedModel_LD0_LGOS2,
    NULL,
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef_setSrcRef,
    0
};

DataAttribute iedModel_LD0_LGOS2_GoCBRef_setSrcRef = {
    DataAttributeModelType,
    "setSrcRef",
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef_setTstRef,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    2201};

DataAttribute iedModel_LD0_LGOS2_GoCBRef_setTstRef = {
    DataAttributeModelType,
    "setTstRef",
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef_setSrcCB,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_GoCBRef_setSrcCB = {
    DataAttributeModelType,
    "setSrcCB",
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef_setTstCB,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    2202};

DataAttribute iedModel_LD0_LGOS2_GoCBRef_setTstCB = {
    DataAttributeModelType,
    "setTstCB",
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef_intAddr,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_129,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_GoCBRef_intAddr = {
    DataAttributeModelType,
    "intAddr",
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef,
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef_tstEna,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_VISIBLE_STRING_255,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LGOS2_GoCBRef_tstEna = {
    DataAttributeModelType,
    "tstEna",
    (ModelNode*) &iedModel_LD0_LGOS2_GoCBRef,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};


LogicalDevice iedModel_MEAS = {
    LogicalDeviceModelType,
    "MEAS",
    (ModelNode*) &iedModel,
    (ModelNode*) &iedModel_CTRL,
    (ModelNode*) &iedModel_MEAS_LLN0
};

LogicalNode iedModel_MEAS_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_MEAS,
    (ModelNode*) &iedModel_MEAS_LPHD1,
    (ModelNode*) &iedModel_MEAS_LLN0_Mod,
};

DataObject iedModel_MEAS_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_MEAS_LLN0,
    (ModelNode*) &iedModel_MEAS_LLN0_Beh,
    (ModelNode*) &iedModel_MEAS_LLN0_Mod_stVal,
    0
};

DataAttribute iedModel_MEAS_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_MEAS_LLN0_Mod,
    (ModelNode*) &iedModel_MEAS_LLN0_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_LLN0_Mod,
    (ModelNode*) &iedModel_MEAS_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_LLN0_Mod,
    (ModelNode*) &iedModel_MEAS_LLN0_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_MEAS_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_MEAS_LLN0,
    (ModelNode*) &iedModel_MEAS_LLN0_Health,
    (ModelNode*) &iedModel_MEAS_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_MEAS_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_MEAS_LLN0_Beh,
    (ModelNode*) &iedModel_MEAS_LLN0_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_LLN0_Beh,
    (ModelNode*) &iedModel_MEAS_LLN0_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_LLN0_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_MEAS_LLN0,
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt,
    (ModelNode*) &iedModel_MEAS_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_MEAS_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_MEAS_LLN0_Health,
    (ModelNode*) &iedModel_MEAS_LLN0_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_LLN0_Health,
    (ModelNode*) &iedModel_MEAS_LLN0_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_LLN0_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_MEAS_LLN0,
    NULL,
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt_vendor,
    0
};

DataAttribute iedModel_MEAS_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt,
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt,
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt,
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt,
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt,
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt_ldNs,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_MEAS_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_MEAS_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_MEAS,
    (ModelNode*) &iedModel_MEAS_MMXU1,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam,
};

DataObject iedModel_MEAS_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_MEAS_LPHD1,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_MEAS_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam_hwRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_PhyNam_hwRev = {
    DataAttributeModelType,
    "hwRev",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_PhyNam_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam_serNum,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_PhyNam_serNum = {
    DataAttributeModelType,
    "serNum",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam_model,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_PhyNam_model = {
    DataAttributeModelType,
    "model",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam_location,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_PhyNam_location = {
    DataAttributeModelType,
    "location",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam_mRID,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_PhyNam_mRID = {
    DataAttributeModelType,
    "mRID",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_MEAS_LPHD1,
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyHealth_stVal,
    0
};

DataAttribute iedModel_MEAS_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyHealth_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyHealth_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_MEAS_LPHD1,
    NULL,
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy_stVal,
    0
};

DataAttribute iedModel_MEAS_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy,
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy,
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy,
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_Proxy_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy,
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_Proxy_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy,
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_Proxy_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy,
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_Proxy_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy,
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_LPHD1_Proxy_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_MEAS_MMXU1 = {
    LogicalNodeModelType,
    "MMXU1",
    (ModelNode*) &iedModel_MEAS,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_Mod,
};

DataObject iedModel_MEAS_MMXU1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_MEAS_MMXU1,
    (ModelNode*) &iedModel_MEAS_MMXU1_Beh,
    (ModelNode*) &iedModel_MEAS_MMXU1_Mod_stVal,
    0
};

DataAttribute iedModel_MEAS_MMXU1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_Mod,
    (ModelNode*) &iedModel_MEAS_MMXU1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_Mod,
    (ModelNode*) &iedModel_MEAS_MMXU1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_Mod,
    (ModelNode*) &iedModel_MEAS_MMXU1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_MEAS_MMXU1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_MMXU1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_MEAS_MMXU1,
    (ModelNode*) &iedModel_MEAS_MMXU1_Health,
    (ModelNode*) &iedModel_MEAS_MMXU1_Beh_stVal,
    0
};

DataAttribute iedModel_MEAS_MMXU1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_Beh,
    (ModelNode*) &iedModel_MEAS_MMXU1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_Beh,
    (ModelNode*) &iedModel_MEAS_MMXU1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_MMXU1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_MEAS_MMXU1,
    (ModelNode*) &iedModel_MEAS_MMXU1_NamPlt,
    (ModelNode*) &iedModel_MEAS_MMXU1_Health_stVal,
    0
};

DataAttribute iedModel_MEAS_MMXU1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_Health,
    (ModelNode*) &iedModel_MEAS_MMXU1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_Health,
    (ModelNode*) &iedModel_MEAS_MMXU1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_MMXU1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_MEAS_MMXU1,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_NamPlt_vendor,
    0
};

DataAttribute iedModel_MEAS_MMXU1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_MEAS_MMXU1_NamPlt,
    (ModelNode*) &iedModel_MEAS_MMXU1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_MEAS_MMXU1_NamPlt,
    (ModelNode*) &iedModel_MEAS_MMXU1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_MEAS_MMXU1_NamPlt,
    (ModelNode*) &iedModel_MEAS_MMXU1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_MMXU1_TotVA = {
    DataObjectModelType,
    "TotVA",
    (ModelNode*) &iedModel_MEAS_MMXU1,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_mag,
    0
};

DataAttribute iedModel_MEAS_MMXU1_TotVA_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_q,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    11101};

DataAttribute iedModel_MEAS_MMXU1_TotVA_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    11102};

DataAttribute iedModel_MEAS_MMXU1_TotVA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    11103};

DataAttribute iedModel_MEAS_MMXU1_TotVA_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_subMag,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    11104};

DataAttribute iedModel_MEAS_MMXU1_TotVA_subMag = {
    DataAttributeModelType,
    "subMag",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_subQ,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_subMag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    11105};

DataAttribute iedModel_MEAS_MMXU1_TotVA_subMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_subMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    11106};

DataAttribute iedModel_MEAS_MMXU1_TotVA_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_units,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    11107};

DataAttribute iedModel_MEAS_MMXU1_TotVA_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_db,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    11108};

DataAttribute iedModel_MEAS_MMXU1_TotVA_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_units,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_zeroDb,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    11109};

DataAttribute iedModel_MEAS_MMXU1_TotVA_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_sVC,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    11110};

DataAttribute iedModel_MEAS_MMXU1_TotVA_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    11111};

DataAttribute iedModel_MEAS_MMXU1_TotVA_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_sVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_smpRate,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_hhLim,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    11112};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_hhLim = {
    DataAttributeModelType,
    "hhLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_hLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_hhLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_hhLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_hhLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_hLim = {
    DataAttributeModelType,
    "hLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_lLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_hLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_hLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_hLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_lLim = {
    DataAttributeModelType,
    "lLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_llLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_lLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_lLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_lLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_llLim = {
    DataAttributeModelType,
    "llLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_min,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_llLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_llLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_llLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_max,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_min_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_min,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_max_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_rangeC_max,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_TotVA_smpRate = {
    DataAttributeModelType,
    "smpRate",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    11113};

DataAttribute iedModel_MEAS_MMXU1_TotVA_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_TotVA,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    11114};

DataObject iedModel_MEAS_MMXU1_Hz = {
    DataObjectModelType,
    "Hz",
    (ModelNode*) &iedModel_MEAS_MMXU1,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_mag,
    0
};

DataAttribute iedModel_MEAS_MMXU1_Hz_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_q,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    11201};

DataAttribute iedModel_MEAS_MMXU1_Hz_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    11202};

DataAttribute iedModel_MEAS_MMXU1_Hz_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    11203};

DataAttribute iedModel_MEAS_MMXU1_Hz_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_subMag,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    11204};

DataAttribute iedModel_MEAS_MMXU1_Hz_subMag = {
    DataAttributeModelType,
    "subMag",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_subQ,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_subMag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    11205};

DataAttribute iedModel_MEAS_MMXU1_Hz_subMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_subMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    11206};

DataAttribute iedModel_MEAS_MMXU1_Hz_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_units,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    11207};

DataAttribute iedModel_MEAS_MMXU1_Hz_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_db,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    11208};

DataAttribute iedModel_MEAS_MMXU1_Hz_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_units,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_zeroDb,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    11209};

DataAttribute iedModel_MEAS_MMXU1_Hz_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_sVC,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    11210};

DataAttribute iedModel_MEAS_MMXU1_Hz_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    11211};

DataAttribute iedModel_MEAS_MMXU1_Hz_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_sVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_smpRate,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_hhLim,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    11212};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_hhLim = {
    DataAttributeModelType,
    "hhLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_hLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_hhLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_hhLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_hhLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_hLim = {
    DataAttributeModelType,
    "hLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_lLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_hLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_hLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_hLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_lLim = {
    DataAttributeModelType,
    "lLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_llLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_lLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_lLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_lLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_llLim = {
    DataAttributeModelType,
    "llLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_min,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_llLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_llLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_llLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_max,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_min_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_min,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_max_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_rangeC_max,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_Hz_smpRate = {
    DataAttributeModelType,
    "smpRate",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    11213};

DataAttribute iedModel_MEAS_MMXU1_Hz_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_Hz,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    11214};

DataObject iedModel_MEAS_MMXU1_PhV = {
    DataObjectModelType,
    "PhV",
    (ModelNode*) &iedModel_MEAS_MMXU1,
    (ModelNode*) &iedModel_MEAS_MMXU1_A,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    0
};

DataObject iedModel_MEAS_MMXU1_PhV_phsA = {
    DataObjectModelType,
    "phsA",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_cVal,
    0
};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_q,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_cVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_cVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_cVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_cVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_cVal_ang_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_cVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_cVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subCVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_subCVal = {
    DataAttributeModelType,
    "subCVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subQ,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subCVal_mag,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_subCVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subCVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subCVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subCVal_mag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_subCVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subCVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_subCVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subCVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subCVal_ang_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_subCVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subCVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_units,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_db,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_units,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_zeroDb,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hhLim,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hhLim = {
    DataAttributeModelType,
    "hhLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hhLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hhLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hhLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hLim = {
    DataAttributeModelType,
    "hLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_lLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_hLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_lLim = {
    DataAttributeModelType,
    "lLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_llLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_lLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_lLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_lLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_llLim = {
    DataAttributeModelType,
    "llLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_min,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_llLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_llLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_llLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_max,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_min_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_min,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_max_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_rangeC_max,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_magSVC = {
    DataAttributeModelType,
    "magSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_magSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_magSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_magSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_magSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_magSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_angSVC = {
    DataAttributeModelType,
    "angSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_angRef,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_angSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_angSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_angSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_angSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_angSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_angRef = {
    DataAttributeModelType,
    "angRef",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_smpRate,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_smpRate = {
    DataAttributeModelType,
    "smpRate",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsA_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsA,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_MMXU1_PhV_phsB = {
    DataObjectModelType,
    "phsB",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_cVal,
    0
};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_q,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_cVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_cVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_cVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_cVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_cVal_ang_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_cVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_cVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subCVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_subCVal = {
    DataAttributeModelType,
    "subCVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subQ,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subCVal_mag,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_subCVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subCVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subCVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subCVal_mag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_subCVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subCVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_subCVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subCVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subCVal_ang_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_subCVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subCVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_units,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_db,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_units,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_zeroDb,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hhLim,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hhLim = {
    DataAttributeModelType,
    "hhLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hhLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hhLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hhLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hLim = {
    DataAttributeModelType,
    "hLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_lLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_hLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_lLim = {
    DataAttributeModelType,
    "lLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_llLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_lLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_lLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_lLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_llLim = {
    DataAttributeModelType,
    "llLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_min,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_llLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_llLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_llLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_max,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_min_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_min,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_max_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_rangeC_max,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_magSVC = {
    DataAttributeModelType,
    "magSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_magSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_magSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_magSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_magSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_magSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_angSVC = {
    DataAttributeModelType,
    "angSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_angRef,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_angSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_angSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_angSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_angSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_angSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_angRef = {
    DataAttributeModelType,
    "angRef",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_smpRate,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_smpRate = {
    DataAttributeModelType,
    "smpRate",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsB_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsB,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_MMXU1_PhV_phsC = {
    DataObjectModelType,
    "phsC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_angRef,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_cVal,
    0
};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_q,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_cVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_cVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_cVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_cVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_cVal_ang_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_cVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_cVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subCVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_subCVal = {
    DataAttributeModelType,
    "subCVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subQ,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subCVal_mag,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_subCVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subCVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subCVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subCVal_mag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_subCVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subCVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_subCVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subCVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subCVal_ang_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_subCVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subCVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_units,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_db,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_units,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_zeroDb,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hhLim,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hhLim = {
    DataAttributeModelType,
    "hhLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hhLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hhLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hhLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hLim = {
    DataAttributeModelType,
    "hLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_lLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_hLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_lLim = {
    DataAttributeModelType,
    "lLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_llLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_lLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_lLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_lLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_llLim = {
    DataAttributeModelType,
    "llLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_min,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_llLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_llLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_llLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_max,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_min_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_min,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_max_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_rangeC_max,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_magSVC = {
    DataAttributeModelType,
    "magSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_magSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_magSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_magSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_magSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_magSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_angSVC = {
    DataAttributeModelType,
    "angSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_angRef,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_angSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_angSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_angSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_angSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_angSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_angRef = {
    DataAttributeModelType,
    "angRef",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_smpRate,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_smpRate = {
    DataAttributeModelType,
    "smpRate",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_phsC_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_phsC,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_PhV_angRef = {
    DataAttributeModelType,
    "angRef",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV,
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    11801};

DataAttribute iedModel_MEAS_MMXU1_PhV_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_PhV,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    11802};

DataObject iedModel_MEAS_MMXU1_A = {
    DataObjectModelType,
    "A",
    (ModelNode*) &iedModel_MEAS_MMXU1,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    0
};

DataObject iedModel_MEAS_MMXU1_A_phsA = {
    DataObjectModelType,
    "phsA",
    (ModelNode*) &iedModel_MEAS_MMXU1_A,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_cVal,
    0
};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_q,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_cVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_cVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_cVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_cVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_cVal_ang_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_cVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_cVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subCVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_subCVal = {
    DataAttributeModelType,
    "subCVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subQ,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subCVal_mag,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_subCVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subCVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subCVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subCVal_mag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_subCVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subCVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_subCVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subCVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subCVal_ang_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_subCVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subCVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_units,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_db,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_units,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_zeroDb,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_hhLim,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_hhLim = {
    DataAttributeModelType,
    "hhLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_hLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_hhLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_hhLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_hhLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_hLim = {
    DataAttributeModelType,
    "hLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_lLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_hLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_hLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_hLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_lLim = {
    DataAttributeModelType,
    "lLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_llLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_lLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_lLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_lLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_llLim = {
    DataAttributeModelType,
    "llLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_min,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_llLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_llLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_llLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_max,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_min_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_min,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_max_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_rangeC_max,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_magSVC = {
    DataAttributeModelType,
    "magSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_magSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_magSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_magSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_magSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_magSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_angSVC = {
    DataAttributeModelType,
    "angSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_angRef,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_angSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_angSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_angSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_angSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_angSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_angRef = {
    DataAttributeModelType,
    "angRef",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_smpRate,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_smpRate = {
    DataAttributeModelType,
    "smpRate",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsA_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsA,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_MMXU1_A_phsB = {
    DataObjectModelType,
    "phsB",
    (ModelNode*) &iedModel_MEAS_MMXU1_A,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_cVal,
    0
};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_q,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_cVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_cVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_cVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_cVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_cVal_ang_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_cVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_cVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subCVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_subCVal = {
    DataAttributeModelType,
    "subCVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subQ,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subCVal_mag,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_subCVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subCVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subCVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subCVal_mag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_subCVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subCVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_subCVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subCVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subCVal_ang_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_subCVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subCVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_units,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_db,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_units,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_zeroDb,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_hhLim,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_hhLim = {
    DataAttributeModelType,
    "hhLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_hLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_hhLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_hhLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_hhLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_hLim = {
    DataAttributeModelType,
    "hLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_lLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_hLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_hLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_hLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_lLim = {
    DataAttributeModelType,
    "lLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_llLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_lLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_lLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_lLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_llLim = {
    DataAttributeModelType,
    "llLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_min,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_llLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_llLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_llLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_max,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_min_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_min,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_max_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_rangeC_max,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_magSVC = {
    DataAttributeModelType,
    "magSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_magSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_magSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_magSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_magSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_magSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_angSVC = {
    DataAttributeModelType,
    "angSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_angRef,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_angSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_angSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_angSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_angSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_angSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_angRef = {
    DataAttributeModelType,
    "angRef",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_smpRate,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_smpRate = {
    DataAttributeModelType,
    "smpRate",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsB_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsB,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_MEAS_MMXU1_A_phsC = {
    DataObjectModelType,
    "phsC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_angRef,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_cVal,
    0
};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_cVal = {
    DataAttributeModelType,
    "cVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_q,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_cVal_mag,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_cVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_cVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_cVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_cVal_mag_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_cVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_cVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_cVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_cVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_cVal_ang_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_cVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_cVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subEna,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subCVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_subCVal = {
    DataAttributeModelType,
    "subCVal",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subQ,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subCVal_mag,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_subCVal_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subCVal,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subCVal_ang,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subCVal_mag_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_subCVal_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subCVal_mag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_subCVal_ang = {
    DataAttributeModelType,
    "ang",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subCVal,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subCVal_ang_f,
    0,
    IEC61850_FC_SV,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_subCVal_ang_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subCVal_ang,
    NULL,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_units,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_db,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_units,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_db = {
    DataAttributeModelType,
    "db",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_zeroDb,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_zeroDb = {
    DataAttributeModelType,
    "zeroDb",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC = {
    DataAttributeModelType,
    "rangeC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_hhLim,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_hhLim = {
    DataAttributeModelType,
    "hhLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_hLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_hhLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_hhLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_hhLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_hLim = {
    DataAttributeModelType,
    "hLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_lLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_hLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_hLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_hLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_lLim = {
    DataAttributeModelType,
    "lLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_llLim,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_lLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_lLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_lLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_llLim = {
    DataAttributeModelType,
    "llLim",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_min,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_llLim_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_llLim_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_llLim,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_min = {
    DataAttributeModelType,
    "min",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_max,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_min_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_min_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_min,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_max = {
    DataAttributeModelType,
    "max",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC,
    NULL,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_max_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_rangeC_max_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_rangeC_max,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_magSVC = {
    DataAttributeModelType,
    "magSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_magSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_magSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_magSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_magSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_magSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_magSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_angSVC = {
    DataAttributeModelType,
    "angSVC",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_angRef,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_angSVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_angSVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_angSVC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_angSVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_angSVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_angSVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_angRef = {
    DataAttributeModelType,
    "angRef",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_smpRate,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_smpRate = {
    DataAttributeModelType,
    "smpRate",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_phsC_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_A_phsC,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_angRef = {
    DataAttributeModelType,
    "angRef",
    (ModelNode*) &iedModel_MEAS_MMXU1_A,
    (ModelNode*) &iedModel_MEAS_MMXU1_A_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_MEAS_MMXU1_A_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_MEAS_MMXU1_A,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};


LogicalDevice iedModel_CTRL = {
    LogicalDeviceModelType,
    "CTRL",
    (ModelNode*) &iedModel,
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_CTRL_LLN0
};

LogicalNode iedModel_CTRL_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_CTRL,
    (ModelNode*) &iedModel_CTRL_LPHD1,
    (ModelNode*) &iedModel_CTRL_LLN0_Mod,
};

DataObject iedModel_CTRL_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_CTRL_LLN0,
    (ModelNode*) &iedModel_CTRL_LLN0_Beh,
    (ModelNode*) &iedModel_CTRL_LLN0_Mod_stVal,
    0
};

DataAttribute iedModel_CTRL_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_LLN0_Mod,
    (ModelNode*) &iedModel_CTRL_LLN0_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_LLN0_Mod,
    (ModelNode*) &iedModel_CTRL_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_LLN0_Mod,
    (ModelNode*) &iedModel_CTRL_LLN0_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_CTRL_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_CTRL_LLN0,
    (ModelNode*) &iedModel_CTRL_LLN0_Health,
    (ModelNode*) &iedModel_CTRL_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_CTRL_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_LLN0_Beh,
    (ModelNode*) &iedModel_CTRL_LLN0_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_LLN0_Beh,
    (ModelNode*) &iedModel_CTRL_LLN0_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_LLN0_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_CTRL_LLN0,
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt,
    (ModelNode*) &iedModel_CTRL_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_CTRL_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_LLN0_Health,
    (ModelNode*) &iedModel_CTRL_LLN0_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_LLN0_Health,
    (ModelNode*) &iedModel_CTRL_LLN0_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_LLN0_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_CTRL_LLN0,
    NULL,
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt_vendor,
    0
};

DataAttribute iedModel_CTRL_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt,
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt,
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt,
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt,
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt,
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt_ldNs,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_CTRL_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_CTRL_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_CTRL,
    (ModelNode*) &iedModel_CTRL_CSWI1,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam,
};

DataObject iedModel_CTRL_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_CTRL_LPHD1,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_CTRL_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam_hwRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_PhyNam_hwRev = {
    DataAttributeModelType,
    "hwRev",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_PhyNam_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam_serNum,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_PhyNam_serNum = {
    DataAttributeModelType,
    "serNum",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam_model,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_PhyNam_model = {
    DataAttributeModelType,
    "model",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam_location,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_PhyNam_location = {
    DataAttributeModelType,
    "location",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam_mRID,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_PhyNam_mRID = {
    DataAttributeModelType,
    "mRID",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_CTRL_LPHD1,
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyHealth_stVal,
    0
};

DataAttribute iedModel_CTRL_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyHealth_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyHealth_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_CTRL_LPHD1,
    NULL,
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy_stVal,
    0
};

DataAttribute iedModel_CTRL_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy,
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy,
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy,
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_Proxy_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy,
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_Proxy_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy,
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_Proxy_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy,
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_Proxy_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy,
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_LPHD1_Proxy_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_CTRL_CSWI1 = {
    LogicalNodeModelType,
    "CSWI1",
    (ModelNode*) &iedModel_CTRL,
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey,
};

DataObject iedModel_CTRL_CSWI1_LocKey = {
    DataObjectModelType,
    "LocKey",
    (ModelNode*) &iedModel_CTRL_CSWI1,
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc,
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey_stVal,
    0
};

DataAttribute iedModel_CTRL_CSWI1_LocKey_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey,
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_LocKey_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey,
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_LocKey_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey,
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_LocKey_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey,
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_LocKey_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey,
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_LocKey_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey,
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_LocKey_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey,
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_LocKey_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_CSWI1_LocKey,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_CSWI1_Loc = {
    DataObjectModelType,
    "Loc",
    (ModelNode*) &iedModel_CTRL_CSWI1,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc_stVal,
    0
};

DataAttribute iedModel_CTRL_CSWI1_Loc_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc,
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Loc_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc,
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Loc_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc,
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Loc_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc,
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Loc_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc,
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Loc_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc,
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Loc_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc,
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Loc_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_CSWI1_Loc,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_CSWI1_OpOpn = {
    DataObjectModelType,
    "OpOpn",
    (ModelNode*) &iedModel_CTRL_CSWI1,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpOpn_general,
    0
};

DataAttribute iedModel_CTRL_CSWI1_OpOpn_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_CTRL_CSWI1_OpOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpOpn_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_OpOpn_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_CSWI1_OpOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpOpn_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_OpOpn_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_CSWI1_OpOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpOpn_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_OpOpn_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_CSWI1_OpOpn,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_CSWI1_SelOpn = {
    DataObjectModelType,
    "SelOpn",
    (ModelNode*) &iedModel_CTRL_CSWI1,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn_stVal,
    0
};

DataAttribute iedModel_CTRL_CSWI1_SelOpn_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelOpn_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelOpn_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelOpn_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelOpn_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelOpn_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelOpn_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelOpn_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelOpn,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_CSWI1_OpCls = {
    DataObjectModelType,
    "OpCls",
    (ModelNode*) &iedModel_CTRL_CSWI1,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpCls_general,
    0
};

DataAttribute iedModel_CTRL_CSWI1_OpCls_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_CTRL_CSWI1_OpCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpCls_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_OpCls_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_CSWI1_OpCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpCls_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_OpCls_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_CSWI1_OpCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_OpCls_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_OpCls_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_CSWI1_OpCls,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_CSWI1_SelCls = {
    DataObjectModelType,
    "SelCls",
    (ModelNode*) &iedModel_CTRL_CSWI1,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls_stVal,
    0
};

DataAttribute iedModel_CTRL_CSWI1_SelCls_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelCls_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelCls_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelCls_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelCls_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelCls_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelCls_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls,
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_SelCls_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_CSWI1_SelCls,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_CSWI1_Pos = {
    DataObjectModelType,
    "Pos",
    (ModelNode*) &iedModel_CTRL_CSWI1,
    NULL,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw,
    0
};

DataAttribute iedModel_CTRL_CSWI1_Pos_SBOw = {
    DataAttributeModelType,
    "SBOw",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_SBOw_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_SBOw_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_ctlNum,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_SBOw_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_origin,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_SBOw_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_SBOw_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_SBOw_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_SBOw_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_SBOw_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_SBOw,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_ctlNum,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_origin,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_stVal,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel_ctlNum,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel_origin,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_CODEDENUM,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_CODEDENUM,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_pulseConfig,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_pulseConfig = {
    DataAttributeModelType,
    "pulseConfig",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_ctlModel,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_pulseConfig_cmdQual,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_pulseConfig_cmdQual = {
    DataAttributeModelType,
    "cmdQual",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_pulseConfig,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_pulseConfig_onDur,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_pulseConfig_onDur = {
    DataAttributeModelType,
    "onDur",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_pulseConfig,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_pulseConfig_offDur,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_pulseConfig_offDur = {
    DataAttributeModelType,
    "offDur",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_pulseConfig,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_pulseConfig_numPls,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_pulseConfig_numPls = {
    DataAttributeModelType,
    "numPls",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_pulseConfig,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_CSWI1_Pos_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_CSWI1_Pos,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_CTRL_XCBR1 = {
    LogicalNodeModelType,
    "XCBR1",
    (ModelNode*) &iedModel_CTRL,
    NULL,
    (ModelNode*) &iedModel_CTRL_XCBR1_Mod,
};

DataObject iedModel_CTRL_XCBR1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_Beh,
    (ModelNode*) &iedModel_CTRL_XCBR1_Mod_stVal,
    0
};

DataAttribute iedModel_CTRL_XCBR1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Mod,
    (ModelNode*) &iedModel_CTRL_XCBR1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_Mod,
    (ModelNode*) &iedModel_CTRL_XCBR1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_Mod,
    (ModelNode*) &iedModel_CTRL_XCBR1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_CTRL_XCBR1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_Health,
    (ModelNode*) &iedModel_CTRL_XCBR1_Beh_stVal,
    0
};

DataAttribute iedModel_CTRL_XCBR1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Beh,
    (ModelNode*) &iedModel_CTRL_XCBR1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_Beh,
    (ModelNode*) &iedModel_CTRL_XCBR1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_NamPlt,
    (ModelNode*) &iedModel_CTRL_XCBR1_Health_stVal,
    0
};

DataAttribute iedModel_CTRL_XCBR1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Health,
    (ModelNode*) &iedModel_CTRL_XCBR1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_Health,
    (ModelNode*) &iedModel_CTRL_XCBR1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey,
    (ModelNode*) &iedModel_CTRL_XCBR1_NamPlt_vendor,
    0
};

DataAttribute iedModel_CTRL_XCBR1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_CTRL_XCBR1_NamPlt,
    (ModelNode*) &iedModel_CTRL_XCBR1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_CTRL_XCBR1_NamPlt,
    (ModelNode*) &iedModel_CTRL_XCBR1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_CTRL_XCBR1_NamPlt,
    (ModelNode*) &iedModel_CTRL_XCBR1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_XCBR1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_LocKey = {
    DataObjectModelType,
    "LocKey",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc,
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey_stVal,
    0
};

DataAttribute iedModel_CTRL_XCBR1_LocKey_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey,
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_LocKey_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey,
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_LocKey_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey,
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_LocKey_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey,
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_LocKey_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey,
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_LocKey_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey,
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_LocKey_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey,
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_LocKey_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_XCBR1_LocKey,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_Loc = {
    DataObjectModelType,
    "Loc",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_Opcnt,
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc_stVal,
    0
};

DataAttribute iedModel_CTRL_XCBR1_Loc_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc,
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Loc_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc,
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Loc_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc,
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Loc_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc,
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Loc_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc,
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Loc_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc,
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Loc_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc,
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Loc_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_XCBR1_Loc,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_Opcnt = {
    DataObjectModelType,
    "Opcnt",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Opcnt_stVal,
    0
};

DataAttribute iedModel_CTRL_XCBR1_Opcnt_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Opcnt,
    (ModelNode*) &iedModel_CTRL_XCBR1_Opcnt_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Opcnt_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_Opcnt,
    (ModelNode*) &iedModel_CTRL_XCBR1_Opcnt_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Opcnt_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_Opcnt,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_Pos = {
    DataObjectModelType,
    "Pos",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw,
    0
};

DataAttribute iedModel_CTRL_XCBR1_Pos_SBOw = {
    DataAttributeModelType,
    "SBOw",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_SBOw_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_SBOw_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_ctlNum,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_SBOw_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_origin,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_SBOw_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_SBOw_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_SBOw_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_SBOw_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_SBOw_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_SBOw,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_ctlNum,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_origin,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_stVal,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel_ctlNum,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel_origin,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_CODEDENUM,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_CODEDENUM,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_pulseConfig,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_pulseConfig = {
    DataAttributeModelType,
    "pulseConfig",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_ctlModel,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_pulseConfig_cmdQual,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_pulseConfig_cmdQual = {
    DataAttributeModelType,
    "cmdQual",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_pulseConfig,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_pulseConfig_onDur,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_pulseConfig_onDur = {
    DataAttributeModelType,
    "onDur",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_pulseConfig,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_pulseConfig_offDur,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_pulseConfig_offDur = {
    DataAttributeModelType,
    "offDur",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_pulseConfig,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_pulseConfig_numPls,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_pulseConfig_numPls = {
    DataAttributeModelType,
    "numPls",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_pulseConfig,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_Pos_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_XCBR1_Pos,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_BlkOpn = {
    DataObjectModelType,
    "BlkOpn",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw,
    0
};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_SBOw = {
    DataAttributeModelType,
    "SBOw",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_SBOw_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_SBOw_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_ctlNum,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_SBOw_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_origin,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_SBOw_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_SBOw_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_SBOw_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_SBOw_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_SBOw_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_SBOw,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_ctlNum,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_origin,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_stVal,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel_ctlNum,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel_origin,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_pulseConfig,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_pulseConfig = {
    DataAttributeModelType,
    "pulseConfig",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_ctlModel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_pulseConfig_cmdQual,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_pulseConfig_cmdQual = {
    DataAttributeModelType,
    "cmdQual",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_pulseConfig,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_pulseConfig_onDur,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_pulseConfig_onDur = {
    DataAttributeModelType,
    "onDur",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_pulseConfig,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_pulseConfig_offDur,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_pulseConfig_offDur = {
    DataAttributeModelType,
    "offDur",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_pulseConfig,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_pulseConfig_numPls,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_pulseConfig_numPls = {
    DataAttributeModelType,
    "numPls",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_pulseConfig,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkOpn_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkOpn,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_BlkCls = {
    DataObjectModelType,
    "BlkCls",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    (ModelNode*) &iedModel_CTRL_XCBR1_CBOpCap,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw,
    0
};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_SBOw = {
    DataAttributeModelType,
    "SBOw",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_SBOw_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_SBOw_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_ctlNum,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_SBOw_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_origin,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_SBOw_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_SBOw_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_SBOw_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_SBOw_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_SBOw_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_SBOw,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_ctlNum,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_origin,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Cancel = {
    DataAttributeModelType,
    "Cancel",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_stVal,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Cancel_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Cancel_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel_ctlNum,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Cancel_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel_origin,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Cancel_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Cancel_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Cancel_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_Cancel_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_Cancel,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_pulseConfig,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_pulseConfig = {
    DataAttributeModelType,
    "pulseConfig",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_ctlModel,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_pulseConfig_cmdQual,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_pulseConfig_cmdQual = {
    DataAttributeModelType,
    "cmdQual",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_pulseConfig,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_pulseConfig_onDur,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_pulseConfig_onDur = {
    DataAttributeModelType,
    "onDur",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_pulseConfig,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_pulseConfig_offDur,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_pulseConfig_offDur = {
    DataAttributeModelType,
    "offDur",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_pulseConfig,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_pulseConfig_numPls,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_pulseConfig_numPls = {
    DataAttributeModelType,
    "numPls",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_pulseConfig,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_sboTimeout,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_sboTimeout = {
    DataAttributeModelType,
    "sboTimeout",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_sboClass,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_sboClass = {
    DataAttributeModelType,
    "sboClass",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_BlkCls_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_CTRL_XCBR1_BlkCls,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_CTRL_XCBR1_CBOpCap = {
    DataObjectModelType,
    "CBOpCap",
    (ModelNode*) &iedModel_CTRL_XCBR1,
    NULL,
    (ModelNode*) &iedModel_CTRL_XCBR1_CBOpCap_stVal,
    0
};

DataAttribute iedModel_CTRL_XCBR1_CBOpCap_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_CTRL_XCBR1_CBOpCap,
    (ModelNode*) &iedModel_CTRL_XCBR1_CBOpCap_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_CBOpCap_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_CTRL_XCBR1_CBOpCap,
    (ModelNode*) &iedModel_CTRL_XCBR1_CBOpCap_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_CTRL_XCBR1_CBOpCap_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_CTRL_XCBR1_CBOpCap,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};


LogicalDevice iedModel_PROT = {
    LogicalDeviceModelType,
    "PROT",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_PROT_LLN0
};

LogicalNode iedModel_PROT_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_LPHD1,
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
};

DataObject iedModel_PROT_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_LLN0,
    (ModelNode*) &iedModel_PROT_LLN0_Beh,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_stVal,
    0
};

DataAttribute iedModel_PROT_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
    (ModelNode*) &iedModel_PROT_LLN0_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LLN0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_LLN0,
    (ModelNode*) &iedModel_PROT_LLN0_Health,
    (ModelNode*) &iedModel_PROT_LLN0_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_LLN0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LLN0_Beh,
    (ModelNode*) &iedModel_PROT_LLN0_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LLN0_Beh,
    (ModelNode*) &iedModel_PROT_LLN0_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LLN0_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LLN0_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_PROT_LLN0,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_Health_stVal,
    0
};

DataAttribute iedModel_PROT_LLN0_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LLN0_Health,
    (ModelNode*) &iedModel_PROT_LLN0_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LLN0_Health,
    (ModelNode*) &iedModel_PROT_LLN0_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LLN0_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LLN0_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_PROT_LLN0,
    NULL,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_vendor,
    0
};

DataAttribute iedModel_PROT_LLN0_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_configRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_NamPlt_configRev = {
    DataAttributeModelType,
    "configRev",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt_ldNs,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LLN0_NamPlt_ldNs = {
    DataAttributeModelType,
    "ldNs",
    (ModelNode*) &iedModel_PROT_LLN0_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_EX,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
};

DataObject iedModel_PROT_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_PROT_LPHD1,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_vendor,
    0
};

DataAttribute iedModel_PROT_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_hwRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_hwRev = {
    DataAttributeModelType,
    "hwRev",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_serNum,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_serNum = {
    DataAttributeModelType,
    "serNum",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_model,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_model = {
    DataAttributeModelType,
    "model",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_location,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_location = {
    DataAttributeModelType,
    "location",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam_mRID,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyNam_mRID = {
    DataAttributeModelType,
    "mRID",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_PROT_LPHD1,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth_stVal,
    0
};

DataAttribute iedModel_PROT_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_PROT_LPHD1,
    NULL,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_stVal,
    0
};

DataAttribute iedModel_PROT_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_Proxy_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_Proxy_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_Proxy_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_Proxy_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_LPHD1_Proxy_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_PTOC1 = {
    LogicalNodeModelType,
    "PTOC1",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_SCPI1,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
};

DataObject iedModel_PROT_PTOC1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_Beh,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_stVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    (ModelNode*) &iedModel_PROT_PTOC1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_PTOC1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_Health,
    (ModelNode*) &iedModel_PROT_PTOC1_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_PTOC1_Beh,
    (ModelNode*) &iedModel_PROT_PTOC1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_PTOC1_Beh,
    (ModelNode*) &iedModel_PROT_PTOC1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_PTOC1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_NamPlt,
    (ModelNode*) &iedModel_PROT_PTOC1_Health_stVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_PTOC1_Health,
    (ModelNode*) &iedModel_PROT_PTOC1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_PTOC1_Health,
    (ModelNode*) &iedModel_PROT_PTOC1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_PTOC1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    (ModelNode*) &iedModel_PROT_PTOC1_NamPlt_vendor,
    0
};

DataAttribute iedModel_PROT_PTOC1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_PTOC1_NamPlt,
    (ModelNode*) &iedModel_PROT_PTOC1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_PTOC1_NamPlt,
    (ModelNode*) &iedModel_PROT_PTOC1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_PROT_PTOC1_NamPlt,
    (ModelNode*) &iedModel_PROT_PTOC1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_PTOC1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_Str = {
    DataObjectModelType,
    "Str",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_Op,
    (ModelNode*) &iedModel_PROT_PTOC1_Str_general,
    0
};

DataAttribute iedModel_PROT_PTOC1_Str_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    (ModelNode*) &iedModel_PROT_PTOC1_Str_dirGeneral,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Str_dirGeneral = {
    DataAttributeModelType,
    "dirGeneral",
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    (ModelNode*) &iedModel_PROT_PTOC1_Str_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Str_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    (ModelNode*) &iedModel_PROT_PTOC1_Str_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Str_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    (ModelNode*) &iedModel_PROT_PTOC1_Str_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Str_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_PTOC1_Str,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_Op = {
    DataObjectModelType,
    "Op",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    (ModelNode*) &iedModel_PROT_PTOC1_Op_general,
    0
};

DataAttribute iedModel_PROT_PTOC1_Op_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_PTOC1_Op,
    (ModelNode*) &iedModel_PROT_PTOC1_Op_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Op_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_PTOC1_Op,
    (ModelNode*) &iedModel_PROT_PTOC1_Op_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Op_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_PTOC1_Op,
    (ModelNode*) &iedModel_PROT_PTOC1_Op_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_Op_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_PTOC1_Op,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_StrVal = {
    DataObjectModelType,
    "StrVal",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_setMag,
    0
};

DataAttribute iedModel_PROT_PTOC1_StrVal_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_units,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_setMag_f,
    0,
    IEC61850_FC_SP,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_setMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_sVC,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_units,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_minVal,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_sVC,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_minVal = {
    DataAttributeModelType,
    "minVal",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_maxVal,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_minVal_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_minVal_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_minVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_maxVal = {
    DataAttributeModelType,
    "maxVal",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_stepSize,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_maxVal_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_maxVal_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_maxVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_stepSize = {
    DataAttributeModelType,
    "stepSize",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_dU,
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_stepSize_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_stepSize_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal_stepSize,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_StrVal_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_PTOC1_StrVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_TmMult = {
    DataObjectModelType,
    "TmMult",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_setMag,
    0
};

DataAttribute iedModel_PROT_PTOC1_TmMult_setMag = {
    DataAttributeModelType,
    "setMag",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_units,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_setMag_f,
    0,
    IEC61850_FC_SP,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_setMag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_setMag,
    NULL,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_units = {
    DataAttributeModelType,
    "units",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_sVC,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_units_SIUnit,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_units_SIUnit = {
    DataAttributeModelType,
    "SIUnit",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_units,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_units_multiplier,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_units_multiplier = {
    DataAttributeModelType,
    "multiplier",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_units,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_sVC = {
    DataAttributeModelType,
    "sVC",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_minVal,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_sVC_scaleFactor,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_sVC_scaleFactor = {
    DataAttributeModelType,
    "scaleFactor",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_sVC,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_sVC_offset,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_sVC_offset = {
    DataAttributeModelType,
    "offset",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_sVC,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_minVal = {
    DataAttributeModelType,
    "minVal",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_maxVal,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_minVal_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_minVal_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_minVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_maxVal = {
    DataAttributeModelType,
    "maxVal",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_stepSize,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_maxVal_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_maxVal_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_maxVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_stepSize = {
    DataAttributeModelType,
    "stepSize",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_dU,
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_stepSize_f,
    0,
    IEC61850_FC_CF,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_stepSize_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult_stepSize,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_TmMult_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_PTOC1_TmMult,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_MinOpTmms = {
    DataObjectModelType,
    "MinOpTmms",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms_setVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_MinOpTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms_minVal,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_MinOpTmms_minVal = {
    DataAttributeModelType,
    "minVal",
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms_maxVal,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_MinOpTmms_maxVal = {
    DataAttributeModelType,
    "maxVal",
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms_stepSize,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_MinOpTmms_stepSize = {
    DataAttributeModelType,
    "stepSize",
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_MinOpTmms_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_PTOC1_MinOpTmms,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_MaxOpTmms = {
    DataObjectModelType,
    "MaxOpTmms",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms_setVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_MaxOpTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms_minVal,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_MaxOpTmms_minVal = {
    DataAttributeModelType,
    "minVal",
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms_maxVal,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_MaxOpTmms_maxVal = {
    DataAttributeModelType,
    "maxVal",
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms_stepSize,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_MaxOpTmms_stepSize = {
    DataAttributeModelType,
    "stepSize",
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_MaxOpTmms_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_PTOC1_MaxOpTmms,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_RsDlTmms = {
    DataObjectModelType,
    "RsDlTmms",
    (ModelNode*) &iedModel_PROT_PTOC1,
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod,
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms_setVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_RsDlTmms_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms_minVal,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_RsDlTmms_minVal = {
    DataAttributeModelType,
    "minVal",
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms_maxVal,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_RsDlTmms_maxVal = {
    DataAttributeModelType,
    "maxVal",
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms_stepSize,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_RsDlTmms_stepSize = {
    DataAttributeModelType,
    "stepSize",
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms,
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_RsDlTmms_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_PTOC1_RsDlTmms,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_PTOC1_DirMod = {
    DataObjectModelType,
    "DirMod",
    (ModelNode*) &iedModel_PROT_PTOC1,
    NULL,
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod_setVal,
    0
};

DataAttribute iedModel_PROT_PTOC1_DirMod_setVal = {
    DataAttributeModelType,
    "setVal",
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod,
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod_minVal,
    NULL,
    0,
    IEC61850_FC_SP,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_DirMod_minVal = {
    DataAttributeModelType,
    "minVal",
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod,
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod_maxVal,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_DirMod_maxVal = {
    DataAttributeModelType,
    "maxVal",
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod,
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod_stepSize,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_DirMod_stepSize = {
    DataAttributeModelType,
    "stepSize",
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod,
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod_dU,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_INT32U,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_PTOC1_DirMod_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_PTOC1_DirMod,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_SCPI1 = {
    LogicalNodeModelType,
    "SCPI1",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_SVPI1,
    (ModelNode*) &iedModel_PROT_SCPI1_Mod,
};

DataObject iedModel_PROT_SCPI1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_SCPI1,
    (ModelNode*) &iedModel_PROT_SCPI1_Beh,
    (ModelNode*) &iedModel_PROT_SCPI1_Mod_stVal,
    0
};

DataAttribute iedModel_PROT_SCPI1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SCPI1_Mod,
    (ModelNode*) &iedModel_PROT_SCPI1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SCPI1_Mod,
    (ModelNode*) &iedModel_PROT_SCPI1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SCPI1_Mod,
    (ModelNode*) &iedModel_PROT_SCPI1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_SCPI1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SCPI1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_SCPI1,
    (ModelNode*) &iedModel_PROT_SCPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SCPI1_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_SCPI1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SCPI1_Beh,
    (ModelNode*) &iedModel_PROT_SCPI1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SCPI1_Beh,
    (ModelNode*) &iedModel_PROT_SCPI1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SCPI1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SCPI1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_PROT_SCPI1,
    (ModelNode*) &iedModel_PROT_SCPI1_Health,
    (ModelNode*) &iedModel_PROT_SCPI1_NamPlt_vendor,
    0
};

DataAttribute iedModel_PROT_SCPI1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_SCPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SCPI1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_SCPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SCPI1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_PROT_SCPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SCPI1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SCPI1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SCPI1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_PROT_SCPI1,
    (ModelNode*) &iedModel_PROT_SCPI1_Abc,
    (ModelNode*) &iedModel_PROT_SCPI1_Health_stVal,
    0
};

DataAttribute iedModel_PROT_SCPI1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SCPI1_Health,
    (ModelNode*) &iedModel_PROT_SCPI1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SCPI1_Health,
    (ModelNode*) &iedModel_PROT_SCPI1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SCPI1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SCPI1_Abc = {
    DataObjectModelType,
    "Abc",
    (ModelNode*) &iedModel_PROT_SCPI1,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_Abc_general,
    0
};

DataAttribute iedModel_PROT_SCPI1_Abc_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_SCPI1_Abc,
    (ModelNode*) &iedModel_PROT_SCPI1_Abc_phsA,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Abc_phsA = {
    DataAttributeModelType,
    "phsA",
    (ModelNode*) &iedModel_PROT_SCPI1_Abc,
    (ModelNode*) &iedModel_PROT_SCPI1_Abc_phsB,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Abc_phsB = {
    DataAttributeModelType,
    "phsB",
    (ModelNode*) &iedModel_PROT_SCPI1_Abc,
    (ModelNode*) &iedModel_PROT_SCPI1_Abc_phsC,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Abc_phsC = {
    DataAttributeModelType,
    "phsC",
    (ModelNode*) &iedModel_PROT_SCPI1_Abc,
    (ModelNode*) &iedModel_PROT_SCPI1_Abc_neut,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Abc_neut = {
    DataAttributeModelType,
    "neut",
    (ModelNode*) &iedModel_PROT_SCPI1_Abc,
    (ModelNode*) &iedModel_PROT_SCPI1_Abc_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Abc_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SCPI1_Abc,
    (ModelNode*) &iedModel_PROT_SCPI1_Abc_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Abc_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SCPI1_Abc,
    (ModelNode*) &iedModel_PROT_SCPI1_Abc_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Abc_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SCPI1_Abc,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SCPI1_DirPrs = {
    DataObjectModelType,
    "DirPrs",
    (ModelNode*) &iedModel_PROT_SCPI1,
    (ModelNode*) &iedModel_PROT_SCPI1_Blk,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_general,
    0
};

DataAttribute iedModel_PROT_SCPI1_DirPrs_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_dirGeneral,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_dirGeneral = {
    DataAttributeModelType,
    "dirGeneral",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_phsA,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_phsA = {
    DataAttributeModelType,
    "phsA",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_dirPhsA,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_dirPhsA = {
    DataAttributeModelType,
    "dirPhsA",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_phsB,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_phsB = {
    DataAttributeModelType,
    "phsB",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_dirPhsB,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_dirPhsB = {
    DataAttributeModelType,
    "dirPhsB",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_phsC,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_phsC = {
    DataAttributeModelType,
    "phsC",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_dirPhsC,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_dirPhsC = {
    DataAttributeModelType,
    "dirPhsC",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_neut,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_neut = {
    DataAttributeModelType,
    "neut",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_dirNeut,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_dirNeut = {
    DataAttributeModelType,
    "dirNeut",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_DirPrs_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SCPI1_DirPrs,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SCPI1_Blk = {
    DataObjectModelType,
    "Blk",
    (ModelNode*) &iedModel_PROT_SCPI1,
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SCPI1_Blk_stVal,
    0
};

DataAttribute iedModel_PROT_SCPI1_Blk_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SCPI1_Blk,
    (ModelNode*) &iedModel_PROT_SCPI1_Blk_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Blk_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SCPI1_Blk,
    (ModelNode*) &iedModel_PROT_SCPI1_Blk_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Blk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SCPI1_Blk,
    (ModelNode*) &iedModel_PROT_SCPI1_Blk_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Blk_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_SCPI1_Blk,
    (ModelNode*) &iedModel_PROT_SCPI1_Blk_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Blk_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_SCPI1_Blk,
    (ModelNode*) &iedModel_PROT_SCPI1_Blk_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Blk_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_SCPI1_Blk,
    (ModelNode*) &iedModel_PROT_SCPI1_Blk_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Blk_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_SCPI1_Blk,
    (ModelNode*) &iedModel_PROT_SCPI1_Blk_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Blk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SCPI1_Blk,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SCPI1_ClcExp = {
    DataObjectModelType,
    "ClcExp",
    (ModelNode*) &iedModel_PROT_SCPI1,
    (ModelNode*) &iedModel_PROT_SCPI1_Mir,
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp_stVal,
    0
};

DataAttribute iedModel_PROT_SCPI1_ClcExp_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_ClcExp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_ClcExp_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_ClcExp_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_ClcExp_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_ClcExp_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_ClcExp_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_ClcExp_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SCPI1_ClcExp,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SCPI1_Mir = {
    DataObjectModelType,
    "Mir",
    (ModelNode*) &iedModel_PROT_SCPI1,
    NULL,
    (ModelNode*) &iedModel_PROT_SCPI1_Mir_stVal,
    0
};

DataAttribute iedModel_PROT_SCPI1_Mir_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SCPI1_Mir,
    (ModelNode*) &iedModel_PROT_SCPI1_Mir_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mir_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SCPI1_Mir,
    (ModelNode*) &iedModel_PROT_SCPI1_Mir_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mir_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SCPI1_Mir,
    (ModelNode*) &iedModel_PROT_SCPI1_Mir_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mir_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_SCPI1_Mir,
    (ModelNode*) &iedModel_PROT_SCPI1_Mir_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mir_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_SCPI1_Mir,
    (ModelNode*) &iedModel_PROT_SCPI1_Mir_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mir_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_SCPI1_Mir,
    (ModelNode*) &iedModel_PROT_SCPI1_Mir_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mir_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_SCPI1_Mir,
    (ModelNode*) &iedModel_PROT_SCPI1_Mir_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SCPI1_Mir_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SCPI1_Mir,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_SVPI1 = {
    LogicalNodeModelType,
    "SVPI1",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_SFPI1,
    (ModelNode*) &iedModel_PROT_SVPI1_Mod,
};

DataObject iedModel_PROT_SVPI1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_SVPI1,
    (ModelNode*) &iedModel_PROT_SVPI1_Beh,
    (ModelNode*) &iedModel_PROT_SVPI1_Mod_stVal,
    0
};

DataAttribute iedModel_PROT_SVPI1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SVPI1_Mod,
    (ModelNode*) &iedModel_PROT_SVPI1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SVPI1_Mod,
    (ModelNode*) &iedModel_PROT_SVPI1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SVPI1_Mod,
    (ModelNode*) &iedModel_PROT_SVPI1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_SVPI1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SVPI1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_SVPI1,
    (ModelNode*) &iedModel_PROT_SVPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SVPI1_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_SVPI1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SVPI1_Beh,
    (ModelNode*) &iedModel_PROT_SVPI1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SVPI1_Beh,
    (ModelNode*) &iedModel_PROT_SVPI1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SVPI1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SVPI1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_PROT_SVPI1,
    (ModelNode*) &iedModel_PROT_SVPI1_Health,
    (ModelNode*) &iedModel_PROT_SVPI1_NamPlt_vendor,
    0
};

DataAttribute iedModel_PROT_SVPI1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_SVPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SVPI1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_SVPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SVPI1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_PROT_SVPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SVPI1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SVPI1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SVPI1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_PROT_SVPI1,
    (ModelNode*) &iedModel_PROT_SVPI1_Abc,
    (ModelNode*) &iedModel_PROT_SVPI1_Health_stVal,
    0
};

DataAttribute iedModel_PROT_SVPI1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SVPI1_Health,
    (ModelNode*) &iedModel_PROT_SVPI1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SVPI1_Health,
    (ModelNode*) &iedModel_PROT_SVPI1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SVPI1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SVPI1_Abc = {
    DataObjectModelType,
    "Abc",
    (ModelNode*) &iedModel_PROT_SVPI1,
    (ModelNode*) &iedModel_PROT_SVPI1_Prs,
    (ModelNode*) &iedModel_PROT_SVPI1_Abc_general,
    0
};

DataAttribute iedModel_PROT_SVPI1_Abc_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_SVPI1_Abc,
    (ModelNode*) &iedModel_PROT_SVPI1_Abc_phsA,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Abc_phsA = {
    DataAttributeModelType,
    "phsA",
    (ModelNode*) &iedModel_PROT_SVPI1_Abc,
    (ModelNode*) &iedModel_PROT_SVPI1_Abc_phsB,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Abc_phsB = {
    DataAttributeModelType,
    "phsB",
    (ModelNode*) &iedModel_PROT_SVPI1_Abc,
    (ModelNode*) &iedModel_PROT_SVPI1_Abc_phsC,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Abc_phsC = {
    DataAttributeModelType,
    "phsC",
    (ModelNode*) &iedModel_PROT_SVPI1_Abc,
    (ModelNode*) &iedModel_PROT_SVPI1_Abc_neut,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Abc_neut = {
    DataAttributeModelType,
    "neut",
    (ModelNode*) &iedModel_PROT_SVPI1_Abc,
    (ModelNode*) &iedModel_PROT_SVPI1_Abc_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Abc_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SVPI1_Abc,
    (ModelNode*) &iedModel_PROT_SVPI1_Abc_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Abc_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SVPI1_Abc,
    (ModelNode*) &iedModel_PROT_SVPI1_Abc_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Abc_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SVPI1_Abc,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SVPI1_Prs = {
    DataObjectModelType,
    "Prs",
    (ModelNode*) &iedModel_PROT_SVPI1,
    (ModelNode*) &iedModel_PROT_SVPI1_Blk,
    (ModelNode*) &iedModel_PROT_SVPI1_Prs_general,
    0
};

DataAttribute iedModel_PROT_SVPI1_Prs_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_SVPI1_Prs,
    (ModelNode*) &iedModel_PROT_SVPI1_Prs_phsA,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Prs_phsA = {
    DataAttributeModelType,
    "phsA",
    (ModelNode*) &iedModel_PROT_SVPI1_Prs,
    (ModelNode*) &iedModel_PROT_SVPI1_Prs_phsB,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Prs_phsB = {
    DataAttributeModelType,
    "phsB",
    (ModelNode*) &iedModel_PROT_SVPI1_Prs,
    (ModelNode*) &iedModel_PROT_SVPI1_Prs_phsC,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Prs_phsC = {
    DataAttributeModelType,
    "phsC",
    (ModelNode*) &iedModel_PROT_SVPI1_Prs,
    (ModelNode*) &iedModel_PROT_SVPI1_Prs_neut,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Prs_neut = {
    DataAttributeModelType,
    "neut",
    (ModelNode*) &iedModel_PROT_SVPI1_Prs,
    (ModelNode*) &iedModel_PROT_SVPI1_Prs_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Prs_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SVPI1_Prs,
    (ModelNode*) &iedModel_PROT_SVPI1_Prs_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Prs_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SVPI1_Prs,
    (ModelNode*) &iedModel_PROT_SVPI1_Prs_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Prs_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SVPI1_Prs,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SVPI1_Blk = {
    DataObjectModelType,
    "Blk",
    (ModelNode*) &iedModel_PROT_SVPI1,
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SVPI1_Blk_stVal,
    0
};

DataAttribute iedModel_PROT_SVPI1_Blk_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SVPI1_Blk,
    (ModelNode*) &iedModel_PROT_SVPI1_Blk_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Blk_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SVPI1_Blk,
    (ModelNode*) &iedModel_PROT_SVPI1_Blk_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Blk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SVPI1_Blk,
    (ModelNode*) &iedModel_PROT_SVPI1_Blk_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Blk_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_SVPI1_Blk,
    (ModelNode*) &iedModel_PROT_SVPI1_Blk_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Blk_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_SVPI1_Blk,
    (ModelNode*) &iedModel_PROT_SVPI1_Blk_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Blk_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_SVPI1_Blk,
    (ModelNode*) &iedModel_PROT_SVPI1_Blk_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Blk_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_SVPI1_Blk,
    (ModelNode*) &iedModel_PROT_SVPI1_Blk_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Blk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SVPI1_Blk,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SVPI1_ClcExp = {
    DataObjectModelType,
    "ClcExp",
    (ModelNode*) &iedModel_PROT_SVPI1,
    (ModelNode*) &iedModel_PROT_SVPI1_Mir,
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp_stVal,
    0
};

DataAttribute iedModel_PROT_SVPI1_ClcExp_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_ClcExp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_ClcExp_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_ClcExp_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_ClcExp_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_ClcExp_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_ClcExp_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_ClcExp_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SVPI1_ClcExp,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SVPI1_Mir = {
    DataObjectModelType,
    "Mir",
    (ModelNode*) &iedModel_PROT_SVPI1,
    NULL,
    (ModelNode*) &iedModel_PROT_SVPI1_Mir_stVal,
    0
};

DataAttribute iedModel_PROT_SVPI1_Mir_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SVPI1_Mir,
    (ModelNode*) &iedModel_PROT_SVPI1_Mir_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mir_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SVPI1_Mir,
    (ModelNode*) &iedModel_PROT_SVPI1_Mir_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mir_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SVPI1_Mir,
    (ModelNode*) &iedModel_PROT_SVPI1_Mir_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mir_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_SVPI1_Mir,
    (ModelNode*) &iedModel_PROT_SVPI1_Mir_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mir_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_SVPI1_Mir,
    (ModelNode*) &iedModel_PROT_SVPI1_Mir_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mir_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_SVPI1_Mir,
    (ModelNode*) &iedModel_PROT_SVPI1_Mir_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mir_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_SVPI1_Mir,
    (ModelNode*) &iedModel_PROT_SVPI1_Mir_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SVPI1_Mir_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SVPI1_Mir,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_SFPI1 = {
    LogicalNodeModelType,
    "SFPI1",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_AFSL1,
    (ModelNode*) &iedModel_PROT_SFPI1_Mod,
};

DataObject iedModel_PROT_SFPI1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_SFPI1,
    (ModelNode*) &iedModel_PROT_SFPI1_Beh,
    (ModelNode*) &iedModel_PROT_SFPI1_Mod_stVal,
    0
};

DataAttribute iedModel_PROT_SFPI1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SFPI1_Mod,
    (ModelNode*) &iedModel_PROT_SFPI1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SFPI1_Mod,
    (ModelNode*) &iedModel_PROT_SFPI1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SFPI1_Mod,
    (ModelNode*) &iedModel_PROT_SFPI1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_SFPI1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SFPI1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_SFPI1,
    (ModelNode*) &iedModel_PROT_SFPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SFPI1_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_SFPI1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SFPI1_Beh,
    (ModelNode*) &iedModel_PROT_SFPI1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SFPI1_Beh,
    (ModelNode*) &iedModel_PROT_SFPI1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SFPI1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SFPI1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_PROT_SFPI1,
    (ModelNode*) &iedModel_PROT_SFPI1_Health,
    (ModelNode*) &iedModel_PROT_SFPI1_NamPlt_vendor,
    0
};

DataAttribute iedModel_PROT_SFPI1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_SFPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SFPI1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_SFPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SFPI1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_PROT_SFPI1_NamPlt,
    (ModelNode*) &iedModel_PROT_SFPI1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SFPI1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SFPI1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_PROT_SFPI1,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_Health_stVal,
    0
};

DataAttribute iedModel_PROT_SFPI1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SFPI1_Health,
    (ModelNode*) &iedModel_PROT_SFPI1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SFPI1_Health,
    (ModelNode*) &iedModel_PROT_SFPI1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SFPI1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SFPI1_FltInd = {
    DataObjectModelType,
    "FltInd",
    (ModelNode*) &iedModel_PROT_SFPI1,
    (ModelNode*) &iedModel_PROT_SFPI1_Blk,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_general,
    0
};

DataAttribute iedModel_PROT_SFPI1_FltInd_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_dirGeneral,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_dirGeneral = {
    DataAttributeModelType,
    "dirGeneral",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_phsA,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_phsA = {
    DataAttributeModelType,
    "phsA",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_dirPhsA,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_dirPhsA = {
    DataAttributeModelType,
    "dirPhsA",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_phsB,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_phsB = {
    DataAttributeModelType,
    "phsB",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_dirPhsB,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_dirPhsB = {
    DataAttributeModelType,
    "dirPhsB",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_phsC,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_phsC = {
    DataAttributeModelType,
    "phsC",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_dirPhsC,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_dirPhsC = {
    DataAttributeModelType,
    "dirPhsC",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_neut,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_neut = {
    DataAttributeModelType,
    "neut",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_dirNeut,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_dirNeut = {
    DataAttributeModelType,
    "dirNeut",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_FltInd_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SFPI1_FltInd,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SFPI1_Blk = {
    DataObjectModelType,
    "Blk",
    (ModelNode*) &iedModel_PROT_SFPI1,
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SFPI1_Blk_stVal,
    0
};

DataAttribute iedModel_PROT_SFPI1_Blk_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SFPI1_Blk,
    (ModelNode*) &iedModel_PROT_SFPI1_Blk_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Blk_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SFPI1_Blk,
    (ModelNode*) &iedModel_PROT_SFPI1_Blk_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Blk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SFPI1_Blk,
    (ModelNode*) &iedModel_PROT_SFPI1_Blk_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Blk_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_SFPI1_Blk,
    (ModelNode*) &iedModel_PROT_SFPI1_Blk_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Blk_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_SFPI1_Blk,
    (ModelNode*) &iedModel_PROT_SFPI1_Blk_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Blk_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_SFPI1_Blk,
    (ModelNode*) &iedModel_PROT_SFPI1_Blk_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Blk_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_SFPI1_Blk,
    (ModelNode*) &iedModel_PROT_SFPI1_Blk_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Blk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SFPI1_Blk,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SFPI1_ClcExp = {
    DataObjectModelType,
    "ClcExp",
    (ModelNode*) &iedModel_PROT_SFPI1,
    (ModelNode*) &iedModel_PROT_SFPI1_Mir,
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp_stVal,
    0
};

DataAttribute iedModel_PROT_SFPI1_ClcExp_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_ClcExp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_ClcExp_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_ClcExp_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_ClcExp_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_ClcExp_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_ClcExp_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp,
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_ClcExp_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SFPI1_ClcExp,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_SFPI1_Mir = {
    DataObjectModelType,
    "Mir",
    (ModelNode*) &iedModel_PROT_SFPI1,
    NULL,
    (ModelNode*) &iedModel_PROT_SFPI1_Mir_stVal,
    0
};

DataAttribute iedModel_PROT_SFPI1_Mir_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_SFPI1_Mir,
    (ModelNode*) &iedModel_PROT_SFPI1_Mir_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mir_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_SFPI1_Mir,
    (ModelNode*) &iedModel_PROT_SFPI1_Mir_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mir_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_SFPI1_Mir,
    (ModelNode*) &iedModel_PROT_SFPI1_Mir_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mir_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_SFPI1_Mir,
    (ModelNode*) &iedModel_PROT_SFPI1_Mir_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mir_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_SFPI1_Mir,
    (ModelNode*) &iedModel_PROT_SFPI1_Mir_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mir_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_SFPI1_Mir,
    (ModelNode*) &iedModel_PROT_SFPI1_Mir_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mir_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_SFPI1_Mir,
    (ModelNode*) &iedModel_PROT_SFPI1_Mir_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_SFPI1_Mir_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_SFPI1_Mir,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_AFSL1 = {
    LogicalNodeModelType,
    "AFSL1",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSL1_Mod,
};

DataObject iedModel_PROT_AFSL1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_AFSL1,
    (ModelNode*) &iedModel_PROT_AFSL1_Beh,
    (ModelNode*) &iedModel_PROT_AFSL1_Mod_stVal,
    0
};

DataAttribute iedModel_PROT_AFSL1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSL1_Mod,
    (ModelNode*) &iedModel_PROT_AFSL1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSL1_Mod,
    (ModelNode*) &iedModel_PROT_AFSL1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSL1_Mod,
    (ModelNode*) &iedModel_PROT_AFSL1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_AFSL1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSL1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_AFSL1,
    (ModelNode*) &iedModel_PROT_AFSL1_NamPlt,
    (ModelNode*) &iedModel_PROT_AFSL1_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_AFSL1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSL1_Beh,
    (ModelNode*) &iedModel_PROT_AFSL1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSL1_Beh,
    (ModelNode*) &iedModel_PROT_AFSL1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSL1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSL1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_PROT_AFSL1,
    (ModelNode*) &iedModel_PROT_AFSL1_Health,
    (ModelNode*) &iedModel_PROT_AFSL1_NamPlt_vendor,
    0
};

DataAttribute iedModel_PROT_AFSL1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_AFSL1_NamPlt,
    (ModelNode*) &iedModel_PROT_AFSL1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_AFSL1_NamPlt,
    (ModelNode*) &iedModel_PROT_AFSL1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_PROT_AFSL1_NamPlt,
    (ModelNode*) &iedModel_PROT_AFSL1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSL1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSL1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_PROT_AFSL1,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocInd,
    (ModelNode*) &iedModel_PROT_AFSL1_Health_stVal,
    0
};

DataAttribute iedModel_PROT_AFSL1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSL1_Health,
    (ModelNode*) &iedModel_PROT_AFSL1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSL1_Health,
    (ModelNode*) &iedModel_PROT_AFSL1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSL1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSL1_FltLocInd = {
    DataObjectModelType,
    "FltLocInd",
    (ModelNode*) &iedModel_PROT_AFSL1,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocInd_general,
    0
};

DataAttribute iedModel_PROT_AFSL1_FltLocInd_general = {
    DataAttributeModelType,
    "general",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocInd,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocInd_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocInd_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocInd,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocInd_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocInd_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocInd,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocInd_dU,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocInd_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocInd,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSL1_FltLocStr = {
    DataObjectModelType,
    "FltLocStr",
    (ModelNode*) &iedModel_PROT_AFSL1,
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr_stVal,
    0
};

DataAttribute iedModel_PROT_AFSL1_FltLocStr_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocStr_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocStr_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocStr_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocStr_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocStr_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocStr_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr,
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_FltLocStr_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSL1_FltLocStr,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSL1_ClcExp = {
    DataObjectModelType,
    "ClcExp",
    (ModelNode*) &iedModel_PROT_AFSL1,
    (ModelNode*) &iedModel_PROT_AFSL1_Mir,
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp_stVal,
    0
};

DataAttribute iedModel_PROT_AFSL1_ClcExp_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_ClcExp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_ClcExp_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_ClcExp_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_ClcExp_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_ClcExp_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_ClcExp_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_ClcExp_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSL1_ClcExp,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSL1_Mir = {
    DataObjectModelType,
    "Mir",
    (ModelNode*) &iedModel_PROT_AFSL1,
    NULL,
    (ModelNode*) &iedModel_PROT_AFSL1_Mir_stVal,
    0
};

DataAttribute iedModel_PROT_AFSL1_Mir_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSL1_Mir,
    (ModelNode*) &iedModel_PROT_AFSL1_Mir_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mir_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSL1_Mir,
    (ModelNode*) &iedModel_PROT_AFSL1_Mir_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mir_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSL1_Mir,
    (ModelNode*) &iedModel_PROT_AFSL1_Mir_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mir_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSL1_Mir,
    (ModelNode*) &iedModel_PROT_AFSL1_Mir_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mir_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSL1_Mir,
    (ModelNode*) &iedModel_PROT_AFSL1_Mir_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mir_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSL1_Mir,
    (ModelNode*) &iedModel_PROT_AFSL1_Mir_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mir_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSL1_Mir,
    (ModelNode*) &iedModel_PROT_AFSL1_Mir_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSL1_Mir_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSL1_Mir,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_AFSI1 = {
    LogicalNodeModelType,
    "AFSI1",
    (ModelNode*) &iedModel_PROT,
    (ModelNode*) &iedModel_PROT_ASRC1,
    (ModelNode*) &iedModel_PROT_AFSI1_Mod,
};

DataObject iedModel_PROT_AFSI1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_Beh,
    (ModelNode*) &iedModel_PROT_AFSI1_Mod_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_Mod,
    (ModelNode*) &iedModel_PROT_AFSI1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_Mod,
    (ModelNode*) &iedModel_PROT_AFSI1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_Mod,
    (ModelNode*) &iedModel_PROT_AFSI1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_AFSI1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_NamPlt,
    (ModelNode*) &iedModel_PROT_AFSI1_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_Beh,
    (ModelNode*) &iedModel_PROT_AFSI1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_Beh,
    (ModelNode*) &iedModel_PROT_AFSI1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_Health,
    (ModelNode*) &iedModel_PROT_AFSI1_NamPlt_vendor,
    0
};

DataAttribute iedModel_PROT_AFSI1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_AFSI1_NamPlt,
    (ModelNode*) &iedModel_PROT_AFSI1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_AFSI1_NamPlt,
    (ModelNode*) &iedModel_PROT_AFSI1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_PROT_AFSI1_NamPlt,
    (ModelNode*) &iedModel_PROT_AFSI1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSI1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd,
    (ModelNode*) &iedModel_PROT_AFSI1_Health_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_Health,
    (ModelNode*) &iedModel_PROT_AFSI1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_Health,
    (ModelNode*) &iedModel_PROT_AFSI1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_FltIsoInd = {
    DataObjectModelType,
    "FltIsoInd",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_FltIsoInd_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoInd_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoInd_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoInd_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoInd_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoInd_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoInd_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoInd_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoInd,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_FltIsoStr = {
    DataObjectModelType,
    "FltIsoStr",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_FltIsoStr_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoStr_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoStr_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoStr_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoStr_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoStr_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoStr_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr,
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_FltIsoStr_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSI1_FltIsoStr,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_LocKey = {
    DataObjectModelType,
    "LocKey",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_Loc,
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_LocKey_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey,
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_LocKey_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey,
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_LocKey_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey,
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_LocKey_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey,
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_LocKey_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey,
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_LocKey_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey,
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_LocKey_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey,
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_LocKey_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSI1_LocKey,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_Loc = {
    DataObjectModelType,
    "Loc",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_Blk,
    (ModelNode*) &iedModel_PROT_AFSI1_Loc_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_Loc_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_Loc,
    (ModelNode*) &iedModel_PROT_AFSI1_Loc_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Loc_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_Loc,
    (ModelNode*) &iedModel_PROT_AFSI1_Loc_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Loc_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_Loc,
    (ModelNode*) &iedModel_PROT_AFSI1_Loc_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Loc_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSI1_Loc,
    (ModelNode*) &iedModel_PROT_AFSI1_Loc_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Loc_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSI1_Loc,
    (ModelNode*) &iedModel_PROT_AFSI1_Loc_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Loc_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSI1_Loc,
    (ModelNode*) &iedModel_PROT_AFSI1_Loc_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Loc_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSI1_Loc,
    (ModelNode*) &iedModel_PROT_AFSI1_Loc_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Loc_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSI1_Loc,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_Blk = {
    DataObjectModelType,
    "Blk",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSI1_Blk_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_Blk_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_Blk,
    (ModelNode*) &iedModel_PROT_AFSI1_Blk_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Blk_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_Blk,
    (ModelNode*) &iedModel_PROT_AFSI1_Blk_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Blk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_Blk,
    (ModelNode*) &iedModel_PROT_AFSI1_Blk_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Blk_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSI1_Blk,
    (ModelNode*) &iedModel_PROT_AFSI1_Blk_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Blk_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSI1_Blk,
    (ModelNode*) &iedModel_PROT_AFSI1_Blk_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Blk_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSI1_Blk,
    (ModelNode*) &iedModel_PROT_AFSI1_Blk_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Blk_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSI1_Blk,
    (ModelNode*) &iedModel_PROT_AFSI1_Blk_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Blk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSI1_Blk,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_ClcExp = {
    DataObjectModelType,
    "ClcExp",
    (ModelNode*) &iedModel_PROT_AFSI1,
    (ModelNode*) &iedModel_PROT_AFSI1_Mir,
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_ClcExp_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_ClcExp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_ClcExp_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_ClcExp_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_ClcExp_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_ClcExp_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_ClcExp_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp,
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_ClcExp_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSI1_ClcExp,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_AFSI1_Mir = {
    DataObjectModelType,
    "Mir",
    (ModelNode*) &iedModel_PROT_AFSI1,
    NULL,
    (ModelNode*) &iedModel_PROT_AFSI1_Mir_stVal,
    0
};

DataAttribute iedModel_PROT_AFSI1_Mir_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_AFSI1_Mir,
    (ModelNode*) &iedModel_PROT_AFSI1_Mir_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mir_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_AFSI1_Mir,
    (ModelNode*) &iedModel_PROT_AFSI1_Mir_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mir_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_AFSI1_Mir,
    (ModelNode*) &iedModel_PROT_AFSI1_Mir_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mir_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_AFSI1_Mir,
    (ModelNode*) &iedModel_PROT_AFSI1_Mir_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mir_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_AFSI1_Mir,
    (ModelNode*) &iedModel_PROT_AFSI1_Mir_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mir_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_AFSI1_Mir,
    (ModelNode*) &iedModel_PROT_AFSI1_Mir_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mir_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_AFSI1_Mir,
    (ModelNode*) &iedModel_PROT_AFSI1_Mir_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_AFSI1_Mir_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_AFSI1_Mir,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

LogicalNode iedModel_PROT_ASRC1 = {
    LogicalNodeModelType,
    "ASRC1",
    (ModelNode*) &iedModel_PROT,
    NULL,
    (ModelNode*) &iedModel_PROT_ASRC1_Mod,
};

DataObject iedModel_PROT_ASRC1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_PROT_ASRC1,
    (ModelNode*) &iedModel_PROT_ASRC1_Beh,
    (ModelNode*) &iedModel_PROT_ASRC1_Mod_stVal,
    0
};

DataAttribute iedModel_PROT_ASRC1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_ASRC1_Mod,
    (ModelNode*) &iedModel_PROT_ASRC1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_ASRC1_Mod,
    (ModelNode*) &iedModel_PROT_ASRC1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_ASRC1_Mod,
    (ModelNode*) &iedModel_PROT_ASRC1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_PROT_ASRC1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_PROT_ASRC1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_PROT_ASRC1,
    (ModelNode*) &iedModel_PROT_ASRC1_NamPlt,
    (ModelNode*) &iedModel_PROT_ASRC1_Beh_stVal,
    0
};

DataAttribute iedModel_PROT_ASRC1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_ASRC1_Beh,
    (ModelNode*) &iedModel_PROT_ASRC1_Beh_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_ASRC1_Beh,
    (ModelNode*) &iedModel_PROT_ASRC1_Beh_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_ASRC1_Beh,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_ASRC1_NamPlt = {
    DataObjectModelType,
    "NamPlt",
    (ModelNode*) &iedModel_PROT_ASRC1,
    (ModelNode*) &iedModel_PROT_ASRC1_Health,
    (ModelNode*) &iedModel_PROT_ASRC1_NamPlt_vendor,
    0
};

DataAttribute iedModel_PROT_ASRC1_NamPlt_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_PROT_ASRC1_NamPlt,
    (ModelNode*) &iedModel_PROT_ASRC1_NamPlt_swRev,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_NamPlt_swRev = {
    DataAttributeModelType,
    "swRev",
    (ModelNode*) &iedModel_PROT_ASRC1_NamPlt,
    (ModelNode*) &iedModel_PROT_ASRC1_NamPlt_d,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_NamPlt_d = {
    DataAttributeModelType,
    "d",
    (ModelNode*) &iedModel_PROT_ASRC1_NamPlt,
    (ModelNode*) &iedModel_PROT_ASRC1_NamPlt_dU,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_NamPlt_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_ASRC1_NamPlt,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_VISIBLE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_ASRC1_Health = {
    DataObjectModelType,
    "Health",
    (ModelNode*) &iedModel_PROT_ASRC1,
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey,
    (ModelNode*) &iedModel_PROT_ASRC1_Health_stVal,
    0
};

DataAttribute iedModel_PROT_ASRC1_Health_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_ASRC1_Health,
    (ModelNode*) &iedModel_PROT_ASRC1_Health_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Health_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_ASRC1_Health,
    (ModelNode*) &iedModel_PROT_ASRC1_Health_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Health_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_ASRC1_Health,
    NULL,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_PROT_ASRC1_LocKey = {
    DataObjectModelType,
    "LocKey",
    (ModelNode*) &iedModel_PROT_ASRC1,
    (ModelNode*) &iedModel_PROT_ASRC1_Loc,
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey_stVal,
    0
};

DataAttribute iedModel_PROT_ASRC1_LocKey_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey,
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_LocKey_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey,
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_LocKey_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey,
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_LocKey_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey,
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_LocKey_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey,
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_LocKey_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey,
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_LocKey_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey,
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_LocKey_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_ASRC1_LocKey,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_ASRC1_Loc = {
    DataObjectModelType,
    "Loc",
    (ModelNode*) &iedModel_PROT_ASRC1,
    (ModelNode*) &iedModel_PROT_ASRC1_Blk,
    (ModelNode*) &iedModel_PROT_ASRC1_Loc_stVal,
    0
};

DataAttribute iedModel_PROT_ASRC1_Loc_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_ASRC1_Loc,
    (ModelNode*) &iedModel_PROT_ASRC1_Loc_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Loc_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_ASRC1_Loc,
    (ModelNode*) &iedModel_PROT_ASRC1_Loc_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Loc_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_ASRC1_Loc,
    (ModelNode*) &iedModel_PROT_ASRC1_Loc_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Loc_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_ASRC1_Loc,
    (ModelNode*) &iedModel_PROT_ASRC1_Loc_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Loc_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_ASRC1_Loc,
    (ModelNode*) &iedModel_PROT_ASRC1_Loc_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Loc_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_ASRC1_Loc,
    (ModelNode*) &iedModel_PROT_ASRC1_Loc_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Loc_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_ASRC1_Loc,
    (ModelNode*) &iedModel_PROT_ASRC1_Loc_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Loc_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_ASRC1_Loc,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_ASRC1_Blk = {
    DataObjectModelType,
    "Blk",
    (ModelNode*) &iedModel_PROT_ASRC1,
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp,
    (ModelNode*) &iedModel_PROT_ASRC1_Blk_stVal,
    0
};

DataAttribute iedModel_PROT_ASRC1_Blk_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_ASRC1_Blk,
    (ModelNode*) &iedModel_PROT_ASRC1_Blk_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Blk_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_ASRC1_Blk,
    (ModelNode*) &iedModel_PROT_ASRC1_Blk_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Blk_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_ASRC1_Blk,
    (ModelNode*) &iedModel_PROT_ASRC1_Blk_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Blk_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_ASRC1_Blk,
    (ModelNode*) &iedModel_PROT_ASRC1_Blk_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Blk_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_ASRC1_Blk,
    (ModelNode*) &iedModel_PROT_ASRC1_Blk_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Blk_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_ASRC1_Blk,
    (ModelNode*) &iedModel_PROT_ASRC1_Blk_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Blk_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_ASRC1_Blk,
    (ModelNode*) &iedModel_PROT_ASRC1_Blk_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Blk_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_ASRC1_Blk,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_ASRC1_ClcExp = {
    DataObjectModelType,
    "ClcExp",
    (ModelNode*) &iedModel_PROT_ASRC1,
    (ModelNode*) &iedModel_PROT_ASRC1_Mir,
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp_stVal,
    0
};

DataAttribute iedModel_PROT_ASRC1_ClcExp_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp,
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_ClcExp_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp,
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_ClcExp_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp,
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_ClcExp_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp,
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_ClcExp_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp,
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_ClcExp_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp,
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_ClcExp_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp,
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_ClcExp_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_ASRC1_ClcExp,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};

DataObject iedModel_PROT_ASRC1_Mir = {
    DataObjectModelType,
    "Mir",
    (ModelNode*) &iedModel_PROT_ASRC1,
    NULL,
    (ModelNode*) &iedModel_PROT_ASRC1_Mir_stVal,
    0
};

DataAttribute iedModel_PROT_ASRC1_Mir_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_PROT_ASRC1_Mir,
    (ModelNode*) &iedModel_PROT_ASRC1_Mir_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_BOOLEAN,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mir_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_PROT_ASRC1_Mir,
    (ModelNode*) &iedModel_PROT_ASRC1_Mir_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mir_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_PROT_ASRC1_Mir,
    (ModelNode*) &iedModel_PROT_ASRC1_Mir_subEna,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mir_subEna = {
    DataAttributeModelType,
    "subEna",
    (ModelNode*) &iedModel_PROT_ASRC1_Mir,
    (ModelNode*) &iedModel_PROT_ASRC1_Mir_subVal,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mir_subVal = {
    DataAttributeModelType,
    "subVal",
    (ModelNode*) &iedModel_PROT_ASRC1_Mir,
    (ModelNode*) &iedModel_PROT_ASRC1_Mir_subQ,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mir_subQ = {
    DataAttributeModelType,
    "subQ",
    (ModelNode*) &iedModel_PROT_ASRC1_Mir,
    (ModelNode*) &iedModel_PROT_ASRC1_Mir_subID,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mir_subID = {
    DataAttributeModelType,
    "subID",
    (ModelNode*) &iedModel_PROT_ASRC1_Mir,
    (ModelNode*) &iedModel_PROT_ASRC1_Mir_dU,
    NULL,
    0,
    IEC61850_FC_SV,
    IEC61850_VISIBLE_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_PROT_ASRC1_Mir_dU = {
    DataAttributeModelType,
    "dU",
    (ModelNode*) &iedModel_PROT_ASRC1_Mir,
    NULL,
    NULL,
    0,
    IEC61850_FC_DC,
    IEC61850_UNICODE_STRING_255,
    0,
    NULL,
    0};




extern GSEControlBlock iedModel_LD0_LLN0_gse0;
extern GSEControlBlock iedModel_MEAS_LLN0_gse0;
extern GSEControlBlock iedModel_PROT_LLN0_gse0;
extern GSEControlBlock iedModel_PROT_LLN0_gse1;

static PhyComAddress iedModel_LD0_LLN0_gse0_address = {
  4,
  1,
  4096,
  {0x1, 0xc, 0xcd, 0x1, 0x0, 0x1}
};

GSEControlBlock iedModel_LD0_LLN0_gse0 = {&iedModel_LD0_LLN0, "gcbRemote", "remote", "RmoteSignal", 1, false, &iedModel_LD0_LLN0_gse0_address, 2, 3000, &iedModel_MEAS_LLN0_gse0};

static PhyComAddress iedModel_MEAS_LLN0_gse0_address = {
  4,
  1,
  4097,
  {0x1, 0xc, 0xcd, 0x1, 0x0, 0x1}
};

GSEControlBlock iedModel_MEAS_LLN0_gse0 = {&iedModel_MEAS_LLN0, "gcbMeasureValues", "analog", "MeasureValues", 1, false, &iedModel_MEAS_LLN0_gse0_address, 1000, 3000, &iedModel_PROT_LLN0_gse0};

static PhyComAddress iedModel_PROT_LLN0_gse0_address = {
  4,
  1,
  4099,
  {0x1, 0xc, 0xcd, 0x1, 0x0, 0x3}
};

GSEControlBlock iedModel_PROT_LLN0_gse0 = {&iedModel_PROT_LLN0, "gcbDistriIndicate", "indicate", "DistriIndicate", 1, false, &iedModel_PROT_LLN0_gse0_address, 2, 3000, &iedModel_PROT_LLN0_gse1};

static PhyComAddress iedModel_PROT_LLN0_gse1_address = {
  4,
  1,
  4100,
  {0x1, 0xc, 0xcd, 0x1, 0x0, 0x3}
};

GSEControlBlock iedModel_PROT_LLN0_gse1 = {&iedModel_PROT_LLN0, "gcbDistriDeal", "deal", "DistriDeal", 1, false, &iedModel_PROT_LLN0_gse1_address, 2, 3000, NULL};





IedModel iedModel = {
    "STU1",
    &iedModel_LD0,
    &iedModelds_LD0_LLN0_RmoteSignal,
    NULL,
    &iedModel_LD0_LLN0_gse0,
    NULL,
    NULL,
    NULL,
    NULL,
    initializeValues
};

static void
initializeValues()
{

iedModel_LD0_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_LD0_LLN0_NamPlt_ldNs.mmsValue = MmsValue_newVisibleString("IEC 61850-7-4:2003");

iedModel_LD0_GGIO1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_LD0_LGOS1_GoCBRef_setSrcRef.mmsValue = MmsValue_newVisibleString("STU2LD0/LLN0$GO$gcbRemote");

iedModel_LD0_LGOS1_GoCBRef_setSrcCB.mmsValue = MmsValue_newVisibleString("STU2LD0/LLN0$RmoteSignal");

iedModel_LD0_LGOS2_GoCBRef_setSrcRef.mmsValue = MmsValue_newVisibleString("STU2LD0/LLN0$GO$gcbMeasureValues");

iedModel_LD0_LGOS2_GoCBRef_setSrcCB.mmsValue = MmsValue_newVisibleString("STU2LD0/LLN0$MeasureValues");

iedModel_MEAS_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_MEAS_LLN0_NamPlt_ldNs.mmsValue = MmsValue_newVisibleString("IEC 61850-7-4:2003");

iedModel_MEAS_MMXU1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_CTRL_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_CTRL_LLN0_NamPlt_ldNs.mmsValue = MmsValue_newVisibleString("IEC 61850-7-4:2003");

iedModel_CTRL_XCBR1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_PROT_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_PROT_LLN0_NamPlt_ldNs.mmsValue = MmsValue_newVisibleString("IEC 61850-7-4:2003");

iedModel_PROT_PTOC1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);
}

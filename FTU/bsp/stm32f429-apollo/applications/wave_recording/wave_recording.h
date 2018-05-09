/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      wave_recording.h
  * @brief:     the application of recodeing waves
  * @version:   ver 1.1 
  * @author:    Mr.Sun
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
#ifndef __WAVE_RECORDING_H__
#define __WAVE_RECORDING_H__


/* INCLUDES FILES ------------------------------------------------------------*/
#include <rtthread.h>
#include "common_data.h" 


/* DEFINES -------------------------------------------------------------------*/
#define WAVE_RECORD_NUM    10     // 录波记录个数
#define WAVE_SDRAM_NUM     10     // SDRAM存储录波个数
#define WAVE_NUM_BEHIND    8      // 后8个周波
#define WAVE_NUM_BEFORE    5      // 前5个周波
#define WAVE_NUM_ALL       (WAVE_NUM_BEHIND + WAVE_NUM_BEFORE)  // 周波总数

#define AnalogChannelCount 8
#define DigitalChannelCount 1
#define ChannelsTotalCount AnalogChannelCount+DigitalChannelCount


/* STRUCTS -------------------------------------------------------------------*/
// 故障时刻
typedef struct time
{
    rt_uint16_t year;
    rt_uint16_t	month;
    rt_uint16_t	day;
    rt_uint16_t hour;
    rt_uint16_t minute;
    rt_uint16_t second;
    rt_uint16_t msecond;
}ComtradeTime;

typedef struct 
{
    rt_int8_t data_in;
    ComtradeTime time;
}WaveInfo;

typedef struct
{
    WaveInfo info[WAVE_RECORD_NUM];
    rt_int8_t in;
    rt_int8_t out;
}WaveRecord;

typedef struct 
{
    rt_int16_t ua[WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM];  
    rt_int16_t ub[WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM];
    rt_int16_t uC[WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM];
    rt_int16_t u0[WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM];
    rt_int16_t ia[WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM];
    rt_int16_t ib[WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM];
    rt_int16_t ic[WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM];
    rt_int16_t i0[WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM];
	
    char digital[WAVE_NUM_ALL * 96];
    ComtradeTime time;
}WaveSDRAM_Str;

//模拟通道
typedef struct AnalogChannel
{
    char *index;
    char *name;
    char *phase;
    char *circuitComponent;
    char *units;
    float a;
    float b;
    float skew;
    short min;
    short max;
    unsigned int primary;
    unsigned int secondary;
    char *isPrimary;	 	
} AnalogChannelInformation;
 
//数字通道
typedef struct DigitalChannel
{
    char *index;
    char *name;
    char *phase;
    char *circuitComponent;
    char *normalState;  
}DigitalChannelInformation;

// 配置文件
typedef struct Configuration
{
    char *stationName;
    char *deviceId;
    char *version;
    char *ChannelsCount;
    AnalogChannelInformation AnalogChannelData[AnalogChannelCount];
    DigitalChannelInformation DigitalChannelData[DigitalChannelCount];
    float frequency;
    int samplingRateCount;
    int samplingFrequency;
    int lastSampleNumber;
    char *firsttime;
    char *triggertime;
    char *filetype;
    char timemult;		
}ConfigurationHandler;


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
/* 采样队列 */
extern rt_int8_t g_WaveIn;  // 入指针
extern rt_int8_t g_WaveOut; // 出指针
extern WaveSDRAM_Str *g_WaveBuf;//[WAVE_SDRAM_NUM];


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
void EnQueueRecord(void);
void QueueRecordToSDRAM(void);
void WaveInit(void);

AnalogChannelInformation *InitAnalogChannelData(char *index, char *name, char *phase, char *units, float a);
DigitalChannelInformation *InitDigitalChannelData(char *index, char *name, char *normalState);
char* AnalogChannelDataToString(AnalogChannelInformation *AnalogChannelData);
char* DigitalChannelDataToString(DigitalChannelInformation *DigitalChannelIData);
char *ConfigurationDataToString(ConfigurationHandler *ConfigurationData,int *configDataSize);
char *CreatConfigurationHandler(char *firsttime, char *triggertime, int *configDataSize);
char *CreatDataHandler(short *Ia, short *Ib, short *Ic, short *Iz, short *Ua, short *Ub, short *Uc, short *Uz, char *D1);
char *Get_firsttime(ComtradeTime *time);
char *Get_triggertime(ComtradeTime *time);
char *Get_filenameDat(ComtradeTime *time, int *intervalNumber, int *recordNumber);
char *Get_filenameCfg(ComtradeTime *time, int *intervalNumber, int *recordNumber);

#endif /* __WAVE_RECORDING_H__ */


/* END OF FILE ---------------------------------------------------------------*/


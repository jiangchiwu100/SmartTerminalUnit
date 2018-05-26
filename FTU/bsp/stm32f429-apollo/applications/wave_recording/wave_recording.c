/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      wave_recording.c
  * @brief:     the application of recording waves.
  * @version:   ver 1.1 
  * @author:    Mr.Sun
  * @date:      2017.12.05
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
	
	
/* INCLUDES FILES ------------------------------------------------------------*/
#include "string.h"
#include "calculator.h"
#include "wave_recording.h"


/* PUBLIC VARIABLES ---------------------------------------------------------*/
WaveSDRAM_Str    *g_WaveBuf; // 录波缓存
rt_int8_t        g_WaveIn;  // 入指针
rt_int8_t        g_WaveOut; // 出指针


/* PRIVATE VARIABLES --------------------------------------------------------*/
static WaveRecord *QueueWaveRecord; // 录波记录队列 包括采样起始指针和时标

static char mystationName[] = "SOJO001";
static char mydeviceId[] = "1";
static char myversion[] = "1999";
static char ChannelsCount[] = "9,8A,1D";
static char myfiletype[] = "BINARY";

static ConfigurationHandler *ConfigurationData;// __attribute__((at(0x00301000 + SDRAM_ADDR_WAVE)));
static AnalogChannelInformation *myAnalogChannelData;// __attribute__((at(0x00302000 + SDRAM_ADDR_WAVE)));

//static ComtradeTime comtradeTime;
//WaveRecord *QueueWaveRecord1;
//WaveSDRAM_Str g_WaveBuf[WAVE_SDRAM_NUM] __attribute__((at(0x00200000 +  SDRAM_ADDR_WAVE))); // 数据存储空间

static char *buffer; // [1000] __attribute__((at(0x00303000 + SDRAM_ADDR_WAVE)));
static DigitalChannelInformation *myDigitalChannelData;// __attribute__((at(0x00304000 + SDRAM_ADDR_WAVE)));
static char *DATbuf; // [26 * 1152] __attribute__((at(0x00305000 + SDRAM_ADDR_WAVE)));
static char buftemp[100];

static char firsttime[100];
static char filenameDAT[100];
//static char triggertime[100];
//static char filenameCFG[100];


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : 入队录波记录（录波起始指针+时标）
  * @param : None.
  * @return: None.
  */
void EnQueueRecord(void)
{
    QueueWaveRecord->info[QueueWaveRecord->in].data_in = g_SampleQueueIn;
    QueueWaveRecord->info[QueueWaveRecord->in].time.year = g_SystemTime.year;
    QueueWaveRecord->info[QueueWaveRecord->in].time.month = g_SystemTime.month;
    QueueWaveRecord->info[QueueWaveRecord->in].time.day = g_SystemTime.day; 
    QueueWaveRecord->info[QueueWaveRecord->in].time.hour = g_SystemTime.hour; 
    QueueWaveRecord->info[QueueWaveRecord->in].time.minute = g_SystemTime.minute; 
    QueueWaveRecord->info[QueueWaveRecord->in].time.second = g_SystemTime.second; 
    QueueWaveRecord->info[QueueWaveRecord->in].time.msecond = g_SystemTime.msecond; 	
	
    if (++QueueWaveRecord->in >= WAVE_RECORD_NUM)
    {
        QueueWaveRecord->in = 0;
    }
}

/**
  * @brief : 录波记录初始化
  * @param : None.
  * @return: None.
  */
void QueueRecordToSDRAM(void)
{
    rt_uint8_t offset;
    rt_uint8_t queue_temp;   
	
    if (QueueWaveRecord->out != QueueWaveRecord->in)
    {
        if (g_SampleQueueIn - QueueWaveRecord->info[QueueWaveRecord->out].data_in >= 0)
        {
            offset = g_SampleQueueIn - QueueWaveRecord->info[QueueWaveRecord->out].data_in;
        }
        else
        {
            offset = (ADC_WAVE_SEMP_NUM * 2) + g_SampleQueueIn - QueueWaveRecord->info[QueueWaveRecord->out].data_in;
        }
		
        if (offset >= WAVE_NUM_BEHIND *2)
        {
            /* 录波数据 */ 
            queue_temp = g_SampleQueueIn;
            
            if (queue_temp < WAVE_NUM_ALL * 2)
            {
                memcpy(g_WaveBuf[g_WaveIn].ua, g_SampleQueueBuf->ua[(ADC_WAVE_SEMP_NUM * 2) + queue_temp - WAVE_NUM_ALL * 2], (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM * sizeof(short)); 
                memcpy(g_WaveBuf[g_WaveIn].ub, g_SampleQueueBuf->ub[(ADC_WAVE_SEMP_NUM * 2) + queue_temp - WAVE_NUM_ALL * 2], (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].uC, g_SampleQueueBuf->uC[(ADC_WAVE_SEMP_NUM * 2) + queue_temp - WAVE_NUM_ALL * 2], (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].u0, g_SampleQueueBuf->u0[(ADC_WAVE_SEMP_NUM * 2) + queue_temp - WAVE_NUM_ALL * 2], (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].ia, g_SampleQueueBuf->ia[(ADC_WAVE_SEMP_NUM * 2) + queue_temp - WAVE_NUM_ALL * 2], (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].ib, g_SampleQueueBuf->ib[(ADC_WAVE_SEMP_NUM * 2) + queue_temp - WAVE_NUM_ALL * 2], (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].ic, g_SampleQueueBuf->ic[(ADC_WAVE_SEMP_NUM * 2) + queue_temp - WAVE_NUM_ALL * 2], (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].i0, g_SampleQueueBuf->i0[(ADC_WAVE_SEMP_NUM * 2) + queue_temp - WAVE_NUM_ALL * 2], (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM * sizeof(short));
			
                if (queue_temp != 0)
                {
                    memcpy(g_WaveBuf[g_WaveIn].ua + (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM, g_SampleQueueBuf->ua, queue_temp * ADC_SAMPLE_NUM * sizeof(short)); 
                    memcpy(g_WaveBuf[g_WaveIn].ub + (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM, g_SampleQueueBuf->ub, queue_temp * ADC_SAMPLE_NUM * sizeof(short)); 
                    memcpy(g_WaveBuf[g_WaveIn].uC + (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM, g_SampleQueueBuf->uC, queue_temp * ADC_SAMPLE_NUM * sizeof(short)); 
                    memcpy(g_WaveBuf[g_WaveIn].u0 + (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM, g_SampleQueueBuf->u0, queue_temp * ADC_SAMPLE_NUM * sizeof(short)); 
                    memcpy(g_WaveBuf[g_WaveIn].ia + (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM, g_SampleQueueBuf->ia, queue_temp * ADC_SAMPLE_NUM * sizeof(short)); 
                    memcpy(g_WaveBuf[g_WaveIn].ib + (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM, g_SampleQueueBuf->ib, queue_temp * ADC_SAMPLE_NUM * sizeof(short)); 
                    memcpy(g_WaveBuf[g_WaveIn].ic + (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM, g_SampleQueueBuf->ic, queue_temp * ADC_SAMPLE_NUM * sizeof(short)); 
                    memcpy(g_WaveBuf[g_WaveIn].i0 + (WAVE_NUM_ALL * 2 - queue_temp) * ADC_SAMPLE_NUM, g_SampleQueueBuf->i0, queue_temp * ADC_SAMPLE_NUM * sizeof(short)); 	                
                }  
            }
            else if (queue_temp >= WAVE_NUM_ALL * 2)
            {
                memcpy(g_WaveBuf[g_WaveIn].ua, g_SampleQueueBuf->ua[queue_temp - WAVE_NUM_ALL * 2], WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM * sizeof(short)); 
                memcpy(g_WaveBuf[g_WaveIn].ub, g_SampleQueueBuf->ub[queue_temp - WAVE_NUM_ALL * 2], WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].uC, g_SampleQueueBuf->uC[queue_temp - WAVE_NUM_ALL * 2], WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].u0, g_SampleQueueBuf->u0[queue_temp - WAVE_NUM_ALL * 2], WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].ia, g_SampleQueueBuf->ia[queue_temp - WAVE_NUM_ALL * 2], WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].ib, g_SampleQueueBuf->ib[queue_temp - WAVE_NUM_ALL * 2], WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].ic, g_SampleQueueBuf->ic[queue_temp - WAVE_NUM_ALL * 2], WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM * sizeof(short));
                memcpy(g_WaveBuf[g_WaveIn].i0, g_SampleQueueBuf->i0[queue_temp - WAVE_NUM_ALL * 2], WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM * sizeof(short));				
            }
            
            /* 录波时间 */
            g_WaveBuf[g_WaveIn].time = QueueWaveRecord->info[QueueWaveRecord->out].time;
			
            if (++g_WaveIn >= WAVE_SDRAM_NUM)
            {
                g_WaveIn = 0;					
            }

//            if (g_WaveIn == g_WaveOut)
//            {
//                g_WaveIn = --g_WaveIn >= 0 ? --g_WaveIn : --g_WaveIn + WAVE_SDRAM_NUM;
//            }
						
            if (++QueueWaveRecord->out >= WAVE_RECORD_NUM)
            {
                QueueWaveRecord->out = 0;					
            }			
        }
    }
}

/**
  * @brief : 录波记录初始化
  * @param : None.
  * @return: None.
  */
int rt_wave_init(void)
{
    g_SampleQueueBuf = (Queue *)rt_malloc(sizeof(Queue));	
    QueueWaveRecord = (WaveRecord *)rt_malloc(sizeof(WaveRecord)); 	
    buffer = rt_malloc(1024);
    DATbuf = rt_malloc(26 * WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM);
    ConfigurationData = (ConfigurationHandler *)rt_malloc(sizeof(ConfigurationHandler));
    myAnalogChannelData = (AnalogChannelInformation *)rt_malloc(sizeof(AnalogChannelInformation));
    myDigitalChannelData = (DigitalChannelInformation *)rt_malloc(sizeof(DigitalChannelInformation));
    g_WaveBuf = (WaveSDRAM_Str *)rt_malloc(sizeof(WaveSDRAM_Str) * WAVE_SDRAM_NUM);
	
    memset(g_SampleQueueBuf, 0, sizeof(Queue));
    memset(QueueWaveRecord, 0, sizeof(WaveRecord));
    memset(g_WaveBuf, 0, sizeof(WaveSDRAM_Str) * WAVE_SDRAM_NUM);

    return(0);    
}
INIT_PREV_EXPORT(rt_wave_init);

/**
  * @brief  初始化模拟通道
  * @param  index:索引
  * @param  name: 名称
  * @param  phase:相别
  * @param  units:单位
  * @param  a:增益系数
  * @retval AnalogChannelInformation
  */
AnalogChannelInformation *InitAnalogChannelData(char *index, char *name, char *phase, char *units, float a)
{
	myAnalogChannelData->index = index;
	myAnalogChannelData->name = name;
	myAnalogChannelData->phase = phase;
	myAnalogChannelData->circuitComponent = "";
	myAnalogChannelData->units = units;
	myAnalogChannelData->a = a;
	myAnalogChannelData->b = 0;
	myAnalogChannelData->skew = 0;
	myAnalogChannelData->min = -32768;
	myAnalogChannelData->max = 32767;
	myAnalogChannelData->primary = 1;
	myAnalogChannelData->secondary = 1;
	myAnalogChannelData->isPrimary = "p";
	return (myAnalogChannelData);
}

/**
  * @brief  初始化数字通道
  * @param  index:索引
  * @param  name: 名称
  * @param  normalState:正常状态
  * @retval DigitalChannelInformation
  */
DigitalChannelInformation *InitDigitalChannelData(char *index, char *name, char *normalState)
{	 //数字通道
	myDigitalChannelData->index = index;
	myDigitalChannelData->name = name;
	myDigitalChannelData->phase = "";
	myDigitalChannelData->circuitComponent = "";
	myDigitalChannelData->normalState = normalState;
	return (myDigitalChannelData);
}
/**
  * @brief  模拟量通道数据转换成string
  * @param  AnalogChannelData:模拟通道
  * @retval 返回字符串数组指针.
  */
char* AnalogChannelDataToString(AnalogChannelInformation *AnalogChannelData)
{
	int j = 0;

	j = sprintf(buftemp, "%s,%s,%s,%s,%s,", AnalogChannelData->index, AnalogChannelData->name, AnalogChannelData->phase,
		AnalogChannelData->circuitComponent, AnalogChannelData->units);
	j += sprintf(buftemp + j, "%f,%f,%f,", AnalogChannelData->a, AnalogChannelData->b, AnalogChannelData->skew);
	j += sprintf(buftemp + j, "%d,%d,%d,%d,", AnalogChannelData->min, AnalogChannelData->max, AnalogChannelData->primary, AnalogChannelData->secondary);
	j += sprintf(buftemp + j, "%s", AnalogChannelData->isPrimary);
	return buftemp;
}
/**
  * @brief  状态量通道数据转换成string
  * @param  AnalogChannelData:状态量通道
  * @retval 返回字符串数组指针.
  */
char* DigitalChannelDataToString(DigitalChannelInformation *DigitalChannelIData)
{
	sprintf(buftemp, "%s,%s,%s,%s,%s", DigitalChannelIData->index, DigitalChannelIData->name, DigitalChannelIData->phase,
	DigitalChannelIData->circuitComponent, DigitalChannelIData->normalState);
	return buftemp;
}

/**
  * @brief  输出.dat文件到字符串数组.
  * @param  ConfigurationData:.dat文件
  * @retval 返回字符串数组指针.
  */
char *ConfigurationDataToString(ConfigurationHandler *ConfigurationData,int *configDataSize)
{
	int j;
	j = sprintf(buffer, "%s,%s,%s\n", ConfigurationData->stationName, ConfigurationData->deviceId, ConfigurationData->version);
	j += sprintf(buffer + j, "%s\n", ConfigurationData->ChannelsCount);
	j += sprintf(buffer + j, "%s\n", AnalogChannelDataToString(&(ConfigurationData->AnalogChannelData[0])));
	j += sprintf(buffer + j, "%s\n", AnalogChannelDataToString(&(ConfigurationData->AnalogChannelData[1])));
	j += sprintf(buffer + j, "%s\n", AnalogChannelDataToString(&(ConfigurationData->AnalogChannelData[2])));
	j += sprintf(buffer + j, "%s\n", AnalogChannelDataToString(&(ConfigurationData->AnalogChannelData[3])));
	j += sprintf(buffer + j, "%s\n", AnalogChannelDataToString(&(ConfigurationData->AnalogChannelData[4])));
	j += sprintf(buffer + j, "%s\n", AnalogChannelDataToString(&(ConfigurationData->AnalogChannelData[5])));
	j += sprintf(buffer + j, "%s\n", AnalogChannelDataToString(&(ConfigurationData->AnalogChannelData[6])));
	j += sprintf(buffer + j, "%s\n", AnalogChannelDataToString(&(ConfigurationData->AnalogChannelData[7])));
	j += sprintf(buffer + j, "%s\n", DigitalChannelDataToString(&(ConfigurationData->DigitalChannelData[0])));
	j += sprintf(buffer + j, "%f\n", ConfigurationData->frequency);
	j += sprintf(buffer + j, "%d\n", ConfigurationData->samplingRateCount);
	j += sprintf(buffer + j, "%d,%d\n", ConfigurationData->samplingFrequency, ConfigurationData->lastSampleNumber);
	j += sprintf(buffer + j, "%s\n", ConfigurationData->firsttime);
	j += sprintf(buffer + j, "%s\n", ConfigurationData->triggertime);
	j += sprintf(buffer + j, "%s\n", ConfigurationData->filetype);
	j += sprintf(buffer + j, "%d\n", ConfigurationData->timemult);
	*configDataSize = j;
	//printf("%s",buffer);
	return buffer;
}

/**
  * @brief  创建CFG文件结构
  * @param  firsttime:第一次采样时间
  * @param  triggertime:触发时间
  * @retval 返回字符串数组指针.
  */
char *CreatConfigurationHandler(char *firsttime, char *triggertime, int *configDataSize)
{
	ConfigurationData->stationName = mystationName;
	ConfigurationData->deviceId = mydeviceId;
	ConfigurationData->version = myversion;
	ConfigurationData->ChannelsCount = ChannelsCount;

	ConfigurationData->AnalogChannelData[0] = *InitAnalogChannelData("1", "IA", "A", "A", g_CalibrateFactor[CALIFACTOR_Ia]);
	ConfigurationData->AnalogChannelData[1] = *InitAnalogChannelData("2", "IB", "B", "A", g_CalibrateFactor[CALIFACTOR_Ib]);
	ConfigurationData->AnalogChannelData[2] = *InitAnalogChannelData("3", "IC", "C", "A", g_CalibrateFactor[CALIFACTOR_Ic]);
	ConfigurationData->AnalogChannelData[3] = *InitAnalogChannelData("4", "IZ", "Z", "A", g_CalibrateFactor[CALIFACTOR_I0]);
	ConfigurationData->AnalogChannelData[4] = *InitAnalogChannelData("5", "UA", "A", "v", g_CalibrateFactor[CALIFACTOR_Uab]);
	ConfigurationData->AnalogChannelData[5] = *InitAnalogChannelData("6", "UB", "B", "v", g_CalibrateFactor[CALIFACTOR_Uac]);
	ConfigurationData->AnalogChannelData[6] = *InitAnalogChannelData("7", "UC", "C", "v", g_CalibrateFactor[CALIFACTOR_Ucb]);
	ConfigurationData->AnalogChannelData[7] = *InitAnalogChannelData("8", "UZ", "Z", "v", g_CalibrateFactor[CALIFACTOR_U0]);

	ConfigurationData->DigitalChannelData[0] = *InitDigitalChannelData("1", "KG", "0");
	ConfigurationData->frequency = 50.00;
	ConfigurationData->samplingRateCount = 1;
	ConfigurationData->samplingFrequency = 4800;
	ConfigurationData->lastSampleNumber = WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM;
	ConfigurationData->firsttime = firsttime;
	ConfigurationData->triggertime = triggertime;
	ConfigurationData->filetype = myfiletype;
	ConfigurationData->timemult = 1;
	//printf("%s",ConfigurationData->AnalogChannelData[7].index);
	//printf("%s",ConfigurationData->firsttime);
	return ConfigurationDataToString(ConfigurationData,configDataSize);
}
/**
  * @brief  创建.DAT文件
	* @param  *Ia:A电流采样数据
	* @param  *Ib:B电流采样数据
	* @param  *Ic:C电流采样数据
	* @param  Iz:I0电流采样数据
	* @param  Ua:A电流采样数据
	* @param  Ub:B电流采样数据
	* @param  Uc:C电流采样数据
	* @param  Uz:零序电流采样数据
	* @param  D1:状态量输入数据
  * @retval ·返回字符串数组指针.
  */
char *CreatDataHandler(short *Ia, short *Ib, short *Ic, short *Iz, short *Ua, short *Ub, short *Uc, short *Uz, char *D1)
{
	int i = 0, j = 0;
	for (i = 0, j = 0; i < WAVE_NUM_ALL * 2 * ADC_SAMPLE_NUM; i++)
	{
		DATbuf[j++] = (i + 1);
		DATbuf[j++] = (i + 1) >> 8;
		DATbuf[j++] = (i + 1) >> 16;
		DATbuf[j++] = (i + 1) >> 24;

		DATbuf[j++] = (i * 208);
		DATbuf[j++] = (i * 208) >> 8;
		DATbuf[j++] = (i * 208) >> 16;
		DATbuf[j++] = (i * 208) >> 24;

		DATbuf[j++] = Ia[i];
		DATbuf[j++] = Ia[i] >> 8;

		DATbuf[j++] = Ib[i];
		DATbuf[j++] = Ib[i] >> 8;

		DATbuf[j++] = Ic[i];
		DATbuf[j++] = Ic[i] >> 8;

		DATbuf[j++] = Iz[i];
		DATbuf[j++] = Iz[i] >> 8;

		DATbuf[j++] = Ua[i];
		DATbuf[j++] = Ua[i] >> 8;

		DATbuf[j++] = Ub[i];
		DATbuf[j++] = Ub[i] >> 8;

		DATbuf[j++] = Uc[i];
		DATbuf[j++] = Uc[i] >> 8;

		DATbuf[j++] = Uz[i];
		DATbuf[j++] = Uz[i] >> 8;

		DATbuf[j++] = D1[i];
		DATbuf[j++] = 0;
	}
	return DATbuf;
}


/**
  * @brief  第一次记录时间.
  * @param  ComtradeTime.
  * @retval 返回字符串数组指针.
  */
char *Get_firsttime(ComtradeTime *time)
{
	int time_temp = 0;
	int day_temp, hour_temp, minute_temp, second_temp, millisecond_temp;

	time_temp = time->msecond + time->second * 1000 + time->minute * 60 * 1000 + time->hour * 60 * 60 * 1000 + time->day * 24 * 60 * 60 * 1000 - 80;
	day_temp = time_temp / 1000 / 60 / 60 / 24;
	hour_temp = time_temp / 1000 / 60 / 60 % 24;
	minute_temp = time_temp / 1000 / 60 % 60;
	second_temp = time_temp / 1000 % 60;
	millisecond_temp = time_temp % 1000;

	sprintf(firsttime, "%02d/%02d/%d,%02d:%02d:%02d.%03d%03d",
		day_temp, time->month, time->year, hour_temp, minute_temp, second_temp, millisecond_temp, 0);
	return firsttime;
}
/**
  * @brief  触发时刻时间.
  * @param  ComtradeTime.
  * @retval 返回字符串数组指针.
  */
char *Get_triggertime(ComtradeTime *time)
{
	sprintf(firsttime, "%02d/%02d/%d,%02d:%02d:%02d.%03d%03d",
		time->day, time->month, time->year, time->hour, time->minute, time->second, time->msecond, 0);
	return firsttime;
}
/**
  * @brief  获取DAT文件名
  * @param  ComtradeTime.
  * @param  intervalNumber 间隔序号
  * @param  ComtradeTime   录波序号
  * @retval 返回字符串数组指针
  */
char *Get_filenameDat(ComtradeTime *time, int *intervalNumber, int *recordNumber)
{
	sprintf(filenameDAT, "BAY%02d_%04d_%d%02d%02d_%02d%02d%02d_%03d.dat",
		*intervalNumber, *recordNumber, time->year, time->month, time->day, time->hour, time->minute, time->second, time->msecond);
	return filenameDAT;
}
/**
  * @brief  获取cfg文件名
  * @param  ComtradeTime.
  * @param  intervalNumber 间隔序号
  * @param  ComtradeTime   录波序号
  * @retval 返回字符串数组指针.
  */
char *Get_filenameCfg(ComtradeTime *time, int *intervalNumber, int *recordNumber)
{
	sprintf(filenameDAT, "BAY%02d_%04d_%d%02d%02d_%02d%02d%02d_%03d.cfg",
		*intervalNumber, *recordNumber, time->year, time->month, time->day, time->hour, time->minute, time->second, time->msecond);
	return filenameDAT;
}


/* END OF FILE ---------------------------------------------------------------*/


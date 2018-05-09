/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      calculator.c
  * @brief:     The calculator application.
  * @version:   V1.0.0 
  * @author:    Mr.Sun
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
	
/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f429xx.h"
#include "calculator.h"
#include "common_data.h"
#include "wave_recording.h"
#include <math.h>
#include <arm_math.h>


/* PUBLIC VARIABLES ----------------------------------------------------------*/
// U-I 角度
float g_Alpha[ALPHA_NUM];

/* PRIVATE VARIABLES ---------------------------------------------------------*/
static arm_cfft_radix2_instance_f32 Scfft;

static float FFT_InputBufUa[CALC_TIMES][FFT_LENGTH * 2]; // FFT输入数组 存储FFT=所用临时变量
static float FFT_InputBufUb[CALC_TIMES][FFT_LENGTH * 2];
static float FFT_InputBufUc[CALC_TIMES][FFT_LENGTH * 2];
static float FFT_InputBufU0[CALC_TIMES][FFT_LENGTH * 2];
static float FFT_InputBufIa[CALC_TIMES][FFT_LENGTH * 2];
static float FFT_InputBufIb[CALC_TIMES][FFT_LENGTH * 2];
static float FFT_InputBufIc[CALC_TIMES][FFT_LENGTH * 2];
static float FFT_InputBufI0[CALC_TIMES][FFT_LENGTH * 2];
static float FFT_InputBufUA[CALC_TIMES][FFT_LENGTH * 2];	
static float FFT_InputBufUC[CALC_TIMES][FFT_LENGTH * 2];

// 计算三次的模值
static float VUa[CALC_TIMES], VUb[CALC_TIMES], VUc[CALC_TIMES], VU0[CALC_TIMES];
static float VIa[CALC_TIMES], VIb[CALC_TIMES], VIc[CALC_TIMES], VI0[CALC_TIMES];
static float VUA[CALC_TIMES], VUC[CALC_TIMES];
//static float VPa[CALC_TIMES], VPb[CALC_TIMES], VPc[CALC_TIMES];
//static float VQa[CALC_TIMES], VQb[CALC_TIMES], VQc[CALC_TIMES];
//static float VSa[CALC_TIMES], VSb[CALC_TIMES], VSc[CALC_TIMES];
static float VUa3[CALC_TIMES],VUb3[CALC_TIMES],VUc3[CALC_TIMES],VU03[CALC_TIMES]; // 电压三次谐波
static float VUa5[CALC_TIMES],VUb5[CALC_TIMES],VUc5[CALC_TIMES],VU05[CALC_TIMES]; // 电压五次谐波
static float VIa2[CALC_TIMES],VIb2[CALC_TIMES],VIc2[CALC_TIMES]; // 电流二次谐波
static float VIa3[CALC_TIMES],VIb3[CALC_TIMES],VIc3[CALC_TIMES],VI03[CALC_TIMES]; // 电流三次谐波
static float VIa5[CALC_TIMES],VIb5[CALC_TIMES],VIc5[CALC_TIMES],VI05[CALC_TIMES]; // 电流五次谐波

// 最终计算的值
static float Ua, Ub, Uc, U0;
static float Ia, Ib, Ic, I0;
static float UA, UC;
static float Udc1, Udc2;
static float Pa, Pb, Pc;
static float Qa, Qb, Qc;
//static float Sa, Sb, Sc;
//static float Fa, Fb, Fc;
static float Ua3,Ub3,Uc3,U03;
static float Ua5,Ub5,Uc5,U05;
static float Ia2,Ib2,Ic2;
static float Ia3,Ib3,Ic3,I03;
static float Ia5,Ib5,Ic5,I05;
static float P, Q, S, F;

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief  处理AD7606采样序列，添加虚部为0
  * @param  buf_from-输入序列.
  * @param  buf_to-输出序列.
  * @retval 无
  */
static void BufDiv(double *buf_from, float buf_to[CALC_TIMES][FFT_LENGTH * 2])
{
    unsigned char i, j;

    for (j = 0; j < CALC_TIMES; j++)
    {
        for (i = 0; i < FFT_LENGTH; i++)
        {
            buf_to[j][2 * i] = buf_from[i * 3 + j];
            buf_to[j][2 * i + 1] = 0;//虚部全为0
        }
    }
}
/**
  * @brief  求计算结果的中间值
  * @param  buf-输入数组
  * @param  n-长度
  * @retval 计算的中间值
  */
static float CalcAverage(float *buf, int n)
{
    int i, j;
    float temp;
    for (j = 0; j < n - 1; j++)
        for (i = 0; i < n - 1 - j; i++)
        {
            if (buf[i] > buf[i + 1])
            {
                temp = buf[i];
                buf[i] = buf[i + 1];
                buf[i + 1] = temp;
            }
        }
    return buf[1];
}

/**
  * @Description: 计算U与I间的夹角
  * @param:  imageU-电压虚部
  * @param:  floatU-电压实部
  * @param:  imageI-电压虚部
  * @param:  floatI-电压实部
  * @return: (radU - radI)-U与I之间的夹角
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static float CalculateAngleU_I(float imageU, float floatU, float imageI, float floatI)
{
    float angleU_I = 0;
    float radU_I = 0;
    int8_t  xU_I,yU_I,zU,zI = 0;

    if((floatU == 0)&&(floatI == 0))
    {
        radU_I = 0;
    }
    else if(floatU == 0)
    {
        radU_I = -3.1415926f/2;
    }
    else if(floatI == 0)
    {
        radU_I = 3.1415926f/2;
    }
    else
    {
        radU_I = atan(((imageU / floatU) - (imageI / floatI))/(1.0f + (imageU / floatU)*(imageI / floatI)));
    }

    yU_I = (imageU>=0 ? 1:-1) * (imageI>=0 ? 1:-1);
    xU_I = (floatU>=0 ? 1:-1) * (floatI>=0 ? 1:-1);
    zU = (imageU>=0 ? 1:-1) * (floatU>=0 ? 1:-1);
    zI = (imageI>=0 ? 1:-1) * (floatI>=0 ? 1:-1);

    if((xU_I>=0) && (yU_I>=0))//同象限
    {
        radU_I = (radU_I + 0.0f);
    }
    else if((xU_I<=0) && (yU_I<=0))//以U为基准，I超前2个象限
    {
        radU_I = (radU_I + 3.1415926f);
    }
    else if((xU_I>=0) && (yU_I<=0))//以U与I上下相邻象限
    {
        if((zU>0)|(zI<0))
        {
            if(radU_I<0)//以U为基准，I超前1个象限
            {
                radU_I = (radU_I + 3.1415926f);
            }
            else
            {
                radU_I = (radU_I + 0.0f);
            }
        }
        else
        {
            if(radU_I<0)//以U为基准，I超前3个象限
            {
                radU_I = (radU_I + 0.0f);
            }
            else
            {
                radU_I = (radU_I + 3.1415926f);	;
            }
        }
    }
    else if((xU_I<=0) && (yU_I>=0))//以U与I左右相邻象限
    {
        if((zU>0)|(zI<0))
        {
            if(radU_I<0)//以U为基准，I超前3个象限
            {
                radU_I = (radU_I + 0.0f);
            }
            else
            {
                radU_I = (radU_I + 3.1415926f);	;
            }
        }
        else
        {
            if(radU_I<0)//以U为基准，I超前1个象限
            {
                radU_I = (radU_I + 3.1415926f);
            }
            else
            {
                radU_I = (radU_I + 0.0f);
            }
        }
    }

    angleU_I = radU_I * FACTOR_RAD_TO_ANGLE;
	
//	angleU_I = angleU_I > 180 ? angleU_I - 360 : angleU_I;
//	angleU_I = angleU_I < (-180) ? angleU_I + 360 : angleU_I;

    return(angleU_I);
}

/**
  * @Description: 对AD7606采样值进行计算.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void CalculateData(void)
{
    int i;
    static rt_uint8_t k; 
    rt_uint8_t queue_temp;
    
    queue_temp = g_SampleQueueIn;
    
    if (queue_temp >= 2)
    {
        for (i =  0; i < 2 * ADC_SAMPLE_NUM; i++)
        {
            g_UaSampleBuf[i] = *(g_SampleQueueBuf->ua[queue_temp - 2]+i);
            g_UbSampleBuf[i] = *(g_SampleQueueBuf->ub[queue_temp - 2]+i);
            g_UcSampleBuf[i] = *(g_SampleQueueBuf->uc[queue_temp - 2]+i);
            g_U0SampleBuf[i] = *(g_SampleQueueBuf->u0[queue_temp - 2]+i);

            g_IaSampleBuf[i] = *(g_SampleQueueBuf->ia[queue_temp - 2]+i);
            g_IbSampleBuf[i] = *(g_SampleQueueBuf->ib[queue_temp - 2]+i);
            g_IcSampleBuf[i] = *(g_SampleQueueBuf->ic[queue_temp - 2]+i);
            g_I0SampleBuf[i] = *(g_SampleQueueBuf->i0[queue_temp - 2]+i);
            
            g_UASampleBuf[i] = *(g_SampleQueueBuf->uA[queue_temp - 2]+i);
            g_UCSampleBuf[i] = *(g_SampleQueueBuf->uC[queue_temp - 2]+i);
            g_Udc1SampleBuf[i] = *(g_SampleQueueBuf->udc1[queue_temp - 2]+i);
            g_Udc2SampleBuf[i] = *(g_SampleQueueBuf->udc2[queue_temp - 2]+i);
        }
    }
    else if (queue_temp == 0)
    {
        for (i =  0; i < 2 * ADC_SAMPLE_NUM; i++)
        {
            g_UaSampleBuf[i] = *(g_SampleQueueBuf->ua[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            g_UbSampleBuf[i] = *(g_SampleQueueBuf->ub[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            g_UcSampleBuf[i] = *(g_SampleQueueBuf->uc[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            g_U0SampleBuf[i] = *(g_SampleQueueBuf->u0[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);

            g_IaSampleBuf[i] = *(g_SampleQueueBuf->ia[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            g_IbSampleBuf[i] = *(g_SampleQueueBuf->ib[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            g_IcSampleBuf[i] = *(g_SampleQueueBuf->ic[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            g_I0SampleBuf[i] = *(g_SampleQueueBuf->i0[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            
            g_UASampleBuf[i] = *(g_SampleQueueBuf->uA[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            g_UCSampleBuf[i] = *(g_SampleQueueBuf->uC[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            g_Udc1SampleBuf[i] = *(g_SampleQueueBuf->udc1[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
            g_Udc2SampleBuf[i] = *(g_SampleQueueBuf->udc2[(ADC_WAVE_SEMP_NUM * 2) - 2]+i);
        }
    }
    else if (queue_temp == 1)
    {
        for (i = 0; i < ADC_SAMPLE_NUM; i++)
        {
            g_UaSampleBuf[i] = *(g_SampleQueueBuf->ua[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            g_UbSampleBuf[i] = *(g_SampleQueueBuf->ub[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            g_UcSampleBuf[i] = *(g_SampleQueueBuf->uc[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            g_U0SampleBuf[i] = *(g_SampleQueueBuf->u0[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);

            g_IaSampleBuf[i] = *(g_SampleQueueBuf->ia[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            g_IbSampleBuf[i] = *(g_SampleQueueBuf->ib[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            g_IcSampleBuf[i] = *(g_SampleQueueBuf->ic[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            g_I0SampleBuf[i] = *(g_SampleQueueBuf->i0[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            
            g_UASampleBuf[i] = *(g_SampleQueueBuf->uA[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            g_UCSampleBuf[i] = *(g_SampleQueueBuf->uC[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            g_Udc1SampleBuf[i] = *(g_SampleQueueBuf->udc1[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
            g_Udc2SampleBuf[i] = *(g_SampleQueueBuf->udc2[(ADC_WAVE_SEMP_NUM * 2) - 1]+i);
        }
        for (i = 0; i < ADC_SAMPLE_NUM; i++)
        {
            g_UaSampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->ua[0]+i);
            g_UbSampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->ub[0]+i);
            g_UcSampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->uc[0]+i);
            g_U0SampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->u0[0]+i);

            g_IaSampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->ia[0]+i);
            g_IbSampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->ib[0]+i);
            g_IcSampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->ic[0]+i);
            g_I0SampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->i0[0]+i);
            
            g_UASampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->uA[0]+i);
            g_UCSampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->uC[0]+i);
            g_Udc1SampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->udc1[0]+i);
            g_Udc2SampleBuf[ADC_SAMPLE_NUM + i] = *(g_SampleQueueBuf->udc2[0]+i);
        }
    }

    BufDiv(g_UaSampleBuf, FFT_InputBufUa);
    BufDiv(g_UbSampleBuf, FFT_InputBufUb);
    BufDiv(g_UcSampleBuf, FFT_InputBufUc);
    BufDiv(g_U0SampleBuf, FFT_InputBufU0);
    BufDiv(g_IaSampleBuf, FFT_InputBufIa);
    BufDiv(g_IbSampleBuf, FFT_InputBufIb);
    BufDiv(g_IcSampleBuf, FFT_InputBufIc);
    BufDiv(g_I0SampleBuf, FFT_InputBufI0);
    BufDiv(g_UASampleBuf, FFT_InputBufUA);
    BufDiv(g_UCSampleBuf, FFT_InputBufUC);

//    printf("%d\n",(short)UA1_SampleBuf[w]);
    
    for (i = 0; i < CALC_TIMES; i++)
    {
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufUa[i]);	//FFT
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufUb[i]);
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufUc[i]);
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufU0[i]);
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufIa[i]);
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufIb[i]);
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufIc[i]);
	  #ifndef SYNTHESIS_I0_ENABLE
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufI0[i]);
	  #endif		        
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufUA[i]);
        arm_cfft_radix2_f32(&Scfft, FFT_InputBufUC[i]);

        VUa[i] = sqrt(FFT_InputBufUa[i][2] * FFT_InputBufUa[i][2] + FFT_InputBufUa[i][3] * FFT_InputBufUa[i][3]);
        VUb[i] = sqrt(FFT_InputBufUb[i][2] * FFT_InputBufUb[i][2] + FFT_InputBufUb[i][3] * FFT_InputBufUb[i][3]);
        VUc[i] = sqrt(FFT_InputBufUc[i][2] * FFT_InputBufUc[i][2] + FFT_InputBufUc[i][3] * FFT_InputBufUc[i][3]);
        VU0[i] = sqrt(FFT_InputBufU0[i][2] * FFT_InputBufU0[i][2] + FFT_InputBufU0[i][3] * FFT_InputBufU0[i][3]);

        VUa3[i] = sqrt(FFT_InputBufUa[i][6] * FFT_InputBufUa[i][6] + FFT_InputBufUa[i][7] * FFT_InputBufUa[i][7]);
        VUb3[i] = sqrt(FFT_InputBufUb[i][6] * FFT_InputBufUb[i][6] + FFT_InputBufUb[i][7] * FFT_InputBufUb[i][7]);
        VUc3[i] = sqrt(FFT_InputBufUc[i][6] * FFT_InputBufUc[i][6] + FFT_InputBufUc[i][7] * FFT_InputBufUc[i][7]);
        VU03[i] = sqrt(FFT_InputBufU0[i][6] * FFT_InputBufU0[i][6] + FFT_InputBufU0[i][7] * FFT_InputBufU0[i][7]);

        VUa5[i] = sqrt(FFT_InputBufUa[i][10] * FFT_InputBufUa[i][10] + FFT_InputBufUa[i][11] * FFT_InputBufUa[i][11]);
        VUb5[i] = sqrt(FFT_InputBufUb[i][10] * FFT_InputBufUb[i][10] + FFT_InputBufUb[i][11] * FFT_InputBufUb[i][11]);
        VUc5[i] = sqrt(FFT_InputBufUc[i][10] * FFT_InputBufUc[i][10] + FFT_InputBufUc[i][11] * FFT_InputBufUc[i][11]);
        VU05[i] = sqrt(FFT_InputBufU0[i][10] * FFT_InputBufU0[i][10] + FFT_InputBufU0[i][11] * FFT_InputBufU0[i][11]);

        VIa[i] = sqrt(FFT_InputBufIa[i][2] * FFT_InputBufIa[i][2] + FFT_InputBufIa[i][3] * FFT_InputBufIa[i][3]);
        VIb[i] = sqrt(FFT_InputBufIb[i][2] * FFT_InputBufIb[i][2] + FFT_InputBufIb[i][3] * FFT_InputBufIb[i][3]);
        VIc[i] = sqrt(FFT_InputBufIc[i][2] * FFT_InputBufIc[i][2] + FFT_InputBufIc[i][3] * FFT_InputBufIc[i][3]);		
	  #ifdef SYNTHESIS_I0_ENABLE
        FFT_InputBufI0[i][2] += (FFT_InputBufIa[i][2] + FFT_InputBufIb[i][2] + FFT_InputBufIc[i][2]);
        FFT_InputBufI0[i][3] += (FFT_InputBufIa[i][3] + FFT_InputBufIb[i][3] + FFT_InputBufIc[i][3]);
	  #endif	  
        VI0[i] = sqrt(FFT_InputBufI0[i][2] * FFT_InputBufI0[i][2] + FFT_InputBufI0[i][3] * FFT_InputBufI0[i][3]);	

        VIa2[i] = sqrt(FFT_InputBufIa[i][4] * FFT_InputBufIa[i][4] + FFT_InputBufIa[i][5] * FFT_InputBufIa[i][5]);
        VIb2[i] = sqrt(FFT_InputBufIb[i][4] * FFT_InputBufIb[i][4] + FFT_InputBufIb[i][5] * FFT_InputBufIb[i][5]);
        VIc2[i] = sqrt(FFT_InputBufIc[i][4] * FFT_InputBufIc[i][4] + FFT_InputBufIc[i][5] * FFT_InputBufIc[i][5]);
		
        VIa3[i] = sqrt(FFT_InputBufIa[i][6] * FFT_InputBufIa[i][6] + FFT_InputBufIa[i][7] * FFT_InputBufIa[i][7]);
        VIb3[i] = sqrt(FFT_InputBufIb[i][6] * FFT_InputBufIb[i][6] + FFT_InputBufIb[i][7] * FFT_InputBufIb[i][7]);
        VIc3[i] = sqrt(FFT_InputBufIc[i][6] * FFT_InputBufIc[i][6] + FFT_InputBufIc[i][7] * FFT_InputBufIc[i][7]);
	  #ifdef SYNTHESIS_I0_ENABLE
        FFT_InputBufI0[i][6] += (FFT_InputBufIa[i][6] + FFT_InputBufIb[i][6] + FFT_InputBufIc[i][6]);
        FFT_InputBufI0[i][7] += (FFT_InputBufIa[i][7] + FFT_InputBufIb[i][7] + FFT_InputBufIc[i][7]);
	  #endif		
        VI03[i] = sqrt(FFT_InputBufI0[i][6] * FFT_InputBufI0[i][6] + FFT_InputBufI0[i][7] * FFT_InputBufI0[i][7]);
        
        VIa5[i] = sqrt(FFT_InputBufIa[i][10] * FFT_InputBufIa[i][10] + FFT_InputBufIa[i][11] * FFT_InputBufIa[i][11]);
        VIb5[i] = sqrt(FFT_InputBufIb[i][10] * FFT_InputBufIb[i][10] + FFT_InputBufIb[i][11] * FFT_InputBufIb[i][11]);
        VIc5[i] = sqrt(FFT_InputBufIc[i][10] * FFT_InputBufIc[i][10] + FFT_InputBufIc[i][11] * FFT_InputBufIc[i][11]);
	  #ifdef SYNTHESIS_I0_ENABLE
        FFT_InputBufI0[i][10] += (FFT_InputBufIa[i][10] + FFT_InputBufIb[i][10] + FFT_InputBufIc[i][10]);
        FFT_InputBufI0[i][11] += (FFT_InputBufIa[i][11] + FFT_InputBufIb[i][11] + FFT_InputBufIc[i][11]);
	  #endif		
        VI05[i] = sqrt(FFT_InputBufI0[i][10] * FFT_InputBufI0[i][10] + FFT_InputBufI0[i][11] * FFT_InputBufI0[i][11]);
        
        VUA[i] = sqrt(FFT_InputBufUA[i][2] * FFT_InputBufUA[i][2] + FFT_InputBufUA[i][3] * FFT_InputBufUA[i][3]);
        VUC[i] = sqrt(FFT_InputBufUC[i][2] * FFT_InputBufUC[i][2] + FFT_InputBufUC[i][3] * FFT_InputBufUC[i][3]);
    }

//  ParameterCheck(CALIBRATE_FACTOR);
    
    /* 计算UI间角度 */
    if (++k % 10 == 1)
    {
        k = 0;
        g_Alpha[0] = CalculateAngleU_I(FFT_InputBufUa[1][3], FFT_InputBufUa[1][2], FFT_InputBufIa[1][3], FFT_InputBufIa[1][2]) - 0.29f;//g_ParameterDB.Data.calibrateFactor.Str.califactorAlphaUaIa;
        g_Alpha[1] = CalculateAngleU_I(FFT_InputBufUb[1][3], FFT_InputBufUb[1][2], FFT_InputBufIb[1][3], FFT_InputBufIb[1][2]) - 0.29f;//g_ParameterDB.Data.calibrateFactor.Str.califactorAlphaUbIb;
        g_Alpha[2] = CalculateAngleU_I(FFT_InputBufUC[1][3], FFT_InputBufUC[1][2], FFT_InputBufIc[1][3], FFT_InputBufIc[1][2]) - 0.29f;//g_ParameterDB.Data.calibrateFactor.Str.califactorAlphaUcIc;
        g_Alpha[3] = CalculateAngleU_I(FFT_InputBufU0[1][3], FFT_InputBufU0[1][2], FFT_InputBufI0[1][3], FFT_InputBufI0[1][2]) - 0.29f;//g_ParameterDB.Data.calibrateFactor.Str.califactorAlphaU0I0;
		g_Alpha[4] = CalculateAngleU_I(FFT_InputBufUa[1][3], FFT_InputBufUa[1][2], FFT_InputBufUC[1][3], FFT_InputBufUC[1][2]) - g_ParameterDB.Data.calibrateFactor.Str.califactorAlphaUaUb;
    }

    Ua = CalcAverage(VUa, CALC_TIMES) * VOLTAGE_RADIO;
    Ub = CalcAverage(VUb, CALC_TIMES) * VOLTAGE_RADIO;
    Uc = CalcAverage(VUc, CALC_TIMES) * VOLTAGE_RADIO;
    U0 = CalcAverage(VU0, CALC_TIMES) * VOLTAGE_RADIO;

    Ua3 = CalcAverage(VUa3, CALC_TIMES) * VOLTAGE_RADIO;
    Ub3 = CalcAverage(VUb3, CALC_TIMES) * VOLTAGE_RADIO;
    Uc3 = CalcAverage(VUc3, CALC_TIMES) * VOLTAGE_RADIO;
    U03 = CalcAverage(VU03, CALC_TIMES) * VOLTAGE_RADIO;

    Ua5 = CalcAverage(VUa5, CALC_TIMES) * VOLTAGE_RADIO;
    Ub5 = CalcAverage(VUb5, CALC_TIMES) * VOLTAGE_RADIO;
    Uc5 = CalcAverage(VUc5, CALC_TIMES) * VOLTAGE_RADIO;
    U05 = CalcAverage(VU05, CALC_TIMES) * VOLTAGE_RADIO;

    Ia = CalcAverage(VIa, CALC_TIMES) * CURRENT_RADIO;
    Ib = CalcAverage(VIb, CALC_TIMES) * CURRENT_RADIO;
    Ic = CalcAverage(VIc, CALC_TIMES) * CURRENT_RADIO;
    I0 = CalcAverage(VI0, CALC_TIMES) * CURRENT_RADIO;

    //printf("%f  %f  %f\n", Ia,Ib,Uc);
    Ia2 = CalcAverage(VIa2, CALC_TIMES) * CURRENT_RADIO;
    Ib2 = CalcAverage(VIb2, CALC_TIMES) * CURRENT_RADIO;
    Ic2 = CalcAverage(VIc2, CALC_TIMES) * CURRENT_RADIO;
	
    Ia3 = CalcAverage(VIa3, CALC_TIMES) * CURRENT_RADIO;
    Ib3 = CalcAverage(VIb3, CALC_TIMES) * CURRENT_RADIO;
    Ic3 = CalcAverage(VIc3, CALC_TIMES) * CURRENT_RADIO;
    I03 = CalcAverage(VI03, CALC_TIMES) * CURRENT_RADIO;

    Ia5 = CalcAverage(VIa5, CALC_TIMES) * CURRENT_RADIO;
    Ib5 = CalcAverage(VIb5, CALC_TIMES) * CURRENT_RADIO;
    Ic5 = CalcAverage(VIc5, CALC_TIMES) * CURRENT_RADIO;
    I05 = CalcAverage(VI05, CALC_TIMES) * CURRENT_RADIO;
    
    UA = CalcAverage(VUA, CALC_TIMES) * VOLTAGE_RADIO;
    UC = CalcAverage(VUC, CALC_TIMES) * VOLTAGE_RADIO;

	for (i = 0; i < ADC_SAMPLE_NUM; i++)
	{
		Udc1 += g_Udc1SampleBuf[i];
		Udc2 += g_Udc2SampleBuf[i];
	}   
	
	Udc1 /= ADC_SAMPLE_NUM;
	Udc2 /= ADC_SAMPLE_NUM;
	
    Ua = Ua * g_ParameterDB.Data.calibrateFactor.Str.califactorUab;
    Ua3 = Ua3 * g_ParameterDB.Data.calibrateFactor.Str.califactorUab;
    Ua5 = Ua5 * g_ParameterDB.Data.calibrateFactor.Str.califactorUab;
    Ub = Ub * g_ParameterDB.Data.calibrateFactor.Str.califactorUac;
    Ub3 = Ub3 * g_ParameterDB.Data.calibrateFactor.Str.califactorUac;
    Ub5 = Ub5 * g_ParameterDB.Data.calibrateFactor.Str.califactorUac;    
    Uc = Uc * g_ParameterDB.Data.calibrateFactor.Str.califactorUbc;
    Uc3 = Uc3 * g_ParameterDB.Data.calibrateFactor.Str.califactorUbc;
    Uc5 = Uc5 * g_ParameterDB.Data.calibrateFactor.Str.califactorUbc;    
    U0 = U0 * g_ParameterDB.Data.calibrateFactor.Str.califactorU0;
    U03 = U03 * g_ParameterDB.Data.calibrateFactor.Str.califactorU0;
    U05 = U05 * g_ParameterDB.Data.calibrateFactor.Str.califactorU0;    
    Ia = Ia * g_ParameterDB.Data.calibrateFactor.Str.califactorIa;
	Ia2 = Ia2 * g_ParameterDB.Data.calibrateFactor.Str.califactorIa;
    Ia3 = Ia3 * g_ParameterDB.Data.calibrateFactor.Str.califactorIa;
    Ia5 = Ia5 * g_ParameterDB.Data.calibrateFactor.Str.califactorIa;    
    Ib = Ib * g_ParameterDB.Data.calibrateFactor.Str.califactorIb;
	Ib2 = Ib2 * g_ParameterDB.Data.calibrateFactor.Str.califactorIb;
    Ib3 = Ib3 * g_ParameterDB.Data.calibrateFactor.Str.califactorIb;
    Ib5 = Ib5 * g_ParameterDB.Data.calibrateFactor.Str.califactorIb;    
    Ic = Ic * g_ParameterDB.Data.calibrateFactor.Str.califactorIc;
	Ic2 = Ic2 * g_ParameterDB.Data.calibrateFactor.Str.califactorIc;
    Ic3 = Ic3 * g_ParameterDB.Data.calibrateFactor.Str.califactorIc;
    Ic5 = Ic5 * g_ParameterDB.Data.calibrateFactor.Str.califactorIc;    
    I0 = I0 * g_ParameterDB.Data.calibrateFactor.Str.califactorI0;
    I03 = I03 * g_ParameterDB.Data.calibrateFactor.Str.califactorI0;
    I05 = I05 * g_ParameterDB.Data.calibrateFactor.Str.califactorI0;    
    UA = UA * g_ParameterDB.Data.calibrateFactor.Str.califactorUAB;
    UC = UC * g_ParameterDB.Data.calibrateFactor.Str.califactorUBC;   
    Udc1 = Udc1 * g_ParameterDB.Data.calibrateFactor.Str.califactorDC;
    Udc2 = Udc2 * g_ParameterDB.Data.calibrateFactor.Str.califactorDC2;  

//    ParameterCheck(ZERODRIFT);

    if (Ua < g_ParameterDB.Data.runPara.Str.zerodriftUab)Ua = 0;
    if (Ub < g_ParameterDB.Data.runPara.Str.zerodriftUac)Ub = 0;
    if (Uc < g_ParameterDB.Data.runPara.Str.zerodriftUbc)Uc = 0;
    if (U0 < g_ParameterDB.Data.runPara.Str.zerodriftU0) U0 = 0;
    if (Ia < g_ParameterDB.Data.runPara.Str.zerodriftIa) Ia = 0;
    if (Ib < g_ParameterDB.Data.runPara.Str.zerodriftIb) Ib = 0;
    if (Ic < g_ParameterDB.Data.runPara.Str.zerodriftIc) Ic = 0;
    if (I0 < g_ParameterDB.Data.runPara.Str.zerodriftI0) I0 = 0;
    if (UA < g_ParameterDB.Data.runPara.Str.zerodriftUAB)UA = 0;
    if (UC < g_ParameterDB.Data.runPara.Str.zerodriftUBC)UC = 0;
    if (Udc1 < g_ParameterDB.Data.runPara.Str.zerodriftDC)Udc1 = 0;
    if (Udc2 < g_ParameterDB.Data.runPara.Str.zerodriftDC2)Udc2 = 0;

    Pa = Ua * Ia * cos(g_Alpha[0] * FACTOR_ANGLE_TO_RAD);
    Pb = 0; //Ub * Ib * cos(g_Alpha[1] * FACTOR_ANGLE_TO_RAD);
    Pc = UC * Ic * cos(g_Alpha[2] * FACTOR_ANGLE_TO_RAD);
    P = (Pa + Pb + Pc) / 1000.0f;

    Qa = Ua * Ia * sin(g_Alpha[0] * FACTOR_ANGLE_TO_RAD);
    Qb = 0; //Ub * Ib * sin(g_Alpha[1] * FACTOR_ANGLE_TO_RAD);
    Qc = UC * Ic * sin(g_Alpha[2] * FACTOR_ANGLE_TO_RAD);
    Q = (Qa + Qb + Qc) / 1000.0f;

    S = sqrt(P * P + Q * Q);
    F = P / S;

    P = fabs(P) < g_ParameterDB.Data.runPara.Str.zerodriftP ? 0 : P;
    Q = fabs(Q) < g_ParameterDB.Data.runPara.Str.zerodriftQ ? 0 : Q;
    S = fabs(S) < g_ParameterDB.Data.runPara.Str.zerodriftS ? 0 : S;

    g_TelemetryDB.Str.Uab = Ua;
    g_TelemetryDB.Str.Ubc = Uc;
    g_TelemetryDB.Str.Uac = Ub;
    g_TelemetryDB.Str.U0 = U0;

    g_TelemetryDB.Str.UAB = UA;
    g_TelemetryDB.Str.UBC = UC;
    
    g_TelemetryDB.Str.Ia = Ia;
    g_TelemetryDB.Str.Ib = Ib;
    g_TelemetryDB.Str.Ic = Ic;
    g_TelemetryDB.Str.I0 = I0;

    g_TelemetryDB.Str.P = P;
    g_TelemetryDB.Str.Q = Q;
    g_TelemetryDB.Str.S = S;
    g_TelemetryDB.Str.pf = F;

	g_TelemetryDB.Str.DCVoltage = Udc1;
	g_TelemetryDB.Str.DCSpare = Udc2;
	
    if((g_TelemetryDB.Str.U0 == 0)||(g_TelemetryDB.Str.I0 == 0))
    {g_TelemetryDB.Str.Alpha0 = 0;}
    else       
    {
//        while(g_Alpha[3]<-180)
//        {g_Alpha[3] += 360;}
//        while(g_Alpha[3]>180)
//        {g_Alpha[3] -= 360;}
        g_TelemetryDB.Str.Alpha0 = g_Alpha[3];
    }

	if((g_TelemetryDB.Str.Uab == 0)||(g_TelemetryDB.Str.UBC == 0))
    {g_TelemetryDB.Str.AlphaU = 0;}
    else       
    {
        if(g_Alpha[4]<-180)
        {g_Alpha[4] += 360;}
        if(g_Alpha[4]>180)
        {g_Alpha[4] -= 360;}
        g_TelemetryDB.Str.AlphaU = g_Alpha[4];
    }
	
    g_TelemetryDB.Str.onceUab = Ua * (g_ParameterDB.Data.runPara.Str.ratioUOneTurn / g_ParameterDB.Data.runPara.Str.ratioUSecondary) / 1000.0f;
    g_TelemetryDB.Str.onceUbc = Uc * (g_ParameterDB.Data.runPara.Str.ratioUOneTurn / g_ParameterDB.Data.runPara.Str.ratioUSecondary) / 1000.0f;
    g_TelemetryDB.Str.onceUac = Ub * (g_ParameterDB.Data.runPara.Str.ratioUOneTurn / g_ParameterDB.Data.runPara.Str.ratioUSecondary) / 1000.0f;
    g_TelemetryDB.Str.onceU0 = U0 * (g_ParameterDB.Data.runPara.Str.ratioU0OneTurn / g_ParameterDB.Data.runPara.Str.ratioU0Secondary) / 1000.0f;
    g_TelemetryDB.Str.onceUBC = UC * (g_ParameterDB.Data.runPara.Str.ratioUOneTurn / g_ParameterDB.Data.runPara.Str.ratioUSecondary) / 1000.0f;
    g_TelemetryDB.Str.onceUAB = UA * (g_ParameterDB.Data.runPara.Str.ratioUOneTurn / g_ParameterDB.Data.runPara.Str.ratioUSecondary) / 1000.0f;
	
    g_TelemetryDB.Str.onceIa = Ia * (g_ParameterDB.Data.runPara.Str.ratioIOneTurn / g_ParameterDB.Data.runPara.Str.ratioISecondary);
    g_TelemetryDB.Str.onceIb = Ib * (g_ParameterDB.Data.runPara.Str.ratioIOneTurn / g_ParameterDB.Data.runPara.Str.ratioISecondary);    
    g_TelemetryDB.Str.onceIc = Ic * (g_ParameterDB.Data.runPara.Str.ratioIOneTurn / g_ParameterDB.Data.runPara.Str.ratioISecondary);
    g_TelemetryDB.Str.onceI0 = I0 * (g_ParameterDB.Data.runPara.Str.ratioI0OneTurn / g_ParameterDB.Data.runPara.Str.ratioI0Secondary);

    g_TelemetryDB.Str.onceP = P * (g_ParameterDB.Data.runPara.Str.ratioUOneTurn / g_ParameterDB.Data.runPara.Str.ratioUSecondary) * (g_ParameterDB.Data.runPara.Str.ratioIOneTurn / g_ParameterDB.Data.runPara.Str.ratioISecondary);
    g_TelemetryDB.Str.onceQ = Q * (g_ParameterDB.Data.runPara.Str.ratioUOneTurn / g_ParameterDB.Data.runPara.Str.ratioUSecondary) * (g_ParameterDB.Data.runPara.Str.ratioIOneTurn / g_ParameterDB.Data.runPara.Str.ratioISecondary);
    g_TelemetryDB.Str.onceS = S * (g_ParameterDB.Data.runPara.Str.ratioUOneTurn / g_ParameterDB.Data.runPara.Str.ratioUSecondary) * (g_ParameterDB.Data.runPara.Str.ratioIOneTurn / g_ParameterDB.Data.runPara.Str.ratioISecondary);

    if (Ua3 < g_ParameterDB.Data.runPara.Str.zerodriftUab)Ua3 = 0;
    if (Ub3 < g_ParameterDB.Data.runPara.Str.zerodriftUac)Ub3 = 0;
    if (Uc3 < g_ParameterDB.Data.runPara.Str.zerodriftUbc)Uc3 = 0;
    if (U03 < g_ParameterDB.Data.runPara.Str.zerodriftU0) U03 = 0;
	
	if (Ia2 < g_ParameterDB.Data.runPara.Str.zerodriftIa) Ia2 = 0;
	if (Ib2 < g_ParameterDB.Data.runPara.Str.zerodriftIb) Ib2 = 0;
	if (Ic2 < g_ParameterDB.Data.runPara.Str.zerodriftIc) Ic2 = 0;
	
    if (Ia3 < g_ParameterDB.Data.runPara.Str.zerodriftIa) Ia3 = 0;
    if (Ib3 < g_ParameterDB.Data.runPara.Str.zerodriftIb) Ib3 = 0;
    if (Ic3 < g_ParameterDB.Data.runPara.Str.zerodriftIc) Ic3 = 0;
    if (I03 < g_ParameterDB.Data.runPara.Str.zerodriftI0) I03 = 0;

    if (Ua5 < g_ParameterDB.Data.runPara.Str.zerodriftUab)Ua5 = 0;
    if (Ub5 < g_ParameterDB.Data.runPara.Str.zerodriftUac)Ub5 = 0;
    if (Uc5 < g_ParameterDB.Data.runPara.Str.zerodriftUbc)Uc5 = 0;
    if (U05 < g_ParameterDB.Data.runPara.Str.zerodriftU0) U05 = 0;
    if (Ia5 < g_ParameterDB.Data.runPara.Str.zerodriftIa) Ia5 = 0;
    if (Ib5 < g_ParameterDB.Data.runPara.Str.zerodriftIb) Ib5 = 0;
    if (Ic5 < g_ParameterDB.Data.runPara.Str.zerodriftIc) Ic5 = 0;
    if (I05 < g_ParameterDB.Data.runPara.Str.zerodriftI0) I05 = 0;

    g_secondHarmonicIa = Ia2;
	g_secondHarmonicIb = Ib2;
	g_secondHarmonicIc = Ic2;
    
	g_TelemetryDB.Str.thirdHarmonicUab = Ua3;    
    g_TelemetryDB.Str.thirdHarmonicUbc = Uc3;
    g_TelemetryDB.Str.thirdHarmonicUac = Ub3;
    g_TelemetryDB.Str.thirdHarmonicU0 = U03;
    g_TelemetryDB.Str.thirdHarmonicIa = Ia3;
    g_TelemetryDB.Str.thirdHarmonicIb = Ib3;
    g_TelemetryDB.Str.thirdHarmonicIc = Ic3;
    g_TelemetryDB.Str.thirdHarmonicI0 = I03;

    g_TelemetryDB.Str.fifthHarmonicUab = Ua5;
    g_TelemetryDB.Str.fifthHarmonicUbc = Uc5;
    g_TelemetryDB.Str.fifthHarmonicUac = Ub5;
    g_TelemetryDB.Str.fifthHarmonicU0 = U05;
    g_TelemetryDB.Str.fifthHarmonicIa = Ia5;
    g_TelemetryDB.Str.fifthHarmonicIb = Ib5;
    g_TelemetryDB.Str.fifthHarmonicIc = Ic5;
    g_TelemetryDB.Str.fifthHarmonicI0 = I05;
    
#if RT_USING_TELEMETRY_SET    
    for (i = 0; i < TELEMETRY_TOTAL_NUM; i++)
    {
        if (g_TelemetrySetEnable.buf[i] == SWITCH_ON)
        {
            g_TelemetryDB.buf[i] = g_TelemetrySetValue.buf[i];
        }
    }
#endif /* RT_USING_TELEMETRY_SET */ 
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @Description: AD7606采样值初始化.
  * @param:  无
  * @return: 无
  */
int calculate_init(void)
{
    arm_cfft_radix2_init_f32(&Scfft, 32, 0, 1); // 初始化Scfft结构体
    	
    return(RT_EOK);     
}
INIT_BOARD_EXPORT(calculate_init);

/**
  * @Description: 计算任务.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void CalcultateTask(void)
{
    CalculateData();

    /* 系数校准 */
    CalibrationFactorCal(CALIBRATE_FACTOR_NUM);
}

/* END OF FILE ---------------------------------------------------------------*/

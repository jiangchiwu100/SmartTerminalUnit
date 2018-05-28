
#ifndef _BREAKER_PROTECT_H_
#define _BREAKER_PROTECT_H_

#include "drv_do.h"
#include "common_data.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

#define BRE_ENTIMERS 0x80000000
#define BRE_TITIMERS 0x7fffffff

#define SWITCH_ON  (1.0f)
#define SWITCH_OFF (0.0f)

#define RESETFLAG  0x80000000 //复位标志
#define MIDDLEFLAG 0x7fff0000 //自身中间标志
#define JUDGRFLAG  0x0000ffff //对外判断标志

enum
{
    BRE_FALSE,                // 
    BRE_TRUE
};

enum
{
    BRE_ATRESIA,            // 所有闭锁信号
    BRE_RESET,                // 复位
    BRE_INRUSH,                // 涌流抑制
    BRE_IACC,                // 后加速
	BRE_INVERSE,                // 反时限
    BRE_OVERCUR,                // 过流
    BRE_OVERCURI0,            // 零序过流
    BRE_SENCONDARYRECLSELOCK,            // 二次重合闸闭锁
    BRE_RECLOSE,                // 重合闸
    BRE_RECLOSEI0,                // 零序重合闸
    BRE_PROMAXNUM
};

typedef struct 
{
    uint16_t addr;             // 地址
    uint8_t *value;            // 值  
}STelesignalStr;

typedef struct 
{
    STelesignalStr switchOpen;             // 开关分位
    STelesignalStr switchClose;            // 开关合位
    STelesignalStr recloseHardStrap;       // 重合闸硬压板
    STelesignalStr functionHardStrap;      // 功能投退硬压板  
    STelesignalStr telecontrol_Pro_Out;    // 遥控保护退出	    
    
    STelesignalStr shortCircuitFault;        // 短路故障   
    STelesignalStr earthingFault;            // 接地故障
    STelesignalStr protectionAct;            // 保护动作  
    STelesignalStr protectionClock;          // 保护闭锁
    STelesignalStr recloseAct;               // 重合动作 
    STelesignalStr recloseClock;             // 重合闭锁
    STelesignalStr overcurrentAccIa;         // A相后加速过流
    STelesignalStr overcurrentAccIb;         // B相后加速过流
    STelesignalStr overcurrentAccIc;         // C相后加速过流    
	STelesignalStr overcurrentInverseIa;         // A相后加速过流
    STelesignalStr overcurrentInverseIb;         // B相后加速过流
    STelesignalStr overcurrentInverseIc;         // C相后加速过流   
    STelesignalStr overcurrentIa[3];           // A相过流保护 
    STelesignalStr overcurrentIb[3];           // B相过流保护 
    STelesignalStr overcurrentIc[3];           // C相过流保护             
    STelesignalStr overcurrentI0[3];           // 零序过流保护 
    STelesignalStr recloseProtection[4];            // 重合闸保护     
    STelesignalStr zeroRecloseProtection[2];   // 零序重合闸保护  
    STelesignalStr clockSecondaryClose;      // 闭锁二次重合闸    
}TelesignalStr;

typedef struct 
{
    float *Ia;                       // A相电流 
    float *Ib;                       // B相电流 
    float *Ic;                       // C相电流 
	float *Ia2;                      // A相二次谐波电流 
    float *Ib2;                      // B相二次谐波电流 
    float *Ic2;                      // C相二次谐波电流 
    float *I0;                       // 零序电流
    float *Uab;                      // 线电压Uab
    float *Ucb;                      // 线电压Ucb
    float *Uac;                      // 线电压Uac
    float *U0;                       // 零序电压                    
}TelemetryStr;

#define SETRESETFLAG 0x7fffffff
#define MANRESETFLAG 0x80000000
typedef struct
{
    struct
    {
        uint32_t *gTime;       //定时器  
        uint32_t flag;
        uint32_t *iACCflag;
		uint32_t *inverseflag;
        uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *recloseflag;
        uint32_t *recloseI0flag;
        uint32_t *secondaryRecloseLockflag;
    }valstr;
    struct
    {
        float *pSwitch;            // 自动复归使能  软压板定值，可以选择“投入”、“退出”
        float *pTime;              // 自动复位时间 时间定值，整定范围：0000~9999ms  
    }parastr;
}RestSts;

//涌流标志保持到分闸
#define INRUSH 0x00000001
#define INRUSH1 0x00010000			//涌流抑制时间到或已发生抑制
typedef struct
{
    struct
    {
        uint32_t *gTime;       //定时器    
        uint32_t flag;
    }valstr;
    struct
    {
        float *pSwitch;            // 涌流抑制投退 软压板定值，可以选择“投入”、“退出”
        float *pTime;              // 涌流抑制时间 时间定值，整定范围：0000~9999ms  
    }parastr;
}InrushSts;

//后加速标志保持到重新合闸
#define IACCTIME     			3000				//后加速保护时间
#define IACCCUR 				0x00000001
#define IACCCURSTA1  			0x00010000    	//发生过流
#define IACCCURSTA2  			0x00020000    	//过流导致分闸
#define IACCCURSTA4  			0x00080000    	//涌流抑制时间已到
typedef struct
{
    struct
    {
		uint32_t *inverseflag;
		uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *recloseflag;
		uint32_t *recloseI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime[2];       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;            // 过流1段投退 “1投入”、“0退出”
        float *pValue;             // 过流1段电流 流定值，整定范围：00.00A~99.99A
        float *pTime;              // 过流1段时间 时间定值，整定范围：00.00s~99.99s  
    }parastr;
}IACCSts;


//反时限标志保持到重新合闸Inverse
#define INVERSECUR 0x00000001
#define INVERSECURSTA1  0x00010000    //发生过流
#define INVERSECURSTA2  0x00020000    //过流导致分闸
typedef struct
{
    struct
    {
		uint32_t *iACCflag;
		uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *inrushflag;
		uint32_t *recloseflag;
		uint32_t *recloseI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;            // 反时限投退 “1投入”、“0退出”
        float *pValue;             // 反时限电流 流定值，整定范围：00.00A~99.99A
        float *pTime;              // 反时限时间 时间定值，整定范围：00.00s~99.99s  
    }parastr;
}InverseSts;

//过流标志保持到重新合闸
#define OVERCUR1 0x00000001
#define OVERCUR2 0x00000002
#define OVERCUR3 0x00000004
#define OVERCURSTA1  0x00010000    //发生过流
#define OVERCURSTA2  0x00020000    //过流导致分闸
typedef struct
{
    struct
    {
        uint32_t *iACCflag;
		uint32_t *inverseflag;
        uint32_t *overcurI0flag;
        uint32_t *inrushflag;
		uint32_t *recloseflag;
		uint32_t *recloseI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime[3];       //定时器  
    }valstr;
    struct
    {
        float *pSwitch[3];            // 过流1段投退 “1投入”、“0退出”
        float *pValue[3];             // 过流1段电流 流定值，整定范围：00.00A~99.99A
        float *pTime[3];              // 过流1段时间 时间定值，整定范围：00.00s~99.99s  
    }parastr;
}OvercurSts;

//零序过流标志保持到重新合闸
#define OVERCURI01 0x00000001
#define OVERCURI02 0x00000002
#define OVERCURI0STA1  0x00010000    //合闸位置发生过流
#define OVERCURI0STA2  0x00020000    //分闸位置允许复位
typedef struct
{
    struct
    {
		uint32_t *iACCflag;
		uint32_t *inverseflag;
        uint32_t *overcurflag;
        uint32_t *inrushflag;
		uint32_t *recloseflag;
		uint32_t *recloseI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime[2];       //定时器  
    }valstr;
    struct
    {
        float *pSwitch[2];            // 过流1段投退 “1投入”、“0退出”
        float *pValue[2];             // 过流1段电流 流定值，整定范围：00.00A~99.99A
        float *pTime[2];              // 过流1段时间 时间定值，整定范围：00.00s~99.99s  
    }parastr;
}OvercurI0Sts;

//第一次重合闸范围内有效
#define SENCONDARYRECLSELOCK 0x00000001
#define SENCONDARYRECLSELOCKSTA1  0x00010000    //合闸计时
#define SENCONDARYRECLSELOCKSTA2  0x00020000    //计时结束
typedef struct
{
    struct
    {
		uint32_t *iACCflag;
		uint32_t *inverseflag;
        uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *recloseflag;
        uint32_t *recloseI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;            // 过流1段投退 “1投入”、“0退出”
        float *pTime;              // 过流1段时间 时间定值，整定范围：00.00s~99.99s  
    }parastr;
}SecondaryRecloseLockSts;

//重合闸标志保持到下次重合闸之前
#define RECLOSE1 0x00000001
#define RECLOSE2 0x00000002
#define RECLOSE3 0x00000004
#define RECLOSE4 0x00000008
#define RECLOSELOCK 0x00000100
#define RECLOSESTA1  0x00010000
typedef struct
{
    struct
    {
        uint32_t *secondaryRecloseLockflag;
		uint32_t *iACCflag;
		uint32_t *inverseflag;
        uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime[2];       //定时器  
        uint8_t time;
    }valstr;
    struct
    {
        float *pTimes;            // 重合闸次数 可以选择：0，1，2，3，4 
        float *pTime[4];          // 重合闸时间 时间定值，整定范围：00.00s~99.99s  
		float *pRechargeTime;	  // 重合闸充电时间
		float *pResetTime;		  // 重合闸复位时间
    }parastr;
}RecloseSts;

//零序重合闸标志保持到下次重合闸之前
#define RECLOSEI01 0x00000001
#define RECLOSEI02 0x00000002
#define RECLOSEI0LOCK 0x00000100
#define RECLOSEI0STA1  0x00010000
typedef struct
{
    struct
    {
        uint32_t *secondaryRecloseLockflag;
		uint32_t *iACCflag;
		uint32_t *inverseflag;
        uint32_t *overcurflag;
        uint32_t *overcurI0flag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime[2];       //定时器  
        uint8_t time;
    }valstr;
    struct
    {
        float *pTimes;            // 重合闸次数 可以选择：0，1，2，3，4 
        float *pTime[2];          // 重合闸时间 时间定值，整定范围：00.00s~99.99s 
		float *pRechargeTime;	  // 重合闸充电时间
		float *pResetTime;	  // 重合闸复位时间
    }parastr;
}RecloseI0Sts;

typedef struct
{
    TelesignalStr yx;
    TelemetryStr yc;
    uint16_t fevent_yc_addr[10];
	int WorkMode;                          // 工作模式
	float *pSwitchType;						  // 开关类型(0-断路器/1-负荷开关)
	float *pBreakWorkMode;                      // 断路器工作模式(0-无/1-常规保护/2-电压时间型/3-电压电流型/4-电流计数型)
	float *pLoadWorkMode;                       // 负荷开关工作模式(0-无/1-电压时间型/2-电压电流型/3-电流计数型/4-分界负荷开关型)
    uint8_t (*opening)(uint16_t addr, uint8_t operateType);
    uint8_t (*closing)(uint16_t addr, uint8_t operateType);
    uint8_t (*outputSoe)(uint16_t addr, uint8_t state);
    uint8_t (*outputFevent)(uint16_t yx_addr, uint16_t *yc_addr, uint16_t yc_num);
}ComProSts;

#endif /* END _BREAKER_PROTECT_H_ */


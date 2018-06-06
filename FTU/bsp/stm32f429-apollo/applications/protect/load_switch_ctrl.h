#ifndef _LOAD_SWITCH_CTRL_H_
#define _LOAD_SWITCH_CTRL_H_

#include "drv_do.h"
#include "common_data.h"

#include "stm32f4xx_hal.h"

#define LOAD_ENTIMERS 0x80000000
#define LOAD_TITIMERS 0x7fffffff

#define SWITCH_ON  (1.0f)
#define SWITCH_OFF (0.0f)

#define RESETFLAG  0x80000000 //复位标志
#define MIDDLEFLAG 0x7fff0000 //自身中间标志
#define JUDGRFLAG  0x0000ffff //对外判断标志

enum
{
    LOAD_FALSE,                // 
    LOAD_TRUE
};

enum
{
    LOAD_RESET,                // 复位
    LOAD_STATEJUDGE,        //状态判断
    LOAD_OVERCURI0,            //零序过流
    LOAD_LOSSTRIP,            // 失电分闸
    LOAD_GETVOLCLOSE,            // 得电合闸
    LOAD_OPENCLOSELOCK,     //合分闸闭锁
    LOAD_OVERVOLU0,     //零序电压保护
    LOAD_FEWVOLLOCK,     //残压闭锁
    LOAD_DOUBLEVOL,      //双侧有压
    LOAD_SINGELELOSSCLOSE //单侧失电延时合闸
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
    STelesignalStr breakContact;           //类型
    STelesignalStr functionHardStrap;      // 功能投退硬压板 
	STelesignalStr FA_HardStrap;           // FA功能投退硬压板 
    STelesignalStr swtichclass;            // 开关类型
    STelesignalStr telecontrol_Pro_Out;    // 遥控保护退出	

    STelesignalStr closingLocked;
    STelesignalStr openingLocked;
    STelesignalStr shortCircuitFault;        // 短路故障   
    STelesignalStr earthingFault;            // 接地故障
    STelesignalStr protectionAct;            // 保护动作  
    STelesignalStr protectionClock;          // 保护闭锁
    STelesignalStr overcurrentIa[3];           // A相过流保护 
    STelesignalStr overcurrentIb[3];           // B相过流保护 
    STelesignalStr overcurrentIc[3];           // C相过流保护 
    STelesignalStr overcurrentI0[2];           // 零序过流保护 
    STelesignalStr lossTrip;                   //失电分闸
    STelesignalStr getClossing;                //得电合闸
    STelesignalStr failAfterClosing;           //合闸到故障
    STelesignalStr earthingFaultTripU0;        //零序电压保护
    STelesignalStr remainClockClossing;        //残压闭锁
    STelesignalStr doubleProhibitClossing;     //双侧有压禁止合闸
    STelesignalStr singleLossClosing;          //单侧失压延时合闸
    STelesignalStr breakingCurrent;        //遮断电流
}TelesignalStr;

typedef struct 
{
    float *Ia;                       // A相电流 
    float *Ib;                       // B相电流 
    float *Ic;                       // C相电流 
    float *I0;                       // 零序电流
    float *Uab;                      // 线电压Uab
    float *Ucb;                      // 线电压Ucb
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
        uint32_t *StateJudgeflag;
        uint32_t *overcurI0flag;
        uint32_t *lossTripflag;
        uint32_t *getVolCloseflag;
        uint32_t *openCloseLockingflag;
        uint32_t *fewVolLockflag;
        uint32_t *doubleVol;
        uint32_t *SingleLossClose;
    }valstr;
    struct
    {
        float *pSwitch;            // 自动复归使能  软压板定值，可以选择“投入”、“退出”
        float *pTime;              // 自动复位时间 时间定值，整定范围：0000~9999ms  
    }parastr;
}RestSts;

//状态标志
#define STATENOVOLTAGE      	0x00000001//无压
#define STATENOCURRENT      	0x00000002//无流
#define STATEUNBLOCKED      	0x00000004//非遮断
#define STATEOVERVOLTAGEU0      0x00000008//零序电压
#define STATEOVERCURNOSW    	0x00000010//过流未投
#define STATEOVERCURI0NOSW  	0x00000020//零序过流未投
#define STATEOVERCUR        	0x00000040//相间过流
#define STATEOVERCURI0      	0x00000080//零序过流
#define STATESINGLEVOLTAGEGET  	0x00000100//单侧有压(得电)
#define STATESINGLEVOLTAGELOSS  0x00000200//单侧有压(失电)
#define STATEDOUBLEVOLTAGE  	0x00000400//双侧有压
#define STATEOVERCURSTA1    	0x00010000//合闸位置发生过流
#define STATEOVERCURSTA2    	0x00020000//分闸位置允许复位
#define STATEOVERCURI0STA1  	0x00040000//合闸位置发生过流
#define STATEOVERCURI0STA2  	0x00080000//分闸位置允许复位
#define STATEOVERCUR1       	0x01000000//相间过流1段
#define STATEOVERCUR2       	0x02000000//相间过流2段
#define STATEOVERCUR3       	0x04000000//相间过流2段
#define STATEOVERCURI01     	0x10000000//零序过流1段
#define STATEOVERCURI02     	0x20000000//零序过流2段
typedef struct
{
    struct
    {
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTimeOvercur[3];       //定时器  
        uint32_t *gTimeOvercurI0[2];       //定时器 
        uint32_t *gTimeConnectSwitchRecognize;       //定时器 
    }valstr;
    struct
    {
        float *pSwitchOvercur[3];            // 过流投退 “1投入”、“0退出”
        float *pValueOvercur[3];             // 过流电流 流定值，整定范围：00.00A~99.99A
        float *pTimeOvercur[3];              // 过流时间 时间定值，整定范围：00.00s~99.99s 
        float *pSwitchOvercurI0[2];            // 零序过流投退 “1投入”、“0退出”
        float *pValueOvercurI0[2];             // 零序过流 流定值，整定范围：00.00A~99.99A
        float *pTimeOvercurI0[2];              // 零序过流时间 时间定值，整定范围：00.00s~99.99s
        float *pValuehavevol;             // 无压 流定值，整定范围：00.00A~99.99v
        float *pValueLossvol;             // 无压 流定值，整定范围：00.00A~99.99v
        float *pValueLosscur;             // 无流 流定值，整定范围：00.00A~99.99A
        float *pSwitchBreakcur;            // 非遮断投退 “1投入”、“0退出”
        float *pValueBreakcur;             // 非遮断电流 流定值，整定范围：00.00A~99.99A
        float *pSwitchVoltageU0;            // 零序电压投退 “1投入”、“0退出”
        float *pValueVoltageU0;            // 零序电压定值，整定范围：00.00A~99.99v
    }parastr;
}StateJudgeSts;

//零序过流标志
#define OVERCURI0STA1  0x00010000    //合闸位置发生过流
#define OVERCURI0STA2  0x00020000    //分闸位置允许复位
typedef struct
{
    struct
    {
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
    }valstr;
}OvercurI0Sts;

//失电分闸
#define LOSSTRIP 0x00000001
#define LOSSTRIPSTA1  0x00010000    //发生失电分闸
#define LOSSTRIPSTA2  0x00020000    //失电分闸后第一次合闸
#define LOSSTRIPSTA3  0x00040000    //达到过流故障判断条件
#define LOSSTRIPSTA4  0x00080000    //一次过流
#define LOSSTRIPSTA5  0x00100000    //一次过流后
typedef struct
{
    struct
    {
		uint32_t *openCloseLockingflag;
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;             // 失电分闸投退 “1投入”、“0退出”
        float *pTime;               // 失压分闸时间
    }parastr;
}LossTripSts;

//得电合闸
#define GETVOLCLOSE      0x00000001       //得电合闸
#define GETVOLCLOSESTA1  0x00010000    //得电合闸
#define GETVOLCLOSESTA2  0x00020000    //得电合闸后第一次分闸
#define GETVOLCLOSESTA3  0x00040000    //得电合闸过程中失压闭锁
typedef struct
{
    struct
    {
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;             // 得电合闸投退 “1投入”、“0退出”
        float *pXTime;              // 得电合闸X时间
    }parastr;
}GetVolCloseSts;

//合分闸闭锁
#define OPENCLOSELOCKINGCLOSELOCK  			0x00000001    //合闸闭锁
#define OVERCURRENTCLOSELOCKINGCLOSELOCK  	0x00000002    //过流合闸闭锁
#define U0CLOSELOCKINGCLOSELOCK  			0x00000004    //过流合闸闭锁
#define OPENCLOSELOCKINGSTA1  				0x00010000    //刚合闸后
#define OPENCLOSELOCKINGSTA2  				0x00020000    //Y时间内故障
#define OPENCLOSELOCKINGSTA3  				0x00040000    //Y时间外
#define OPENCLOSELOCKINGSTA4  				0x00080000    //分闸动作
#define OPENCLOSELOCKINGSTA5  				0x00100000    //分闸闭锁计时
#define OPENCLOSELOCKINGSTA6  				0x00200000    //分闸后复位
//#define OPENCLOSELOCKINGSTA7  			0x00400000    //过流分闸
#define OPENCLOSELOCKINGSTA8  				0x00800000    //手动分闸
//#define OPENCLOSELOCKINGSTA9  			0x01000000    //过流跳闸
#define OPENCLOSELOCKINGSTA10 				0x02000000    //分合闸闭锁复归
//#define OPENCLOSELOCKINGSTA11 			0x04000000    //Y时间内故障（瞬间）
typedef struct
{
    struct
    {
		uint32_t *getVolCloseflag;
		uint32_t *singleLossCloseflag;        
        uint32_t *overcurI0flag;
        uint32_t *lossTripflag;       
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pYTime;              // 故障Y时间
        float *pOpenLockTime;              // 分闸闭锁时间
    }parastr;
}OpenCloseLockingSts;

//残压闭锁
#define FEWVOLLOCK      0x00000001        //
#define FEWVOLLOCKUABSTA1  0x00010000    //初始无压状态
#define FEWVOLLOCKUABSTA2  0x00020000    //有压状态
#define FEWVOLLOCKUABSTA3  0x00040000    //检测到最低残压判断
#define FEWVOLLOCKUABSTA4  0x00080000    //合闸闭锁
#define FEWVOLLOCKUCBSTA1  0x00100000    //初始无压状态
#define FEWVOLLOCKUCBSTA2  0x00200000    //有压状态
#define FEWVOLLOCKUCBSTA3  0x00400000    //检测到最低残压判断
#define FEWVOLLOCKUCBSTA4  0x00800000    //合闸闭锁
typedef struct
{
    struct
    {
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gUabTime;       //定时器  
		uint32_t *gUcbTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;             // 投退 “1投入”、“0退出”
        float *pValue;            // 电压值
        float *pXTime;              // 故障X时间
		float *pXLTime;              // 失压延时时间
    }parastr;
}FewVolLockSts;

//双侧有压
#define DOUBLEVOL      0x00000001    //
#define DOUBLEVOLSTA1  0x00010000   //双侧有压状态
typedef struct
{
    struct
    {
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag; 
    }valstr;
        struct
    {
        float *pSwitch;             // 投退 “1投入”、“0退出”
    }parastr;
}DoubleVolSts;

//单侧失电合闸
#define SINGELELOSSCLOSE      0x00000001
#define SINGELELOSSCLOSETIME  0x00003A98    //双侧有压固有时间15s
#define SINGELELOSSCLOSESTA1  0x00010000    //双侧有压大于固有时间
#define SINGELELOSSCLOSESTA2  0x00020000    //单侧有压
#define SINGELELOSSCLOSESTA3  0x00040000    //单侧有压合闸
#define SINGELELOSSCLOSESTA4  0x00080000    //单侧有压合闸复位
typedef struct
{
    struct
    {
        uint32_t *stateJudgeflag;
        uint32_t *resetflag;
        uint32_t flag;
        uint32_t *gTime;       //定时器  
    }valstr;
    struct
    {
        float *pSwitch;             // 失电分闸投退 “1投入”、“0退出”
        float *pTime;                // 失压分闸时间
    }parastr;
}SingleLossCloseSts;

typedef struct
{
    TelesignalStr yx;
    TelemetryStr yc;
	int WorkMode;                          // 工作模式
	float *pBreakWorkMode;                      // 断路器工作模式(0-无/1-常规保护/2-电压时间型/3-电压电流型/4-电流计数型)
	float *pLoadWorkMode;                       // 负荷开关工作模式(0-无/1-电压时间型/2-电压电流型/3-电流计数型/4-分界负荷开关型)
    float *pConnectSwitchRecognize; //联络开关自动识别（0-手动，1-自动）
    uint8_t (*opening)(uint16_t addr, uint8_t operateType);
    uint8_t (*closing)(uint16_t addr, uint8_t operateType);
    uint8_t (*outputSoe)(uint16_t addr, uint8_t state);
    uint8_t (*outputFevent)(uint16_t yx_addr, uint16_t *yc_addr, uint16_t yc_num);
}ComProSts;

#endif

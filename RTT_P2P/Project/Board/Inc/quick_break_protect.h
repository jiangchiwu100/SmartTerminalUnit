#ifndef _QUICK_BREAK_PROTECT_H
#define _QUICK_BREAK_PROTECT_H

#include "drv_do.h"
#include "common_data.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "breaker_protect.h"
#include "breaker_interface.h"
#include "distribution.h"

extern SwitchProperty curStation;

#define RESETFLAG  0x80000000 
//速断保护
#define OVERCUR1 0x00000001			//此标志位当发生过流时置位，其它保护逻辑若判断置位，将不能进入保护逻辑
#define OVERCURSTA1  0x00010000    	//发生过流
#define OVERCURSTA2  0x00020000    	//过流导致分闸
typedef struct
{
	struct
    {
        uint32_t *overcurI0flag;
        uint32_t flag;
        uint32_t *gTime;                 //定时器 
    }valstr;							
    struct
    {
        float *pSwitch;        			//过流一段投退 “1投入”、“0退出”    
        float *pValue;                  //过流一段电流 流定值 0.00A~99.99A
        float *pTime;              		//过流一段时间 时间定值，整定范围：0.00S~99.99S
    }parastr;							//设定参数结构体
	
}CurQuickBreakSts;

//零序过流保护
#define OVERCURI01 0x00000001
#define OVERCURI02 0x00000002
#define OVERCURI0STA1  0x00010000          //合闸位置发生过流
#define OVERCURI0STA2  0x00020000    	   //分闸位置允许复位
typedef struct
{
    struct
    {
        uint32_t *overcurflag;
        uint32_t flag;
        uint32_t *gTime[2];                //定时器 
    }valstr;
    struct
    {
        float *pSwitch[2];        			//过流一段投退 “1投入”、“0退出”    
        float *pValue[2];                   //过流一段电流 流定值 0.00A~99.99A
        float *pTime[2];              		//过流一段时间 时间定值，整定范围：0.00S~99.99S
    }parastr;
}CurQuickBreakI0Sts;

void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void CurrentValue(CurQuickBreakSts *curquickbreakSts, float Ki, float Ikmax);
void CurQuickBreak_ctrl(ComProSts *comProSts,CurQuickBreakSts *curquickbreakSts);
void CurQuickBreakI0_ctrl(ComProSts *comProSts,CurQuickBreakI0Sts *curquickbreakSts);
void CurProtectCtrlClock(void);

#endif /*_QUICK_BREAK_PROTECT_H*/


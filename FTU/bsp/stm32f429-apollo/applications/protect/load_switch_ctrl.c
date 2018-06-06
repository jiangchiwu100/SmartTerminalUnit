/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      load_switch_ctrl.c
  * @brief:     The switch protect logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.09.05
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */


/* INCLUDES ------------------------------------------------------------------*/
#include "load_switch_ctrl.h"
#include "load_switch_interface.h"


/* PRIVATE VARIABLES --------------------------------------------------------*/
static List s_ListTimers[LOAD_DEVMAXNUM];//定时器链表链表//0x80000000使能位

static ComProSts s_ComProSts[LOAD_DEVMAXNUM];

static RestSts s_Rest[LOAD_DEVMAXNUM];
static StateJudgeSts s_stateJudge[LOAD_DEVMAXNUM];
static OvercurI0Sts s_OvercurI0[LOAD_DEVMAXNUM];
static LossTripSts s_LossTrip[LOAD_DEVMAXNUM];
static GetVolCloseSts s_GetVolClose[LOAD_DEVMAXNUM];
static OpenCloseLockingSts s_OpenCloseLocking[LOAD_DEVMAXNUM];
static FewVolLockSts s_FewVolLock[LOAD_DEVMAXNUM];
static DoubleVolSts s_DoubleVol[LOAD_DEVMAXNUM];
static SingleLossCloseSts s_SingleLossClose[LOAD_DEVMAXNUM];


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @Description: 申请定时器.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void addtimers(uint8_t pdrv,uint32_t **dev)
{
    uint32_t *s_timer;
        
    s_timer = rt_malloc(sizeof(uint32_t));
    
    *dev = s_timer;
    *s_timer = 0;
    
    list_ins_next(&s_ListTimers[pdrv],NULL,s_timer); 
}

/**
  * @Description: 定时器增加.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void add_timers(uint8_t pdrv)
{
    ListElmt *element;
    
    element = s_ListTimers[pdrv].head;
    while(element != NULL)
    {
        if((*((uint32_t *)(element->data)))&LOAD_ENTIMERS)
        {
            (*((uint32_t *)(element->data)))++;
        }
        element = element->next;
    }
}

/**
  * @Description: 生成SOE及变位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void addSOE(ComProSts *comProSts,STelesignalStr *telesignal,uint8_t value)
{
    if(*(telesignal->value) != value)
    {
        comProSts->outputSoe(telesignal->addr,value);
        if(value == ON)
        {
            comProSts->outputFevent(telesignal->addr,NULL,0);
        }
    }
}

/**
  * @Description: 复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void rest_ctrl(uint8_t pdrv,ComProSts *comProSts,RestSts *restSts)
{
    static uint64_t temp[LOAD_DEVMAXNUM];
    uint64_t tempk;

    //自动复归
    if(*(restSts->parastr.pSwitch)==SWITCH_ON)
    {
        tempk = temp[pdrv];
        temp[pdrv] = 0;
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.StateJudgeflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.overcurI0flag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.lossTripflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.getVolCloseflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.openCloseLockingflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.fewVolLockflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.doubleVol)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.SingleLossClose)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|((*(comProSts->yx.closingLocked.value)&ON)||\
                          (*(comProSts->yx.openingLocked.value)&ON)||\
                          (*(comProSts->yx.shortCircuitFault.value)&ON)||\
                          (*(comProSts->yx.earthingFault.value)&ON)||\
                          (*(comProSts->yx.protectionAct.value)&ON)||\
                          (*(comProSts->yx.protectionClock.value)&ON)||\
                          (*(comProSts->yx.lossTrip.value)&ON)||\
                          (*(comProSts->yx.getClossing.value)&ON)||\
                          (*(comProSts->yx.failAfterClosing.value)&ON)||\
                          (*(comProSts->yx.earthingFaultTripU0.value)&ON)||\
                          (*(comProSts->yx.remainClockClossing.value)&ON)||\
                          (*(comProSts->yx.doubleProhibitClossing.value)&ON)||\
                          (*(comProSts->yx.singleLossClosing.value)&ON));

        if(tempk != temp[pdrv] )
        {
            *(restSts->valstr.gTime) = LOAD_ENTIMERS;//启动定时器
        }

        if((*(restSts->valstr.gTime)&LOAD_TITIMERS)>=(uint32_t)(*(restSts->parastr.pTime)*1000))
        {
            restSts->valstr.flag |= SETRESETFLAG;
            *(restSts->valstr.gTime) = 0;
            temp[pdrv] = 0;
        }
    }
    else
    {
        *(restSts->valstr.gTime) = 0;
        temp[pdrv] = 0;
    }

    //手动复归
    if(restSts->valstr.flag&MANRESETFLAG)
    {
        restSts->valstr.flag = SETRESETFLAG;
        *(restSts->valstr.gTime) = 0;
        temp[pdrv] = 0;
    }
}

/**
  * @Description: 状态判断.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void state_judge(ComProSts *comProSts,StateJudgeSts *stateJudegeSts)
{
    uint8_t i;
    const float factor = 0.95;

    //相间过流
    if((*(stateJudegeSts->parastr.pSwitchOvercur[0])==SWITCH_ON)||\
            (*(stateJudegeSts->parastr.pSwitchOvercur[1])==SWITCH_ON)||\
            (*(stateJudegeSts->parastr.pSwitchOvercur[2])==SWITCH_ON))//过流未投
    {
        stateJudegeSts->valstr.flag &= ~STATEOVERCURNOSW;
    }
    else
    {
        stateJudegeSts->valstr.flag |= STATEOVERCURNOSW;
    }

    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
    {
        if(stateJudegeSts->valstr.flag&STATEOVERCURSTA2)//过流后合闸//复位状态量
        {
            stateJudegeSts->valstr.flag &= ~STATEOVERCURSTA2;
            *(stateJudegeSts->valstr.gTimeOvercur[0]) = 0;
            *(stateJudegeSts->valstr.gTimeOvercur[1]) = 0;
            *(stateJudegeSts->valstr.gTimeOvercur[2]) = 0;
            for(i=0; i<3; i++)
            {
                addSOE(comProSts,&comProSts->yx.overcurrentIa[i],OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentIb[i],OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentIc[i],OFF);
                stateJudegeSts->valstr.flag &= ~(STATEOVERCUR1<<i);
            }
            addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
        }

        for(i=0; i<3; i++) //检测过流
        {
            if(((*(stateJudegeSts->parastr.pSwitchOvercur[i])==SWITCH_ON)&&\
                (!(stateJudegeSts->valstr.flag&(STATEOVERCUR1<<i))))&&\
                    ((*(comProSts->yc.Ia)>*(stateJudegeSts->parastr.pValueOvercur[i]))||\
                     (*(comProSts->yc.Ib)>*(stateJudegeSts->parastr.pValueOvercur[i]))||\
                     (*(comProSts->yc.Ic)>*(stateJudegeSts->parastr.pValueOvercur[i]))))//过流
            {

                if(!(*(stateJudegeSts->valstr.gTimeOvercur[i])&LOAD_ENTIMERS))
                {
                    *(stateJudegeSts->valstr.gTimeOvercur[i]) = LOAD_ENTIMERS;//启动定时
                }
                if((*(stateJudegeSts->valstr.gTimeOvercur[i])&LOAD_TITIMERS)>=(uint32_t)(*(stateJudegeSts->parastr.pTimeOvercur[i])*1000))
                {
                    stateJudegeSts->valstr.flag |= (STATEOVERCUR1<<i)|STATEOVERCURSTA1;
                    stateJudegeSts->valstr.flag |= RESETFLAG;
                    if(*(comProSts->yc.Ia)>*(stateJudegeSts->parastr.pValueOvercur[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIa[i],OFF);
                        addSOE(comProSts,&comProSts->yx.overcurrentIa[i],ON);
                    }
                    if(*(comProSts->yc.Ib)>*(stateJudegeSts->parastr.pValueOvercur[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIb[i],OFF);
                        addSOE(comProSts,&comProSts->yx.overcurrentIb[i],ON);
                    }
                    if(*(comProSts->yc.Ic)>*(stateJudegeSts->parastr.pValueOvercur[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIc[i],OFF);
                        addSOE(comProSts,&comProSts->yx.overcurrentIc[i],ON);
                    }
                    addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
                    addSOE(comProSts,&comProSts->yx.shortCircuitFault,ON);
                }
            }
            else//不过流
            {
                *(stateJudegeSts->valstr.gTimeOvercur[i]) = 0;
                if(((*(stateJudegeSts->parastr.pSwitchOvercur[i])==SWITCH_ON)&&\
                        (stateJudegeSts->valstr.flag&(STATEOVERCUR1<<i)))&&\
                        ((*(comProSts->yc.Ia)<*(stateJudegeSts->parastr.pValueOvercur[i])*factor)&&\
                         (*(comProSts->yc.Ib)<*(stateJudegeSts->parastr.pValueOvercur[i])*factor)&&\
                         (*(comProSts->yc.Ic)<*(stateJudegeSts->parastr.pValueOvercur[i])*factor)))
                {
                    stateJudegeSts->valstr.flag &= ~(STATEOVERCUR1<<i);
                }
            }
        }
    }
    else if(stateJudegeSts->valstr.flag&STATEOVERCURSTA1)//分位//过流后分闸
    {
        stateJudegeSts->valstr.flag &= (~STATEOVERCURSTA1);
        stateJudegeSts->valstr.flag |= STATEOVERCURSTA2;
    }
    
    if(stateJudegeSts->valstr.flag&(STATEOVERCUR1|STATEOVERCUR2|STATEOVERCUR3))
    {
        stateJudegeSts->valstr.flag |= STATEOVERCUR;
    }
    else
    {
        stateJudegeSts->valstr.flag &= ~STATEOVERCUR;
    }

    //零序过流
    if(((*(stateJudegeSts->parastr.pSwitchOvercurI0[0])==SWITCH_ON)||(*(stateJudegeSts->parastr.pSwitchOvercurI0[0])==SWITCH_ALARM))||\
            ((*(stateJudegeSts->parastr.pSwitchOvercurI0[1])==SWITCH_ON)||(*(stateJudegeSts->parastr.pSwitchOvercurI0[1])==SWITCH_ALARM)))//零序过流未投
    {
        stateJudegeSts->valstr.flag &= ~STATEOVERCURI0NOSW;
    }
    else
    {
        stateJudegeSts->valstr.flag |= STATEOVERCURI0NOSW;
    }

    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位//无涌流抑制
    {
        if(stateJudegeSts->valstr.flag&STATEOVERCURI0STA2)//过流后合闸//复位状态量
        {
            stateJudegeSts->valstr.flag &= ~STATEOVERCURI0STA2;
            *(stateJudegeSts->valstr.gTimeOvercurI0[0]) = 0;
            *(stateJudegeSts->valstr.gTimeOvercurI0[1]) = 0;
            for(i=0; i<2; i++)
            {
                addSOE(comProSts,&comProSts->yx.overcurrentI0[i],OFF);
            }
            addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
        }

        for(i=0; i<2; i++) //检测过流
        {
            if((((*(stateJudegeSts->parastr.pSwitchOvercurI0[i])==SWITCH_ON)||(*(stateJudegeSts->parastr.pSwitchOvercurI0[i])==SWITCH_ALARM))&&\
                (!(stateJudegeSts->valstr.flag&(STATEOVERCURI01<<i))))&&(*(comProSts->yc.I0)>*(stateJudegeSts->parastr.pValueOvercurI0[i])))//过流
            {
                if(!(*(stateJudegeSts->valstr.gTimeOvercurI0[i])&LOAD_ENTIMERS))
                {
                    *(stateJudegeSts->valstr.gTimeOvercurI0[i]) = LOAD_ENTIMERS;//启动定时
                }
                if((*(stateJudegeSts->valstr.gTimeOvercurI0[i])&LOAD_TITIMERS)>=(uint32_t)(*(stateJudegeSts->parastr.pTimeOvercurI0[i])*1000))
                {
                    stateJudegeSts->valstr.flag |= (STATEOVERCURI01<<i)|STATEOVERCURI0STA1;
                    stateJudegeSts->valstr.flag |= RESETFLAG;
                    if(*(comProSts->yc.I0)>*(stateJudegeSts->parastr.pValueOvercurI0[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentI0[i],OFF);
                        addSOE(comProSts,&comProSts->yx.overcurrentI0[i],ON);
                    }
                    addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
                    addSOE(comProSts,&comProSts->yx.earthingFault,ON);
                }
            }
            else
            {
                *(stateJudegeSts->valstr.gTimeOvercurI0[i]) = 0;
                if(((*(stateJudegeSts->parastr.pSwitchOvercurI0[i])==SWITCH_ON)&&(stateJudegeSts->valstr.flag&STATEOVERCURI0))&&\
                        (*(comProSts->yc.I0)<*(stateJudegeSts->parastr.pValueOvercurI0[i])*factor))//过流
                {
                    stateJudegeSts->valstr.flag &= ~(STATEOVERCURI01<<i);
                }
            }
        }
    }
    else if(stateJudegeSts->valstr.flag&STATEOVERCURI0STA1)//分闸//过流后分闸
    {
        stateJudegeSts->valstr.flag &= (~STATEOVERCURI0STA1);
        stateJudegeSts->valstr.flag |= STATEOVERCURI0STA2;
    }

    if(stateJudegeSts->valstr.flag&(STATEOVERCURI01|STATEOVERCURI02))
    {
        stateJudegeSts->valstr.flag |= STATEOVERCURI0;
    }
    else
    {
        stateJudegeSts->valstr.flag &= ~STATEOVERCURI0;
    }
    
    //相间非遮断
    if(*(stateJudegeSts->parastr.pSwitchBreakcur)==SWITCH_ON)
    {
        if((*(comProSts->yc.Ia)>*(stateJudegeSts->parastr.pValueBreakcur))||\
                (*(comProSts->yc.Ib)>*(stateJudegeSts->parastr.pValueBreakcur))||\
                (*(comProSts->yc.Ic)>*(stateJudegeSts->parastr.pValueBreakcur))||\
                (*(comProSts->yc.I0)>*(stateJudegeSts->parastr.pValueBreakcur)))
        {
            stateJudegeSts->valstr.flag |= STATEUNBLOCKED;
            addSOE(comProSts,&comProSts->yx.breakingCurrent,ON);
        }
        else if((*(comProSts->yc.Ia)<*(stateJudegeSts->parastr.pValueBreakcur)*factor)&&\
                (*(comProSts->yc.Ib)<*(stateJudegeSts->parastr.pValueBreakcur)*factor)&&\
                (*(comProSts->yc.Ic)<*(stateJudegeSts->parastr.pValueBreakcur)*factor)&&\
                (*(comProSts->yc.I0)<*(stateJudegeSts->parastr.pValueBreakcur)*factor))
        {
            stateJudegeSts->valstr.flag &= ~STATEUNBLOCKED;
            addSOE(comProSts,&comProSts->yx.breakingCurrent,OFF);
        }
    }
    else
    {
        stateJudegeSts->valstr.flag &= ~STATEUNBLOCKED;
        addSOE(comProSts,&comProSts->yx.breakingCurrent,OFF);
    }

    //零序电压保护
    if(*(stateJudegeSts->parastr.pSwitchVoltageU0)==SWITCH_ON)
    {
        if(*(comProSts->yc.U0)>*(stateJudegeSts->parastr.pValueVoltageU0))
        {
            stateJudegeSts->valstr.flag |= STATEOVERVOLTAGEU0;
            addSOE(comProSts,&comProSts->yx.earthingFaultTripU0,ON);
        }
        else if(*(comProSts->yc.U0)<*(stateJudegeSts->parastr.pValueVoltageU0)*factor)
        {
            stateJudegeSts->valstr.flag &= ~STATEOVERVOLTAGEU0;
            addSOE(comProSts,&comProSts->yx.earthingFaultTripU0,OFF);
        }
    }
    else
    {
        stateJudegeSts->valstr.flag &= ~STATEOVERVOLTAGEU0;
        addSOE(comProSts,&comProSts->yx.earthingFaultTripU0,OFF);
    }
    
    //无压
    if((*(comProSts->yc.Uab)<=*(stateJudegeSts->parastr.pValueLossvol))&&\
            (*(comProSts->yc.Ucb)<=*(stateJudegeSts->parastr.pValueLossvol)))
    {
        stateJudegeSts->valstr.flag |= STATENOVOLTAGE;
        stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEGET;
		stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGELOSS;
        stateJudegeSts->valstr.flag &= ~STATEDOUBLEVOLTAGE;
    }
    else
    {
        if((*(comProSts->yc.Uab)>*(stateJudegeSts->parastr.pValuehavevol))&&\
                (*(comProSts->yc.Ucb)>*(stateJudegeSts->parastr.pValuehavevol)))//双侧有压
        {
            stateJudegeSts->valstr.flag |= STATEDOUBLEVOLTAGE;
            stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEGET;
			stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGELOSS;
            stateJudegeSts->valstr.flag &= ~STATENOVOLTAGE;
        }
        else if(((*(comProSts->yc.Uab)>*(stateJudegeSts->parastr.pValuehavevol))&&(*(comProSts->yc.Ucb)<=*(stateJudegeSts->parastr.pValueLossvol)))||\
                ((*(comProSts->yc.Ucb)>*(stateJudegeSts->parastr.pValuehavevol))&&(*(comProSts->yc.Uab)<=*(stateJudegeSts->parastr.pValueLossvol))))//单侧有压
        {
			if(stateJudegeSts->valstr.flag & STATEDOUBLEVOLTAGE)
			{
				stateJudegeSts->valstr.flag |= STATESINGLEVOLTAGELOSS;
				stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGEGET;
			}
			else if(stateJudegeSts->valstr.flag & STATENOVOLTAGE)
			{				
				stateJudegeSts->valstr.flag |= STATESINGLEVOLTAGEGET;
				stateJudegeSts->valstr.flag &= ~STATESINGLEVOLTAGELOSS;
			}
            stateJudegeSts->valstr.flag &= ~STATEDOUBLEVOLTAGE;
            stateJudegeSts->valstr.flag &= ~STATENOVOLTAGE;
        }
    }

    //无流
    if((*(comProSts->yc.Ia)<=*(stateJudegeSts->parastr.pValueLosscur)*factor)&&\
            (*(comProSts->yc.Ib)<=*(stateJudegeSts->parastr.pValueLosscur)*factor)&&\
            (*(comProSts->yc.Ic)<=*(stateJudegeSts->parastr.pValueLosscur)*factor))
    {
        stateJudegeSts->valstr.flag |= STATENOCURRENT;
    }
    else if((*(comProSts->yc.Ia)>=*(stateJudegeSts->parastr.pValueLosscur))||\
            (*(comProSts->yc.Ib)>=*(stateJudegeSts->parastr.pValueLosscur))||\
            (*(comProSts->yc.Ic)>=*(stateJudegeSts->parastr.pValueLosscur)))
    {
        stateJudegeSts->valstr.flag &= ~STATENOCURRENT;
    }
    
    //自动判别分段，联络
    if((*(comProSts->pConnectSwitchRecognize) == SWITCH_ON)&&(*(comProSts->yx.breakContact.value) == OFF))
    {
        if((stateJudegeSts->valstr.flag&STATEDOUBLEVOLTAGE)&&\
            ((*(comProSts->yx.switchClose.value) == OFF)&&(*(comProSts->yx.switchOpen.value) == ON)))//双侧有压,开关分位
        {
            if(!(*(stateJudegeSts->valstr.gTimeConnectSwitchRecognize)&LOAD_ENTIMERS))
            {
                *(stateJudegeSts->valstr.gTimeConnectSwitchRecognize) = LOAD_ENTIMERS;//启动定时
            }
            if((*(stateJudegeSts->valstr.gTimeConnectSwitchRecognize)&LOAD_TITIMERS)>=(uint32_t)(15*1000))
            {
                addSOE(comProSts,&comProSts->yx.breakContact,ON);
                *(stateJudegeSts->valstr.gTimeConnectSwitchRecognize) = 0;
            }
        }  
    }
}

/**
* @Description: 状态判断复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void state_judge_rest(ComProSts *comProSts,StateJudgeSts *stateJudegeSts)
{
    uint8_t i;

    if(*(stateJudegeSts->valstr.resetflag)&(1<<LOAD_STATEJUDGE))//复位
    {
        *(stateJudegeSts->valstr.resetflag) &= ~(1<<LOAD_STATEJUDGE);
        stateJudegeSts->valstr.flag = (STATENOVOLTAGE|STATENOCURRENT);
        *(stateJudegeSts->valstr.gTimeOvercur[0]) = 0;
        *(stateJudegeSts->valstr.gTimeOvercur[1]) = 0;
        *(stateJudegeSts->valstr.gTimeOvercur[2]) = 0;
        for(i=0; i<3; i++)
        {
            addSOE(comProSts,&comProSts->yx.overcurrentIa[i],OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentIb[i],OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentIc[i],OFF);
        }
        addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
        *(stateJudegeSts->valstr.gTimeOvercurI0[0]) = 0;
        *(stateJudegeSts->valstr.gTimeOvercurI0[1]) = 0;
        for(i=0; i<2; i++)
        {
            addSOE(comProSts,&comProSts->yx.overcurrentI0[i],OFF);
        }
        addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
    }
}

/**
  * @Description: 零序过流.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void overcurI0_ctrl(ComProSts *comProSts,OvercurI0Sts *overcurI0Sts)
{
    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
    {
        if(overcurI0Sts->valstr.flag&OVERCURI0STA2)//过流后合闸//复位状态量
        {
            overcurI0Sts->valstr.flag = 0;
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        }
        if((*(overcurI0Sts->valstr.stateJudgeflag)&STATEOVERCURI0)&&\
                (!(overcurI0Sts->valstr.flag&OVERCURI0STA1))) //零序过流
        {
            if(!(*(overcurI0Sts->valstr.stateJudgeflag)&STATEUNBLOCKED)&&\
                    (*(comProSts->yx.openingLocked.value) == OFF))//非遮断
            {
                comProSts->opening(ADDR_LOGIC_ACT,DO_OPEN);
                addSOE(comProSts,&comProSts->yx.protectionAct,ON);
                overcurI0Sts->valstr.flag |= OVERCURI0STA1|RESETFLAG;
            }
        }
    }
    else if(overcurI0Sts->valstr.flag&OVERCURI0STA1)//分闸//过流后分闸
    {
        overcurI0Sts->valstr.flag &= (~OVERCURI0STA1);
        overcurI0Sts->valstr.flag |= OVERCURI0STA2;
    }
}

/**
  * @Description: 零序过流复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void overcurI0_rest(ComProSts *comProSts,OvercurI0Sts *overcurI0Sts)
{
    if(*(overcurI0Sts->valstr.resetflag)&(1<<LOAD_OVERCURI0))//复位
    {
        *(overcurI0Sts->valstr.resetflag) &= ~(1<<LOAD_OVERCURI0);
        overcurI0Sts->valstr.flag = 0;
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
    }
}

/**
  * @Descr b iption: 失压跳闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void lossTrip_ctrl(ComProSts *comProSts,LossTripSts *lossTripSts)
{
    if((*(comProSts->yx.switchClose.value)==ON)&&(*(comProSts->yx.switchOpen.value)==OFF))//合位
    {
        if(lossTripSts->valstr.flag&LOSSTRIPSTA2)//动作后合闸//复位状态量
        {
            lossTripSts->valstr.flag &= (~LOSSTRIPSTA2);
			lossTripSts->valstr.flag &= (~LOSSTRIP);
            *(lossTripSts->valstr.gTime) = 0;
            addSOE(comProSts,&comProSts->yx.lossTrip,OFF);
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
        }

        if((*(lossTripSts->parastr.pSwitch)==SWITCH_ON)&&(!(lossTripSts->valstr.flag&LOSSTRIPSTA1)))
        {
            if((((*(lossTripSts->valstr.stateJudgeflag)&STATEOVERCUR)||(*(lossTripSts->valstr.stateJudgeflag)&STATEOVERCURI0))&&\
                    (!(lossTripSts->valstr.flag&LOSSTRIPSTA3))))//过流后
            {
                if((comProSts->WorkMode == TYPE_BREAKER_CURCOUNT)||((comProSts->WorkMode == TYPE_LOADSWTICH_CURCOUNT)))//电流计数型
                {
                    if(!(lossTripSts->valstr.flag&LOSSTRIPSTA4))//第一次过流
                    {
                        lossTripSts->valstr.flag |= LOSSTRIPSTA4;
                    }
                    if(lossTripSts->valstr.flag&LOSSTRIPSTA5)//第二次过流
                    {
                        lossTripSts->valstr.flag |= LOSSTRIPSTA3;
                    }
                }
                if(comProSts->WorkMode == TYPE_LOADSWTICH_DIVIDE)//负荷开关型
                {
                    lossTripSts->valstr.flag |= LOSSTRIPSTA3;
                }
            }

            //无流计数
            if((((!(*(lossTripSts->valstr.stateJudgeflag)&STATEOVERCUR))&&(!(*(lossTripSts->valstr.stateJudgeflag)&STATEOVERCURI0)))||\
                    ((*(lossTripSts->valstr.stateJudgeflag)&STATEOVERCURNOSW)&&(*(lossTripSts->valstr.stateJudgeflag)&STATEOVERCURI0NOSW)))&&\
                    (lossTripSts->valstr.flag&LOSSTRIPSTA4)&&(!(lossTripSts->valstr.flag&LOSSTRIPSTA5)))//第一次过流后
            {
                lossTripSts->valstr.flag |= LOSSTRIPSTA5;
            }

            //无压无流//电流计数型需判断2次电流
            if((*(lossTripSts->valstr.stateJudgeflag)&STATENOVOLTAGE)&&(*(lossTripSts->valstr.stateJudgeflag)&STATENOCURRENT)&&(*(comProSts->yx.openingLocked.value)==OFF)&&\
                (((comProSts->WorkMode == TYPE_BREAKER_VOLTIME)||(comProSts->WorkMode == TYPE_LOADSWTICH_VOLTIME)||(comProSts->WorkMode == TYPE_BREAKER_VOLCUR)||(comProSts->WorkMode == TYPE_LOADSWTICH_VOLCUR))||\
                (((comProSts->WorkMode == TYPE_BREAKER_CURCOUNT)||(comProSts->WorkMode == TYPE_LOADSWTICH_CURCOUNT)||(comProSts->WorkMode == TYPE_LOADSWTICH_DIVIDE))&&(lossTripSts->valstr.flag&LOSSTRIPSTA3))))//无压无流
            {
                if(!(*(lossTripSts->valstr.gTime)&LOAD_ENTIMERS))
                {
                    *(lossTripSts->valstr.gTime) = LOAD_ENTIMERS;//启动定时
                }
                if((*(lossTripSts->valstr.gTime)&LOAD_TITIMERS)>=(uint32_t)(*(lossTripSts->parastr.pTime)*1000))
                {
                    comProSts->opening(ADDR_LOGIC_ACT,DO_OPEN);
                    lossTripSts->valstr.flag |= LOSSTRIP|LOSSTRIPSTA1|RESETFLAG;
                    addSOE(comProSts,&comProSts->yx.lossTrip,ON);
                    if((lossTripSts->valstr.flag&LOSSTRIPSTA3)||(*(lossTripSts->valstr.openCloseLockingflag)&JUDGRFLAG))
                    {
                        addSOE(comProSts,&comProSts->yx.closingLocked,ON);
                    }
                }
            }
            else
            {
                *(lossTripSts->valstr.gTime) = 0;
            }
        }
        else
        {
            *(lossTripSts->valstr.gTime) = 0;
            lossTripSts->valstr.flag &= ~LOSSTRIPSTA3;
        }
    }
    else if(lossTripSts->valstr.flag&(LOSSTRIPSTA1|LOSSTRIPSTA4|LOSSTRIPSTA3|LOSSTRIPSTA5))//分位//过流后
    {
        lossTripSts->valstr.flag &= (~LOSSTRIPSTA1);
        lossTripSts->valstr.flag |= LOSSTRIPSTA2;
    }
    else
    {
        *(lossTripSts->valstr.gTime) = 0;
    }
}

/**
  * @Descr b iption: 失压跳闸复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void lossTrip_rest(ComProSts *comProSts,LossTripSts *lossTripSts)
{
    if(*(lossTripSts->valstr.resetflag)&(1<<LOAD_LOSSTRIP))//复位
    {
        *(lossTripSts->valstr.resetflag) &= ~(1<<LOAD_LOSSTRIP);
        lossTripSts->valstr.flag = 0;
        *(lossTripSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.lossTrip,OFF);
    }
}

/**
  * @Description: 得电合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void getVolClose_ctrl(ComProSts *comProSts,GetVolCloseSts *getVolCloseSts)
{
    if((*(comProSts->yx.switchOpen.value)==ON)&&(*(comProSts->yx.switchClose.value)==OFF))//分位
    {
        if(getVolCloseSts->valstr.flag&GETVOLCLOSESTA2)//动作后合闸//复位状态量
        {
            getVolCloseSts->valstr.flag = 0;
            *(getVolCloseSts->valstr.gTime) = 0;
            addSOE(comProSts,&comProSts->yx.getClossing,OFF);
        }

        if((*(getVolCloseSts->parastr.pSwitch)==SWITCH_ON)&&(*(comProSts->yx.closingLocked.value)==OFF)&&\
                (*(getVolCloseSts->valstr.stateJudgeflag)&STATESINGLEVOLTAGEGET)&&\
                (!(getVolCloseSts->valstr.flag&GETVOLCLOSESTA1)))//单侧有压
        {
            if(!(*(getVolCloseSts->valstr.gTime)&LOAD_ENTIMERS))
            {
                *(getVolCloseSts->valstr.gTime) = LOAD_ENTIMERS;//启动定时
            }
            if((*(getVolCloseSts->valstr.gTime)&LOAD_TITIMERS)>=(uint32_t)(*(getVolCloseSts->parastr.pXTime))*1000)
            {
                comProSts->closing(ADDR_LOGIC_ACT,DO_CLOSE);
                getVolCloseSts->valstr.flag |= GETVOLCLOSE|GETVOLCLOSESTA1|RESETFLAG;
                addSOE(comProSts,&comProSts->yx.getClossing,ON);
            } 
			 			 
        }
        else
        {
		  #ifdef GETPOWERLOSSVOLTAGELOCKCLOSE
			 if((*(getVolCloseSts->valstr.gTime)&LOAD_ENTIMERS) && (*(getVolCloseSts->valstr.stateJudgeflag)&STATENOVOLTAGE))
			 {
				 addSOE(comProSts,&comProSts->yx.closingLocked,ON);
				 getVolCloseSts->valstr.flag |= GETVOLCLOSESTA3;
			 }
		  #endif 			 
            *(getVolCloseSts->valstr.gTime) = 0;
        }  				        
    }
    else if(getVolCloseSts->valstr.flag&GETVOLCLOSESTA1)//分位//过流后
    {
        getVolCloseSts->valstr.flag &= (~GETVOLCLOSESTA1);
        getVolCloseSts->valstr.flag |= GETVOLCLOSESTA2;
    }
	
  #ifdef GETPOWERLOSSVOLTAGELOCKCLOSE
	if((*(comProSts->yx.switchOpen.value)==OFF)&&(*(comProSts->yx.switchClose.value)==ON))//合位
	{
		if(getVolCloseSts->valstr.flag&GETVOLCLOSESTA3)
		{
			addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
			getVolCloseSts->valstr.flag &= (~GETVOLCLOSESTA3);
		}
	}
  #endif
}

/**
  * @Description: 得电合闸复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void getVolClose_rest(ComProSts *comProSts,GetVolCloseSts *getVolCloseSts)
{
    if(*(getVolCloseSts->valstr.resetflag)&(1<<LOAD_GETVOLCLOSE))//复位
    {
        *(getVolCloseSts->valstr.resetflag) &= ~(1<<LOAD_GETVOLCLOSE);
        getVolCloseSts->valstr.flag = 0;
        *(getVolCloseSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.getClossing,OFF);
	  #ifdef GETPOWERLOSSVOLTAGELOCKCLOSE
		addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
	  #endif
    }
}

/**
  * @Description: 分合闸闭锁.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void openCloseLocking_ctrl(ComProSts *comProSts,OpenCloseLockingSts *openCloseLockingSts)
{
    if((*(comProSts->yx.switchOpen.value)==ON)&&(*(comProSts->yx.switchClose.value)==OFF))//分位
    {
		if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA10)//由合到分
		{
			addSOE(comProSts,&comProSts->yx.openingLocked,OFF);
            *(openCloseLockingSts->valstr.gTime) = 0;
			openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA10;
			openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA2;
		}
		
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA8)//手动分闸
        {
            if(!((*(openCloseLockingSts->valstr.lossTripflag)&LOSSTRIP)||\
                (*(comProSts->yx.protectionAct.value)==ON)))//手动分闸或摇杆分闸闭锁合闸
            {
                addSOE(comProSts,&comProSts->yx.closingLocked,ON);
                openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA8;
            }
        }
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA1)//刚合闸标志
        {
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA1;
        }
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA5)//分闸闭锁计时标志
        {
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA5;
            addSOE(comProSts,&comProSts->yx.openingLocked,OFF);
        }
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA4)//分闸标志
        {
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA4;
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGCLOSELOCK;
			openCloseLockingSts->valstr.flag &= ~OVERCURRENTCLOSELOCKINGCLOSELOCK;
			openCloseLockingSts->valstr.flag &= ~U0CLOSELOCKINGCLOSELOCK;
            openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA6;
        }
    }
    else if((*(comProSts->yx.switchOpen.value)==OFF)&&(*(comProSts->yx.switchClose.value)==ON))//合位
    {
		if(!(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA10))//由分到合
		{
			addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
			openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA10;
		}
		
        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA6)//分闸后复位标志
        {
            openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA6;
            addSOE(comProSts,&comProSts->yx.failAfterClosing,OFF);
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        }
        
        openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA8;

        if(!(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA1))//刚合闸后
        {
            if(!(*(openCloseLockingSts->valstr.gTime)&LOAD_ENTIMERS))
            {
                *(openCloseLockingSts->valstr.gTime) = LOAD_ENTIMERS;//启动定时
            }
            if((*(openCloseLockingSts->valstr.gTime)&LOAD_TITIMERS)<(uint32_t)(*(openCloseLockingSts->parastr.pYTime))*1000)
            {
                //Y时间内 
				//电压时间型
				if((comProSts->WorkMode == TYPE_BREAKER_VOLTIME)||(comProSts->WorkMode == TYPE_LOADSWTICH_VOLTIME))
				{
					if(*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERVOLTAGEU0)//零序过压
					{
						if(!(*(openCloseLockingSts->valstr.stateJudgeflag)&STATEUNBLOCKED))	//非遮断
						{
							openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA1;
							*(openCloseLockingSts->valstr.gTime) = 0;
							comProSts->opening(ADDR_LOGIC_ACT,DO_OPEN);
							addSOE(comProSts,&comProSts->yx.protectionAct,ON);
							addSOE(comProSts,&comProSts->yx.closingLocked,ON);
						}
						openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA2;
						openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA4;
						openCloseLockingSts->valstr.flag |= RESETFLAG;
						addSOE(comProSts,&comProSts->yx.failAfterClosing,ON);
						if((*(openCloseLockingSts->valstr.getVolCloseflag)&JUDGRFLAG)||\
                          (*(openCloseLockingSts->valstr.singleLossCloseflag)&JUDGRFLAG))//得电合闸或失压合闸闭锁
                        {
                            openCloseLockingSts->valstr.flag |= U0CLOSELOCKINGCLOSELOCK;
                        }
					}
					if(*(openCloseLockingSts->valstr.stateJudgeflag)&STATENOVOLTAGE)//无压
                    {
						openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA1;
                        openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA4;
                        openCloseLockingSts->valstr.flag |= RESETFLAG;
						addSOE(comProSts,&comProSts->yx.failAfterClosing,ON);
                        if((*(openCloseLockingSts->valstr.getVolCloseflag)&JUDGRFLAG)||\
                          (*(openCloseLockingSts->valstr.singleLossCloseflag)&JUDGRFLAG))//得电合闸或失压合闸闭锁
                        {
                            openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGCLOSELOCK;
                        }
						*(openCloseLockingSts->valstr.gTime) = 0;
                    }
				}
				if((comProSts->WorkMode == TYPE_BREAKER_VOLCUR)||(comProSts->WorkMode == TYPE_LOADSWTICH_VOLCUR))
				{
					//断路器电压电流型  零序过压、过流相序过流
					if((comProSts->WorkMode == TYPE_BREAKER_VOLCUR)&&((*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERVOLTAGEU0)||\
					  (*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCUR)||(*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCURI0)))
					{
						openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA1;
						openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA4;
						openCloseLockingSts->valstr.flag |= RESETFLAG;
						comProSts->opening(ADDR_LOGIC_ACT,DO_OPEN);
						addSOE(comProSts,&comProSts->yx.protectionAct,ON);
						addSOE(comProSts,&comProSts->yx.closingLocked,ON);
						*(openCloseLockingSts->valstr.gTime) = 0;
					}
					//负荷开关电压电流型  
					if(comProSts->WorkMode == TYPE_LOADSWTICH_VOLCUR)
					{
						if(*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERVOLTAGEU0)//零序过压
						{
							if(!(*(openCloseLockingSts->valstr.stateJudgeflag)&STATEUNBLOCKED))	//非遮断
							{
								openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA1;
								*(openCloseLockingSts->valstr.gTime) = 0;
								comProSts->opening(ADDR_LOGIC_ACT,DO_OPEN);
								addSOE(comProSts,&comProSts->yx.protectionAct,ON);
								addSOE(comProSts,&comProSts->yx.closingLocked,ON);
							}
							openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA2;
							openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA4;
							openCloseLockingSts->valstr.flag |= RESETFLAG;
							addSOE(comProSts,&comProSts->yx.failAfterClosing,ON);
							if((*(openCloseLockingSts->valstr.getVolCloseflag)&JUDGRFLAG)||\
							  (*(openCloseLockingSts->valstr.singleLossCloseflag)&JUDGRFLAG))//得电合闸或失压合闸闭锁
							{
								openCloseLockingSts->valstr.flag |= U0CLOSELOCKINGCLOSELOCK;
							}
						}
						//零序过压、过流相序过流
						if(*(openCloseLockingSts->valstr.stateJudgeflag)&STATENOVOLTAGE)
						{
							if(((*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERVOLTAGEU0)||\
							  (*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCUR)||\
							  (*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCURI0)))
							{
								openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA1;
								*(openCloseLockingSts->valstr.gTime) = 0;
								addSOE(comProSts,&comProSts->yx.failAfterClosing,ON);
								if((*(openCloseLockingSts->valstr.getVolCloseflag)&JUDGRFLAG)||\
								  (*(openCloseLockingSts->valstr.singleLossCloseflag)&JUDGRFLAG))//得电合闸或失压合闸闭锁
								{
									openCloseLockingSts->valstr.flag |= OVERCURRENTCLOSELOCKINGCLOSELOCK;
								}
							}
							openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA2;
							openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA4;
							openCloseLockingSts->valstr.flag |= RESETFLAG;
						}	
					}
				}
				
				//分界负荷开关
				if(comProSts->WorkMode == TYPE_LOADSWTICH_DIVIDE)
				{}
				
            }
            else                //Y时间外
            {
				if(!(openCloseLockingSts->valstr.flag & OPENCLOSELOCKINGSTA2))
				{
					openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA3;
				}
                openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA1;
                *(openCloseLockingSts->valstr.gTime) = 0;
            }
        }
		if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA3)//Y时间外
		{
			openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA3;
			openCloseLockingSts->valstr.flag |= OPENCLOSELOCKINGSTA5;
			if(!((*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCUR)||(*(openCloseLockingSts->valstr.stateJudgeflag)&STATEOVERCURI0)))
			{
				addSOE(comProSts,&comProSts->yx.openingLocked,ON);
			}
		}

        if(openCloseLockingSts->valstr.flag&OPENCLOSELOCKINGSTA5)//分闸闭锁计时
        {
            if(!(*(openCloseLockingSts->valstr.gTime)&LOAD_ENTIMERS))
            {
                *(openCloseLockingSts->valstr.gTime) = LOAD_ENTIMERS;//启动定时
            }
            if((*(openCloseLockingSts->valstr.gTime)&LOAD_TITIMERS)>=(uint32_t)(*(openCloseLockingSts->parastr.pOpenLockTime))*1000)
            {
                openCloseLockingSts->valstr.flag = OPENCLOSELOCKINGSTA1;//非刚合闸状态
                *(openCloseLockingSts->valstr.gTime) = 0;
                addSOE(comProSts,&comProSts->yx.openingLocked,OFF);
            }
        }
    }
}

/**
  * @Description: 分合闸闭锁复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void openCloseLocking_rest(ComProSts *comProSts,OpenCloseLockingSts *openCloseLockingSts)
{
    if(*(openCloseLockingSts->valstr.resetflag)&(1<<LOAD_OPENCLOSELOCK))//复位
    {
        *(openCloseLockingSts->valstr.resetflag) &= ~(1<<LOAD_OPENCLOSELOCK);
        openCloseLockingSts->valstr.flag = OPENCLOSELOCKINGSTA1;//非刚合闸状态
		openCloseLockingSts->valstr.flag &= ~OPENCLOSELOCKINGSTA2;
        *(openCloseLockingSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.failAfterClosing,OFF);
        addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
        addSOE(comProSts,&comProSts->yx.openingLocked,OFF);
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
    }
}

/**
  * @Description: 残压闭锁.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void fewVolLock_ctrl(ComProSts *comProSts,FewVolLockSts *fewVolLockSts)
{
    if((*(comProSts->yx.switchOpen.value)==ON)&&(*(comProSts->yx.switchClose.value)==OFF))//分位
    {
        if(*(fewVolLockSts->parastr.pSwitch) == SWITCH_ON)
        {
            //Uab侧
            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA1))&&\
                    (*(comProSts->yc.Uab)<*(fewVolLockSts->parastr.pValue)))//小于残压
            {
                fewVolLockSts->valstr.flag |= FEWVOLLOCKUABSTA1;
            }

            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA3))&&(fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA1)&&\
                    ((*(comProSts->yc.Uab)>*(fewVolLockSts->parastr.pValue))))//检测到残压
            {
                fewVolLockSts->valstr.flag |= FEWVOLLOCKUABSTA3;
            }
            if((!(fewVolLockSts->valstr.flag&(FEWVOLLOCKUABSTA2|FEWVOLLOCKUABSTA4)))&&(fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA3))
            {
                if(!(*(fewVolLockSts->valstr.gUabTime)&LOAD_ENTIMERS))
                {
                    *(fewVolLockSts->valstr.gUabTime) = LOAD_ENTIMERS;//启动定时
                }
                //分断X时间，联络XL时间
                if(((*(comProSts->yx.breakContact.value)==OFF)&&((*(fewVolLockSts->valstr.gUabTime)&LOAD_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pXTime))*1000))||\
                        ((*(comProSts->yx.breakContact.value)==ON)&&((*(fewVolLockSts->valstr.gUabTime)&LOAD_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pXLTime))*1000)))
                {
                    //时间内
                    if(*(comProSts->yc.Uab)<*(fewVolLockSts->parastr.pValue))//小于残压
                    {
                        fewVolLockSts->valstr.flag |= FEWVOLLOCKUABSTA4;
                        fewVolLockSts->valstr.flag |= RESETFLAG;
                        addSOE(comProSts,&comProSts->yx.remainClockClossing,ON);
                        addSOE(comProSts,&comProSts->yx.closingLocked,ON);
                        *(fewVolLockSts->valstr.gUabTime) = 0;
                    }
                }
                else
                {
                    //时间外
                    fewVolLockSts->valstr.flag |= FEWVOLLOCKUABSTA2;
                    *(fewVolLockSts->valstr.gUabTime) = 0;
                }
            }
            if((fewVolLockSts->valstr.flag&FEWVOLLOCKUABSTA2)&&\
                    (*(comProSts->yc.Uab)<*(fewVolLockSts->parastr.pValue)))//无压复位
            {
                fewVolLockSts->valstr.flag &= ~(FEWVOLLOCKUABSTA1|FEWVOLLOCKUABSTA2|FEWVOLLOCKUABSTA3|FEWVOLLOCKUABSTA4);
                *(fewVolLockSts->valstr.gUabTime) = 0;
            }

            //Ucb侧
            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA1))&&\
                    (*(comProSts->yc.Ucb)<*(fewVolLockSts->parastr.pValue)))//小于残压
            {
                fewVolLockSts->valstr.flag |= FEWVOLLOCKUCBSTA1;
            }

            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA3))&&(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA1)&&\
                    ((*(comProSts->yc.Ucb)>*(fewVolLockSts->parastr.pValue))))//检测到残压
            {
                fewVolLockSts->valstr.flag |= FEWVOLLOCKUCBSTA3;
            }

            if((!(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA4))&&(!(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA2))&&(fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA3))
            {
                if(!(*(fewVolLockSts->valstr.gUcbTime)&LOAD_ENTIMERS))
                {
                    *(fewVolLockSts->valstr.gUcbTime) = LOAD_ENTIMERS;//启动定时
                }
                //分断X时间，联络XL时间
                if(((*(comProSts->yx.breakContact.value)==OFF)&&((*(fewVolLockSts->valstr.gUcbTime)&LOAD_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pXTime))*1000))||\
                        ((*(comProSts->yx.breakContact.value)==ON)&&((*(fewVolLockSts->valstr.gUcbTime)&LOAD_TITIMERS)<(uint32_t)(*(fewVolLockSts->parastr.pXLTime))*1000)))
                {
                    //时间内
                    if(*(comProSts->yc.Ucb)<*(fewVolLockSts->parastr.pValue))//小于残压
                    {
                        fewVolLockSts->valstr.flag |= FEWVOLLOCKUCBSTA4;
                        fewVolLockSts->valstr.flag |= RESETFLAG;
                        addSOE(comProSts,&comProSts->yx.remainClockClossing,ON);
                        addSOE(comProSts,&comProSts->yx.closingLocked,ON);
                        *(fewVolLockSts->valstr.gUcbTime) = 0;
                    }
                }
                else
                {
                    //时间外
                    fewVolLockSts->valstr.flag |= FEWVOLLOCKUCBSTA2;
                    *(fewVolLockSts->valstr.gUcbTime) = 0;
                }
            }
            if((fewVolLockSts->valstr.flag&FEWVOLLOCKUCBSTA2)&&\
                    (*(comProSts->yc.Ucb)<*(fewVolLockSts->parastr.pValue)))//无压复位
            {
                fewVolLockSts->valstr.flag &= ~(FEWVOLLOCKUCBSTA1|FEWVOLLOCKUCBSTA2|FEWVOLLOCKUCBSTA3|FEWVOLLOCKUCBSTA4);
                *(fewVolLockSts->valstr.gUcbTime) = 0;
            }
        }
    }
    else
    {
        fewVolLockSts->valstr.flag = 0;
        *(fewVolLockSts->valstr.gUabTime) = 0;
        *(fewVolLockSts->valstr.gUcbTime) = 0;
        addSOE(comProSts,&comProSts->yx.remainClockClossing,OFF);
    }
}

/**
  * @Description: 残压闭锁复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void fewVolLock_rest(ComProSts *comProSts,FewVolLockSts *fewVolLockSts)
{
    if(*(fewVolLockSts->valstr.resetflag)&(1<<LOAD_FEWVOLLOCK))//复位
    {
        *(fewVolLockSts->valstr.resetflag) &= ~(1<<LOAD_FEWVOLLOCK);
        fewVolLockSts->valstr.flag = 0;
        *(fewVolLockSts->valstr.gUabTime) = 0;
        *(fewVolLockSts->valstr.gUcbTime) = 0;
        addSOE(comProSts,&comProSts->yx.remainClockClossing,OFF);
        addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
    }
}

/**
  * @Description: 双侧有压禁止合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void doubleVol_ctrl(ComProSts *comProSts,DoubleVolSts *doubleVolSts)
{
    if((*(comProSts->yx.switchOpen.value)==ON)&&(*(comProSts->yx.switchClose.value)==OFF))//分位
    {
        if((*(doubleVolSts->valstr.stateJudgeflag)&STATEDOUBLEVOLTAGE)&&(*(doubleVolSts->parastr.pSwitch) == SWITCH_ON))
        {
            addSOE(comProSts,&comProSts->yx.doubleProhibitClossing,ON);
            addSOE(comProSts,&comProSts->yx.closingLocked,ON);
            doubleVolSts->valstr.flag = DOUBLEVOLSTA1;
        }
        else if(doubleVolSts->valstr.flag&DOUBLEVOLSTA1)
        {
            doubleVolSts->valstr.flag = 0;
            addSOE(comProSts,&comProSts->yx.doubleProhibitClossing,OFF);
			if(*(comProSts->yx.remainClockClossing.value)==OFF)
			{
				addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
			} 
        }
    }
    else if((*(comProSts->yx.switchOpen.value)==OFF)&&(*(comProSts->yx.switchClose.value)==ON))//合位
    {
        if(doubleVolSts->valstr.flag&DOUBLEVOLSTA1)
        {
            doubleVolSts->valstr.flag = 0;
            addSOE(comProSts,&comProSts->yx.doubleProhibitClossing,OFF);
            addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
        }
    }
}

/**
  * @Description: 双侧有压禁止合闸复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void doubleVol_rest(ComProSts *comProSts,DoubleVolSts *doubleVolSts)
{
    if(*(doubleVolSts->valstr.resetflag)&(1<<LOAD_DOUBLEVOL))//复位
    {
        *(doubleVolSts->valstr.resetflag) &= ~(1<<LOAD_DOUBLEVOL);
        doubleVolSts->valstr.flag = 0;
        addSOE(comProSts,&comProSts->yx.remainClockClossing,OFF);
        addSOE(comProSts,&comProSts->yx.closingLocked,OFF);
    }
}

/**
  * @Description: 单侧失压合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void SingleLossClose_ctrl(ComProSts *comProSts,SingleLossCloseSts *singleLossCloseSts)
{

    if(((*(singleLossCloseSts->parastr.pSwitch) == SWITCH_ON)||(*(singleLossCloseSts->parastr.pSwitch) == SWITCH_ALARM))&&\
            ((*(comProSts->yx.switchClose.value)==OFF)&&(*(comProSts->yx.switchOpen.value)==ON)))//分位
    {
        if(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA4)
        {
            singleLossCloseSts->valstr.flag = 0;
        }
        if(((!(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA1))||(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA2))&&\
                (*(singleLossCloseSts->valstr.stateJudgeflag)&STATEDOUBLEVOLTAGE))
        {
            if(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA2)//复位双侧有压后单侧有压
            {
                singleLossCloseSts->valstr.flag = 0;
                *(singleLossCloseSts->valstr.gTime) = 0;
            }
            if(!(*(singleLossCloseSts->valstr.gTime)&LOAD_ENTIMERS))
            {
                *(singleLossCloseSts->valstr.gTime) = LOAD_ENTIMERS;//启动定时
            }
            if((*(singleLossCloseSts->valstr.gTime)&LOAD_TITIMERS)>SINGELELOSSCLOSETIME)
            {
                singleLossCloseSts->valstr.flag |= SINGELELOSSCLOSESTA1;//双侧有压固有时间以上
                *(singleLossCloseSts->valstr.gTime) = 0;
            }
        }
        else if((singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA1)&&(*(comProSts->yx.closingLocked.value)==OFF))
        {
            if(*(singleLossCloseSts->valstr.stateJudgeflag)&STATESINGLEVOLTAGELOSS)
            {
                singleLossCloseSts->valstr.flag |= SINGELELOSSCLOSESTA2;//双侧有压后单侧有压
                if(!(*(singleLossCloseSts->valstr.gTime)&LOAD_ENTIMERS))
                {
                    *(singleLossCloseSts->valstr.gTime) = LOAD_ENTIMERS;//启动定时
                }
                if((*(singleLossCloseSts->valstr.gTime)&LOAD_TITIMERS)>(uint32_t)(*(singleLossCloseSts->parastr.pTime))*1000)
                {
                    singleLossCloseSts->valstr.flag |= SINGELELOSSCLOSE|SINGELELOSSCLOSESTA3|RESETFLAG;
                    if(*(singleLossCloseSts->parastr.pSwitch) == SWITCH_ON)
                    {
                        comProSts->closing(ADDR_LOGIC_ACT,DO_CLOSE);
                    }
                    addSOE(comProSts,&comProSts->yx.singleLossClosing,ON);
                }
            }
            else if(!(*(singleLossCloseSts->valstr.stateJudgeflag)&STATEDOUBLEVOLTAGE))
            {
                singleLossCloseSts->valstr.flag = 0;
                *(singleLossCloseSts->valstr.gTime) = 0;
            }
        }
    }
    else
    {
        if(singleLossCloseSts->valstr.flag&SINGELELOSSCLOSESTA3)
        {
            singleLossCloseSts->valstr.flag |= SINGELELOSSCLOSESTA4;
        }
        else
        {
            singleLossCloseSts->valstr.flag = 0;
            *(singleLossCloseSts->valstr.gTime) = 0;
            addSOE(comProSts,&comProSts->yx.singleLossClosing,OFF);
        }
    }
}

/**
  * @Description: 单侧失压合闸复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void SingleLossClose_rest(ComProSts *comProSts,SingleLossCloseSts *singleLossCloseSts)
{
    if (*(singleLossCloseSts->valstr.resetflag)&(1<<LOAD_SINGELELOSSCLOSE))//复位
    {
        *(singleLossCloseSts->valstr.resetflag) &= ~(1<<LOAD_SINGELELOSSCLOSE);
        singleLossCloseSts->valstr.flag = 0;
        *(singleLossCloseSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.singleLossClosing,OFF);
    }
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @Description: 初始化.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void LoadSwitchCtrlInit(void)
{
    uint8_t pdrv;

    for(pdrv=0; pdrv<LOAD_DEVMAXNUM; pdrv++)
    {
        switch(pdrv)
        {
        case LOAD_DEV0:
            list_init(&s_ListTimers[pdrv]);
            //通用
			s_ComProSts[pdrv].pBreakWorkMode = &g_Parameter[BREAK_WORK_MODE];
			s_ComProSts[pdrv].pLoadWorkMode = &g_Parameter[LOAD_WORK_MODE];
            s_ComProSts[pdrv].pConnectSwitchRecognize = &g_Parameter[CONNECT_SWITCH_RECOGNIZE];
            s_ComProSts[pdrv].yx.switchOpen.value = &g_TelesignalDB[g_TelesignalAddr.switchOpen];
            s_ComProSts[pdrv].yx.switchClose.value = &g_TelesignalDB[g_TelesignalAddr.switchClose];
            s_ComProSts[pdrv].yx.functionHardStrap.value = &g_TelesignalDB[g_TelesignalAddr.functionHardStrap];
            s_ComProSts[pdrv].yx.FA_HardStrap.value = &g_TelesignalDB[g_TelesignalAddr.recloseFAHardStrap];
            s_ComProSts[pdrv].yx.swtichclass.value = &g_TelesignalDB[g_TelesignalAddr.swtichclass];
            s_ComProSts[pdrv].yx.telecontrol_Pro_Out.value = &g_TelesignalDB[g_TelesignalAddr.telecontrolProOut];

            s_ComProSts[pdrv].yx.breakContact.addr =  g_TelesignalAddr.breakContact;
            s_ComProSts[pdrv].yx.closingLocked.addr = g_TelesignalAddr.closingLocked;
            s_ComProSts[pdrv].yx.openingLocked.addr = g_TelesignalAddr.openingLocked;
            s_ComProSts[pdrv].yx.shortCircuitFault.addr = g_TelesignalAddr.shortCircuitFault;
            s_ComProSts[pdrv].yx.earthingFault.addr = g_TelesignalAddr.earthingFault;
            s_ComProSts[pdrv].yx.protectionAct.addr = g_TelesignalAddr.protectionAct;
            s_ComProSts[pdrv].yx.protectionClock.addr = g_TelesignalAddr.protectionLock;
            s_ComProSts[pdrv].yx.overcurrentIa[0].addr = g_TelesignalAddr.overcurrentIa1;
            s_ComProSts[pdrv].yx.overcurrentIb[0].addr = g_TelesignalAddr.overcurrentIb1;
            s_ComProSts[pdrv].yx.overcurrentIc[0].addr = g_TelesignalAddr.overcurrentIc1;
            s_ComProSts[pdrv].yx.overcurrentIa[1].addr = g_TelesignalAddr.overcurrentIa2;
            s_ComProSts[pdrv].yx.overcurrentIb[1].addr = g_TelesignalAddr.overcurrentIb2;
            s_ComProSts[pdrv].yx.overcurrentIc[1].addr = g_TelesignalAddr.overcurrentIc2;
            s_ComProSts[pdrv].yx.overcurrentIa[2].addr = g_TelesignalAddr.overcurrentIa3;
            s_ComProSts[pdrv].yx.overcurrentIb[2].addr = g_TelesignalAddr.overcurrentIb3;
            s_ComProSts[pdrv].yx.overcurrentIc[2].addr = g_TelesignalAddr.overcurrentIc3;
            s_ComProSts[pdrv].yx.overcurrentI0[0].addr = g_TelesignalAddr.overcurrentI01;
            s_ComProSts[pdrv].yx.overcurrentI0[1].addr = g_TelesignalAddr.overcurrentI02;
            s_ComProSts[pdrv].yx.lossTrip.addr = g_TelesignalAddr.lossTrip;
            s_ComProSts[pdrv].yx.getClossing.addr = g_TelesignalAddr.getClossing;
            s_ComProSts[pdrv].yx.failAfterClosing.addr = g_TelesignalAddr.closingFaultEvent;
            s_ComProSts[pdrv].yx.earthingFaultTripU0.addr = g_TelesignalAddr.earthingFaultTripU0;
            s_ComProSts[pdrv].yx.remainClockClossing.addr = g_TelesignalAddr.remainClockClossing;
            s_ComProSts[pdrv].yx.doubleProhibitClossing.addr = g_TelesignalAddr.doubleProhibitClossing;
            s_ComProSts[pdrv].yx.singleLossClosing.addr = g_TelesignalAddr.singleLossClosingEvent;
            s_ComProSts[pdrv].yx.breakingCurrent.addr = g_TelesignalAddr.breakingCurrent;

            s_ComProSts[pdrv].yx.breakContact.value =  &g_TelesignalDB[g_TelesignalAddr.breakContact];
            s_ComProSts[pdrv].yx.closingLocked.value = &g_TelesignalDB[g_TelesignalAddr.closingLocked];
            s_ComProSts[pdrv].yx.openingLocked.value = &g_TelesignalDB[g_TelesignalAddr.openingLocked];
            s_ComProSts[pdrv].yx.shortCircuitFault.value = &g_TelesignalDB[g_TelesignalAddr.shortCircuitFault];
            s_ComProSts[pdrv].yx.earthingFault.value = &g_TelesignalDB[g_TelesignalAddr.earthingFault];
            s_ComProSts[pdrv].yx.protectionAct.value = &g_TelesignalDB[g_TelesignalAddr.protectionAct];
            s_ComProSts[pdrv].yx.protectionClock.value = &g_TelesignalDB[g_TelesignalAddr.protectionLock];
            s_ComProSts[pdrv].yx.overcurrentIa[0].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIa1];
            s_ComProSts[pdrv].yx.overcurrentIb[0].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIb1];
            s_ComProSts[pdrv].yx.overcurrentIc[0].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIc1];
            s_ComProSts[pdrv].yx.overcurrentIa[1].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIa2];
            s_ComProSts[pdrv].yx.overcurrentIb[1].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIb2];
            s_ComProSts[pdrv].yx.overcurrentIc[1].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIc2];
            s_ComProSts[pdrv].yx.overcurrentIa[2].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIa3];
            s_ComProSts[pdrv].yx.overcurrentIb[2].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIb3];
            s_ComProSts[pdrv].yx.overcurrentIc[2].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentIc3];
            s_ComProSts[pdrv].yx.overcurrentI0[0].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentI01];
            s_ComProSts[pdrv].yx.overcurrentI0[1].value = &g_TelesignalDB[g_TelesignalAddr.overcurrentI02];
            s_ComProSts[pdrv].yx.lossTrip.value = &g_TelesignalDB[g_TelesignalAddr.lossTrip];
            s_ComProSts[pdrv].yx.getClossing.value = &g_TelesignalDB[g_TelesignalAddr.getClossing];
            s_ComProSts[pdrv].yx.failAfterClosing.value = &g_TelesignalDB[g_TelesignalAddr.closingFaultEvent];
            s_ComProSts[pdrv].yx.earthingFaultTripU0.value = &g_TelesignalDB[g_TelesignalAddr.earthingFaultTripU0];
            s_ComProSts[pdrv].yx.remainClockClossing.value = &g_TelesignalDB[g_TelesignalAddr.remainClockClossing];
            s_ComProSts[pdrv].yx.doubleProhibitClossing.value = &g_TelesignalDB[g_TelesignalAddr.doubleProhibitClossing];
            s_ComProSts[pdrv].yx.singleLossClosing.value = &g_TelesignalDB[g_TelesignalAddr.singleLossClosingEvent];
            s_ComProSts[pdrv].yx.breakingCurrent.value = &g_TelesignalDB[g_TelesignalAddr.breakingCurrent];

            s_ComProSts[pdrv].yc.Ia = &g_TelemetryDB[g_TelemetryAddr.Ia];
            s_ComProSts[pdrv].yc.Ib = &g_TelemetryDB[g_TelemetryAddr.Ib];
            s_ComProSts[pdrv].yc.Ic = &g_TelemetryDB[g_TelemetryAddr.Ic];
            s_ComProSts[pdrv].yc.I0 = &g_TelemetryDB[g_TelemetryAddr.I0];
            s_ComProSts[pdrv].yc.Uab = &g_TelemetryDB[g_TelemetryAddr.Uab];            
            s_ComProSts[pdrv].yc.U0 = &g_TelemetryDB[g_TelemetryAddr.U0];

            s_ComProSts[pdrv].opening = &rt_hw_do_operate;
            s_ComProSts[pdrv].closing = &rt_hw_do_operate;
            s_ComProSts[pdrv].outputSoe = &DBWriteSOE;
            s_ComProSts[pdrv].outputFevent = &DBWriteFEVENT;
            
            //复位
            s_Rest[pdrv].valstr.flag = 0;
            addtimers(pdrv,&s_Rest[pdrv].valstr.gTime);
            s_Rest[pdrv].valstr.StateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.lossTripflag = &s_LossTrip[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.getVolCloseflag = &s_GetVolClose[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.openCloseLockingflag = &s_OpenCloseLocking[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.fewVolLockflag = &s_FewVolLock[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.doubleVol = &s_DoubleVol[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.SingleLossClose = &s_SingleLossClose[pdrv].valstr.flag;
            s_Rest[pdrv].parastr.pSwitch = &g_pFixedValue[AUTOMATIC_RESET_SWITCH];
            s_Rest[pdrv].parastr.pTime = &g_pFixedValue[AUTOMATIC_RESET_TIME];
            //状态标志
            s_stateJudge[pdrv].valstr.flag = 0;
            s_stateJudge[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_stateJudge[pdrv].valstr.gTimeOvercur[0]);
            addtimers(pdrv,&s_stateJudge[pdrv].valstr.gTimeOvercur[1]);
            addtimers(pdrv,&s_stateJudge[pdrv].valstr.gTimeOvercur[2]);
            s_stateJudge[pdrv].parastr.pSwitchOvercur[0] = &g_pFixedValue[OVER_CURRENT_SWITCH1];
            s_stateJudge[pdrv].parastr.pSwitchOvercur[1] = &g_pFixedValue[OVER_CURRENT_SWITCH2];
            s_stateJudge[pdrv].parastr.pSwitchOvercur[2] = &g_pFixedValue[OVER_CURRENT_SWITCH3];
            s_stateJudge[pdrv].parastr.pValueOvercur[0] = &g_pFixedValue[OVER_CURRENT_VALUE1];
            s_stateJudge[pdrv].parastr.pValueOvercur[1] = &g_pFixedValue[OVER_CURRENT_VALUE2];
            s_stateJudge[pdrv].parastr.pValueOvercur[2] = &g_pFixedValue[OVER_CURRENT_VALUE3];
            s_stateJudge[pdrv].parastr.pTimeOvercur[0] = &g_pFixedValue[OVER_CURRENT_TIME1];
            s_stateJudge[pdrv].parastr.pTimeOvercur[1] = &g_pFixedValue[OVER_CURRENT_TIME2];
            s_stateJudge[pdrv].parastr.pTimeOvercur[2] = &g_pFixedValue[OVER_CURRENT_TIME3];
            addtimers(pdrv,&s_stateJudge[pdrv].valstr.gTimeOvercurI0[0]);
            addtimers(pdrv,&s_stateJudge[pdrv].valstr.gTimeOvercurI0[1]);
            s_stateJudge[pdrv].parastr.pSwitchOvercurI0[0] = &g_pFixedValue[OVER_CURRENTI0_SWITCH1];
            s_stateJudge[pdrv].parastr.pSwitchOvercurI0[1] = &g_pFixedValue[OVER_CURRENTI0_SWITCH2];
            s_stateJudge[pdrv].parastr.pValueOvercurI0[0] = &g_pFixedValue[OVER_CURRENTI0_VALUE1];
            s_stateJudge[pdrv].parastr.pValueOvercurI0[1] = &g_pFixedValue[OVER_CURRENTI0_VALUE2];
            s_stateJudge[pdrv].parastr.pTimeOvercurI0[0] = &g_pFixedValue[OVER_CURRENTI0_TIME1];
            s_stateJudge[pdrv].parastr.pTimeOvercurI0[1] = &g_pFixedValue[OVER_CURRENTI0_TIME2];
            s_stateJudge[pdrv].parastr.pValuehavevol = &g_pFixedValue[VOLTAGE_VALUE];
            s_stateJudge[pdrv].parastr.pValueLossvol = &g_pFixedValue[NONVOLTAGE_VALUE];
            s_stateJudge[pdrv].parastr.pValueLosscur = &g_pFixedValue[CURRENT_VALUE];
            s_stateJudge[pdrv].parastr.pSwitchBreakcur = &g_pFixedValue[BREAKING_CURRENT_SWITCH];
            s_stateJudge[pdrv].parastr.pValueBreakcur = &g_pFixedValue[BREAKING_CURRENT_VALUE];
            s_stateJudge[pdrv].parastr.pSwitchVoltageU0 = &g_pFixedValue[VOLTAGE_U0_SWITCH];
            s_stateJudge[pdrv].parastr.pValueVoltageU0 = &g_pFixedValue[VOLTAGE_U0_VALUE];
            addtimers(pdrv,&s_stateJudge[pdrv].valstr.gTimeConnectSwitchRecognize);  
			s_stateJudge[pdrv].valstr.flag = (STATENOVOLTAGE|STATENOCURRENT);
            //零序过流
            s_OvercurI0[pdrv].valstr.flag = 0;
            s_OvercurI0[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            //失电分闸
            s_LossTrip[pdrv].valstr.flag = 0;
            s_LossTrip[pdrv].valstr.openCloseLockingflag = &s_OpenCloseLocking[pdrv].valstr.flag;
            s_LossTrip[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_LossTrip[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_LossTrip[pdrv].valstr.gTime);
            s_LossTrip[pdrv].parastr.pSwitch = &g_pFixedValue[LOSS_ELECTRICITY_SWITCH];
            s_LossTrip[pdrv].parastr.pTime = &g_pFixedValue[LOSS_OPENING_TIME];
            //得电合闸
            s_GetVolClose[pdrv].valstr.flag = 0;
            s_GetVolClose[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_GetVolClose[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_GetVolClose[pdrv].valstr.gTime);
            s_GetVolClose[pdrv].parastr.pSwitch = &g_pFixedValue[GET_VOLTAGE_CLOSSING_SWITCH];
            s_GetVolClose[pdrv].parastr.pXTime = &g_pFixedValue[GET_VOLTAGE_CLOSSING_X_TIME];
            //合分闸闭锁
            s_OpenCloseLocking[pdrv].valstr.flag = 0;
            s_OpenCloseLocking[pdrv].valstr.getVolCloseflag = &s_GetVolClose[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.singleLossCloseflag = &s_SingleLossClose[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.lossTripflag = &s_LossTrip[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_OpenCloseLocking[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_OpenCloseLocking[pdrv].valstr.gTime);
//            s_OpenCloseLocking[pdrv].parastr.pSwitchClose = &pValueParaDB->Data.loadSwitchValue.Str.clossingLockedSwitch;
//            s_OpenCloseLocking[pdrv].parastr.pSwitchOpen = &pValueParaDB->Data.loadSwitchValue.Str.openingLockedSwitch;
            s_OpenCloseLocking[pdrv].parastr.pYTime = &g_pFixedValue[FAULT_CHECK_Y_TIME];
            s_OpenCloseLocking[pdrv].parastr.pOpenLockTime = &g_pFixedValue[SHORT_LOCKED_OPENING_TIME];
            //残压闭锁
            s_FewVolLock[pdrv].valstr.flag = 0;
            s_FewVolLock[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_FewVolLock[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_FewVolLock[pdrv].valstr.gUabTime);
            addtimers(pdrv,&s_FewVolLock[pdrv].valstr.gUcbTime);
            s_FewVolLock[pdrv].parastr.pSwitch = &g_pFixedValue[LOSS_ELECTRICITY_SWITCH];
            s_FewVolLock[pdrv].parastr.pValue = &g_pFixedValue[REMAIN_VOLTAGE_VALUE];
            s_FewVolLock[pdrv].parastr.pXTime = &g_pFixedValue[GET_VOLTAGE_CLOSSING_SWITCH];
            s_FewVolLock[pdrv].parastr.pXLTime = &g_pFixedValue[SINGLE_LOSS_VOLTAGE_XLTIME];
            //双侧有压
            s_DoubleVol[pdrv].valstr.flag = 0;
            s_DoubleVol[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_DoubleVol[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            s_DoubleVol[pdrv].parastr.pSwitch = &g_pFixedValue[DOUBLE_VOLTAGE_SWITCH];
            //单侧失电合闸
            s_SingleLossClose[pdrv].valstr.flag = 0;
            s_SingleLossClose[pdrv].valstr.stateJudgeflag = &s_stateJudge[pdrv].valstr.flag;
            s_SingleLossClose[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_SingleLossClose[pdrv].valstr.gTime);
            s_SingleLossClose[pdrv].parastr.pSwitch = &g_pFixedValue[SINGLE_LOSS_VOLTAGE_SWITCH];
            s_SingleLossClose[pdrv].parastr.pTime = &g_pFixedValue[SINGLE_LOSS_VOLTAGE_XLTIME];
			//初始闭锁清除
			addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.openingLocked,OFF);
            break;
        }
    }
}

/**
  * @Description: 主保护程序.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void LoadSwitchCtrlClock(void)
{
    uint8_t pdrv;

    for(pdrv=0; pdrv<LOAD_DEVMAXNUM; pdrv++)
    {
        add_timers(pdrv);//定时增加
        		
        switch(pdrv)
        {
        case LOAD_DEV0:
            if(*(s_ComProSts[pdrv].yx.swtichclass.value)==OFF)
            {
                s_ComProSts[pdrv].WorkMode = *(s_ComProSts[pdrv].pBreakWorkMode);
            }
            else
            {
                s_ComProSts[pdrv].WorkMode = TYPE_BREAKER_NUM + *(s_ComProSts[pdrv].pLoadWorkMode);
            }
            if(g_Parameter[CFG_PRO_VOL_N] == 0)
            {s_ComProSts[pdrv].yc.Ucb = &g_TelemetryDB[g_TelemetryAddr.UAB];}
            else
            {s_ComProSts[pdrv].yc.Ucb = &g_TelemetryDB[g_TelemetryAddr.UCB];}
            
			if((!(s_ComProSts[pdrv].WorkMode == TYPE_BREAKER_COMMON))&&\
				(!(s_ComProSts[pdrv].WorkMode == TYPE_BREAKER_NONE))&&\
				(!(s_ComProSts[pdrv].WorkMode == TYPE_LOADSWTICH_NONE)))
			{
				if((*(s_ComProSts[pdrv].yx.functionHardStrap.value)==ON)&&(*(s_ComProSts[pdrv].yx.telecontrol_Pro_Out.value)==OFF))//保护压板
				{
					state_judge(&s_ComProSts[pdrv],&s_stateJudge[pdrv]);//状态判断
				  #ifdef FAPROTECTIONENABLING
					if(*(s_ComProSts[pdrv].yx.FA_HardStrap.value)==ON)//FA保护压板
				  #endif
					{
						if(s_ComProSts[pdrv].WorkMode==TYPE_LOADSWTICH_DIVIDE)//分界负荷开关
						{
							overcurI0_ctrl(&s_ComProSts[pdrv],&s_OvercurI0[pdrv]);//零序过流
							getVolClose_ctrl(&s_ComProSts[pdrv],&s_GetVolClose[pdrv]);//得电合闸
							openCloseLocking_ctrl(&s_ComProSts[pdrv],&s_OpenCloseLocking[pdrv]);//分合闸闭锁
							lossTrip_ctrl(&s_ComProSts[pdrv],&s_LossTrip[pdrv]);//失压跳闸
							fewVolLock_ctrl(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
						}
						else if((s_ComProSts[pdrv].WorkMode==TYPE_BREAKER_CURCOUNT)||(s_ComProSts[pdrv].WorkMode==TYPE_LOADSWTICH_CURCOUNT))//电流计数型
						{
							overcurI0_ctrl(&s_ComProSts[pdrv],&s_OvercurI0[pdrv]);//零序过流
							getVolClose_ctrl(&s_ComProSts[pdrv],&s_GetVolClose[pdrv]);//得电合闸
							openCloseLocking_ctrl(&s_ComProSts[pdrv],&s_OpenCloseLocking[pdrv]);//分合闸闭锁
							lossTrip_ctrl(&s_ComProSts[pdrv],&s_LossTrip[pdrv]);//失压跳闸
							fewVolLock_ctrl(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
						}
						else
						{
							if(*(s_ComProSts[pdrv].yx.breakContact.value)==OFF)//分断
							{
								getVolClose_ctrl(&s_ComProSts[pdrv],&s_GetVolClose[pdrv]);//得电合闸
								openCloseLocking_ctrl(&s_ComProSts[pdrv],&s_OpenCloseLocking[pdrv]);//分合闸闭锁
								lossTrip_ctrl(&s_ComProSts[pdrv],&s_LossTrip[pdrv]);//失压跳闸
								fewVolLock_ctrl(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
							}
							else//联络
							{
								SingleLossClose_ctrl(&s_ComProSts[pdrv],&s_SingleLossClose[pdrv]);//单侧失压合闸
								openCloseLocking_ctrl(&s_ComProSts[pdrv],&s_OpenCloseLocking[pdrv]);//分合闸闭锁
								lossTrip_ctrl(&s_ComProSts[pdrv],&s_LossTrip[pdrv]);//失压跳闸
								fewVolLock_ctrl(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
								doubleVol_ctrl(&s_ComProSts[pdrv],&s_DoubleVol[pdrv]);//双侧有压禁止合闸
							}
						}
					}
				}
			}
            rest_ctrl(pdrv,&s_ComProSts[pdrv],&s_Rest[pdrv]);//复位
            state_judge_rest(&s_ComProSts[pdrv],&s_stateJudge[pdrv]);//状态复位
            overcurI0_rest(&s_ComProSts[pdrv],&s_OvercurI0[pdrv]);//零序过流
            lossTrip_rest(&s_ComProSts[pdrv],&s_LossTrip[pdrv]);//失压跳闸
            getVolClose_rest(&s_ComProSts[pdrv],&s_GetVolClose[pdrv]);//得电合闸
            openCloseLocking_rest(&s_ComProSts[pdrv],&s_OpenCloseLocking[pdrv]);//分合闸闭锁
            fewVolLock_rest(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
            doubleVol_rest(&s_ComProSts[pdrv],&s_DoubleVol[pdrv]);//双侧有压禁止合闸
            fewVolLock_rest(&s_ComProSts[pdrv],&s_FewVolLock[pdrv]);//残压闭锁
            SingleLossClose_rest(&s_ComProSts[pdrv],&s_SingleLossClose[pdrv]);//单侧失压合闸
            break;
        }
    }
}

/**
  * @Description: 手动复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void LoadSwitchCtrlReset(uint8_t pdrv)
{
    s_Rest[pdrv].valstr.flag |= MANRESETFLAG;//复位标志
}


/* END OF FILE ---------------------------------------------------------------*/


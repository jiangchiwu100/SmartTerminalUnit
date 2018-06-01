/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      breaker_protect.c
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2017.09.05
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */


/* INCLUDES ------------------------------------------------------------------*/
#include "breaker_protect.h"
#include "breaker_interface.h"
#include "math.h"


/* PRIVATE VARIABLES --------------------------------------------------------*/
static List s_ListTimers[BRE_DEVMAXNUM];//定时器链表链表//0x80000000使能位
static ComProSts s_ComProSts[BRE_DEVMAXNUM];
static RestSts s_Rest[BRE_DEVMAXNUM];
static InrushSts s_Inrush[BRE_DEVMAXNUM];
static IACCSts s_IACC[BRE_DEVMAXNUM];
static InverseSts s_Inverse[BRE_DEVMAXNUM];
static OvercurSts s_Overcur[BRE_DEVMAXNUM];
static OvercurI0Sts s_OvercurI0[BRE_DEVMAXNUM];
static SecondaryRecloseLockSts s_SecondaryRecloseLock[BRE_DEVMAXNUM];
static RecloseSts s_Reclose[BRE_DEVMAXNUM];
static RecloseI0Sts s_RecloseI0[BRE_DEVMAXNUM];


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : Make the TIMER.
  * @param : [pdrv]
  * @param : [dev]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void addtimers(uint8_t pdrv, uint32_t **dev)
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
        if((*((uint32_t *)(element->data)))&BRE_ENTIMERS)
        {
            (*((uint32_t *)(element->data)))++;
        }
        element = element->next;
    }
}

/**
  * @brief : Make the SOE.
  * @param : [comProSts]
	* @param : [telesignal]
	* @param : [value]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void addSOE(ComProSts *comProSts, STelesignalStr *telesignal, uint8_t value)
{
    if (*(telesignal->value) != value)
    {
        comProSts->outputSoe(telesignal->addr,value);
        if(value == ON)
        {
            comProSts->outputFevent(telesignal->addr,comProSts->fevent_yc_addr,8);
        }
        if(telesignal->addr == comProSts->yx.shortCircuitFault.addr)
        {
            if(value == ON)
            {
                g_StartWave = 1;// 录波开始
            }
        }
    }
}

/**
  * @brief : Reset protect.
  * @param : [comProSts]
	* @param : [inrushSts]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void rest_ctrl(uint8_t pdrv,ComProSts *comProSts, RestSts *restSts)
{
    static uint64_t temp[BRE_DEVMAXNUM];
    uint64_t tempk;

    // 自动复归
    if(*(restSts->parastr.pSwitch)==SWITCH_ON)
    {
        tempk = temp[pdrv];
        temp[pdrv] = 0;
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.iACCflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.inverseflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.overcurflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.overcurI0flag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.recloseflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.recloseI0flag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|(*(restSts->valstr.secondaryRecloseLockflag)&RESETFLAG);
        temp[pdrv] = (temp[pdrv]>>1)|((*(comProSts->yx.shortCircuitFault.value)&ON)||\
                          (*(comProSts->yx.earthingFault.value)&ON)||\
                          (*(comProSts->yx.protectionAct.value)&ON)||\
                          (*(comProSts->yx.protectionClock.value)&ON)||\
                          (*(comProSts->yx.recloseAct.value)&ON)||\
                          (*(comProSts->yx.recloseClock.value)&ON));

        if(tempk != temp[pdrv] )
        {
            *(restSts->valstr.gTime) = BRE_ENTIMERS;//启动定时器
        }

        if((*(restSts->valstr.gTime)&BRE_TITIMERS)>=(uint32_t)(*(restSts->parastr.pTime)*1000))
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

    // 手动复归
    if (restSts->valstr.flag&MANRESETFLAG)
    {
        restSts->valstr.flag = SETRESETFLAG;
        *(restSts->valstr.gTime) = 0;
        temp[pdrv] = 0;
    }
}

/**
  * @brief : Inrush suppression protect.
  * @param : [comProSts]
	* @param : [inrushSts]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void inrush_ctrl(ComProSts *comProSts, InrushSts *inrushSts)
{
	const float k = 0.2;
    if ((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))	//合位
    {
        if (*(inrushSts->parastr.pSwitch) == SWITCH_ON)	//涌流抑制投入	
        {
			if (!((inrushSts->valstr.flag)&INRUSH1))	//涌流抑制未发生
			{
                if (!(*(inrushSts->valstr.gTime)&BRE_ENTIMERS))
                {
                    *(inrushSts->valstr.gTime) = BRE_ENTIMERS;	//启动时钟
                }
                if ((*(inrushSts->valstr.gTime)&BRE_TITIMERS)>(uint32_t)(*(inrushSts->parastr.pTime)*1000))
                {
                    inrushSts->valstr.flag |= INRUSH1;
					inrushSts->valstr.flag |= INRUSH;    //无涌流抑制
                }
				else
				{
					if(((*(comProSts->yc.Ia2)>*(comProSts->yc.Ia)*k)||\
						(*(comProSts->yc.Ib2)>*(comProSts->yc.Ib)*k)||\
						(*(comProSts->yc.Ic2)>*(comProSts->yc.Ic)*k)))
					{
						inrushSts->valstr.flag &= ~INRUSH;	
					}
					else
					{
						inrushSts->valstr.flag |= INRUSH;
					}
				}
			}
        }
        else
        {
            inrushSts->valstr.flag |= INRUSH;
        }
    }
    else // 分位
    {
        inrushSts->valstr.flag = 0;						//标志复位
        *(inrushSts->valstr.gTime) = 0;
    }
}

/**
  * @brief : Inrush suppression reset.
  * @param : [comProSts]
	* @param : [inrushSts]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void inrush_rest(ComProSts *comProSts, InrushSts *inrushSts)
{
}

/**
  * @brief : iACC protect.
  * @param : [comProSts]
	* @param : [iACCSts]
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void iACC_ctrl(ComProSts *comProSts, IACCSts *iACCSts)
{
    if ((*(comProSts->yx.switchClose.value) == ON) && (*(comProSts->yx.switchOpen.value) == OFF))//合位
    {
        if(iACCSts->valstr.flag&IACCCURSTA2)//过流后合闸//复位状态量
        {
            iACCSts->valstr.flag = IACCCURSTA4;
            addSOE(comProSts,&comProSts->yx.overcurrentAccIa,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentAccIb,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentAccIc,OFF);
            addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
//          addSOE(comProSts,&comProSts->yx.recloseClock,OFF);					
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        }
		if(iACCSts->valstr.flag&IACCCURSTA4)
		{
			if(!(*(iACCSts->valstr.gTime[1])&BRE_ENTIMERS))
			{
				*(iACCSts->valstr.gTime[1]) = BRE_ENTIMERS;//启动定时
			}
			if((*(iACCSts->valstr.gTime[1])&BRE_TITIMERS)<(uint32_t)(*(iACCSts->parastr.pTime)*1000) + IACCTIME)
			{
				if(((*(iACCSts->parastr.pSwitch)==SWITCH_ON)&&((*(iACCSts->valstr.recloseflag)&JUDGRFLAG)||(*(iACCSts->valstr.recloseI0flag)&JUDGRFLAG))&&\
					(!(iACCSts->valstr.flag&IACCCURSTA1)))&&\
					(!(*(iACCSts->valstr.overcurflag)&JUDGRFLAG))&&(!(*(iACCSts->valstr.overcurI0flag)&JUDGRFLAG))&&\
					((*(comProSts->yc.Ia)>*(iACCSts->parastr.pValue))||\
					 (*(comProSts->yc.Ib)>*(iACCSts->parastr.pValue))||\
					 (*(comProSts->yc.Ic)>*(iACCSts->parastr.pValue))))//过流
				{

					if(!(*(iACCSts->valstr.gTime[0])&BRE_ENTIMERS))
					{
						*(iACCSts->valstr.gTime[0]) = BRE_ENTIMERS;//启动定时
					}
					if((*(iACCSts->valstr.gTime[0])&BRE_TITIMERS)>=(uint32_t)(*(iACCSts->parastr.pTime)*1000))
					{
						iACCSts->valstr.flag |= IACCCUR|IACCCURSTA1;
						comProSts->opening(ADDR_LOGIC_ACT,DO_OPEN);
						iACCSts->valstr.flag |= RESETFLAG;
						if(*(comProSts->yc.Ia)>*(iACCSts->parastr.pValue))
						{
							//SOE
							addSOE(comProSts,&comProSts->yx.overcurrentAccIa,ON);
						}
						if(*(comProSts->yc.Ib)>*(iACCSts->parastr.pValue))
						{
							//SOE
							addSOE(comProSts,&comProSts->yx.overcurrentAccIb,ON);
						}
						if(*(comProSts->yc.Ic)>*(iACCSts->parastr.pValue))
						{
							//SOE
							addSOE(comProSts,&comProSts->yx.overcurrentAccIc,ON);
						}
						addSOE(comProSts,&comProSts->yx.shortCircuitFault,ON);
						//addSOE(comProSts,&comProSts->yx.recloseClock,ON);
						addSOE(comProSts,&comProSts->yx.protectionAct,ON);
					}
				}
				else//不过流
				{
					*(iACCSts->valstr.gTime[0]) = 0;
				}
			}
			else
			{
				iACCSts->valstr.flag &= ~IACCCURSTA4;
				*(iACCSts->valstr.gTime[0]) = 0;
				*(iACCSts->valstr.gTime[1]) = 0;
			}
		}
        
    }
    else //if(iACCSts->valstr.flag&IACCCURSTA1)//分位//过流后分闸
    {
        iACCSts->valstr.flag &= (~IACCCURSTA1);
		iACCSts->valstr.flag |= IACCCURSTA4;
        iACCSts->valstr.flag |= IACCCURSTA2;
		*(iACCSts->valstr.gTime[1]) = 0;
		*(iACCSts->valstr.gTime[0]) = 0;

    }
}

/**
  * @brief:  iACC reset.
  * @param:  none
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void iACC_rest(ComProSts *comProSts,IACCSts *iACCSts)
{
    if(*(iACCSts->valstr.resetflag)&(1<<BRE_IACC))//复位
    {
        *(iACCSts->valstr.resetflag) &= ~(1<<BRE_IACC);
        iACCSts->valstr.flag = 0;
        *(iACCSts->valstr.gTime[0]) = 0;
		*(iACCSts->valstr.gTime[1]) = 0;
        addSOE(comProSts,&comProSts->yx.overcurrentAccIa,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentAccIb,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentAccIc,OFF);
        addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
        //addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
    }
}

/**
  * @Description: 反时限.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void inverse_ctrl(ComProSts *comProSts,InverseSts *inverseSts)
{

    const float k_k = 0.14;
    const float k_a = 1;
    const double k_n = 0.02;
    const float k_b = 2.97;
    const float k_minratio = 1.1;//最小倍数
    double curratio;
    float curtime;


    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位//无涌流抑制
    {
        if(inverseSts->valstr.flag&INVERSECURSTA2)//过流后合闸//复位状态量
        {
            inverseSts->valstr.flag = 0;
            *(inverseSts->valstr.gTime) = 0;
            addSOE(comProSts,&comProSts->yx.overcurrentInverseIa,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentInverseIb,OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentInverseIc,OFF);
            addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        }

        if(((*(inverseSts->parastr.pSwitch)==SWITCH_ON)&&\
                (!(inverseSts->valstr.flag&INVERSECURSTA1)))&&\
				(((*(inverseSts->valstr.recloseflag)&JUDGRFLAG)||(*(inverseSts->valstr.recloseI0flag)&JUDGRFLAG))||(*(inverseSts->valstr.inrushflag)&JUDGRFLAG))&&\
                (!(*(inverseSts->valstr.iACCflag)&JUDGRFLAG))&&(!(*(inverseSts->valstr.overcurflag)&JUDGRFLAG))&&(!(*(inverseSts->valstr.overcurI0flag)&JUDGRFLAG))&&\
                ((*(comProSts->yc.Ia)>*(inverseSts->parastr.pValue))||\
                 (*(comProSts->yc.Ib)>*(inverseSts->parastr.pValue))||\
                 (*(comProSts->yc.Ic)>*(inverseSts->parastr.pValue))))//过流
        {

            if(!(*(inverseSts->valstr.gTime)&BRE_ENTIMERS))
            {
                *(inverseSts->valstr.gTime) = BRE_ENTIMERS;//启动定时
            }

            curratio = ((((*(comProSts->yc.Ia)>*(comProSts->yc.Ib))?*(comProSts->yc.Ia):\
                          *(comProSts->yc.Ib))>*(comProSts->yc.Ic))?((*(comProSts->yc.Ia)>*(comProSts->yc.Ib))?*(comProSts->yc.Ia):\
                                  *(comProSts->yc.Ib)):*(comProSts->yc.Ic))/(*(inverseSts->parastr.pValue));

            curratio = curratio <= k_minratio ? k_minratio : curratio;

            curtime = k_k*(*(inverseSts->parastr.pTime)*1000)/((float)pow(curratio,k_n)-k_a)/k_b;

            curtime = curtime <= 0 ? 0 : curtime;

            if((*(inverseSts->valstr.gTime)&BRE_TITIMERS)>=(uint32_t)(curtime))
            {
                inverseSts->valstr.flag |= INVERSECUR|INVERSECURSTA1;
                comProSts->opening(ADDR_LOGIC_ACT,DO_OPEN);
                inverseSts->valstr.flag |= RESETFLAG;
                if(*(comProSts->yc.Ia)>*(inverseSts->parastr.pValue))
                {
                    //SOE
                    addSOE(comProSts,&comProSts->yx.overcurrentInverseIa,ON);
                }
                if(*(comProSts->yc.Ib)>*(inverseSts->parastr.pValue))
                {
                    //SOE
                    addSOE(comProSts,&comProSts->yx.overcurrentInverseIb,ON);
                }
                if(*(comProSts->yc.Ic)>*(inverseSts->parastr.pValue))
                {
                    //SOE
                    addSOE(comProSts,&comProSts->yx.overcurrentInverseIc,ON);
                }
                addSOE(comProSts,&comProSts->yx.shortCircuitFault,ON);
                addSOE(comProSts,&comProSts->yx.protectionAct,ON);
            }
        }
        else//不过流
        {
            *(inverseSts->valstr.gTime) = 0;
        }
    }
    else// if(inverseSts->valstr.flag&INVERSECURSTA1)//分位//过流后分闸
    {
        inverseSts->valstr.flag &= (~INVERSECURSTA1);
        inverseSts->valstr.flag |= INVERSECURSTA2;
    }
}

/**
  * @Description: 反时限保护复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void inverse_rest(ComProSts *comProSts,InverseSts *inverseSts)
{
    if(*(inverseSts->valstr.resetflag)&(1<<BRE_INVERSE))//复位
    {
        *(inverseSts->valstr.resetflag) &= ~(1<<BRE_INVERSE);
        inverseSts->valstr.flag = 0;
        *(inverseSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.overcurrentInverseIa,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentInverseIb,OFF);
        addSOE(comProSts,&comProSts->yx.overcurrentInverseIc,OFF);
        addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
    }
}

/**
  * @Description: 相间过流.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void overcur_ctrl(ComProSts *comProSts,OvercurSts *overcurSts)
{
    uint8_t i;

    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位//无涌流抑制
    {
        if(overcurSts->valstr.flag&OVERCURSTA2)//过流后合闸//复位状态量
        {
            overcurSts->valstr.flag = 0;
            *(overcurSts->valstr.gTime[0]) = 0;
            *(overcurSts->valstr.gTime[1]) = 0;
            *(overcurSts->valstr.gTime[2]) = 0;
            for(i=0; i<3; i++)
            {
                addSOE(comProSts,&comProSts->yx.overcurrentIa[i],OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentIb[i],OFF);
                addSOE(comProSts,&comProSts->yx.overcurrentIc[i],OFF);
            }
            addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        }

        for(i=0; i<3; i++) //检测过流
        {
            if(((*(overcurSts->parastr.pSwitch[i])==SWITCH_ON)&&\
				(((*(overcurSts->valstr.recloseflag)&JUDGRFLAG)||(*(overcurSts->valstr.recloseI0flag)&JUDGRFLAG))||(*(overcurSts->valstr.inrushflag)&JUDGRFLAG))&&\
                    (!(*(overcurSts->valstr.iACCflag)&JUDGRFLAG))&&(!(*(overcurSts->valstr.inverseflag)&JUDGRFLAG))&&(!(*(overcurSts->valstr.overcurI0flag)&JUDGRFLAG))&&\
                    (!(overcurSts->valstr.flag&(OVERCUR1|OVERCUR2|OVERCUR3))))&&\
                    ((*(comProSts->yc.Ia)>*(overcurSts->parastr.pValue[i]))||\
                     (*(comProSts->yc.Ib)>*(overcurSts->parastr.pValue[i]))||\
                     (*(comProSts->yc.Ic)>*(overcurSts->parastr.pValue[i]))))//过流
            {

                if(!(*(overcurSts->valstr.gTime[i])&BRE_ENTIMERS))
                {
                    *(overcurSts->valstr.gTime[i]) = BRE_ENTIMERS;//启动定时
                }
                if((*(overcurSts->valstr.gTime[i])&BRE_TITIMERS)>=(uint32_t)(*(overcurSts->parastr.pTime[i])*1000))
                {
                    overcurSts->valstr.flag |= (OVERCUR1<<i)|OVERCURSTA1;
                    comProSts->opening(ADDR_LOGIC_ACT,DO_OPEN);
                    overcurSts->valstr.flag |= RESETFLAG;
                    if(*(comProSts->yc.Ia)>*(overcurSts->parastr.pValue[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIa[i],ON);
                    }
                    if(*(comProSts->yc.Ib)>*(overcurSts->parastr.pValue[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIb[i],ON);
                    }
                    if(*(comProSts->yc.Ic)>*(overcurSts->parastr.pValue[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentIc[i],ON);
                    }
                    addSOE(comProSts,&comProSts->yx.shortCircuitFault,ON);
                    addSOE(comProSts,&comProSts->yx.protectionAct,ON);
                }
            }
            else//不过流
            {
                *(overcurSts->valstr.gTime[i]) = 0;
            }
        }
    }
    else //if(overcurSts->valstr.flag&OVERCURSTA1)//分位//过流后分闸
    {
        overcurSts->valstr.flag &= (~OVERCURSTA1);
        overcurSts->valstr.flag |= OVERCURSTA2;
    }
}

/**
  * @Description: 过流复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void overcur_rest(ComProSts *comProSts,OvercurSts *overcurSts)
{
    uint8_t i;

    if(*(overcurSts->valstr.resetflag)&(1<<BRE_OVERCUR))//复位
    {
        *(overcurSts->valstr.resetflag) &= ~(1<<BRE_OVERCUR);
        overcurSts->valstr.flag = 0;
        *(overcurSts->valstr.gTime[0]) = 0;
        *(overcurSts->valstr.gTime[1]) = 0;
        *(overcurSts->valstr.gTime[2]) = 0;
        for(i=0; i<3; i++)
        {
            addSOE(comProSts,&comProSts->yx.overcurrentIa[i],OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentIb[i],OFF);
            addSOE(comProSts,&comProSts->yx.overcurrentIc[i],OFF);
        }
        addSOE(comProSts,&comProSts->yx.shortCircuitFault,OFF);
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
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
    uint8_t i;

    if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位//无涌流抑制
    {
        if(overcurI0Sts->valstr.flag&OVERCURI0STA2)//过流后合闸//复位状态量
        {
            overcurI0Sts->valstr.flag = 0;
            *(overcurI0Sts->valstr.gTime[0]) = 0;
            *(overcurI0Sts->valstr.gTime[1]) = 0;
            for(i=0; i<2; i++)
            {
                addSOE(comProSts,&comProSts->yx.overcurrentI0[i],OFF);
            }
            addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
            addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
        }

        for(i=0; i<2; i++) //检测过流
        {		//（零序过流保护投入||零序报警）&&（重合闸||零序重合闸||）&&无后加速&&无反时限&&无零序过流保护&&无零序过流标志&&零序过流
            if((((*(overcurI0Sts->parastr.pSwitch[i])==SWITCH_ON)||(*(overcurI0Sts->parastr.pSwitch[i])==SWITCH_ALARM))&&\
				(((*(overcurI0Sts->valstr.recloseflag)&JUDGRFLAG)||(*(overcurI0Sts->valstr.recloseI0flag)&JUDGRFLAG))||(*(overcurI0Sts->valstr.inrushflag)&JUDGRFLAG))&&\
                    (!(*(overcurI0Sts->valstr.iACCflag)&JUDGRFLAG))&&(!(*(overcurI0Sts->valstr.inverseflag)&JUDGRFLAG))&&(!(*(overcurI0Sts->valstr.overcurflag)&JUDGRFLAG))&&\
                    (!(overcurI0Sts->valstr.flag&(OVERCURI01|OVERCURI02))))&&\
                    (*(comProSts->yc.I0)>*(overcurI0Sts->parastr.pValue[i])))//过流
            {
                if(!(*(overcurI0Sts->valstr.gTime[i])&BRE_ENTIMERS))
                {
                    *(overcurI0Sts->valstr.gTime[i]) = BRE_ENTIMERS;//启动定时
                }
                if((*(overcurI0Sts->valstr.gTime[i])&BRE_TITIMERS)>=(uint32_t)(*(overcurI0Sts->parastr.pTime[i])*1000))
                {
                    overcurI0Sts->valstr.flag |= (OVERCURI01<<i)|OVERCURI0STA1;                    
                    overcurI0Sts->valstr.flag |= RESETFLAG;
                    if(*(overcurI0Sts->parastr.pSwitch[i])==SWITCH_ON)
                    {
                        comProSts->opening(ADDR_LOGIC_ACT,DO_OPEN);
                        addSOE(comProSts,&comProSts->yx.protectionAct,ON);
                    }
                    if(*(comProSts->yc.I0)>*(overcurI0Sts->parastr.pValue[i]))
                    {
                        //SOE
                        addSOE(comProSts,&comProSts->yx.overcurrentI0[i],ON);
                    }
                    addSOE(comProSts,&comProSts->yx.earthingFault,ON);
                }
            }
            else
            {
                *(overcurI0Sts->valstr.gTime[i]) = 0;
            }
        }
    }
    else //if(overcurI0Sts->valstr.flag&OVERCURI0STA1)//分闸//过流后分闸
    {
        overcurI0Sts->valstr.flag &= (~OVERCURI0STA1);
        overcurI0Sts->valstr.flag |= OVERCURI0STA2;
    }
}

/**
  * @Description: 零序过流复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void overcurI0_rest(ComProSts *comProSts,OvercurI0Sts *overcurI0Sts)
{
    uint8_t i;

    if(*(overcurI0Sts->valstr.resetflag)&(1<<BRE_OVERCURI0))//复位
    {
        *(overcurI0Sts->valstr.resetflag) &= ~(1<<BRE_OVERCURI0);
        overcurI0Sts->valstr.flag = 0;
        *(overcurI0Sts->valstr.gTime[0]) = 0;
        *(overcurI0Sts->valstr.gTime[1]) = 0;
        for(i=0; i<2; i++)
        {
            addSOE(comProSts,&comProSts->yx.overcurrentI0[i],OFF);
        }
        addSOE(comProSts,&comProSts->yx.earthingFault,OFF);
        addSOE(comProSts,&comProSts->yx.protectionAct,OFF);
    }
}

/**
  * @Description: 二次重合闸闭锁.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void secondaryRecloseLock_ctrl(ComProSts *comProSts,SecondaryRecloseLockSts *secondaryRecloseLockSts)
{
    if((*(secondaryRecloseLockSts->parastr.pSwitch)==SWITCH_ON)&&\
            ((*(secondaryRecloseLockSts->valstr.recloseflag)&RECLOSE1)||\
             (*(secondaryRecloseLockSts->valstr.recloseI0flag)&RECLOSEI01)))//第一次重合闸状态有效
    {
        if((!(secondaryRecloseLockSts->valstr.flag&SENCONDARYRECLSELOCKSTA1))&&\
                (*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合闸//重合状态
        {
            if(!(*(secondaryRecloseLockSts->valstr.gTime)&BRE_ENTIMERS))
            {
                *(secondaryRecloseLockSts->valstr.gTime) = BRE_ENTIMERS;//启动定时
            }
            secondaryRecloseLockSts->valstr.flag |= SENCONDARYRECLSELOCKSTA1;
        }
        if((!(secondaryRecloseLockSts->valstr.flag&SENCONDARYRECLSELOCKSTA2))&&(secondaryRecloseLockSts->valstr.flag&SENCONDARYRECLSELOCKSTA1))
        {
            if((*(secondaryRecloseLockSts->valstr.gTime)&BRE_TITIMERS)<=(uint32_t)(*(secondaryRecloseLockSts->parastr.pTime)*1000))
            {
                if(((*(comProSts->yx.switchClose.value) == OFF)&&(*(comProSts->yx.switchOpen.value) == ON))&&\
                        ((*(secondaryRecloseLockSts->valstr.overcurflag)&JUDGRFLAG)||(*(secondaryRecloseLockSts->valstr.overcurI0flag)&JUDGRFLAG)||\
                         (*(secondaryRecloseLockSts->valstr.iACCflag)&JUDGRFLAG)||(*(secondaryRecloseLockSts->valstr.inverseflag)&JUDGRFLAG)))//分闸//任一种过流
                {
                    secondaryRecloseLockSts->valstr.flag |= SENCONDARYRECLSELOCKSTA2;
                    secondaryRecloseLockSts->valstr.flag |= SENCONDARYRECLSELOCK;
                    secondaryRecloseLockSts->valstr.flag |= RESETFLAG;
                    //soe,闭锁
                    addSOE(comProSts,&comProSts->yx.clockSecondaryClose,ON);
                    addSOE(comProSts,&comProSts->yx.recloseClock,ON);
                }
            }
            else
            {
                *(secondaryRecloseLockSts->valstr.gTime) = 0;
                secondaryRecloseLockSts->valstr.flag |= SENCONDARYRECLSELOCKSTA2;
            }
        }
    }
    else
    {
        secondaryRecloseLockSts->valstr.flag = 0;
        *(secondaryRecloseLockSts->valstr.gTime) = 0;
//        addSOE(comProSts,&comProSts->yx.clockSecondaryClose,OFF);
    }
	if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
	{
		if(secondaryRecloseLockSts->valstr.flag&SENCONDARYRECLSELOCK)
		{
			addSOE(comProSts,&comProSts->yx.recloseClock,OFF);	
			secondaryRecloseLockSts->valstr.flag = 0;
			*(secondaryRecloseLockSts->valstr.gTime) = 0;
			addSOE(comProSts,&comProSts->yx.clockSecondaryClose,OFF);
		}		
	}
}

/**
  * @Description: 二次重合闸闭锁复位.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void secondaryRecloseLock_rest(ComProSts *comProSts,SecondaryRecloseLockSts *secondaryRecloseLockSts)
{
    if(*(secondaryRecloseLockSts->valstr.resetflag)&(1<<BRE_SENCONDARYRECLSELOCK))//复位
    {
        *(secondaryRecloseLockSts->valstr.resetflag) &= ~(1<<BRE_SENCONDARYRECLSELOCK);
        secondaryRecloseLockSts->valstr.flag = 0;
        *(secondaryRecloseLockSts->valstr.gTime) = 0;
        addSOE(comProSts,&comProSts->yx.clockSecondaryClose,OFF);
        addSOE(comProSts,&comProSts->yx.recloseClock,OFF);	//注释
    }
}

/**
  * @Description: 相间重合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void reclose_ctrl(ComProSts *comProSts,RecloseSts *recloseSts)
{
    if((*(comProSts->yx.switchClose.value) == OFF)&&(*(comProSts->yx.switchOpen.value) == ON))//分位
    {
        if((*(comProSts->yx.recloseClock.value) == OFF)&&(!(recloseSts->valstr.flag&RECLOSESTA1)))//无闭锁合闸
        {
            if((*(recloseSts->valstr.iACCflag)&JUDGRFLAG)||(*(recloseSts->valstr.overcurflag)&JUDGRFLAG)||(*(recloseSts->valstr.inverseflag)&JUDGRFLAG))//过流导致分闸
            {
                if(recloseSts->valstr.time < (uint8_t)(*(recloseSts->parastr.pTimes)))//未达到重合次数
                {
                    if(!(*(recloseSts->valstr.gTime[0])&BRE_ENTIMERS))
                    {
                        *(recloseSts->valstr.gTime[0]) = BRE_ENTIMERS;//启动定时
                    }
                    if(((*(recloseSts->valstr.gTime[0])&BRE_TITIMERS))>=(uint32_t)((*(recloseSts->parastr.pTime[recloseSts->valstr.time]) + *(recloseSts->parastr.pRechargeTime))*1000))
                    {
                        comProSts->closing(ADDR_LOGIC_ACT,DO_CLOSE);
                        recloseSts->valstr.flag = (1<<recloseSts->valstr.time)|RECLOSESTA1;
                        recloseSts->valstr.flag |= RESETFLAG;
                        *(recloseSts->valstr.gTime[0]) = 0;
                        //soe,重合闸
                        addSOE(comProSts,&comProSts->yx.recloseProtection[recloseSts->valstr.time],ON);
                        addSOE(comProSts,&comProSts->yx.recloseProtection[recloseSts->valstr.time],OFF);
                        addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
                        addSOE(comProSts,&comProSts->yx.recloseAct,ON);
                        recloseSts->valstr.time++;
                    }
                }
                else//达到重合闸次数
                {
                    //soe,闭锁重合闸
                    addSOE(comProSts,&comProSts->yx.recloseClock,ON);
                    recloseSts->valstr.flag |= RECLOSELOCK|RESETFLAG;
                }
            }
            else if(!(*(recloseSts->valstr.overcurI0flag)&JUDGRFLAG))//无零序过流//手动合闸
            {
                recloseSts->valstr.time = 0;//清除重合闸次数
                recloseSts->valstr.flag = 0;//清除重合闸状态
                addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
                addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
            }
        }
        if(*(recloseSts->valstr.secondaryRecloseLockflag)&JUDGRFLAG)//二次重合闸闭锁，清除重合闸标志
        {
            recloseSts->valstr.time = 0;
            recloseSts->valstr.flag &= ~(RECLOSE1|RECLOSE2|RECLOSE3|RECLOSE4);
        }
        if(*(comProSts->yx.recloseClock.value) == ON)
        {
            recloseSts->valstr.flag |= RECLOSELOCK;
        }
        *(recloseSts->valstr.gTime[1]) = 0;
    }
    else if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
    {
        *(recloseSts->valstr.gTime[0]) = 0;
        if(recloseSts->valstr.flag&RECLOSESTA1)
        {
            recloseSts->valstr.flag &= (~RECLOSESTA1);
        }
		if(recloseSts->valstr.flag&RECLOSELOCK)
		{
			recloseSts->valstr.time = 0;//清除重合闸次数
			recloseSts->valstr.flag = 0;//清除重合闸状态
			addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
			addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
		}
        if((recloseSts->valstr.time != 0)&&(*(recloseSts->parastr.pResetTime) != 0))		//次数不为0						
        {
            if(!(*(recloseSts->valstr.gTime[1])&BRE_ENTIMERS))
            {
                *(recloseSts->valstr.gTime[1]) = BRE_ENTIMERS;//启动定时
            }
            if(((*(recloseSts->valstr.gTime[1])&BRE_TITIMERS))>=(uint32_t)(*(recloseSts->parastr.pResetTime)*1000))
            {
                recloseSts->valstr.time = 0;
                recloseSts->valstr.flag &= ~(RECLOSE1|RECLOSE2|RECLOSE3|RECLOSE4);
            }
        }
        else
        {
            *(recloseSts->valstr.gTime[1]) = 0;
        }        
    }
}

/**
  * @Description: 重合闸复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void reclose_rest(ComProSts *comProSts,RecloseSts *recloseSts)
{
    uint8_t i;
    if(*(recloseSts->valstr.resetflag)&(1<<BRE_RECLOSE))//自动、手动复位
    {
        *(recloseSts->valstr.resetflag) &= ~(1<<BRE_RECLOSE);
		
		{
			recloseSts->valstr.time = 0;
			*(recloseSts->valstr.gTime[0]) = 0;
			recloseSts->valstr.flag = 0;
			for(i=0; i<4; i++)
			{
				addSOE(comProSts,&comProSts->yx.recloseProtection[recloseSts->valstr.time],OFF);
			}
			addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
			addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
		}
    }
}

/**
  * @Description: 零序重合闸.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void recloseI0_ctrl(ComProSts *comProSts,RecloseI0Sts *recloseI0Sts)
{
    if((*(comProSts->yx.switchClose.value) == OFF)&&(*(comProSts->yx.switchOpen.value) == ON))//分位
    {
        if((*(comProSts->yx.recloseClock.value) == OFF)&&(!(recloseI0Sts->valstr.flag&RECLOSEI0STA1)))//无闭锁合闸
        {
            if(*(recloseI0Sts->valstr.overcurI0flag)&JUDGRFLAG)//过流导致分闸
            {
                if(recloseI0Sts->valstr.time < (uint8_t)(*(recloseI0Sts->parastr.pTimes)))//未达到重合次数
                {
                    if(!(*(recloseI0Sts->valstr.gTime[0])&BRE_ENTIMERS))
                    {
                        *(recloseI0Sts->valstr.gTime[0]) = BRE_ENTIMERS;//启动定时
                    }
                    if(((*(recloseI0Sts->valstr.gTime[0])&BRE_TITIMERS))>=((uint32_t)((*(recloseI0Sts->parastr.pTime[recloseI0Sts->valstr.time]) + *(recloseI0Sts->parastr.pRechargeTime))*1000)))
                    {
                        comProSts->closing(ADDR_LOGIC_ACT,DO_CLOSE);
                        recloseI0Sts->valstr.flag = (1<<recloseI0Sts->valstr.time)|RECLOSEI0STA1;
                        recloseI0Sts->valstr.flag |= RESETFLAG;
                        *(recloseI0Sts->valstr.gTime[0]) = 0;
                        //soe,重合闸
                        addSOE(comProSts,&comProSts->yx.zeroRecloseProtection[recloseI0Sts->valstr.time],ON);
                        addSOE(comProSts,&comProSts->yx.zeroRecloseProtection[recloseI0Sts->valstr.time],OFF);
                        addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
                        addSOE(comProSts,&comProSts->yx.recloseAct,ON);
                        recloseI0Sts->valstr.time++;
                    }
                }
                else//达到重合闸次数
                {
                    //soe,闭锁重合闸
                    addSOE(comProSts,&comProSts->yx.recloseClock,ON);
                    recloseI0Sts->valstr.flag |= (RECLOSEI0LOCK|RESETFLAG);
                }
            }
            else if((!(*(recloseI0Sts->valstr.overcurflag)&JUDGRFLAG))&&(!(*(recloseI0Sts->valstr.iACCflag)&JUDGRFLAG))&&(!(*(recloseI0Sts->valstr.inverseflag)&JUDGRFLAG)))//无相间过流//手动合闸
            {
                recloseI0Sts->valstr.time = 0;//清除重合闸次数
                recloseI0Sts->valstr.flag = 0;//清除重合闸状态
                addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
                addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
            }
        }
        if(*(recloseI0Sts->valstr.secondaryRecloseLockflag)&JUDGRFLAG)//二次重合闸闭锁，清除重合闸标志
        {
            recloseI0Sts->valstr.time = 0;
            recloseI0Sts->valstr.flag &= ~(RECLOSEI01|RECLOSEI02);
        }
        if(*(comProSts->yx.recloseClock.value) == ON)
        {
            recloseI0Sts->valstr.flag |= RECLOSEI0LOCK;
        }
        *(recloseI0Sts->valstr.gTime[1]) = 0;
    }
    else if((*(comProSts->yx.switchClose.value) == ON)&&(*(comProSts->yx.switchOpen.value) == OFF))//合位
    {
        *(recloseI0Sts->valstr.gTime[0]) = 0;
        if(recloseI0Sts->valstr.flag&RECLOSEI0STA1)
        {
            recloseI0Sts->valstr.flag &= (~RECLOSEI0STA1);
        }
		if(recloseI0Sts->valstr.flag&RECLOSEI0LOCK)
		{
			recloseI0Sts->valstr.time = 0;//清除重合闸次数
			recloseI0Sts->valstr.flag = 0;//清除重合闸状态
			addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
			addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
		}
        if((recloseI0Sts->valstr.time != 0)&&(*(recloseI0Sts->parastr.pResetTime) != 0))		//次数不为0						
        {
            if(!(*(recloseI0Sts->valstr.gTime[1])&BRE_ENTIMERS))
            {
                *(recloseI0Sts->valstr.gTime[1]) = BRE_ENTIMERS;//启动定时
            }
            if(((*(recloseI0Sts->valstr.gTime[1])&BRE_TITIMERS))>=(uint32_t)(*(recloseI0Sts->parastr.pResetTime)*1000))
            {
                recloseI0Sts->valstr.time = 0;
                recloseI0Sts->valstr.flag &= ~(RECLOSEI01|RECLOSEI02);
            }
        }
        else
        {
            *(recloseI0Sts->valstr.gTime[1]) = 0;
        }
    }
}

/**
  * @Description: 零序重合闸复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void recloseI0_rest(ComProSts *comProSts,RecloseI0Sts *recloseI0Sts)
{
    uint8_t i;

    if(*(recloseI0Sts->valstr.resetflag)&(1<<BRE_RECLOSEI0))//手动复位
    {
        *(recloseI0Sts->valstr.resetflag) &= ~(1<<BRE_RECLOSEI0);		
		recloseI0Sts->valstr.time = 0;
		recloseI0Sts->valstr.flag = 0;
		*(recloseI0Sts->valstr.gTime[0]) = 0;
		for(i=0; i<2; i++)
		{
			addSOE(comProSts,&comProSts->yx.zeroRecloseProtection[recloseI0Sts->valstr.time],OFF);
		}
		addSOE(comProSts,&comProSts->yx.recloseClock,OFF);
		addSOE(comProSts,&comProSts->yx.recloseAct,OFF);
    }
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @Description: 初始化.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void BreakerCtrlInit(void)
{
    uint8_t pdrv;

    for(pdrv=0; pdrv<BRE_DEVMAXNUM; pdrv++)
    {
        switch(pdrv)
        {
        case BRE_DEV0:
            list_init(&s_ListTimers[pdrv]);
            //通用
			s_ComProSts[pdrv].pSwitchType = &g_Parameter[SWITCH_TYPE];
			s_ComProSts[pdrv].pBreakWorkMode = &g_Parameter[BREAK_WORK_MODE];
			s_ComProSts[pdrv].pLoadWorkMode = &g_Parameter[LOAD_WORK_MODE];
            s_ComProSts[pdrv].yx.switchOpen.value = &g_TelesignalDB[g_TelesignalAddr.switchOpen];
            s_ComProSts[pdrv].yx.switchClose.value = &g_TelesignalDB[g_TelesignalAddr.switchClose];
            s_ComProSts[pdrv].yx.recloseHardStrap.value = &g_TelesignalDB[g_TelesignalAddr.recloseFAHardStrap];
            s_ComProSts[pdrv].yx.functionHardStrap.value = &g_TelesignalDB[g_TelesignalAddr.functionHardStrap];
            s_ComProSts[pdrv].yx.telecontrol_Pro_Out.value = &g_TelesignalDB[g_TelesignalAddr.telecontrolProOut];

            s_ComProSts[pdrv].yx.shortCircuitFault.addr = g_TelesignalAddr.shortCircuitFault;
            s_ComProSts[pdrv].yx.earthingFault.addr = g_TelesignalAddr.earthingFault;
            s_ComProSts[pdrv].yx.protectionAct.addr = g_TelesignalAddr.protectionAct;
            s_ComProSts[pdrv].yx.protectionClock.addr = g_TelesignalAddr.protectionLock;
            s_ComProSts[pdrv].yx.recloseAct.addr = g_TelesignalAddr.recloseAct;
            s_ComProSts[pdrv].yx.recloseClock.addr = g_TelesignalAddr.recloseLock;
            s_ComProSts[pdrv].yx.overcurrentAccIa.addr = g_TelesignalAddr.overcurrentAccIa;
            s_ComProSts[pdrv].yx.overcurrentAccIb.addr = g_TelesignalAddr.overcurrentAccIb;
            s_ComProSts[pdrv].yx.overcurrentAccIc.addr = g_TelesignalAddr.overcurrentAccIc;
            s_ComProSts[pdrv].yx.overcurrentInverseIa.addr = g_TelesignalAddr.overcurrentInverseIa;
            s_ComProSts[pdrv].yx.overcurrentInverseIb.addr = g_TelesignalAddr.overcurrentInverseIb;
            s_ComProSts[pdrv].yx.overcurrentInverseIc.addr = g_TelesignalAddr.overcurrentInverseIc;
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
            s_ComProSts[pdrv].yx.recloseProtection[0].addr = g_TelesignalAddr.recloseProtection1;
            s_ComProSts[pdrv].yx.recloseProtection[1].addr = g_TelesignalAddr.recloseProtection2;
            s_ComProSts[pdrv].yx.recloseProtection[2].addr = g_TelesignalAddr.recloseProtection3;
            s_ComProSts[pdrv].yx.recloseProtection[3].addr = g_TelesignalAddr.recloseProtection4;
            s_ComProSts[pdrv].yx.zeroRecloseProtection[0].addr = g_TelesignalAddr.zeroRecloseProtection1;
            s_ComProSts[pdrv].yx.zeroRecloseProtection[1].addr = g_TelesignalAddr.zeroRecloseProtection2;
            s_ComProSts[pdrv].yx.clockSecondaryClose.addr = g_TelesignalAddr.clockSecondaryClose;

            s_ComProSts[pdrv].yx.shortCircuitFault.value = &g_TelesignalDB[g_TelesignalAddr.shortCircuitFault];
            s_ComProSts[pdrv].yx.earthingFault.value = &g_TelesignalDB[g_TelesignalAddr.earthingFault];
            s_ComProSts[pdrv].yx.protectionAct.value = &g_TelesignalDB[g_TelesignalAddr.protectionAct];
            s_ComProSts[pdrv].yx.protectionClock.value = &g_TelesignalDB[g_TelesignalAddr.protectionLock];
            s_ComProSts[pdrv].yx.recloseAct.value = &g_TelesignalDB[g_TelesignalAddr.recloseAct];
            s_ComProSts[pdrv].yx.recloseClock.value = &g_TelesignalDB[g_TelesignalAddr.recloseLock];
            s_ComProSts[pdrv].yx.overcurrentAccIa.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentAccIa];
            s_ComProSts[pdrv].yx.overcurrentAccIb.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentAccIb];
            s_ComProSts[pdrv].yx.overcurrentAccIc.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentAccIc];
            s_ComProSts[pdrv].yx.overcurrentInverseIa.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentInverseIa];
            s_ComProSts[pdrv].yx.overcurrentInverseIb.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentInverseIb];
            s_ComProSts[pdrv].yx.overcurrentInverseIc.value = &g_TelesignalDB[g_TelesignalAddr.overcurrentInverseIc];
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
            s_ComProSts[pdrv].yx.recloseProtection[0].value = &g_TelesignalDB[g_TelesignalAddr.recloseProtection1];
            s_ComProSts[pdrv].yx.recloseProtection[1].value = &g_TelesignalDB[g_TelesignalAddr.recloseProtection2];
            s_ComProSts[pdrv].yx.recloseProtection[2].value = &g_TelesignalDB[g_TelesignalAddr.recloseProtection3];
            s_ComProSts[pdrv].yx.recloseProtection[3].value = &g_TelesignalDB[g_TelesignalAddr.recloseProtection4];
            s_ComProSts[pdrv].yx.zeroRecloseProtection[0].value = &g_TelesignalDB[g_TelesignalAddr.zeroRecloseProtection1];
            s_ComProSts[pdrv].yx.zeroRecloseProtection[1].value = &g_TelesignalDB[g_TelesignalAddr.zeroRecloseProtection2];
            s_ComProSts[pdrv].yx.clockSecondaryClose.value = &g_TelesignalDB[g_TelesignalAddr.clockSecondaryClose];

            s_ComProSts[pdrv].yc.Ia = &g_TelemetryDB[g_TelemetryAddr.Ia];
            s_ComProSts[pdrv].yc.Ib = &g_TelemetryDB[g_TelemetryAddr.Ib];
            s_ComProSts[pdrv].yc.Ic = &g_TelemetryDB[g_TelemetryAddr.Ic];
		 	s_ComProSts[pdrv].yc.Ia2 = &g_secondHarmonicIa;
            s_ComProSts[pdrv].yc.Ib2 = &g_secondHarmonicIb;
            s_ComProSts[pdrv].yc.Ic2 = &g_secondHarmonicIc;
            s_ComProSts[pdrv].yc.I0 = &g_TelemetryDB[g_TelemetryAddr.I0];
            s_ComProSts[pdrv].yc.Uab = &g_TelemetryDB[g_TelemetryAddr.Uab];
            s_ComProSts[pdrv].yc.Uac = &g_TelemetryDB[g_TelemetryAddr.Uac];
            s_ComProSts[pdrv].yc.U0 = &g_TelemetryDB[g_TelemetryAddr.U0];
            
            s_ComProSts[pdrv].fevent_yc_addr[0] = g_TelemetryAddr.Ia;
            s_ComProSts[pdrv].fevent_yc_addr[1] = g_TelemetryAddr.Ib;
            s_ComProSts[pdrv].fevent_yc_addr[2] = g_TelemetryAddr.Ic;
            s_ComProSts[pdrv].fevent_yc_addr[3] = g_TelemetryAddr.I0;
            s_ComProSts[pdrv].fevent_yc_addr[4] = g_TelemetryAddr.Uab;         
            s_ComProSts[pdrv].fevent_yc_addr[6] = g_TelemetryAddr.Uac;
            s_ComProSts[pdrv].fevent_yc_addr[7] = g_TelemetryAddr.U0;

            s_ComProSts[pdrv].opening = &rt_hw_do_operate;
            s_ComProSts[pdrv].closing = &rt_hw_do_operate;
            s_ComProSts[pdrv].outputSoe = &DBWriteSOE;
            s_ComProSts[pdrv].outputFevent = &DBWriteFEVENT;
            
            //复位
            addtimers(pdrv,&s_Rest[pdrv].valstr.gTime);
            s_Rest[pdrv].valstr.flag = 0;
            s_Rest[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.inverseflag = &s_Inverse[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_Rest[pdrv].valstr.secondaryRecloseLockflag = &s_SecondaryRecloseLock[pdrv].valstr.flag;
            s_Rest[pdrv].parastr.pSwitch = &g_pFixedValue[AUTOMATIC_RESET_SWITCH];
            s_Rest[pdrv].parastr.pTime = &g_pFixedValue[AUTOMATIC_RESET_TIME];
            //涌流抑制
            addtimers(pdrv,&s_Inrush[pdrv].valstr.gTime);
			s_Inrush[pdrv].valstr.flag = 0;
            s_Inrush[pdrv].parastr.pSwitch = &g_pFixedValue[INRUSH_SWITCH];
            s_Inrush[pdrv].parastr.pTime = &g_pFixedValue[INRUSH_TIME];
            //后加速
			s_IACC[pdrv].valstr.flag = 0;
            s_IACC[pdrv].valstr.inverseflag = &s_Inverse[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_IACC[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_IACC[pdrv].valstr.gTime[0]);
			addtimers(pdrv,&s_IACC[pdrv].valstr.gTime[1]);
            s_IACC[pdrv].parastr.pSwitch = &g_pFixedValue[IACC_SWITCH];
            s_IACC[pdrv].parastr.pValue = &g_pFixedValue[IACC_CURRENT_VALUE];
            s_IACC[pdrv].parastr.pTime = &g_pFixedValue[IACC_TIME];
            //反时限
			s_Inverse[pdrv].valstr.flag = 0;
            s_Inverse[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_Inverse[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_Inverse[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_Inverse[pdrv].valstr.inrushflag = &s_Inrush[pdrv].valstr.flag;
			s_Inverse[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_Inverse[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_Inverse[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_Inverse[pdrv].valstr.gTime);
            s_Inverse[pdrv].parastr.pSwitch = &g_pFixedValue[INVERSE_SWITCH];
            s_Inverse[pdrv].parastr.pValue = &g_pFixedValue[INVERSE_CURRENT_VALUE];
            s_Inverse[pdrv].parastr.pTime = &g_pFixedValue[INVERSE_TIME];
            //过流
			s_Overcur[pdrv].valstr.flag = 0;
            s_Overcur[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_Overcur[pdrv].valstr.inverseflag = &s_Inverse[pdrv].valstr.flag;
            s_Overcur[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_Overcur[pdrv].valstr.inrushflag = &s_Inrush[pdrv].valstr.flag;
			s_Overcur[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_Overcur[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_Overcur[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_Overcur[pdrv].valstr.gTime[0]);
            addtimers(pdrv,&s_Overcur[pdrv].valstr.gTime[1]);
            addtimers(pdrv,&s_Overcur[pdrv].valstr.gTime[2]);
            s_Overcur[pdrv].parastr.pSwitch[0] = &g_pFixedValue[OVER_CURRENT_SWITCH1];
            s_Overcur[pdrv].parastr.pSwitch[1] = &g_pFixedValue[OVER_CURRENT_SWITCH2];
            s_Overcur[pdrv].parastr.pSwitch[2] = &g_pFixedValue[OVER_CURRENT_SWITCH3];
            s_Overcur[pdrv].parastr.pValue[0] = &g_pFixedValue[OVER_CURRENT_VALUE1];
            s_Overcur[pdrv].parastr.pValue[1] = &g_pFixedValue[OVER_CURRENT_VALUE2];
            s_Overcur[pdrv].parastr.pValue[2] = &g_pFixedValue[OVER_CURRENT_VALUE3];
            s_Overcur[pdrv].parastr.pTime[0] = &g_pFixedValue[OVER_CURRENT_TIME1];
            s_Overcur[pdrv].parastr.pTime[1] = &g_pFixedValue[OVER_CURRENT_TIME2];
            s_Overcur[pdrv].parastr.pTime[2] = &g_pFixedValue[OVER_CURRENT_TIME3];
            //零序过流
			s_OvercurI0[pdrv].valstr.flag = 0;
            s_OvercurI0[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.inverseflag = &s_Inverse[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.inrushflag = &s_Inrush[pdrv].valstr.flag;
			s_OvercurI0[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_OvercurI0[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_OvercurI0[pdrv].valstr.gTime[0]);
            addtimers(pdrv,&s_OvercurI0[pdrv].valstr.gTime[1]);
            s_OvercurI0[pdrv].parastr.pSwitch[0] = &g_pFixedValue[OVER_CURRENTI0_SWITCH1];
            s_OvercurI0[pdrv].parastr.pSwitch[1] = &g_pFixedValue[OVER_CURRENTI0_SWITCH2];
            s_OvercurI0[pdrv].parastr.pValue[0] = &g_pFixedValue[OVER_CURRENTI0_VALUE1];
            s_OvercurI0[pdrv].parastr.pValue[1] = &g_pFixedValue[OVER_CURRENTI0_VALUE2];
            s_OvercurI0[pdrv].parastr.pTime[0] = &g_pFixedValue[OVER_CURRENTI0_TIME1];
            s_OvercurI0[pdrv].parastr.pTime[1] = &g_pFixedValue[OVER_CURRENTI0_TIME2];
            // 二次重合闸闭锁
			s_SecondaryRecloseLock[pdrv].valstr.flag = 0;
			s_SecondaryRecloseLock[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.inverseflag = &s_Inverse[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.recloseflag = &s_Reclose[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.recloseI0flag = &s_RecloseI0[pdrv].valstr.flag;
            s_SecondaryRecloseLock[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_SecondaryRecloseLock[pdrv].valstr.gTime);
            s_SecondaryRecloseLock[pdrv].parastr.pSwitch = &g_pFixedValue[OVER_CURRENTI0_TIME2];
            s_SecondaryRecloseLock[pdrv].parastr.pTime = &g_pFixedValue[SECONDARY_RECLOSE_LOCK_TIME];
            // 过流重合闸
			s_Reclose[pdrv].valstr.flag = 0;
            s_Reclose[pdrv].valstr.secondaryRecloseLockflag = &s_SecondaryRecloseLock[pdrv].valstr.flag;
			s_Reclose[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_Reclose[pdrv].valstr.inverseflag = &s_Inverse[pdrv].valstr.flag;
            s_Reclose[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_Reclose[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_Reclose[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_Reclose[pdrv].valstr.gTime[0]);
			addtimers(pdrv,&s_Reclose[pdrv].valstr.gTime[1]);
            s_Reclose[pdrv].parastr.pTimes = &g_pFixedValue[RECLOSE_TIMES_I];
            s_Reclose[pdrv].parastr.pTime[0] = &g_pFixedValue[RECLOSE_TIMER1];
            s_Reclose[pdrv].parastr.pTime[1] = &g_pFixedValue[RECLOSE_TIMER2];
            s_Reclose[pdrv].parastr.pTime[2] = &g_pFixedValue[RECLOSE_TIMER3];
            s_Reclose[pdrv].parastr.pTime[3] = &g_pFixedValue[RECLOSE_TIMER4];
			s_Reclose[pdrv].parastr.pRechargeTime = &g_pFixedValue[RE_CHARGE_TIME];
			s_Reclose[pdrv].parastr.pResetTime = &g_pFixedValue[RE_RESET_TIME];
            //零序过流重合闸
			s_RecloseI0[pdrv].valstr.flag = 0;
            s_RecloseI0[pdrv].valstr.secondaryRecloseLockflag = &s_SecondaryRecloseLock[pdrv].valstr.flag;
			s_RecloseI0[pdrv].valstr.iACCflag = &s_IACC[pdrv].valstr.flag;
            s_RecloseI0[pdrv].valstr.inverseflag = &s_Inverse[pdrv].valstr.flag;
            s_RecloseI0[pdrv].valstr.overcurflag = &s_Overcur[pdrv].valstr.flag;
            s_RecloseI0[pdrv].valstr.overcurI0flag = &s_OvercurI0[pdrv].valstr.flag;
            s_RecloseI0[pdrv].valstr.resetflag = &s_Rest[pdrv].valstr.flag;
            addtimers(pdrv,&s_RecloseI0[pdrv].valstr.gTime[0]);
			addtimers(pdrv,&s_RecloseI0[pdrv].valstr.gTime[1]);
            s_RecloseI0[pdrv].parastr.pTimes = &g_pFixedValue[RECLOSE_TIMES_I0];
            s_RecloseI0[pdrv].parastr.pTime[0] = &g_pFixedValue[RECLOSE_TIMER1];
            s_RecloseI0[pdrv].parastr.pTime[1] = &g_pFixedValue[RECLOSE_TIMER2];
			s_RecloseI0[pdrv].parastr.pRechargeTime = &g_pFixedValue[RE_CHARGE_TIME];
			s_RecloseI0[pdrv].parastr.pResetTime = &g_pFixedValue[RE_RESET_TIME];
            //初始化清除SOE
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.shortCircuitFault,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.earthingFault,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.protectionAct,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentAccIa,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentAccIb,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentAccIc,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentInverseIa,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentInverseIb,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentInverseIc,OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIa[0],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIb[0],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIc[0],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIa[1],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIb[1],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIc[1],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIa[2],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIb[2],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentIc[2],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentI0[0],OFF);
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.overcurrentI0[1],OFF); 
            addSOE(&s_ComProSts[pdrv],&s_ComProSts[pdrv].yx.clockSecondaryClose,OFF);
            break;
        }
    }
}


/**
  * @Description: 保护程序.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void BreakerCtrlClock(void)
{
    uint8_t pdrv;
	
    for(pdrv=0; pdrv<BRE_DEVMAXNUM; pdrv++)
    {
        add_timers(pdrv);//定时增加
		
		if(*(s_ComProSts[pdrv].pSwitchType) == SWITCH_OFF)
		{
			s_ComProSts[pdrv].WorkMode = *(s_ComProSts[pdrv].pBreakWorkMode);
		}
		else
		{
			s_ComProSts[pdrv].WorkMode = TYPE_BREAKER_NUM + *(s_ComProSts[pdrv].pLoadWorkMode);
		}
        
        switch(pdrv)
        {
			case BRE_DEV0:
                if(g_Parameter[CFG_PRO_VOL_N] == 0)
                {s_ComProSts[pdrv].yc.Ucb = &g_TelemetryDB[g_TelemetryAddr.UAB];}
                else
                {s_ComProSts[pdrv].yc.Ucb = &g_TelemetryDB[g_TelemetryAddr.UCB];}
                
                if(g_Parameter[CFG_PRO_VOL_N] == 0)
                {s_ComProSts[pdrv].fevent_yc_addr[5] = g_TelemetryAddr.UAB;}
                else
                {s_ComProSts[pdrv].fevent_yc_addr[5] = g_TelemetryAddr.UCB;}  
            
				if(s_ComProSts[pdrv].WorkMode == TYPE_BREAKER_COMMON)
				{
					if((*(s_ComProSts[pdrv].yx.functionHardStrap.value)==ON)&&(*(s_ComProSts[pdrv].yx.telecontrol_Pro_Out.value)==OFF))//保护压板
					{
						inrush_ctrl(&s_ComProSts[pdrv],&s_Inrush[pdrv]);//涌流抑制
						iACC_ctrl(&s_ComProSts[pdrv],&s_IACC[pdrv]);//后加速
						inverse_ctrl(&s_ComProSts[pdrv],&s_Inverse[pdrv]);//反时限
						overcur_ctrl(&s_ComProSts[pdrv],&s_Overcur[pdrv]);//过流
						overcurI0_ctrl(&s_ComProSts[pdrv],&s_OvercurI0[pdrv]);//零序过流
						secondaryRecloseLock_ctrl(&s_ComProSts[pdrv],&s_SecondaryRecloseLock[pdrv]);//二次重合闸闭锁
						if(*(s_ComProSts[pdrv].yx.recloseHardStrap.value) == ON)
						{
							reclose_ctrl(&s_ComProSts[pdrv],&s_Reclose[pdrv]);//重合闸
							recloseI0_ctrl(&s_ComProSts[pdrv],&s_RecloseI0[pdrv]);//零序重合闸
						}
					}
				}
				rest_ctrl(pdrv,&s_ComProSts[pdrv],&s_Rest[pdrv]);//复位
				inrush_rest(&s_ComProSts[pdrv],&s_Inrush[pdrv]);//涌流抑制
				iACC_rest(&s_ComProSts[pdrv],&s_IACC[pdrv]);//后加速
				inverse_rest(&s_ComProSts[pdrv],&s_Inverse[pdrv]);//反时限
				overcur_rest(&s_ComProSts[pdrv],&s_Overcur[pdrv]);//过流
				overcurI0_rest(&s_ComProSts[pdrv],&s_OvercurI0[pdrv]);//零序过流
				secondaryRecloseLock_rest(&s_ComProSts[pdrv],&s_SecondaryRecloseLock[pdrv]);//二次重合闸闭锁
				reclose_rest(&s_ComProSts[pdrv],&s_Reclose[pdrv]);//重合闸
				recloseI0_rest(&s_ComProSts[pdrv],&s_RecloseI0[pdrv]);//零序重合闸
            break;
        }
    }
}

/**
  * @Description: 手动复归.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void BreakerCtrlReset(uint8_t pdrv)
{
    s_Rest[pdrv].valstr.flag |= MANRESETFLAG;//复位标志
}

/* END OF FILE ---------------------------------------------------------------*/

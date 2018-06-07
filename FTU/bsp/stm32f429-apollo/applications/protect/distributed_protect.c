/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distributed_protect.c
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    J.Lee
  * @date:      2018.03.30
  */


/* INCLUDES ------------------------------------------------------------------*/
#include "distributed_protect.h"
#include "distributed_interface.h"
#include ".\MultiThread\multi_thread.h"
#include "string.h"

uint8_t g_dataHaveChange;
//test
static uint8_t Sdev[][20] = {{1,5},//1
                            {2,5},//2
                            {3,5},//3
                            {4,9},//4
                            {5,5}};//5
static uint8_t Mdev[][20] = {{0},//1
                            {1, 1,5},//2
                            {1, 2,5},//3
                            {1, 3,5},//4
                            {1, 4,9}};//5
static uint8_t Ndev[][20] = {{1, 2,5},//1
                            {1, 3,5},//2
                            {1, 4,9},//3
                            {1, 5,5},//4
                            {0}};//5
static uint8_t Supplydev[][40] = {{1, 4, 2,5,3,5,4,9,5,5},//1
                                {0},//2
                                {0},//3
                                {0},//4
                                {1, 4, 4,9,3,5,2,5,1,5}};//5
//static uint8_t Sdev[][20] = {{1,5},//1
//                            {2,9},//2
//                            {3,5},//3
//                            {4,4},//4
//                            {5,9},//5
//                            {6,5},//6
//                            {7,5},//7
//                            {8,6},//8
//                            {9,6},//9
//                            {10,6}};//10
//static uint8_t Mdev[][20] = {{0},//1
//                            {2, 1,5, 8,6},//2
//                            {1, 2,9},//3
//                            {3, 3,5, 5,9, 9,6},//4
//                            {3, 3,5, 4,5, 9,6},//5
//                            {1, 5,9},//6
//                            {2, 6,5, 10,6},//7
//                            {2, 1,5, 2,9},//8
//                            {3, 3,5, 4,5, 5,9},//9
//                            {2, 6,5, 7,5}};//10
//static uint8_t Ndev[][20] = {{2, 2,9, 8,6},//1
//                            {1, 3,5},//2
//                            {4, 4,5, 5,9, 9,6},//3
//                            {0},//4
//                            {1, 6,5},//5
//                            {2, 7,5, 10,6},//6
//                            {0},//7
//                            {0},//8
//                            {0},//9
//                            {0}};//10
//static uint8_t Supplydev[][40] = {{1, 4, 1,5,2,9,3,5,4,5},//1
//                                {0},//2
//                                {0},//3
//                                {2, 4, 4,5,3,5,2,9,1,5,  4, 4,5,5,9,6,5,7,5},//4
//                                {0},//5
//                                {0},//6
//                                {1, 4, 7,5,6,5,5,9,4,5},//7
//                                {0},//8
//                                {0},//9
//                                {0}};//10
/* PUBLIC VARIABLES --------------------------------------------------------*/
DevStr g_SelfSts[DISTRIBUT_DEVMAXNUM];

/* PRIVATE VARIABLES --------------------------------------------------------*/
static uint32_t s_SelfFlag[DISTRIBUT_DEVMAXNUM];
static DevStr s_SelfSts[DISTRIBUT_DEVMAXNUM];
static StoreQueueMemberStr s_StoreQueueMemberStr[DISTRIBUT_DEVMAXNUM][256];
static ComProSts s_ComProSts[DISTRIBUT_DEVMAXNUM];
static FunctionalRetreatSts s_FunctionalRetreat[DISTRIBUT_DEVMAXNUM];
static SelfstateJudgeSts s_SelfstateJudge[DISTRIBUT_DEVMAXNUM];
static FaultIsolationSts s_FaultIsolation[DISTRIBUT_DEVMAXNUM];
static List s_ListTimers[DISTRIBUT_DEVMAXNUM];//定时器链表链表
static List s_ListDevStorage[DISTRIBUT_DEVMAXNUM];//设备仓库链表
static List s_ListDevM[DISTRIBUT_DEVMAXNUM];//M侧设备链表
static List s_ListDevN[DISTRIBUT_DEVMAXNUM];//N侧设备链表
static List s_ListDevSupply[DISTRIBUT_DEVMAXNUM];//转供电链表

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
void add_timers(uint8_t pdrv)
{
    ListElmt *element;
    
    element = s_ListTimers[pdrv].head;
    while(element != NULL)
    {
        if((*((uint32_t *)(element->data)))&DISTRIBUT_ENTIMERS)
        {
            (*((uint32_t *)(element->data)))++;
        }
        element = element->next;
    }
}

/**
  * @Description: 智能分布式投退.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void functional_retreat(uint8_t pdrv)
{
    uint32_t communicatState = 0;
    ListElmt *element;
    DevStr *dev;
    
//    variableState[pdrv] &= ~_DISTRIBUT_V_CLEAN_COMMUNICAT_EXIT;
    //投退开关引用
    element = s_ListDevStorage[pdrv].head;
    while(element != NULL)
    {
        dev = (DevStr *)(element->data);
        if(((*(dev->gTime))&DISTRIBUT_TITIMERS) > 10000)//10秒未收到此ip传递的信息
        {
            *(dev->gTime) &= ~DISTRIBUT_ENTIMERS;
            
            communicatState |= _DISTRIBUT_V_COMMUNICAT_FAULT_SELF;
        }
        if((dev->comstr.variableState)&_DISTRIBUT_V_CLEAN_COMMUNICAT_EXIT)
        {
            if(!(*(s_FunctionalRetreat->valstr.gTime)&DISTRIBUT_ENTIMERS))//未启动定时
            {
                *(s_FunctionalRetreat->valstr.gTime) = DISTRIBUT_ENTIMERS;//启动定时
                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_CLEAN_COMMUNICAT_EXIT;
            }
        }
        if(((dev->comstr.operation&_DISTRIBUT_O_IP) == s_SelfSts[pdrv].ip)&&(!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_SPACER)||\
                        (s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE))))//无拒动//无隔离
        {
            if((dev->comstr.operation&_DISTRIBUT_O_SWTICHOPEN)&&(*(s_ComProSts[pdrv].yx.switchOpen.value) == OFF)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == ON))//合位
            {
                if(!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)))
                {
                    s_ComProSts[pdrv].opening(ADDR_LOGIC_ACT,DO_OPEN);//控分
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_ACT;
                }
            }
            else if((dev->comstr.operation&_DISTRIBUT_O_SWTICHCLOSE)&&(*(s_ComProSts[pdrv].yx.switchOpen.value) == ON)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == OFF))//分位
            {
                if(!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)))
                {
                    s_ComProSts[pdrv].closing(ADDR_LOGIC_ACT,DO_CLOSE);//控合
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_ACT;
                }
            }           
        }
        if(dev->ip == (s_SelfSts[pdrv].comstr.operation&_DISTRIBUT_O_IP))
        {
            if(((dev->comstr.variableState&_DISTRIBUT_V_REQUIRED_SPACER)||(dev->comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE))||\
                ((s_SelfSts[pdrv].comstr.operation&_DISTRIBUT_O_SWTICHOPEN)&&(dev->comstr.variableState&_DISTRIBUT_V_SWTICHOPEN))||\
                ((s_SelfSts[pdrv].comstr.operation&_DISTRIBUT_O_SWTICHCLOSE)&&(dev->comstr.variableState&_DISTRIBUT_V_SWTICHCLOSE)))
                //拒动//隔离//分位控分//合位控合
            {
                s_SelfSts[pdrv].comstr.operation = 0;
            }              
        }
        element = element->next;
    }
    
    if(*(s_FunctionalRetreat->valstr.gTime)&DISTRIBUT_ENTIMERS)
    {
        s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_COMMUNICAT_FAULT_OTHER;
        s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_EXIT_OTHER;
        if((*(s_FunctionalRetreat->valstr.gTime)&DISTRIBUT_TITIMERS)> 10000)
        {
            *(s_FunctionalRetreat->valstr.gTime) = 0;
        }
        else if((*(s_FunctionalRetreat->valstr.gTime)&DISTRIBUT_TITIMERS)>2000)
        {
            s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_CLEAN_COMMUNICAT_EXIT;
        }
    }
    else
    {
        if((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_COMMUNICAT_FAULT_SELF)&&(!(communicatState&_DISTRIBUT_V_COMMUNICAT_FAULT_SELF)))//通讯由无到有
        {
                *(s_FunctionalRetreat->valstr.gTime) = DISTRIBUT_ENTIMERS;//启动定时
                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_CLEAN_COMMUNICAT_EXIT;   
                s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_COMMUNICAT_FAULT_SELF;           
        }
        if((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_EXIT_SELF)&&(!(g_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_EXIT_SELF)))//投退开关由无到有
        {
                *(s_FunctionalRetreat->valstr.gTime) = DISTRIBUT_ENTIMERS;//启动定时
                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_CLEAN_COMMUNICAT_EXIT;
                s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_EXIT_SELF;             
        }
        
        s_SelfSts[pdrv].comstr.variableState |= communicatState;
        
        element = s_ListDevStorage[pdrv].head;
        while(element != NULL)
        {
            dev = (DevStr *)(element->data);
            if((dev->comstr.variableState)&(_DISTRIBUT_V_COMMUNICAT_FAULT_SELF|_DISTRIBUT_V_COMMUNICAT_FAULT_OTHER))
            {
                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_COMMUNICAT_FAULT_OTHER;             
            }
            if((dev->comstr.variableState)&(_DISTRIBUT_V_EXIT_SELF|_DISTRIBUT_V_EXIT_OTHER))
            {
                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_EXIT_OTHER;             
            }
            element = element->next;
        }        
    }       
}

/**
  * @Description: 自身状态判断.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void selfstate_judge(uint8_t pdrv)
{    
    const float factor = 0.9;
    
    if(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)
    {
        if(!((s_SelfFlag[pdrv]&_DISTRIBUT_FLAG_SWTICHOPEN)||(s_SelfFlag[pdrv]&_DISTRIBUT_FLAG_SWTICHCLOSE)))//记录变化前状态
        {
            if((*(s_ComProSts[pdrv].yx.switchOpen.value) == OFF)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == ON))//合位
            {
                s_SelfFlag[pdrv] |= _DISTRIBUT_FLAG_SWTICHCLOSE;
            }
            else if((*(s_ComProSts[pdrv].yx.switchOpen.value) == ON)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == OFF))//分位
            {
                s_SelfFlag[pdrv] |= _DISTRIBUT_FLAG_SWTICHOPEN;
            }
        }
        else
        {
            if(!(*(s_SelfstateJudge->valstr.gTime)&DISTRIBUT_ENTIMERS))
            {
                *(s_SelfstateJudge->valstr.gTime) = DISTRIBUT_ENTIMERS;//启动定时
            }
            if((*(s_SelfstateJudge->valstr.gTime)&DISTRIBUT_TITIMERS) > 100)//超过100ms拒动
            {
                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_FAILURE_OPERATE;
                s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_REQUIRED_ACT;
            }
            if(((s_SelfFlag[pdrv]&_DISTRIBUT_FLAG_SWTICHCLOSE)&&((*(s_ComProSts[pdrv].yx.switchOpen.value) == ON)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == OFF)))||\
               ((s_SelfFlag[pdrv]&_DISTRIBUT_FLAG_SWTICHOPEN)&&((*(s_ComProSts[pdrv].yx.switchOpen.value) == OFF)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == ON)))) //规定时间内正常动作
            {
                s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_REQUIRED_ACT;
                if(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_SPACER)
                {
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_FINISH_SPACER;
                }
            }
        }
    }
    else
    {
        s_SelfFlag[pdrv] &= ~_DISTRIBUT_FLAG_SWTICHCLOSE;
        s_SelfFlag[pdrv] &= ~_DISTRIBUT_FLAG_SWTICHOPEN;
        *(s_SelfstateJudge->valstr.gTime) = 0;
    }
      

    if((*(s_ComProSts[pdrv].yc.Ia)> (float)0.3)||(*(s_ComProSts[pdrv].yc.Ib)> (float)0.3)||(*(s_ComProSts[pdrv].yc.Ic)> (float)0.3))//过流
    {
        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_OVERCURRENT;
    }
    else if((*(s_ComProSts[pdrv].yc.Ia)< (float)0.3*factor)&&(*(s_ComProSts[pdrv].yc.Ib)< (float)0.3*factor)&&(*(s_ComProSts[pdrv].yc.Ic)< (float)0.3*factor))
    {
        s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_OVERCURRENT;
    }
    
    if(*(s_ComProSts[pdrv].yc.Uab)> 20)//M侧有压
    {
        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_VOLTAGE_M;
    }
    else if(*(s_ComProSts[pdrv].yc.Uab)< 20*factor)
    {
        s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_VOLTAGE_M;
    }
    if(*(s_ComProSts[pdrv].yc.Ubc)> 20)//N侧有压
    {
        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_VOLTAGE_N;
    }
    else if(*(s_ComProSts[pdrv].yc.Ubc)< 20*factor)
    {
        s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_VOLTAGE_N;
    }
           
    if((*(s_ComProSts[pdrv].yx.switchOpen.value) == OFF)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == ON))//合位
    {
        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_SWTICHCLOSE;
        s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_SWTICHOPEN;        
    }
    else if((*(s_ComProSts[pdrv].yx.switchOpen.value) == ON)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == OFF))//分位
    {
        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_SWTICHOPEN;
        s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_SWTICHCLOSE;
    } 
}
    
/**
  * @Description: 方向判断.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void direction_judge(uint8_t pdrv)
{
    ListElmt *element;
//    static uint8_t sta[DISTRIBUT_DEVMAXNUM] = {0};
    
    if(((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_M)&&(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_N))
        &&((*(s_ComProSts[pdrv].yx.switchOpen.value) == OFF)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == ON)))//合位//两侧有压
    {
        if(!(s_SelfFlag[pdrv]&_DISTRIBUT_FLAG_DOUBLEVOLTAGE))
        {
            s_SelfFlag[pdrv] |= _DISTRIBUT_FLAG_DOUBLEVOLTAGE;
            s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_POSITIVE;
            s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_BACKWARD;
        }
        if(!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_POSITIVE)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_BACKWARD)))//无方向
        {
			//M侧
            element = s_ListDevM[pdrv].head;
			if(element == NULL)//M侧无设备链接
			{
				if(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_TRUNK)//主线
				{
					s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_POSITIVE;
					return;
				}
				else if(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_BRANCH)//支线
				{
					s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_BACKWARD;
					return;
				}
			}
            while(element != NULL)
            {
                if((((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_POSITIVE)&&\
                    ((!(((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_SWTICHOPEN))&&\
                (((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_SWTICHCLOSE)))
                {
					s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_POSITIVE;
					return;
                }
                element = element->next; 
            }
			
			//N侧
			element = s_ListDevN[pdrv].head;
			if(element == NULL)//N侧无设备链接
			{
				if(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_TRUNK)//主线
				{
					s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_BACKWARD;
					return;
				}
				else if(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_BRANCH)//支线
				{
					s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_POSITIVE;
					return;
				}
			}
            while(element != NULL)
            {
                if((((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_BACKWARD)&&\
                    ((!(((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_SWTICHOPEN))&&\
                (((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_SWTICHCLOSE)))
                {
					s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_BACKWARD;
					return;
                }
                element = element->next; 
            }
        }
    }
    else//两侧无压清除状态
    {
        s_SelfFlag[pdrv] &= ~_DISTRIBUT_FLAG_DOUBLEVOLTAGE;
    }
}

/**
  * @Description: 故障隔离
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void fault_isolation(uint8_t pdrv)
{
    ListElmt *element;
    
    if((*(s_ComProSts[pdrv].yx.switchOpen.value) == OFF)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == ON))//合位
    {
        if(s_SelfFlag[pdrv]&_DISTRIBUT_FLAG_OVERCURRENT_ACT)
        {
            s_SelfSts[pdrv].comstr.variableState &= ~_DISTRIBUT_V_OVERCURRENT_ACT;//清除过流动作标志
            s_SelfFlag[pdrv] &= ~_DISTRIBUT_FLAG_OVERCURRENT_ACT;
        }
        
        if(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_OVERCURRENT)//自身过流
        {
            if(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_BRANCH)//支线
            {
                if(!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)))
                {
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_ACT;
                    s_ComProSts[pdrv].opening(ADDR_LOGIC_ACT,DO_OPEN);//跳闸      
                }
            }
            else if(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_TRUNK)//主线
            {
                if(!(*(s_FaultIsolation->valstr.gTime)&DISTRIBUT_ENTIMERS))
                {
                    *(s_FaultIsolation->valstr.gTime) = DISTRIBUT_ENTIMERS;//启动定时
                }
                if((*(s_FaultIsolation->valstr.gTime)&DISTRIBUT_TITIMERS) > 10)//过流20ms
                {                 
                    if(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_POSITIVE)//正向
                    {
                        //N侧
                        element = s_ListDevN[pdrv].head;
                        while(element != NULL)
                        {
                            if(((((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_POSITIVE)&&\
                                (((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_OVERCURRENT)))//正向过流
                            {
                                if(((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)//拒动
                                {
                                    if(!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)))
                                    {
                                        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_ACT;
                                        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_SPACER;
                                        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_OVERCURRENT_ACT;
                                        s_ComProSts[pdrv].opening(ADDR_LOGIC_ACT,DO_OPEN);//跳闸
                                    }
                                }
                                s_SelfFlag[pdrv] |= _DISTRIBUT_FLAG_FLAUT;//检测出故障点
                                return;
                            }
                            element = element->next; 
                        }
                        if((!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)))&&\
                            (!(s_SelfFlag[pdrv]&_DISTRIBUT_FLAG_FLAUT)))//自身过流，前端未检测出故障点
                        {
                            s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_ACT;//无正向过流
                            s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_SPACER;
                            s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_OVERCURRENT_ACT;
                            s_ComProSts[pdrv].opening(ADDR_LOGIC_ACT,DO_OPEN);//跳闸
                        }
                        return;
                    }
                    if(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_BACKWARD)//反向
                    {
                        //M侧
                        element = s_ListDevM[pdrv].head;
                        while(element != NULL)
                        {
                            if(((((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_BACKWARD)&&\
                                (((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_OVERCURRENT)))//反向过流
                            {
                                if(((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)//拒动
                                {
                                    if(!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)))
                                    {
                                        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_ACT;
                                        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_SPACER;
                                        s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_OVERCURRENT_ACT;
                                        s_ComProSts[pdrv].opening(ADDR_LOGIC_ACT,DO_OPEN);//跳闸
                                    }
                                }
                                s_SelfFlag[pdrv] |= _DISTRIBUT_FLAG_FLAUT;//检测出故障点
                                return;
                            }
                            element = element->next; 
                        }
                        if((!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)))&&\
                            (!(s_SelfFlag[pdrv]&_DISTRIBUT_FLAG_FLAUT)))//自身过流，前端未检测出故障点
                        {
                            s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_ACT;//无反向过流
                            s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_SPACER;
                            s_ComProSts[pdrv].opening(ADDR_LOGIC_ACT,DO_OPEN);//跳闸
                            s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_OVERCURRENT_ACT;
                        }
                        return;
                    }
                }
            }
        }
        else
        {     
            *(s_FaultIsolation->valstr.gTime) = 0; 
            s_SelfFlag[pdrv] &= ~_DISTRIBUT_FLAG_FLAUT;             
            if(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_TRUNK)//主线
            {           
                //故障发生在上流侧，跳闸
                if(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_POSITIVE)//正向
                {
                    //M侧
                    element = s_ListDevM[pdrv].head;
                    while(element != NULL)
                    {
                        if((((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_POSITIVE)&&\
                            (((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_OVERCURRENT_ACT)&&\
                            (!(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_M)))//上端正向过流，上端失压
                        {
                            if(!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)))
                            {
                                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_ACT;
                                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_SPACER;
                                s_ComProSts[pdrv].opening(ADDR_LOGIC_ACT,DO_OPEN);//跳闸
                            }
                            return;
                        }
                        element = element->next; 
                    }
                    return;
                }
                if(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_BACKWARD)//反向
                {
                    //N侧
                    element = s_ListDevN[pdrv].head;
                    while(element != NULL)
                    {
                        if((((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_BACKWARD)&&\
                            (((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_OVERCURRENT_ACT)&&\
                            (!(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_N)))//上端反向过流，上端失压
                        {
                            if(!((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_REQUIRED_ACT)||(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE)))
                            {
                                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_ACT;
                                s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_SPACER;
                                s_ComProSts[pdrv].opening(ADDR_LOGIC_ACT,DO_OPEN);//跳闸
                            }
                            return;
                        }
                        element = element->next; 
                    }
                    return;
                }
            }
        }
    }
    else if((*(s_ComProSts[pdrv].yx.switchOpen.value) == ON)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == OFF))//分位
    {
        *(s_FaultIsolation->valstr.gTime) = 0;
        s_SelfFlag[pdrv] &= ~_DISTRIBUT_FLAG_FLAUT;
        if(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_OVERCURRENT_ACT)//因过流跳闸，置FLAG
        {
            s_SelfFlag[pdrv] |= _DISTRIBUT_FLAG_OVERCURRENT_ACT;
        }
        if(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_TRUNK)//主线
        {
            //M侧
            element = s_ListDevM[pdrv].head;
            while(element != NULL)
            {
                if((((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_POSITIVE)&&\
                    (((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_OVERCURRENT_ACT)&&\
                    (!(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_M)))//上端正向过流，上端失压
                {
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_SPACER;
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_FINISH_SPACER;
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_BLOCKING;
                    return;
                }
                element = element->next; 
            }
            //N侧
            element = s_ListDevN[pdrv].head;
            while(element != NULL)
            {
                if((((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_BACKWARD)&&\
                    (((DevStr *)(element->data))->comstr.variableState&_DISTRIBUT_V_OVERCURRENT_ACT)&&\
                    (!(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_N)))//上端反向过流，上端失压
                {
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_REQUIRED_SPACER;
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_FINISH_SPACER;
                    s_SelfSts[pdrv].comstr.variableState |= _DISTRIBUT_V_BLOCKING;
                    return;
                }
                element = element->next; 
            }
        }        
    }
    
}
    
/**
  * @Description: 转供电.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void change_power(uint8_t pdrv)
{
    ListElmt *elementfirst;//主链表
    ListElmt *elementSecond;//辅链表
    uint8_t ip=0;    
    
    if(((*(s_ComProSts[pdrv].yx.switchOpen.value) == OFF)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == ON))&&\
        ((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_M)&&(s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_N))&&\
        ((s_ListDevSupply[pdrv].head != NULL)&&(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_TRUNK)))//开关合位//双侧有压有链表//主线
    {
//        if(((s_ListDevM[pdrv].head != NULL)&&(!(s_SelfSts[pdrv].variableState&_DISTRIBUT_V_VOLTAGE_M)))||\
//          ((s_ListDevN[pdrv].head != NULL)&&(!(s_SelfSts[pdrv].variableState&_DISTRIBUT_V_VOLTAGE_N))))  //M侧有设备无压//N侧有设备无压
        {
            //加延时
            
            elementfirst = s_ListDevSupply[pdrv].head;
            
            while(elementfirst != NULL)
            {
                if(1)//判断负荷,允许
                {
                    elementSecond = ((List *)(elementfirst->data))->head;
                    
                    while(elementSecond != NULL)
                    {
                        if(ip)
                        {
                            if((!((((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_VOLTAGE_M)||\
                                (((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_VOLTAGE_N)))&&\
                                ((((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHOPEN)&&\
                                (!(((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHCLOSE)))&&\
                                (((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_REQUIRED_SPACER)&&\
                                (!(((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_OVERCURRENT_ACT)))//联络后部无压且分位且隔离且不因过流跳闸
                            {
                                if(s_SelfSts[pdrv].comstr.operation == 0)//控制其他设备//控合
                                {
                                    s_SelfSts[pdrv].comstr.operation = ip;
                                    s_SelfSts[pdrv].comstr.operation |= _DISTRIBUT_O_SWTICHCLOSE;
                                    return;
                                }
                            }
                        }
                            
                        if(((((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_REQUIRED_SPACER)||\
                            (((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE))||\
                            (s_SelfSts[pdrv].comstr.operation != 0))//拒动//隔离//有控制操作
                        {  
                            return;
                        }
                        else
                        {
                            if((((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHOPEN)&&\
                              (!(((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHCLOSE))) //分位
                            {
                                ip = ((DevStr *)(elementSecond->data))->ip;
                            }
                        }
                        elementSecond = elementSecond->next;  
                    }
                }
                elementfirst = elementfirst->next;
            }
        }
    }
}
 
/**
  * @Description: 自愈.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void self_healing(uint8_t pdrv)
{    
    ListElmt *elementfirst;//主链表
    ListElmt *elementSecond;//辅链表
    
    if(((*(s_ComProSts[pdrv].yx.switchOpen.value) == ON)&&(*(s_ComProSts[pdrv].yx.switchClose.value) == OFF))&&\
        ((s_ListDevSupply[pdrv].head != NULL)&&(s_SelfSts[pdrv].steadyState&_DISTRIBUT_S_TRUNK)))//有链表//主线
    {
        if(((s_ListDevM[pdrv].head == NULL)&&((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_M)))||\
          ((s_ListDevN[pdrv].head == NULL)&&((s_SelfSts[pdrv].comstr.variableState&_DISTRIBUT_V_VOLTAGE_N))))  //M侧无设备有压//N侧无设备有压
        {
            //检查所有供电线路故障是否排除
            elementfirst = s_ListDevSupply[pdrv].head;
            
            while(elementfirst != NULL)
            {
                if((!(((DevStr *)((((List *)(elementfirst->data))->tail)->data))->comstr.variableState&_DISTRIBUT_V_SWTICHCLOSE))&&\
                    (((DevStr *)((((List *)(elementfirst->data))->tail)->data))->comstr.variableState&_DISTRIBUT_V_SWTICHOPEN))//终点分位
                {
                    return;
                }
                    
                elementSecond = ((List *)(elementfirst->data))->head;
                
                while(elementSecond != NULL)
                {
                    if((((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_REQUIRED_SPACER)||\
                        (((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_FAILURE_OPERATE))//拒动//隔离
                    {  
                        return;
                    }
                    elementSecond = elementSecond->next;  
                }
                elementfirst = elementfirst->next;
            }
            
            //所有线路故障排除，恢复供电，负荷开关分
            elementfirst = s_ListDevSupply[pdrv].head;
            
            while(elementfirst != NULL)
            {
                elementSecond = ((List *)(elementfirst->data))->head;
                
                while(elementSecond != NULL)
                {
                    if(((DevStr *)(elementSecond->data))->steadyState&_DISTRIBUT_S_LOADSWTICH)
                    {  
                        if(((((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHCLOSE))&&\
                            (!(((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHOPEN)))
                        {
                            s_SelfSts[pdrv].comstr.operation = 0;
                            s_SelfSts[pdrv].comstr.operation |= _DISTRIBUT_O_SWTICHOPEN;//控制其他//控分
                            s_SelfSts[pdrv].comstr.operation |= ((DevStr *)(elementSecond->data))->ip;
                            return;
                        }
                    }
                    elementSecond = elementSecond->next;  
                }
                elementfirst = elementfirst->next;
            } 

            //所有线路故障排除，恢复供电，检查所有负荷开关是否分位
            elementfirst = s_ListDevSupply[pdrv].head;
            
            while(elementfirst != NULL)
            {
                elementSecond = ((List *)(elementfirst->data))->head;
                
                while(elementSecond != NULL)
                {
                    if(((DevStr *)(elementSecond->data))->steadyState&_DISTRIBUT_S_LOADSWTICH)
                    {  
                        if((!(((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHCLOSE))&&\
                            ((((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHOPEN)))
                        {
                            return;
                        }
                    }
                    elementSecond = elementSecond->next;  
                }
                elementfirst = elementfirst->next;
            } 
            
            //所有线路故障排除，恢复供电，断路器合
            elementfirst = s_ListDevSupply[pdrv].head;
            
            while(elementfirst != NULL)
            {
                elementSecond = ((List *)(elementfirst->data))->head;
                
                elementSecond = elementSecond->next; //排除自己
                
                while(elementSecond != NULL)
                {
                    if(((DevStr *)(elementSecond->data))->steadyState&_DISTRIBUT_S_BREAKER)
                    {  
                        if((!(((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHCLOSE))&&\
                            ((((DevStr *)(elementSecond->data))->comstr.variableState&_DISTRIBUT_V_SWTICHOPEN)))
                        {
                            s_SelfSts[pdrv].comstr.operation = 0;
                            s_SelfSts[pdrv].comstr.operation |= _DISTRIBUT_O_SWTICHCLOSE;//控制其他//控合
                            s_SelfSts[pdrv].comstr.operation |= ((DevStr *)(elementSecond->data))->ip;
                            return;
                        }
                    }
                    elementSecond = elementSecond->next;  
                }
                elementfirst = elementfirst->next;
            }  
            s_ComProSts[pdrv].closing(ADDR_LOGIC_ACT,DO_CLOSE);//控合
        }
    }
}

/**
  * @Description: 存入设备仓库.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
DevStr *ListDevStorage(uint8_t pdrv,uint8_t ip,uint32_t steadyState)
{
    ListElmt *element;
    DevStr *dev;
    
    //存入设备仓库
    element = s_ListDevStorage[pdrv].head;
    while(element != NULL)
    {
        if(((DevStr *)(element->data))->ip == ip)
        {
            return((DevStr *)(element->data));
        }
        element = element->next;
    }
    if(element == NULL)
    {
        dev = rt_malloc(sizeof(DevStr));
        dev->ip = ip;
        dev->steadyState = steadyState;   
        dev->comstr.variableState = 0;
        dev->comstr.operation = 0;
        addtimers(pdrv,&dev->gTime);
        *(dev->gTime) = DISTRIBUT_ENTIMERS;
        list_ins_next(&s_ListDevStorage[pdrv],NULL,dev);
    }
    return(dev);
}
 
/**
  * @Description: 状态转换.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint32_t steadyStateChange(uint8_t pdrv,uint8_t sta1,uint8_t sta2)
{
    uint32_t State = 0;
    
    if(sta1)
    {State |= _DISTRIBUT_S_BREAKER;}
    else
    {State |= _DISTRIBUT_S_LOADSWTICH;}
    if(sta2)
    {State |= _DISTRIBUT_S_TRUNK;}
    else
    {State |= _DISTRIBUT_S_BRANCH;}
    
    return(State);
}

/**
  * @Description: 链表初始化.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void ListInit(uint8_t pdrv)
{
    uint16_t iS,iM,iN,iL,jL,tempS,tempM,tempN,tempLi,tempLj;
    DevStr *dev;
    List *list;
    uint8_t ip;
    uint32_t steadyState;
    
    list_init(&s_ListTimers[pdrv]);
    list_init(&s_ListDevStorage[pdrv]);
    list_init(&s_ListDevM[pdrv]);
    list_init(&s_ListDevN[pdrv]);
    list_init(&s_ListDevSupply[pdrv]);       
    
    s_SelfSts[pdrv].ip = g_Parameter[NET_IP2_3];
    s_SelfSts[pdrv].steadyState = 0;
    addtimers(pdrv,&s_SelfSts[pdrv].gTime);
    for(iS=0,tempS=1;iS<GridStructureSet[0];iS++)
    {
        if(GridStructureSet[tempS + 2] == s_SelfSts[pdrv].ip)
        {
            s_SelfSts[pdrv].steadyState = steadyStateChange(pdrv,GridStructureSet[tempS + 3],GridStructureSet[tempS + 4]);
            for(iM=0,tempM=tempS+6;iM<GridStructureSet[tempS + 5];iM++)
            {
                ip = GridStructureSet[tempM];
                steadyState = steadyStateChange(pdrv,GridStructureSet[tempM + 1],GridStructureSet[tempM + 2]);
                dev = ListDevStorage(pdrv,ip,steadyState);
                list_ins_next(&s_ListDevM[pdrv],NULL,dev);
                tempM += 3;
            }
            for(iN=0,tempN=tempM+1;iN<GridStructureSet[tempM];iN++)
            {
                ip = GridStructureSet[tempN];
                steadyState = steadyStateChange(pdrv,GridStructureSet[tempN + 1],GridStructureSet[tempN + 2]);
                dev = ListDevStorage(pdrv,ip,steadyState);
                list_ins_next(&s_ListDevN[pdrv],NULL,dev);
                tempN += 3;            
            }
            for(iL=0,tempLi=tempN+1;iL<GridStructureSet[tempN];iL++)//供电链表添加
            {
                list = rt_malloc(sizeof(List));
                list_init(list); 
                list_ins_next(&s_ListDevSupply[pdrv],NULL,list);
                for(jL=0,tempLj=tempLi+1;jL<GridStructureSet[tempLi];jL++)
                {
                    ip = GridStructureSet[tempLj];
                    steadyState = steadyStateChange(pdrv,GridStructureSet[tempLj + 1],GridStructureSet[tempLj + 2]);
                    dev = ListDevStorage(pdrv,ip,steadyState);
                    list_ins_next(list,list->tail,dev);
                    tempLj += 3; 
                }
                tempLi = tempLj;
            } 
            break;       
        }
        tempS += *(uint16_t *)&GridStructureSet[tempS] + 2;
    }  
}

/**
  * @Description: 更新自身状态.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void distributUpdataSelfState(uint8_t pdrv)   
{
    if((g_SelfSts[pdrv].comstr.variableState != s_SelfSts[pdrv].comstr.variableState))
    {
        g_SelfSts[pdrv].comstr.variableState = s_SelfSts[pdrv].comstr.variableState;
        //如果不一样发送事件  
        rt_event_send(&w5500_event, EVENT_GOOSE_HAVE_CHANGE);
    }
    memcpy(&g_SelfSts[pdrv],&s_SelfSts[pdrv],sizeof(s_SelfSts[pdrv]));    
}
/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @Description: 更新数组.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void distributUpdataQueue(uint8_t pdrv,uint8_t ip,uint32_t *pdata)   
{
    s_StoreQueueMemberStr[pdrv][ip].getflag = ON;
    memcpy(&s_StoreQueueMemberStr[pdrv][ip].comstr,pdata,sizeof(s_StoreQueueMemberStr[pdrv][ip].comstr));
//    s_SstoreQueue[pdrv].storeIn->ip = ip;
//    memcpy(&s_SstoreQueue[pdrv].storeIn->comstr,pdata,sizeof(ComStr));
//    if(++s_SstoreQueue[pdrv].storeIn >= s_SstoreQueue[pdrv].store + _DISTRIBUT_STOREQUEUE_SIZE)
//    {
//        s_SstoreQueue[pdrv].storeIn = s_SstoreQueue[pdrv].store;
//    }    
}

/**
  * @Description: 更新仓库.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void distributUpdataDevStorage(uint8_t pdrv)
{   
    ListElmt *element;
    DevStr *dev;
         
    element = s_ListDevStorage[pdrv].head;
    while(element != NULL)
    {
        dev = (DevStr *)(element->data);
        if(s_StoreQueueMemberStr[pdrv][dev->ip].getflag == ON)//ip符合
        {
            s_StoreQueueMemberStr[pdrv][dev->ip].getflag = OFF;
            *(dev->gTime) = DISTRIBUT_ENTIMERS;//定时清零
            dev->comstr.variableState = s_StoreQueueMemberStr[pdrv][dev->ip].comstr.variableState;//赋值
            dev->comstr.operation = s_StoreQueueMemberStr[pdrv][dev->ip].comstr.operation;//赋值
        }
        element = element->next;
    }          
    
//    while(s_SstoreQueue[pdrv].storeIn != s_SstoreQueue[pdrv].storeOut)
//    {        
//        element = s_ListDevStorage[pdrv].head;
//        while(element != NULL)
//        {
//            dev = (DevStr *)(element->data);
//            if(dev->ip == s_SstoreQueue[pdrv].storeOut->ip)//ip符合
//            {
//                *(dev->gTime) = DISTRIBUT_ENTIMERS;//定时清零
//                dev->comstr.variableState = s_SstoreQueue[pdrv].storeOut->comstr.variableState;//赋值
//                dev->comstr.operation = s_SstoreQueue[pdrv].storeOut->comstr.operation;//赋值
//                break;
//            }
//            element = element->next;
//        }
//        if(++s_SstoreQueue[pdrv].storeOut >= s_SstoreQueue[pdrv].store + _DISTRIBUT_STOREQUEUE_SIZE)
//        {
//            s_SstoreQueue[pdrv].storeOut = s_SstoreQueue[pdrv].store;
//        }           
//    }
}

/**
  * @Description: 初始化.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void distributInit(void)
{
    uint8_t pdrv;

    for(pdrv=0; pdrv<DISTRIBUT_DEVMAXNUM; pdrv++)
    {
        switch(pdrv)
        {
            case DISTRIBUT_DEV0:
//                s_SstoreQueue[pdrv].storeIn = s_SstoreQueue[pdrv].store;
//                s_SstoreQueue[pdrv].storeOut = s_SstoreQueue[pdrv].store;
                ListInit(pdrv);  
                s_ComProSts[pdrv].yx.switchOpen.value = &g_TelesignalDB[g_TelesignalAddr.switchOpen];
                s_ComProSts[pdrv].yx.switchClose.value = &g_TelesignalDB[g_TelesignalAddr.switchClose];    

                s_ComProSts[pdrv].yc.Ia = &g_TelemetryDB[g_TelemetryAddr.Ia];
                s_ComProSts[pdrv].yc.Ib = &g_TelemetryDB[g_TelemetryAddr.Ib];
                s_ComProSts[pdrv].yc.Ic = &g_TelemetryDB[g_TelemetryAddr.Ic];
                s_ComProSts[pdrv].yc.I0 = &g_TelemetryDB[g_TelemetryAddr.I0];
                s_ComProSts[pdrv].yc.Uab = &g_TelemetryDB[g_TelemetryAddr.Uab];
                s_ComProSts[pdrv].yc.Ubc = &g_TelemetryDB[g_TelemetryAddr.UCB];
                s_ComProSts[pdrv].yc.Uac = &g_TelemetryDB[g_TelemetryAddr.Uac];
                s_ComProSts[pdrv].yc.U0 = &g_TelemetryDB[g_TelemetryAddr.U0];   
            
                s_ComProSts[pdrv].opening = &rt_hw_do_operate;
                s_ComProSts[pdrv].closing = &rt_hw_do_operate;

                //智能分布式投退
                addtimers(pdrv,&s_FunctionalRetreat[pdrv].valstr.gTime);
                //状态判断
                addtimers(pdrv,&s_SelfstateJudge[pdrv].valstr.gTime);
                //故障隔离
                addtimers(pdrv,&s_FaultIsolation[pdrv].valstr.gTime);            
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
void distributClock(void)
{
    uint8_t pdrv;
	
    for(pdrv=0; pdrv<DISTRIBUT_DEVMAXNUM; pdrv++)
    {
        add_timers(pdrv);//定时增加

        switch(pdrv)
        {
            case DISTRIBUT_DEV0:
                distributUpdataDevStorage(pdrv);//更新仓库
                functional_retreat(pdrv);//智能分布式投退
                selfstate_judge(pdrv);//状态判断
                direction_judge(pdrv);//方向判断
                fault_isolation(pdrv);//故障隔离
                change_power(pdrv);//转供电
                distributUpdataSelfState(pdrv);//更新自身状态
//                self_healing(pdrv);//自愈
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
void distributReset(uint8_t pdrv)
{
    s_SelfSts[pdrv].comstr.variableState = 0;
}

/* END OF FILE ---------------------------------------------------------------*/

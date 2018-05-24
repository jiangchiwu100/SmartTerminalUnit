
#include "userVariable.h"
#include <rtthread.h>
#include "common_data.h"
#include "drv_do.h"

/* 遥信显示信息 */
YaoxinDisplayInfo yxInfo;
/* 遥测显示信息 0一次 1二次 2 谐波 */
YaoceDisplayInfo yceInfo[3];
/* 配置定值显示信息 */
DzhiDisplayInfo dzhi0Info[DZ0_ALLNUM];
/* 运行定值显示信息 */
DzhiDisplayInfo dzhi1Info[DZ1_ALLNUM];
/* Soe显示信息 */
SoeDisplayInfo soeInfo;
/* 故障事件显示信息 */
FeventDisplayInfo fEventInfo;
/* 命令下发 */
struct HmiCmdSend hmiCmdItems[] = {
	{"清除记录",0},
	{"分闸",1},
	{"合闸",2},
	{"复归",3},
};
HmiCmdSendInfo hcmdInfo;
/* 版本信息 */
VersionInfo versInfo;

void GetDisplayTime(SystemTimeDisplay *tim)
{
	uint16_t tms;
	struct CP56Time2a_t cpTime;
	DBReadSystemTime(&cpTime);
	tim->year = cpTime.year;
	tim->month = cpTime.month;
	tim->day = cpTime.dayofWeek & 0x1F;
	tim->hour = cpTime.hour;
	tim->min = cpTime.minute;
	tms = (cpTime.msecondH << 8) + cpTime.msecondL;
	tim->s = tms / 1000;
	tim->ms = tms % 1000;
}

/**
  *@brief  遥信显示初始化
  *@param  None
  *@retval None
  */
static void YaoxinDisplayInit(void)
{
	/* 遥信显示个数 */
	uint8_t YaoxinItem = 0;
	uint8_t i = 0;
	
	yxInfo.pRoot = TelesignalCfg;
	for(i = 0; i < g_TelesignalCfg_Len; i++){//查找可用遥信
		if(yxInfo.pRoot[i].enable == 0){
			continue;
		}
		YaoxinItem ++;
	}
	yxInfo.Num = YaoxinItem;
	yxInfo.pBuff = (uint8_t *)rt_malloc(yxInfo.Num);
	if(yxInfo.pBuff == NULL){
		rt_kprintf("遥信内存获取失败");
		return;
	}
	YaoxinItem = 0;
	for(i = 0; i < g_TelesignalCfg_Len; i++){//遥信显示缓冲
		if(yxInfo.pRoot[i].enable == 0){
			continue;
		}
		yxInfo.pBuff[YaoxinItem ++] = i;
	}
}

/**
  *@brief  遥测显示初始化
  *@param  None
  *@retval None
  */
static void YaoceDisplayInit(void)
{
	uint8_t yaoceItemsAll,typeIs;
	uint8_t YaoceItem = 0;
	uint8_t i,j;
	
	yaoceItemsAll = g_TelemetryCfg_Len;
	for(i = 0; i < 3; i++){
		if(i == 0){
			typeIs = ONCE;
		}
		else if(i == 1){
			typeIs = SECONDRY;
		}
		else{
			typeIs = HARMONIC;
		}
		/* 获取遥测实体指针 */
		yceInfo[i].pRoot = TelemetryCfg;
		YaoceItem = 0;
		for(j = 0; j < yaoceItemsAll; j++){//查找可用遥测
			if(yceInfo[i].pRoot[j].enable == 0 || yceInfo[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			YaoceItem ++;
		}
		yceInfo[i].num = YaoceItem;
		yceInfo[i].pBuff = (uint8_t *)rt_malloc(yceInfo[i].num);
		if(yceInfo[i].pBuff == NULL){
			rt_kprintf("遥测内存获取失败");
			return;
		}
		YaoceItem = 0;
		for(j = 0; j < yaoceItemsAll; j++){//遥测显示缓冲
			if(yceInfo[i].pRoot[j].enable == 0 || yceInfo[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			yceInfo[i].pBuff[YaoceItem ++] = j;
		}
	}
}

/**
  *@brief  配置定值修改保存
  *@param  addr 地址号
  *@retval None
  */
static void Dzhi0ModfiySave(uint16_t addr)
{
	addr = addr;
	rt_multi_common_data_save_value_to_fram(0);
}

/**
  *@brief  运行定值修改保存
  *@param  addr 地址号
  *@retval None
  */
static void Dzhi1ModfiySave(uint16_t addr)
{
	addr = addr;
	rt_multi_common_data_save_value_to_fram(g_ValueParaOperateInfo.currentSN);
}
/**
  *@brief  配置显示初始化
  *@param  None
  *@retval None
  */
static void Dzhi0DisplayInit(void)
{
	uint16_t dzhiItemsAll,typeIs;
	uint8_t dzhiItem = 0;
	uint16_t i,j;
	dzhiItemsAll = g_ParameterCfg_Len;
	for(i = 0 ; i < DZ0_ALLNUM; i++){
		if(i == 0){
			typeIs = ME_BASIC_SET;
		}
		else if(i == 1){
			typeIs = ME_ZERODRIFT;
		}
		else{
			typeIs = ME_DEADEZONE;
		}
		dzhiItem = 0;
		dzhi0Info[i].SaveModify = Dzhi0ModfiySave;
		dzhi0Info[i].pRoot = ParameterCfg;
		for(j = 0; j < dzhiItemsAll; j++){//查找可用定值
			if(dzhi0Info[i].pRoot[j].enable == 0 || dzhi0Info[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			dzhiItem ++;
		}
		dzhi0Info[i].num = dzhiItem;
		dzhi0Info[i].pBuff = (uint8_t *)rt_malloc(dzhi0Info[i].num);
		if(dzhi0Info[i].pBuff == NULL){
			rt_kprintf("定值内存获取失败");
			return;
		}
		dzhiItem = 0;
		for(j = 0; j < dzhiItemsAll; j++){//定值显示缓冲
			if(dzhi0Info[i].pRoot[j].enable == 0 || dzhi0Info[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			dzhi0Info[i].pBuff[dzhiItem ++] = j;
		}
	}
}

/**
  *@brief  运行定值类型重映射
  *@param  i 当前序列号 type 原始类型
  *@retval None
  */
static void Dzhi1TypeRemap(uint16_t i,uint16_t *type)
{
	switch(i)
	{
		case DZ1_INTERGHASE:*type = INTERGHASE;break;
		case DZ1_ZERO_SEQUE:*type = ZERO_SEQUE;break;
		case DZ1_LIMITATION:*type = LIMITATION;break;
		case DZ1_HEAVY_LOAD:*type = HEAVY_LOAD;break;
		case DZ1_OVERLOAD:*type = OVERLOAD;break;
		case DZ1_IBATTERY_SET:*type = BATTERY_SET;break;
		case DZ1_AUTO_RESET:*type = AUTO_RESET;break;
		case DZ1_LIMIT_V_F:*type = LIMIT_V_F;break;
		case DZ1_LOOP_CLOSE:*type = LOOP_CLOSE;break;
		case DZ1_FAULT_SWITCH:*type = FAULT_SWITCH;break;
		case DZ1_OTHER_PROTEC:*type = OTHER_PROTEC;break;
		case DZ1_LOGICAL_FUN:*type = LOGICAL_FUN;break;
		default:*type = INTERGHASE;break;
	}
}
/**
  *@brief  运行显示初始化
  *@param  None
  *@retval None
  */
static void Dzhi1DisplayInit(void)
{
	uint16_t dzhiItemsAll,typeIs;
	uint8_t dzhiItem = 0;
	uint16_t i,j;
	dzhiItemsAll = g_FixedValueCfg1_Len;
	for(i = 0 ; i < DZ1_ALLNUM; i++){
		Dzhi1TypeRemap(i,&typeIs);
		dzhiItem = 0;
		dzhi1Info[i].SaveModify = Dzhi1ModfiySave;
		dzhi1Info[i].pRoot = g_pFixedValueCfg;
		for(j = 0; j < dzhiItemsAll; j++){//查找可用定值
			if(dzhi1Info[i].pRoot[j].enable == 0 || dzhi1Info[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			dzhiItem ++;
		}
		dzhi1Info[i].num = dzhiItem;
		dzhi1Info[i].pBuff = (uint8_t *)rt_malloc(dzhi1Info[i].num);
		if(dzhi1Info[i].pBuff == NULL){
			rt_kprintf("定值内存获取失败");
			return;
		}
		dzhiItem = 0;
		for(j = 0; j < dzhiItemsAll; j++){//定值显示缓冲
			if(dzhi1Info[i].pRoot[j].enable == 0 || dzhi1Info[i].pRoot[j].menuNum != typeIs){
				continue;
			}
			dzhi1Info[i].pBuff[dzhiItem ++] = j;
		}
	}
}

/**
  *@brief  检测SOE更新
  *@param  None
  *@retval 0 无更新 1有更新
  */
uint8_t CheckSoeUpdata(void)
{
	if(soeInfo.in != g_FlagDB.queue_soe.in){
		soeInfo.in = g_FlagDB.queue_soe.in;
		return 1;
	}
	return 0;
}

/**
  *@brief  获取SOE
  *@param  soeNo soe号
  *@retval 0 成功 1 失败
  */
uint8_t GetSoeNoContent(uint16_t soeNo,SoeContent *pSoe)
{
	uint16_t pNo;
	uint16_t tms;
	GetSoeNum();
	if(soeNo >= soeInfo.num){
		return 1;
	}
	if(g_FlagDB.queue_soe.full == FULL){
		if(soeNo < g_FlagDB.queue_soe.in){
			pNo = g_FlagDB.queue_soe.in - soeNo - 1;
		}
		else{
			pNo = soeInfo.num - soeNo + g_FlagDB.queue_soe.in - 1;
		}
	}
	else{
		pNo = g_FlagDB.queue_soe.in - 1 - soeNo;
	}
	
	pSoe->time.year = g_SOEDB[pNo].time.year;
	pSoe->time.month = g_SOEDB[pNo].time.month;
	pSoe->time.day = g_SOEDB[pNo].time.dayofWeek & 0x1F;
	pSoe->time.hour = g_SOEDB[pNo].time.hour;
	pSoe->time.min = g_SOEDB[pNo].time.minute;
	tms = (g_SOEDB[pNo].time.msecondH << 8) + g_SOEDB[pNo].time.msecondL;
	pSoe->time.s = tms / 1000;
	pSoe->time.ms = tms % 1000;
	
	if(g_SOEDB[pNo].addr < 0x6000){
		if(g_SOEDB[pNo].addr <= g_TelesignalCfg_Len && g_SOEDB[pNo].addr > 0){	
			pSoe->pName = (uint8_t *)TelesignalCfg[g_SOEDB[pNo].addr - 1].pName;
			pSoe->pContent = (uint8_t *)TelesignalCfg[g_SOEDB[pNo].addr - 1].pContentSoe[g_SOEDB[pNo].value - 1];
		}
		if(g_SOEDB[pNo].value - 1){
			pSoe->highlight = 1;
		}
		else{
			pSoe->highlight = 0;
		}
	}
	else{
		return 1;
	}
	return 0;
}

/**
  *@brief  获取SOE总数
  *@param  soeNo soe号
  *@retval soe数量
  */
uint16_t GetSoeNum(void)
{
	if(g_FlagDB.queue_soe.full == FULL){
		soeInfo.num = SOE_MAX_NUM;
	}
	else{
		soeInfo.num = g_FlagDB.queue_soe.in;
	}
	return soeInfo.num;
}

/**
  *@brief  检测故障事件更新
  *@param  None
  *@retval 0 无更新 1有更新
  */
uint8_t CheckFeventUpdata(void)
{
	if(fEventInfo.in != g_FlagDB.queue_fevent.in){
		fEventInfo.in = g_FlagDB.queue_fevent.in;
		return 1;
	}
	return 0;
}
/**
  *@brief  获取故障事件
  *@param  feventNo 故障事件号
  *@retval 0 成功 1 失败
  */
uint8_t GetFeventNoContent(uint16_t feventNo,FeventContent *pEvent)
{
	struct SOE_Str *tSoe;
	uint16_t pNo,tAddr;
	uint16_t tms;
	GetFeventNum();
	if(feventNo >= fEventInfo.num){
		return 1;
	}
	if(g_FlagDB.queue_fevent.full == FULL){
		if(feventNo < g_FlagDB.queue_fevent.in){
			pNo = g_FlagDB.queue_fevent.in - feventNo - 1;
		}
		else{
			pNo = fEventInfo.num - feventNo + g_FlagDB.queue_fevent.in - 1;
		}
	}
	else{
		pNo = g_FlagDB.queue_fevent.in - 1 - feventNo;
	}
	tSoe = &g_FeventDB[pNo].yx[0];
	pEvent->time.year = tSoe->time.year;
	pEvent->time.month =  tSoe->time.month;
	pEvent->time.day =  tSoe->time.dayofWeek & 0x1F;
	pEvent->time.hour =  tSoe->time.hour;
	pEvent->time.min =  tSoe->time.minute;
	tms = ( tSoe->time.msecondH << 8) + tSoe->time.msecondL;
	pEvent->time.s = tms / 1000;
	pEvent->time.ms = tms % 1000;
	if(tSoe->addr < 0x6000){
		if(tSoe->addr <= g_TelesignalCfg_Len){
			pEvent->pName = (uint8_t *)TelesignalCfg[tSoe->addr - 1].pName;
			pEvent->pContent = (uint8_t *)TelesignalCfg[tSoe->addr - 1].pContentSoe[tSoe->value - 1];
		}
	}
	pEvent->yaoceNum = g_FeventDB[pNo].yc_num;
	if(pEvent->yaoceNum > 16){
		pEvent->yaoceNum = 16;
	} 
	for(uint8_t i = 0; i < pEvent->yaoceNum; i ++){
		tAddr = g_FeventDB[pNo].yc[i].addr - TELEMETRY_START_ADDR;
		if(tAddr < g_TelemetryCfg_Len){
			pEvent->pYaoceName[i] = (uint8_t *)TelemetryCfg[tAddr].pName;
			pEvent->pYaoceUnit[i] = (uint8_t *)TelemetryCfg[tAddr].pUnit;
			pEvent->pYaoceValue[i] = g_FeventDB[pNo].yc[i].value;
		}
	}
	return 0;
}

/**
  *@brief  获取故障事件总数
  *@param  None
  *@retval 故障事件总数
  */
uint16_t GetFeventNum(void)
{
	if(g_FlagDB.queue_fevent.full == FULL){
		fEventInfo.num = FEVENT_MAX_NUM;
	}
	else{
		fEventInfo.num = g_FlagDB.queue_fevent.in;
	}
	return fEventInfo.num;
}

/**
  *@brief  命令下发回调函数
  *@param  cmdIs 命令号
  *@retval 故障事件总数
  */
static void HmiCmdSendFun(uint8_t cmdIs)
{
	switch(cmdIs)
	{
		case 0:/* 清除记录 */
			DBClear();
			DBWriteSOE(DISTANT_CLAER_HISTORY_ADDR,ON);
			break;
		case 1:/* 分闸 */
			if(g_TelesignalDB[ADDR_REMOTE_EARTH] != ON){
				rt_hw_do_operate(DO_OPEN, LOCAL);
			}break;
		case 2:/* 合闸 */
			if(g_TelesignalDB[ADDR_REMOTE_EARTH] != ON){
				rt_hw_do_operate(DO_CLOSE, LOCAL);
			}break;
		case 3:/* 复归 */
			DBRevert(LOCAL);break;
		default:break;
	}
}

/**
  *@brief  命令下发初始化
  *@param  None
  *@retval None
  */
static void HmiCmdSendInit(void)
{
	hcmdInfo.itemsNum = sizeof(hmiCmdItems)/sizeof(struct HmiCmdSend);
	hcmdInfo.pHmiCmd = hmiCmdItems;
	hcmdInfo.cmdfun = HmiCmdSendFun;
}

/**
  *@brief  版本信息初始化
  *@param  None
  *@retval None
  */
static void VersionInfoInit(void)
{
	versInfo.num = g_InherentParaCfg_Len;
	versInfo.pItems = InherentParaCfg;
}
/**
  *@brief  用户显示数据初始化
  *@param  None
  *@retval None
  */
void userVariableDisplayInit(void)
{
	YaoxinDisplayInit();
	YaoceDisplayInit();
	Dzhi1DisplayInit();
	Dzhi0DisplayInit();
	HmiCmdSendInit();
	VersionInfoInit();
}

const struct YaoCeItem yaoCe1Items[YAOCE1_NUM] = {
	{ YAOCE_START_ADDR,	   8,3,"f",  {"Hz", ""}},
	{(YAOCE_START_ADDR+20),8,3,"Ia", {"A","KA"}},
	{(YAOCE_START_ADDR+21),8,3,"Ib", {"A","KA"}},
	{(YAOCE_START_ADDR+22),8,3,"Ic", {"A","KA"}},
	{(YAOCE_START_ADDR+23),8,3,"I0", {"A","KA"}},
	{(YAOCE_START_ADDR+24),8,3,"Uab",{"V","KV"}},
	{(YAOCE_START_ADDR+25),8,3,"Ubc",{"V","KV"}},
	{(YAOCE_START_ADDR+26),8,3,"Uac",{"V","KV"}},
	{(YAOCE_START_ADDR+27),8,3,"U0", {"V","KV"}},
	{(YAOCE_START_ADDR+28),8,3,"UAB",{"V","KV"}},
	{(YAOCE_START_ADDR+29),8,3,"UBC",{"V","KV"}},
	{(YAOCE_START_ADDR+30),8,3,"P",  {"KV","" }},
	{(YAOCE_START_ADDR+31),8,3,"Q",  {"Kvar",""}},
	{(YAOCE_START_ADDR+32),8,3,"S",  {"KVA",""}},
	{(YAOCE_START_ADDR+14),8,3,"pf", {"",   ""}},
	{(YAOCE_START_ADDR+15),8,3,"∠0", {"°","--"}},
	{(YAOCE_START_ADDR+16),8,3,"∠ab", {"°","--"}},	
	{(YAOCE_START_ADDR+17),8,3,"DC1",{"V","KV"}},
	{(YAOCE_START_ADDR+18),8,3,"DC2",{"V","KV"}},
	{(YAOCE_START_ADDR+19),8,3,"T",  {"℃", ""}}
};
const struct YaoCeItem yaoCe2Items[YAOCE2_NUM] = {
	{ YAOCE_START_ADDR,	  8,3,"f",  {"Hz", ""}},
	{(YAOCE_START_ADDR+1),8,3,"Ia", {"A","KA"}},
	{(YAOCE_START_ADDR+2),8,3,"Ib", {"A","KA"}},
	{(YAOCE_START_ADDR+3),8,3,"Ic", {"A","KA"}},
	{(YAOCE_START_ADDR+4),8,3,"I0", {"A","KA"}},
	{(YAOCE_START_ADDR+5),8,3,"Uab",{"V","KV"}},
	{(YAOCE_START_ADDR+6),8,3,"Ubc",{"V","KV"}},
	{(YAOCE_START_ADDR+7),8,3,"Uac",{"V","KV"}},
	{(YAOCE_START_ADDR+8),8,3,"U0", {"V","KV"}},
	{(YAOCE_START_ADDR+9),8,3,"UAB",{"V","KV"}},
	{(YAOCE_START_ADDR+10),8,3,"UBC",{"V","KV"}},
	{(YAOCE_START_ADDR+11),8,3,"P",  {"KV","" }},
	{(YAOCE_START_ADDR+12),8,3,"Q",  {"Kvar",""}},
	{(YAOCE_START_ADDR+13),8,3,"S",  {"KVA",""}},
	{(YAOCE_START_ADDR+14),8,3,"pf", {"",   ""}},
	{(YAOCE_START_ADDR+15),8,3,"∠0", {"°","--"}},
	{(YAOCE_START_ADDR+16),8,3,"∠ab", {"°","--"}},	
	{(YAOCE_START_ADDR+17),8,3,"DC1",{"V","KV"}},
	{(YAOCE_START_ADDR+18),8,3,"DC2",{"V","KV"}},
	{(YAOCE_START_ADDR+19),8,3,"T",  {"℃", ""}}
};

const struct YaoCeItem yaoCe3Items[HARMONIC_NUM] = {
	{HARMONIC_START_ADDR,	 8,3,"三次Ua",{"V","KV"}},
	{(HARMONIC_START_ADDR+1),8,3,"三次Ub",{"V","KV"}},
	{(HARMONIC_START_ADDR+2),8,3,"三次Ua",{"V","KV"}},
	{(HARMONIC_START_ADDR+3),8,3,"三次U0",{"V","KV"}},
	{(HARMONIC_START_ADDR+4),8,3,"三次Ia",{"A","KA"}},
	{(HARMONIC_START_ADDR+5),8,3,"三次Ib",{"A","KA"}},
	{(HARMONIC_START_ADDR+6),8,3,"三次Ic",{"A","KA"}},
	{(HARMONIC_START_ADDR+7),8,3,"三次I0",{"A","KA"}},
	
	{(HARMONIC_START_ADDR+8),8,3, "五次Ua",{"V","KV"}},
	{(HARMONIC_START_ADDR+9),8,3, "五次Ub",{"V","KV"}},
	{(HARMONIC_START_ADDR+10),8,3,"五次Ua",{"V","KV"}},
	{(HARMONIC_START_ADDR+11),8,3,"五次U0",{"V","KV"}},
	{(HARMONIC_START_ADDR+12),8,3,"五次Ia",{"A","KA"}},
	{(HARMONIC_START_ADDR+13),8,3,"五次Ib",{"A","KA"}},
	{(HARMONIC_START_ADDR+14),8,3,"五次Ic",{"A","KA"}},
	{(HARMONIC_START_ADDR+15),8,3,"五次I0",{"A","KA"}},
};
const struct DingZhi1Item InterphaseOverItems[InterphaseOver_NUM - 3]={/* 相间过流 */
	{InterphaseOver_START_ADDR,    "1段过流投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(InterphaseOver_START_ADDR+2),"1段过流电流",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(InterphaseOver_START_ADDR+3),"1段过流时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},	
	{(InterphaseOver_START_ADDR+4),"2段过流投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},	
	{(InterphaseOver_START_ADDR+6),"2段过流电流",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(InterphaseOver_START_ADDR+7),"2段过流时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},	
	{(InterphaseOver_START_ADDR+8),"3段过流投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(InterphaseOver_START_ADDR+10),"3段过流电流",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(InterphaseOver_START_ADDR+11),"3段过流时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item ZeroSequenceOverItems[ZeroSequenceOver_NUM]={/* 零序过流 */
	{ZeroSequenceOver_START_ADDR,    "1段过流投退",3,"0","0",0,0,    {"-","-"},{"退","投","告警"}},
	{(ZeroSequenceOver_START_ADDR+1),"1段过流电流",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(ZeroSequenceOver_START_ADDR+2),"1段过流时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(ZeroSequenceOver_START_ADDR+3),"2段过流投退",3,"0","0",0,0,    {"-","-"},{"退","投","告警"}},
	{(ZeroSequenceOver_START_ADDR+4),"2段过流电流",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(ZeroSequenceOver_START_ADDR+5),"2段过流时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item ThresholdAlarmItems[ThresholdAlarm_NUM]={/* 越线报警 */
	//name 标志 max min float 长 小数 单位 汉字
	{ThresholdAlarm_START_ADDR,    "越限报警投退",2,"0","0",0,0,{"-","-"},{"退","投"}},
	{(ThresholdAlarm_START_ADDR+1),"越限上限系数",0,"1","0",5,2,{"-","-"},{"-","-"}},
	{(ThresholdAlarm_START_ADDR+2),"越限下限系数",0,"2","1",5,2,{"-","-"},{"-","-"},},
	{(ThresholdAlarm_START_ADDR+3),"越限延时",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	
	{(ThresholdAlarm_START_ADDR+4),"Uab越限投退",2,"0","0",0,0,  {"-","-"},{"退","投"}},//Uab
	{(ThresholdAlarm_START_ADDR+5),"Ubc越限投退",2,"0","0",0,0,  {"-","-"},{"退","投"}},//Ubc
	{(ThresholdAlarm_START_ADDR+6),"Uac越限投退",2,"0","0",0,0,  {"-","-"},{"退","投"}},//Uac
    {(ThresholdAlarm_START_ADDR+7),"U上限定值",0,"999.9","0",6,2,{"V","KV"},{"-","-"}},
	{(ThresholdAlarm_START_ADDR+8),"U下限定值",0,"999.9","0",6,2,{"V","KV"},{"-","-"}},
	
    {(ThresholdAlarm_START_ADDR+9), "Ia越限投退",2,"0","0",0,0,   {"-","-"},{"退","投"}},//Ia
	{(ThresholdAlarm_START_ADDR+10),"Ib越限投退",2,"0","0",0,0,   {"-","-"},{"退","投"}},//Ib
	{(ThresholdAlarm_START_ADDR+11),"Ic越限投退",2,"0","0",0,0,   {"-","-"},{"退","投"}},//Ic
	{(ThresholdAlarm_START_ADDR+12),"I上限定值",0,"99.99","0",6,2,{"A","KA"},{"-","-"}},
	{(ThresholdAlarm_START_ADDR+13),"I下限定值",0,"99.99","0",6,2,{"A","KA"},{"-","-"}},
	
	{Threshold_ZERO_START_ADDR,    "U0越限投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},//U0
	{(Threshold_ZERO_START_ADDR+1),"U0上限定值",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(Threshold_ZERO_START_ADDR+2),"U0下限定值",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(Threshold_ZERO_START_ADDR+3),"I0越限投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},//Ia
	{(Threshold_ZERO_START_ADDR+4),"I0上限定值",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(Threshold_ZERO_START_ADDR+5),"I0下限定值",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	
	{Threshold_DC_START_ADDR,	 "U_DC越限投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},//U_DC
	{(Threshold_DC_START_ADDR+1),"U_DC上限定值",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(Threshold_DC_START_ADDR+2),"U_DC下限定值",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(Threshold_DC_START_ADDR+3),"I_DC越限投退",2,"0","0",0,0,	  {"-","-"},{"退","投"}},//I_DC
	{(Threshold_DC_START_ADDR+4),"I_DC上限定值",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(Threshold_DC_START_ADDR+5),"I_DC下限定值",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
};

const struct DingZhi1Item OverloadMuchItems[OverloadMuch_NUM]={/* 重过载 */
	{OverloadMuch_START_ADDR,    "重过载投退",2,"0","0",0,0,   {"-","-"},{"退","投"}},
	{(OverloadMuch_START_ADDR+1),"重过载定值",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(OverloadMuch_START_ADDR+2),"重过载系数",0,"1","0",5,2,	{"-","-"},{"-","-"}},
	{(OverloadMuch_START_ADDR+3),"重过载延时",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item OverloadItems[Overload_NUM]={/* 过负荷 */
	{Overload_START_ADDR,    "过负荷投退",2,"0","0",0,0,   {"-","-"},{"退","投"}},
	{(Overload_START_ADDR+1),"过负荷定值",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(Overload_START_ADDR+2),"过负荷系数",0,"1","0",5,2,	{"-","-"},{"-","-"}},
	{(Overload_START_ADDR+3),"过负荷延时",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item OverVoltageItems[OverVoltage_NUM]={/* 过电压 */
	{OverVoltage_START_ADDR,    "过电压投退",3,"0","0",0,0,    {"-","-"},{"退","投","告警"}},
	{(OverVoltage_START_ADDR+1),"过电压定值",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(OverVoltage_START_ADDR+2),"过电压时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(OverVoltage_START_ADDR+3),"过电压系数",0,"1","0",5,2,	   {"-","-"},{"-","-"}},
	{(OverVoltage_START_ADDR+4),"过频投退",3,"0","0",0,0,    {"-","-"},{"退","投","告警"}},
	{(OverVoltage_START_ADDR+5),"过频定值",0,"99.99","0",5,2,{"HZ","HZ"},{"-","-"}},
	{(OverVoltage_START_ADDR+6),"过频时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(OverVoltage_START_ADDR+7),"过频系数",0,"1","0",5,2,	   {"-","-"},{"-","-"}},
	{(OverVoltage_START_ADDR+8),"低压投退",3,"0","0",0,0,    {"-","-"},{"退","投","告警"}},
	{(OverVoltage_START_ADDR+9),"低压定值",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(OverVoltage_START_ADDR+10),"低压时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(OverVoltage_START_ADDR+11),"低压系数",0,"2","1",5,2,	   {"-","-"},{"-","-"}},
	{(OverVoltage_START_ADDR+12),"低频投退",3,"0","0",0,0,    {"-","-"},{"退","投","告警"}},
	{(OverVoltage_START_ADDR+13),"低频定值",0,"99.99","0",5,2,{"HZ","HZ"},{"-","-"}},
	{(OverVoltage_START_ADDR+14),"低频时间",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
	{(OverVoltage_START_ADDR+15),"低频系数",0,"2","1",5,2,	   {"-","-"},{"-","-"}},
};

const struct DingZhi1Item BatterySetItems[BatterySet_NUM]={/* 电池设置 */
	{BatterySet_START_ADDR,    "低压报警投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(BatterySet_START_ADDR+1),"电池低压定值",0,"999.9","0",5,2,{"V","KV"},{"-","-"}},
	{(BatterySet_START_ADDR+2),"电池低压系数",0,"1","0",5,2,	   {"-","-"},{"-","-"}},
	{(BatterySet_START_ADDR+3),"电池低压延时",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	
	{(BatterySet_START_ADDR+4),"电池活化投退",2,"0","0",0,0,    {"-", "-"},{"退","投"}},
	{(BatterySet_START_ADDR+5),"电池活化周期",0,"99999","0",5,2,{"天","h"},{"-","-"}},
	{(BatterySet_START_ADDR+6),"电池活化时刻",0,"23","0",5,2,	 {"h", "-"},{"-","-"}},
	{(BatterySet_START_ADDR+7),"活化故障电压",0,"9999", "0",5,2,{"V","KV"},{"-","-"}},
	{(BatterySet_START_ADDR+8),"活化故障延时",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
};
const struct DingZhi1Item AutoResetItems[AutoReset_NUM]={/* 自动复归 */
	{AutoReset_START_ADDR,    "自动复归投退",2,"0","0",0,0,      {"-","-"},{"退","投"}},
	{(AutoReset_START_ADDR+1),"自动复归时间",0,"9999999","0",7,0,{"s","ms"},{"-","-"}},
};
const struct DingZhi1Item RingUniteItems[RingUnite_NUM]={/* 合环 */
	{RingUnite_START_ADDR,    "合环投退",2,"0","0",0,0,      {"-","-"},{"退","投"}},
	{(RingUnite_START_ADDR+1),"两侧压差",0,"30","0",5,2,     {"V","KV"},{"-","-"}},
	{(RingUnite_START_ADDR+2),"相角差",  0,"10","0",5,2,     {"°","-"},{"-","-"}},
};
const struct DingZhi1Item BreakDownItems[BreakDown_NUM]={/* 故障投退 */
	{BreakDown_START_ADDR,    "控制回路掉线",2,"0","0",0,0,  {"-","-"},{"退","投"}},
};
const struct DingZhi1Item LoadSwitchItems[LoadSwitch_NUM]={/* 逻辑功能 */
	{LoadSwitch_START_ADDR,     "有流定值",    0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(LoadSwitch_START_ADDR+1), "有压定值",    0,"9999","0",5,2,{"V","KV"},{"-","-"}},
	{(LoadSwitch_START_ADDR+2), "Y延时",       0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+3), "分闸闭锁时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+4), "失电分闸投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(LoadSwitch_START_ADDR+5), "失电分闸时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+6), "得电合闸投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(LoadSwitch_START_ADDR+7), "得电合闸时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+8),"单侧失压合闸",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(LoadSwitch_START_ADDR+9),"单侧失压延时",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+10),"双侧有压投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(LoadSwitch_START_ADDR+11),"残压脉冲投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(LoadSwitch_START_ADDR+12),"残压整定电压",0,"99.99","0",5,2,{"V","KV"},{"-","-"}},
	{(LoadSwitch_START_ADDR+13),"U0保护投退",  2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(LoadSwitch_START_ADDR+14),"U0保护电压",  0,"99.99","0",5,2,{"V","KV"},{"-","-"}},
	{(LoadSwitch_START_ADDR+15),"U0电压延时",  0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(LoadSwitch_START_ADDR+16),"非遮断投退",  2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(LoadSwitch_START_ADDR+17),"非遮断电流",  0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
};
const struct DingZhi1Item CutoutSwitchItems[CutoutSwitch_NUM]={/* 断路器 */
	{CutoutSwitch_START_ADDR,    "反时限投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(CutoutSwitch_START_ADDR+1),"反时限定值",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+2),"反时限延时",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+3),"后加速投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(CutoutSwitch_START_ADDR+4),"后加速电流",0,"99.99","0",5,2,{"A","KA"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+5),"后加速时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+6),"涌流抑制投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(CutoutSwitch_START_ADDR+7),"涌流抑制时间",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+8),"重合闭锁投退",2,"0","0",0,0,    {"-","-"},{"退","投"}},
	{(CutoutSwitch_START_ADDR+9),"重合闭锁时间",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+10),"相间重合次数",0,"5","0",5,0,    {"次","-"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+11),"零序重合次数",0,"2","0",5,2,    {"次","-"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+12),"重合闸1时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+13),"重合闸2时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+14),"重合闸3时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+15),"重合闸4时间",0,"99.99","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+16),"重合复归时间",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
	{(CutoutSwitch_START_ADDR+17),"重合充电时间",0,"999.9","0",5,2,{"s","ms"},{"-","-"}},
};

const struct DingZhi1Item DeadZoneItems[DeadZone_NUM]={/* 死区设置 */
	{DeadZone_START_ADDR,    "频率",    0,"99.99","0",5,2,{"%","Hz"},{"-","-"}},
	{(DeadZone_START_ADDR+1),"电流A相", 0,"99.99","0",5,2,{"%","A"},{"-","-"}},
	{(DeadZone_START_ADDR+2),"电流B相", 0,"99.99","0",5,2,{"%","A"},{"-","-"}},
	{(DeadZone_START_ADDR+3),"电流C相", 0,"99.99","0",5,2,{"%","A"},{"-","-"}},
	{(DeadZone_START_ADDR+4),"零序电流",0,"99.99","0",5,2,{"%","A"},{"-","-"}},
	{(DeadZone_START_ADDR+5),"线电压ab",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+6),"线电压bc",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+7),"线电压ac",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+8),"零序电压",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+9), "线电压AB", 0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+10),"线电压BC", 0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+11),"有功功率", 0,"99.99","0",5,2,{"%","-"},{"-","-"}},
	{(DeadZone_START_ADDR+12),"无功功率", 0,"99.99","0",5,2,{"%","-"},{"-","-"}},
	{(DeadZone_START_ADDR+13),"视在功率", 0,"99.99","0",5,2,{"%","-"},{"-","-"}},
	{(DeadZone_START_ADDR+14),"功率因数", 0,"99.99","0",5,2,{"%","-"},{"-","-"}},
	{(DeadZone_START_ADDR+15),"直流电压1",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+16),"直流电压2",0,"99.99","0",5,2,{"%","V"},{"-","-"}},
	{(DeadZone_START_ADDR+17),"温度",     0,"99.99","0",5,2,{"%","-"},{"-","-"}},
};

const struct DingZhi1Item ZeroDriftItems[ZeroDrift_NUM]={/* 零漂设置 */
	{ZeroDrift_START_ADDR,     "频率",     0,"99.99","0",5,2,{"Hz","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+1), "电流A相",  0,"99.99","0",5,2,{"A","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+2), "电流B相",  0,"99.99","0",5,2,{"A","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+3), "电流C相",  0,"99.99","0",5,2,{"A","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+4), "零序电流", 0,"99.99","0",5,2,{"A","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+5), "线电压ab", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+6), "线电压bc", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+7), "线电压ac", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+8), "零序电压", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},	
	{(ZeroDrift_START_ADDR+9), "线电压AB", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+10),"线电压BC", 0,"99.99","0",5,2,{"V","%"},{"-","-"}},
	{(ZeroDrift_START_ADDR+11),"有功功率", 0,"99.99","0",5,2,{"KW","-"},{"-","-"}},
	{(ZeroDrift_START_ADDR+12),"无功功率", 0,"99.99","0",5,2,{"Kvr","-"},{"-","-"}},
	{(ZeroDrift_START_ADDR+13),"视在功率", 0,"99.99","0",5,2,{"KVA","-"},{"-","-"}},
	{(ZeroDrift_START_ADDR+14),"功率因数", 0,"99.99","0",5,2,{"-","-"},{"-","-"}},
	{(ZeroDrift_START_ADDR+15),"直流电压1",0,"99.99","0",5,2,{"V","V"},{"-","-"}},
	{(ZeroDrift_START_ADDR+16),"直流电压2",0,"99.99","0",5,2,{"V","V"},{"-","-"}},
	{(ZeroDrift_START_ADDR+17),"温度",     0,"99.99","0",5,2,{"℃","-"},{"-","-"}},
};
const struct DingZhi1Item BasicConfigureItems[BasicConfigure_NUM]={/* 基本设置 */
	{BasicConfigure_START_ADDR,     "操作机构",    2,"0","0",0,0,{"-","-"},{"弹簧","永磁"}},
	{(BasicConfigure_START_ADDR+1), "开关类型",    2,"0","0",0,0,{"-","-"},{"断路器","负荷开关"}},
	{(BasicConfigure_START_ADDR+2), "断路器模式",  5,"0","0",0,0,{"-","-"},{"无","常规保护","电压时间型","电压电流型","电流计数型"}},
	{(BasicConfigure_START_ADDR+3), "负荷开关模式",5,"0","0",0,0,{"-","-"},{"无","电压时间型","电压电流型","电流计数型","分界"}},
	{(BasicConfigure_START_ADDR+4), "联络开关识别",2,"0","0",0,0,{"-","-"},{"手动","自动"}},
	{(BasicConfigure_START_ADDR+5), "永磁输出翻转",2,"0","0",5,2,{"-","-"},{"不翻转","翻转"}},
	{(BasicConfigure_START_ADDR+6), "合闸欠压定值",0,"250","0",6,0,{"V","KV"},{"-","-"}},	
	{(BasicConfigure_START_ADDR+7), "分闸欠压定值",0,"250","0",6,0,{"V","KV"},{"-","-"}},
	{(BasicConfigure_START_ADDR+8), "电容欠压系数",0,"2","1",5,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+9), "一次变比_U", 0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+10), "二次变比_U", 0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+11),"一次变比_U0",0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+12),"二次变比_U0",0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+13),"一次变比_I", 0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+14),"二次变比_I", 0,"10000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+15),"一次变比_I0",0,"1000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+16),"二次变比_I0",0,"1000","0",6,2,{"-","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+17),"开入防抖",   0,"99999","0",5,0,{"ms","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+18),"合闸脉宽",   0,"99999","0",5,0,{"ms","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+19),"分闸脉宽",   0,"99999","0",5,0,{"ms","-"},{"-","-"}},
	{(BasicConfigure_START_ADDR+20),"返校时间",   0,"99999","0",5,0,{"ms","-"},{"-","-"}},
};
const struct SOEItem SOEItems[SOE_NUM] = {//SOE
	{SOE_START_ADDR,    "开关分位",      {"合->分","分->合"}},
    {(SOE_START_ADDR+1),"开关合位",      {"合->分","分->合"}},
    {(SOE_START_ADDR+2),"储能/有压",     {"有->无","无->有"}},	
	{(SOE_START_ADDR+3),"低气压",        {"告警->复位","无->告警"}},	
	{(SOE_START_ADDR+4),"电源故障告警",  {"告警->复位","无->告警"}},
	{(SOE_START_ADDR+5),"电池欠压告警",  {"告警->复位","无->告警"}},
	{(SOE_START_ADDR+6),"电池活化状态",  {"活化->结束","开始活化"}},
	{(SOE_START_ADDR+7),"输入失电告警",  {"无->告警","告警->结束"}},
	{(SOE_START_ADDR+8),"工作模式",      {"联络->分段","分段->联络"}},
	{(SOE_START_ADDR+9),"重合压板/FA",   {"投->退","退->投"}},
    {(SOE_START_ADDR+10),"保护压板",     {"投->退","退->投"}},
	{(SOE_START_ADDR+11),"远方/就地",    {"远方->就地","就地->远方"}},
	
	{(START_TO_SHORTBD_ADDR),  "过流保护",{"复位","保护"}},
	{(START_TO_SHORTBD_ADDR+1),"零序保护",{"复位","保护"}},
	{(START_TO_SHORTBD_ADDR+2),"保护动作",{"复位","动作"}},
	{(START_TO_SHORTBD_ADDR+3),"保护闭锁",{"复位","闭锁"}},
	{(START_TO_SHORTBD_ADDR+4),"合闸闭锁",{"复位","闭锁"}},
	{(START_TO_SHORTBD_ADDR+5),"分闸闭锁",{"复位","闭锁"}},
	{(START_TO_SHORTBD_ADDR+6),"重合动作",{"复位","动作"}},
	{(START_TO_SHORTBD_ADDR+7),"重合闭锁",{"复位","闭锁"}},
	{(START_TO_SHORTBD_ADDR+8),"装置故障",{"无","故障"}},
	{(START_TO_SHORTBD_ADDR+9),"自检异常",{"无","异常"}},
	{(START_TO_SHORTBD_ADDR+10),"通信",   {"断","通"}},
	{(START_TO_SHORTBD_ADDR+11),"开关类型",{"断路器","负荷开关"}},	
	{(START_TO_SHORTBD_ADDR+12),"电池低压告警",{"告警->结束","无->告警"}},
	{(START_TO_SHORTBD_ADDR+13),"双点遥信",    {"合->分","分->合"}},
	{(START_TO_SHORTBD_ADDR+14),"控制回路断线",{"复位","断线"}},
	
	{(START_TO_AOVERP_ADDR),  "A相过流一段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+1),"B相过流一段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+2),"C相过流一段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+3),"A相过流二段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+4),"B相过流二段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+5),"C相过流二段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+6),"A相过流三段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+7),"B相过流三段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+8),"C相过流三段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+9), "零序过流一段",{"复位","保护"}},
	{(START_TO_AOVERP_ADDR+10),"零序过流二段",{"复位","保护"}},
	
	{(START_TO_POSTACCELE_ADDR),  "A相后加速",{"复位","过流"}},
	{(START_TO_POSTACCELE_ADDR+1),"B相后加速",{"复位","过流"}},
	{(START_TO_POSTACCELE_ADDR+2),"C相后加速",{"复位","过流"}},
	{(START_TO_POSTACCELE_ADDR+3),"A相反时限",{"复位","过流"}},
	{(START_TO_POSTACCELE_ADDR+4),"B相反时限",{"复位","过流"}},
	{(START_TO_POSTACCELE_ADDR+5),"C相反时限",{"复位","过流"}},
	{(START_TO_POSTACCELE_ADDR+6),"重合闸一次",{"复位","保护"}},
	{(START_TO_POSTACCELE_ADDR+7),"重合闸二次",{"复位","保护"}},
	{(START_TO_POSTACCELE_ADDR+8),"重合闸三次",{"复位","保护"}},
	{(START_TO_POSTACCELE_ADDR+9),"重合闸四次",{"复位","保护"}},
	{(START_TO_POSTACCELE_ADDR+10),"重合闸闭锁",  {"复位","闭锁"}},
	{(START_TO_POSTACCELE_ADDR+11),"零序重合一次",{"复位","保护"}},
	{(START_TO_POSTACCELE_ADDR+12),"零序重合二次",{"复位","保护"}},
	
	{(START_TO_TWOCLOSE_ADDR),  "二次重合闭锁",{"复位","闭锁"}},	
	{(START_TO_TWOCLOSE_ADDR+1),"失电跳闸",    {"复位","跳闸"}},
	{(START_TO_TWOCLOSE_ADDR+2),"得电合闸",    {"复位","合闸"}},
	{(START_TO_TWOCLOSE_ADDR+3),"合闸到故障",  {"无","故障"}},
	{(START_TO_TWOCLOSE_ADDR+4),"保护闭锁",    {"复位","闭锁"}},
	{(START_TO_TWOCLOSE_ADDR+5),"双压禁止合闸",{"复位","保护"}},
	{(START_TO_TWOCLOSE_ADDR+6),"单侧失压合闸",{"复位","动作"}},
	{(START_TO_TWOCLOSE_ADDR+7),"残压闭锁合闸",{"复位","保护"}},	
	{(START_TO_TWOCLOSE_ADDR+8),"零序电压接地",{"复位","保护"}},
	{(START_TO_TWOCLOSE_ADDR+9),"小信号接地",  {"复位","跳闸"}},
	{(START_TO_TWOCLOSE_ADDR+10),"遮断电流",   {"复位","保护"}},
	{(START_TO_TWOCLOSE_ADDR+11),"过负荷",     {"复位","告警"}},
	{(START_TO_TWOCLOSE_ADDR+12),"重过载",     {"复位","告警"}},
	{(START_TO_TWOCLOSE_ADDR+13),"过压保护",   {"复位","保护"}},
	{(START_TO_TWOCLOSE_ADDR+14),"过频保护",   {"复位","保护"}},
	{(START_TO_TWOCLOSE_ADDR+15),"低压保护",   {"复位","保护"}},
	{(START_TO_TWOCLOSE_ADDR+16),"低频保护",   {"复位","保护"}},
	
	{(START_TO_VLIMIT_A_ADDR),  "Uab越限",{"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+1),"Uab越限",{"复位","越下限"}},
	{(START_TO_VLIMIT_A_ADDR+2),"Uca越限",{"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+3),"Uca越限",{"复位","越下限"}},
	{(START_TO_VLIMIT_A_ADDR+4),"UBC越限",{"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+5),"UBC越限",{"复位","越下限"}},
	{(START_TO_VLIMIT_A_ADDR+6),"零序电压越限",{"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+7),"零序电压越限",{"复位","越下限"}},
	{(START_TO_VLIMIT_A_ADDR+8),"A相电流越限", {"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+9),"A相电流越限", {"复位","越下限"}},
	
	{(START_TO_VLIMIT_A_ADDR+10),"B相电流越限", {"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+11),"B相电流越限", {"复位","越下限"}},
	{(START_TO_VLIMIT_A_ADDR+12),"C相电流越限", {"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+13),"C相电流越限", {"复位","越下限"}},
	{(START_TO_VLIMIT_A_ADDR+14),"零序电流越限",{"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+15),"零序电流越限",{"复位","越下限"}},	
	{(START_TO_VLIMIT_A_ADDR+16),"直流电压越限",{"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+17),"直流电压越限",{"复位","越下限"}},	
	{(START_TO_VLIMIT_A_ADDR+18),"直流电流越限",{"复位","越上限"}},
	{(START_TO_VLIMIT_A_ADDR+19),"直流电流越限",{"复位","越下限"}},
	{(START_TO_VLIMIT_A_ADDR+20),"装置掉电",    {"复位","掉电"}},

	{0x00006001,"远方分闸",{"选择失败","选择成功"}},
	{0x00016001,"远方分闸",{"执行失败","执行成功"}},
	{0x00026001,"远方合闸",{"选择失败","选择成功"}},
	{0x00036001,"远方合闸",{"执行失败","执行成功"}},
	{0x00046001,"远方合环",{"选择失败","选择成功"}},
	{0x00056001,"远方合环",{"执行失败","执行成功"}},	
	{0x00066001,"遥控取消",                {"",""}},
	{0x00006002,"远方活化",            {"断","通"}},
	{0x00006003,"远方复位",        {"失败","成功"}},
	{0x00006004,"远方清除记录",    {"失败","成功"}},
	{0x00006005,"本地分闸",{"选择失败","选择成功"}},
	{0x00016005,"本地分闸",{"执行失败","执行成功"}},
	{0x00026005,"本地合闸",{"选择失败","选择成功"}},
	{0x00036005,"本地合闸",{"执行失败","执行成功"}},
	{0x00046005,"本地合环",{"选择失败","选择成功"}},
	{0x00056005,"本地合环",{"执行失败","执行成功"}},
	{0x00006006,"本地复位",        {"失败","成功"}},
	{0x00006007,"本地清除记录",    {"失败","成功"}},
	{0x00006008,"手持分闸",{"选择失败","选择成功"}},
	{0x00016008,"手持分闸",{"执行失败","执行成功"}},
	{0x00026008,"手持合闸",{"选择失败","选择成功"}},
	{0x00036008,"手持合闸",{"执行失败","执行成功"}},
	{0x00046008,"手持合环",{"选择失败","选择成功"}},
	{0x00056008,"手持合环",{"执行失败","执行成功"}},
};

const struct ConstParameter ConstParItems[ConstParameter_NUM] = {//固有参数
	{ConstParameter_START_ADDR,    "终端类型:"},	     									                                      
	{(ConstParameter_START_ADDR+1),"终端操作系统:"},
	{(ConstParameter_START_ADDR+2),"终端制造商:"},
	{(ConstParameter_START_ADDR+3),"终端硬件版本:"},
	{(ConstParameter_START_ADDR+4),"终端软件版本"},
	{(ConstParameter_START_ADDR+5),"版本校验码:"},
	{(ConstParameter_START_ADDR+6),"通信规约类型:"},
	{(ConstParameter_START_ADDR+7),"终端出厂型号:"},
	{(ConstParameter_START_ADDR+8),"终端ID号:"},
	{(ConstParameter_START_ADDR+9),"终端网卡MAC地址:"},
	{(ConstParameter_START_ADDR+10),"版本序列号:"},
};

const struct CmdSend CmdSendItems[CmdSend_NUM] = {//命令下发
	{CS_SOECLEAR_MENUCMD_NUM,"清除记录"},
	{CS_OPENSWITCH_NUM,		"分闸"},
	{CS_CLOSESWITCH_NUM,	"合闸"},
	{CS_RESETKEY_NUM,		"复归"},
};

const struct PassWordMessage PassWordItems[PassWord_NUM] = {//密码管理
	{"100000","请输入密码:"},
};

/* 0.1.2   0 为大版本号，0目前为南网 1 为 国网 */
const uint8_t *HMiVersion = "180412";

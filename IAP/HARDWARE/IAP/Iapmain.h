
#ifndef __IAPMAIN_H
#define	__IAPMAIN_H

#include "stm32f4xx.h"

#define REVIAPSLAVE_MAX		1024
#define SENDIAPSLAVE_MAX	1024

#define REGTABMUN 5

#define IAPREG_BEGINADDR 0x0001
enum IapRegAddrTab{
	IAPREG_CONNECT = 0x0001,/* 与主板的连接状态 */
	IAPREG_FIRSTPOWER,/* 第一次上电 */
	IAPREG_RUNSTATUS,/* 运行状态 */
	IAPREG_UPDATA,/* 开始更新 */
	IAPREG_GETPROGRAM,/* 获取程序 */
};
struct RegisterTab{
	const uint16_t regAddr;
	const uint8_t type;
	uint8_t *value;
};

typedef struct Type1Reg_{
	const uint16_t addr;
	uint8_t value;
}Type1Reg;

struct IapRegInfo{
	Type1Reg connect;/* 与主板的连接状态 */
	Type1Reg firstPower;/* 第一次上电 */
	Type1Reg runStatus;	/* 运行状态 */
	Type1Reg updata;/* 开始更新 */
	Type1Reg GetProgram;/* 获取程序 */
	Type1Reg iapJump;	/* 跳转命令 */
};

struct IapTimeOutCheck{
	uint8_t flag;//是否进行超时检测 0 不进行 1 进行
	uint8_t source;//超时源
	uint32_t count;//计数
	uint32_t outTime;//超时时间
	void (*overFun)(uint8_t source);
};

enum IapStepEnum{
	IAP_INIT,
	IAP_WAITCONNECT,
	IAP_CONNECTFINISH,
	IAP_WAITCONTROL,
};

struct FileOperate{
	uint8_t number;
	uint8_t flag;
	uint8_t *pbuff;
	uint16_t buffSize;
	uint16_t offset;
};

void IapmainInit(void);
void IapMianScan(void);

#endif /* __IAPMAIN_H */

/* END */

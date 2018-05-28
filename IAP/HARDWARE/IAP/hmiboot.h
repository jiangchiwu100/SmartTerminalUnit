#ifndef __HMIBOOT_H
#define	__HMIBOOT_H

#include "stm32f4xx.h"

#define REVIAPMASTER_MAX		1024
#define SENDIAPMASTER_MAX		1024
#define FILEBUFFSIZE 				4096
/* 面板程序SDRAM */
#define HMIPROGRAMSIZEADDR 	0xC00C0000
#define HMIPROGRAMSTARTADDR (HMIPROGRAMSIZEADDR + 4)

/* 寄存器数量 */
#define REGTABMUN 6
#define IAPREG_BEGINADDR 0x0001

enum IapRegAddrTab{
	IAPREG_CONNECT = 0x0001,/* 与主板的连接状态 */
	IAPREG_FIRSTPOWER,/* 第一次上电 */
	IAPREG_RUNSTATUS,/* 运行状态 */
	IAPREG_UPDATA,/* 开始更新 */
	IAPREG_GETPROGRAM,/* 获取程序 */
	IAPREG_IAPJUMP,
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
	uint8_t *name;
	uint32_t count;//计数
	uint32_t outTime;//超时时间
	void (*overFun)(uint8_t source,uint8_t *name);
};

enum TimeCheckStatus{
	TC_DISENABLE,
	TC_ENABLE,
};
enum TimeOutNumber{
	TO_NONE,
	TO_SAVEFILEFALSE,
	TO_ERASEFALSE,
	TO_WRITEPROFLASE,
	TOHMIFINISH,
	TOJUMP,
	TOHMIBACK,
};

struct FileOperate{
	uint8_t number;//文件代号
	uint8_t flag;//操作使用
	uint8_t fileFlag;//文件应答使用
	uint32_t fileSize;//文件内容大小
	uint8_t *const pbuff;//文件缓存
	const uint16_t buffSize;//缓存大小
	uint16_t pBuffIn;
	uint16_t len;//发送长度
	uint32_t offset;//文件偏移
};

void IapHmiInit(void);
void IapHmiScan(void);
uint8_t SaveHmiProgram(uint8_t *flag);
uint8_t HmiProgramErase(uint8_t *flag);
uint8_t WriteHmiProgram(uint8_t *flag);
uint8_t HmiProgramUpadta(uint8_t *flag);
uint8_t HmiProgramBack(uint8_t *flag);
void IapOutTimCheck(void);
uint8_t GetprogramTofile(void);
void HmiProgramJump(void);
void testCreatFile(void);
#endif /* __HMIBOOT_H */

/* END */

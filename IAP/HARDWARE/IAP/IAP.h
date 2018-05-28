#ifndef _IAP_H
#define _IAP_H
#include "sys.h"

#define DEBUG_

#define SDRAMSTARTADDR  0XC0000000	/* 新程序开始地址 */
#define READEACHSIZE 	4096		/* 保存程序时，每4096为一个单位 */
#define IAPSTARTADDR 	0x08040000	/* 主程序开始地址 */
#define BEGINSECTOR 	6			/* 第六个扇区为开始擦除扇区 */
#define MAXBSECTOR 		(11 - BEGINSECTOR + 1)/* 可用扇区数*/
#define IAPENDADDR 		0x080FFC04    //第九个扇区结束地址  (9-6+1)*128 = 512KB

/* 跳转状态 */
#define IAPJUMP_FIRSTRUN 	0
#define IAPJUMP_UPDATA 		1
#define IAPJUMP_RUNOK 		2 
#define IAPJUMP_ROLLBACK 	3

typedef  void (*iapfun)(void);

struct ProgramState_{
	uint8_t firstRun;
	uint8_t currentState;
};

extern struct ProgramState_ ProgramState;//存放FM25XX读出变量
void Iapmain(void);
void iap_load_app(u32 appxaddr,uint8_t useID);
void FATFSInit(void);
void testIAP(void);
uint8_t readProgramFlag(void);
u32 STMFLASH_ReadWord(u32 faddr);

void programUpdata(void);
uint8_t SaveRunProgram(void);
void programRunError(void);
uint8_t writeProgramToFlash(uint8_t *fileName);
uint8_t updateProgramToFlash(void);

void PowerOnCount(void);
uint8_t SetFlagIsFirstRun(void);

#endif

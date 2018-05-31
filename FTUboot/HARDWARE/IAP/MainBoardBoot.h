#ifndef __MAINBOARDBOOT_H
#define	__MAINBOARDBOOT_H

#include "stm32f4xx.h"

/* 主板SDRAM开始地址 */
#define MAIN_SDRAM_STARTADDR  0XC0000000

#define WRITEFILEBUFF 4096


uint8_t SaveMainProgram(void);
uint8_t UpdataMainProgram(void);
uint8_t MainProgramBack(void);
void MtestCreatFile(void);
#endif /* __MAINBOARDBOOT_H */

/* END */

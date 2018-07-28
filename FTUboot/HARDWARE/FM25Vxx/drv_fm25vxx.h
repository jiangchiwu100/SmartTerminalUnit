/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fm25vxx.h
  * @brief:     The driver of FM25Vxx10.
  * @version:   ver 1.1 
  * @author:    Mr.Sun
  * @date:      2017.09.05
  * @update:    [YYYY-MM-DD] [更改人姓名][变更描述]
  */

#ifndef __DRV_FM25VXX_H
#define __DRV_FM25VXX_H

/* INCLUDES ------------------------------------------------------------------*/	
#include "stm32f4xx_hal.h"

/* DEFINE --------------------------------------------------------------------*/	
#define  IO_SPI1_SCK_HIGH()               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET) 
#define  IO_SPI1_SCK_LOW()                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET) 
#define  IO_SPI1_MOSI_HIGH()              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET) 
#define  IO_SPI1_MOSI_LOW()               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET) 
#define  IO_SPI1_MISO_READ()              HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
#define  IO_SPI1_CS_HIGH()                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET) 
#define  IO_SPI1_CS_LOW()                 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET) 

#define FM25V10_MAX_ADDR					0x3FFF 

#define PRO_STATE_BEGINADDR		0x00001		/* 程序状态开始地址 */
#define PRO_STATE_ALLBYTE			3
enum ProgramState{
	PRO_FIRSTPOWER,	/* 第一次上电 */
	PRO_MAINBOARD_STATE,	/* 主板状态 */
	PRO_BOOTCOUNT,				/* 开机计数 */
};

#define IS_FIRSTRUN_FLAG					0x55//第一次上电标志flag
#define IS_WILLUPDATA_FLAG				0x01//程序更新标志
#define IS_UPDATAFINISH_FLAG			0x02//程序更新结束标志
#define IS_UPDATAFAULT_FLAG				0x04//程序更新异常标志
#define IS_RUNOK_FLAG							0x08//运行正常标志
#define IS_RUNERROR_FLAG					(!IS_RUNOK_FLAG)//运行异常标志
#define IS_ROLLBACK_FLAG					0x20//回退标志

#define FRAM_WREN                       0x06     // Set write enable latch
#define FRAM_WRDI                       0x04     // Write disable
#define FRAM_RDSR                       0x05     // Read status register
#define FRAM_WRSR                       0x01     // Write status register
#define FRAM_READ                       0x03     // Read memory data
#define FRAM_FSTRD                      0x0B     // Fast read memory data
#define FRAM_WRITE                      0x02     // Write memory data
#define FRAM_SLEEP                      0xB9     // Enter sleep mode
#define FRAM_RIID                       0x9F     // Read device ID
#define FRAM_SNR                        0xC3     // Read S/N


#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif /* END FALSE */
#include "fm25v10.h"

/* PUBLIC FUNCTION  ----------------------------------------------------------*/
#define FM25VxxReadData(addr, flag, pBuf,  len) FramReadInduce(addr, len, pBuf)
#define FM25VxxWriteData(addr, flag, pBuf,  len) FramWriteInduce(addr, len, pBuf)
//void FM25VxxInit(void);
//uint8_t FM25VxxReadData(uint32_t addr, uint8_t *flag, uint8_t *pBuf, uint32_t len);
//uint8_t FM25VxxWriteData(uint32_t addr, uint8_t *flag, uint8_t *pBuf, uint32_t len);
void FM25VxxUseKill(void);



#endif

/* END OF FILE ---------------------------------------------------------------*/

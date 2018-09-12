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
#include "sys.h"

/* DEFINE --------------------------------------------------------------------*/
#define DEVICEID  		0x2400			  //FRAM设备ID

#define  IO_SPI2_SCK_HIGH()               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET) 
#define  IO_SPI2_SCK_LOW()                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET) 
#define  IO_SPI2_MOSI_HIGH()              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET) 
#define  IO_SPI2_MOSI_LOW()               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET) 
#define  IO_SPI2_MISO_READ()              HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
#define  IO_SPI2_CS_HIGH()                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET) 
#define  IO_SPI2_CS_LOW()                 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET) 

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

#define WREN                       0x06     // Set write enable latch
#define WRDI                       0x04     // Write disable
#define RDSR                       0x05     // Read status register
#define WRSR                       0x01     // Write status register
#define READ                       0x03     // Read memory data
#define FSTRD                      0x0B     // Fast read memory data
#define WRITE                      0x02     // Write memory data
#define SLEEP                      0xB9     // Enter sleep mode
#define RDID                       0x9F     // Read device ID
#define SNR                        0xC3     // Read S/N
#define FRAM_CS							PFout(6)

#ifndef  FALSE
#define  FALSE     0
#define  TRUE      !FALSE
#endif /* END FALSE */


/* PUBLIC FUNCTION  ----------------------------------------------------------*/
extern uint8_t (*FramReadDate)(uint32_t address, uint32_t number, uint8_t* p);
extern void (*FramWriteDate)(uint32_t address, uint32_t number, uint8_t* p);
void FRAM_Init(void);
void FramSPI5WriteString(uint32_t address, uint32_t number, uint8_t* p);
void FramSPI2WriteString(uint32_t address, uint32_t number, uint8_t* p);
uint8_t FramSPI5ReadString(uint32_t address, uint32_t number, uint8_t* p);
uint8_t FramSPI2ReadString(uint32_t address, uint32_t number, uint8_t* p);



void FM25VxxInit(void);						//FRAM初始化

void FRAM_SPI5_Init(void);					

void FramWriteByte(uint32_t address, uint8_t da);	
uint8_t FramReadByte(uint32_t address);
void FramWriteString(uint32_t address, uint32_t number, uint8_t* p);
uint8_t FramReadString(uint32_t address, uint32_t number, uint8_t* p);

void IO_SPI2_Init(void);
uint8_t IO_SPI2_ReadWriteByte(uint8_t byte);

uint16_t FRAM_SPI2_ReadID(void);
uint16_t FRAM_SPI5_ReadID(void);

void Fram_Check(void);						//检查fram
void FM25VxxUseKill(void);

#define FM25VxxReadData(addr, flag, pBuf,  len) FramReadDate(addr, len, pBuf)
#define FM25VxxWriteData(addr, flag, pBuf,  len) FramWriteDate(addr, len, pBuf)
#endif

/* END OF FILE ---------------------------------------------------------------*/

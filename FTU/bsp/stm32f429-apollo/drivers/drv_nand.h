/**
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_nand.h
  * @brief:     The driver of nand flash,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    lizhen9880
  * @date:      2017-04-10
  * @update:    [2018-02-05][Lexun][make the code clean up]
  */
#ifndef __DRV_NAND_H__
#define __DRV_NAND_H__


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

	 
/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include <board.h>
#include "stm32f4xx.h"
#include <rtdevice.h>
     
 
/* DEFINES -------------------------------------------------------------------*/
//#define DEBUG // debug option

#ifdef DEBUG
#define NAND_PRINTF(...)        rt_kprintf(__VA_ARGS)
#else
#define NAND_PRINTF(...)
#endif

#define NAND_MAX_PAGE_SIZE			4096		//定义NAND FLASH的最大的PAGE大小（不包括SPARE区），默认4096字节
#define NAND_ECC_SECTOR_SIZE		512			//执行ECC计算的单元大小，默认512字节

/* nandflash confg */
#define PAGES_PER_BLOCK         64
#define PAGE_DATA_SIZE          2048
#define PAGE_OOB_SIZE           64
#define ECC_SIZE                4

#define SET_NAND_CMD(_c)        do{*(volatile rt_uint8_t*)(NAND_ADDRESS|NAND_CMD) = _c;}while(0)
#define SET_NAND_ADD(_a)        do{*(volatile rt_uint8_t*)(NAND_ADDRESS|NAND_ADDR) = _a;}while(0)    
#define SET_NAND_DAT(_d)        do{*(volatile rt_uint8_t*)NAND_ADDRESS = _d;}while(0) 
#define GET_NAND_DAT(_d)        do{_d = *(volatile rt_uint8_t*)NAND_ADDRESS;}while(0) 

// 位带操作,实现51类似的GPIO控制功能
// 具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
// IO口操作宏定义
#define GPIOD_IDR_Addr          (GPIOD_BASE+16) //0x40020C10 
#define BITBAND(addr, bitnum)   ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)          *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)  MEM_ADDR(BITBAND(addr, bitnum)) 
#define PDin(n)                 BIT_ADDR(GPIOD_IDR_Addr,n) // 输入 

#define NAND_RB                 PDin(6) // NAND Flash的闲/忙引脚 

#define NAND_ADDRESS            ((rt_uint32_t)0x80000000)	// nand flash的访问地址,接NCE3,地址为:0X8000 0000
#define NAND_CMD                (uint32_t)(0x010000) // 发送命令
#define NAND_ADDR               (uint32_t)(0x020000) // 发送地址

// NAND FLASH命令
#define NAND_READID             0X90 // 读ID指令
#define NAND_FEATURE            0XEF // 设置特性指令
#define NAND_RESET              0XFF // 复位NAND
#define NAND_READSTA            0X70 // 读状态
#define NAND_AREA_A             0X00   
#define NAND_AREA_TRUE1         0X30  
#define NAND_WRITE0             0X80
#define NAND_WRITE_TURE1        0X10
#define NAND_ERASE0             0X60
#define NAND_ERASE1             0XD0
#define NAND_MOVEDATA_CMD0      0X00
#define NAND_MOVEDATA_CMD1      0X35
#define NAND_MOVEDATA_CMD2      0X85
#define NAND_MOVEDATA_CMD3      0X10

// NAND FLASH状态
#define NSTA_READY              0X40 // nand已经准备好
#define NSTA_ERROR              0X01 // nand错误
#define NSTA_TIMEOUT            0X02 // 超时
#define NSTA_ECC1BITERR       	0X03 // ECC 1bit错误
#define NSTA_ECC2BITERR       	0X04 // ECC 2bit以上错误

// NAND FLASH型号和对应的ID号
#define MT29F4G08ABADA          0XDC909556 // MT29F4G08ABADA
#define MT29F16G08ABABA         0X48002689 // MT29F16G08ABABA


/* STRUCTS -------------------------------------------------------------------*/
// NAND属性结构体
typedef struct
{
    uint16_t page_totalsize;     	//每页总大小，main区和spare区总和
    uint16_t page_mainsize;      	//每页的main区大小
    uint16_t page_sparesize;     	//每页的spare区大小
    uint8_t  block_pagenum;      	//每个块包含的页数量
    uint16_t plane_blocknum;     	//每个plane包含的块数量
    uint16_t block_totalnum;     	//总的块数量
    uint16_t good_blocknum;      	//好块数量    
    uint16_t valid_blocknum;     	//有效块数量(供文件系统使用的好块数量)
    uint32_t id;             		//NAND FLASH ID
    uint16_t *lut;      			   	//LUT表，用作逻辑块-物理块转换
    uint32_t ecc_hard;				//硬件计算出来的ECC值
    uint32_t ecc_hdbuf[NAND_MAX_PAGE_SIZE/NAND_ECC_SECTOR_SIZE];//ECC硬件计算值缓冲区  	
    uint32_t ecc_rdbuf[NAND_MAX_PAGE_SIZE/NAND_ECC_SECTOR_SIZE];//ECC读取的值缓冲区
}nand_attriute;      

struct stm32f4_nand
{
    rt_uint8_t id[5];
    struct rt_mutex lock;
    struct rt_completion comp;
};


/* PUBLIC VARIABLES ----------------------------------------------------------*/


/* PUBLIC FUNCTION -----------------------------------------------------------*/
extern int rt_hw_nand_init(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __DRV_NAND_H__ */


/* END OF FILE ---------------------------------------------------------------*/


/*
** ------------------------------- Copyright (c) -------------------------------                                     
**
**                           http://www.sojoline.com
**
** note:    Copyright SOJO. ALL RIGHTS RESERVED.
**          This software is provided under license and contains proprietary and
**          confidential material which is the property of Company Name tech. 
**    
** -------------------------------- File Info ----------------------------------
** File name:               lwip_comm.h
** Descriptions:            The hardware driver of lwip. 
**
** -----------------------------------------------------------------------------
** Created by:              Mr.Lee
** Created date:            2017.02.08
** System:                  SJFD2000 communication software system
** Version:                 V4.01
** Descriptions:            The original version
**
** -----------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
** ---------------------------------------------------------------------------*/
#ifndef _DP83848_H_
#define _DP83848_H_ 

/* INCLUDE FILES -------------------------------------------------------------*/
//#include "sys.h"
//#include "lwip/netif.h"

#include "stm32f4xx_hal.h"
#include "stdint.h"


#define  INT_ETHERNET_PRIO									               ((0x00<<4)|0x01) // 以太网中断优先级最高

/* DEFINE --------------------------------------------------------------------*/
   

/* GLOBAL VARIABLES ----------------------------------------------------------*/

extern ETH_HandleTypeDef ETH_Handler;               //以太网句柄

extern ETH_DMADescTypeDef DMARxDscrTab[ETH_RXBUFNB];	//以太网DMA接收描述符数据结构体指针
extern ETH_DMADescTypeDef DMATxDscrTab[ETH_TXBUFNB];	//以太网DMA发送描述符数据结构体指针
extern uint8_t Rx_Buff[ETH_RX_BUF_SIZE * ETH_RXBUFNB]; 					//以太网底层驱动接收buffers指针
extern uint8_t Tx_Buff[ETH_TX_BUF_SIZE * ETH_TXBUFNB]; 					//以太网底层驱动发送buffers指针


extern ETH_DMADescTypeDef  *DMATxDescToSet;			//DMA发送描述符追踪指针
extern ETH_DMADescTypeDef  *DMARxDescToGet; 		//DMA接收描述符追踪指针 
 

/* PUBLIC FUNCTION APIS ------------------------------------------------------*/
uint8_t DP83848_Init(void);
void ETHERNET_IRQHandler(void);
uint8_t Netif_LinkStatus(void);
void Netif_Config(void);
uint8_t ETH_MACDMA_Config(void);

uint32_t  ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDesc);


#endif /* END _DP83848_H_ */

/* END OF FILE ---------------------------------------------------------------*/



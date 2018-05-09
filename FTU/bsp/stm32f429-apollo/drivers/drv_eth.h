 /** 
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_eth.h
  * @brief:     The driver of ethernet,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    tanek
  * @date:      2017-06-08
  * @update:    [2017-11-22][Lexun][The STM32F7 version(V1.0.0)]
  */
#ifndef __DRV_ETH_H__
#define __DRV_ETH_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include <rtdevice.h>


/* DEFINES -------------------------------------------------------------------*/
/* debug option */
//#define DEBUG
//#define ETH_RX_DUMP
//#define ETH_TX_DUMP


#ifdef DEBUG
#define ETH_PRINTF          rt_kprintf
#else
#define ETH_PRINTF(...)
#endif

/** RMII interface
  *  ETH_MDIO ------------------> PA2
  *  ETH_MDC -------------------> PC1
  *  ETH_RMII_REF_CLK-----------> PA1
  *  ETH_RMII_CRS_DV -----------> PA7
  *  ETH_RMII_RXD0 -------------> PC4
  *  ETH_RMII_RXD1 -------------> PC5
  *  ETH_RMII_TX_EN ------------> PG11
  *  ETH_RMII_TXD0 -------------> PG13
  *  ETH_RMII_TXD1 -------------> PG14
  *  ETH_RESET------------------> PH2
  */
#define ETH_MDIO_PORN             GPIOA
#define ETH_MDIO_PIN              GPIO_PIN_2
#define ETH_MDC_PORN              GPIOC
#define ETH_MDC_PIN               GPIO_PIN_1
#define ETH_RMII_REF_CLK_PORN     GPIOA
#define ETH_RMII_REF_CLK_PIN 			GPIO_PIN_1
#define ETH_RMII_CRS_DV_PORN 			GPIOA
#define ETH_RMII_CRS_DV_PIN 			GPIO_PIN_7
#define ETH_RMII_RXD0_PORN 				GPIOC
#define ETH_RMII_RXD0_PIN 				GPIO_PIN_4
#define ETH_RMII_RXD1_PORN 				GPIOC
#define ETH_RMII_RXD1_PIN 				GPIO_PIN_5
#define ETH_RMII_TX_EN_PORN 			GPIOG
#define ETH_RMII_TX_EN_PIN 				GPIO_PIN_11
#define ETH_RMII_TXD0_PORN 				GPIOG
#define ETH_RMII_TXD0_PIN 				GPIO_PIN_13
#define ETH_RMII_TXD1_PORN 				GPIOG
#define ETH_RMII_TXD1_PIN 				GPIO_PIN_14
#define ETH_WKRST_PORN 				    GPIOH
#define ETH_WKRST_PIN 				    GPIO_PIN_2

#define ETH_WKRST_ON              HAL_GPIO_WritePin(GPIOH,GPIO_PIN_2,GPIO_PIN_SET);
#define ETH_WKRST_OFF 				    HAL_GPIO_WritePin(GPIOH,GPIO_PIN_2,GPIO_PIN_RESET);

#define LAN8742A_PHY_ADDRESS      0x00

#define MAX_ADDR_LEN              6


/* STRUCTS -------------------------------------------------------------------*/
struct rt_stm32_eth
{
    struct eth_device parent; // inherit from ethernet device

    /* interface address info. */
    rt_uint8_t  dev_addr[MAX_ADDR_LEN]; // hw address

    uint32_t ETH_Speed; // ETH_Speed
    uint32_t ETH_Mode;  // ETH_Duplex_Mode
};


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
extern int rt_hw_eth_init(void);


#endif /* END __DRV_ETH_H__ */


/* END OF FILE ---------------------------------------------------------------*/


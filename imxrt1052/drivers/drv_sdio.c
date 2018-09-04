/*
 * File      : drv_sdio.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006-2013, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-10-10     Tanek        the first version.
 * 2018-05-11     Liu2guang    refactor sdio code. 
 */
 
#include "drv_sdio.h" 

#include "fsl_common.h" 
#include "fsl_iomuxc.h" 
#include "fsl_gpio.h"
#include "fsl_usdhc.h"

#define RT_USING_SDIO1 
// #define RT_USING_SDIO2
// #define SDIO_USING_DEBUG

#if defined(RT_USING_SDIO1) || defined(RT_USING_SDIO2) 

#if defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL
    #error "Please don't define 'FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL'!"
#endif

#ifndef SDIO_USING_DEBUG 
#define SDIO_INFO(fmt, ...) 
#else
#define SDIO_INFO(fmt, ...)                   \
    do{                                       \
        rt_kprintf("[\033[32mSDIO\033[0m] "); \
        rt_kprintf(fmt, ##__VA_ARGS__);       \
    }while(0)
#endif

/* SDIO时钟配置: 0:PLL2PFD2, 1:PLL2PFD0 */ 
#if !defined(USDHC1_CLK_SOURCE) && defined(RT_USING_SDIO1)
#define USDHC1_CLK_SOURCE         (1U) /* PLL2PFD2 */ 
#endif
#if !defined(USDHC1_CLK_SOURCE_DIVIDER) && defined(RT_USING_SDIO1)
#define USDHC1_CLK_SOURCE_DIVIDER (4U) /* 1-8div */
#endif 

#if !defined(USDHC2_CLK_SOURCE) && defined(RT_USING_SDIO2)
#define USDHC2_CLK_SOURCE         (1U) /* PLL2PFD2 */ 
#endif
#if !defined(USDHC2_CLK_SOURCE_DIVIDER) && defined(RT_USING_SDIO2)
#define USDHC2_CLK_SOURCE_DIVIDER (4U) /* 1-8div */
#endif 
    
#if !defined(USDHC_CLK_MIN_FREQ) 
#define USDHC_CLK_MIN_FREQ        (375UL*1000UL) 
#endif 
#if !defined(USDHC_CLK_MAX_FREQ) 
#define USDHC_CLK_MAX_FREQ        (20UL*1000UL*1000UL) 
#endif 
    
/* SDIO句柄结构体 */
struct rt1050_sdio
{
    struct rt_mmcsd_host *host; 
    struct rt_mmcsd_req  *req; 
    struct rt_mmcsd_cmd  *cmd; 
    
    usdhc_host_t         usdhc_host; 
    usdhc_adma_config_t  dma_config; 
    
    uint32_t *sdio_adma_table; 
}; 

#if defined(RT_USING_SDIO1) 
struct rt1050_sdio sdio1 = {0}; 
ALIGN(4) uint32_t sdio1_adma_table[8] SECTION("NonCacheable");
#endif 

#if defined(RT_USING_SDIO2) 
static struct rt1050_sdio sdio2 = {0}; 
ALIGN(4) uint32_t sdio2_adma_table[8] SECTION("NonCacheable");
#endif 
    
/* 获取SDIO输入时钟 */ 
static uint32_t rt1050_sdio_clk_freq(USDHC_Type *base)
{
    uint32_t freq = 0; 
    
    if(base == USDHC1)
    {
        freq  = (CLOCK_GetMux(kCLOCK_Usdhc1Mux) == 0) ? CLOCK_GetSysPfdFreq(kCLOCK_Pfd2) : CLOCK_GetSysPfdFreq(kCLOCK_Pfd0); 
        freq /= (CLOCK_GetDiv(kCLOCK_Usdhc1Div) + 1U); 
    }
    else if(base == USDHC2) 
    {
        freq  = (CLOCK_GetMux(kCLOCK_Usdhc2Mux) == 0) ? CLOCK_GetSysPfdFreq(kCLOCK_Pfd2) : CLOCK_GetSysPfdFreq(kCLOCK_Pfd0); 
        freq /= (CLOCK_GetDiv(kCLOCK_Usdhc2Div) + 1U); 
    }
    
    return freq;
}

static void rt1050_sdio_clk_init(void) 
{
    /* Init sdio clk */ 
#if (USDHC1_CLK_SOURCE == 0 || USDHC2_CLK_SOURCE == 0)
    /* PLL2PFD2_CLK = PLL2(528M) * 18 / PFD2_FRAC = 528*18/24 = 352M, PFD2_FRAC = 12~35 */ 
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 27); 
    SDIO_INFO("P2PDF2 CLK: %.3d.%.6dMHz\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk)/1000000UL, CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk)%1000000UL); 
#endif 
    
#if (USDHC1_CLK_SOURCE == 1 || USDHC2_CLK_SOURCE == 1)
    /* PLL2PFD0_CLK = PLL2(528M) * 18 / PFD0_FRAC = 528*18/27 = 352M, PFD0_FRAC = 12~35 */ 
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 27); 
    SDIO_INFO("P2PDF0 CLK: %.3d.%.6dMHz\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk)/1000000UL, CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk)%1000000UL); 
#endif 
    
#if defined(RT_USING_SDIO1) 
    /* USDHC1_CLK_ROOT = USDHC1_CLK_SOURCE / DIV(1-8) */ 
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, USDHC1_CLK_SOURCE); 
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, USDHC1_CLK_SOURCE_DIVIDER-1); 
    CLOCK_EnableClock(kCLOCK_Usdhc1); 
    
    SDIO_INFO("USDHC1 CLK: %.3d.%.6dMHz\n", rt1050_sdio_clk_freq(USDHC1)/1000000UL, rt1050_sdio_clk_freq(USDHC1)%1000000UL); 
#endif
    
#if defined(RT_USING_SDIO2) 
    /* USDHC2_CLK_ROOT = USDHC2_CLK_SOURCE / DIV(1-8) */ 
    CLOCK_SetMux(kCLOCK_Usdhc2Mux, USDHC2_CLK_SOURCE); 
    CLOCK_SetDiv(kCLOCK_Usdhc2Div, USDHC2_CLK_SOURCE_DIVIDER-1); 
    CLOCK_EnableClock(kCLOCK_Usdhc2);
    
    SDIO_INFO("USDHC2 CLK: %.3d.%.6dMHz\n", rt1050_sdio_clk_freq(USDHC2)/1000000UL, rt1050_sdio_clk_freq(USDHC2)%1000000UL); 
#endif 
}

static void rt1050_sdio_gpio_init(void) 
{
    CLOCK_EnableClock(kCLOCK_Iomuxc); 
    
#if defined(RT_USING_SDIO1) 
    IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_00_USDHC1_CMD,   0); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD,   0x00017089u); 
    IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_01_USDHC1_CLK,   0); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK,   0x00014089u); 
    IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0, 0); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0, 0x00017089u); 
    IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1, 0); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1, 0x00017089u); 
    IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2, 0); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2, 0x00017089u); 
    IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3, 0); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3, 0x00017089u); 
    
    IOMUXC_SetPinMux   (IOMUXC_GPIO_B1_14_USDHC1_VSELECT,  0);   
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_USDHC1_VSELECT,  0x000170A1u); 
    IOMUXC_SetPinMux   (IOMUXC_GPIO_B1_12_GPIO2_IO28,      0);  
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_GPIO2_IO28,      0x00017089u); 
    IOMUXC_SetPinMux   (IOMUXC_GPIO_AD_B0_05_GPIO1_IO05,   0);   
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_05_GPIO1_IO05,   0x000010B0u); 
    
    gpio_pin_config_t gpio; 
    gpio.direction     = kGPIO_DigitalOutput; 
    gpio.outputLogic   = 1; 
    gpio.interruptMode = kGPIO_NoIntmode; 
    GPIO_PinInit (GPIO1, 5U, &gpio); 
    
    SDIO_INFO("USDHC1 GPIO Init Successful\n"); 
#endif 

#if defined(RT_USING_SDIO2) 
    SDIO_INFO("USDHC2 GPIO Init Successful\n"); 
#endif 
}

static void rt1050_sdio_host_init(void) 
{
#if defined(RT_USING_SDIO1) 
    sdio1.usdhc_host.base                       = USDHC1; 
    sdio1.usdhc_host.config.dataTimeout         = 0xFU; 
    sdio1.usdhc_host.config.endianMode          = kUSDHC_EndianModeLittle; 
    sdio1.usdhc_host.config.readBurstLen        = 8U; 
    sdio1.usdhc_host.config.writeBurstLen       = 8U; 
    sdio1.usdhc_host.config.readWatermarkLevel  = 0x80U; 
    sdio1.usdhc_host.config.writeWatermarkLevel = 0x80U; 
    
    USDHC_Init(sdio1.usdhc_host.base, &(sdio1.usdhc_host.config)); 
    SDIO_INFO("USDHC1 Host Init Successful\n"); 
#endif 

#if defined(RT_USING_SDIO2) 
    sdio2.usdhc_host.base                       = USDHC2; 
    sdio2.usdhc_host.config.dataTimeout         = 0xFU; 
    sdio2.usdhc_host.config.endianMode          = kUSDHC_EndianModeLittle; 
    sdio2.usdhc_host.config.readBurstLen        = 8U; 
    sdio2.usdhc_host.config.writeBurstLen       = 8U; 
    sdio2.usdhc_host.config.readWatermarkLevel  = 0x80U;
    sdio2.usdhc_host.config.writeWatermarkLevel = 0x80U;
    
    USDHC_Init(sdio2.usdhc_host.base, &(sdio2.usdhc_host.config)); 
    SDIO_INFO("USDHC2 Host Init Successful\n"); 
#endif 
}

static void SDMMCHOST_ErrorRecovery(USDHC_Type *base)
{
    uint32_t status = 0U;
    /* get host present status */
    status = USDHC_GetPresentStatusFlags(base);
    /* check command inhibit status flag */
    if ((status & kUSDHC_CommandInhibitFlag) != 0U)
    {
        /* reset command line */
        USDHC_Reset(base, kUSDHC_ResetCommand, 1000U);
    }
    /* check data inhibit status flag */
    if ((status & kUSDHC_DataInhibitFlag) != 0U)
    {
        /* reset data line */
        USDHC_Reset(base, kUSDHC_ResetData, 1000U);
    }
}

static void rt1050_sdio_request(struct rt_mmcsd_host *host, struct rt_mmcsd_req *req)
{
    struct rt1050_sdio *sdio = RT_NULL; 
    
    struct rt_mmcsd_cmd  *cmd; 
    struct rt_mmcsd_data *data;
    
    usdhc_adma_config_t adma     = {0}; 
    usdhc_command_t  fsl_command = {0}; 
    usdhc_transfer_t fsl_content = {0}; 
    usdhc_data_t fsl_data        = {0}; 
    rt_uint32_t *buf = RT_NULL; 
    status_t error;
    
    RT_ASSERT(host != RT_NULL); 
    RT_ASSERT(req  != RT_NULL); 
    
    sdio = (struct rt1050_sdio *)(host->private_data); 
    
    cmd  = req->cmd; 
    data = cmd->data; 
    
    /* 配置amda */ 
    adma.dmaMode        = kUSDHC_DmaModeAdma2;
    adma.burstLen       = kUSDHC_EnBurstLenForINCR;
    adma.admaTable      = sdio->sdio_adma_table;
    adma.admaTableWords = 8U; 
    
    fsl_command.index    = cmd->cmd_code;
    fsl_command.argument = cmd->arg; 
    
    if(cmd->cmd_code == STOP_TRANSMISSION)
    {
        fsl_command.type = kCARD_CommandTypeAbort; 
    }
    else
    {
        fsl_command.type = kCARD_CommandTypeNormal; 
    }
    
    switch(cmd->flags & RESP_MASK)
    {
    case RESP_NONE:
        fsl_command.responseType = kCARD_ResponseTypeNone;
        break;
    case RESP_R1:
        fsl_command.responseType = kCARD_ResponseTypeR1;
        break;
    case RESP_R1B:
        fsl_command.responseType = kCARD_ResponseTypeR1b;
        break;
    case RESP_R2:
        fsl_command.responseType = kCARD_ResponseTypeR2;
        break;
    case RESP_R3:
        fsl_command.responseType = kCARD_ResponseTypeR3;
        break;
    case RESP_R4:
        fsl_command.responseType = kCARD_ResponseTypeR4;
        break;
    case RESP_R6:
        fsl_command.responseType = kCARD_ResponseTypeR6;
        break;
    case RESP_R7:
        fsl_command.responseType = kCARD_ResponseTypeR7;
        break;
    case RESP_R5:
        fsl_command.responseType = kCARD_ResponseTypeR5;
        break;
//    case RESP_R5B:
//        fsl_command.responseType = kCARD_ResponseTypeR5b;
//        break;
    default:
        RT_ASSERT(NULL);
    }
    
    fsl_command.flags   = 0;
    fsl_content.command = &fsl_command; 
    
    if (data)
    {
        if (req->stop != RT_NULL)
        {
            fsl_data.enableAutoCommand12 = true; 
        }
        else
        {
            fsl_data.enableAutoCommand12 = false; 
        }
        
        fsl_data.enableAutoCommand23 = false; 

        fsl_data.enableIgnoreError = false;
        fsl_data.dataType = kUSDHC_TransferDataNormal;  //todo : update data type
        fsl_data.blockSize = data->blksize;
        fsl_data.blockCount = data->blks; 
        
        SDIO_INFO("blksize:%d, blks:%d\n", fsl_data.blockSize, fsl_data.blockCount); 
        
        if (((rt_uint32_t)data->buf & (32 - 1)) ||         // align cache(32byte)
                ((rt_uint32_t)data->buf >  0x00000000 && (rt_uint32_t)data->buf < 0x00080000) /*||  // ITCM
            ((rt_uint32_t)data->buf >= 0x20000000 && (rt_uint32_t)data->buf < 0x20080000)*/)    // DTCM
        {

            buf = rt_malloc_align(fsl_data.blockSize * fsl_data.blockCount, 32); 
            RT_ASSERT(buf != RT_NULL);

            SDIO_INFO("malloc buf: %p, data->buf:%p, %d\n", buf, data->buf, fsl_data.blockSize * fsl_data.blockCount);
        }


        if ((cmd->cmd_code == WRITE_BLOCK) || (cmd->cmd_code == WRITE_MULTIPLE_BLOCK))
        {
            if (buf)
            {
                SDIO_INFO("write(data->buf to buf)\n");
                rt_memcpy(buf, data->buf, fsl_data.blockSize * fsl_data.blockCount);
                fsl_data.txData = (uint32_t const *)buf;
            }
            else
            {
                fsl_data.txData = (uint32_t const *)data->buf;
            }

            fsl_data.rxData = NULL;
        }
        else
        {
            if (buf)
            {
                fsl_data.rxData = (uint32_t *)buf;
            }
            else
            {
                fsl_data.rxData = (uint32_t *)data->buf;
            }

            fsl_data.txData = NULL;
        }

        fsl_content.data = &fsl_data;
    }
    else
    {
        fsl_content.data = RT_NULL; 
    }
    
    error = USDHC_TransferBlocking(sdio->usdhc_host.base, &adma, &fsl_content);
    if (error == kStatus_Fail)
    {
        SDMMCHOST_ErrorRecovery(sdio->usdhc_host.base);
        SDIO_INFO(" ***USDHC_TransferBlocking error: %d*** --> \n", error);
        cmd->err = -RT_ERROR;
    }

    if (buf)
    {
        if (fsl_data.rxData)
        {
            SDIO_INFO("read copy buf to data->buf\n");
            rt_memcpy(data->buf, buf, fsl_data.blockSize * fsl_data.blockCount);
        }

        rt_free_align(buf);
    }

    if ((cmd->flags & RESP_MASK) == RESP_R2)
    {
        cmd->resp[3] = fsl_command.response[0];
        cmd->resp[2] = fsl_command.response[1];
        cmd->resp[1] = fsl_command.response[2];
        cmd->resp[0] = fsl_command.response[3];
        SDIO_INFO("resp 0x%08X 0x%08X 0x%08X 0x%08X\n",
                  cmd->resp[0], cmd->resp[1], cmd->resp[2], cmd->resp[3]);
    }
    else
    {
        cmd->resp[0] = fsl_command.response[0];
        SDIO_INFO(" resp 0x%08X\n", cmd->resp[0]);
    }

    mmcsd_req_complete(host);

    return;
}

static void rt1050_sdio_set_iocfg(struct rt_mmcsd_host *host, struct rt_mmcsd_io_cfg *io_cfg)
{
    struct rt1050_sdio *sdio = RT_NULL; 
    
    RT_ASSERT(host   != RT_NULL); 
    RT_ASSERT(io_cfg != RT_NULL); 
    
    sdio = (struct rt1050_sdio *)(host->private_data); 
    
    rt_uint32_t clk   = io_cfg->clock; 
    rt_uint32_t width = io_cfg->bus_width; 
    
    /* 限制SDIO最打时钟 */ 
    clk = (clk > USDHC_CLK_MAX_FREQ) ? USDHC_CLK_MAX_FREQ : clk; 
    if(clk == 0)
    {
        return; 
    }
    
    uint32_t src_clk = rt1050_sdio_clk_freq(sdio->usdhc_host.base); 
    USDHC_SetSdClock(sdio->usdhc_host.base, src_clk, clk); 

    switch(width)
    {
    case MMCSD_BUS_WIDTH_1:
        USDHC_SetDataBusWidth(sdio->usdhc_host.base, kUSDHC_DataBusWidth1Bit); 
    break; 
    
    case MMCSD_BUS_WIDTH_4:
        USDHC_SetDataBusWidth(sdio->usdhc_host.base, kUSDHC_DataBusWidth4Bit); 
    break; 
    
    case MMCSD_BUS_WIDTH_8:
        USDHC_SetDataBusWidth(sdio->usdhc_host.base, kUSDHC_DataBusWidth8Bit); 
    break; 
    
    default:
        RT_ASSERT(RT_NULL);
    }
    
#if defined(SDIO_USING_DEBUG)
//    if(sdio->usdhc_host.base == USDHC1)
//    {
//        SDIO_INFO("USDHC1 src_clk: %dHz, bus_clk: %dHz\n", src_clk, clk); 
//    }
//    if(sdio->usdhc_host.base == USDHC2)
//    {
//        SDIO_INFO("USDHC1 src_clk: %dHz, bus_clk: %dHz\n", src_clk, clk); 
//    }
#endif 
}

const struct rt_mmcsd_host_ops rt1050_sdio_ops =
{
    rt1050_sdio_request, 
    rt1050_sdio_set_iocfg, 
    RT_NULL, 
    RT_NULL 
}; 

int rt_hw_sdio_init(void)
{
    /* 初始化句柄 */
#if defined(RT_USING_SDIO1) 
    sdio1.host = mmcsd_alloc_host(); 
    if(sdio1.host == RT_NULL)
    {
        SDIO_INFO("USDHC1 RTT Host alloc failed\n"); 
        return RT_ERROR; 
    }
    else
    {
        SDIO_INFO("USDHC1 RTT Host alloc successful\n"); 
    }
    
    sdio1.host->ops           = &rt1050_sdio_ops; 
    sdio1.host->freq_min      = USDHC_CLK_MIN_FREQ; 
    sdio1.host->freq_max      = USDHC_CLK_MAX_FREQ; 
    sdio1.host->valid_ocr     = VDD_32_33 | VDD_33_34; 
    sdio1.host->flags         = MMCSD_BUSWIDTH_4 | MMCSD_MUTBLKWRITE | MMCSD_SUP_HIGHSPEED | MMCSD_SUP_SDIO_IRQ; 
    sdio1.host->max_seg_size  = 65535;
    sdio1.host->max_dma_segs  = 2;
    sdio1.host->max_blk_size  = 512;
    sdio1.host->max_blk_count = 4096;
    sdio1.host->private_data  = (void *)&sdio1; 
    
    sdio1.usdhc_host.base     = USDHC1; 
    sdio1.sdio_adma_table     = sdio1_adma_table; 
#endif 
    
#if defined(RT_USING_SDIO2) 
    sdio2.host = mmcsd_alloc_host(); 
    if(sdio2.host == RT_NULL)
    {
        SDIO_INFO("USDHC2 RTT Host alloc failed!\n"); 
        return RT_ERROR; 
    }
    else
    {
        SDIO_INFO("USDHC2 RTT Host alloc successful!\n"); 
    }
    
    sdio2.host->ops           = &rt1050_sdio_ops; 
    sdio2.host->freq_min      = USDHC_CLK_MIN_FREQ; 
    sdio2.host->freq_max      = USDHC_CLK_MAX_FREQ; 
    sdio2.host->valid_ocr     = VDD_32_33 | VDD_33_34; 
    sdio2.host->flags         = MMCSD_BUSWIDTH_4 | MMCSD_MUTBLKWRITE | MMCSD_SUP_HIGHSPEED | MMCSD_SUP_SDIO_IRQ; 
    sdio2.host->max_seg_size  = 65535;
    sdio2.host->max_dma_segs  = 2;
    sdio2.host->max_blk_size  = 512;
    sdio2.host->max_blk_count = 4096; 
    sdio2.host->private_data  = (void *)&sdio2; 
    
    sdio2.usdhc_host.base     = USDHC2; 
    sdio2.sdio_adma_table     = sdio2_adma_table; 
#endif 
    
    rt1050_sdio_clk_init(); 
    rt1050_sdio_gpio_init(); 
    rt1050_sdio_host_init(); 
    
#if defined(RT_USING_SDIO1) 
    mmcsd_change(sdio1.host); 
#endif 
    
#if defined(RT_USING_SDIO2) 
    mmcsd_change(sdio2.host); 
#endif 
    
    return RT_EOK; 
}
INIT_DEVICE_EXPORT(rt_hw_sdio_init); 

#endif

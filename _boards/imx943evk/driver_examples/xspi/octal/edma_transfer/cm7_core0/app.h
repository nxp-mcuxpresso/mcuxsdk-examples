/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_cache.h"
#include "fsl_clock.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_XSPI                    XSPI1
#define FLASH_SIZE                      0x20000 /* 1Gb */
#define EXAMPLE_XSPI_AMBA_BASE          XSPI1_AMBA_BASE
#define FLASH_PAGE_SIZE                 256
#define EXAMPLE_SECTOR                  20
#define SECTOR_SIZE                     0x1000 /* 4K */
#define EXAMPLE_XSPI_CLOCK              kCLOCK_Xspi1
#define EXAMPLE_XSPI_RX_SAMPLE_CLOCK    kXSPI_ReadSampleClkExternalInputFromDqsPad

#define FLASH_ENABLE_OCTAL_CMD   0xE7

#define NOR_CMD_LUT_SEQ_IDX_READ                   0
#define NOR_CMD_LUT_SEQ_IDX_READ_STATUS            1
#define NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI        2
#define NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE           3
#define NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_OPI       4
#define NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR           5
#define NOR_CMD_LUT_SEQ_IDX_ERASE_CHIP      	   6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL      7
#define NOR_CMD_LUT_SEQ_IDX_ENTER_OPI        	   8
#define NOR_CMD_LUT_SEQ_IDX_READ_ID_SPI            9
#define NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI            10
#define NOR_CMD_LUT_SEQ_IDX_WRITE                  11

#define CUSTOM_LUT_LENGTH        60
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0
#define FLASH_WE_STATUS_OFFSET   7

/* DMA related. */
#define EXAMPLE_XSPI_DMA (EDMA2)

#define XSPI_TX_DMA_CHANNEL 0
#define XSPI_RX_DMA_CHANNEL 1
#define XSPI_TX_DMA_REQUEST_SOURCE kDma2RequestMuxXspi1Tx
#define XSPI_RX_DMA_REQUEST_SOURCE kDma2RequestMuxXspi1Rx

#define XSPI_TX_DMA_IRQn  DMA5_2_0_1_IRQn
#define XSPI_TX_DMA_ISR   DMA5_2_0_1_DriverIRQHandler
#define XSPI_RX_DMA_IRQn  DMA5_2_0_1_IRQn
#define XSPI_RX_DMA_ISR   DMA5_2_0_1_DriverIRQHandler

#define ENABLE_SFP_CONFIG        1

#define DEMO_INVALIDATE_CACHES do { \
       SCB_InvalidateDCache();      \
} while(0)
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
typedef struct _xspi_cache_status
{
    volatile bool DCacheEnableFlag;
    volatile bool ICacheEnableFlag;
} xspi_cache_status_t;
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
static inline void xspi_clock_init(void)
{
    /*Clock setting for xspi*/
    clk_t clk = {
        .clkId = EXAMPLE_XSPI_CLOCK,
        .pclkId = kCLOCK_Syspll1dfs1,
        .rate = 400000000UL, /* 400MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    CLOCK_SetParent(&clk);
    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);
}
/*${prototype:end}*/

#endif /* _APP_H_ */

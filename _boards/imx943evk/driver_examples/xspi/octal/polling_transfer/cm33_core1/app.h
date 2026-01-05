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
#include "board.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_XSPI                 XSPI1
#define FLASH_SIZE                   0x20000 /* 1Gb*/
#define EXAMPLE_XSPI_AMBA_BASE       XSPI1_AMBA_BASE
#define FLASH_PAGE_SIZE              256
#define EXAMPLE_SECTOR               20
#define SECTOR_SIZE                  0x1000 /* 4K */
#define EXAMPLE_XSPI_CLOCK           kCLOCK_Xspi1
//#define FLASH_PORT                   kXSPI_PortA1
//#define EXAMPLE_XSPI_RX_SAMPLE_CLOCK kXSPI_ReadSampleClkLoopbackInternally
#define EXAMPLE_XSPI_RX_SAMPLE_CLOCK kXSPI_ReadSampleClkExternalInputFromDqsPad

#define FLASH_ENABLE_OCTAL_CMD          0xE7

#define NOR_CMD_LUT_SEQ_IDX_READ            	0
#define NOR_CMD_LUT_SEQ_IDX_READ_STATUS      	1
#define NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE     	2
#define NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI     	3
#define NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_OPI 	4
#define NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR     	5
#define NOR_CMD_LUT_SEQ_IDX_ERASE_CHIP      	6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL   7
#define NOR_CMD_LUT_SEQ_IDX_ENTER_OPI        	8
/* NOTE: Workaround for debugger.
   Must define AHB write FlexSPI sequence index to 9 to avoid debugger issue.
   Debugger can attach to the CM33 core only when ROM executes to certain place.
   At that point, AHB write FlexSPI sequence index is set to 9, but in LUT, the
   command is not filled by ROM. If the debugger sets software breakpoint at flash
   after reset/attachment, FlexSPI AHB write command will be triggered. It may
   cause AHB bus hang if the command in LUT sequence index 9 is any read opeartion.
   So we need to ensure at any time, the FlexSPI LUT sequence 9 for the flash must
   be set to STOP command to avoid unexpected debugger behaivor.
 */
#define NOR_CMD_LUT_SEQ_IDX_WRITE          9
#define NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI 10

#define CUSTOM_LUT_LENGTH        60
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0
#define FLASH_ERROR_STATUS_MASK  0x0e
/*
 * If cache is enabled, this example should maintain the cache to make sure
 * CPU core accesses the memory, not cache only.
 */
#define CACHE_MAINTAIN           1

#define ENABLE_SFP_CONFIG        1

#define DEMO_INVALIDATE_CACHES do { \
       XCACHE_InvalidateCache(SOC_CORE_DCACHE_CTRL);      \
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

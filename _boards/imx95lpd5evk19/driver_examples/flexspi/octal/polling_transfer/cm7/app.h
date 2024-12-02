/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_cache.h"
#include "hal_clock.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_FLEXSPI                 FLEXSPI
#define FLASH_SIZE                      0x20000 /* 1Gb*/
#define EXAMPLE_FLEXSPI_AMBA_BASE       FlexSPI_AMBA_BASE
#define FLASH_PAGE_SIZE                 256
#define EXAMPLE_SECTOR                  20
#define SECTOR_SIZE                     0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK           hal_clock_flexspi1
#define FLASH_PORT                      kFLEXSPI_PortA1
//#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkLoopbackInternally
#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkExternalInputFromDqsPad

#define FLASH_ENABLE_OCTAL_CMD          0xE7

#define NOR_CMD_LUT_SEQ_IDX_READ            0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS      1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE     2
#define NOR_CMD_LUT_SEQ_IDX_READID_OPI      3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI 4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR     5
#define NOR_CMD_LUT_SEQ_IDX_CHIPERASE       6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM     7
#define NOR_CMD_LUT_SEQ_IDX_ENTEROPI        8
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
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI 10

#define CUSTOM_LUT_LENGTH        60
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0
#define FLASH_ERROR_STATUS_MASK  0x0e
/*
 * If cache is enabled, this example should maintain the cache to make sure
 * CPU core accesses the memory, not cache only.
 */
#define CACHE_MAINTAIN           1

/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
typedef struct _flexspi_cache_status
{
    volatile bool DCacheEnableFlag;
    volatile bool ICacheEnableFlag;
} flexspi_cache_status_t;
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
static inline void flexspi_clock_init(void)
{
    /*Clock setting for flexspi*/
    hal_clk_t hal_clk = {
        .clk_id = EXAMPLE_FLEXSPI_CLOCK,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    HAL_ClockSetRootClk(&hal_clk);
}
/*${prototype:end}*/

#endif /* _APP_H_ */

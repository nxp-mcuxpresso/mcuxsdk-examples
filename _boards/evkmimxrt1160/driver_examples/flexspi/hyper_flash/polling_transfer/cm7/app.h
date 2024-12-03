/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_flexspi.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_FLEXSPI                        FLEXSPI1
#define FLASH_SIZE                             0x10000
#define EXAMPLE_FLEXSPI_AMBA_BASE              FlexSPI1_AMBA_BASE
#define FLASH_PAGE_SIZE                        512
#define EXAMPLE_SECTOR                         101
#define SECTOR_SIZE                            0x40000
#define EXAMPLE_FLEXSPI_CLOCK                  kCLOCK_Flexspi1
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA    0
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA   1
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS  2
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE 4
#define HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR 6
#define HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM 10
#define HYPERFLASH_CMD_LUT_SEQ_IDX_ERASECHIP   12
#define CUSTOM_LUT_LENGTH                      64
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
#if (defined CACHE_MAINTAIN) && (CACHE_MAINTAIN == 1)
typedef struct _flexspi_cache_status
{
    volatile bool DCacheEnableFlag;
    volatile bool ICacheEnableFlag;
} flexspi_cache_status_t;
#endif
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
static inline void flexspi_clock_init(void)
{
    /*Clock setting for flexspi1*/
    CLOCK_SetRootClockDiv(kCLOCK_Root_Flexspi1, 2);
    CLOCK_SetRootClockMux(kCLOCK_Root_Flexspi1, 0);
}

static inline void flexspi_clock_update(void)
{
    /* Program finished, speed the clock to 166M. */
    FLEXSPI_Enable(EXAMPLE_FLEXSPI, false);
    CLOCK_DisableClock(EXAMPLE_FLEXSPI_CLOCK);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Flexspi1, 0); /* flexspi clock 332M, DDR mode, internal clock 166M. */
    CLOCK_EnableClock(EXAMPLE_FLEXSPI_CLOCK);
    FLEXSPI_Enable(EXAMPLE_FLEXSPI, true);
}
/*${prototype:end}*/

#endif /* _APP_H_ */

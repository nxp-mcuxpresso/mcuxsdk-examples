/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_
/*${header:start}*/
#include "fsl_qspi.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_QSPI        QUADSPI
#define QSPI_CLK_FREQ       CLOCK_GetFreq(kCLOCK_QspiSfClk)
#define FLASH_PAGE_SIZE     256U
/* Sector size 4KB. */
#define FLASH_SECTORE_SIZE  0x1000U
/* Flash size 64MB. */
#define FLASH_SIZE          0x800000U
#define QSPI_CMD_SEQ_ERASE_SECTOR    5U
#define QSPI_CMD_SEQ_READ_STATUS_REG 10U
#define QSPI_CMD_SEQ_PROGRAM_PAGE    15U
#define QSPI_CMD_AUTO_WRITE_ENABLE   1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern uint32_t lut[FSL_FEATURE_QSPI_LUT_DEPTH];
extern qspi_flash_config_t single_config;

void BOARD_InitHardware(void);

/*${prototype:end}*/

#endif /* _APP_H_ */

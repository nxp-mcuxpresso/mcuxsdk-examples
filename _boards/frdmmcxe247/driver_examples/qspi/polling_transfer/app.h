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
#define EXAMPLE_QSPI                QuadSPI
#define QSPI_CLK_FREQ               CLOCK_GetFreq(kCLOCK_ScgSysPllAsyncDiv1Clk)
#define EXAMPLE_QSPI_HAS_SOC_CONFIG 1

/* Flash page size. */
#define FLASH_PAGE_SIZE 256U
/* Sector size 4KB. */
#define FLASH_SECTORE_SIZE 4096U
/* Flash size 8MB. */
#define FLASH_SIZE 0x800000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern uint32_t lut[FSL_FEATURE_QSPI_LUT_DEPTH];
extern qspi_flash_config_t single_config;

void BOARD_InitHardware(void);
void BOARD_QspiSocConfigure(QuadSPI_Type *base);

/*${prototype:end}*/

#endif /* _APP_H_ */

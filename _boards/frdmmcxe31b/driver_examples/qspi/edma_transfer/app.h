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
#define EXAMPLE_QSPI                QUADSPI
#define QSPI_CLK_FREQ               CLOCK_GetFreq(kCLOCK_QspiSfClk)
#define EXAMPLE_QSPI_HAS_SOC_CONFIG 1

/* Flash page size 256 bytes. */
#define FLASH_PAGE_SIZE             256U
/* Sector size 4KB. */
#define FLASH_SECTORE_SIZE          4096U
/* Flash size 8MB. */
#define FLASH_SIZE                  0x800000U

#define QSPI_CMD_SEQ_WRITE_ENABLE    5U
#define QSPI_CMD_SEQ_ERASE_SECTOR    5U
#define QSPI_CMD_SEQ_READ_STATUS_REG 10U
#define QSPI_CMD_SEQ_PROGRAM_PAGE    15U

#define QSPI_CMD_REUSE_LUT 1
#define QSPI_CMD_TYPE_WRITE_ENABLE 0
#define QSPI_CMD_TYPE_ERASE_SECTOR 1

#define EXAMPLE_DMA        EDMA
#define EXAMPLE_DMAMUX     DMAMUX_0
#define EXAMPLE_CHANNEL    0U
#define EXAMPLE_DMA_SOURCE kDmaRequestMux0QSPITx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern uint32_t lut[FSL_FEATURE_QSPI_LUT_DEPTH];
extern qspi_flash_config_t single_config;

void BOARD_InitHardware(void);
void BOARD_QspiSocConfigure(QuadSPI_Type *base);
#if defined(QSPI_CMD_REUSE_LUT) && QSPI_CMD_REUSE_LUT
void BOARD_QspiUpdateLUT(uint8_t seqID, uint8_t cmdType);
#endif

/*${prototype:end}*/

#endif /* _APP_H_ */

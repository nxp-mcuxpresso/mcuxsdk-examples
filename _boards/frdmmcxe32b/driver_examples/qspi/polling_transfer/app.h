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

#define FLASH_PAGE_SIZE 256U
/* Sector size 4KB. */
#define FLASH_SECTORE_SIZE 4096U
/* Flash size 8MB. */
#define FLASH_SIZE 0x800000U

#define QSPI_CMD_SEQ_WRITE_ENABLE    5U
#define QSPI_CMD_SEQ_ERASE_SECTOR    5U
#define QSPI_CMD_SEQ_READ_STATUS_REG 10U
#define QSPI_CMD_SEQ_PROGRAM_PAGE    15U

#define QSPI_CMD_REUSE_LUT         1
#define QSPI_CMD_TYPE_WRITE_ENABLE 0
#define QSPI_CMD_TYPE_ERASE_SECTOR 1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern uint32_t lut[FSL_FEATURE_QSPI_LUT_DEPTH];
#if defined(FLASH_NEED_DQS)
extern qspi_dqs_config_t dqsConfig;
#endif
extern qspi_flash_config_t single_config;

void BOARD_InitHardware(void);
void BOARD_QspiSocConfigure(QuadSPI_Type *base);
#if defined(QSPI_CMD_REUSE_LUT) && QSPI_CMD_REUSE_LUT
void BOARD_QspiUpdateLUT(uint8_t seqID, uint8_t cmdType);
#endif
#if defined(FLASH_ENABLE_QUAD_CMD)
void enable_quad_mode(void);
#endif
#if defined(FSL_FEATURE_QSPI_HAS_DDR) && FSL_FEATURE_QSPI_HAS_DDR
void enable_ddr_mode(void);
#endif
#if defined(FLASH_ENABLE_OCTAL_CMD)
void enable_octal_mode(void);
#endif
void erase_sector(uint32_t addr);
void erase_all(void);
void program_page(uint32_t dest_addr, uint32_t *src_addr);
void ip_read_flash(uint32_t addr, uint32_t *buffer, uint32_t size);
void qspi_nor_flash_init(QuadSPI_Type *base);

/*${prototype:end}*/

#endif /* _APP_H_ */

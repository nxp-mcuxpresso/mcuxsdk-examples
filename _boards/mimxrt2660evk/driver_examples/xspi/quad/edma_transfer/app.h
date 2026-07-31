/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
#include "board.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_XSPI MAIN__XSPI_0

/* Direct (LLC-bypass) flash window -- the same path the boot ROM XIPs
 * through, so AHB verify reads only involve the CM85 D-cache and the XSPI
 * AHB buffers, not the LLC. */
#define EXAMPLE_XSPI_AMBA_BASE 0x60000000U

/* The on-board W25H512NWEAM is driven in plain Quad I/O (1-4-4) - command on
 * 1 pad, address/data on 4 pads - which avoids the stateful Enter-QPI and keeps
 * every command reachable in SPI mode. Set to 1 to exercise QPI instead. */
#define EXAMPLE_FLASH_QPI_MODE 0

/* The boot clock (BOARD_SwitchXspi0ClockFromRam in clock_config.c) sets
 * xspi0_fclk to SYSPLLDIV4 (500 MHz) / DIV 2 -> 2x clock 250 MHz, SND_DIV 2
 * -> SCK 125 MHz. xspiRootClk below is the 2x clock (250 MHz). */
#define EXAMPLE_XSPI_ROOT_CLOCK_FREQ 250000000U

#define FLASH_SIZE      0x10000U /* W25H512: 64 MB in KB */
#define SECTOR_SIZE     0x1000U  /* 4 KB */
#define FLASH_PAGE_SIZE 256U
/* Test sector at 32 MB. This offset is DELIBERATELY beyond the 16 MB reach of a
 * 3-byte address: erase/program/read only land here if the flash is genuinely
 * in 4-byte address mode, so a passing test proves 4-byte addressing works. */
#define EXAMPLE_SECTOR  0x2000U

#define FLASH_BUSY_STATUS_POL    1U
#define FLASH_BUSY_STATUS_OFFSET 0U

/*
 * LUT sequence indices for the W25H512NWEAM Quad-I/O NOR flash.
 */
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI      0U
#define NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS    1U
#define NOR_CMD_LUT_SEQ_IDX_QPI_RESET_ENABLE   2U
#define NOR_CMD_LUT_SEQ_IDX_QPI_RESET_MEMORY   3U
#define NOR_CMD_LUT_SEQ_IDX_QPI_WRITE_ENABLE   4U
#define NOR_CMD_LUT_SEQ_IDX_QPI_ERASE_SECTOR   5U
#define NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM   6U
#define NOR_CMD_LUT_SEQ_IDX_SPI_RESET_ENABLE   7U
#define NOR_CMD_LUT_SEQ_IDX_SPI_RESET_MEMORY   8U
#define NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2       9U
#define NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_ENABLE   10U
#define NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_SR2      11U
#define NOR_CMD_LUT_SEQ_IDX_ENTER_4BYTE        12U
#define NOR_CMD_LUT_SEQ_IDX_ENTER_QPI          13U
#define NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR3       14U
#define NOR_CMD_LUT_SEQ_IDX_QPI_READ_ID        15U

#define CUSTOM_LUT_LENGTH 80U

/* DMA related: the XSPI0 TX/RX buffer requests are wired to MAIN__EDMA3
 * (channel-mux-1 request sources). The example replicates the CPU master ID
 * onto the DMA channels so XSPI/TRDC access checks see the same initiator as
 * the CPU's IP accesses. */
#define EXAMPLE_XSPI_DMA (MAIN__EDMA3)

#define XSPI_TX_DMA_CHANNEL 0U
#define XSPI_RX_DMA_CHANNEL 1U

#define XSPI_TX_DMA_IRQn MAIN_EDMA3_CH0_IRQn
#define XSPI_RX_DMA_IRQn MAIN_EDMA3_CH1_IRQn

#define XSPI_TX_DMA_REQUEST_SOURCE kDmaRequestMux1MainXSPI0Tx
#define XSPI_RX_DMA_REQUEST_SOURCE kDmaRequestMux1MainXSPI0Rx

#define DEMO_INVALIDATE_CACHES xspi_quad_invalidate_caches()

/* BOARD_InitBootClocks (run from BOARD_CommonSetting) already put xspi0_fclk
 * on SYSPLLDIV4 at the ROM's frequency; nothing further to configure. */
#define EXAMPLE_XSPI_CLOCK_INIT() \
    do                            \
    {                             \
    } while (false)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void xspi_quad_invalidate_caches(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

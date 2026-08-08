/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* XSPI1 drives the 32 MB APS512XXN Xccela PSRAM used as the LLC test memory. */
#define EXAMPLE_XSPI MAIN__XSPI_1

/* LLC instance that owns the external-memory test region on RT2660. */
#define BOARD_LLC_INSTANCE CMPT__LLC

/* Two independent HW paths to the SAME physical PSRAM cells (see board.c
 * BOARD_ConfigMPU / BOARD_EarlyConfigLLC): 0x8000_0000 bypasses the LLC,
 * 0x8800_0000 routes through the LLC. The LLC example uses only the cached
 * window so every CPU burst becomes an aligned LLC line fill. */
#define BOARD_LLC_DIRECT_BASE 0x80000000U
#define BOARD_LLC_CACHED_BASE 0x88000000U
#define BOARD_LLC_REGION_SIZE 0x02000000U /* 32 MB APS512XXN PSRAM */

/* XSPI1 AHB window base on the LLC-cached path (0x8800_0000). All CPU (AHB)
 * PSRAM accesses use this path: the LLC turns any CPU burst into aligned
 * cache-line fills, which the XSPI AHB read buffering requires - unaligned
 * bursts on the direct (LLC-bypass) 0x8000_0000 path lose/insert stale bytes
 * at internal buffer-line boundaries (measured on silicon). */
#define BOARD_XSPI1_AMBA_BASE 0x88000000U

/* Device offset 0 as seen by the IP-command engine (SFAR map, starts at the
 * direct window base). Used by the mode-register / reset / IP data sequences
 * inside the reused xspi_psram_ops.c. */
#define BOARD_XSPI1_AMBA_BASE_DIRECT 0x80000000U

/* The LLC example never re-links into PSRAM, so the test window is always
 * device offset 0 of the LLC-cached window and the IP-command base is device
 * offset 0 of the direct window. These names are what xspi_psram_ops.c
 * references. */
#define EXAMPLE_XSPI_AMBA_BASE        BOARD_XSPI1_AMBA_BASE
#define EXAMPLE_XSPI_AMBA_BASE_DIRECT BOARD_XSPI1_AMBA_BASE_DIRECT

/* LUT sequence indices (APS512XXN Xccela command set, LUT defined in the
 * board xspi_psram_ops.c). */
#define HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ  0
#define HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE 1
#define HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ    2
#define HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE   3
#define HYPERRAM_CMD_LUT_SEQ_IDX_RESET       4

#define CUSTOM_LUT_LENGTH 80
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

/* Software PSRAM bring-up, implemented by the reused board ops file. */
void xspi_hyper_ram_init(XSPI_Type *base);
/*${prototype:end}*/

#endif /* _APP_H_ */

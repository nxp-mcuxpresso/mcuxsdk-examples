/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*${macro:start}*/
#define EXAMPLE_XSPI MAIN__XSPI_1

/* The example's AHB accesses are plain memcpy with no cache maintenance,
 * so run them through the 8 MB window that BOARD_ConfigMPU maps
 * non-cacheable (0x81800000..0x81FFFFFF, upper 8 MB of the 32 MB device)
 * to stay coherent with the IP-command path. */
#define EXAMPLE_XSPI_AMBA_BASE 0x81800000U

/* Byte range swept by the example in 1 KB steps. */
#define DRAM_SIZE 0x8000U

/* Sequence indices of the LUT programmed by BOARD_Init16bitsPsRam
 * (APS256XXN Xccela command set, see board.c LUTID_APMEM_*). */
#define HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ  0 /* 20h linear burst read (AHB + IP) */
#define HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE   1 /* C0h mode register write */
#define HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ    2 /* 40h mode register read */
#define HYPERRAM_CMD_LUT_SEQ_IDX_RESET       6 /* FFh global reset */
#define HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE 9 /* A0h linear burst write (AHB + IP) */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_FLEXSPI                    BOARD_FLEXSPI_PSRAM
#define EXAMPLE_FLEXSPI_PORT               kFLEXSPI_PortB1
#define HYPERRAM_CMD_LUT_SEQ_IDX_READDATA  11
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA 12
#define HYPERRAM_CMD_LUT_SEQ_IDX_READREG   13
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG  14

#define DRAM_SIZE          0x400000U
#define EXAMPLE_DMA        DMA0
#define EXAMPLE_TX_CHANNEL 29
#define EXAMPLE_RX_CHANNEL 28
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

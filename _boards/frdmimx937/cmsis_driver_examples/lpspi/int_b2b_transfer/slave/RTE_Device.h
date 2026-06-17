/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void BOARD_InitLPSPIPins();
extern void BOARD_DeInitLPSPIPins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPSPIX_GetFreq/LPSPIX_InitPins/LPSPIX_DeinitPins for the enabled
 * LPSPI instance. */
#define RTE_SPI7        1
#define RTE_SPI7_DMA_EN 0

/* SPI configuration. */
#define RTE_SPI7_PCS_TO_SCK_DELAY       1000
#define RTE_SPI7_SCK_TO_PSC_DELAY       1000
#define RTE_SPI7_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI7_MASTER_PCS_PIN_SEL     (kLPSPI_MasterPcs0)
#define RTE_SPI7_SLAVE_PCS_PIN_SEL      (kLPSPI_SlavePcs0)
#define RTE_SPI7_PIN_INIT               BOARD_InitLPSPIPins
#define RTE_SPI7_PIN_DEINIT             BOARD_DeInitLPSPIPins

#endif /* _RTE_DEVICE_H */

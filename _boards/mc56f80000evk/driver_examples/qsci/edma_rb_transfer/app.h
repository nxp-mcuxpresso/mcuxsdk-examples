/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_QSCI                 QSCI0
#define EXAMPLE_QSCI_CLK_FREQ        CLOCK_GetIpClkSrcFreq(BOARD_DEBUG_UART_CLOCK_NAME)
#define QSCI_TX_DMA_CHANNEL          kEDMA_Channel1
#define QSCI_RX_DMA_CHANNEL          kEDMA_Channel0
#define QSCI_TX_DMAMUX_CHANNEL       kDMAMUX_DMAChannel1
#define QSCI_RX_DMAMUX_CHANNEL       kDMAMUX_DMAChannel0
#define QSCI_TX_DMA_REQUEST          kDmaRequestMuxSCI0TE
#define QSCI_RX_DMA_REQUEST          kDmaRequestMuxSCI0RF
#define EXAMPLE_QSCI_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_QSCI_DMA_BASEADDR    DMA0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

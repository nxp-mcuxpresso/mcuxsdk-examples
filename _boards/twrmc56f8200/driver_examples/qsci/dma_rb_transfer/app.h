/*
 * Copyright 2021 NXP
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
#define EXAMPLE_QSCI              QSCI1
#define EXAMPLE_QSCI_CLK_FREQ     CLOCK_GetIpClkSrcFreq(kCLOCK_QSCI1)
#define QSCI_TX_DMA_CHANNEL       kDMA_Channel1
#define QSCI_RX_DMA_CHANNEL       kDMA_Channel0
#define QSCI_TX_DMA_REQUEST       kDmaCH1_SCI1_TE
#define QSCI_RX_DMA_REQUEST       kDmaCH0_SCI1_RF
#define EXAMPLE_QSCI_DMA_BASEADDR DMA0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

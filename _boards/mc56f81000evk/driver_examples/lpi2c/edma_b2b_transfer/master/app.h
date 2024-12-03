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
#define EXAMPLE_LPI2C_MASTER         LPI2C0
#define EXAMPLE_LPI2C_MASTER_DMA     DMA0
#define EXAMPLE_LPI2C_RX_DMA_CHANNEL kEDMA_Channel0
#define EXAMPLE_LPI2C_TX_DMA_CHANNEL kEDMA_Channel0
#define EXAMPLE_LPI2C_DMAMUX         DMAMUX
#define EXAMPLE_I2C_DMAMUX_CHANNEL   kDMAMUX_DMAChannel0
#define EXAMPLE_DMA_REQUEST_SRC      kDmaRequestMuxLPI2C0MSTXRX
#define LPI2C_MASTER_CLOCK_FREQUENCY CLOCK_GetIpClkSrcFreq(kCLOCK_LPI2C0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

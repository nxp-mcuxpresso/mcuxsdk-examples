/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SLAVE              I3C0
#define I3C_SLAVE_CLOCK_FREQUENCY  24000000
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1EU
#define I3C_DATA_LENGTH            33

#define EXAMPLE_DMA                     DMA0
#define EXAMPLE_I3C_TX_DMA_CHANNEL      (0U)
#define EXAMPLE_I3C_RX_DMA_CHANNEL      (1U)
#define EXAMPLE_I3C_TX_DMA_CHANNEL_MUX  kDmaRequestMuxI3c0Tx
#define EXAMPLE_I3C_RX_DMA_CHANNEL_MUX  kDmaRequestMuxI3c0Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

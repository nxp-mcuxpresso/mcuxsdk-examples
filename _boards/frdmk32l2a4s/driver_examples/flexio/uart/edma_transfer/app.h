/*
 * Copyright 2019 NXP
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
#define BOARD_FLEXIO_BASE                   FLEXIO0
#define FLEXIO_UART_TX_PIN                  3U
#define FLEXIO_UART_RX_PIN                  2U
#define FLEXIO_CLOCK_FREQUENCY              CLOCK_GetIpFreq(kCLOCK_Flexio0)
#define FLEXIO_DMA_REQUEST_BASE             kDmaRequestMux0FlexIO0Channel0
#define EXAMPLE_FLEXIO_UART_DMAMUX_BASEADDR DMAMUX0
#define EXAMPLE_FLEXIO_UART_DMA_BASEADDR    DMA0
#define FLEXIO_UART_TX_DMA_CHANNEL          0U
#define FLEXIO_UART_RX_DMA_CHANNEL          1U
#define FLEXIO_TX_SHIFTER_INDEX             0U
#define FLEXIO_RX_SHIFTER_INDEX             1U
#define EXAMPLE_TX_DMA_SOURCE               (FLEXIO_DMA_REQUEST_BASE + FLEXIO_TX_SHIFTER_INDEX)
#define EXAMPLE_RX_DMA_SOURCE               (FLEXIO_DMA_REQUEST_BASE + FLEXIO_RX_SHIFTER_INDEX)
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

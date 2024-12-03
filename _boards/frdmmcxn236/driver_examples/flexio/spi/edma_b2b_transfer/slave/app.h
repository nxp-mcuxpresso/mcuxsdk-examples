/*
 * Copyright 2023 NXP
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
#define BOARD_FLEXIO_BASE   (FLEXIO0)
#define FLEXIO_SPI_MOSI_PIN 16U
#define FLEXIO_SPI_MISO_PIN 17U
#define FLEXIO_SPI_SCK_PIN  18U
#define FLEXIO_SPI_CSn_PIN  19U

#define EXAMPLE_FLEXIO_SPI_DMA_BASEADDR DMA0
#define FLEXIO_SPI_TX_DMA_CHANNEL       (0U)
#define FLEXIO_SPI_RX_DMA_CHANNEL       (1U)
#define FLEXIO_TX_SHIFTER_INDEX         0U
#define FLEXIO_RX_SHIFTER_INDEX         2U
#define EXAMPLE_TX_DMA_SOURCE           kDma0RequestMuxFlexIO0ShiftRegister0Request
#define EXAMPLE_RX_DMA_SOURCE           kDma0RequestMuxFlexIO0ShiftRegister2Request
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

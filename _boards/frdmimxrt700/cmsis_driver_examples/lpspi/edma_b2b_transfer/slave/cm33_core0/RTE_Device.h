/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPSPI16_InitPins();
extern void LPSPI16_DeinitPins();

/*Driver name mapping.*/
/* User needs to provide the implementation of LPSPIX_GetFreq/LPSPIX_InitPins/LPSPIX_DeinitPins for the enabled LPSPI
 * instance. */
#define RTE_SPI16        1
#define RTE_SPI16_DMA_EN 1

/* SPI configuration. */
#define RTE_SPI16_PIN_INIT               LPSPI16_InitPins
#define RTE_SPI16_PIN_DEINIT             LPSPI16_DeinitPins
#define RTE_SPI16_DMA_TX_CH              0
#define RTE_SPI16_DMA_TX_PERI_SEL        kDmaRequestMuxLpspi16Tx
#define RTE_SPI16_DMA_TX_DMA_BASE        DMA0
#define RTE_SPI16_DMA_RX_CH              1
#define RTE_SPI16_DMA_RX_PERI_SEL        kDmaRequestMuxLpspi16Rx
#define RTE_SPI16_DMA_RX_DMA_BASE        DMA0

#endif /* _RTE_DEVICE_H */

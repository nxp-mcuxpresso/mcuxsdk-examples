/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPSPI7_InitPins();
extern void LPSPI7_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPSPIX_GetFreq/LPSPIX_InitPins/LPSPIX_DeinitPins for the enabled
 * LPSPI instance. */
#define RTE_SPI7        1
#define RTE_SPI7_DMA_EN 1

/* SPI configuration. */
#define RTE_SPI7_PCS_TO_SCK_DELAY       1000
#define RTE_SPI7_SCK_TO_PSC_DELAY       1000
#define RTE_SPI7_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI7_MASTER_PCS_PIN_SEL     (kLPSPI_MasterPcs0)
#define RTE_SPI7_SLAVE_PCS_PIN_SEL      (kLPSPI_SlavePcs0)
#define RTE_SPI7_PIN_INIT               LPSPI7_InitPins
#define RTE_SPI7_PIN_DEINIT             LPSPI7_DeinitPins
#define RTE_SPI7_DMA_TX_CH              0
#define RTE_SPI7_DMA_TX_PERI_SEL        (uint8_t) Dma5RequestMuxLPSPI7Tx
#define RTE_SPI7_DMA_TX_DMA_BASE        EDMA5_2
#define RTE_SPI7_DMA_RX_CH              1
#define RTE_SPI7_DMA_RX_PERI_SEL        (uint8_t) Dma5RequestMuxLPSPI7Rx
#define RTE_SPI7_DMA_RX_DMA_BASE        EDMA5_2

#endif /* _RTE_DEVICE_H */

/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void SPI3_InitPins();
extern void SPI3_DeinitPins();
extern void SPI5_InitPins();
extern void SPI5_DeinitPins();

/*Driver name mapping.*/
/* User needs to provide the implementation of SPIX_GetFreq/SPIX_InitPins/SPIX_DeinitPins for the enabled SPI instance.
 */
#define RTE_SPI3        1
#define RTE_SPI3_DMA_EN 1
#define RTE_SPI5        1
#define RTE_SPI5_DMA_EN 1

/* SPI configuration. */

#define RTE_SPI3_SSEL_NUM        kSPI_Ssel2
#define RTE_SPI3_PIN_INIT        SPI3_InitPins
#define RTE_SPI3_PIN_DEINIT      SPI3_DeinitPins
#define RTE_SPI3_DMA_TX_CH       7
#define RTE_SPI3_DMA_TX_DMA_BASE DMA0
#define RTE_SPI3_DMA_RX_CH       6
#define RTE_SPI3_DMA_RX_DMA_BASE DMA0

#define RTE_SPI5_SSEL_NUM        kSPI_Ssel2
#define RTE_SPI5_PIN_INIT        SPI5_InitPins
#define RTE_SPI5_PIN_DEINIT      SPI5_DeinitPins
#define RTE_SPI5_DMA_TX_CH       11
#define RTE_SPI5_DMA_TX_DMA_BASE DMA0
#define RTE_SPI5_DMA_RX_CH       10
#define RTE_SPI5_DMA_RX_DMA_BASE DMA0

#endif /* _RTE_DEVICE_H */

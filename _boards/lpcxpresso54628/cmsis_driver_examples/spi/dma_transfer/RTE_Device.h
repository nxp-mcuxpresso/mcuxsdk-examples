/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void SPI2_InitPins();
extern void SPI2_DeinitPins();
extern void SPI9_InitPins();
extern void SPI9_DeinitPins();

/*Driver name mapping.*/
/* User needs to provide the implementation of SPIX_GetFreq/SPIX_InitPins/SPIX_DeinitPins for the enabled SPI instance.
 */
#define RTE_SPI2        1
#define RTE_SPI2_DMA_EN 1
#define RTE_SPI9        1
#define RTE_SPI9_DMA_EN 1

/* SPI configuration. */

#define RTE_SPI2_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI2_PIN_INIT        SPI2_InitPins
#define RTE_SPI2_PIN_DEINIT      SPI2_DeinitPins
#define RTE_SPI2_DMA_TX_CH       5
#define RTE_SPI2_DMA_TX_DMA_BASE DMA0
#define RTE_SPI2_DMA_RX_CH       4
#define RTE_SPI2_DMA_RX_DMA_BASE DMA0

#define RTE_SPI9_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI9_PIN_INIT        SPI9_InitPins
#define RTE_SPI9_PIN_DEINIT      SPI9_DeinitPins
#define RTE_SPI9_DMA_TX_CH       23
#define RTE_SPI9_DMA_TX_DMA_BASE DMA0
#define RTE_SPI9_DMA_RX_CH       22
#define RTE_SPI9_DMA_RX_DMA_BASE DMA0
#endif /* _RTE_DEVICE_H */

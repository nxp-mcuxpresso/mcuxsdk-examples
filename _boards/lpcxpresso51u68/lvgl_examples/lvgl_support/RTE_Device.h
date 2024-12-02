/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void SPI5_InitPins();
extern void SPI5_DeinitPins();
extern void I2C4_InitPins();
extern void I2C4_DeinitPins();
extern void SPI5_InitPins();
extern void SPI5_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of SPIX_GetFreq/SPIX_InitPins/SPIX_DeinitPins for the enabled SPI instance.
 */
#define RTE_SPI5            1
#define RTE_SPI5_PIN_INIT   SPI5_InitPins
#define RTE_SPI5_PIN_DEINIT SPI5_DeinitPins
#define RTE_SPI5_DMA_EN     1

/* User needs to provide the implementation of I2CX_GetFreq/I2CX_InitPins/I2CX_DeinitPins for the enabled I2C instance.
 */
#define RTE_I2C4            1
#define RTE_I2C4_PIN_INIT   I2C4_InitPins
#define RTE_I2C4_PIN_DEINIT I2C4_DeinitPins
#define RTE_I2C4_DMA_EN     0

/* SPI configuration. */
#define RTE_SPI5_SSEL_NUM        kSPI_Ssel2
#define RTE_SPI5_PIN_INIT        SPI5_InitPins
#define RTE_SPI5_PIN_DEINIT      SPI5_DeinitPins
#define RTE_SPI5_DMA_TX_CH       11
#define RTE_SPI5_DMA_TX_DMA_BASE DMA0
#define RTE_SPI5_DMA_RX_CH       10
#define RTE_SPI5_DMA_RX_DMA_BASE DMA0

#endif /* _RTE_DEVICE_H */

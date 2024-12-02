/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void SPI8_InitPins();
extern void SPI8_DeinitPins();
extern void SPI8_InitPins();
extern void SPI8_DeinitPins();
extern void I2C4_InitPins();
extern void I2C4_DeinitPins();

/*Driver name mapping*/
/* User needs to provide the implementation of SPIX_GetFreq/SPIX_InitPins/SPIX_DeinitPins for the enabled SPI instance.
 */
#define RTE_SPI8            1
#define RTE_SPI8_PIN_INIT   SPI8_InitPins
#define RTE_SPI8_PIN_DEINIT SPI8_DeinitPins
#define RTE_SPI8_DMA_EN     1

/* SPI configuration. */
#define RTE_SPI8_SSEL_NUM kSPI_Ssel1

/* SPI DMA configuration */
#if RTE_SPI8_DMA_EN
#define RTE_SPI8_PIN_INIT        SPI8_InitPins
#define RTE_SPI8_PIN_DEINIT      SPI8_DeinitPins
#define RTE_SPI8_DMA_TX_CH       3
#define RTE_SPI8_DMA_TX_DMA_BASE DMA0
#define RTE_SPI8_DMA_RX_CH       2
#define RTE_SPI8_DMA_RX_DMA_BASE DMA0
#endif

/* User needs to provide the implementation of I2CX_GetFreq/I2CX_InitPins/I2CX_DeinitPins for the enabled I2C instance.
 */
#define RTE_I2C4            1
#define RTE_I2C4_PIN_INIT   I2C4_InitPins
#define RTE_I2C4_PIN_DEINIT I2C4_DeinitPins
#define RTE_I2C4_DMA_EN     0

#endif /* _RTE_DEVICE_H */

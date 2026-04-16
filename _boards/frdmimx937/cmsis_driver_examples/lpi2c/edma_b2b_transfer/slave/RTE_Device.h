/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void BOARD_InitI2C6Pins();
extern void BOARD_DeInitI2C6Pins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPI2CX_GetFreq/LPI2CX_InitPins/LPI2CX_DeinitPins for the enabled
 * LPI2C instance. */
#define RTE_I2C6        1
#define RTE_I2C6_DMA_EN 0

/* LPI2C configuration. */

#define RTE_I2C6_PIN_INIT        BOARD_InitI2C6Pins
#define RTE_I2C6_PIN_DEINIT      BOARD_DeInitI2C6Pins
#define RTE_I2C6_DMA_TX_CH       2
#define RTE_I2C6_DMA_TX_PERI_SEL (uint8_t) Dma5RequestMuxLPI2C6Tx
#define RTE_I2C6_DMA_TX_DMA_BASE EDMA5_2
#define RTE_I2C6_DMA_RX_CH       3
#define RTE_I2C6_DMA_RX_PERI_SEL (uint8_t) Dma5RequestMuxLPI2C6Rx
#define RTE_I2C6_DMA_RX_DMA_BASE EDMA5_2

#endif /* _RTE_DEVICE_H */

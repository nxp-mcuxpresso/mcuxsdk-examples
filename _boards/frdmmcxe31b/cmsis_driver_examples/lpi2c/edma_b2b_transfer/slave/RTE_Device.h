/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void BOARD_InitI2CPins();
extern void BOARD_InitI2CPins_deinit();

/* Driver name mapping. */
/* User needs to provide the implementation of LPI2CX_GetFreq/LPI2CX_InitPins/LPI2CX_DeinitPins for the enabled LPI2C
 * instance. */
#define RTE_I2C1        1
#define RTE_I2C1_DMA_EN 0

/* LPI2C configuration. */
#define RTE_I2C1_PIN_INIT        BOARD_InitI2CPins
#define RTE_I2C1_PIN_DEINIT      BOARD_InitI2CPins_deinit
#define RTE_I2C1_DMA_TX_CH       16
#define RTE_I2C1_DMA_TX_PERI_SEL (uint16_t) kDmaRequestMux1LPI2C1Tx
#define RTE_I2C1_DMA_TX_DMAMUX_BASE DMAMUX_1
#define RTE_I2C1_DMA_TX_DMA_BASE EDMA
#define RTE_I2C1_DMA_RX_CH       17
#define RTE_I2C1_DMA_RX_PERI_SEL (uint16_t) kDmaRequestMux1LPI2C1Rx
#define RTE_I2C1_DMA_RX_DMAMUX_BASE DMAMUX_1
#define RTE_I2C1_DMA_RX_DMA_BASE EDMA

#endif /* _RTE_DEVICE_H */

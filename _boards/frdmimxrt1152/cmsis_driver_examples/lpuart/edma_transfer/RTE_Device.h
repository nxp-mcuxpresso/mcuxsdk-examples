/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void BOARD_InitDEBUG_UARTPins();
extern void BOARD_InitDEBUG_UARTPins_deinit();

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART1         1
#define RTE_USART1_DMA_EN  1

/* UART configuration. */
#define RTE_USART1_PIN_INIT        BOARD_InitDEBUG_UARTPins
#define RTE_USART1_PIN_DEINIT      BOARD_InitDEBUG_UARTPins_deinit
#define RTE_USART1_DMA_TX_CH       0
#define RTE_USART1_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMuxLPUART1Tx
#if __CORTEX_M == 7
#define RTE_USART1_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_USART1_DMA_TX_DMA_BASE    DMA0
#else
#define RTE_USART1_DMA_TX_DMAMUX_BASE DMAMUX1
#define RTE_USART1_DMA_TX_DMA_BASE    DMA1
#endif
#define RTE_USART1_DMA_RX_CH       1
#define RTE_USART1_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMuxLPUART1Rx
#if __CORTEX_M == 7
#define RTE_USART1_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_USART1_DMA_RX_DMA_BASE    DMA0
#else
#define RTE_USART1_DMA_RX_DMAMUX_BASE DMAMUX1
#define RTE_USART1_DMA_RX_DMA_BASE    DMA1
#endif

#endif /* _RTE_DEVICE_H */

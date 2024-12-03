/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

#include "board.h"

#if BOARD_DEBUG_UART_INSTANCE == 1

extern void LPUART1_InitPins();
extern void LPUART1_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART1        1
#define RTE_USART1_DMA_EN 1

/* UART configuration. */

#define RTE_USART1_PIN_INIT        LPUART1_InitPins
#define RTE_USART1_PIN_DEINIT      LPUART1_DeinitPins
#define RTE_USART1_DMA_RX_CH       (uint8_t) Dma3RequestMuxLPUART1Rx
#define RTE_USART1_DMA_RX_PERI_SEL (uint8_t) Dma3RequestMuxLPUART1Rx
#define RTE_USART1_DMA_RX_DMA_BASE DMA3
#define RTE_USART1_DMA_TX_CH       (uint8_t) Dma3RequestMuxLPUART1Tx
#define RTE_USART1_DMA_TX_PERI_SEL (uint8_t) Dma3RequestMuxLPUART1Tx
#define RTE_USART1_DMA_TX_DMA_BASE DMA3

#elif BOARD_DEBUG_UART_INSTANCE == 2

extern void LPUART2_InitPins();
extern void LPUART2_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART2        1
#define RTE_USART2_DMA_EN 1

/* UART configuration. */

#define RTE_USART2_PIN_INIT        LPUART2_InitPins
#define RTE_USART2_PIN_DEINIT      LPUART2_DeinitPins
#define RTE_USART2_DMA_RX_CH       (uint8_t) Dma3RequestMuxLPUART2Rx
#define RTE_USART2_DMA_RX_PERI_SEL (uint8_t) Dma3RequestMuxLPUART2Rx
#define RTE_USART2_DMA_RX_DMA_BASE DMA3
#define RTE_USART2_DMA_TX_CH       (uint8_t) Dma3RequestMuxLPUART2Tx
#define RTE_USART2_DMA_TX_PERI_SEL (uint8_t) Dma3RequestMuxLPUART2Tx
#define RTE_USART2_DMA_TX_DMA_BASE DMA3

#elif BOARD_DEBUG_UART_INSTANCE == 3

extern void LPUART3_InitPins();
extern void LPUART3_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART3        1
#define RTE_USART3_DMA_EN 1

/* UART configuration. */

#define RTE_USART3_PIN_INIT        LPUART3_InitPins
#define RTE_USART3_PIN_DEINIT      LPUART3_DeinitPins
#define RTE_USART3_DMA_RX_CH       (uint8_t) Dma5RequestMuxLPUART3Rx
#define RTE_USART3_DMA_RX_PERI_SEL (uint8_t) Dma5RequestMuxLPUART3Rx
#define RTE_USART3_DMA_RX_DMA_BASE EDMA5_2
#define RTE_USART3_DMA_TX_CH       (uint8_t) Dma5RequestMuxLPUART3Tx
#define RTE_USART3_DMA_TX_PERI_SEL (uint8_t) Dma5RequestMuxLPUART3Tx
#define RTE_USART3_DMA_TX_DMA_BASE EDMA5_2

#endif

#endif /* _RTE_DEVICE_H */

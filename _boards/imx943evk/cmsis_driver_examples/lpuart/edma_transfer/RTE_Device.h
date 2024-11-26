/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

#include "board.h"

#if BOARD_DEBUG_UART_INSTANCE == 1

/* Driver name mapping. */
/* User needs to provide the implementation of BOARD_LpuartXGetFreq/BOARD_InitLpuartXPins/BOARD_DeinitLpuartXPins for the enabled
 * LPUART instance. */
#define RTE_USART1        1
#define RTE_USART1_DMA_EN 1

/* UART configuration. */

#define RTE_USART1_PIN_INIT        BOARD_InitLpuart1Pins
#define RTE_USART1_PIN_DEINIT      BOARD_DeinitLpuart1Pins
#define RTE_USART1_DMA_RX_CH       (uint8_t) kDma1RequestMuxLpuart1Rx
#define RTE_USART1_DMA_RX_PERI_SEL (uint8_t) kDma1RequestMuxLpuart1Rx
#define RTE_USART1_DMA_RX_DMA_BASE EDMA1
#define RTE_USART1_DMA_TX_CH       (uint8_t) kDma1RequestMuxLpuart1Tx
#define RTE_USART1_DMA_TX_PERI_SEL (uint8_t) kDma1RequestMuxLpuart1Tx
#define RTE_USART1_DMA_TX_DMA_BASE EDMA1

#elif BOARD_DEBUG_UART_INSTANCE == 2

/* Driver name mapping. */
/* User needs to provide the implementation of BOARD_LpuartXGetFreq/BOARD_InitLpuartXPins/BOARD_DeinitLpuartXPins for the enabled
 * LPUART instance. */
#define RTE_USART2        1
#define RTE_USART2_DMA_EN 1

/* UART configuration. */

#define RTE_USART2_PIN_INIT        BOARD_InitLpuart2Pins
#define RTE_USART2_PIN_DEINIT      BOARD_DeinitLpuart2Pins
#define RTE_USART2_DMA_RX_CH       (uint8_t) kDma1RequestMuxLpuart2Rx
#define RTE_USART2_DMA_RX_PERI_SEL (uint8_t) kDma1RequestMuxLpuart2Rx
#define RTE_USART2_DMA_RX_DMA_BASE EDMA1
#define RTE_USART2_DMA_TX_CH       (uint8_t) kDma1RequestMuxLpuart2Tx
#define RTE_USART2_DMA_TX_PERI_SEL (uint8_t) kDma1RequestMuxLpuart2Tx
#define RTE_USART2_DMA_TX_DMA_BASE EDMA1

#elif BOARD_DEBUG_UART_INSTANCE == 3

/* Driver name mapping. */
/* User needs to provide the implementation of BOARD_LpuartXGetFreq/BOARD_InitLpuartXPins/BOARD_DeinitLpuartXPins for the enabled
 * LPUART instance. */
#define RTE_USART3        1
#define RTE_USART3_DMA_EN 1

/* UART configuration. */

#define RTE_USART3_PIN_INIT        BOARD_InitLpuart3Pins
#define RTE_USART3_PIN_DEINIT      BOARD_DeinitLpuart3Pins
#define RTE_USART3_DMA_RX_CH       (uint8_t) 0
#define RTE_USART3_DMA_RX_PERI_SEL (uint8_t) kDma2RequestMuxLpuart3Rx
#define RTE_USART3_DMA_RX_DMA_BASE EDMA2
#define RTE_USART3_DMA_TX_CH       (uint8_t) 1
#define RTE_USART3_DMA_TX_PERI_SEL (uint8_t) kDma2RequestMuxLpuart3Tx
#define RTE_USART3_DMA_TX_DMA_BASE EDMA2

#elif BOARD_DEBUG_UART_INSTANCE == 4

/* Driver name mapping. */
/* User needs to provide the implementation of BOARD_LpuartXGetFreq/BOARD_InitLpuartXPins/BOARD_DeinitLpuartXPins for the enabled
 * LPUART instance. */
#define RTE_USART4        1
#define RTE_USART4_DMA_EN 1

/* UART configuration. */

#define RTE_USART4_PIN_INIT        BOARD_InitLpuart4Pins
#define RTE_USART4_PIN_DEINIT      BOARD_DeinitLpuart4Pins
#define RTE_USART4_DMA_RX_CH       (uint8_t) 0
#define RTE_USART4_DMA_RX_PERI_SEL (uint8_t) kDma3RequestMuxLpuart4Rx
#define RTE_USART4_DMA_RX_DMA_BASE EDMA2
#define RTE_USART4_DMA_TX_CH       (uint8_t) 1
#define RTE_USART4_DMA_TX_PERI_SEL (uint8_t) kDma3RequestMuxLpuart4Tx
#define RTE_USART4_DMA_TX_DMA_BASE EDMA2

#elif BOARD_DEBUG_UART_INSTANCE == 5

/* Driver name mapping. */
/* User needs to provide the implementation of BOARD_LpuartXGetFreq/BOARD_InitLpuartXPins/BOARD_DeinitLpuartXPins for the enabled
 * LPUART instance. */
#define RTE_USART5        1
#define RTE_USART5_DMA_EN 1

/* UART configuration. */

#define RTE_USART5_PIN_INIT        BOARD_InitLpuart5Pins
#define RTE_USART5_PIN_DEINIT      BOARD_DeinitLpuart5Pins
#define RTE_USART5_DMA_RX_CH       (uint8_t) 0
#define RTE_USART5_DMA_RX_PERI_SEL (uint8_t) kDma2RequestMuxLpuart5Rx
#define RTE_USART5_DMA_RX_DMA_BASE EDMA2
#define RTE_USART5_DMA_TX_CH       (uint8_t) 1
#define RTE_USART5_DMA_TX_PERI_SEL (uint8_t) kDma2RequestMuxLpuart5Tx
#define RTE_USART5_DMA_TX_DMA_BASE EDMA2

#elif BOARD_DEBUG_UART_INSTANCE == 8

/* Driver name mapping. */
/* User needs to provide the implementation of BOARD_LpuartXGetFreq/BOARD_InitLpuartXPins/BOARD_DeinitLpuartXPins for the enabled
 * LPUART instance. */
#define RTE_USART8        1
#define RTE_USART8_DMA_EN 1

/* UART configuration. */

#define RTE_USART8_PIN_INIT        BOARD_InitLpuart8Pins
#define RTE_USART8_PIN_DEINIT      BOARD_DeinitLpuart8Pins
#define RTE_USART8_DMA_RX_CH       (uint8_t) 0
#define RTE_USART8_DMA_RX_PERI_SEL (uint8_t) kDma3RequestMuxLpuart8Rx
#define RTE_USART8_DMA_RX_DMA_BASE EDMA3
#define RTE_USART8_DMA_TX_CH       (uint8_t) 1
#define RTE_USART8_DMA_TX_PERI_SEL (uint8_t) kDma3RequestMuxLpuart8Tx
#define RTE_USART8_DMA_TX_DMA_BASE EDMA3

#elif BOARD_DEBUG_UART_INSTANCE == 11

/* Driver name mapping. */
/* User needs to provide the implementation of BOARD_LpuartXGetFreq/BOARD_InitLpuartXPins/BOARD_DeinitLpuartXPins for the enabled
 * LPUART instance. */
#define RTE_USART11        1
#define RTE_USART11_DMA_EN 1

/* UART configuration. */

#define RTE_USART11_PIN_INIT        BOARD_InitLpuart11Pins
#define RTE_USART11_PIN_DEINIT      BOARD_DeinitLpuart11Pins
#define RTE_USART11_DMA_RX_CH       (uint8_t) 0
#define RTE_USART11_DMA_RX_PERI_SEL (uint8_t) kDma3RequestMuxLpuart11Rx
#define RTE_USART11_DMA_RX_DMA_BASE EDMA3
#define RTE_USART11_DMA_TX_CH       (uint8_t) 1
#define RTE_USART11_DMA_TX_PERI_SEL (uint8_t) kDma3RequestMuxLpuart11Tx
#define RTE_USART11_DMA_TX_DMA_BASE EDMA3

#elif BOARD_DEBUG_UART_INSTANCE == 12

/* Driver name mapping. */
/* User needs to provide the implementation of BOARD_LpuartXGetFreq/BOARD_InitLpuartXPins/BOARD_DeinitLpuartXPins for the enabled
 * LPUART instance. */
#define RTE_USART12        1
#define RTE_USART12_DMA_EN 1

/* UART configuration. */

#define RTE_USART12_PIN_INIT        BOARD_InitLpuart12Pins
#define RTE_USART12_PIN_DEINIT      BOARD_DeinitLpuart12Pins
#define RTE_USART12_DMA_RX_CH       (uint8_t) 0
#define RTE_USART12_DMA_RX_PERI_SEL (uint8_t) kDma3RequestMuxLpuart12Rx
#define RTE_USART12_DMA_RX_DMA_BASE EDMA3
#define RTE_USART12_DMA_TX_CH       (uint8_t) 1
#define RTE_USART12_DMA_TX_PERI_SEL (uint8_t) kDma3RequestMuxLpuart12Tx
#define RTE_USART12_DMA_TX_DMA_BASE EDMA3

#else
#error "Pls define RTE macros"
#endif

#endif /* _RTE_DEVICE_H */

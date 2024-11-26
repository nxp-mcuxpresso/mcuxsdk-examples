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
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART1        1
#define RTE_USART1_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART1_RX_BUFFER_ENABLE 1

#define RTE_USART1_PIN_INIT   BOARD_InitLpuart1Pins
#define RTE_USART1_PIN_DEINIT BOARD_DeinitLpuart1Pins

#elif BOARD_DEBUG_UART_INSTANCE == 2

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART2        1
#define RTE_USART2_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART2_RX_BUFFER_ENABLE 1

#define RTE_USART2_PIN_INIT   BOARD_InitLpuart2Pins
#define RTE_USART2_PIN_DEINIT BOARD_DeinitLpuart2Pins

#elif BOARD_DEBUG_UART_INSTANCE == 3

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART3        1
#define RTE_USART3_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART3_RX_BUFFER_ENABLE 1

#define RTE_USART3_PIN_INIT   BOARD_InitLpuart3Pins
#define RTE_USART3_PIN_DEINIT BOARD_DeinitLpuart3Pins

#elif BOARD_DEBUG_UART_INSTANCE == 4

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART4        1
#define RTE_USART4_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART4_RX_BUFFER_ENABLE 1

#define RTE_USART4_PIN_INIT   BOARD_InitLpuart4Pins
#define RTE_USART4_PIN_DEINIT BOARD_DeinitLpuart4Pins



#elif BOARD_DEBUG_UART_INSTANCE == 5

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART5        1
#define RTE_USART5_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART5_RX_BUFFER_ENABLE 1

#define RTE_USART5_PIN_INIT   BOARD_InitLpuart5Pins
#define RTE_USART5_PIN_DEINIT BOARD_DeinitLpuart5Pins

#elif BOARD_DEBUG_UART_INSTANCE == 8

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART8        1
#define RTE_USART8_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART8_RX_BUFFER_ENABLE 1

#define RTE_USART8_PIN_INIT   BOARD_InitLpuart8Pins
#define RTE_USART8_PIN_DEINIT BOARD_DeinitLpuart8Pins

#elif BOARD_DEBUG_UART_INSTANCE == 11

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART11        1
#define RTE_USART11_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART11_RX_BUFFER_ENABLE 1

#define RTE_USART11_PIN_INIT   BOARD_InitLpuart11Pins
#define RTE_USART11_PIN_DEINIT BOARD_DeinitLpuart11Pins

#elif BOARD_DEBUG_UART_INSTANCE == 12

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART12        1
#define RTE_USART12_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART12_RX_BUFFER_ENABLE 1

#define RTE_USART12_PIN_INIT   BOARD_InitLpuart12Pins
#define RTE_USART12_PIN_DEINIT BOARD_DeinitLpuart12Pins

#else
#error "Pls define RTE macros"
#endif
#endif /* _RTE_DEVICE_H */

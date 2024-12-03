/*
 * Copyright 2023 NXP
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
#define RTE_USART1_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART1_RX_BUFFER_ENABLE 1

#define RTE_USART1_PIN_INIT   LPUART1_InitPins
#define RTE_USART1_PIN_DEINIT LPUART1_DeinitPins

#elif BOARD_DEBUG_UART_INSTANCE == 2

extern void LPUART2_InitPins();
extern void LPUART2_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART2        1
#define RTE_USART2_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART2_RX_BUFFER_ENABLE 1

#define RTE_USART2_PIN_INIT   LPUART2_InitPins
#define RTE_USART2_PIN_DEINIT LPUART2_DeinitPins

#elif BOARD_DEBUG_UART_INSTANCE == 3

extern void LPUART3_InitPins();
extern void LPUART3_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART3        1
#define RTE_USART3_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART3_RX_BUFFER_ENABLE 1

#define RTE_USART3_PIN_INIT   LPUART3_InitPins
#define RTE_USART3_PIN_DEINIT LPUART3_DeinitPins

#endif
#endif /* _RTE_DEVICE_H */

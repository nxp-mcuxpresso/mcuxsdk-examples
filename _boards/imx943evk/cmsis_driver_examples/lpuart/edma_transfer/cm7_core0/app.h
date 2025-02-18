/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_lpuart_cmsis.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#if BOARD_DEBUG_UART_INSTANCE == 1

#define DEMO_USART                 Driver_USART1
#define EXAMPLE_USART_DMA_BASEADDR EDMA1

#elif BOARD_DEBUG_UART_INSTANCE == 2

#define DEMO_USART                 Driver_USART2
#define EXAMPLE_USART_DMA_BASEADDR EDMA1

#elif BOARD_DEBUG_UART_INSTANCE == 3

#define DEMO_USART                 Driver_USART3
#define EXAMPLE_USART_DMA_BASEADDR EDMA2

#elif BOARD_DEBUG_UART_INSTANCE == 5

#define DEMO_USART                 Driver_USART5
#define EXAMPLE_USART_DMA_BASEADDR EDMA2

#elif BOARD_DEBUG_UART_INSTANCE == 8

#define DEMO_USART                 Driver_USART8
#define EXAMPLE_USART_DMA_BASEADDR EDMA4

#elif BOARD_DEBUG_UART_INSTANCE == 11

#define DEMO_USART                 Driver_USART11
#define EXAMPLE_USART_DMA_BASEADDR EDMA2

#elif BOARD_DEBUG_UART_INSTANCE == 12

#define DEMO_USART                 Driver_USART12
#define EXAMPLE_USART_DMA_BASEADDR EDMA4

#else
#error "Pls define macros(DEMO_USART, EXAMPLE_USART_DMA_BASEADDR)"
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

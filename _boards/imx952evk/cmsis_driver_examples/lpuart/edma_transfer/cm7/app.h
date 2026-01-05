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
#define EXAMPLE_USART_DMA_BASEADDR DMA3

#elif BOARD_DEBUG_UART_INSTANCE == 2

#define DEMO_USART                 Driver_USART2
#define EXAMPLE_USART_DMA_BASEADDR DMA3

#elif BOARD_DEBUG_UART_INSTANCE == 3

#define DEMO_USART                 Driver_USART3
#define EXAMPLE_USART_DMA_BASEADDR EDMA5_2

#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

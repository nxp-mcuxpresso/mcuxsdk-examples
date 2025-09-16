/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_lpuart_cmsis.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_USART                 Driver_USART0
#define EXAMPLE_USART_DMA_BASEADDR DMA0
#define EXAMPLE_DMA_CLOCK          kCLOCK_GateDMA0
#define DEMO_LPUART_CLK_FREQ       BOARD_DEBUG_UART_CLK_FREQ
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
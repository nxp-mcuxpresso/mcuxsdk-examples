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
#define DEMO_USART                 Driver_USART5
#define EXAMPLE_USART_DMA_BASEADDR EDMA
#define EXAMPLE_USART_DMAMUX_BASEADDR DMAMUX_1
#define DEMO_LPUART_CLK_FREQ       CLOCK_GetFreq(kCLOCK_Lpuart5Clk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

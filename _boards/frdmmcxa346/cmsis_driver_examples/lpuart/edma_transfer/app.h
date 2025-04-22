/*
 * Copyright 2024 NXP
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
#define DEMO_USART                 Driver_USART2
#define EXAMPLE_USART_DMA_BASEADDR DMA0
#define EXAMPLE_DMA_CLOCK          kCLOCK_GateDMA
#define DEMO_LPUART_CLK_FREQ       CLOCK_GetLpuartClkFreq(2u)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_USART                USART3
#define DEMO_USART_CLK_SRC        kCLOCK_Flexcomm3
#define DEMO_USART_CLK_FREQ       CLOCK_GetFlexCommClkFreq(3U)
#define USART_RX_DMA_CHANNEL      6
#define USART_TX_DMA_CHANNEL      7
#define EXAMPLE_UART_DMA_BASEADDR DMA0

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

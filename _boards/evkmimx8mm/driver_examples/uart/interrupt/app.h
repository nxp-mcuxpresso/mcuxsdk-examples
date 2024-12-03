/*
 * Copyright 2018 NXP
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
#define DEMO_UART            UART4
#define DEMO_UART_CLK_FREQ   BOARD_DEBUG_UART_CLK_FREQ
#define DEMO_UART_BAUDRATE   115200U
#define DEMO_IRQn            UART4_IRQn
#define DEMO_UART_IRQHandler UART4_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

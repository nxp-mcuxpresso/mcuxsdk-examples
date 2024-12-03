/*
 * Copyright 2024 NXP
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
/* UART instance and clock */
/* UART2: PTD2,PTD3 used which is J1-5,J1-7 on FRDM-MCXC242 */
#define DEMO_UART          UART2
#define DEMO_UART_CLKSRC   UART2_CLK_SRC
#define DEMO_UART_CLK_FREQ CLOCK_GetFreq(UART2_CLK_SRC)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

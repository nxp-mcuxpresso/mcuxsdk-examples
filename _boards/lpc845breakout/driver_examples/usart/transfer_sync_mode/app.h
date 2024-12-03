/*
 * Copyright  2019 NXP
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
#define USART_MASTER_BASEADDR USART1
#define USART_SLAVE_BASEADDR  USART2
#define USART_MASTER_CLK_SRC  kCLOCK_MainClk
#define USART_MASTER_CLK_FREQ CLOCK_GetFreq(USART_MASTER_CLK_SRC)
#define USART_SLAVE_CLK_SRC   kCLOCK_MainClk
#define USART_SLAVE_CLK_FREQ  CLOCK_GetFreq(USART_SLAVE_CLK_SRC)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

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
/* CAN instance and clock */
#define EXAMPLE_MSCAN            MSCAN
#define EXAMPLE_MSCAN_CLK_FREQ   CLOCK_GetFreq(kCLOCK_ScgSysOscAsyncDiv2Clk)
#define EXAMPLE_MSCAN_IRQn       MSCAN_Rx_IRQn
#define EXAMPLE_MSCAN_IRQHandler MSCAN_Rx_IRQHandler

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

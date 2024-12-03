/*
 * Copyright 2021 NXP
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
#define BUS_CLK_FREQ            CLOCK_GetFreq(kCLOCK_BusClk)
#define PIT_CHANNEL             kPIT_Chnl_0
#define PIT_PERIOD              1000000U
#define DEMO_PIT_BASEADDR       PIT0
#define DEMO_XBAR_BASEADDR      XBAR
#define DEMO_XBAR_INPUT_SIGNAL  kXBAR_InputPitOut
#define DEMO_XBAR_OUTPUT_SIGNAL kXBAR_OutputXbDmaInt
#define DEMO_XBAR_IRQn          XBAR_IRQn
#define DEMO_XBAR_IRQHandler    XBAR_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

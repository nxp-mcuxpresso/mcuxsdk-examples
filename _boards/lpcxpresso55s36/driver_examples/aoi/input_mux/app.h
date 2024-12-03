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
#define DEMO_CMP_P_CHANNEL 0
#define DEMO_CMP_N_CHANNEL 4

#define DEMO_CMP_BASEADDR      CMP1
#define DEMO_AOI_BASEADDR      AOI0
#define DEMO_INPUTMUX_BASEADDR INPUTMUX

#define SCTIMER_CLK_FREQ        CLOCK_GetFreq(kCLOCK_BusClk)
#define DEMO_FIRST_SCTIMER_OUT  kSCTIMER_Out_0
#define DEMO_SECOND_SCTIMER_OUT kSCTIMER_Out_4

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void Inputmux_Configuration(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

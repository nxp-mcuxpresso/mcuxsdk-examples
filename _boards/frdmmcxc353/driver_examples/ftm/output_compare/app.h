/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* The Flextimer instance/channel used for board */
#define BOARD_FTM_BASEADDR    FTM0
#define BOARD_FTM_OUT_CHANNEL kFTM_Chnl_0

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_CoreSysClk)
/*${macro:end}*/

/* This board's default core clock is 96MHz. At 10Hz (the example's default 100ms period),
 * FTM_CalculateCounterClkDiv's assert fails because even the max prescaler(128)/counter(0xFFFF)
 * combination can only reach ~11.44Hz. Shorten the period so the requested frequency stays
 * above that floor. */
#define DEMO_TIMER_PERIOD_MS (50U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

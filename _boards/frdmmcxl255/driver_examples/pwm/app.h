/*
 * Copyright 2025 NXP
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
#define BOARD_PWM_BASEADDR     (EFLEXPWM0)
#define PWM_SRC_CLK_FREQ       (CLOCK_GetFreq(kCLOCK_MainClk))
#define DEMO_PWM_CLOCK_DEVIDER (kPWM_Prescale_Divide_2)
#define DEMO_PWM_FAULT_LEVEL   true
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

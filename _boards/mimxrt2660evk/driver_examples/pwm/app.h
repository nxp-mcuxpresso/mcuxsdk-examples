/*
 * Copyright 2018, 2026 NXP
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
/* The PWM base address */
#define BOARD_PWM_BASEADDR        HSP__FLEXPWM_1
#define DEMO_PWM_CLOCK_DEVIDER    kFLEXPWM_Prescale_Divide_1
#define APP_DEFAULT_PWM_FREQUENCY (2000U)
#define DEMO_PWM_FAULT_LEVEL      true

#ifdef SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY
#undef SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY
#endif

#define PWM_SRC_CLK_FREQ                       CLOCK_GetRootClockFreq(kCLOCK_Root_CGU_MAIN_ROOTCLK)
#define SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

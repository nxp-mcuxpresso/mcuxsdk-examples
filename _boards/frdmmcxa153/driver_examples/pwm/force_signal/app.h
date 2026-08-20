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
#define BOARD_PWM_BASEADDR     (FLEXPWM0)
#define PWM_SRC_CLK_FREQ       (CLOCK_GetFreq(kCLOCK_MainClk))
#define DEMO_PWM_CLOCK_DIVIDER (kPWM_Prescale_Divide_4)
#define DEMO_PWM_FAULT_LEVEL   true

/* GPIO toggled on every phase transition (Green LED: GPIO3 pin 13) */
#define DEMO_PHASE_GPIO     GPIO3
#define DEMO_PHASE_GPIO_PIN (13U)
#define DEMO_TOGGLE_PHASE_GPIO() GPIO_PortToggle(DEMO_PHASE_GPIO, 1UL << DEMO_PHASE_GPIO_PIN)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

/*
 * Copyright 2024, 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_TIME_DELAY_FOR_DUTY_CYCLE_UPDATE (2000000U)
#define DEMO_FLEXIO_BASEADDR        HSP__FLEXIO_0
#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexio0_fclk)
#define DEMO_FLEXIO_OUTPUTPIN       (8U) /* Select FLEXIO0_D08 as PWM output */

#define DEMO_FLEXIO_TIMER_CH (0U) /* Flexio timer0 used */

/* FLEXIO output PWM frequency */
#define DEMO_FLEXIO_FREQUENCY (48000U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

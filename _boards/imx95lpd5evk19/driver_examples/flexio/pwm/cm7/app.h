/*
 * Copyright 2023 NXP
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
#define DEMO_TIME_DELAY_FOR_DUTY_CYCLE_UPDATE 100000U
#define DEMO_FLEXIO_BASEADDR                  FLEXIO1
#define DEMO_FLEXIO_OUTPUTPIN                 2U /* Select flexio1_flexio2 as PWM output */
#define DEMO_FLEXIO_TIMER_CH                  0U /* Flexio timer0 used */

#define DEMO_FLEXIO_CLOCK_FREQUENCY HAL_ClockGetIpFreq(hal_clock_flexio1)
#define DEMO_FLEXIO_FREQUENCY       100000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

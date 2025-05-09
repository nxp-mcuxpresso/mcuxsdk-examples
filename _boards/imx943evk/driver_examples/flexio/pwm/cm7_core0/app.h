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
#define DEMO_FLEXIO_OUTPUTPIN                 5U /* Select flexio1_3_1_flexio5 as PWM output */
#define DEMO_FLEXIO_TIMER_CH                  0U /* Flexio timer0 used */
#define DEMO_FLEXIO_CLOCK kCLOCK_Flexio1
#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetRate(DEMO_FLEXIO_CLOCK)
#define DEMO_FLEXIO_FREQUENCY       100000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

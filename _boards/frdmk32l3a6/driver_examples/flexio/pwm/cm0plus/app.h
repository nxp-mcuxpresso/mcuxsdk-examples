/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define DEMO_FLEXIO_BASEADDR                  FLEXIO0
#define DEMO_FLEXIO_OUTPUTPIN                 5U /* Select FXIO_D5 as PWM output */
#define DEMO_FLEXIO_TIMER_CH                  0U /* Flexio timer0 used */

#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Flexio0)
#define DEMO_FLEXIO_FREQUENCY       500000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

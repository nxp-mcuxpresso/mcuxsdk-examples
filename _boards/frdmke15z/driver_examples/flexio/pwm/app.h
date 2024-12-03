/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define DEMO_FLEXIO_BASEADDR                  FLEXIO
#define DEMO_FLEXIO_OUTPUTPIN                 3U /* Select FXIO_D3 as PWM output */
#define DEMO_FLEXIO_TIMER_CH                  0U /* Flexio timer0 used */

#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Flexio0)
#define DEMO_FLEXIO_FREQUENCY       48000
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

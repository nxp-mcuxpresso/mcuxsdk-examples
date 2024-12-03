/*
 * Copyright 2024 NXP
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
#define DEMO_FLEXIO_BASEADDR  FLEXIO0
#define DEMO_FLEXIO_OUTPUTPIN 20U /* Select FXIO_D20 as PWM output */
#define DEMO_FLEXIO_TIMER_CH  0U  /* Flexio timer0 used */

#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Flexio0)
#define DEMO_FLEXIO_FREQUENCY       100000U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

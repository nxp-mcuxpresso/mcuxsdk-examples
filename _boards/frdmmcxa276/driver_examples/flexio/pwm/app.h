/*
 * Copyright 2024 NXP
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
#define DEMO_FLEXIO_OUTPUTPIN 16U /* Select FXIO_D16 as PWM output */
#define DEMO_FLEXIO_TIMER_CH  0U /* Flexio timer0 used */

#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()
#define DEMO_FLEXIO_FREQUENCY       100000U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

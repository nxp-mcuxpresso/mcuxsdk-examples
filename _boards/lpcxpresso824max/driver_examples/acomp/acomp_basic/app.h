/*
 * Copyright 2017 NXP
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
#define DEMO_ACOMP                ACOMP
#define DEMO_ACOMP_POSITIVE_INPUT 0U /* Voltage ladder output. */
#define DEMO_ACOMP_NEGATIVE_INPUT 3U /* ACMP_I3. */
#define BOARD_LED_PORT            0U
#define BOARD_LED_PIN             12U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

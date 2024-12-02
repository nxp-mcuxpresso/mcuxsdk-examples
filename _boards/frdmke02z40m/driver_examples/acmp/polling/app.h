/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ACMP_BASEADDR       ACMP0
#define DEMO_ACMP_POSITIVE_INPUT kACMP_InternalDACOutput
#define DEMO_ACMP_NEGATIVE_INPUT kACMP_ExternalReference0

#define LED_INIT() LED_RED1_INIT(LOGIC_LED_OFF)
#define LED_OFF()  LED_RED1_OFF()
#define LED_ON()   LED_RED1_ON()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

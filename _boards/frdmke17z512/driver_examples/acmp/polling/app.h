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
#define DEMO_ACMP_BASEADDR     CMP0
#define DEMO_ACMP_USER_CHANNEL 0U
#define LED_INIT()             LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED_ON()               LED_GREEN_ON()
#define LED_OFF()              LED_GREEN_OFF()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

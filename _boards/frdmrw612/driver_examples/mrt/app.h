/*
 * Copyright 2022 NXP
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
#define APP_LED_INIT   (LED_BLUE_INIT(LOGIC_LED_OFF));
#define APP_LED_ON     (LED_BLUE_ON());
#define APP_LED_TOGGLE (LED_BLUE_TOGGLE());
#define MRT_CLK_FREQ   CLOCK_GetCoreSysClkFreq()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

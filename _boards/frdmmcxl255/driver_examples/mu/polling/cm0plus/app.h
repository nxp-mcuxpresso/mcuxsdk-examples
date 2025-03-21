/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
#define LED_INIT()        LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()      LED_RED_TOGGLE()
#define APP_MU            MUB
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

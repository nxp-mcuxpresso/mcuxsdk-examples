/*
 * Copyright 2020 NXP
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
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitLED(void);
void BOARD_TurnOnLed(uint32_t index);
void BOARD_TurnOffLed(uint32_t index);
void BOARD_TurnOnAllLed(void);
void BOARD_TurnOffAllLed(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

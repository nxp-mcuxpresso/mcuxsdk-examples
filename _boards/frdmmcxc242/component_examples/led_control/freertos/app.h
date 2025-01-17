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
#define LED_TYPE_MONOCHROME_COUNT 1
#define BOARD_SOURCE_CLOCK        48000000
#define RED_INSTACE               2
#define RED_CHANNEL               0

#define BUTTON_COUNT 1
#define BUTTON_NAME  BOARD_SW2_NAME

#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

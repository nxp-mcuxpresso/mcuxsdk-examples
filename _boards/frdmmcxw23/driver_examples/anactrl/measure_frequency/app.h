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
#define DEMO_INPUTMUX                  INPUTMUX
#define DEMO_ANACTRL                   ANACTRL
#define DEMO_REFERENCE_CLOCK           kINPUTMUX_Fro1MhzToFreqmeasRef
#define DEMO_REFERENCE_CLOCK_FREQUENCY 1000000U
#define DEMO_TARGET_CLOCK              kINPUTMUX_32KhzOscToFreqmeasTarget
#define DEMO_SCALE                     11U
#define DEMO_REFERENCE_CLOCK_SOURCE    "\r\nReference clock source: FRO 1MHz clock.\r\n"
#define DEMO_TARGET_CLOCK_SOURCE       "\r\nTarget clock source: 32kHz RTC clock.\r\n"
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

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
#define DEMO_DAC_BASE                          DACA
#define DEMO_CLOCK_FREQUENCY                   CLOCK_GetFreq(kCLOCK_SysClk)
#define DEMO_CLOCK_IP_NAME                     kCLOCK_DACA
#define DEMO_TRIANGLE_WAVEFORM_FREQ            100U
#define DEMO_TRIANGLE_WAVEFORM_INIT_STEP_VALUE 4U
#define DEMO_TRIANGLE_WAVEFORM_INIT_MIN_VALUE  0U
#define DEMO_TRIANGLE_WAVEFORM_INIT_MAX_VALUE  4095U

#define DEMO_XBARA_BASE           XBARA
#define DEMO_XBARA_SWITCH_CHANNEL kXBARA_InputXbIn7
#define DEMO_XBARA_DAC_CHANNEL    kXBARA_OutputDaca12bSync

#define DEMO_SWITCH_NAME "SW3"
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

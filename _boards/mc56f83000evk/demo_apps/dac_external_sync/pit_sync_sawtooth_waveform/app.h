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
#define DEMO_CLOCK_IP_NAME                     kCLOCK_DACA
#define DEMO_SAWTOOTH_WAVEFORM_FREQ            100U
#define DEMO_SAWTOOTH_WAVEFORM_INIT_STEP_VALUE 4U
#define DEMO_SAWTOOTH_WAVEFORM_INIT_MIN_VALUE  0U
#define DEMO_SAWTOOTH_WAVEFORM_INIT_MAX_VALUE  4095U

#define DEMO_XBARA_BASE        XBARA
#define DEMO_XBARA_PIT_CHANNEL kXBARA_InputPit0SyncOut
#define DEMO_XBARA_DAC_CHANNEL kXBARA_OutputDaca12bSync

#define DEMO_PIT_BASE         PIT0
#define DEMO_PIT_PERIOD_COUNT MSEC_TO_COUNT(1000, (CLOCK_GetFreq(kCLOCK_BusClk) / (1U << kPIT_PrescalerDivBy2048)))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern void DEMO_PIT_IRQHander(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

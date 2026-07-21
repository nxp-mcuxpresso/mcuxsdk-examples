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
#define DEMO_DAC_INSTANCE               DAC0
#define DEMO_DAC_VREF                   kDAC_ReferenceVoltageSourceAlt1
#define DEMO_CTIMER_INSTANCE            CTIMER4
#define DEMO_GET_CTIMER_CLK_FREQ        CLOCK_GetCTimerClkFreq(4U)
#define DEMO_SINEWAVE_POINTS            (1000U)
#define DEMO_PI                         (3.1415926535897932384626433832795F)
#define DEMO_DAC_REF_VOLTAGE            (3.3F)
#define DEMO_DAC_MAX_VAL                (4095U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

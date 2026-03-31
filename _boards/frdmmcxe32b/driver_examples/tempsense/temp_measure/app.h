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
#define DEMO_ADC_BASE                   ADC_0
#define DEMO_USED_CHANNELS              1U
#define DEMO_ADC_CHANNEL0               49U
#define DEMO_ADC_VREF                   3.282F /* VREFH Pin voltage. */
#define DEMO_ADC_RESOLUTION             15U
#define DEMO_ADC_CLOCK_FREQ             kADC_ModuleClockFreqDivide8
#define DEMO_TEMPSENSE_BASE             TEMPSENSE
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

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
#define DEMO_LPADC_BASE ADC0
/* Use adc to measure VREF_OUT. */
#define DEMO_LPADC_USER_CHANNEL 23U
#define DEMO_LPADC_USER_CMDID   1U
/* Use VDD_ANA. */
#define DEMO_LPADC_VREF_SOURCE  kLPADC_ReferenceVoltageAlt1
/* VDD_ANA is used as the ADC reference voltage, on the LOC board, the VDD_ANA is about 3.05v. */
#define DEMO_LPADC_VREF_VOLTAGE 3.050f

#define DEMO_VREF_BASE VREF0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

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

#define DEMO_OPAMP_BASE OPAMP0

#define DEMO_LPADC_BASE                  ADC0
#define DEMO_LPADC_USER_CHANNEL1         3U  /* LPADC channel 3 connected to the OPAMP0_INT */
#define DEMO_LPADC_USER_CHANNEL2         28U /* LPADC channel 28 connected to the OPAMP0_OBS */
#define DEMO_LPADC_USER_CMDID1           1U
#define DEMO_LPADC_USER_CMDID2           2U
#define DEMO_LPADC_VREF_SOURCE           kLPADC_ReferenceVoltageAlt3
#define DEMO_LPADC_VREF_VOLTAGE          3.300F
#define DEMO_LPADC_DO_OFFSET_CALIBRATION true

#define DEMO_DAC_BASE      DAC0
#define DEMO_DAC_VREF      kDAC_ReferenceVoltageSourceAlt1
#define DEMO_DAC_VOLT_STEP 0.806F
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

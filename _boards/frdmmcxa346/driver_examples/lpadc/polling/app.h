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
#define DEMO_LPADC_BASE                  ADC1
#define DEMO_LPADC_USER_CHANNEL          8U
#define DEMO_LPADC_USER_CMDID            1U                          /* CMD1 */
#define DEMO_LPADC_VREF_SOURCE           kLPADC_ReferenceVoltageAlt3 /* VDDA */
#define DEMO_LPADC_DO_OFFSET_CALIBRATION true
#define DEMO_LPADC_USE_HIGH_RESOLUTION   true
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

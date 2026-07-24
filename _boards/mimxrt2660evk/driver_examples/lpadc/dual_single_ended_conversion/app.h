/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPADC_BASE          HSP__ADC_0
#define DEMO_LPADC_USER_CHANNELA 0U  /* CH0A: PIO3_0 (J94-13) */
#define DEMO_LPADC_USER_CHANNELB 1U  /* CH1B: PIO3_3 (J94-16) */
#define DEMO_LPADC_USER_CMDID    1U
#define DEMO_LPADC_VREF_SOURCE             kLPADC_ReferenceVoltageAlt1
#define DEMO_LPADC_USE_HIGH_RESOLUTION     true
#define DEMO_LPADC_OFFSET_CALIBRATION_MODE kLPADC_OffsetCalibration16bitMode
#define DEMO_LPADC_DO_OFFSET_CALIBRATION   true
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

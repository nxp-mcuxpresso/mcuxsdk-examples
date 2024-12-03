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
#define DEMO_CADC_BASEADDR             ADC
#define DEMO_CADC_SAMPLE0_CHANNEL_NAME kCADC_SingleEndANA4_DiffANA4pANA5n
#define DEMO_CADC_SAMPLE1_CHANNEL_NAME kCADC_SingleEndANA6_DiffANA6pANA7n
#define DEMO_CADC_SAMPLE8_CHANNEL_NAME kCADC_SingleEndANB4_DiffANB4pANB5n
#define DEMO_CADC_SAMPLE9_CHANNEL_NAME kCADC_SingleEndANB2_DiffANB2pANB3n
#define DEMO_CADC_SAMPLE0_CHANNEL_MODE kCADC_ANA4_5_FullyDifferential
#define DEMO_CADC_SAMPLE1_CHANNEL_MODE kCADC_ANA6_7_FullyDifferential
#define DEMO_CADC_SAMPLE8_CHANNEL_MODE kCADC_ANB4_5_FullyDifferential
#define DEMO_CADC_SAMPLE9_CHANNEL_MODE kCADC_ANB2_3_FullyDifferential

#define DEMO_CLOCK_IP_NAME kCLOCK_CYCADC

#define DEMO_ADC_CC0_IRQn ADC_CC0_IRQn
#define DEMO_ADC_CC1_IRQn ADC_CC1_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern void CADC_ConverterA_End_Scan_IRQHandler(void);
extern void CADC_ConverterB_End_Scan_IRQHandler(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

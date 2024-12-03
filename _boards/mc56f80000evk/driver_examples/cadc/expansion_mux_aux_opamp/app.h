/*
 * Copyright 2022 NXP
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
#define DEMO_CADC_BASEADDR ADC
#define DEMO_CLOCK_IP_NAME kCLOCK_CYCADC
#define DEMO_ADC_CC0_IRQn  ADC_CC0_IRQn

#define CDAC_THERMISTOR_CHANNEL_MODE   kCADC_ANA4_5_SingleEnd
#define CADC_THERMISTOR_CHANNEL_NUMBER kCADC_SingleEndANA4_DiffANA4pANA5n

#define DEMO_OPAMP OPAMPA

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern void CADC_ConverterA_End_Scan_IRQHandler(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

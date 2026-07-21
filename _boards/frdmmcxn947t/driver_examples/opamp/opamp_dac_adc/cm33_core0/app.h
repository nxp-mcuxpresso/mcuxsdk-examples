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

#define DEMO_OPAMP_BASEADDR OPAMP0

#define DEMO_LPADC_BASE          ADC1
#define DEMO_LPADC_USER_CHANNELA 1U
#define DEMO_LPADC_USER_CHANNELB 24U
#define DEMO_LPADC_USER_CMDID    1U
#define DEMO_LPADC_VREF_SOURCE   kLPADC_ReferenceVoltageAlt3
#define DEMO_LPADC_VREF_VOLTAGE  3.300f

#define DEMO_DAC_BASEADDR  DAC0
#define DEMO_DAC_VREF      kDAC_ReferenceVoltageSourceAlt1
#define DEMO_DAC_VOLT_STEP 0.806f

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

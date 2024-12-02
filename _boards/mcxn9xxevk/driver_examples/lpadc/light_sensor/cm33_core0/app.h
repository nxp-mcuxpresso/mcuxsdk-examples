/*
 * Copyright 2023 NXP
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
#define DEMO_VREF_BASE  VREF0
#define DEMO_SPC_BASE   SPC0
#define DEMO_LPADC_BASE ADC1
/* Use VREF_OUT driven from the VREF block as the reference volatage,
   note that the bit combinations for controlling the LPADC reference voltage
   on different chips are different, see chip Reference Manual for details. */
#define DEMO_LPADC_VREF_SOURCE    kLPADC_ReferenceVoltageAlt2
#define DEMO_LPADC_USER_TRIGGERID 0U
#define DEMO_LPADC_USER_CMDID     1U
#define DEMO_LPADC_USER_FIFOID    0U
#define DEMO_LPADC_USER_CHANNEL   0U
#define DEMO_LPADC_OFFSET_VALUE_A 0x10U
#define DEMO_LPADC_OFFSET_VALUE_B 0x10U

/* Set VREF output voltage to 1.8v. */
#define DEMO_VREF_OUTPUT_VOLTAGE 0x8U

/* The user needs to set this macro parameter according to the lpadc resolution of the chip used. */
#define DEMO_LPADC_FULL_RANGE 65536U
/* Different lpadc resolution cause different result shift, see chip reference manual for detail. */
#define DEMO_LPADC_RESULT_SHIFT 0U
/* Single-end mode, only A-side channel is converted. */
#define DEMO_LPADC_SAMPLE_CHANNEL_MODE kLPADC_SampleChannelSingleEndSideA
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

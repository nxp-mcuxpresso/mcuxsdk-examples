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
#define DEMO_LPADC_BASE                  ADC0
#define DEMO_LPADC_IRQn                  ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC      ADC0_IRQHandler
#define DEMO_LPADC_VREF_SOURCE           kLPADC_ReferenceVoltageAlt3
#define DEMO_LPADC_DO_OFFSET_CALIBRATION true

#define DEMO_SW_NAME BOARD_SW2_NAME

#define DEMO_HIGH_PRIORITY_TRIGGER_ID (1U)
#define DEMO_HIGH_PRIORITY_CMAD_ID    (2U)
#define DEMO_LPADC_VDD_CHANNEL        29U

#define DEMO_LOW_PRIORITY_TRIGGER_ID (0U)
#define DEMO_LOW_PRIORITY_CMAD_ID    (1U)
#define DEMO_LPADC_EXTERNAL_CHANNEL  0U

#define DEMO_LOOP_COUNT (4U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitTimer(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

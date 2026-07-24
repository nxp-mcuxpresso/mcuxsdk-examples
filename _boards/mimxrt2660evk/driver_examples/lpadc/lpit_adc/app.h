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
#define LPIT_CLK_FREQ        CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpit0_fclk)
#define DEMO_LPIT_BASE       HSP__LPIT_0
#define LPIT_CHANNEL         kLPIT_Chnl_0
#define DEMO_LPIT_IRQn       HSP_LPIT0_IRQn
#define DEMO_LPIT_IRQHandler HSP_LPIT0_IRQHandler
#define LPIT_PERIOD          1000000U

#define DEMO_LPADC_BASE             HSP__ADC_0
#define DEMO_LPADC_IRQn             HSP_ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC HSP_ADC0_IRQHandler
#define DEMO_LPADC_USER_CHANNEL     0U  /* CH0A: PIO3_0 (J94-13) */
#define DEMO_LPADC_USER_CMDID       1U
#define DEMO_LPADC_VREF_SOURCE kLPADC_ReferenceVoltageAlt1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

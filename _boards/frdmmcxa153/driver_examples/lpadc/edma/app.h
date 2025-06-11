/*
 * Copyright 2025 NXP
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

/* ADC Defines */
#define DEMO_LPADC_BASE                  ADC0
#define DEMO_LPADC_IRQn                  ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC      ADC0_IRQHandler
#define DEMO_LPADC_USER_CHANNEL          0U
#define DEMO_LPADC_USER_CMDID            1U /* CMD1 */
#define DEMO_LPADC_VREF_SOURCE           kLPADC_ReferenceVoltageAlt3
#define DEMO_LPADC_DO_OFFSET_CALIBRATION true
#define DEMO_LPADC_USE_HIGH_RESOLUTION   true

/* EDMA Defines */
#define DEMO_DMA_BASEADDR    DMA0
#define DEMO_DMA_CHANNEL_0   0U
#define DEMO_DMA_IRQ         DMA_CH0_IRQn
#define DEMO_DMA_IRQ_HANDLER DMA_CH0_IRQHandler
#define BUFFER_LENGTH        50U
#define DEMO_DMA_REQUEST     kDma0RequestMuxAdc0FifoRequest

/* Low power timer for ADC Trigger */
#define LPTMR_TRIG_BASE         LPTMR0
#define LPTMR_TRIG_USEC_COUNT   1000U
#define LPTMR_TRIG_IRQn         LPTMR0_IRQn
#define LPTMR_TRIG_HANDLER      LPTMR0_IRQHandler
#define ADC_LPTMR_TRIG_CLOCK    kLPTMR_PrescalerClock_1
#define LPTMR_TRIG_SOURCE_CLOCK (16000U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

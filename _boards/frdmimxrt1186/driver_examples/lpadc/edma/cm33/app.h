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

/* ADC Defines */
#define DEMO_LPADC_BASE                  ADC1
#define DEMO_LPADC_IRQn                  ADC1_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC      ADC1_IRQHandler
#define DEMO_LPADC_USER_CHANNEL          1U
#define DEMO_LPADC_USER_CMDID            1U /* CMD1 */
/* ERRATA051385: ADC INL/DNL degrade under high ADC clock frequency when VREFH selected as reference. */
#define DEMO_LPADC_VREF_SOURCE             kLPADC_ReferenceVoltageAlt2
#define DEMO_LPADC_USE_HIGH_RESOLUTION   true
#define DEMO_LPADC_OFFSET_CALIBRATION_MODE kLPADC_OffsetCalibration16bitMode
#define DEMO_LPADC_DO_OFFSET_CALIBRATION true

/* EDMA Defines */
#define DEMO_DMA_BASEADDR    DMA4
#define DEMO_DMA_CHANNEL_0   0U
#define DEMO_DMA_IRQ         DMA4_CH0_CH1_CH32_CH33_IRQn
#define DEMO_DMA_IRQ_HANDLER DMA4_CH0_CH1_CH32_CH33_IRQHandler
#define BUFFER_LENGTH        50U
#define DEMO_DMA_REQUEST     kDma4RequestMuxADC1Request0

/* Low power timer for ADC Trigger */
#define LPTMR_TRIG_BASE         LPTMR1
#define LPTMR_TRIG_USEC_COUNT   1000U
#define LPTMR_TRIG_IRQn         LPTMR1_IRQn
#define LPTMR_TRIG_HANDLER      LPTMR1_IRQHandler
#define ADC_LPTMR_TRIG_CLOCK    kLPTMR_PrescalerClock_2
#define LPTMR_TRIG_SOURCE_CLOCK (16000U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

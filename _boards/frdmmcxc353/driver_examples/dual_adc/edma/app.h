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
/* DualADC defines */
#define DEMO_DUALADC_BASE                  ADC0
#define DEMO_DUALADC_IRQn                  ADC0_A_IRQn
#define DEMO_DUALADC_IRQ_HANDLER_FUNC      ADC0_A_IRQHandler
#define DEMO_DUALADC_USER_CHANNEL_0        5U   /* ADC0_A5 */
#define DEMO_DUALADC_USER_CHANNEL_1        5U   /* ADC0_B5 */
#define DEMO_DUALADC_USER_CHANNEL_2        11U  /* ADC0_A11 */
#define DEMO_DUALADC_USER_CHANNEL_3        11U  /* ADC0_B11 */

/* eDMA defines */
#define DEMO_DMA_BASEADDR    DMA0
#define DEMO_DMA_CHANNEL_0   0U
#define DEMO_DMA_IRQ         DMA_CH0_IRQn
#define DEMO_DMA_IRQ_HANDLER DMA_CH0_IRQHandler
#define DEMO_DMA_REQUEST     kDma0RequestADC0aRequest /* ADCA (ADCA+ADCB in cooperation mode) single-end request. */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

/*
 * Copyright 2021 NXP
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
#define DEMO_ADC16_BASEADDR      ADC
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_CHANNEL       19U
#define ADC16_RESULT_REG_ADDR    (uint32_t)(&ADC->R[0]) /* Get ADC16 result register address */

#define DEMO_DMA_BASEADDR DMA0

#define DEMO_DMA_IRQ_ID           DMA0_IRQn
#define DEMO_DMA_IRQ_HANDLER_FUNC DMA0_IRQHandler
#define DEMO_DMA_CHANNEL          0U
#define DEMO_DMA_ADC_SOURCE       kDmaRequestMux0ADC

#define DEMO_DMAMUX_BASEADDR DMAMUX0

#define DEMO_LPTMR_BASE LPTMR

#define LED_INIT()   LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() LED_RED_TOGGLE()

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ConfigTriggerSource(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

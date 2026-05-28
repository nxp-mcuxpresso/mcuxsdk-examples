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
#define DEMO_ADC_BASE                   ADC
#define DEMO_ADC_IRQn                   ADC_EOC_IRQn
#define DEMO_ADC_IRQ_HANDLER_FUNC       ADC_EOC_IRQHandler
#define DEMO_ADC_USED_CHANNELS          1U
#define DEMO_ADC_CHANNEL                0U
#define DEMO_ADC_CLOCK_FREQ             kADC_FullBusFrequency
#define DEMO_ADC_RES_ARRAY_SIZE         2500

#define DEMO_LPIT_BASE                  LPIT1
#define DEMO_LPIT_IRQn                  LPIT1_IRQn
#define DEMO_LPIT_CHANNEL               kLPIT_Chnl_0
#define LPIT_MASTER_CLOCK_ROOT          kCLOCK_busaon
#define DEMO_LPIT_CLOCK_FREQ            CLOCK_GetRate(LPIT_MASTER_CLOCK_ROOT)
#define DEMO_TIMER_IRQ_HANDLER_FUNC     LPIT1_IRQHandler

#define DEMO_DMA_BASE                   DMA3
#define DEMO_DMA_CHANNEL                7U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

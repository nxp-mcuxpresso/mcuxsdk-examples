/*
 * Copyright 2024 NXP
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
#define DEMO_ADC_CHANNEL0               0U

#define DEMO_LPIT_BASE                  LPIT1
#define DEMO_LPIT_IRQn                  LPIT1_IRQn
#define DEMO_LPIT_CHANNEL               kLPIT_Chnl_0
#define LPIT_MASTER_CLOCK_ROOT          hal_clock_busaon
#define DEMO_LPIT_CLOCK_FREQ            HAL_ClockGetIpFreq(LPIT_MASTER_CLOCK_ROOT)
#define DEMO_LPIT_IRQ_HANDLER_FUNC      LPIT1_IRQHandler

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

/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ADC_BASE                   ADC_0
#define DEMO_ADC_USED_CHANNELS          1U
#define DEMO_ADC_CHANNEL                48U
#define DEMO_ADC_CLOCK_FREQ             kADC_ModuleClockFreq;
#define DEMO_ADC_RES_ARRAY_SIZE         2500

#define DEMO_PIT_BASE                   PIT_0
#define DEMO_PIT_IRQn                   PIT0_IRQn
#define DEMO_PIT_CHANNEL                kPIT_Chnl_0
#define DEMO_PIT_CLOCK_FREQ             CLOCK_GetFreq(kCLOCK_Pit0Clk)
#define DEMO_TIMER_IRQ_HANDLER_FUNC     PIT0_IRQHandler

#define DEMO_DMA_BASE                   EDMA
#define DEMO_DMA_CHANNEL                0U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

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

/* PIT setting */
#define DEMO_PIT_BASEADDR PIT0
#define BUS_CLK_FREQ      (24U * 1000U * 1000U)

/* XBAR setting */
#define DEMO_XBAR_INPUT_PIT_SIGNAL    kXBAR1_InputPitSyncOut
#define DEMO_XBAR_OUTPUT_AOI_SIGNAL_2 kXBAR1_OutputAoiSm0InputB
#define DEMO_XBAR_INPUT_AOI_SIGNAL    kXBAR1_InputAoiSm0Out
#define DEMO_XBAR_OUTPUT_SIGNAL       kXBAR1_OutputDmaReq35Int99

#define DEMO_XBAR_IRQ_HANDLER_FUNC XBAR0_IRQ0_IRQHandler
#define DEMO_XBAR_IRQ_ID           XBAR0_IRQ0_IRQn

/* AOI setting */
#define DEMO_AOI_BASEADDR AOI0

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

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

/* PIT setting */
#define DEMO_LPIT_BASE          LPIT1
#define DEMO_LPIT_CHANNEL       kLPIT_Chnl_0
#define LPIT_SOURCECLOCK        CLOCK_GetRate(kCLOCK_Busaon)

/* XBAR setting */
#define DEMO_XBAR_INPUT_PIT_SIGNAL    kXBAR2_InputLpit1LpitTrigOut0
#define DEMO_XBAR_OUTPUT_AOI_SIGNAL_2   kXBAR2_OutputAoi1In01

#define DEMO_XBAR_INPUT_AOI_SIGNAL      kXBAR1_InputAoi1AoiOut0
#define DEMO_XBAR_OUTPUT_SIGNAL         kXBAR1_OutputEdma4IpdReq76

#define DEMO_XBAR_IRQ_HANDLER_FUNC      XBAR1_CH0_CH1_IRQHandler
#define DEMO_XBAR_IRQ_ID                XBAR1_CH0_CH1_IRQn

/* AOI setting */
#define DEMO_AOI_BASEADDR               AOI1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

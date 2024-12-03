/*
 * Copyright 2023 NXP
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
/* The Flextimer instance/channel used for board */
#define DEMO_FTM_BASEADDR FTM0

/* FTM channel pair used for the dual-edge capture, channel pair 0 uses channels 1 and 2 */
#define BOARD_FTM_INPUT_CAPTURE_CHANNEL_PAIR kFTM_Chnl_0

/* Interrupt number and interrupt handler for the FTM instance used */
#define FTM_INTERRUPT_NUMBER      FTM0_IRQn
#define FTM_INPUT_CAPTURE_HANDLER FTM0_IRQHandler

/* Interrupt to enable and flag to read; depends on the FTM channel used for dual-edge capture */
#define FTM_FIRST_CHANNEL_INTERRUPT_ENABLE  kFTM_Chnl0InterruptEnable
#define FTM_FIRST_CHANNEL_FLAG              kFTM_Chnl0Flag
#define FTM_SECOND_CHANNEL_INTERRUPT_ENABLE kFTM_Chnl1InterruptEnable
#define FTM_SECOND_CHANNEL_FLAG             kFTM_Chnl1Flag

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_CoreSysClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

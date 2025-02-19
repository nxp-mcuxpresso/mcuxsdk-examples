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
/* The Flextimer base address/channel pair used for board */
#define BOARD_FTM_BASEADDR FTM3
/* FTM channel pair 3 works with FTM channels 6 and 7 */
#define BOARD_FTM_CHANNEL_PAIR kFTM_Chnl_3

/* Interrupt number and interrupt handler for the FTM base address used */
#define FTM_INTERRUPT_NUMBER FTM3_Ch6_Ch7_IRQn
#define FTM_LED_HANDLER      FTM3_Ch6_Ch7_IRQHandler

/* Interrupt to enable and flag to read */
#define FTM_CHANNEL_INTERRUPT_ENABLE kFTM_Chnl6InterruptEnable
#define FTM_CHANNEL_FLAG             kFTM_Chnl6Flag

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

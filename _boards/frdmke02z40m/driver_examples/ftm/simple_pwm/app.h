/*
 * Copyright 2017 NXP
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
#define BOARD_FTM_BASEADDR FTM2
#define BOARD_FTM_CHANNEL  kFTM_Chnl_1

/* Interrupt number and interrupt handler for the FTM instance used */
#define FTM_INTERRUPT_NUMBER FTM2_IRQn
#define FTM_LED_HANDLER      FTM2_IRQHandler

/* Interrupt to enable */
#define FTM_CHANNEL_INTERRUPT_ENABLE kFTM_TimeOverflowInterruptEnable

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

/*
 * Copyright 2019-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_GPT_TIMER_INSTANCE GPT1
#define BOARD_GPT_TIMER_IRQ      GPT1_IRQn
#define BOARD_GPT_CLOCK_SOURCE   kGPT_ClockSource_Periph
#define BOARD_TIMER_IRQ_HANDLER  GPT1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

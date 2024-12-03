/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_AFE_FIRST_CHANNEL  2U
#define DEMO_AFE_SECOND_CHANNEL 3U
/* About 23 mV */
#define DEMO_AFE_VREF_TRIM               46U
#define DEMO_AFE_BASEADDR                AFE
#define DEMO_AFE_FIRST_IRQ_HANDLER_FUNC  AFE_CH2_IRQHandler
#define DEMO_AFE_SECOND_IRQ_HANDLER_FUNC AFE_CH3_IRQHandler
#define DEMO_AFE_FIRST_CHANNEL_IRQn      AFE_CH2_IRQn
#define DEMO_AFE_SECOND_CHANNEL_IRQn     AFE_CH3_IRQn

#define DEMO_CMP_BASEADDR      CMP1
#define DEMO_CMP_PLUS_CHANNEL  2U
#define DEMO_CMP_MINUS_CHANNEL 3U

#define TMRPRCLK                 (double)(CLOCK_GetFreq(kCLOCK_BusClk) / 64)
#define DEMO_QUAD_TIMER_BASEADDR TMR0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

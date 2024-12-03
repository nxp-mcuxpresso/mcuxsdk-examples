/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define RIT_IRQ_ID RIT_IRQn
/* Get source clock for RIT driver */
#define RIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define LED_INIT()       LED1_INIT(LOGIC_LED_ON)
#define LED_TOGGLE()     LED1_TOGGLE()
#define APP_RIT_HANDLER  RIT_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

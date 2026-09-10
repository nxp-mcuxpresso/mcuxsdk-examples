/*
 * Copyright 2026 NXP
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
#define EXAMPLE_PWT_BASE       PWT0
#define EXAMPLE_PWT_INPUT_PORT kPWT_InputPort_0

/* Interrupt number for the PWT instance used */
#define PWT_INTERRUPT_NUMBER PWT0_READY_IRQn
#define PWT_EXAMPLE_HANDLER  PWT0_READY_IRQHandler

/* Get source clock for PWT driver */
#define PWT_SOURCE_CLOCK (24U * 1000U * 1000U) // CLOCK_GetFreq(kCLOCK_BusClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

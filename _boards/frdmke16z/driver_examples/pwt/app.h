/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Interrupt number for the PWT instance used */
#define PWT_INTERRUPT_NUMBER PWT_LPTMR0_IRQn
#define PWT_EXAMPLE_HANDLER  PWT_LPTMR0_IRQHandler

/* Get source clock for PWT driver */
#define PWT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

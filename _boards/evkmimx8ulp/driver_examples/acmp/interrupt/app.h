/*
 * Copyright 2021,2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ACMP_BASEADDR         CMP0
#define DEMO_ACMP_MINUS_INPUT      4U /* External input 4 for plus channel(reference input 3) */
#define DEMO_ACMP_PLUS_INPUT       7U /*  Internal 8bit DAC output. */
#define DEMO_ACMP_IRQ_ID           CMP0_IRQn
#define DEMO_ACMP_IRQ_HANDLER_FUNC CMP0_IRQHandler

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

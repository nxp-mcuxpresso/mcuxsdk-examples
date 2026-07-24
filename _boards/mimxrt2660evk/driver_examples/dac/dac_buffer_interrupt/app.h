/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_DAC_BASEADDR         HSP__DAC
#define DEMO_DAC_IRQ_ID           HSP_DAC_IRQn
#define DEMO_DAC_IRQ_HANDLER_FUNC HSP_DAC_IRQHandler
#define DEMO_DAC_VREF             kDAC_ReferenceVoltageSourceAlt1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

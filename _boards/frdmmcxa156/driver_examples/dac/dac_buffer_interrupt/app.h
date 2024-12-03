/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_DAC_BASEADDR         DAC0
#define DEMO_DAC_IRQ_ID           DAC0_IRQn
#define DEMO_DAC_IRQ_HANDLER_FUNC DAC0_IRQHandler
#define DEMO_DAC_VREF             kDAC_ReferenceVoltageSourceAlt1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

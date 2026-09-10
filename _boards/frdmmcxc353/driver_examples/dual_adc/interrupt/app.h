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
#define DEMO_DUALADC_BASE                  ADC0
#define DEMO_DUALADC_IRQn                  ADC0_A_IRQn
#define DEMO_DUALADC_IRQ_HANDLER_FUNC      ADC0_A_IRQHandler
#define DEMO_DUALADC_USER_CHANNEL          0U
#define DEMO_DUALADC_USER_CMDID            1U /* CMD1 */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

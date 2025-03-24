/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORE0_H_
#define _APP_CORE0_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_GINT0_PORT kGINT_Port0

/* Select two inputs, active low for GINT0. SW2 & SW5 must be connected to the same port */
#define DEMO_GINT0_POL_MASK ~((1U << BOARD_BUTTON1_GPIO_PIN) | (1U << BOARD_WAKEUP_GPIO_PIN))
#define DEMO_GINT0_ENA_MASK ((1U << BOARD_BUTTON1_GPIO_PIN) | (1U << BOARD_WAKEUP_GPIO_PIN))
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE0_H_ */

/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_GINT0_PORT kGINT_Port0


/* TODO: Select one input, active low for GINT0 */
#define DEMO_GINT0_POL_MASK ~(1U << BOARD_SW1_GPIO_PIN)
#define DEMO_GINT0_ENA_MASK (1U << BOARD_SW1_GPIO_PIN)

#define GINT0 CTRLSS__GINT_0

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

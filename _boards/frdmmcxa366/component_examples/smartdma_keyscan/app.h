/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
volatile uint32_t g_keyscan_gpio_register[8] = {
		0x40105000U + 0x60U + 28U,         /* ROW1, P3_28, Pin Data security Register */
		0x40105000U + 0x60U + 27U,         /* ROW2, P3_27, Pin Data security Register */
		0x40103000U + 0x60U + 5U,          /* ROW3, P1_5, Pin Data security Register */
		0x40103000U + 0x60U + 4U,          /* ROW4, P1_4, Pin Data security Register */
		0x40103000U + 0x60U + 1U,          /* COL1, P1_1, Pin Data security Register */
		0x40103000U + 0x60U + 2U,          /* COL2, P1_2, Pin Data security Register */
		0x40103000U + 0x60U + 0U,          /* COL3, P1_0, Pin Data security Register */
		0x40103000U + 0x60U + 6U,          /* COL4, P1_6, Pin Data security Register */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);

#endif /* _APP_H_ */

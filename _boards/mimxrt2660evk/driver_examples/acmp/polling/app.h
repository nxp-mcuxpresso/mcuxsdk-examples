/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//${macro:start}
#define DEMO_ACMP_BASEADDR     WAKE__ACMP_0
#define DEMO_ACMP_USER_CHANNEL 1U /* INA0: PIO3_4 (J94-17) */
#define DEMO_ACMP_MINUS_INPUT  DEMO_ACMP_USER_CHANNEL
#define DEMO_ACMP_PLUS_INPUT   7U /* Internal 8bit DAC output. */
#define DEMO_CMP_USE_VIN1      false
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//${prototype:start}
void BOARD_InitHardware(void);
//${prototype:end}

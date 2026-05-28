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
/* SAI and I2C instance and clock */
#define DEMO_SAI              SAI3

/* Get frequency of sai3 clock */
#define SAI_CLOCK_ROOT          kCLOCK_sai3
#define DEMO_SAI_CLK_FREQ       CLOCK_GetRate(SAI_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

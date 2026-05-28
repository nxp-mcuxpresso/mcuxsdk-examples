/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI and I2C instance and clock */
#define DEMO_SAI              SAI1

/* Get frequency of sai1 clock */
#define SAI_CLOCK_ROOT    kCLOCK_sai1
#define DEMO_SAI_CLK_FREQ CLOCK_GetRate(SAI_CLOCK_ROOT)
#define HMCLK_FREQ        DEMO_SAI_CLK_FREQ
#define INIT_MQS_IN_BOARD 1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

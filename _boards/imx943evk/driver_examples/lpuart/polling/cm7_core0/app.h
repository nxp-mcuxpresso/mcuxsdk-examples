/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-8-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Get source clock for LPIT driver */
#define DEMO_LPUART          LPUART11
#define DEMO_LPUART_INSTANCE_IDX (11U)
#define LPUART_CLOCK_ROOT    kCLOCK_Lpuart11
#define DEMO_LPUART_CLK_FREQ CLOCK_GetRate(LPUART_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

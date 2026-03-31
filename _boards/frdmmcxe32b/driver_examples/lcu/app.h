/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LCU              LCU_0
#define DEMO_LCU_OUTPUT       kLCU_Lc0Output0
#define DEMO_LCU_INPUT        kLCU_Lc0Input0
#define DEMO_LCU_INPUT_SOURCE kLCU_MuxSelInput0

#define DEMO_LCU_IRQn       LCU0_IRQn
#define DEMO_LCU_IRQHandler LCU0_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

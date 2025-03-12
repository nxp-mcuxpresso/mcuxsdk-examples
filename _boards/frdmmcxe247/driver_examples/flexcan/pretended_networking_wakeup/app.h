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
#define EXAMPLE_CAN                CAN0
#define TX_MESSAGE_BUFFER_NUM      (0U)
#define EXAMPLE_CAN_CLK_FREQ       CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_SetLowerPowerConfig(void);
void APP_EnterLowerPowerMode(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

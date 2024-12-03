/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_CAN                CAN1
#define TX_MESSAGE_BUFFER_NUM      (9U)
#define EXAMPLE_CAN_CLK_FREQ       CLOCK_GetFlexcanClkFreq(1U)
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

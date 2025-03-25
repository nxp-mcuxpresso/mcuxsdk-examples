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
#define STM          STM_0
#define STM_CLK_FREQ CLOCK_GetStmClkFreq(0U)
#define STM_CHANNEL_0 kSTM_Channel_0
#define STM_CHANNEL_1 kSTM_Channel_1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

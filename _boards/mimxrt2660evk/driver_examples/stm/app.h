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
#define STM           HSP__STM
#define STM_CLK_FREQ  CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_main_clk_divided)
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

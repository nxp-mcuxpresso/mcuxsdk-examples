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
/*${macro:start}*/
#define BOARD_FLEXIO_BASE      FLEXIO1

#if BOARD_IMX943_TYPE == BOARD_IMX943_EMULATOR
#warning "For Emulator"
#define FLEXIO_T_FORMAT_DR_PIN    9U
#define FLEXIO_T_FORMAT_TX_PIN    11U
#define FLEXIO_T_FORMAT_RX_PIN    8U
#else
#define FLEXIO_T_FORMAT_DR_PIN    1U
#define FLEXIO_T_FORMAT_TX_PIN    2U
#define FLEXIO_T_FORMAT_RX_PIN    3U
#endif

#define T_FORMAT_TX_SHIFTER_INDEX 0U
#define T_FORMAT_RX_SHIFTER_INDEX 1U
#define T_FORMAT_DR_TIMER_INDEX   0U
#define T_FORMAT_TX_TIMER_INDEX   1U
#define T_FORMAT_RX_TIMER_INDEX   2U
#define DEMO_DFS_DIV2_CLOCK       (hal_clock_syspll1dfs0div2)
#define DEMO_FLEXIO_CLOCK         (hal_clock_flexio1)
#define FLEXIO_CLOCK_FREQUENCY    HAL_ClockGetRate(DEMO_FLEXIO_CLOCK)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

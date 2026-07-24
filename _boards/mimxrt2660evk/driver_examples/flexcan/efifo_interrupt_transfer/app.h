/*
 * Copyright 2021, 2026 NXP
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
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define EXAMPLE_CAN                HSP__FLEXCAN_2
#else
#define EXAMPLE_CAN                HSP__FLEXCAN_1
#endif
#define TX_MESSAGE_BUFFER_NUM      (0)
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define EXAMPLE_CAN_CLK_FREQ       CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexcan2_fclk)
#else
#define EXAMPLE_CAN_CLK_FREQ       CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexcan1_fclk)
#endif
#define USE_IMPROVED_TIMING_CONFIG (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

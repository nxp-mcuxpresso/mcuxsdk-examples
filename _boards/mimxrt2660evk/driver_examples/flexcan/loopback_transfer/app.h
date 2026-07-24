/*
 * Copyright 2022, 2026 NXP
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
#define EXAMPLE_CAN HSP__FLEXCAN_2
#else
#define EXAMPLE_CAN HSP__FLEXCAN_1
#endif
#define RX_MESSAGE_BUFFER_NUM (9)
#define TX_MESSAGE_BUFFER_NUM (8)
#define DLC                   (8)

/* Get frequency of flexcan clock */
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexcan2_fclk)
#else
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexcan1_fclk)
#endif
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

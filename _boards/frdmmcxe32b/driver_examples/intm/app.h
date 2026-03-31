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
#define INTM0 INTM
#define INTM_CHANNEL      FSL_FEATURE_INTM_MONITOR_COUNT
#define INTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_AipsPlatClk)
#define BOARD_SW_EIRQ        13U /* EIRQ13*/
#define BOARD_SW_IRQ         SIUL2_1_IRQn
#define BOARD_SW_IRQ_HANDLER SIUL2_1_IRQHandler
#define BOARD_SW_NAME        "SW3"
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitSWConfig(void);
void BOARD_ClearSWIntFlag(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

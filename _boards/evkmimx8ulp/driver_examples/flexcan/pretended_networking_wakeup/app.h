/*
 * Copyright 2021 NXP
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
#define EXAMPLE_CAN           CAN0
#define TX_MESSAGE_BUFFER_NUM (0)

#define EXAMPLE_CAN_CLOCK_NAME   (kCLOCK_Flexcan)
#define EXAMPLE_CAN_CLOCK_SOURCE (kCLOCK_Pcc1BusIpSrcSysOscDiv2)
#define EXAMPLE_CAN_CLK_FREQ     (CLOCK_GetFlexcanClkFreq())

/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)

#define EXAMPLE_ENABLE_FLEXCAN_DOZE_MODE (1U)
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

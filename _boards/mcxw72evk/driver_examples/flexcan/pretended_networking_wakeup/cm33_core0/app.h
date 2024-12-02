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
#define TX_MESSAGE_BUFFER_NUM (9U)
#define EXAMPLE_CAN_CLK_FREQ  CLOCK_GetFreq(kCLOCK_ScgSysOscClk)
// #define EXAMPLE_CAN_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Can0)
#define USE_IMPROVED_TIMING_CONFIG (1U)

/* Pretended Networking functionality can making FlexCan able to process Rx message filtering as defined by the
 * configuration registers when module in Doze mode or Stop mode. If the chip enter low power mode sent the Doze request
 * to FlexCan, we need set EXAMPLE_ENABLE_FLEXCAN_DOZE_MODE macro. */
#define EXAMPLE_ENABLE_FLEXCAN_DOZE_MODE (1)
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

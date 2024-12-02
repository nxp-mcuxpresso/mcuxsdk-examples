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
#define EXAMPLE_CAN           CAN1
#define RX_MESSAGE_BUFFER_NUM (7)
#define TX_MESSAGE_BUFFER_NUM (6)
#define USE_CANFD             (1)

#define FLEXCAN_CLOCK_ROOT   hal_clock_can1
#define EXAMPLE_CAN_CLK_FREQ HAL_ClockGetIpFreq(FLEXCAN_CLOCK_ROOT)
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

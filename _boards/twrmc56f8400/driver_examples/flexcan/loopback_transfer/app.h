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
#define EXAMPLE_CAN CAN0

#define RX_MESSAGE_BUFFER_NUM (2)
#define TX_MESSAGE_BUFFER_NUM (1)

#define EXAMPLE_CAN_CLK_SOURCE kFLEXCAN_ClkSrc1
#define EXAMPLE_CAN_CLK_FREQ   CLOCK_GetFreq(kCLOCK_BusClk)

#define EXAMPLE_FLEXCAN_IRQn CAN_MB_OR_IRQn
#define EXAMPLE_FLEXCAN_PRIO 1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

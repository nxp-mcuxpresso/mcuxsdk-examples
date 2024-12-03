/*
 * Copyright 2020 NXP
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

#define RX_MESSAGE_BUFFER_NUM (1)
#define TX_MESSAGE_BUFFER_NUM (0)
#define USE_CANFD \
    (1) /* Define USE_CANFD to no-zero value to evaluate CANFD mode, define to 0 to use classical CAN mode*/
/* Use peripheral clock as the CAN engine clock source */
#define EXAMPLE_CAN_CLK_SOURCE kFLEXCAN_ClkSrc1
#define EXAMPLE_CAN_CLK_FREQ   CLOCK_GetFreq(kCLOCK_BusClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

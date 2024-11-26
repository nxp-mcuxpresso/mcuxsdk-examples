/*
 * Copyright 2024 NXP
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
#define FLEXIO_UART_TX_PIN     4U
#define FLEXIO_UART_RX_PIN     5U
#else
#define FLEXIO_UART_TX_PIN     2U
#define FLEXIO_UART_RX_PIN     3U
#endif
#define DEMO_FLEXIO_CLOCK (hal_clock_flexio1)
#define FLEXIO_CLOCK_FREQUENCY HAL_ClockGetRate(DEMO_FLEXIO_CLOCK)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */

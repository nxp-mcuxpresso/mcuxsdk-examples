/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ${header:start} */
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_intmux.h"
/* ${header:end} */

/* ${function:start} */
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(SLAVE_LPI2C_CLOCK_NAME, SLAVE_LPI2C_CLOCK_SOURCE);
    CLOCK_SetIpSrc(MASTER_FLEXIO_CLOCK_NAME, MASTER_FLEXIO_CLOCK_SOURCE);

    INTMUX_Init(INTMUX1);
    INTMUX_SetChannelMode(INTMUX1, EXAMPLE_FLEXIO_INTMUX_CHANNEL, kINTMUX_ChannelLogicOR);
    INTMUX_EnableInterrupt(INTMUX1, EXAMPLE_FLEXIO_INTMUX_CHANNEL, MASTER_FLEXIO_IRQ);
    NVIC_SetPriority(INTMUX1_0_IRQn, 1);
}
/* ${function:end} */

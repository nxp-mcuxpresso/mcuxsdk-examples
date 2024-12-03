/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ${header:start} */
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_intmux.h"
#include "app.h"
/* ${header:end} */

/* ${function:start} */
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(SLAVE_LPI2C_CLOCK_NAME, SLAVE_LPI2C_CLOCK_SOURCE);
    CLOCK_SetIpSrc(MASTER_FLEXIO_CLOCK_NAME, MASTER_FLEXIO_CLOCK_SOURCE);

    INTMUX_Init(INTMUX0);
    INTMUX_EnableInterrupt(INTMUX0, 0, BOARD_LPI2C_SLAVE_IRQn);
    NVIC_SetPriority(INTMUX0_0_IRQn, 0U);
}
/* ${function:end} */

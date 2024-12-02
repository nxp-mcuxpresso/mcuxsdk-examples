/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set clock source for LPSPI master and get the clock source */
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_MASTER_CLOCK_NAME, EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE);
    CLOCK_SetIpSrcDiv(EXAMPLE_LPSPI_MASTER_CLOCK_NAME, kSCG_SysClkDivBy16);
    /* Set clock source for LPSPI and get slave clock source */
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME, EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE);
    CLOCK_SetIpSrcDiv(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME, kSCG_SysClkDivBy16);
}
/*${function:end}*/

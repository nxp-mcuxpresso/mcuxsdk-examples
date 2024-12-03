/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_MASTER_CLOCK_NAME, EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE);
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME, EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE);

/* Set interrupt priorities */
#if (I2C_MASTER_SLAVE == isMaster)
    NVIC_SetPriority(EXAMPLE_LPSPI_MASTER_IRQN, 3);
#elif (I2C_MASTER_SLAVE == isSLAVE)
    NVIC_SetPriority(EXAMPLE_LPSPI_SLAVE_IRQN, 2);
#endif
}
/*${function:end}*/

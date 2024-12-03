/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_common.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(kCLOCK_Lpspi0, EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE);
    CLOCK_SetIpSrc(kCLOCK_Lpspi1, EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE);

    /* Set IRQ priority for freertos_lpi2c */
    NVIC_SetPriority(EXAMPLE_LPSPI_MASTER_IRQN, 3);
    NVIC_SetPriority(EXAMPLE_LPSPI_SLAVE_IRQN, 2);
}
/*${function:end}*/

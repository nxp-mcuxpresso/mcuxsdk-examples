/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI and FlexIO*/
    CLOCK_SetIpSrc(MASTER_FLEXIO_SPI_CLOCK_NAME, MASTER_FLEXIO_SPI_CLOCK_SOURCE);
    CLOCK_SetIpSrc(SLAVE_LPSPI_CLOCK_NAME, SLAVE_LPSPI_CLOCK_SOURCE);
}
/*${function:end}*/

/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_intmux.h"
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

    INTMUX_Init(INTMUX0);
    INTMUX_SetChannelMode(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, kINTMUX_ChannelLogicOR);
    INTMUX_EnableInterrupt(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, SLAVE_LPSPI_IRQN);
}
/*${function:end}*/

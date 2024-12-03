/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
    CLOCK_SetIpSrc(MASTER_LPSPI_CLOCK_NAME, MASTER_LPSPI_CLOCK_SOURCE);
    CLOCK_SetIpSrc(SLAVE_FLEXIO_SPI_CLOCK_NAME, SLAVE_FLEXIO_SPI_CLOCK_SOURCE);

    INTMUX_Init(INTMUX1);
    INTMUX_SetChannelMode(INTMUX1, EXAMPLE_LPSPI_INTMUX_CHANNLE, kINTMUX_ChannelLogicOR);
    INTMUX_EnableInterrupt(INTMUX1, EXAMPLE_LPSPI_INTMUX_CHANNLE, MASTER_LPSPI_IRQN);
    INTMUX_SetChannelMode(INTMUX1, EXAMPLE_FLEXIO_INTMUX_CHANNEL, kINTMUX_ChannelLogicOR);
    INTMUX_EnableInterrupt(INTMUX1, EXAMPLE_FLEXIO_INTMUX_CHANNEL, SLAVE_FLEXIO_SPI_IRQ);
    NVIC_SetPriority(INTMUX1_0_IRQn, 1U);
    NVIC_SetPriority(INTMUX1_1_IRQn, 0U);
}
/*${function:end}*/

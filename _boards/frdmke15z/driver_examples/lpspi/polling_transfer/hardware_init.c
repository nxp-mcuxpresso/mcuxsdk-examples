/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
#endif

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_MASTER_CLOCK_NAME, EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE);
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME, EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE);

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
    if (EXAMPLE_LPSPI_SLAVE_IRQN > FSL_FEATURE_INTERRUPT_IRQ_MAX)
    {
        INTMUX_Init(INTMUX0);
        INTMUX_SetChannelMode(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, kINTMUX_ChannelLogicOR);
        INTMUX_EnableInterrupt(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, EXAMPLE_LPSPI_SLAVE_IRQN);
    }
#endif
}
/*${function:end}*/

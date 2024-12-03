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

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
#endif
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    bool isMasterIrqInIntmux = false;
    bool isSlaveIrqInIntmux  = false;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_MASTER_CLOCK_NAME, EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE);
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME, EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE);

/* Set the intmux and NVIC priority. */
#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
    INTMUX_Init(INTMUX0);
    INTMUX_SetChannelMode(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, kINTMUX_ChannelLogicOR);

    if (EXAMPLE_LPSPI_SLAVE_IRQN > FSL_FEATURE_INTERRUPT_IRQ_MAX)
    {
        isSlaveIrqInIntmux = true;
        INTMUX_EnableInterrupt(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, EXAMPLE_LPSPI_SLAVE_IRQN);
        NVIC_SetPriority(INTMUX0_0_IRQn, 0);
    }

    if (EXAMPLE_LPSPI_MASTER_IRQN > FSL_FEATURE_INTERRUPT_IRQ_MAX)
    {
        isMasterIrqInIntmux = true;
        INTMUX_EnableInterrupt(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, EXAMPLE_LPSPI_MASTER_IRQN);
        NVIC_SetPriority(INTMUX0_0_IRQn, 1);
    }
#endif

    /*Set slave interrupt priority higher than slave*/
    /*If master interrupt is not in Intmux*/
    if (!isMasterIrqInIntmux)
    {
        NVIC_SetPriority(EXAMPLE_LPSPI_MASTER_IRQN, 1);
    }

    if (!isSlaveIrqInIntmux)
    {
        NVIC_SetPriority(EXAMPLE_LPSPI_SLAVE_IRQN, 0);
    }
}

uint32_t LPSPI0_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpspi0);
}
uint32_t LPSPI1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpspi1);
}
/*${function:end}*/

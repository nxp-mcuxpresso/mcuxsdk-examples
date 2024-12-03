/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "pin_mux.h"
#include "app.h"
#include "board_fusionf1.h"
#include "fsl_common.h"
#include "fsl_reset.h"
#include "fsl_debug_console.h"
#include "fsl_pdm_edma.h"
#include "fsl_sai_edma.h"
/*${header:end}*/

/*${prototype:start}*/
void DMA0_DriverIRQHandler(uint32_t Channel);
/*${prototype:end}*/

/*${function:start}*/
static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);
    xos_start_system_timer(-1, 0);

    /* Configure interrupts. */
    xos_register_interrupt_handler(DMA0_0_IRQn, (XosIntFunc *)DMA0_DriverIRQHandler, (void *)0U);
    xos_register_interrupt_handler(DMA0_1_IRQn, (XosIntFunc *)DMA0_DriverIRQHandler, (void *)1U);
    xos_interrupt_enable(DMA0_0_IRQn);
    xos_interrupt_enable(DMA0_1_IRQn);
}

static void BOARD_InitClock(void)
{
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ); /* sets external XTAL OSC freq */

    CLOCK_SetIpSrc(kCLOCK_Micfil, kCLOCK_FusionMicfilClkSrcFro24);
    RESET_PeripheralReset(kRESET_Micfil);

    CLOCK_EnableClock(kCLOCK_Dma0Ch0);
    CLOCK_EnableClock(kCLOCK_Dma0Ch1);
}

void BOARD_InitHardware(void)
{
    XOS_Init();
    BOARD_InitBootPins();
    BOARD_InitClock();
    BOARD_InitDebugConsole();
}
/*${function:end}*/

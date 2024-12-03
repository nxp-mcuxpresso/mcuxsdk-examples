/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_cmc.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void EXAMPLE_EnableDeepSleepIRQ(void)
{
}

void EXAMPLE_EnterDeepSleep(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepSleepMode;
    config.wake_domain = kCMC_DeepSleepMode;

    CMC_EnterLowPowerMode(CMC0, &config);
}

void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* attach Clk16k[2] to OSTIMER */
    CLOCK_SetupClk16KClocking(kCLOCK_Clk16KToWake);
    CLOCK_AttachClk(kCLK_16K2_to_OSTIMER);
}
/*${function:end}*/

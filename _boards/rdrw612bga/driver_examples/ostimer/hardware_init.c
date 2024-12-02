/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void EXAMPLE_EnableDeepSleepIRQ(void)
{
    POWER_EnableWakeup(OS_EVENT_IRQn);
}

void EXAMPLE_EnterDeepSleep(void)
{
    power_sleep_config_t slpCfg;

    slpCfg.pm2MemPuCfg = APP_PM2_MEM_PU_CFG;
    slpCfg.pm2AnaPuCfg = APP_PM2_ANA_PU_CFG;
    slpCfg.clkGate     = APP_SOURCE_CLK_GATE;
    slpCfg.memPdCfg    = APP_MEM_PD_CFG;
    slpCfg.pm3BuckCfg  = APP_PM3_BUCK_CFG;

    /* Enter deep sleep mode by using power API. */
    POWER_EnterPowerMode(2U, &slpCfg);
}
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_AttachClk(kLPOSC_to_OSTIMER_CLK);
}
/*${function:end}*/

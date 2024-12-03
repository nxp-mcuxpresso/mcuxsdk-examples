/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "app.h"

#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void APP_InitBod(void)
{
    power_bod_config_t config;

    /*
     *  bodConfig->enableReset = true;
     *  bodConfig->resetLevel = kBod_ResetLevel0;
     *  bodConfig->enableInterrupt = false;
     *  bodConfig->interruptLevel = kBod_InterruptLevel0;
     */
    POWER_GetDefaultBodConfig(&config);
    config.enableReset     = false;
    config.enableInterrupt = true;
    config.interruptLevel  = kBod_InterruptLevel3;

    POWER_InitBod(&config);
    POWER_DisablePD(kPDRUNCFG_PD_BOD_INTR);
}

void APP_DeinitBod(void)
{
    POWER_DeinitBod();
    NVIC_DisableIRQ(APP_BOD_IRQn);
}
/*${function:end}*/

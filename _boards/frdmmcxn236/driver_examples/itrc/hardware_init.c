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
#include "fsl_clock.h"
#include "fsl_vbat.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void ITRC_CDOG_EnableIRQ(void)
{
    /* Set ITRC to reset values after leaving Boot ROM */
    ITRC0->OUT_SEL[3][0] = 0xAAAAAAAA;
    ITRC0->OUT_SEL[4][0] = 0xAAAAAAAA;
}

void VBAT_Init(void)
{
    // Enable and ungate FRO16K clock
    vbat_fro16k_config_t fro16kConfig = {
        .enableFRO16k = true,
        .enabledConnectionsMask = (uint8_t)kVBAT_EnableClockToDomain0,
    };
    VBAT_ConfigFRO16k(VBAT0, &fro16kConfig);

    VBAT_EnableBandgap(VBAT0, true);

    // Configure the VBAT voltage and temperature monitors
    vbat_tamper_config_t tamperConfig = {
        .enableVoltageDetect = true,
        .enableTemperatureDetect = true,
        .lock = false,
    };
    VBAT_InitTamper(VBAT0, &tamperConfig);

    // Wait for the above changes to take effect and clear the STATUS flags
    uint32_t mask = (uint32_t)kVBAT_StatusFlagVoltageDetect | (uint32_t)kVBAT_StatusFlagPORDetect;
    while ((VBAT_GetStatusFlags(VBAT0) & mask) != 0U)
    {
        VBAT_ClearStatusFlags(VBAT0, mask);
    }
}

void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    VBAT_Init();

    ITRC_CDOG_EnableIRQ();
}
/*${function:end}*/

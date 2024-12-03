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
    VBAT0->FROCTLA = VBAT_FROCTLA_FRO_EN(0x1);
    VBAT0->FROCTLB = VBAT_FROCTLB_INVERSE(0x0);
    VBAT0->FROCLKE = VBAT_FROCLKE_CLKE(0x1);

    // Configure the VBAT voltage and temperature monitors
    VBAT0->TAMCTLA = VBAT_TAMCTLA_VOLT_EN(0x1) | VBAT_TAMCTLA_TEMP_EN(0x1);
    VBAT0->TAMCTLB = VBAT_TAMCTLB_INVERSE(0xc);

    // Wait for the above changes to take effect and clear the STATUS flags
    uint32_t mask = VBAT_STATUSA_VOLT_DET(0x1) | VBAT_STATUSA_POR_DET(0x1);
    while (((VBAT0->STATUSA & VBAT_STATUSA_VOLT_DET_MASK) != 0) || ((VBAT0->STATUSA & VBAT_STATUSA_POR_DET_MASK) != 0))
    {
        VBAT0->STATUSA = mask;
        VBAT0->STATUSB = ~mask;
    }
}

void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    VBAT_Init();

    ITRC_CDOG_EnableIRQ();
}
/*${function:end}*/

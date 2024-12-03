/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_smartcard_usim.h"
#include "fsl_debug_console.h"
/*${header:end}*/

extern smartcard_context_t *g_smartcardContext;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Use 52 MHz clock for the USIM */
    CLOCK_AttachClk(kMAIN_CLK_to_USIM_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivUsimClk, 5U);

    /* Use 16 MHz clock for the Ctimer0 */
    CLOCK_AttachClk(kSFRO_to_CTIMER0);
}

/*! @brief IRQ handler for usim */
void USIM_IRQHandler(void)
{
    SMARTCARD_USIM_IRQHandler(BOARD_SMARTCARD_MODULE, g_smartcardContext);
}

/* Ctimer 0 IRQ handler */
void CTIMER0_IRQHandler(void)
{
    SMARTCARD_USIM_TSExpiryCallback(BOARD_SMARTCARD_MODULE, g_smartcardContext);
}
/*${function:end}*/

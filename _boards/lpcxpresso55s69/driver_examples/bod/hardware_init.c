/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_anactrl.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void APP_InitBod(void)
{
    /* set BOD VBAT level to 2.0V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel2000mv, kPOWER_BodHystLevel25mv, false);

    ANACTRL_Init(ANACTRL);
    ANACTRL_ClearInterrupts(ANACTRL, kANACTRL_BodVbatInterruptEnable);
    ANACTRL_EnableInterrupts(ANACTRL, kANACTRL_BodVbatInterruptEnable);
}

void APP_DeinitBod(void)
{
    ANACTRL_ClearInterrupts(ANACTRL, kANACTRL_BodVbatInterruptEnable);
    ANACTRL_DisableInterrupts(ANACTRL, kANACTRL_BodVbatInterruptEnable);
    POWER_DisablePD(kPDRUNCFG_PD_BODVBAT);
}

/*${function:end}*/

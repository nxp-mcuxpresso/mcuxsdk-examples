/*
 * Copyright 2024 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitLpitPwmPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();
}

void BOARD_ConfigTriggerSource()
{
    /* Init xbara module. */
    XBAR_Init(kXBAR_DSC1);

    /* Configure the XBARA signal connections. */
    XBAR_SetSignalsConnection(kXBAR1_InputLpit3LpitTrigOut0, kXBAR1_OutputIomuxXbarOut40);
}
/*${function:end}*/

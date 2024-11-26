/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbar.h"
#include "app.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn44, kXBAR1_OutputEnc1PhaseAInput);
    XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn45, kXBAR1_OutputEnc1PhaseBInput);
    XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn46, kXBAR1_OutputEnc1Index);
}
/*${function:end}*/

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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitEqdcPins();
    BOARD_InitI2C6Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    /* Select Motor controller 1 */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);

    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn45, kXBAR1_OutputEnc1PhaseBInput);
    XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn46, kXBAR1_OutputEnc1Index);
    XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn48, kXBAR1_OutputEnc1PhaseAInput);
}
/*${function:end}*/

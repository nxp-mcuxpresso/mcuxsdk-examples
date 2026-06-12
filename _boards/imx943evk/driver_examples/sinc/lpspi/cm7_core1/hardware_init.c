/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitLPSPI3Pins();

    BOARD_InitSinc2Pins();

    BOARD_InitI2C6Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SD2_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SD2_SEL);
}
/*${function:end}*/

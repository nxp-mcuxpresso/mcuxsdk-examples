/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "display_support.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitXSPI2Pins();

    BOARD_InitI2CPins();

    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_Init16bitsPsRam(XSPI2);

    POWER_DisablePD(kPDRUNCFG_APD_PNGDEC);
    POWER_DisablePD(kPDRUNCFG_PPD_PNGDEC);
    POWER_ApplyPD();
}
/*${function:end}*/

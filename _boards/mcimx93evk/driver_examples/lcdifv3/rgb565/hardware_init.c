/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "lcdifv3_support.h"
#include "app.h"
#include "fsl_misc_soc.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    SOC_MixPowerInit(SOC_MIX_PD_MEDIAMIX);
    SOC_DisableIsolation();
}
/*${function:end}*/

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitBootClocks();

    //BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);
    POWER_DisablePD(kPDRUNCFG_DSR_VDD2N_MEDIA);
    POWER_ApplyPD();

    CLOCK_AttachClk(kOSC_CLK_to_FCCLK0);
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM2);

    RESET_ClearPeripheralReset(kGPIO3_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio3);

    BOARD_InitAHBSC();
}
/*${function:end}*/

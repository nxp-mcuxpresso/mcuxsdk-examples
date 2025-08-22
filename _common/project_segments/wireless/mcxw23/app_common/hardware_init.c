/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"

/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    POWER_Init();

    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_PortInit(GPIO, 0);

    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    CLOCK_EnableClock(kCLOCK_FlexComm0);

    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
    CLOCK_EnableClock(kCLOCK_FlexComm2);

    CLOCK_AttachClk(kOSC32K_to_CTIMER0);

    BOARD_Configure32kOsc();

    BOARD_InitPins();
    BOARD_BootClockXTAL32M();

#if (defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1))
    BOARD_InitDebugConsole();
#endif
}
/*${function:end}*/

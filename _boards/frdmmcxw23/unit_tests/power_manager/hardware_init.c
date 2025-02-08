/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
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
    CLOCK_EnableClock(kCLOCK_FlexComm0);
    GPIO_PortInit(GPIO, 0);
    
    BOARD_InitPins();
    BOARD_InitDebugConsole();
    BOARD_BootClockXTAL32M();
    BOARD_Configure32kOsc();

}
/*${function:end}*/

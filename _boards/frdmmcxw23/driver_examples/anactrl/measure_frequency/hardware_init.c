/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    POWER_Init();

    CLOCK_EnableClock(kCLOCK_Iocon);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_PortInit(GPIO, 0);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    CLOCK_EnableClock(kCLOCK_FlexComm0);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable freqme and rtc clock modules. */
    CLOCK_EnableClock(kCLOCK_Freqme);
    CLOCK_EnableClock(kCLOCK_Rtc);

    /* Enable FRO 1 MHz clock for Frequency Measure module. */
    CLOCK_Enable1MFRO(true);
    POWER_PeripheralPowerOn(kPOWERCFG_FRO1M);
    POWER_PeripheralPowerOn(kPOWERCFG_FRO32K);
    /* Switch OSC32K to FRO32K */
    CLOCK_AttachClk(kFRO32K_to_OSC32K);
}
/*${function:end}*/

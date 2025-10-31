/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_port.h"
#include "board.h"
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void APP_InitTamperPins()
{
    /* Write to PORT0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);

    /* Tamper0 */
    const port_pin_config_t port0_4_config = {/* Internal pull-up/down resistor is pullup */
                                              kPORT_PullUp,
                                              /* Low internal pull resistor value is selected. */
                                              kPORT_LowPullResistor,
                                              /* Fast slew rate is configured */
                                              kPORT_FastSlewRate,
                                              /* Passive input filter is disabled */
                                              kPORT_PassiveFilterEnable,
                                              /* Open drain output is disabled */
                                              kPORT_OpenDrainDisable,
                                              /* Low drive strength is configured */
                                              kPORT_HighDriveStrength,
                                              /* Normal drive strength is configured */
                                              kPORT_NormalDriveStrength,
                                              /* Pin is configured as GPIO */
                                              kPORT_MuxAlt0,
                                              /* Digital input enabled */
                                              kPORT_InputBufferEnable,
                                              /* Digital input is not inverted */
                                              kPORT_InputNormal,
                                              /* Pin Control Register fields [15:0] are not locked */
                                              kPORT_UnlockRegister};
    /* PORT0_4 is configured as GPIO input for tamper0 */
    PORT_SetPinConfig(AON__PORT0, 4U, &port0_4_config);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    APP_InitTamperPins();
    CLOCK_InitRosc(true);
}
/*${function:end}*/
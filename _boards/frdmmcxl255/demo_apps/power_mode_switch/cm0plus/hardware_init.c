/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "app.h"
#include "fsl_mu.h"
#include <stdbool.h>

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_power.h"

#include "fsl_lptmr.h"
#include "fsl_smm.h"

#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_BootAs3MHzClocks(void)
{
    CLOCK_EnableClock(kCLOCK_GateAonAPB);
    AON__CGU->CLOCK_DIV |= CGU_CLOCK_DIV_CLK_DIV_EN_MASK;

    /* After DPD2 exit both AON FROs may still be disabled (set during DPD2
     * entry).  Re-enable them so CLOCK_SetupFROAonClocking can compute a
     * valid coreClock_Hz for its internal delays. */
    AON__CGU->CLK_CONFIG |= CGU_CLK_CONFIG_ULPIRC_EN_MASK | CGU_CLK_CONFIG_LPIRC_EN_MASK;

    CLOCK_SetupFROAonClocking(3000000U);
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);

    AON__CGU->CLOCK_DIV |= 1UL << 1UL;
    CLOCK_EnableClock(kCLOCK_GateAonUART);
    SystemCoreClock = 3000000U;
    
    CLOCK_AttachClk(kFROdiv1_to_AON_CPU);
}

void BOARD_InitTamper_ButtonPins(void)
{
    /* GPIO0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);
    /* PORT0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    const port_pin_config_t port0_4_config = {/* Internal pull-up/down resistor is disabled */
                                              kPORT_PullUp,
                                              /* Low internal pull resistor value is selected. */
                                              kPORT_HighPullResistor,
                                              /* Fast slew rate is configured */
                                              kPORT_FastSlewRate,
                                              /* Passive input filter is disabled */
                                              kPORT_PassiveFilterDisable,
                                              /* Open drain output is disabled */
                                              kPORT_OpenDrainDisable,
                                              /* Low drive strength is configured */
                                              kPORT_LowDriveStrength,
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

    PORT_SetPinConfig(AON__PORT0, 4U, &port0_4_config);
}

void BOARD_DeinitTamper_ButtonPins(void)
{
    CLOCK_DisableClock(kCLOCK_GateAonGPIO);
    CLOCK_DisableClock(kCLOCK_GateAonPORT);
}

void BOARD_DeinitDebugConsole(void)
{
    DbgConsole_Deinit();
    RESET_SetPeripheralReset(kAonUART_RST_SHIFT_RSTn);
    CLOCK_DisableClock(kCLOCK_GateAonPORT);
    CLOCK_DisableClock(kCLOCK_GateAonGPIO);
}

void BOARD_InitHardware(void)
{
    CLOCK_DisableADVCControl();
    BOARD_BootAs3MHzClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();

    SMM_DisableAonCpuIso(AON__SMM);
    EnableIRQ(SMM_EXT_IRQn);
    AON__SMM->PWDN_CONFIG &= ~SMM_PWDN_CONFIG_Q_TMT_EN_MASK;
}

/*${function:end}*/

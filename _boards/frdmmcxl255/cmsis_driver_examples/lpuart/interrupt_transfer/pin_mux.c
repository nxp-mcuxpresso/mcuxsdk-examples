/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "fsl_reset.h"
#include "board.h"

void BOARD_InitDEBUG_UARTPins(void)
{
    /* Write to PORT1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT2);
    /* LPUART0 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kLPUART0_RST_SHIFT_RSTn);
    /* PORT1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT2_RST_SHIFT_RSTn);

    /* TX */
    const port_pin_config_t port2_11_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
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
                                                      /* Pin is configured as LPUART0_TXD */
                                                      kPORT_MuxAlt2,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};
    /* PORT2_11 is configured as LPUART0_TXD */
    PORT_SetPinConfig(PORT2, 11U, &port2_11_config);

    /* RX */
    const port_pin_config_t port2_10_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
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
                                                      /* Pin is configured as LPUART0_RXD */
                                                      kPORT_MuxAlt2,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};
    /* PORT2_10 is configured as LPUART0_RXD */
    PORT_SetPinConfig(PORT2, 10U, &port2_10_config);
}

void BOARD_InitSWD_DEBUGPins(void)
{
}

void LPUART0_InitPins(void)
{
    /* Write to PORT2: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT2);
    /* LPUART0 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kLPUART0_RST_SHIFT_RSTn);
    /* GPIO1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    /* PORT2 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT2_RST_SHIFT_RSTn);

    const port_pin_config_t port2_11_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
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
                                                      /* Pin is configured as LPUART0_TXD */
                                                      kPORT_MuxAlt2,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};
    /* PORT2_11 is configured as LPUART0_TXD */
    PORT_SetPinConfig(PORT2, 11U, &port2_11_config);

    const port_pin_config_t port2_10_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
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
                                                      /* Pin is configured as LPUART0_RXD */
                                                      kPORT_MuxAlt2,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};
    /* PORT2_10 is configured as LPUART0_RXD */
    PORT_SetPinConfig(PORT2, 10U, &port2_10_config);
}

void LPUART0_DeinitPins(void)
{
    /* Write to PORT2: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT2);
    /* LPUART0 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kLPUART0_RST_SHIFT_RSTn);
    /* GPIO1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    /* PORT2 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT2_RST_SHIFT_RSTn);

    const port_pin_config_t port2_11_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
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
                                                      /* Pin is configured as P2_11 */
                                                      kPORT_MuxAlt0,
                                                      /* Digital input disabled; it is required for analog functions */
                                                      kPORT_InputBufferDisable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};
    /* PORT2_11 is configured as P2_11 */
    PORT_SetPinConfig(PORT2, 11U, &port2_11_config);

    const port_pin_config_t port2_10_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
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
                                                      /* Pin is configured as P2_10 */
                                                      kPORT_MuxAlt0,
                                                      /* Digital input disabled; it is required for analog functions */
                                                      kPORT_InputBufferDisable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};
    /* PORT2_10 is configured as P2_10 */
    PORT_SetPinConfig(PORT2, 10U, &port2_10_config);
}
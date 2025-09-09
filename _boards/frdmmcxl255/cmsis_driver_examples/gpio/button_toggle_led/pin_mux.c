/*
 * Copyright 2025 NXP
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

void BOARD_InitBUTTONsPins(void)
{
    /* GPIO1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    /* PORT1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    /* GPIO1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    /* PORT1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);

    gpio_pin_config_t SW2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };

    /* Initialize GPIO functionality on pin PIO1_14   */
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &SW2_config);

    const port_pin_config_t SW2 = {/* Internal pull-up/down resistor is disabled */
                                   .pullSelect = kPORT_PullDisable,
                                   /* Low internal pull resistor value is selected. */
                                   .pullValueSelect = kPORT_LowPullResistor,
                                   /* Fast slew rate is configured */
                                   .slewRate = kPORT_FastSlewRate,
                                   /* Passive input filter is disabled */
                                   .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                   /* Open drain output is disabled */
                                   .openDrainEnable = kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   .driveStrength = kPORT_LowDriveStrength,
                                   /* Normal drive strength is configured */
                                   .driveStrength1 = kPORT_NormalDriveStrength,
                                   /* Pin is configured as P1_14 */
                                   .mux = kPORT_MuxAlt0,
                                   /* Digital input enabled */
                                   .inputBuffer = kPORT_InputBufferEnable,
                                   /* Digital input is not inverted */
                                   .invertInput = kPORT_InputNormal,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   .lockRegister = kPORT_UnlockRegister};
    /* PORT1_14 is configured as P1_14 */
    PORT_SetPinConfig(PORT1, BOARD_SW2_GPIO_PIN, &SW2);
}
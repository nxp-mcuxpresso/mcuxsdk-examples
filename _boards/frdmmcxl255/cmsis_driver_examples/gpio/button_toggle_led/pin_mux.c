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
    /* Port Clock Enable: Enable */
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    /* Clock gate control PORT1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    /* GPIO Clock Enable: Enable */
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);
    /* Clock gate control GPIO1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    /* GPIO1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    /* PORT1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);

    const port_pin_config_t port1_14_pinB1_config = {/* Internal pull-up/down resistor is disabled */
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
                                                     /* Pin is configured as P1_14 */
                                                     kPORT_MuxAlt0,
                                                     /* Digital input enabled */
                                                     kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     kPORT_UnlockRegister};
    /* PORT1_14 (pin B1) is configured as P1_14 */
    PORT_SetPinConfig(PORT1, 14U, &port1_14_pinB1_config);

    gpio_pin_config_t gpio1_pinB1_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin P1_14 (pin B1)  */
    GPIO_PinInit(GPIO1, 14U, &gpio1_pinB1_config);

    /* Interrupt configuration on GPIO1_14 (pin B1): Interrupt/DMA request is disabled */
    GPIO_SetPinInterruptChannel(GPIO1, 14U, kGPIO_InterruptOutput0);
    GPIO_SetPinInterruptConfig(GPIO1, 14U, kGPIO_InterruptStatusFlagDisabled);
}

void BOARD_InitLEDsPins(void)
{
    /* Clock gate control PORT1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    /* Clock gate control GPIO1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    /* GPIO1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    /* PORT1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);

    const port_pin_config_t LED_RED = {/* Internal pull-up/down resistor is disabled */
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
                                       /* Pin is configured as P1_15 */
                                       kPORT_MuxAlt0,
                                       /* Digital input enabled */
                                       kPORT_InputBufferEnable,
                                       /* Digital input is not inverted */
                                       kPORT_InputNormal,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORT1_15 (pin C3) is configured as P1_15 */
    PORT_SetPinConfig(BOARD_INITLEDSPINS_LED_RED_PORT, BOARD_INITLEDSPINS_LED_RED_PIN, &LED_RED);

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin P1_15 (pin C3)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_RED_GPIO, BOARD_INITLEDSPINS_LED_RED_PIN, &LED_RED_config);
}
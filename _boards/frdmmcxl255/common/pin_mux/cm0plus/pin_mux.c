/*
 * Copyright 2024,2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_reset.h"

void BOARD_InitDEBUG_UARTPins(void)
{

    /* LPUART0 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kAonUART_RST_SHIFT_RSTn);


    /* TX */
    const port_pin_config_t port0_6_config = {/* Internal pull-up/down resistor is disabled */
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
                                                      /* Pin is configured as LPUART_TXD */
                                                      kPORT_MuxAlt4,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 6U, &port0_6_config);

    /* RX */
    const port_pin_config_t port0_7_config = {/* Internal pull-up/down resistor is disabled */
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
                                                      /* Pin is configured as LPUART_RXD */
                                                      kPORT_MuxAlt4,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 7U, &port0_7_config);
}

void BOARD_InitSWD_DEBUGPins(void)
{
}

void BOARD_InitI2CPins(void)
{
    /* AONI2C0_SDA */
    const port_pin_config_t port0_13_config = {/* Internal pull-up resistor is enabled */
                                                     kPORT_PullUp,
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
                                                     /* Pin is configured as AONI2C0_SDA */
                                                     kPORT_MuxAlt2,
                                                     /* Digital input enabled */
                                                     kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 13U, &port0_13_config);

    /* AONI2C0_SCL */
    const port_pin_config_t port0_12_config = {/* Internal pull-up resistor is enabled */
                                                     kPORT_PullUp,
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
                                                     /* Pin is configured as AONI2C0_SCL */
                                                     kPORT_MuxAlt2,
                                                     /* Digital input enabled */
                                                     kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 12U, &port0_12_config);
}

void BOARD_InitQTMRPins(void) {
    /* AONQTMR1_OUT1 used as output */
    const port_pin_config_t port0_13_config = {/* Internal pull-up/down resistor is disabled */
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
                                                     /* Pin is configured as AONQTMR1_OUT1 */
                                                     .mux = kPORT_MuxAlt5,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(AON__PORT0, 13U, &port0_13_config);

    /* AONTRIG_IN4 used as input */
    const port_pin_config_t port0_12_config = {/* Internal pull-up/down resistor is enabled */
                                                     .pullSelect = kPORT_PullUp,
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
                                                     /* Pin is configured as AONTRIG_IN4 */
                                                     .mux = kPORT_MuxAlt1,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(AON__PORT0, 12U, &port0_12_config);
}

void BOARD_InitACMPPins()
{
    const port_pin_config_t port0_4_config = {/* Internal pull-up/down resistor is disabled */
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
                                                 /* Pin is configured as AON ACMP IN3 */
                                                 kPORT_MuxAlt0,
                                                 /* Digital input disabled; it is required for analog functions */
                                                 kPORT_InputBufferDisable,
                                                 /* Digital input is not inverted */
                                                 kPORT_InputNormal,
                                                 /* Pin Control Register fields [15:0] are not locked */
                                                 kPORT_UnlockRegister};
    /* PORT0_5 is configured as AON ACMP IN3 */
    PORT_SetPinConfig(AON__PORT0, 4U, &port0_4_config);
}

void BOARD_InitLPCMPPins()
{
    const port_pin_config_t port0_5_config = {/* Internal pull-up/down resistor is disabled */
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
                                                 /* Pin is configured as CMP0_IN5 */
                                                 kPORT_MuxAlt0,
                                                 /* Digital input disabled; it is required for analog functions */
                                                 kPORT_InputBufferDisable,
                                                 /* Digital input is not inverted */
                                                 kPORT_InputNormal,
                                                 /* Pin Control Register fields [15:0] are not locked */
                                                 kPORT_UnlockRegister};
    /* PORT0_5 is configured as CMP0_IN5 */
    PORT_SetPinConfig(AON__PORT0, 5U, &port0_5_config);
}

void BOARD_InitLPADCPins()
{
    const port_pin_config_t port0_13_config = {/* Internal pull-up/down resistor is disabled */
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
                                                 /* Pin is configured as AON_LPADC0_A3 */
                                                 kPORT_MuxAlt0,
                                                 /* Digital input disabled; it is required for analog functions */
                                                 kPORT_InputBufferDisable,
                                                 /* Digital input is not inverted */
                                                 kPORT_InputNormal,
                                                 /* Pin Control Register fields [15:0] are not locked */
                                                 kPORT_UnlockRegister};
    /* PORT0_13 is configured as AON_LPADC0_A3 */
    PORT_SetPinConfig(AON__PORT0, 13U, &port0_13_config);
}

void BOARD_InitKPPPins()
{
    /* AonKPP peripheral is released from reset */
    RESET_ReleasePeripheralReset(kAonKPP_RST_SHIFT_RSTn);

    const port_pin_config_t port0_13_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullUp,
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
                                                      /* Pin is configured as KPP_ROW_1 */
                                                      kPORT_MuxAlt3,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 13U, &port0_13_config);

    const port_pin_config_t port0_12_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullUp,
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
                                                      /* Pin is configured as KPP_ROW_0 */
                                                      kPORT_MuxAlt3,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 12U, &port0_12_config);

      const port_pin_config_t port0_3_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
                                                      /* Fast slew rate is configured */
                                                      kPORT_FastSlewRate,
                                                      /* Passive input filter is disabled */
                                                      kPORT_PassiveFilterDisable,
                                                      /* Open drain output is disabled */
                                                      kPORT_OpenDrainEnable,
                                                      /* Low drive strength is configured */
                                                      kPORT_LowDriveStrength,
                                                      /* Normal drive strength is configured */
                                                      kPORT_NormalDriveStrength,
                                                      /* Pin is configured as KPP_COL_1 */
                                                      kPORT_MuxAlt3,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 3U, &port0_3_config);

    const port_pin_config_t port0_2_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
                                                      /* Fast slew rate is configured */
                                                      kPORT_FastSlewRate,
                                                      /* Passive input filter is disabled */
                                                      kPORT_PassiveFilterDisable,
                                                      /* Open drain output is disabled */
                                                      kPORT_OpenDrainEnable,
                                                      /* Low drive strength is configured */
                                                      kPORT_LowDriveStrength,
                                                      /* Normal drive strength is configured */
                                                      kPORT_NormalDriveStrength,
                                                      /* Pin is configured as KPP_COL_0 */
                                                      kPORT_MuxAlt3,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 2U, &port0_2_config);
}

void BOARD_InitLPACMPPins(void)
{
    const port_pin_config_t port0_10_config = {/* Internal pull-up/down resistor is disabled */
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
                                                 /* Pin is configured as AON_ACMP_IN[4] */
                                                 kPORT_MuxAlt0,
                                                 /* Digital input disabled; it is required for analog functions */
                                                 kPORT_InputBufferDisable,
                                                 /* Digital input is not inverted */
                                                 kPORT_InputNormal,
                                                 /* Pin Control Register fields [15:0] are not locked */
                                                 kPORT_UnlockRegister};
    /* PORT0_10 is configured as AON_ACMP_IN[4] */
    PORT_SetPinConfig(AON__PORT0, 10U, &port0_10_config);
}

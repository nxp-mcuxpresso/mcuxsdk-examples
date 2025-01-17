/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v16.0
processor: MCXA166
package_id: MCXA166VLQ
mcu_data: ksdk2_0
processor_version: 0.16.12
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '137', peripheral: INPUTMUX0, signal: 'TRIG_OUT, 0', pin_signal: P1_2/TRIG_OUT0/LPSPI0_SDI/LPI2C1_SDAS/CT1_MAT0/CT_INP0/CAN0_TXD/ADC0_A18/CMP2_IN3,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, input_buffer: enable, invert_input: normal}
  - {pin_num: '135', peripheral: INPUTMUX0, signal: 'TRIG_IN, 0', pin_signal: P1_0/WUU0_IN6/LPTMR0_ALT3/TRIG_IN0/LPSPI0_SDO/LPI2C1_SDA/CT_INP4/CT0_MAT2/ADC0_A16/CMP0_IN3,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable, input_buffer: enable, invert_input: normal}
  - {pin_num: '136', peripheral: INPUTMUX0, signal: 'TRIG_IN, 1', pin_signal: P1_1/TRIG_IN1/LPSPI0_SCK/LPI2C1_SCL/CT_INP5/CT0_MAT3/ADC0_A17/CMP1_IN3, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable, input_buffer: enable, invert_input: normal}
  - {pin_num: '36', peripheral: LPUART2, signal: RX, pin_signal: P2_3/WUU0_IN19/TRIG_IN7/LPUART0_CTS_B/LPUART2_RXD/CT_INP13/CT2_MAT3/EZH_PIO27/ADC0_A3/CMP1_IN0/ADC1_A4,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, input_buffer: enable, invert_input: normal}
  - {pin_num: '35', peripheral: LPUART2, signal: TX, pin_signal: P2_2/TRIG_IN6/LPUART0_RTS_B/LPUART2_TXD/CT_INP12/CT2_MAT2/EZH_PIO26/ADC0_A4/CMP0_IN0/DAC0_OUT, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, input_buffer: enable, invert_input: normal}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    /* PORT1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    /* PORT2: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT2);
    /* INPUTMUX0 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    /* PORT1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);
    /* LPUART2 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kLPUART2_RST_SHIFT_RSTn);
    /* PORT2 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT2_RST_SHIFT_RSTn);

    const port_pin_config_t port1_0_pin135_config = {/* Internal pull-up/down resistor is disabled */
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
                                                     /* Pin is configured as TRIG_IN0 */
                                                     .mux = kPORT_MuxAlt1,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    /* PORT1_0 (pin 135) is configured as TRIG_IN0 */
    PORT_SetPinConfig(PORT1, 0U, &port1_0_pin135_config);

    const port_pin_config_t port1_1_pin136_config = {/* Internal pull-up/down resistor is disabled */
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
                                                     /* Pin is configured as TRIG_IN1 */
                                                     .mux = kPORT_MuxAlt1,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    /* PORT1_1 (pin 136) is configured as TRIG_IN1 */
    PORT_SetPinConfig(PORT1, 1U, &port1_1_pin136_config);

    const port_pin_config_t port1_2_pin137_config = {/* Internal pull-up/down resistor is disabled */
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
                                                     /* Pin is configured as TRIG_OUT0 */
                                                     .mux = kPORT_MuxAlt1,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    /* PORT1_2 (pin 137) is configured as TRIG_OUT0 */
    PORT_SetPinConfig(PORT1, 2U, &port1_2_pin137_config);

    const port_pin_config_t port2_2_pin35_config = {/* Internal pull-up/down resistor is disabled */
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
                                                    /* Pin is configured as LPUART2_TXD */
                                                    .mux = kPORT_MuxAlt3,
                                                    /* Digital input enabled */
                                                    .inputBuffer = kPORT_InputBufferEnable,
                                                    /* Digital input is not inverted */
                                                    .invertInput = kPORT_InputNormal,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    .lockRegister = kPORT_UnlockRegister};
    /* PORT2_2 (pin 35) is configured as LPUART2_TXD */
    PORT_SetPinConfig(PORT2, 2U, &port2_2_pin35_config);

    const port_pin_config_t port2_3_pin36_config = {/* Internal pull-up/down resistor is disabled */
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
                                                    /* Pin is configured as LPUART2_RXD */
                                                    .mux = kPORT_MuxAlt3,
                                                    /* Digital input enabled */
                                                    .inputBuffer = kPORT_InputBufferEnable,
                                                    /* Digital input is not inverted */
                                                    .invertInput = kPORT_InputNormal,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    .lockRegister = kPORT_UnlockRegister};
    /* PORT2_3 (pin 36) is configured as LPUART2_RXD */
    PORT_SetPinConfig(PORT2, 3U, &port2_3_pin36_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

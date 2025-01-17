/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
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
processor: MCXN236
package_id: MCXN236VDF
mcu_data: ksdk2_0
processor_version: 0.16.22
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_inputmux.h"
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
  - {pin_num: A1, peripheral: LP_FLEXCOMM4, signal: LPFLEXCOMM_P0, pin_signal: PIO1_8/WUU0_IN10/LPTMR1_ALT3/TRACE_DATA0/FC4_P0/FC5_P4/CT_INP8/FLEXIO0_D16/EZH_PIO4/I3C1_SDA/ADC1_A8,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable, pull_value: low, input_buffer: enable,
    invert_input: normal}
  - {pin_num: B1, peripheral: LP_FLEXCOMM4, signal: LPFLEXCOMM_P1, pin_signal: PIO1_9/TRACE_DATA1/FC4_P1/FC5_P5/CT_INP9/FLEXIO0_D17/EZH_PIO5/I3C1_SCL/ADC1_A9, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable, input_buffer: enable, invert_input: normal}
  - {peripheral: QDC0, signal: INDEX, pin_signal: TRIG_IN5}
  - {peripheral: QDC0, signal: 'PHASE, A', pin_signal: TRIG_IN9}
  - {peripheral: QDC0, signal: 'PHASE, B', pin_signal: TRIG_IN8}
  - {pin_num: H2, peripheral: INPUTMUX0, signal: 'TRIG_IN, 5', pin_signal: PIO2_0/TRIG_IN5/PWM1_A3/FLEXIO0_D8/EZH_PIO20/SAI0_RX_BCLK, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, input_buffer: enable, invert_input: normal}
  - {pin_num: T11, peripheral: INPUTMUX0, signal: 'TRIG_IN, 9', pin_signal: PIO4_21/TRIG_IN9/FC2_P5/CT2_MAT1/FLEXIO0_D29/ADC1_B6, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, input_buffer: enable, invert_input: normal}
  - {pin_num: T7, peripheral: INPUTMUX0, signal: 'TRIG_IN, 8', pin_signal: PIO4_13/TRIG_IN8/FC2_P1/USB1_ID/USB1_OTG_ID/CT4_MAT1/FLEXIO0_D21/CAN0_TXD/ADC0_B5/ADC1_B5,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, input_buffer: enable, invert_input: normal}
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
    /* Enables the clock for INPUTMUX: Enables clock */
    CLOCK_EnableClock(kCLOCK_InputMux0);
    /* Enables the clock for PORT1: Enables clock */
    CLOCK_EnableClock(kCLOCK_Port1);
    /* Enables the clock for PORT2: Enables clock */
    CLOCK_EnableClock(kCLOCK_Port2);
    /* Enables the clock for PORT4: Enables clock */
    CLOCK_EnableClock(kCLOCK_Port4);
    /* TRIG_IN5 is selected as trigger input for QDC0 INDEX channel 0 */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_TrigIn5ToQdc0Index);
    /* TRIG_IN9 is selected as trigger input for QDC0 PHASEA channel 0 */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_TrigIn9ToQdc0Phasea);
    /* TRIG_IN8 is selected as trigger input for QDC0 PHASEB channel 0 */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_TrigIn8ToQdc0Phaseb);

    const port_pin_config_t port1_8_pinA1_config = {/* Internal pull-up/down resistor is disabled */
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
                                                    /* Pin is configured as FC4_P0 */
                                                    .mux = kPORT_MuxAlt2,
                                                    /* Digital input enabled */
                                                    .inputBuffer = kPORT_InputBufferEnable,
                                                    /* Digital input is not inverted */
                                                    .invertInput = kPORT_InputNormal,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    .lockRegister = kPORT_UnlockRegister};
    /* PORT1_8 (pin A1) is configured as FC4_P0 */
    PORT_SetPinConfig(PORT1, 8U, &port1_8_pinA1_config);

    const port_pin_config_t port1_9_pinB1_config = {/* Internal pull-up/down resistor is disabled */
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
                                                    /* Pin is configured as FC4_P1 */
                                                    .mux = kPORT_MuxAlt2,
                                                    /* Digital input enabled */
                                                    .inputBuffer = kPORT_InputBufferEnable,
                                                    /* Digital input is not inverted */
                                                    .invertInput = kPORT_InputNormal,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    .lockRegister = kPORT_UnlockRegister};
    /* PORT1_9 (pin B1) is configured as FC4_P1 */
    PORT_SetPinConfig(PORT1, 9U, &port1_9_pinB1_config);

    const port_pin_config_t port2_0_pinH2_config = {/* Internal pull-up/down resistor is disabled */
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
                                                    /* Pin is configured as TRIG_IN5 */
                                                    .mux = kPORT_MuxAlt1,
                                                    /* Digital input enabled */
                                                    .inputBuffer = kPORT_InputBufferEnable,
                                                    /* Digital input is not inverted */
                                                    .invertInput = kPORT_InputNormal,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    .lockRegister = kPORT_UnlockRegister};
    /* PORT2_0 (pin H2) is configured as TRIG_IN5 */
    PORT_SetPinConfig(PORT2, 0U, &port2_0_pinH2_config);

    const port_pin_config_t port4_13_pinT7_config = {/* Internal pull-up/down resistor is disabled */
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
                                                     /* Pin is configured as TRIG_IN8 */
                                                     .mux = kPORT_MuxAlt1,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    /* PORT4_13 (pin T7) is configured as TRIG_IN8 */
    PORT_SetPinConfig(PORT4, 13U, &port4_13_pinT7_config);

    const port_pin_config_t port4_21_pinT11_config = {/* Internal pull-up/down resistor is disabled */
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
                                                      /* Pin is configured as TRIG_IN9 */
                                                      .mux = kPORT_MuxAlt1,
                                                      /* Digital input enabled */
                                                      .inputBuffer = kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      .invertInput = kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      .lockRegister = kPORT_UnlockRegister};
    /* PORT4_21 (pin T11) is configured as TRIG_IN9 */
    PORT_SetPinConfig(PORT4, 21U, &port4_21_pinT11_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

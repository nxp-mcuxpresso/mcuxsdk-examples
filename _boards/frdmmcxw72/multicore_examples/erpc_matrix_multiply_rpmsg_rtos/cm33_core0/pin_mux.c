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
processor: MCXW727CxxxA
package_id: MCXW727CMFTA
mcu_data: ksdk2_0
processor_version: 0.0.0
pin_labels:
- {pin_num: '10', pin_signal: ADC0_A10/CMP0_IN0/PTA4/WUU0_P2/RF_XTAL_OUT_ENABLE/RF_GPO_9/TPM0_CLKIN/TRACE_SWO/FLEXIO0_D4/BOOT_CONFIG, label: SW3, identifier: SW3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
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
  - {pin_num: '39', peripheral: LPUART1, signal: RX, pin_signal: PTC2/WUU0_P9/LPSPI1_SOUT/LPUART1_RX/LPI2C1_SCLS/TPM1_CH2/I3C0_PUR/FLEXIO0_D18, pull_select: down,
    pull_enable: disable, slew_rate: fast, open_drain: disable, drive_strength: low, invert_input: disable}
  - {pin_num: '40', peripheral: LPUART1, signal: TX, pin_signal: PTC3/LPSPI1_SCK/LPUART1_TX/LPI2C1_SDAS/TPM1_CH3/FLEXIO0_D19, pull_select: down, pull_enable: disable,
    slew_rate: fast, open_drain: disable, drive_strength: low, invert_input: disable}
  - {pin_num: '10', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: ADC0_A10/CMP0_IN0/PTA4/WUU0_P2/RF_XTAL_OUT_ENABLE/RF_GPO_9/TPM0_CLKIN/TRACE_SWO/FLEXIO0_D4/BOOT_CONFIG,
    direction: INPUT, pull_select: down, pull_enable: enable, slew_rate: fast, open_drain: disable, drive_strength: low, invert_input: disable}
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
    /* Clock Config: Peripheral clocks are enabled; module does not stall low power mode entry */
    CLOCK_EnableClock(kCLOCK_GpioA);
    /* Clock Config: Peripheral clocks are enabled; module does not stall low power mode entry */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Config: Peripheral clocks are enabled; module does not stall low power mode entry */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t SW3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA4 (pin 10)  */
    GPIO_PinInit(BOARD_INITPINS_SW3_GPIO, BOARD_INITPINS_SW3_PIN, &SW3_config);

    const port_pin_config_t SW3 = {/* Internal pull-down resistor is enabled */
                                   .pullSelect = (uint16_t)kPORT_PullDown,
                                   /* Low internal pull resistor value is selected. */
                                   .pullValueSelect = (uint16_t)kPORT_LowPullResistor,
                                   /* Fast slew rate is configured */
                                   .slewRate = (uint16_t)kPORT_FastSlewRate,
                                   /* Passive input filter is disabled */
                                   .passiveFilterEnable = (uint16_t)kPORT_PassiveFilterDisable,
                                   /* Open drain output is disabled */
                                   .openDrainEnable = (uint16_t)kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   .driveStrength = (uint16_t)kPORT_LowDriveStrength,
                                   /* Normal drive strength is configured */
                                   .driveStrength1 = (uint16_t)kPORT_NormalDriveStrength,
                                   /* Pin is configured as PTA4 */
                                   .mux = (uint16_t)kPORT_MuxAsGpio,
                                   /* Digital input is not inverted */
                                   .invertInput = (uint16_t)kPORT_InputNormal,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   .lockRegister = (uint16_t)kPORT_UnlockRegister};
    /* PORTA4 (pin 10) is configured as PTA4 */
    PORT_SetPinConfig(BOARD_INITPINS_SW3_PORT, BOARD_INITPINS_SW3_PIN, &SW3);

    const port_pin_config_t portc2_pin39_config = {/* Internal pull-up/down resistor is disabled */
                                                   .pullSelect = (uint16_t)kPORT_PullDisable,
                                                   /* Low internal pull resistor value is selected. */
                                                   .pullValueSelect = (uint16_t)kPORT_LowPullResistor,
                                                   /* Fast slew rate is configured */
                                                   .slewRate = (uint16_t)kPORT_FastSlewRate,
                                                   /* Passive input filter is disabled */
                                                   .passiveFilterEnable = (uint16_t)kPORT_PassiveFilterDisable,
                                                   /* Open drain output is disabled */
                                                   .openDrainEnable = (uint16_t)kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   .driveStrength = (uint16_t)kPORT_LowDriveStrength,
                                                   /* Normal drive strength is configured */
                                                   .driveStrength1 = (uint16_t)kPORT_NormalDriveStrength,
                                                   /* Pin is configured as LPUART1_RX */
                                                   .mux = (uint16_t)kPORT_MuxAlt3,
                                                   /* Digital input is not inverted */
                                                   .invertInput = (uint16_t)kPORT_InputNormal,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   .lockRegister = (uint16_t)kPORT_UnlockRegister};
    /* PORTC2 (pin 39) is configured as LPUART1_RX */
    PORT_SetPinConfig(PORTC, 2U, &portc2_pin39_config);

    const port_pin_config_t portc3_pin40_config = {/* Internal pull-up/down resistor is disabled */
                                                   .pullSelect = (uint16_t)kPORT_PullDisable,
                                                   /* Low internal pull resistor value is selected. */
                                                   .pullValueSelect = (uint16_t)kPORT_LowPullResistor,
                                                   /* Fast slew rate is configured */
                                                   .slewRate = (uint16_t)kPORT_FastSlewRate,
                                                   /* Passive input filter is disabled */
                                                   .passiveFilterEnable = (uint16_t)kPORT_PassiveFilterDisable,
                                                   /* Open drain output is disabled */
                                                   .openDrainEnable = (uint16_t)kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   .driveStrength = (uint16_t)kPORT_LowDriveStrength,
                                                   /* Normal drive strength is configured */
                                                   .driveStrength1 = (uint16_t)kPORT_NormalDriveStrength,
                                                   /* Pin is configured as LPUART1_TX */
                                                   .mux = (uint16_t)kPORT_MuxAlt3,
                                                   /* Digital input is not inverted */
                                                   .invertInput = (uint16_t)kPORT_InputNormal,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   .lockRegister = (uint16_t)kPORT_UnlockRegister};
    /* PORTC3 (pin 40) is configured as LPUART1_TX */
    PORT_SetPinConfig(PORTC, 3U, &portc3_pin40_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

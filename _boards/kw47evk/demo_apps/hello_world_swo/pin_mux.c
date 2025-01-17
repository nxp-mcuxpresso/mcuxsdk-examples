/*
 * Copyright 2021-2024 NXP
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
product: Pins v15.0
processor: KW47B42ZB7xxxA
package_id: KW47B42ZB7AFTA
mcu_data: ksdk2_0
processor_version: 0.0.0
pin_labels:
- {pin_num: '44', pin_signal: ADC0_A8/PTC6/WUU0_P11/LPSPI1_PCS1/TPM1_CH5/FLEXIO0_D22, label: SW3, identifier: SW3}
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
  - {pin_num: '10', peripheral: TPIU, signal: SWO, pin_signal: ADC0_A10/CMP0_IN0/PTA4/WUU0_P2/RF_XTAL_OUT_ENABLE/RF_GPO_9/TPM0_CLKIN/TRACE_SWO/FLEXIO0_D4/BOOT_CONFIG,
    pull_select: down, pull_enable: enable, slew_rate: fast, open_drain: disable, drive_strength: low, invert_input: disable}
  - {pin_num: '44', peripheral: GPIOC, signal: 'GPIO, 6', pin_signal: ADC0_A8/PTC6/WUU0_P11/LPSPI1_PCS1/TPM1_CH5/FLEXIO0_D22, direction: INPUT, gpio_per_interrupt: kGPIO_InterruptFallingEdge,
    eft_interrupt: disable, pull_select: up, pull_enable: enable, slew_rate: fast, open_drain: disable, drive_strength: low, invert_input: disable}
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
    CLOCK_EnableClock(kCLOCK_GpioC);
    /* Clock Config: Peripheral clocks are enabled; module does not stall low power mode entry */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Config: Peripheral clocks are enabled; module does not stall low power mode entry */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t SW3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC6 (pin 44)  */
    GPIO_PinInit(BOARD_INITPINS_SW3_GPIO, BOARD_INITPINS_SW3_PIN, &SW3_config);

    /* Interrupt configuration on GPIOC6 (pin 44): Interrupt on falling edge */
    GPIO_SetPinInterruptConfig(BOARD_INITPINS_SW3_GPIO, BOARD_INITPINS_SW3_PIN, kGPIO_InterruptFallingEdge);

    const port_pin_config_t porta4_pin10_config = {/* Internal pull-down resistor is enabled */
                                                   (uint16_t)kPORT_PullDown,
                                                   /* Low internal pull resistor value is selected. */
                                                   (uint16_t)kPORT_LowPullResistor,
                                                   /* Fast slew rate is configured */
                                                   (uint16_t)kPORT_FastSlewRate,
                                                   /* Passive input filter is disabled */
                                                   (uint16_t)kPORT_PassiveFilterDisable,
                                                   /* Open drain output is disabled */
                                                   (uint16_t)kPORT_OpenDrainDisable,
                                                   /* Low drive strength is configured */
                                                   (uint16_t)kPORT_LowDriveStrength,
                                                   /* Normal drive strength is configured */
                                                   (uint16_t)kPORT_NormalDriveStrength,
                                                   /* Pin is configured as TRACE_SWO */
                                                   (uint16_t)kPORT_MuxAlt5,
                                                   /* Does not invert */
                                                   (uint16_t)kPORT_InputNormal,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   (uint16_t)kPORT_UnlockRegister};
    /* PORTA4 (pin 10) is configured as TRACE_SWO */
    PORT_SetPinConfig(PORTA, 4U, &porta4_pin10_config);

    /* EFT detect interrupts configuration on PORTC */
    PORT_DisableEFTDetectInterrupts(PORTC, 0x40u);

    const port_pin_config_t SW3 = {/* Internal pull-up resistor is enabled */
                                   (uint16_t)kPORT_PullUp,
                                   /* Low internal pull resistor value is selected. */
                                   (uint16_t)kPORT_LowPullResistor,
                                   /* Fast slew rate is configured */
                                   (uint16_t)kPORT_FastSlewRate,
                                   /* Passive input filter is disabled */
                                   (uint16_t)kPORT_PassiveFilterDisable,
                                   /* Open drain output is disabled */
                                   (uint16_t)kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   (uint16_t)kPORT_LowDriveStrength,
                                   /* Normal drive strength is configured */
                                   (uint16_t)kPORT_NormalDriveStrength,
                                   /* Pin is configured as PTC6 */
                                   (uint16_t)kPORT_MuxAsGpio,
                                   /* Does not invert */
                                   (uint16_t)kPORT_InputNormal,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   (uint16_t)kPORT_UnlockRegister};
    /* PORTC6 (pin 44) is configured as PTC6 */
    PORT_SetPinConfig(BOARD_INITPINS_SW3_PORT, BOARD_INITPINS_SW3_PIN, &SW3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

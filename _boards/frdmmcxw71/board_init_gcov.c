/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board_init_gcov.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_cmc.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void Board_InitNMI(void)
{
    CMC_EnableNonMaskablePinInterrupt(CMC0, false);

    gpio_pin_config_t gpiod_pin24_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1U,
    };
    /* Initialize GPIO functionality on pin PTD1 (pin 24)  */
    GPIO_PinInit(GPIOD, 1U, &gpiod_pin24_config);

    const port_pin_config_t portd1_pin24_config_gpio = {/* Internal pull-up/down resistor is disabled */
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
                                                        /* Pin is configured as PTD1 */
                                                        kPORT_MuxAsGpio,
                                                        /* Pin Control Register fields [15:0] are not locked */
                                                        kPORT_UnlockRegister};
    /* PORTD1 (pin 24) is configured as PTD1 */
    PORT_SetPinConfig(PORTD, 1U, &portd1_pin24_config_gpio);

    CMC_EnableNonMaskablePinInterrupt(CMC0, true);

    const port_pin_config_t portd1_pin24_config_nmi = {/* Internal pull-up resistor is enabled */
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
                                                       /* Pin is configured as NMI_b */
                                                       kPORT_MuxAlt3,
                                                       /* Pin Control Register fields [15:0] are not locked */
                                                       kPORT_UnlockRegister};
    /* PORTD1 (pin 24) is configured as NMI_b */
    PORT_SetPinConfig(PORTD, 1U, &portd1_pin24_config_nmi);
}

void Board_DeInitNMI(void)
{
    CMC_EnableNonMaskablePinInterrupt(CMC0, false);

    const port_pin_config_t portd1_pin24_config = {/* Internal pull-up/down resistor is disabled */
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
                                                   /* Pin is configured as PTD1 */
                                                   kPORT_MuxAsGpio,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTD1 (pin 24) is configured as PTD1 */
    PORT_SetPinConfig(PORTD, 1U, &portd1_pin24_config);
}

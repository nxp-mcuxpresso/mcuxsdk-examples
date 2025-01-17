/*
 * Copyright 2022 NXP
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
product: Pins v12.0
processor: MC56F80748
package_id: MC56F80748VLH
mcu_data: ksdk2_0
processor_version: 0.12.7
board: MC56F80000-EVK
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
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
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '3', peripheral: OCCS, signal: EXTAL, pin_signal: GPIOC0/EXTAL/CLKIN0}
  - {pin_num: '4', peripheral: OCCS, signal: XTAL, pin_signal: GPIOC1/XTAL}
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
    /* GPIOC IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOC);
    /* Enable peripheral functionality on pin GPIOC0 (pin 3) */
    GPIO_PinSetPeripheralMode(BOARD_EXTAL_GPIO, BOARD_EXTAL_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC0 (pin 3) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C0_EXTAL);
    /* Enable peripheral functionality on pin GPIOC1 (pin 4) */
    GPIO_PinSetPeripheralMode(BOARD_XTAL_GPIO, BOARD_XTAL_PIN_MASK, kGPIO_ModePeripheral);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

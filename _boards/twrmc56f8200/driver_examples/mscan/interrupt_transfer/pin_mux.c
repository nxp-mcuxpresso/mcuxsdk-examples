/*
 * Copyright 2020-2021 NXP
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
product: Pins v10.0
processor: MC56F82748
package_id: MC56F82748VLH
mcu_data: ksdk2_0
processor_version: 0.10.5
board: TWR-MC56F8200
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_sim.h"
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
  - {pin_num: '41', peripheral: QSCI1, signal: TXD, pin_signal: GPIOF4/TXD1/XB_OUT8/PWM_0X/PWM_FAULT6}
  - {pin_num: '42', peripheral: QSCI1, signal: RXD, pin_signal: GPIOF5/RXD1/XB_OUT9/PWM_1X/PWM_FAULT7}
  - {pin_num: '38', peripheral: CAN0, signal: RX, pin_signal: GPIOC12/CANRX/SDA0/RXD1}
  - {pin_num: '37', peripheral: CAN0, signal: TX, pin_signal: GPIOC11/CANTX/SCL0/TXD1}
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
    /* GPIOF IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOF);
    /* Enable peripheral functionality on pin GPIOC11 (pin 37) */
    GPIO_PinSetPeripheralMode(BOARD_CANTX_GPIO, BOARD_CANTX_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC11 (pin 37) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C11_CANTX);
    /* Enable peripheral functionality on pin GPIOC12 (pin 38) */
    GPIO_PinSetPeripheralMode(BOARD_CANRX_GPIO, BOARD_CANRX_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC12 (pin 38) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C12_CANRX);
    /* Enable peripheral functionality on pin GPIOF4 (pin 41) */
    GPIO_PinSetPeripheralMode(GPIOF, kGPIO_Pin4, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOF4 (pin 41) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_F4_TXD1);
    /* Enable peripheral functionality on pin GPIOF5 (pin 42) */
    GPIO_PinSetPeripheralMode(GPIOF, kGPIO_Pin5, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOF5 (pin 42) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_F5_RXD1);

    /* Internal Peripheral Select */
    /* Select SCI1_RXD Input.: Select SCI1_RXD Input, GPIOC12 or GPIOF5. */
    SIM_SetInternalPeriInput(SIM, kSIM_SCI1_RXDInputIndex, kSIM_SCI1_RXDInput_GPIOC12_GPIOF5);

}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

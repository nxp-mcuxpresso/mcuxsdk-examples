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
processor: MC56F84789
package_id: MC56F84789VLL
mcu_data: ksdk2_0
processor_version: 0.10.4
board: TWR-MC56F8400
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
  - {pin_num: '62', peripheral: QSCI1, signal: TXD, pin_signal: GPIOF4/TXD1/XB_OUT8}
  - {pin_num: '63', peripheral: QSCI1, signal: RXD, pin_signal: GPIOF5/RXD1/XB_OUT9}
  - {pin_num: '21', peripheral: ADC, signal: 'ANA, 4', pin_signal: GPIOA4/ANA4/ANC8/CMPD_IN0}
  - {pin_num: '20', peripheral: ADC, signal: 'ANA, 5', pin_signal: GPIOA5/ANA5/ANC9}
  - {pin_num: '19', peripheral: ADC, signal: 'ANA, 6', pin_signal: GPIOA6/ANA6/ANC10}
  - {pin_num: '17', peripheral: ADC, signal: 'ANA, 7', pin_signal: GPIOA7/ANA7/ANC11}
  - {pin_num: '30', peripheral: ADC, signal: 'ANB, 4', pin_signal: GPIOB4/ANB4/ANC12/CMPC_IN1}
  - {pin_num: '29', peripheral: ADC, signal: 'ANB, 5', pin_signal: GPIOB5/ANB5/ANC13/CMPC_IN2}
  - {pin_num: '28', peripheral: ADC, signal: 'ANB, 6', pin_signal: GPIOB6/ANB6/ANC14/CMPB_IN1}
  - {pin_num: '26', peripheral: ADC, signal: 'ANB, 7', pin_signal: GPIOB7/ANB7/ANC15/CMPB_IN2}
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
    /* GPIOA IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOA);
    /* GPIOB IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOB);
    /* GPIOF IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOF);
    /* Enable peripheral functionality on pin GPIOA4 (pin 21) */
    GPIO_PinSetPeripheralMode(BOARD_THM0P_GPIO, BOARD_THM0P_PIN_MASK, kGPIO_ModePeripheral);
    /* Enable peripheral functionality on pin GPIOA5 (pin 20) */
    GPIO_PinSetPeripheralMode(BOARD_THM0N_GPIO, BOARD_THM0N_PIN_MASK, kGPIO_ModePeripheral);
    /* Enable peripheral functionality on pin GPIOA6 (pin 19) */
    GPIO_PinSetPeripheralMode(BOARD_THM1P_GPIO, BOARD_THM1P_PIN_MASK, kGPIO_ModePeripheral);
    /* Enable peripheral functionality on pin GPIOA7 (pin 17) */
    GPIO_PinSetPeripheralMode(BOARD_THM1N_GPIO, BOARD_THM1N_PIN_MASK, kGPIO_ModePeripheral);
    /* Enable peripheral functionality on pin GPIOB4 (pin 30) */
    GPIO_PinSetPeripheralMode(BOARD_THM2P_GPIO, BOARD_THM2P_PIN_MASK, kGPIO_ModePeripheral);
    /* Enable peripheral functionality on pin GPIOB5 (pin 29) */
    GPIO_PinSetPeripheralMode(BOARD_THM2N_GPIO, BOARD_THM2N_PIN_MASK, kGPIO_ModePeripheral);
    /* Enable peripheral functionality on pin GPIOB6 (pin 28) */
    GPIO_PinSetPeripheralMode(BOARD_THM3P_GPIO, BOARD_THM3P_PIN_MASK, kGPIO_ModePeripheral);
    /* Enable peripheral functionality on pin GPIOB7 (pin 26) */
    GPIO_PinSetPeripheralMode(BOARD_THM3N_GPIO, BOARD_THM3N_PIN_MASK, kGPIO_ModePeripheral);
    /* Enable peripheral functionality on pin GPIOF4 (pin 62) */
    GPIO_PinSetPeripheralMode(GPIOF, kGPIO_Pin4, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOF4 (pin 62) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_F4_TXD1);
    /* Enable peripheral functionality on pin GPIOF5 (pin 63) */
    GPIO_PinSetPeripheralMode(GPIOF, kGPIO_Pin5, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOF5 (pin 63) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_F5_RXD1);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

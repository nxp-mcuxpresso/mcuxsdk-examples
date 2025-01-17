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
  - {pin_num: '7', peripheral: QSCI0, signal: RXD, pin_signal: GPIOC3/TA0/CMPA_O/RXD0/CLKIN1}
  - {pin_num: '5', peripheral: QSCI0, signal: TXD, pin_signal: GPIOC2/TXD0/XB_OUT11/XB_IN2/CLKO0}
  - {pin_num: '3', peripheral: OCCS, signal: EXTAL, pin_signal: GPIOC0/EXTAL/CLKIN0}
  - {pin_num: '4', peripheral: OCCS, signal: XTAL, pin_signal: GPIOC1/XTAL}
  - {pin_num: '33', peripheral: QSPI0, signal: MISO, pin_signal: GPIOC8/MISO0/RXD0/XB_IN9, pull_enable: enable}
  - {pin_num: '34', peripheral: QSPI0, signal: SCLK, pin_signal: GPIOC9/SCLK0/XB_IN4/TXD0/XB_OUT8, pull_enable: enable}
  - {pin_num: '35', peripheral: QSPI0, signal: MOSI, pin_signal: GPIOC10/MOSI0/XB_IN5/MISO0/XB_OUT9, pull_enable: enable}
  - {pin_num: '31', peripheral: QSPI0, signal: SS, pin_signal: GPIOC6/TA2/XB_IN3/CMP_REF/SS0_B, pull_enable: enable}
  - {pin_num: '32', peripheral: GPIOC, signal: 'GPIO, 7', pin_signal: GPIOC7/SS0_B/TXD0/XB_IN8/XB_OUT6, direction: OUTPUT, gpio_init_state: 'true'}
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
    /* Enable peripheral functionality on pin GPIOC2 (pin 5) */
    GPIO_PinSetPeripheralMode(BOARD_Boot_TXD0_GPIO, BOARD_Boot_TXD0_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC2 (pin 5) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C2_TXD0);
    /* Enable peripheral functionality on pin GPIOC3 (pin 7) */
    GPIO_PinSetPeripheralMode(BOARD_Boot_RXD0_GPIO, BOARD_Boot_RXD0_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC3 (pin 7) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C3_RXD0);
    /* Enable peripheral functionality on pin GPIOC6 (pin 31) */
    GPIO_PinSetPeripheralMode(BOARD_ENC_I_GPIO, BOARD_ENC_I_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC6 (pin 31) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C6_SS0_B);
    /* GPIO functionality on pin GPIOC7 (pin 32) */
    GPIO_PinSetPeripheralMode(BOARD_SS0_GPIO, BOARD_SS0_PIN_MASK, kGPIO_ModeGpio);
    /* Enable peripheral functionality on pin GPIOC8 (pin 33) */
    GPIO_PinSetPeripheralMode(BOARD_MISO0_GPIO, BOARD_MISO0_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC8 (pin 33) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C8_MISO0);
    /* Enable peripheral functionality on pin GPIOC9 (pin 34) */
    GPIO_PinSetPeripheralMode(BOARD_SCK0_GPIO, BOARD_SCK0_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC9 (pin 34) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C9_SCLK0);
    /* Enable peripheral functionality on pin GPIOC10 (pin 35) */
    GPIO_PinSetPeripheralMode(BOARD_MOSI0_GPIO, BOARD_MOSI0_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC10 (pin 35) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C10_MOSI0);

    /* Initialize GPIO functionality on pin GPIOC7 (pin 32)  */
    GPIO_PinWrite(BOARD_SS0_GPIO, BOARD_SS0_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_SS0_GPIO, BOARD_SS0_PIN_MASK, kGPIO_DigitalOutput);

    /* Internal Peripheral Select */
    /* Select SCI0_RXD Input.: Select SCI0_RXD Input, GPIOC3 or GPIOC8 or GPIOF8. */
    SIM_SetInternalPeriInput(SIM, kSIM_SCI0_RXDInputIndex, kSIM_SCI0_RXDInput_GPIOC3_GPIOC8_GPIOF8);


    GPIOC->PUR = ((GPIOC->PUR &
                   /* Mask bits to zero which are setting */
                   (~(GPIO_PUR_PU_MASK)))

                  /* Pull Resistor Enable Bits: 0x0740u. */
                  | GPIO_PUR_PU(0x0740u));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

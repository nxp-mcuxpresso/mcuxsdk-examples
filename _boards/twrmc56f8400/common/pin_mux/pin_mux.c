/*
 * Copyright 2024 NXP
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
processor: MC56F84789
package_id: MC56F84789VLL
mcu_data: ksdk2_0
processor_version: 16.3.0
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
- pin_list: []
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
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitBUTTONsPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '5', peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: GPIOC2/TXD0/TB0/XB_IN2/CLKO0, direction: INPUT, gpio_interrupt: gpioIrqFallingEdge, pull_enable: disable}
  - {pin_num: '6', peripheral: GPIOF, signal: 'GPIO, 8', pin_signal: GPIOF8/RXD0/TB1/CMPD_O, direction: INPUT, gpio_interrupt: gpioIrqFallingEdge, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBUTTONsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBUTTONsPins(void)
{
    /* GPIOC IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOC);
    /* GPIOF IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOF);
    /* GPIO functionality on pin GPIOC2 (pin 5) */
    GPIO_PinSetPeripheralMode(BOARD_BUTTON_SW1_GPIO, BOARD_BUTTON_SW1_PIN_MASK, kGPIO_ModeGpio);
    /* GPIO functionality on pin GPIOF8 (pin 6) */
    GPIO_PinSetPeripheralMode(BOARD_BUTTON_SW2_GPIO, BOARD_BUTTON_SW2_PIN_MASK, kGPIO_ModeGpio);

    /* Initialize GPIO functionality on pin GPIOC2 (pin 5)  */
    GPIO_PinSetDirection(BOARD_BUTTON_SW1_GPIO, BOARD_BUTTON_SW1_PIN_MASK, kGPIO_DigitalInput);

    /* Initialize GPIO functionality on pin GPIOF8 (pin 6)  */
    GPIO_PinSetDirection(BOARD_BUTTON_SW2_GPIO, BOARD_BUTTON_SW2_PIN_MASK, kGPIO_DigitalInput);

    /* Interrupt configuration on GPIOC2 (pin 5): Interrupt on falling edge */
    GPIO_PinSetInterruptConfig(BOARD_BUTTON_SW1_GPIO, BOARD_BUTTON_SW1_PIN_MASK, kGPIO_InterruptFallingEdge);

    /* Interrupt configuration on GPIOF8 (pin 6): Interrupt on falling edge */
    GPIO_PinSetInterruptConfig(BOARD_BUTTON_SW2_GPIO, BOARD_BUTTON_SW2_PIN_MASK, kGPIO_InterruptFallingEdge);

    GPIOC->PUR = ((GPIOC->PUR &
                   /* Mask bits to zero which are setting */
                   (~(GPIO_PUR_PU_2_MASK)))

                  /* Pull Resistor Enable Bits: Pull resistor is disabled. */
                  | GPIO_PUR_PU(PUR_PU_2_DISABLED));

    GPIOF->PUR = ((GPIOF->PUR &
                   /* Mask bits to zero which are setting */
                   (~(GPIO_PUR_PU_8_MASK)))

                  /* Pull Resistor Enable Bits: Pull resistor is disabled. */
                  | GPIO_PUR_PU(PUR_PU_8_DISABLED));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '68', peripheral: GPIOE, signal: 'GPIO, 0', pin_signal: GPIOE0/PWMA_0B, identifier: LED_0, direction: OUTPUT, gpio_init_state: 'true', pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: '69', peripheral: GPIOE, signal: 'GPIO, 1', pin_signal: GPIOE1/PWMA_0A, identifier: LED_1, direction: OUTPUT, gpio_init_state: 'true', pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: '74', peripheral: GPIOE, signal: 'GPIO, 2', pin_signal: GPIOE2/PWMA_1B, identifier: LED_2, direction: OUTPUT, gpio_init_state: 'true', pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: '75', peripheral: GPIOE, signal: 'GPIO, 3', pin_signal: GPIOE3/PWMA_1A, identifier: LED_3, direction: OUTPUT, gpio_init_state: 'true', pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: '82', peripheral: GPIOE, signal: 'GPIO, 4', pin_signal: GPIOE4/PWMA_2B/XB_IN2, identifier: LED_4, direction: OUTPUT, gpio_init_state: 'true', pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: '83', peripheral: GPIOE, signal: 'GPIO, 5', pin_signal: GPIOE5/PWMA_2A/XB_IN3, identifier: LED_5, direction: OUTPUT, gpio_init_state: 'true', pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: '84', peripheral: GPIOE, signal: 'GPIO, 6', pin_signal: GPIOE6/PWMA_3B/XB_IN4/PWMB_2B, identifier: LED_6, direction: OUTPUT, gpio_init_state: 'true',
    pull_select: no_init, pull_enable: no_init}
  - {pin_num: '85', peripheral: GPIOE, signal: 'GPIO, 7', pin_signal: GPIOE7/PWMA_3A/XB_IN5/PWMB_2A, identifier: LED_7, direction: OUTPUT, gpio_init_state: 'true',
    pull_select: no_init, pull_enable: no_init}
  - {pin_num: '94', peripheral: GPIOF, signal: 'GPIO, 6', pin_signal: GPIOF6/TB2/PWMA_3X/PWMB_3X/XB_IN2, identifier: LED_8, direction: OUTPUT, gpio_init_state: 'true',
    pull_select: no_init, pull_enable: no_init}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLEDsPins(void)
{
    /* GPIOE IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOE);
    /* GPIOF IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOF);
    /* GPIO functionality on pin GPIOE0 (pin 68) */
    GPIO_PinSetPeripheralMode(BOARD_LED_0_GPIO, BOARD_LED_0_PIN_MASK, kGPIO_ModeGpio);
    /* GPIO functionality on pin GPIOE1 (pin 69) */
    GPIO_PinSetPeripheralMode(BOARD_LED_1_GPIO, BOARD_LED_1_PIN_MASK, kGPIO_ModeGpio);
    /* GPIO functionality on pin GPIOE2 (pin 74) */
    GPIO_PinSetPeripheralMode(BOARD_LED_2_GPIO, BOARD_LED_2_PIN_MASK, kGPIO_ModeGpio);
    /* GPIO functionality on pin GPIOE3 (pin 75) */
    GPIO_PinSetPeripheralMode(BOARD_LED_3_GPIO, BOARD_LED_3_PIN_MASK, kGPIO_ModeGpio);
    /* GPIO functionality on pin GPIOE4 (pin 82) */
    GPIO_PinSetPeripheralMode(BOARD_LED_4_GPIO, BOARD_LED_4_PIN_MASK, kGPIO_ModeGpio);
    /* GPIO functionality on pin GPIOE5 (pin 83) */
    GPIO_PinSetPeripheralMode(BOARD_LED_5_GPIO, BOARD_LED_5_PIN_MASK, kGPIO_ModeGpio);
    /* GPIO functionality on pin GPIOE6 (pin 84) */
    GPIO_PinSetPeripheralMode(BOARD_LED_6_GPIO, BOARD_LED_6_PIN_MASK, kGPIO_ModeGpio);
    /* GPIO functionality on pin GPIOE7 (pin 85) */
    GPIO_PinSetPeripheralMode(BOARD_LED_7_GPIO, BOARD_LED_7_PIN_MASK, kGPIO_ModeGpio);
    /* GPIO functionality on pin GPIOF6 (pin 94) */
    GPIO_PinSetPeripheralMode(BOARD_LED_8_GPIO, BOARD_LED_8_PIN_MASK, kGPIO_ModeGpio);

    /* Initialize GPIO functionality on pin GPIOE0 (pin 68)  */
    GPIO_PinWrite(BOARD_LED_0_GPIO, BOARD_LED_0_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_LED_0_GPIO, BOARD_LED_0_PIN_MASK, kGPIO_DigitalOutput);

    /* Initialize GPIO functionality on pin GPIOE1 (pin 69)  */
    GPIO_PinWrite(BOARD_LED_1_GPIO, BOARD_LED_1_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_LED_1_GPIO, BOARD_LED_1_PIN_MASK, kGPIO_DigitalOutput);

    /* Initialize GPIO functionality on pin GPIOE2 (pin 74)  */
    GPIO_PinWrite(BOARD_LED_2_GPIO, BOARD_LED_2_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_LED_2_GPIO, BOARD_LED_2_PIN_MASK, kGPIO_DigitalOutput);

    /* Initialize GPIO functionality on pin GPIOE3 (pin 75)  */
    GPIO_PinWrite(BOARD_LED_3_GPIO, BOARD_LED_3_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_LED_3_GPIO, BOARD_LED_3_PIN_MASK, kGPIO_DigitalOutput);

    /* Initialize GPIO functionality on pin GPIOE4 (pin 82)  */
    GPIO_PinWrite(BOARD_LED_4_GPIO, BOARD_LED_4_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_LED_4_GPIO, BOARD_LED_4_PIN_MASK, kGPIO_DigitalOutput);

    /* Initialize GPIO functionality on pin GPIOE5 (pin 83)  */
    GPIO_PinWrite(BOARD_LED_5_GPIO, BOARD_LED_5_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_LED_5_GPIO, BOARD_LED_5_PIN_MASK, kGPIO_DigitalOutput);

    /* Initialize GPIO functionality on pin GPIOE6 (pin 84)  */
    GPIO_PinWrite(BOARD_LED_6_GPIO, BOARD_LED_6_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_LED_6_GPIO, BOARD_LED_6_PIN_MASK, kGPIO_DigitalOutput);

    /* Initialize GPIO functionality on pin GPIOE7 (pin 85)  */
    GPIO_PinWrite(BOARD_LED_7_GPIO, BOARD_LED_7_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_LED_7_GPIO, BOARD_LED_7_PIN_MASK, kGPIO_DigitalOutput);

    /* Initialize GPIO functionality on pin GPIOF6 (pin 94)  */
    GPIO_PinWrite(BOARD_LED_8_GPIO, BOARD_LED_8_PIN_MASK, kGPIO_OutputHigh);
    GPIO_PinSetDirection(BOARD_LED_8_GPIO, BOARD_LED_8_PIN_MASK, kGPIO_DigitalOutput);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitTHERMISTORPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '17', peripheral: ADC, signal: 'ANA, 7', pin_signal: GPIOA7/ANA7/ANC11}
  - {pin_num: '19', peripheral: ADC, signal: 'ANA, 6', pin_signal: GPIOA6/ANA6/ANC10}
  - {pin_num: '20', peripheral: ADC, signal: 'ANA, 5', pin_signal: GPIOA5/ANA5/ANC9}
  - {pin_num: '21', peripheral: ADC, signal: 'ANA, 4', pin_signal: GPIOA4/ANA4/ANC8/CMPD_IN0}
  - {pin_num: '26', peripheral: ADC, signal: 'ANB, 7', pin_signal: GPIOB7/ANB7/ANC15/CMPB_IN2}
  - {pin_num: '28', peripheral: ADC, signal: 'ANB, 6', pin_signal: GPIOB6/ANB6/ANC14/CMPB_IN1}
  - {pin_num: '29', peripheral: ADC, signal: 'ANB, 5', pin_signal: GPIOB5/ANB5/ANC13/CMPC_IN2}
  - {pin_num: '30', peripheral: ADC, signal: 'ANB, 4', pin_signal: GPIOB4/ANB4/ANC12/CMPC_IN1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitTHERMISTORPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitTHERMISTORPins(void)
{
    /* GPIOA IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOA);
    /* GPIOB IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOB);
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
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitCANPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '58', peripheral: CAN0, signal: TX, pin_signal: GPIOC11/CANTX/SCL1/TXD1}
  - {pin_num: '59', peripheral: CAN0, signal: RX, pin_signal: GPIOC12/CANRX/SDA1/RXD1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitCANPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitCANPins(void)
{
    /* GPIOC IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOC);
    /* Enable peripheral functionality on pin GPIOC11 (pin 58) */
    GPIO_PinSetPeripheralMode(BOARD_CANTX_GPIO, BOARD_CANTX_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC11 (pin 58) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C11_CANTX);
    /* Enable peripheral functionality on pin GPIOC12 (pin 59) */
    GPIO_PinSetPeripheralMode(BOARD_CANRX_GPIO, BOARD_CANRX_PIN_MASK, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC12 (pin 59) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C12_CANRX);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '62', peripheral: QSCI1, signal: TXD, pin_signal: GPIOF4/TXD1/XB_OUT8}
  - {pin_num: '63', peripheral: QSCI1, signal: RXD, pin_signal: GPIOF5/RXD1/XB_OUT9}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitDEBUG_UARTPins(void)
{
    /* GPIOF IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOF);
    /* Enable peripheral functionality on pin GPIOF4 (pin 62) */
    GPIO_PinSetPeripheralMode(GPIOF, kGPIO_Pin4, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOF4 (pin 62) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_F4_TXD1);
    /* Enable peripheral functionality on pin GPIOF5 (pin 63) */
    GPIO_PinSetPeripheralMode(GPIOF, kGPIO_Pin5, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOF5 (pin 63) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_F5_RXD1);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitI2CPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '87', peripheral: I2C0, signal: SDA, pin_signal: GPIOC14/SDA0/XB_OUT4, open_drain: enable}
  - {pin_num: '88', peripheral: I2C0, signal: SCL, pin_signal: GPIOC15/SCL0/XB_OUT5, open_drain: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitI2CPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitI2CPins(void)
{
    /* GPIOC IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOC);
    /* Enable peripheral functionality on pin GPIOC14 (pin 87) */
    GPIO_PinSetPeripheralMode(GPIOC, kGPIO_Pin14, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC14 (pin 87) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C14_SDA0);
    /* Enable peripheral functionality on pin GPIOC15 (pin 88) */
    GPIO_PinSetPeripheralMode(GPIOC, kGPIO_Pin15, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC15 (pin 88) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C15_SCL0);

    /* Pin configuration on GPIOC14 (pin 87) */
    /* Pull-Pull/Open drain output mode configuration: Open drain output mode */
    GPIO_PinSetOutputMode(GPIOC, kGPIO_Pin14, kGPIO_OutputOpenDrain);

    /* Pin configuration on GPIOC15 (pin 88) */
    /* Pull-Pull/Open drain output mode configuration: Open drain output mode */
    GPIO_PinSetOutputMode(GPIOC, kGPIO_Pin15, kGPIO_OutputOpenDrain);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSPIPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '50', peripheral: QSPI0, signal: SS, pin_signal: GPIOC7/SS0_B/TXD0}
  - {pin_num: '52', peripheral: QSPI0, signal: MISO, pin_signal: GPIOC8/MISO0/RXD0/XB_IN9}
  - {pin_num: '53', peripheral: QSPI0, signal: SCLK, pin_signal: GPIOC9/SCLK0/XB_IN4}
  - {pin_num: '54', peripheral: QSPI0, signal: MOSI, pin_signal: GPIOC10/MOSI0/XB_IN5/MISO0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSPIPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitSPIPins(void)
{
    /* GPIOC IPBus Clock Enable: The peripheral is clocked. */
    CLOCK_EnableClock(kCLOCK_GPIOC);
    /* Enable peripheral functionality on pin GPIOC7 (pin 50) */
    GPIO_PinSetPeripheralMode(GPIOC, kGPIO_Pin7, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC7 (pin 50) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C7_SS0_B);
    /* Enable peripheral functionality on pin GPIOC8 (pin 52) */
    GPIO_PinSetPeripheralMode(GPIOC, kGPIO_Pin8, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC8 (pin 52) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C8_MISO0);
    /* Enable peripheral functionality on pin GPIOC9 (pin 53) */
    GPIO_PinSetPeripheralMode(GPIOC, kGPIO_Pin9, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC9 (pin 53) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C9_SCLK0);
    /* Enable peripheral functionality on pin GPIOC10 (pin 54) */
    GPIO_PinSetPeripheralMode(GPIOC, kGPIO_Pin10, kGPIO_ModePeripheral);
    /* Peripheral functionality on pin GPIOC10 (pin 54) */
    GPIO_PinSetPeripheralMux(kGPIO_Peri_C10_MOSI0);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

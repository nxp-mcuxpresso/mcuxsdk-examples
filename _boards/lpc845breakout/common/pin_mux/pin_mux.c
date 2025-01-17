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
product: Pins v15.0
processor: LPC845
package_id: LPC845M301JBD48
mcu_data: ksdk2_0
processor_version: 0.15.6
board: LPC845BREAKOUT
expansion_headers:
- id: breakout_expansion
  name: Breakout Board Expansion Connector
  connectors:
  - id: C1
    name: CN1
    pins:
    - {id: 1, pin_num: '15', pin_signal: PIO0_16}
    - {id: 2, pin_num: '48', pin_signal: PIO0_17/ADC_9/DACOUT_0}
    - {id: 3, pin_num: '47', pin_signal: PIO0_18/ADC_8}
    - {id: 4, pin_num: '46', pin_signal: PIO0_19/ADC_7}
    - {id: 5, pin_num: '45', pin_signal: PIO0_20/ADC_6}
    - {id: 6, pin_num: '44', pin_signal: PIO0_21/ADC_5}
    - {id: 7, pin_num: '43', pin_signal: PIO0_22/ADC_4}
    - {id: 8, pin_num: '39', pin_signal: PIO0_23/ADC_3/ACMP_I4}
    - {id: 9, name: RESET, pin_num: '5', pin_signal: RESETN/PIO0_5}
    - {id: 10, name: TXD, pin_num: '20', pin_signal: PIO0_24}
    - {id: 11, name: RXD, pin_num: '19', pin_signal: PIO0_25}
    - {id: 12, pin_num: '18', pin_signal: PIO0_26}
    - {id: 13, pin_num: '17', pin_signal: PIO0_27}
    - {id: 14, pin_num: '7', pin_signal: PIO0_28/WKTCLKIN}
    - {id: 15, pin_num: '38', pin_signal: PIO0_29/DACOUT_1}
    - {id: 16, pin_num: '42', pin_signal: PIO0_30/ACMP_I5}
    - {id: 17, pin_num: '9', pin_signal: PIO0_31/CAPT_X0}
    - {id: 18, pin_num: '25', pin_signal: PIO0_9/XTALOUT}
    - {id: 19, pin_num: '26', pin_signal: PIO0_8/XTALIN}
    - {id: 20, name: GND, external_signal_types: ground}
    - {id: 21, pin_num: '26', pin_signal: PIO0_8/XTALIN}
    - {id: 22, pin_num: '25', pin_signal: PIO0_9/XTALOUT}
    - {id: 23, pin_num: '13', pin_signal: PIO0_10/I2C0_SCL}
    - {id: 24, pin_num: '12', pin_signal: PIO0_11/I2C0_SDA}
    - {id: 25, name: ISP, pin_num: '4', pin_signal: PIO0_12}
    - {id: 26, pin_num: '2', pin_signal: PIO0_13/ADC_10}
    - {id: 27, pin_num: '37', pin_signal: PIO0_14/ACMP_I3/ADC_2}
    - {id: 28, pin_num: '22', pin_signal: PIO0_15}
    - {id: 29, name: RED LED, pin_num: '16', pin_signal: PIO1_2/CAPT_X3}
    - {id: 30, name: BLUE LED, pin_num: '14', pin_signal: PIO1_1/CAPT_X2}
    - {id: 31, name: GRN LED, pin_num: '11', pin_signal: PIO1_0/CAPT_X1}
    - {id: 32, name: ADC/POT, pin_num: '33', pin_signal: PIO0_7/ADC_0}
    - {id: 33, pin_num: '34', pin_signal: PIO0_6/ADC_1/ACMPVREF}
    - {id: 34, name: RESET, pin_num: '5', pin_signal: RESETN/PIO0_5}
    - {id: 35, name: USER, pin_num: '6', pin_signal: PIO0_4/ADC_11}
    - {id: 36, name: SWCLK, pin_num: '8', pin_signal: SWCLK/PIO0_3}
    - {id: 37, name: SWDIO, pin_num: '10', pin_signal: SWDIO/PIO0_2}
    - {id: 38, pin_num: '24', pin_signal: PIO0_1/ACMP_I2/CLKIN}
    - {id: 39, pin_num: '36', pin_signal: PIO0_0/ACMP_I1}
    - {id: 40, name: VDD, external_signal_types: power_supply_3.3V}
pin_labels:
- {pin_num: '11', pin_signal: PIO1_0/CAPT_X1, label: 'LD1[3]/CN1[31]/PIO1_0/CAPT_X1', identifier: LED_GREEN}
- {pin_num: '14', pin_signal: PIO1_1/CAPT_X2, label: 'LD1[4]/CN1[30]/PIO1_1/CAPT_X2', identifier: LED_BLUE}
- {pin_num: '16', pin_signal: PIO1_2/CAPT_X3, label: 'LD1[2]/CN1[29]/PIO1_2/CAPT_X3', identifier: LED_RED}
- {pin_num: '1', pin_signal: PIO1_8/CAPT_YL, label: S1/PIO1_8/CAPT_YL, identifier: CAPY_LOW}
- {pin_num: '2', pin_signal: PIO0_13/ADC_10, label: 'CN1[26]/PIO0_13/ADC_10'}
- {pin_num: '3', pin_signal: PIO1_9/CAPT_YH, label: S1/PIO1_9/CAPT_YH, identifier: CAPY_HIGH}
- {pin_num: '4', pin_signal: PIO0_12, label: 'K1/CN1[25]/PIO0_12', identifier: K1}
- {pin_num: '5', pin_signal: RESETN/PIO0_5, label: 'K2/CN3[10]/CN1[9]/CN1[34]/RESET/PIO0_5', identifier: K2}
- {pin_num: '6', pin_signal: PIO0_4/ADC_11, label: 'K3/CN1[35]/PIO0_4/ADC_11/TRSTN/WAKEUP', identifier: K3}
- {pin_num: '7', pin_signal: PIO0_28/WKTCLKIN, label: 'CN1[14]/PIO0_28/WKTCLKIN'}
- {pin_num: '8', pin_signal: SWCLK/PIO0_3, label: 'CN3[4]/CN1[36]/SWCLK/PIO0_3/TCK'}
- {pin_num: '9', pin_signal: PIO0_31/CAPT_X0, label: S1/PIO0_31/CAPT_X0, identifier: CAPX}
- {pin_num: '10', pin_signal: SWDIO/PIO0_2, label: 'CN3[2]/CN1[37]/SWDIO/PIO0_2/TMS'}
- {pin_num: '12', pin_signal: PIO0_11/I2C0_SDA, label: 'CN1[24]/PIO0_11/I2C0_SDA'}
- {pin_num: '13', pin_signal: PIO0_10/I2C0_SCL, label: 'CN1[23]/PIO0_10/I2C0_SCL'}
- {pin_num: '15', pin_signal: PIO0_16, label: 'CN1[1]/PIO0_16'}
- {pin_num: '17', pin_signal: PIO0_27, label: 'CN1[13]/PIO0_27'}
- {pin_num: '18', pin_signal: PIO0_26, label: 'CN1[12]/PIO0_26'}
- {pin_num: '19', pin_signal: PIO0_25, label: 'CN1[11]/PIO0_25', identifier: DEBUG_UART_TX}
- {pin_num: '20', pin_signal: PIO0_24, label: 'CN1[10]/PIO0_24', identifier: DEBUG_UART_RX}
- {pin_num: '21', pin_signal: PIO1_3/CAPT_X4, label: PIO1_3/CAPT_X4}
- {pin_num: '22', pin_signal: PIO0_15, label: 'CN1[28]/PIO0_15'}
- {pin_num: '23', pin_signal: PIO1_4/CAPT_X5, label: PIO1_4/CAPT_X5}
- {pin_num: '24', pin_signal: PIO0_1/ACMP_I2/CLKIN, label: 'CN1[38]/PIO0_1/ACMP_I2/CLKIN/TDI'}
- {pin_num: '25', pin_signal: PIO0_9/XTALOUT, label: 'CN1[18]/CN1[22]/PIO0_9/XTALOUT'}
- {pin_num: '26', pin_signal: PIO0_8/XTALIN, label: 'CN1[19]/CN1[21]/PIO0_8/XTALIN'}
- {pin_num: '27', pin_signal: PIO1_5/CAPT_X6, label: PIO1_5/CAPT_X6}
- {pin_num: '28', pin_signal: PIO1_6/CAPT_X7, label: PIO1_6/CAPT_X7}
- {pin_num: '29', pin_signal: VDD29, label: VDD, identifier: CPU_VDD}
- {pin_num: '30', pin_signal: VSS30, label: VSS}
- {pin_num: '31', pin_signal: VREFN, label: FB2/VREFN}
- {pin_num: '32', pin_signal: VREFP, label: FB1/VREFP}
- {pin_num: '33', pin_signal: PIO0_7/ADC_0, label: 'CN1[32]/RV1/PIO0_7/ADC_0'}
- {pin_num: '34', pin_signal: PIO0_6/ADC_1/ACMPVREF, label: 'CN1[33]/PIO0_6/ADC_1/ACMPVREF'}
- {pin_num: '35', pin_signal: PIO1_7/CAPT_X8, label: PIO1_7/CAPT_X8}
- {pin_num: '36', pin_signal: PIO0_0/ACMP_I1, label: 'CN1[39]/PIO0_0/ACMP_I1/TDO'}
- {pin_num: '37', pin_signal: PIO0_14/ACMP_I3/ADC_2, label: 'CN1[27]/PIO0_14/ACMP_I3/ADC_2'}
- {pin_num: '38', pin_signal: PIO0_29/DACOUT_1, label: 'CN1[15]/PIO0_29/DACOUT_1'}
- {pin_num: '39', pin_signal: PIO0_23/ADC_3/ACMP_I4, label: 'CN1[8]/PIO0_23/ADC_3/ACMP_I4'}
- {pin_num: '40', pin_signal: VDDA, label: FB1/VDDA, identifier: VDDA}
- {pin_num: '41', pin_signal: VSSA, label: FB2/VSSA, identifier: VSSA}
- {pin_num: '42', pin_signal: PIO0_30/ACMP_I5, label: S1/PIO0_30/ACMP_I5, identifier: CAPY_R}
- {pin_num: '43', pin_signal: PIO0_22/ADC_4, label: 'CN1[7]/PIO0_22/ADC_4'}
- {pin_num: '44', pin_signal: PIO0_21/ADC_5, label: 'CN1[6]/PIO0_21/ADC_5'}
- {pin_num: '45', pin_signal: PIO0_20/ADC_6, label: 'CN1[5]/PIO0_20/ADC_6'}
- {pin_num: '46', pin_signal: PIO0_19/ADC_7, label: 'CN1[4]/PIO0_19/ADC_7'}
- {pin_num: '47', pin_signal: PIO0_18/ADC_8, label: 'CN1[3]/PIO0_18/ADC_8'}
- {pin_num: '48', pin_signal: PIO0_17/ADC_9/DACOUT_0, label: 'CN1[2]/PIO0_17/ADC_9/DACOUT_0'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "fsl_swm.h"
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
    BOARD_InitLEDsPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitBUTTONsPins();
    BOARD_InitCAPTPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
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
/* Function assigned for the Cortex-M0P */
void BOARD_InitPins(void)
{
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '11', peripheral: GPIO, signal: 'PIO1, 0', pin_signal: PIO1_0/CAPT_X1, direction: OUTPUT, gpio_init_state: 'true', mode: inactive, invert: disabled,
    hysteresis: enabled, opendrain: disabled, smode: bypass, clkdiv: div0}
  - {pin_num: '14', peripheral: GPIO, signal: 'PIO1, 1', pin_signal: PIO1_1/CAPT_X2, direction: OUTPUT, gpio_init_state: 'true', mode: inactive, invert: disabled,
    hysteresis: enabled, opendrain: disabled, smode: bypass, clkdiv: div0}
  - {pin_num: '16', peripheral: GPIO, signal: 'PIO1, 2', pin_signal: PIO1_2/CAPT_X3, direction: OUTPUT, gpio_init_state: 'true', mode: inactive, invert: disabled,
    hysteresis: enabled, opendrain: disabled, smode: bypass, clkdiv: div0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitLEDsPins(void)
{
    /* Enables clock for IOCON.: enable */
    CLOCK_EnableClock(kCLOCK_Iocon);
    /* Enables the clock for the GPIO1 module */
    CLOCK_EnableClock(kCLOCK_Gpio1);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U,
    };
    /* Initialize GPIO functionality on pin PIO1_0 (pin 11)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_GREEN_GPIO, BOARD_INITLEDSPINS_LED_GREEN_PORT, BOARD_INITLEDSPINS_LED_GREEN_PIN, &LED_GREEN_config);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U,
    };
    /* Initialize GPIO functionality on pin PIO1_1 (pin 14)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_BLUE_GPIO, BOARD_INITLEDSPINS_LED_BLUE_PORT, BOARD_INITLEDSPINS_LED_BLUE_PIN, &LED_BLUE_config);

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U,
    };
    /* Initialize GPIO functionality on pin PIO1_2 (pin 16)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_RED_GPIO, BOARD_INITLEDSPINS_LED_RED_PORT, BOARD_INITLEDSPINS_LED_RED_PIN, &LED_RED_config);

    const uint32_t LED_GREEN = (/* No addition pin function */
                                IOCON_PIO_MODE_INACT |
                                /* Enable hysteresis */
                                IOCON_PIO_HYS_EN |
                                /* Input not invert */
                                IOCON_PIO_INV_DI |
                                /* Disables Open-drain function */
                                IOCON_PIO_OD_DI |
                                /* Bypass input filter */
                                IOCON_PIO_SMODE_BYPASS |
                                /* IOCONCLKDIV0 */
                                IOCON_PIO_CLKDIV0);
    /* PIO1 PIN0 (coords: 11) is configured as GPIO, PIO1, 0. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO1_0, LED_GREEN);

    const uint32_t LED_BLUE = (/* No addition pin function */
                               IOCON_PIO_MODE_INACT |
                               /* Enable hysteresis */
                               IOCON_PIO_HYS_EN |
                               /* Input not invert */
                               IOCON_PIO_INV_DI |
                               /* Disables Open-drain function */
                               IOCON_PIO_OD_DI |
                               /* Bypass input filter */
                               IOCON_PIO_SMODE_BYPASS |
                               /* IOCONCLKDIV0 */
                               IOCON_PIO_CLKDIV0);
    /* PIO1 PIN1 (coords: 14) is configured as GPIO, PIO1, 1. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO1_1, LED_BLUE);

    const uint32_t LED_RED = (/* No addition pin function */
                              IOCON_PIO_MODE_INACT |
                              /* Enable hysteresis */
                              IOCON_PIO_HYS_EN |
                              /* Input not invert */
                              IOCON_PIO_INV_DI |
                              /* Disables Open-drain function */
                              IOCON_PIO_OD_DI |
                              /* Bypass input filter */
                              IOCON_PIO_SMODE_BYPASS |
                              /* IOCONCLKDIV0 */
                              IOCON_PIO_CLKDIV0);
    /* PIO1 PIN2 (coords: 16) is configured as GPIO, PIO1, 2. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO1_2, LED_RED);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '19', peripheral: USART0, signal: TXD, pin_signal: PIO0_25, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
  - {pin_num: '20', peripheral: USART0, signal: RXD, pin_signal: PIO0_24, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitDEBUG_UARTPins(void)
{
    /* Enables clock for IOCON.: enable */
    CLOCK_EnableClock(kCLOCK_Iocon);
    /* Enables clock for switch matrix.: enable */
    CLOCK_EnableClock(kCLOCK_Swm);

    const uint32_t DEBUG_UART_RX = (/* Selects pull-up function */
                                    IOCON_PIO_MODE_PULLUP |
                                    /* Enable hysteresis */
                                    IOCON_PIO_HYS_EN |
                                    /* Input not invert */
                                    IOCON_PIO_INV_DI |
                                    /* Disables Open-drain function */
                                    IOCON_PIO_OD_DI |
                                    /* Bypass input filter */
                                    IOCON_PIO_SMODE_BYPASS |
                                    /* IOCONCLKDIV0 */
                                    IOCON_PIO_CLKDIV0);
    /* PIO0 PIN24 (coords: 20) is configured as USART0, RXD. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_24, DEBUG_UART_RX);

    const uint32_t DEBUG_UART_TX = (/* Selects pull-up function */
                                    IOCON_PIO_MODE_PULLUP |
                                    /* Enable hysteresis */
                                    IOCON_PIO_HYS_EN |
                                    /* Input not invert */
                                    IOCON_PIO_INV_DI |
                                    /* Disables Open-drain function */
                                    IOCON_PIO_OD_DI |
                                    /* Bypass input filter */
                                    IOCON_PIO_SMODE_BYPASS |
                                    /* IOCONCLKDIV0 */
                                    IOCON_PIO_CLKDIV0);
    /* PIO0 PIN25 (coords: 19) is configured as USART0, TXD. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_25, DEBUG_UART_TX);

    /* USART0_TXD connect to P0_25 */
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_TXD, kSWM_PortPin_P0_25);

    /* USART0_RXD connect to P0_24 */
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_RXD, kSWM_PortPin_P0_24);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSWD_DEBUGPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '10', peripheral: SWD, signal: SWDIO, pin_signal: SWDIO/PIO0_2, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
  - {pin_num: '8', peripheral: SWD, signal: SWCLK, pin_signal: SWCLK/PIO0_3, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
  - {pin_num: '5', peripheral: SYSCON, signal: RESETN, pin_signal: RESETN/PIO0_5, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSWD_DEBUGPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitSWD_DEBUGPins(void)
{
    /* Enables clock for IOCON.: enable */
    CLOCK_EnableClock(kCLOCK_Iocon);
    /* Enables clock for switch matrix.: enable */
    CLOCK_EnableClock(kCLOCK_Swm);

    const uint32_t IOCON_INDEX_PIO0_2_config = (/* Selects pull-up function */
                                                IOCON_PIO_MODE_PULLUP |
                                                /* Enable hysteresis */
                                                IOCON_PIO_HYS_EN |
                                                /* Input not invert */
                                                IOCON_PIO_INV_DI |
                                                /* Disables Open-drain function */
                                                IOCON_PIO_OD_DI |
                                                /* Bypass input filter */
                                                IOCON_PIO_SMODE_BYPASS |
                                                /* IOCONCLKDIV0 */
                                                IOCON_PIO_CLKDIV0);
    /* PIO0 PIN2 (coords: 10) is configured as SWD, SWDIO. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_2, IOCON_INDEX_PIO0_2_config);

    const uint32_t IOCON_INDEX_PIO0_3_config = (/* Selects pull-up function */
                                                IOCON_PIO_MODE_PULLUP |
                                                /* Enable hysteresis */
                                                IOCON_PIO_HYS_EN |
                                                /* Input not invert */
                                                IOCON_PIO_INV_DI |
                                                /* Disables Open-drain function */
                                                IOCON_PIO_OD_DI |
                                                /* Bypass input filter */
                                                IOCON_PIO_SMODE_BYPASS |
                                                /* IOCONCLKDIV0 */
                                                IOCON_PIO_CLKDIV0);
    /* PIO0 PIN3 (coords: 8) is configured as SWD, SWCLK. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_3, IOCON_INDEX_PIO0_3_config);

    const uint32_t K2 = (/* Selects pull-up function */
                         IOCON_PIO_MODE_PULLUP |
                         /* Enable hysteresis */
                         IOCON_PIO_HYS_EN |
                         /* Input not invert */
                         IOCON_PIO_INV_DI |
                         /* Disables Open-drain function */
                         IOCON_PIO_OD_DI |
                         /* Bypass input filter */
                         IOCON_PIO_SMODE_BYPASS |
                         /* IOCONCLKDIV0 */
                         IOCON_PIO_CLKDIV0);
    /* PIO0 PIN5 (coords: 5) is configured as SYSCON, RESETN. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_5, K2);

    /* SWCLK connect to P0_3 */
    SWM_SetFixedPinSelect(SWM0, kSWM_SWCLK, true);

    /* SWDIO connect to P0_2 */
    SWM_SetFixedPinSelect(SWM0, kSWM_SWDIO, true);

    /* RESETN connect to P0_5 */
    SWM_SetFixedPinSelect(SWM0, kSWM_RESETN, true);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitBUTTONsPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '6', peripheral: GPIO, signal: 'PIO0, 4', pin_signal: PIO0_4/ADC_11, direction: INPUT, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled,
    smode: bypass, clkdiv: div0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBUTTONsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitBUTTONsPins(void)
{
    /* Enables clock for IOCON.: enable */
    CLOCK_EnableClock(kCLOCK_Iocon);
    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_Gpio0);

    gpio_pin_config_t K3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_4 (pin 6)  */
    GPIO_PinInit(BOARD_INITBUTTONSPINS_K3_GPIO, BOARD_INITBUTTONSPINS_K3_PORT, BOARD_INITBUTTONSPINS_K3_PIN, &K3_config);

    const uint32_t K3 = (/* Selects pull-up function */
                         IOCON_PIO_MODE_PULLUP |
                         /* Enable hysteresis */
                         IOCON_PIO_HYS_EN |
                         /* Input not invert */
                         IOCON_PIO_INV_DI |
                         /* Disables Open-drain function */
                         IOCON_PIO_OD_DI |
                         /* Bypass input filter */
                         IOCON_PIO_SMODE_BYPASS |
                         /* IOCONCLKDIV0 */
                         IOCON_PIO_CLKDIV0);
    /* PIO0 PIN4 (coords: 6) is configured as GPIO, PIO0, 4. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_4, K3);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitCAPTPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: CAPT, signal: CAPTYL, pin_signal: PIO1_8/CAPT_YL, mode: inactive, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
  - {pin_num: '3', peripheral: CAPT, signal: CAPTYH, pin_signal: PIO1_9/CAPT_YH, mode: inactive, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
  - {pin_num: '9', peripheral: CAPT, signal: 'CAPTX, 0', pin_signal: PIO0_31/CAPT_X0, mode: inactive, invert: disabled, hysteresis: enabled, opendrain: disabled,
    smode: bypass, clkdiv: div0}
  - {pin_num: '42', peripheral: ACMP, signal: ACMP_IN5, pin_signal: PIO0_30/ACMP_I5, mode: inactive, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitCAPTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitCAPTPins(void)
{
    /* Enables clock for IOCON.: enable */
    CLOCK_EnableClock(kCLOCK_Iocon);
    /* Enables clock for switch matrix.: enable */
    CLOCK_EnableClock(kCLOCK_Swm);

    const uint32_t CAPY_R = (/* No addition pin function */
                             IOCON_PIO_MODE_INACT |
                             /* Enable hysteresis */
                             IOCON_PIO_HYS_EN |
                             /* Input not invert */
                             IOCON_PIO_INV_DI |
                             /* Disables Open-drain function */
                             IOCON_PIO_OD_DI |
                             /* Bypass input filter */
                             IOCON_PIO_SMODE_BYPASS |
                             /* IOCONCLKDIV0 */
                             IOCON_PIO_CLKDIV0);
    /* PIO0 PIN30 (coords: 42) is configured as ACMP, ACMP_IN5. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_30, CAPY_R);

    const uint32_t CAPX = (/* No addition pin function */
                           IOCON_PIO_MODE_INACT |
                           /* Enable hysteresis */
                           IOCON_PIO_HYS_EN |
                           /* Input not invert */
                           IOCON_PIO_INV_DI |
                           /* Disables Open-drain function */
                           IOCON_PIO_OD_DI |
                           /* Bypass input filter */
                           IOCON_PIO_SMODE_BYPASS |
                           /* IOCONCLKDIV0 */
                           IOCON_PIO_CLKDIV0);
    /* PIO0 PIN31 (coords: 9) is configured as CAPT, CAPTX, 0. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_31, CAPX);

    const uint32_t CAPY_LOW = (/* No addition pin function */
                               IOCON_PIO_MODE_INACT |
                               /* Enable hysteresis */
                               IOCON_PIO_HYS_EN |
                               /* Input not invert */
                               IOCON_PIO_INV_DI |
                               /* Disables Open-drain function */
                               IOCON_PIO_OD_DI |
                               /* Bypass input filter */
                               IOCON_PIO_SMODE_BYPASS |
                               /* IOCONCLKDIV0 */
                               IOCON_PIO_CLKDIV0);
    /* PIO1 PIN8 (coords: 1) is configured as CAPT, CAPTYL. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO1_8, CAPY_LOW);

    const uint32_t CAPY_HIGH = (/* No addition pin function */
                                IOCON_PIO_MODE_INACT |
                                /* Enable hysteresis */
                                IOCON_PIO_HYS_EN |
                                /* Input not invert */
                                IOCON_PIO_INV_DI |
                                /* Disables Open-drain function */
                                IOCON_PIO_OD_DI |
                                /* Bypass input filter */
                                IOCON_PIO_SMODE_BYPASS |
                                /* IOCONCLKDIV0 */
                                IOCON_PIO_CLKDIV0);
    /* PIO1 PIN9 (coords: 3) is configured as CAPT, CAPTYH. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO1_9, CAPY_HIGH);

    /* ACMP_INPUT5 connect to P0_30 */
    SWM_SetFixedPinSelect(SWM0, kSWM_ACMP_INPUT5, true);

    /* CAPT_X0 connect to P0_31 */
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X0, true);

    /* CAPT_YL connect to P1_8 */
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_YL, true);

    /* CAPT_YH connect to P1_9 */
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_YH, true);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

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
processor: LPC802
package_id: LPC802M001JDH20
mcu_data: ksdk2_0
processor_version: 0.15.6
board: LPCXpresso802
pin_labels:
- {pin_num: '14', pin_signal: PIO0_8/ADC_5, label: 'CN5[7]/CN3[25]/CN6[4]/LD19/LD20/LD4/PB3/PIO0_8_RXD_User_GPIO2_LED1', identifier: LED_BLUE;User_PB3}
- {pin_num: '13', pin_signal: PIO0_9/ADC_4, label: 'CN5[8]/CN6[1]/CN3[28]/LD21/LD22/LD5/PIO0_9_TXD_GPIO1_LED2', identifier: LED_RED}
- {pin_num: '4', pin_signal: PIO0_12, label: 'CN4[4]/CN3[26]/CN3[24]/CN6[3]/CN9[1]/LD23/LD24/LD6/PB1/PIO0_12_ISP_GPIO0_LED3', identifier: LED_GREEN;ISP_PB1}
- {pin_num: '6', pin_signal: PIO0_4/ADC_11, label: 'CN4[6]/CN3[32]/PIO0_4_TXD_SCK', identifier: DEBUG_UART_TX}
- {pin_num: '19', pin_signal: PIO0_0/ACMP_I1, label: 'CN5[2]/CN3[31]/PIO0_0_RXD', identifier: DEBUG_UART_RX}
- {pin_num: '8', pin_signal: SWDIO/PIO0_2, label: 'CN2[2]/CN4[8]/U3[17]/PIO0_2_SWDIO', identifier: DEBUG_SWD_SWDIO}
- {pin_num: '7', pin_signal: SWCLK/PIO0_3, label: 'CN2[4]/CN4[7]/U3[16]/PIO0_3_SWCLK', identifier: DEBUG_SWD_SWCLK}
- {pin_num: '5', pin_signal: RESETN/PIO0_5, label: 'CN4[5]/CN2[10]/CN3[3]/U3[3]/U3[8]/PB2/PIO0_5_nRST', identifier: DEBUG_SWD_RESETN}
- {pin_num: '1', pin_signal: PIO0_16/ACMP_I4/ADC_3, label: 'CN4[1]/CN3[15]/CN7[1]/U7[2]/PIO0_16_SCL', identifier: I2C_SCL}
- {pin_num: '10', pin_signal: PIO0_10/ADC_7, label: 'CN4[10]/CN3[16]/CN7[2]/U7[1]/PIO0_10_SDA', identifier: I2C_SDA}
- {pin_num: '2', pin_signal: PIO0_17/ADC_9, label: 'CN4[2]/CN3[23]/CN9[2]/T2/IO0_17_PWM4_VDDIO'}
- {pin_num: '3', pin_signal: PIO0_13/ADC_10, label: 'CN4[3]/CN3[21]/U6[5]/PIO0_13_MOSI'}
- {pin_num: '9', pin_signal: PIO0_11/ADC_6/WKTCLKIN, label: 'CN4[9]/CN3[22]/U6[1]/PIO0_11_SPI_CS'}
- {pin_num: '11', pin_signal: PIO0_15/ADC_8, label: 'CN5[10]/CN3[9]/CN8[4]/RV1/PIO0_15_Ain0'}
- {pin_num: '12', pin_signal: PIO0_1/ADC_0/ACMP_I2/CLKIN, label: 'CN5[9]/CN3[20]/U6[2]/PIO0_1_MISO'}
- {pin_num: '17', pin_signal: PIO0_7/ADC_1/ACMPVREF, label: 'CN5[4]/CN3[27]/CN6[2]/LD17/LD18/PIO0_7_GPIO3'}
- {pin_num: '20', pin_signal: PIO0_14/ACMP_I3/ADC_2, label: 'CN5[1]/U6[6]/PIO0_14_SCLK'}
- {pin_num: '18', pin_signal: VREFP, label: 'CN5[3]/CN3[17]/CN8[3]/RV1/JP2/M_VREF'}
- {pin_num: '15', pin_signal: VDD, label: 'CN5[6]/JP1/MCU_VDD'}
- {pin_num: '16', pin_signal: VSS, label: GND}
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
    BOARD_InitDEBUG_UARTPins();
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
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '14', peripheral: GPIO, signal: 'PIO0, 8', pin_signal: PIO0_8/ADC_5, identifier: LED_BLUE, direction: OUTPUT, gpio_init_state: 'true', mode: inactive,
    invert: disabled, hysteresis: enabled, opendrain: disabled}
  - {pin_num: '13', peripheral: GPIO, signal: 'PIO0, 9', pin_signal: PIO0_9/ADC_4, direction: OUTPUT, gpio_init_state: 'true', mode: inactive, invert: disabled, hysteresis: enabled,
    opendrain: disabled}
  - {pin_num: '4', peripheral: GPIO, signal: 'PIO0, 12', pin_signal: PIO0_12, identifier: LED_GREEN, direction: OUTPUT, gpio_init_state: 'true', mode: inactive, invert: disabled,
    hysteresis: enabled, opendrain: disabled}
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
    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_Gpio0);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U,
    };
    /* Initialize GPIO functionality on pin PIO0_8 (pin 14)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_BLUE_GPIO, BOARD_INITLEDSPINS_LED_BLUE_PORT, BOARD_INITLEDSPINS_LED_BLUE_PIN, &LED_BLUE_config);

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U,
    };
    /* Initialize GPIO functionality on pin PIO0_9 (pin 13)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_RED_GPIO, BOARD_INITLEDSPINS_LED_RED_PORT, BOARD_INITLEDSPINS_LED_RED_PIN, &LED_RED_config);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U,
    };
    /* Initialize GPIO functionality on pin PIO0_12 (pin 4)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_GREEN_GPIO, BOARD_INITLEDSPINS_LED_GREEN_PORT, BOARD_INITLEDSPINS_LED_GREEN_PIN, &LED_GREEN_config);

    const uint32_t LED_GREEN = (/* No addition pin function */
                                IOCON_PIO_MODE_INACT |
                                /* Enable hysteresis */
                                IOCON_PIO_HYS_EN |
                                /* Input not invert */
                                IOCON_PIO_INV_DI |
                                /* Disables Open-drain function */
                                IOCON_PIO_OD_DI);
    /* PIO0 PIN12 (coords: 4) is configured as GPIO, PIO0, 12. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_12, LED_GREEN);

    const uint32_t LED_BLUE = (/* No addition pin function */
                               IOCON_PIO_MODE_INACT |
                               /* Enable hysteresis */
                               IOCON_PIO_HYS_EN |
                               /* Input not invert */
                               IOCON_PIO_INV_DI |
                               /* Disables Open-drain function */
                               IOCON_PIO_OD_DI);
    /* PIO0 PIN8 (coords: 14) is configured as GPIO, PIO0, 8. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_8, LED_BLUE);

    const uint32_t LED_RED = (/* No addition pin function */
                              IOCON_PIO_MODE_INACT |
                              /* Enable hysteresis */
                              IOCON_PIO_HYS_EN |
                              /* Input not invert */
                              IOCON_PIO_INV_DI |
                              /* Disables Open-drain function */
                              IOCON_PIO_OD_DI);
    /* PIO0 PIN9 (coords: 13) is configured as GPIO, PIO0, 9. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_9, LED_RED);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '6', peripheral: USART0, signal: TXD, pin_signal: PIO0_4/ADC_11, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled}
  - {pin_num: '19', peripheral: USART0, signal: RXD, pin_signal: PIO0_0/ACMP_I1, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled}
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
                                    IOCON_PIO_OD_DI);
    /* PIO0 PIN0 (coords: 19) is configured as USART0, RXD. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_0, DEBUG_UART_RX);

    const uint32_t DEBUG_UART_TX = (/* Selects pull-up function */
                                    IOCON_PIO_MODE_PULLUP |
                                    /* Enable hysteresis */
                                    IOCON_PIO_HYS_EN |
                                    /* Input not invert */
                                    IOCON_PIO_INV_DI |
                                    /* Disables Open-drain function */
                                    IOCON_PIO_OD_DI);
    /* PIO0 PIN4 (coords: 6) is configured as USART0, TXD. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_4, DEBUG_UART_TX);

    /* USART0_TXD connect to P0_4 */
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_TXD, kSWM_PortPin_P0_4);

    /* USART0_RXD connect to P0_0 */
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_RXD, kSWM_PortPin_P0_0);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSWD_DEBUGPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '7', peripheral: SWD, signal: SWCLK, pin_signal: SWCLK/PIO0_3, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled}
  - {pin_num: '8', peripheral: SWD, signal: SWDIO, pin_signal: SWDIO/PIO0_2, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled}
  - {pin_num: '5', peripheral: SYSCON, signal: RESETN, pin_signal: RESETN/PIO0_5, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled}
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

    const uint32_t DEBUG_SWD_SWDIO = (/* Selects pull-up function */
                                      IOCON_PIO_MODE_PULLUP |
                                      /* Enable hysteresis */
                                      IOCON_PIO_HYS_EN |
                                      /* Input not invert */
                                      IOCON_PIO_INV_DI |
                                      /* Disables Open-drain function */
                                      IOCON_PIO_OD_DI);
    /* PIO0 PIN2 (coords: 8) is configured as SWD, SWDIO. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_2, DEBUG_SWD_SWDIO);

    const uint32_t DEBUG_SWD_SWCLK = (/* Selects pull-up function */
                                      IOCON_PIO_MODE_PULLUP |
                                      /* Enable hysteresis */
                                      IOCON_PIO_HYS_EN |
                                      /* Input not invert */
                                      IOCON_PIO_INV_DI |
                                      /* Disables Open-drain function */
                                      IOCON_PIO_OD_DI);
    /* PIO0 PIN3 (coords: 7) is configured as SWD, SWCLK. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_3, DEBUG_SWD_SWCLK);

    const uint32_t DEBUG_SWD_RESETN = (/* Selects pull-up function */
                                       IOCON_PIO_MODE_PULLUP |
                                       /* Enable hysteresis */
                                       IOCON_PIO_HYS_EN |
                                       /* Input not invert */
                                       IOCON_PIO_INV_DI |
                                       /* Disables Open-drain function */
                                       IOCON_PIO_OD_DI);
    /* PIO0 PIN5 (coords: 5) is configured as SYSCON, RESETN. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_5, DEBUG_SWD_RESETN);

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
BOARD_InitI2CPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: I2C0, signal: SCL, pin_signal: PIO0_16/ACMP_I4/ADC_3, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: enabled}
  - {pin_num: '10', peripheral: I2C0, signal: SDA, pin_signal: PIO0_10/ADC_7, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: enabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitI2CPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitI2CPins(void)
{
    /* Enables clock for IOCON.: enable */
    CLOCK_EnableClock(kCLOCK_Iocon);
    /* Enables clock for switch matrix.: enable */
    CLOCK_EnableClock(kCLOCK_Swm);

    const uint32_t I2C_SDA = (/* Selects pull-up function */
                              IOCON_PIO_MODE_PULLUP |
                              /* Enable hysteresis */
                              IOCON_PIO_HYS_EN |
                              /* Input not invert */
                              IOCON_PIO_INV_DI |
                              /* Enables Open-drain function */
                              IOCON_PIO_OD_EN);
    /* PIO0 PIN10 (coords: 10) is configured as I2C0, SDA. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_10, I2C_SDA);

    const uint32_t I2C_SCL = (/* Selects pull-up function */
                              IOCON_PIO_MODE_PULLUP |
                              /* Enable hysteresis */
                              IOCON_PIO_HYS_EN |
                              /* Input not invert */
                              IOCON_PIO_INV_DI |
                              /* Enables Open-drain function */
                              IOCON_PIO_OD_EN);
    /* PIO0 PIN16 (coords: 1) is configured as I2C0, SCL. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_16, I2C_SCL);

    /* I2C0_SDA connect to P0_10 */
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C0_SDA, kSWM_PortPin_P0_10);

    /* I2C0_SCL connect to P0_16 */
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C0_SCL, kSWM_PortPin_P0_16);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitBUTTONsPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '14', peripheral: GPIO, signal: 'PIO0, 8', pin_signal: PIO0_8/ADC_5, identifier: User_PB3, direction: INPUT, mode: pullUp, invert: disabled, hysteresis: enabled,
    opendrain: disabled}
  - {pin_num: '4', peripheral: GPIO, signal: 'PIO0, 12', pin_signal: PIO0_12, identifier: ISP_PB1, direction: INPUT, mode: pullUp, invert: disabled, hysteresis: enabled,
    opendrain: disabled}
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

    gpio_pin_config_t User_PB3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_8 (pin 14)  */
    GPIO_PinInit(BOARD_INITBUTTONSPINS_User_PB3_GPIO, BOARD_INITBUTTONSPINS_User_PB3_PORT, BOARD_INITBUTTONSPINS_User_PB3_PIN, &User_PB3_config);

    gpio_pin_config_t ISP_PB1_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_12 (pin 4)  */
    GPIO_PinInit(BOARD_INITBUTTONSPINS_ISP_PB1_GPIO, BOARD_INITBUTTONSPINS_ISP_PB1_PORT, BOARD_INITBUTTONSPINS_ISP_PB1_PIN, &ISP_PB1_config);

    const uint32_t ISP_PB1 = (/* Selects pull-up function */
                              IOCON_PIO_MODE_PULLUP |
                              /* Enable hysteresis */
                              IOCON_PIO_HYS_EN |
                              /* Input not invert */
                              IOCON_PIO_INV_DI |
                              /* Disables Open-drain function */
                              IOCON_PIO_OD_DI);
    /* PIO0 PIN12 (coords: 4) is configured as GPIO, PIO0, 12. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_12, ISP_PB1);

    const uint32_t User_PB3 = (/* Selects pull-up function */
                               IOCON_PIO_MODE_PULLUP |
                               /* Enable hysteresis */
                               IOCON_PIO_HYS_EN |
                               /* Input not invert */
                               IOCON_PIO_INV_DI |
                               /* Disables Open-drain function */
                               IOCON_PIO_OD_DI);
    /* PIO0 PIN8 (coords: 14) is configured as GPIO, PIO0, 8. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_8, User_PB3);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

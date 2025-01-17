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
processor: RW612
package_id: RW612ETA2I
mcu_data: ksdk2_0
processor_version: 16.3.0
board: FRDM-RW612
pin_labels:
- {pin_num: M2, pin_signal: GPIO_11, label: 'J1[6]', identifier: SW2}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_io_mux.h"
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
- options: {callFromInitBoot: 'true', coreID: cm33, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : 
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitPins(void)
{
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: E5, peripheral: FLEXCOMM3, signal: USART_RXD, pin_signal: GPIO_24}
  - {pin_num: F6, peripheral: FLEXCOMM3, signal: USART_TXD, pin_signal: GPIO_26}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitDEBUG_UARTPins(void)
{
    /* Initialize FC3_USART_DATA functionality on pin GPIO_24, GPIO_26 (pin E5_F6) */
    IO_MUX_SetPinMux(IO_MUX_FC3_USART_DATA);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSWD_DEBUGPins:
- options: {callFromInitBoot: 'false', coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: G5, peripheral: SWD, signal: CLK, pin_signal: GPIO_13}
  - {pin_num: K4, peripheral: SWD, signal: IO, pin_signal: GPIO_14}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSWD_DEBUGPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitSWD_DEBUGPins(void)
{

    MCI_IO_MUX->C_TIMER_IN = ((MCI_IO_MUX->C_TIMER_IN &
                               /* Mask bits to zero which are setting */
                               (~(MCI_IO_MUX_C_TIMER_IN_CT_INP3_SEL_MASK | MCI_IO_MUX_C_TIMER_IN_CT_INP4_SEL_MASK)))

                              /* ct_inp3 sel: 0x00u */
                              | MCI_IO_MUX_C_TIMER_IN_CT_INP3_SEL(0x00u)
                              /* ct_inp4 sel: 0x00u */
                              | MCI_IO_MUX_C_TIMER_IN_CT_INP4_SEL(0x00u));

    MCI_IO_MUX->C_TIMER_OUT = ((MCI_IO_MUX->C_TIMER_OUT &
                                /* Mask bits to zero which are setting */
                                (~(MCI_IO_MUX_C_TIMER_OUT_CT0MAT3_SEL_MASK | MCI_IO_MUX_C_TIMER_OUT_CT1MAT0_SEL_MASK)))

                               /* ct0mat3 sel: 0x00u */
                               | MCI_IO_MUX_C_TIMER_OUT_CT0MAT3_SEL(0x00u)
                               /* ct1mat0 sel: 0x00u */
                               | MCI_IO_MUX_C_TIMER_OUT_CT1MAT0_SEL(0x00u));

    MCI_IO_MUX->FC2 =
        ((MCI_IO_MUX->FC2 &
          /* Mask bits to zero which are setting */
          (~(MCI_IO_MUX_FC2_SEL_FC2_I2C_MASK | MCI_IO_MUX_FC2_SEL_FC2_I2S_MASK | MCI_IO_MUX_FC2_SEL_FC2_SPI_MASK | MCI_IO_MUX_FC2_SEL_FC2_I2S_DATA_ONLY_MASK | MCI_IO_MUX_FC2_SEL_FC2_USART_DATA_MASK)))

         /* flexcomm2:select GPIO-13/14 as i2c function: 0x00u */
         | MCI_IO_MUX_FC2_SEL_FC2_I2C(0x00u)
         /* flexcomm2:select GPIO-13/14/15 as i2s function: 0x00u */
         | MCI_IO_MUX_FC2_SEL_FC2_I2S(0x00u)
         /* flexcomm2:select GPIO-13/14/15/16 as spi function: 0x00u */
         | MCI_IO_MUX_FC2_SEL_FC2_SPI(0x00u)
         /* flexcomm2:select GPIO-13 as i2s data function: 0x00u */
         | MCI_IO_MUX_FC2_SEL_FC2_I2S_DATA_ONLY(0x00u)
         /* flexcomm2:select GPIO-13/14 as usart rxd/txd: 0x00u */
         | MCI_IO_MUX_FC2_SEL_FC2_USART_DATA(0x00u));

    MCI_IO_MUX->GPIO_GRP0 = ((MCI_IO_MUX->GPIO_GRP0 &
                              /* Mask bits to zero which are setting */
                              (~(MCI_IO_MUX_GPIO_GRP0_SEL_13_MASK | MCI_IO_MUX_GPIO_GRP0_SEL_14_MASK)))

                             /* pio0[31:0] selection, high valid; sel[i]->pio0[i]->GPIO[i]: 0x00u */
                             | MCI_IO_MUX_GPIO_GRP0_SEL(0x00u));

    SOCCTRL->MCI_IOMUX_EN0 = ((SOCCTRL->MCI_IOMUX_EN0 &
                               /* Mask bits to zero which are setting */
                               (~(SOCCIU_MCI_IOMUX_EN0_EN_21_0_13_MASK | SOCCIU_MCI_IOMUX_EN0_EN_21_0_14_MASK)))

                              /* Bitwise enable control for mci_io_mux GPIO[21:0]: 0x00u */
                              | SOCCIU_MCI_IOMUX_EN0_EN_21_0(0x00u));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: M1, peripheral: GPIO, signal: 'PIO0, 0', pin_signal: GPIO_0, direction: OUTPUT}
  - {pin_num: N2, peripheral: GPIO, signal: 'PIO0, 1', pin_signal: GPIO_1, direction: OUTPUT}
  - {pin_num: N8, peripheral: GPIO, signal: 'PIO0, 12', pin_signal: GPIO_12, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitLEDsPins(void)
{
    /* Enables the clock for the GPIO0 module */
    GPIO_PortInit(GPIO, 0);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_0 (pin M1)  */
    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, &LED_BLUE_config);

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_1 (pin N2)  */
    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, &LED_RED_config);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_12 (pin N8)  */
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, &LED_GREEN_config);
    /* Initialize GPIO0 functionality on pin GPIO_0 (pin M1) */
    IO_MUX_SetPinMux(IO_MUX_GPIO0);
    /* Initialize GPIO1 functionality on pin GPIO_1 (pin N2) */
    IO_MUX_SetPinMux(IO_MUX_GPIO1);
    /* Initialize GPIO12 functionality on pin GPIO_12 (pin N8) */
    IO_MUX_SetPinMux(IO_MUX_GPIO12);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitBUTTONsPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: M2, peripheral: GPIO, signal: 'PIO0, 11', pin_signal: GPIO_11, direction: INPUT, gpio_interrupt_a_enable: enabled, gpio_interrupt_config: gpioIrqFallingEdge}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBUTTONsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitBUTTONsPins(void)
{
    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_HsGpio0);

    gpio_pin_config_t SW2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    gpio_interrupt_config_t SW2_int_config = {
        .mode = kGPIO_PinIntEnableEdge,
        .polarity = kGPIO_PinIntEnableLowOrFall
    };
    /* Initialize GPIO functionality on pin PIO0_11 (pin M2)  */
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_PORT, BOARD_SW2_PIN, &SW2_config);
    GPIO_SetPinInterruptConfig(BOARD_SW2_GPIO, BOARD_SW2_PORT, BOARD_SW2_PIN, &SW2_int_config);
    GPIO_PinEnableInterrupt(BOARD_SW2_GPIO, BOARD_SW2_PORT, BOARD_SW2_PIN, (uint32_t)kGPIO_InterruptA);
    /* Initialize GPIO11 functionality on pin GPIO_11 (pin M2) */
    IO_MUX_SetPinMux(IO_MUX_GPIO11);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitNETPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: E7, peripheral: ENET, signal: ENET_REF_CLK, pin_signal: GPIO_25, slew_rate: weak}
  - {pin_num: B6, peripheral: ENET, signal: 'ENET_RX_DATA, 0', pin_signal: GPIO_22}
  - {pin_num: B5, peripheral: ENET, signal: 'ENET_RX_DATA, 1', pin_signal: GPIO_23}
  - {pin_num: G11, peripheral: ENET, signal: 'ENET_TX_DATA, 0', pin_signal: GPIO_58, slew_rate: weak}
  - {pin_num: D12, peripheral: ENET, signal: 'ENET_TX_DATA, 1', pin_signal: GPIO_59, slew_rate: weak}
  - {pin_num: B13, peripheral: ENET, signal: ENET_MDIO, pin_signal: GPIO_57}
  - {pin_num: H4, peripheral: GPIO, signal: 'PIO0, 21', pin_signal: GPIO_21, direction: OUTPUT, gpio_init_state: 'true'}
  - {pin_num: K6, peripheral: GPIO, signal: 'PIO1, 23', pin_signal: GPIO_55, direction: INPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitNETPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitNETPins(void)
{
    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_HsGpio0);
    /* Enables the clock for the GPIO1 module */
    GPIO_PortInit(GPIO, 1);

    gpio_pin_config_t gpio0_pinH4_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PIO0_21 (pin H4)  */
    GPIO_PinInit(GPIO, 0U, 21U, &gpio0_pinH4_config);

    gpio_pin_config_t gpio1_pinK6_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO1_23 (pin K6)  */
    GPIO_PinInit(GPIO, 1U, 23U, &gpio1_pinK6_config);
    /* Initialize ENET_CLK functionality on pin GPIO_25 (pin E7) */
    IO_MUX_SetPinMux(IO_MUX_ENET_CLK);
    /* Initialize ENET_RX functionality on pin GPIO_22, GPIO_23 (pin B6_B5) */
    IO_MUX_SetPinMux(IO_MUX_ENET_RX);
    /* Initialize ENET_TX functionality on pin GPIO_58, GPIO_59 (pin G11_D12) */
    IO_MUX_SetPinMux(IO_MUX_ENET_TX);
    /* Initialize ENET_MDIO functionality on pin GPIO_57 (pin B13) */
    IO_MUX_SetPinMux(IO_MUX_ENET_MDIO);
    /* Initialize GPIO21 functionality on pin GPIO_21 (pin H4) */
    IO_MUX_SetPinMux(IO_MUX_GPIO21);
    /* Initialize GPIO55 functionality on pin GPIO_55 (pin K6) */
    IO_MUX_SetPinMux(IO_MUX_GPIO55);
    /* Set GPIO_25 (pin E7) configuration - Enable pull-up; weak slew rate */
    IO_MUX_SetPinConfig(25U, IO_MUX_PinConfigPullUpDriveWeak);
    /* Set GPIO_58 (pin G11_D12) configuration - Enable pull-up; weak slew rate */
    IO_MUX_SetPinConfig(58U, IO_MUX_PinConfigPullUpDriveWeak);
    /* Set GPIO_59 (pin G11_D12) configuration - Enable pull-up; weak slew rate */
    IO_MUX_SetPinConfig(59U, IO_MUX_PinConfigPullUpDriveWeak);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitUSBPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitUSBPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitUSBPins(void)
{
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitTEMP_SENSORPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: H2, peripheral: FLEXCOMM2, signal: I2C_SCL, pin_signal: GPIO_17}
  - {pin_num: J3, peripheral: FLEXCOMM2, signal: I2C_SDA, pin_signal: GPIO_16}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitTEMP_SENSORPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitTEMP_SENSORPins(void)
{
    /* Initialize FC2_I2C_16_17 functionality on pin GPIO_17, GPIO_16 (pin H2_J3) */
    IO_MUX_SetPinMux(IO_MUX_FC2_I2C_16_17);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSPIPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: M6, peripheral: FLEXCOMM1, signal: SPI_MISO, pin_signal: GPIO_8, pull_up_down: disable}
  - {pin_num: L9, peripheral: FLEXCOMM1, signal: SPI_SCK, pin_signal: GPIO_7, pull_up_down: disable}
  - {pin_num: M4, peripheral: FLEXCOMM1, signal: SPI_MOSI, pin_signal: GPIO_9, pull_up_down: disable}
  - {pin_num: L5, peripheral: FLEXCOMM1, signal: SPI_SSELN0, pin_signal: GPIO_6, pull_up_down: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSPIPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitSPIPins(void)
{
    /* Initialize FC1_SPI_SS0 functionality on pin GPIO_8, GPIO_7, GPIO_9, GPIO_6 (pin M6_L9_M4_L5) */
    IO_MUX_SetPinMux(IO_MUX_FC1_SPI_SS0);
    /* Set GPIO_6 (pin M6_L9_M4_L5) configuration - Disable pull-up / pull-down; strongest slew rate */
    IO_MUX_SetPinConfig(6U, IO_MUX_PinConfigNoPull);
    /* Set GPIO_7 (pin M6_L9_M4_L5) configuration - Disable pull-up / pull-down; strongest slew rate */
    IO_MUX_SetPinConfig(7U, IO_MUX_PinConfigNoPull);
    /* Set GPIO_8 (pin M6_L9_M4_L5) configuration - Disable pull-up / pull-down; strongest slew rate */
    IO_MUX_SetPinConfig(8U, IO_MUX_PinConfigNoPull);
    /* Set GPIO_9 (pin M6_L9_M4_L5) configuration - Disable pull-up / pull-down; strongest slew rate */
    IO_MUX_SetPinConfig(9U, IO_MUX_PinConfigNoPull);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitI2CPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: H2, peripheral: FLEXCOMM2, signal: I2C_SCL, pin_signal: GPIO_17}
  - {pin_num: J3, peripheral: FLEXCOMM2, signal: I2C_SDA, pin_signal: GPIO_16}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitI2CPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitI2CPins(void)
{
    /* Initialize FC2_I2C_16_17 functionality on pin GPIO_17, GPIO_16 (pin H2_J3) */
    IO_MUX_SetPinMux(IO_MUX_FC2_I2C_16_17);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitFLASHPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: E3, peripheral: FLEXSPI, signal: FLASH_CLK0, pin_signal: GPIO_34}
  - {pin_num: F4, peripheral: FLEXSPI, signal: FLASH_SSEL0, pin_signal: GPIO_28}
  - {pin_num: F2, peripheral: FLEXSPI, signal: FLASH_DATA2, pin_signal: GPIO_32}
  - {pin_num: G3, peripheral: FLEXSPI, signal: FLASH_DATA3, pin_signal: GPIO_33}
  - {pin_num: D2, peripheral: FLEXSPI, signal: FLASH_DQS, pin_signal: GPIO_29}
  - {pin_num: D3, peripheral: FLEXSPI, signal: FLASH_DATA1, pin_signal: GPIO_31}
  - {pin_num: F3, peripheral: FLEXSPI, signal: FLASH_DATA0, pin_signal: GPIO_30}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitFLASHPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitFLASHPins(void)
{
    /* Initialize QUAD_SPI_FLASH functionality on pin GPIO_34, GPIO_28, GPIO_32, GPIO_33, GPIO_29, GPIO_31, GPIO_30
     * (pin E3_F4_F2_G3_D2_D3_F3) */
    IO_MUX_SetPinMux(IO_MUX_QUAD_SPI_FLASH);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPSRAMPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: B3, peripheral: FLEXSPI, signal: SRAM_CLK0, pin_signal: GPIO_35}
  - {pin_num: B2, peripheral: FLEXSPI, signal: SRAM_DQS, pin_signal: GPIO_37, pull_up_down: down}
  - {pin_num: B1, peripheral: FLEXSPI, signal: SRAM_DATA3, pin_signal: GPIO_41}
  - {pin_num: A1, peripheral: FLEXSPI, signal: SRAM_DATA2, pin_signal: GPIO_40}
  - {pin_num: B4, peripheral: FLEXSPI, signal: SRAM_SSEL0, pin_signal: GPIO_36}
  - {pin_num: C3, peripheral: FLEXSPI, signal: SRAM_DATA0, pin_signal: GPIO_38}
  - {pin_num: A2, peripheral: FLEXSPI, signal: SRAM_DATA1, pin_signal: GPIO_39}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPSRAMPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitPSRAMPins(void)
{
    /* Initialize QUAD_SPI_PSRAM functionality on pin GPIO_35, GPIO_37, GPIO_41, GPIO_40, GPIO_36, GPIO_38, GPIO_39
     * (pin B3_B2_B1_A1_B4_C3_A2) */
    IO_MUX_SetPinMux(IO_MUX_QUAD_SPI_PSRAM);
    /* Set GPIO_37 (pin B3_B2_B1_A1_B4_C3_A2) configuration - Enable pull-down; strongest slew rate */
    IO_MUX_SetPinConfig(37U, IO_MUX_PinConfigPullDown);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKE15Z256xxx7
package_id: MKE15Z256VLL7
mcu_data: ksdk2_0
processor_version: 0.0.9
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '81', peripheral: LPUART1, signal: RX, pin_signal: ADC1_SE4/TSI0_CH15/PTC6/LPUART1_RX}
  - {pin_num: '80', peripheral: LPUART1, signal: TX, pin_signal: ADC1_SE5/TSI0_CH16/PTC7/LPUART1_TX}
  - {pin_num: '70', peripheral: ADC1, signal: 'SE, 3', pin_signal: ADC1_SE3/PTD3/LPSPI1_PCS0/FXIO_D5/TRGMUX_IN4/NMI_b}
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
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTC6 (pin 81) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt2);

    /* PORTC7 (pin 80) is configured as LPUART1_TX */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt2);

    /* PORTD3 (pin 70) is configured as ADC1_SE3 */
    PORT_SetPinMux(PORTD, 3U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '73', peripheral: LPI2C0, signal: SDA, pin_signal: ADC1_SE0/PTA2/LPI2C0_SDA/EWM_OUT_b/LPUART0_RX, drive_strength: low, pull_select: up, pull_enable: enable,
    passive_filter: disable}
  - {pin_num: '72', peripheral: LPI2C0, signal: SCL, pin_signal: ADC1_SE1/PTA3/LPI2C0_SCL/EWM_IN/LPUART0_TX, drive_strength: low, pull_select: up, pull_enable: enable,
    passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPI2C0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPI2C0_InitPins(void)
{
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);

    const port_pin_config_t porta2_pin73_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPI2C0_SDA */
                                                   kPORT_MuxAlt3,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA2 (pin 73) is configured as LPI2C0_SDA */
    PORT_SetPinConfig(PORTA, 2U, &porta2_pin73_config);

    const port_pin_config_t porta3_pin72_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPI2C0_SCL */
                                                   kPORT_MuxAlt3,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA3 (pin 72) is configured as LPI2C0_SCL */
    PORT_SetPinConfig(PORTA, 3U, &porta3_pin72_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C0_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '73', peripheral: ADC1, signal: 'SE, 0', pin_signal: ADC1_SE0/PTA2/LPI2C0_SDA/EWM_OUT_b/LPUART0_RX}
  - {pin_num: '72', peripheral: ADC1, signal: 'SE, 1', pin_signal: ADC1_SE1/PTA3/LPI2C0_SCL/EWM_IN/LPUART0_TX}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPI2C0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPI2C0_DeinitPins(void)
{
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA2 (pin 73) is configured as ADC1_SE0 */
    PORT_SetPinMux(PORTA, 2U, kPORT_PinDisabledOrAnalog);

    /* PORTA3 (pin 72) is configured as ADC1_SE1 */
    PORT_SetPinMux(PORTA, 3U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C1_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '42', peripheral: LPI2C1, signal: SDA, pin_signal: PTD8/LPI2C1_SDA/FTM2_FLT2, drive_strength: low, pull_select: up, pull_enable: enable, passive_filter: disable}
  - {pin_num: '41', peripheral: LPI2C1, signal: SCL, pin_signal: ACMP1_IN5/PTD9/LPI2C1_SCL/FTM2_FLT3, drive_strength: low, pull_select: up, pull_enable: enable, passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPI2C1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPI2C1_InitPins(void)
{
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    const port_pin_config_t portd8_pin42_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPI2C1_SDA */
                                                   kPORT_MuxAlt2,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTD8 (pin 42) is configured as LPI2C1_SDA */
    PORT_SetPinConfig(PORTD, 8U, &portd8_pin42_config);

    const port_pin_config_t portd9_pin41_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPI2C1_SCL */
                                                   kPORT_MuxAlt2,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTD9 (pin 41) is configured as LPI2C1_SCL */
    PORT_SetPinConfig(PORTD, 9U, &portd9_pin41_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C1_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '42', peripheral: n/a, signal: disabled, pin_signal: PTD8/LPI2C1_SDA/FTM2_FLT2}
  - {pin_num: '41', peripheral: CMP1, signal: 'IN, 5', pin_signal: ACMP1_IN5/PTD9/LPI2C1_SCL/FTM2_FLT3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPI2C1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPI2C1_DeinitPins(void)
{
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD8 (pin 42) is disabled */
    PORT_SetPinMux(PORTD, 8U, kPORT_PinDisabledOrAnalog);

    /* PORTD9 (pin 41) is configured as ACMP1_IN5 */
    PORT_SetPinMux(PORTD, 9U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

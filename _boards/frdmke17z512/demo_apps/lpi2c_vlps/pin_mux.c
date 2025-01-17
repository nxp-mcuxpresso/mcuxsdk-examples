/*
 * Copyright 2023 NXP
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
product: Pins v13.1
processor: MKE17Z512xxx9
package_id: MKE17Z512VLL9
mcu_data: ksdk2_0
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
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
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: cm0plus, enableClock: 'true'}
- pin_list:
  - {pin_num: '20', peripheral: LPUART2, signal: RX, pin_signal: PTD17/FTM0_FLT2/LPUART2_RX/TRGMUX_OUT2}
  - {pin_num: '19', peripheral: LPUART2, signal: TX, pin_signal: PTE12/FTM0_FLT3/LPUART2_TX/TRGMUX_OUT3}
  - {pin_num: '77', peripheral: LPI2C0, signal: SCL, pin_signal: PTB8/LPI2C0_SCL/FXIO_D4}
  - {pin_num: '82', peripheral: LPI2C0, signal: SDA, pin_signal: PTA16/FTM1_CH3/LPSPI1_PCS2/LPI2C0_SDA}
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
    /* Clock Gate Control: Enables the clock. The current clock selection and divider options are locked and cannot be modified. */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Gate Control: Enables the clock. The current clock selection and divider options are locked and cannot be modified. */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Gate Control: Enables the clock. The current clock selection and divider options are locked and cannot be modified. */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Gate Control: Enables the clock. The current clock selection and divider options are locked and cannot be modified. */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA16 (pin 82) is configured as LPI2C0_SDA */
    PORT_SetPinMux(PORTA, 16U, kPORT_MuxAlt4);

    /* PORTB8 (pin 77) is configured as LPI2C0_SCL */
    PORT_SetPinMux(PORTB, 8U, kPORT_MuxAlt4);

    /* PORTD17 (pin 20) is configured as LPUART2_RX */
    PORT_SetPinMux(PORTD, 17U, kPORT_MuxAlt3);

    /* PORTE12 (pin 19) is configured as LPUART2_TX */
    PORT_SetPinMux(PORTE, 12U, kPORT_MuxAlt3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

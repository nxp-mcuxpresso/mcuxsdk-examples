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
- options: {callFromInitBoot: 'true', coreID: cm0plus, enableClock: 'true'}
- pin_list:
  - {pin_num: '20', peripheral: LPUART2, signal: RX, pin_signal: PTD17/FTM0_FLT2/LPUART2_RX/TRGMUX_OUT2}
  - {pin_num: '19', peripheral: LPUART2, signal: TX, pin_signal: PTE12/FTM0_FLT3/LPUART2_TX/TRGMUX_OUT3}
  - {pin_num: '100', peripheral: TRGMUX0, signal: 'OUT, 4', pin_signal: PTA8/FXIO_D6/TRGMUX_OUT0}
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
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Gate Control: Enables the clock. The current clock selection and divider options are locked and cannot be modified. */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA8 (pin 100) is configured as TRGMUX_OUT0 */
    PORT_SetPinMux(PORTA, 8U, kPORT_MuxAlt7);

    /* PORTD17 (pin 20) is configured as LPUART2_RX */
    PORT_SetPinMux(PORTD, 17U, kPORT_MuxAlt3);

    /* PORTE12 (pin 19) is configured as LPUART2_TX */
    PORT_SetPinMux(PORTE, 12U, kPORT_MuxAlt3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

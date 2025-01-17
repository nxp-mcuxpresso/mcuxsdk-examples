/*
 * Copyright 2020 NXP
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
product: Pins v8.0
processor: LPC54S018
package_id: LPC54S018JET180
mcu_data: ksdk2_0
processor_version: 0.0.2
board: LPCXpresso54S018
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_iocon.h"
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
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A2, peripheral: FLEXCOMM0, signal: TXD_SCL_MISO, pin_signal: PIO0_30/FC0_TXD_SCL_MISO/CTIMER0_MAT0/SCT0_OUT9/TRACEDATA(1), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: B13, peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI, pin_signal: PIO0_29/FC0_RXD_SDA_MOSI/CTIMER2_MAT3/SCT0_OUT8/TRACEDATA(2), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: M6, peripheral: CAN0, signal: CAN_TD, pin_signal: PIO3_18/FC8_CTS_SDA_SSEL0/SD_D(6)/CTIMER4_MAT0/CAN0_TD/SCT0_OUT5, mode: pullUp, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: J3, peripheral: CAN0, signal: CAN_RD, pin_signal: PIO3_19/FC8_RTS_SCL_SSEL1/SD_D(7)/CTIMER4_MAT1/CAN0_RD/SCT0_OUT6, mode: pullUp, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M4F */
void BOARD_InitPins(void)
{
    /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t ISP_FC0_RXD = (/* Pin is configured as FC0_RXD_SDA_MOSI */
                                  IOCON_PIO_FUNC1 |
                                  /* No addition pin function */
                                  IOCON_PIO_MODE_INACT |
                                  /* Input function is not inverted */
                                  IOCON_PIO_INV_DI |
                                  /* Enables digital function */
                                  IOCON_PIO_DIGITAL_EN |
                                  /* Input filter disabled */
                                  IOCON_PIO_INPFILT_OFF |
                                  /* Standard mode, output slew rate control is enabled */
                                  IOCON_PIO_SLEW_STANDARD |
                                  /* Open drain is disabled */
                                  IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN29 (coords: B13) is configured as FC0_RXD_SDA_MOSI */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_ISP_FC0_RXD_PORT, BOARD_INITPINS_ISP_FC0_RXD_PIN, ISP_FC0_RXD);

    const uint32_t ISP_FC0_TXD = (/* Pin is configured as FC0_TXD_SCL_MISO */
                                  IOCON_PIO_FUNC1 |
                                  /* No addition pin function */
                                  IOCON_PIO_MODE_INACT |
                                  /* Input function is not inverted */
                                  IOCON_PIO_INV_DI |
                                  /* Enables digital function */
                                  IOCON_PIO_DIGITAL_EN |
                                  /* Input filter disabled */
                                  IOCON_PIO_INPFILT_OFF |
                                  /* Standard mode, output slew rate control is enabled */
                                  IOCON_PIO_SLEW_STANDARD |
                                  /* Open drain is disabled */
                                  IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN30 (coords: A2) is configured as FC0_TXD_SCL_MISO */
    IOCON_PinMuxSet(IOCON, BOARD_INITPINS_ISP_FC0_TXD_PORT, BOARD_INITPINS_ISP_FC0_TXD_PIN, ISP_FC0_TXD);

    const uint32_t port3_pin18_config = (/* Pin is configured as CAN0_TD */
                                         IOCON_PIO_FUNC4 |
                                         /* Selects pull-up function */
                                         IOCON_PIO_MODE_PULLUP |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Input filter disabled */
                                         IOCON_PIO_INPFILT_OFF |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT3 PIN18 (coords: M6) is configured as CAN0_TD */
    IOCON_PinMuxSet(IOCON, 3U, 18U, port3_pin18_config);

    const uint32_t port3_pin19_config = (/* Pin is configured as CAN0_RD */
                                         IOCON_PIO_FUNC4 |
                                         /* Selects pull-up function */
                                         IOCON_PIO_MODE_PULLUP |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Input filter disabled */
                                         IOCON_PIO_INPFILT_OFF |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT3 PIN19 (coords: J3) is configured as CAN0_RD */
    IOCON_PinMuxSet(IOCON, 3U, 19U, port3_pin19_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

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
product: Pins v12.0
processor: MIMXRT595S
package_id: MIMXRT595SFFOC
mcu_data: ksdk2_0
processor_version: 12.0.1
board: MIMXRT595-EVK
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_iopctl.h"
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
- pin_list:
  - {pin_num: H16, peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_2/FC0_RXD_SDA_MOSI_DATA/CTIMER0_MAT2/I2S_BRIDGE_DATA_IN/SEC_PIO0_2, ibena: enabled}
  - {pin_num: G16, peripheral: FLEXCOMM0, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_1/FC0_TXD_SCL_MISO_WS/CTIMER0_MAT1/I2S_BRIDGE_WS_IN/SEC_PIO0_1}
  - {pin_num: K16, peripheral: GPIO, signal: 'PIO0, 10', pin_signal: PIO0_10/FC1_CTS_SDA_SSEL0/SCT0_GPI7/SCT0_OUT7/CTIMER1_MAT3/FC0_SSEL2/SEC_PIO0_10, ibena: enabled}
  - {pin_num: K6, peripheral: FLEXCOMM15, signal: SDA, pin_signal: PMIC_I2C_SDA, ibena: enabled, odena: enabled}
  - {pin_num: K4, peripheral: FLEXCOMM15, signal: SCL, pin_signal: PMIC_I2C_SCL, ibena: enabled, odena: enabled}
  - {pin_num: N3, peripheral: SWD, signal: SWO, pin_signal: PIO2_24/SWO/GPIO_INT_BMAT/SMARTDMA_PIO24, pupdena: enabled, pupdsel: pullUp}
  - {pin_num: C12, peripheral: GPIO, signal: 'PIO0, 25', pin_signal: PIO0_25/FC3_RTS_SCL_SSEL1/FREQME_GPIO_CLK/CTIMER_INP6/FC2_SSEL3/TRACEDATA_3/CLKIN/SEC_PIO0_25,
    ibena: enabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitPins(void)
{

    const uint32_t fc15_i2c_scl_config = (/* Pin is configured as I2C_SCL */
                                          IOPCTL_PIO_FUNC0 |
                                          /* Disable pull-up / pull-down function */
                                          IOPCTL_PIO_PUPD_DI |
                                          /* Enable pull-down function */
                                          IOPCTL_PIO_PULLDOWN_EN |
                                          /* Enables input buffer function */
                                          IOPCTL_PIO_INBUF_EN |
                                          /* Normal mode */
                                          IOPCTL_PIO_SLEW_RATE_NORMAL |
                                          /* Normal drive */
                                          IOPCTL_PIO_FULLDRIVE_DI |
                                          /* Analog mux is disabled */
                                          IOPCTL_PIO_ANAMUX_DI |
                                          /* Pseudo Output Drain is enabled */
                                          IOPCTL_PIO_PSEDRAIN_EN |
                                          /* Input function is not inverted */
                                          IOPCTL_PIO_INV_DI);
    /* FC15_SCL PIN (coords: K4) is configured as I2C SCL */
    IOPCTL->FC15_I2C_SCL = fc15_i2c_scl_config;

    const uint32_t fc15_i2c_sda_config = (/* Pin is configured as I2C_SDA */
                                          IOPCTL_PIO_FUNC0 |
                                          /* Disable pull-up / pull-down function */
                                          IOPCTL_PIO_PUPD_DI |
                                          /* Enable pull-down function */
                                          IOPCTL_PIO_PULLDOWN_EN |
                                          /* Enables input buffer function */
                                          IOPCTL_PIO_INBUF_EN |
                                          /* Normal mode */
                                          IOPCTL_PIO_SLEW_RATE_NORMAL |
                                          /* Normal drive */
                                          IOPCTL_PIO_FULLDRIVE_DI |
                                          /* Analog mux is disabled */
                                          IOPCTL_PIO_ANAMUX_DI |
                                          /* Pseudo Output Drain is enabled */
                                          IOPCTL_PIO_PSEDRAIN_EN |
                                          /* Input function is not inverted */
                                          IOPCTL_PIO_INV_DI);
    /* FC15_SDA PIN (coords: K6) is configured as I2C SDA */
    IOPCTL->FC15_I2C_SDA = fc15_i2c_sda_config;

    const uint32_t port0_pin1_config = (/* Pin is configured as FC0_TXD_SCL_MISO_WS */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Disable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_DI |
                                        /* Enable pull-down function */
                                        IOPCTL_PIO_PULLDOWN_EN |
                                        /* Disable input buffer function */
                                        IOPCTL_PIO_INBUF_DI |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Normal drive */
                                        IOPCTL_PIO_FULLDRIVE_DI |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT0 PIN1 (coords: G16) is configured as FC0_TXD_SCL_MISO_WS */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 1U, port0_pin1_config);

    const uint32_t SW2 = (/* Pin is configured as PIO0_10 */
                          IOPCTL_PIO_FUNC0 |
                          /* Disable pull-up / pull-down function */
                          IOPCTL_PIO_PUPD_DI |
                          /* Enable pull-down function */
                          IOPCTL_PIO_PULLDOWN_EN |
                          /* Enables input buffer function */
                          IOPCTL_PIO_INBUF_EN |
                          /* Normal mode */
                          IOPCTL_PIO_SLEW_RATE_NORMAL |
                          /* Normal drive */
                          IOPCTL_PIO_FULLDRIVE_DI |
                          /* Analog mux is disabled */
                          IOPCTL_PIO_ANAMUX_DI |
                          /* Pseudo Output Drain is disabled */
                          IOPCTL_PIO_PSEDRAIN_DI |
                          /* Input function is not inverted */
                          IOPCTL_PIO_INV_DI);
    /* PORT0 PIN10 (coords: K16) is configured as PIO0_10 */
    IOPCTL_PinMuxSet(IOPCTL, BOARD_INITPINS_SW2_PORT, BOARD_INITPINS_SW2_PIN, SW2);

    const uint32_t port0_pin2_config = (/* Pin is configured as FC0_RXD_SDA_MOSI_DATA */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Disable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_DI |
                                        /* Enable pull-down function */
                                        IOPCTL_PIO_PULLDOWN_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Normal drive */
                                        IOPCTL_PIO_FULLDRIVE_DI |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT0 PIN2 (coords: H16) is configured as FC0_RXD_SDA_MOSI_DATA */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 2U, port0_pin2_config);

    const uint32_t SW1 = (/* Pin is configured as PIO0_25 */
                          IOPCTL_PIO_FUNC0 |
                          /* Disable pull-up / pull-down function */
                          IOPCTL_PIO_PUPD_DI |
                          /* Enable pull-down function */
                          IOPCTL_PIO_PULLDOWN_EN |
                          /* Enables input buffer function */
                          IOPCTL_PIO_INBUF_EN |
                          /* Normal mode */
                          IOPCTL_PIO_SLEW_RATE_NORMAL |
                          /* Normal drive */
                          IOPCTL_PIO_FULLDRIVE_DI |
                          /* Analog mux is disabled */
                          IOPCTL_PIO_ANAMUX_DI |
                          /* Pseudo Output Drain is disabled */
                          IOPCTL_PIO_PSEDRAIN_DI |
                          /* Input function is not inverted */
                          IOPCTL_PIO_INV_DI);
    /* PORT0 PIN25 (coords: C12) is configured as PIO0_25 */
    IOPCTL_PinMuxSet(IOPCTL, BOARD_INITPINS_SW1_PORT, BOARD_INITPINS_SW1_PIN, SW1);

    const uint32_t port2_pin24_config = (/* Pin is configured as SWO */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Enable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_EN |
                                         /* Enable pull-up function */
                                         IOPCTL_PIO_PULLUP_EN |
                                         /* Disable input buffer function */
                                         IOPCTL_PIO_INBUF_DI |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT2 PIN24 (coords: N3) is configured as SWO */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 24U, port2_pin24_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

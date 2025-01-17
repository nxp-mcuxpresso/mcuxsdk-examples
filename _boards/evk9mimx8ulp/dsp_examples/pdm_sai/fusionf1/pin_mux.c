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

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v13.1
processor: MIMX8UD5xxx08
package_id: MIMX8UD5DVK08
mcu_data: ksdk2_0
processor_version: 0.13.4
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
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

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33}
- pin_list:
  - {pin_num: D2, peripheral: LPUART1, signal: lpuart_tx, pin_signal: PTA10, PS: UP, PE: ENABLED}
  - {pin_num: C1, peripheral: LPUART1, signal: lpuart_rx, pin_signal: PTA11, PS: UP, PE: ENABLED}
  - {pin_num: A9, peripheral: MICFIL0, signal: 'micfil_clk, 01', pin_signal: PTB0}
  - {pin_num: B10, peripheral: MICFIL0, signal: 'micfil_data, 01', pin_signal: PTB1, PS: UP, PE: ENABLED}
  - {pin_num: A11, peripheral: MICFIL0, signal: 'micfil_data, 23', pin_signal: PTB3, PS: UP, PE: ENABLED}
  - {pin_num: G1, peripheral: I2S0, signal: i2s_rx_bclk, pin_signal: PTA0, DSE: HIDRIVE}
  - {pin_num: H4, peripheral: I2S0, signal: i2s_rx_fs, pin_signal: PTA1, DSE: HIDRIVE}
  - {pin_num: H2, peripheral: I2S0, signal: 'i2s_rxd, 0', pin_signal: PTA2}
  - {pin_num: F4, peripheral: I2S0, signal: i2s_mclk, pin_signal: PTA4, DSE: HIDRIVE, OBE: ENABLED}
  - {pin_num: E3, peripheral: I2S0, signal: 'i2s_txd, 0', pin_signal: PTA7, DSE: HIDRIVE}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-M33[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PTA0_I2S0_RX_BCLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA0_I2S0_RX_BCLK,
                        IOMUXC_PCR_DSE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA1_I2S0_RX_FS, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA1_I2S0_RX_FS,
                        IOMUXC_PCR_DSE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA10_LPUART1_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA10_LPUART1_TX,
                        IOMUXC_PCR_PE_MASK |
                        IOMUXC_PCR_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA11_LPUART1_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA11_LPUART1_RX,
                        IOMUXC_PCR_PE_MASK |
                        IOMUXC_PCR_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA2_I2S0_RXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTA4_I2S0_MCLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA4_I2S0_MCLK,
                        IOMUXC_PCR_OBE_MASK |
                        IOMUXC_PCR_DSE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTA7_I2S0_TXD0, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTA7_I2S0_TXD0,
                        IOMUXC_PCR_DSE_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB0_MICFIL0_CLK01, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTB1_MICFIL0_DATA01, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB1_MICFIL0_DATA01,
                        IOMUXC_PCR_PE_MASK |
                        IOMUXC_PCR_PS_MASK);
    IOMUXC_SetPinMux(IOMUXC_PTB3_MICFIL0_DATA23, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB3_MICFIL0_DATA23,
                        IOMUXC_PCR_PE_MASK |
                        IOMUXC_PCR_PS_MASK);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

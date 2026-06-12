/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Unified pin mux configuration for imx943evk board.
 * Each function configures pins for a specific peripheral.
 * Demos should call only the functions they need.
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_xbar.h"
#include "pin_mux.h"

/*******************************************************************************
 * I2C Pin Configuration
 ******************************************************************************/

void BOARD_InitI2C3Pins(void) {
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO16__LPI2C3_SDA, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO17__LPI2C3_SCL, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO16__LPI2C3_SDA, 
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO17__LPI2C3_SCL, 
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
}

void BOARD_InitI2C4Pins(void) 
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO19__LPI2C4_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO18__LPI2C4_SDA, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO19__LPI2C4_SCL,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO18__LPI2C4_SDA,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
}

void BOARD_InitI3c2Pins(void) {
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO01__I3C2_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO02__I3C2_SDA, 1U);
}

/*******************************************************************************
 * SPI Pin Configuration
 ******************************************************************************/

void BOARD_InitLPSPI3Pins(void) {
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO08__LPSPI3_PCS0, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO08__LPSPI3_PCS0,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(3U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO09__LPSPI3_SIN, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO09__LPSPI3_SIN,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(3U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO10__LPSPI3_SOUT, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO10__LPSPI3_SOUT,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(3U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO11__LPSPI3_SCK, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO11__LPSPI3_SCK,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(3U) |
                        IOMUXC_PAD_PU_MASK);
}

void BOARD_InitLPSPI8Pins(void) {
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO12__LPSPI8_PCS0, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO12__LPSPI8_PCS0,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(3U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO13__LPSPI8_SIN, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO13__LPSPI8_SIN,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(3U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO14__LPSPI8_SOUT, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO14__LPSPI8_SOUT,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(3U) |
                        IOMUXC_PAD_PU_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO15__LPSPI8_SCK, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO15__LPSPI8_SCK,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(3U) |
                        IOMUXC_PAD_PU_MASK);
}

void BOARD_InitSincLpspi3Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO08__LPSPI3_PCS0, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO08__LPSPI3_PCS0,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO09__LPSPI3_SIN, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO09__LPSPI3_SIN,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO10__LPSPI3_SOUT, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO10__LPSPI3_SOUT,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO11__LPSPI3_SCK, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO11__LPSPI3_SCK,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);
}

/*******************************************************************************
 * SAI/Audio Pin Configuration
 ******************************************************************************/

void BOARD_InitSAI1Pins(void) {
    IOMUXC_SetPinMux(IOMUXC_PAD_SAI1_TXC__SAI1_TX_BCLK, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_I2C2_SDA__SAI1_MCLK, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_SAI1_RXD0__SAI1_RX_DATA0, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_SAI1_TXD0__SAI1_TX_DATA0, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_SAI1_TXFS__SAI1_TX_SYNC, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_SAI1_TXC__SAI1_TX_BCLK,
                        IOMUXC_PAD_DSE(7U));
    IOMUXC_SetPinConfig(IOMUXC_PAD_I2C2_SDA__SAI1_MCLK,
                        IOMUXC_PAD_DSE(7U));
    IOMUXC_SetPinConfig(IOMUXC_PAD_SAI1_RXD0__SAI1_RX_DATA0,
                         IOMUXC_PAD_DSE(7U));
    IOMUXC_SetPinConfig(IOMUXC_PAD_SAI1_TXD0__SAI1_TX_DATA0,
                        IOMUXC_PAD_DSE(7U));
    IOMUXC_SetPinConfig(IOMUXC_PAD_SAI1_TXFS__SAI1_TX_SYNC,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitPDMPins(void)  {
    IOMUXC_SetPinMux(IOMUXC_PAD_PDM_BIT_STREAM0__PDM_BIT_STREAM0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_PDM_BIT_STREAM1__PDM_BIT_STREAM1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_PDM_CLK__PDM_CLK, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_PDM_BIT_STREAM0__PDM_BIT_STREAM0,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(3U));
    IOMUXC_SetPinConfig(IOMUXC_PAD_PDM_BIT_STREAM1__PDM_BIT_STREAM1,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(3U));
    IOMUXC_SetPinConfig(IOMUXC_PAD_PDM_CLK__PDM_CLK,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(3U));   
}

/*******************************************************************************
 * CAN Pin Configuration
 ******************************************************************************/

void BOARD_InitCAN1Pins(void) {
    IOMUXC_SetPinMux(IOMUXC_PAD_PDM_CLK__CAN1_TX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_PDM_BIT_STREAM0__CAN1_RX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_PDM_CLK__CAN1_TX,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_PDM_BIT_STREAM0__CAN1_RX,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

}

/*******************************************************************************
 * GPIO Pin Configuration
 ******************************************************************************/

void BOARD_InitGPIOPins(void) {

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO31__GPIO2_IO31, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO31__GPIO2_IO31,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);
}

/*******************************************************************************
 * EtherCAT Pin Configuration
 ******************************************************************************/

void BOARD_InitEcatResetPins(void) 
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_TX_CTL__ECAT_RESET_OUT, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_TX_CTL__ECAT_RESET_OUT,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(3));
}

void BOARD_InitEcatLinkPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_CRS__ECAT_LINK0, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_COL__ECAT_LINK1, 0);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_CRS__ECAT_LINK0,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(3));
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_COL__ECAT_LINK1,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(3));

}

void BOARD_InitEcatMDIOPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_MDIO_GPIO2__ECAT_MDIO, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_MDC_GPIO1__ECAT_MDC, 0U);  
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_MDIO_GPIO2__ECAT_MDIO,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(3));
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_MDC_GPIO1__ECAT_MDC,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(3));            
}

void BOARD_InitEcatCLKPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_SD2_GPIO1__ECAT_CLK25, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_TXD2__ECAT_CLK25, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_SD2_GPIO1__ECAT_CLK25,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(3));     
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_TXD2__ECAT_CLK25,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(3));               

}

void BOARD_InitEcatI2CPins(void)
{

/* Using GPIO_IO14/GPIO_IO15 */
    // IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO14__ECAT_SDA, 0U);
    // IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO15__ECAT_SCL, 0U);
    // IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO14__ECAT_SDA,
    //                     IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_OD_MASK | IOMUXC_PAD_FSEL1(3));
    // IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO15__ECAT_SCL,
    //                     IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_OD_MASK | IOMUXC_PAD_FSEL1(3));

#if 1

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO34__ECAT_SDA, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO35__ECAT_SCL, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO34__ECAT_SDA,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_OD_MASK | IOMUXC_PAD_FSEL1(3));
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO35__ECAT_SCL,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_OD_MASK | IOMUXC_PAD_FSEL1(3));
#endif
}

void BOARD_InitEcatPortRmiiPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD0__ECAT_PT1_TXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD1__ECAT_PT1_TXD1, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TX_EN__ECAT_PT1_TX_EN, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TX_CLK__ECAT_PT1_TX_CLK, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD0__ECAT_PT1_RXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD1__ECAT_PT1_RXD1, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RX_DV__ECAT_PT1_RX_DV, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RX_ER__ECAT_PT1_RX_ER, 0);

    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD0__ECAT_PT0_TXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD1__ECAT_PT0_TXD1, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TX_EN__ECAT_PT0_TX_EN, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TX_CLK__ECAT_PT0_TX_CLK, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD0__ECAT_PT0_RXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD1__ECAT_PT0_RXD1, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RX_DV__ECAT_PT0_RX_DV, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RX_ER__ECAT_PT0_RX_ER, 0);
}

void BOARD_InitEcatPortMiiPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD0__ECAT_PT1_TXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD1__ECAT_PT1_TXD1, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD2__ECAT_PT1_TXD2, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD3__ECAT_PT1_TXD3, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TX_EN__ECAT_PT1_TX_EN, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TX_CLK__ECAT_PT1_TX_CLK, 0);

    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD0__ECAT_PT1_RXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD1__ECAT_PT1_RXD1, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD2__ECAT_PT1_RXD2, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD3__ECAT_PT1_RXD3, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RX_DV__ECAT_PT1_RX_DV, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RX_ER__ECAT_PT1_RX_ER, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RX_CLK__ECAT_PT1_RX_CLK,0);

    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD0__ECAT_PT0_TXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD1__ECAT_PT0_TXD1, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD2__ECAT_PT0_TXD2, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD3__ECAT_PT0_TXD3, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TX_EN__ECAT_PT0_TX_EN, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TX_CLK__ECAT_PT0_TX_CLK, 0);

    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD0__ECAT_PT0_RXD0, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD1__ECAT_PT0_RXD1, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD2__ECAT_PT0_RXD2, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD3__ECAT_PT0_RXD3, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RX_DV__ECAT_PT0_RX_DV, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RX_ER__ECAT_PT0_RX_ER, 0);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RX_CLK__ECAT_PT0_RX_CLK,0);
}

/*******************************************************************************
 * XSPI/Flash Pin Configuration
 ******************************************************************************/

void BOARD_InitXspiPins(void) {
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_SCLK__XSPI1_A_SCLK, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_SCLK__XSPI1_A_SCLK,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_SS0_B__XSPI1_A_SS0_B, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_SS0_B__XSPI1_A_SS0_B,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_SS1_B__XSPI1_A_SS1_B, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_SS1_B__XSPI1_A_SS1_B,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_DQS__XSPI1_A_DQS, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_DQS__XSPI1_A_DQS,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_DATA0__XSPI1_A_DATA0, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_DATA0__XSPI1_A_DATA0,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_DATA1__XSPI1_A_DATA1, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_DATA1__XSPI1_A_DATA1,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_DATA2__XSPI1_A_DATA2, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_DATA2__XSPI1_A_DATA2,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_DATA3__XSPI1_A_DATA3, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_DATA3__XSPI1_A_DATA3,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_DATA4__XSPI1_A_DATA4, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_DATA4__XSPI1_A_DATA4,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_DATA5__XSPI1_A_DATA5, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_DATA5__XSPI1_A_DATA5,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_DATA6__XSPI1_A_DATA6, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_DATA6__XSPI1_A_DATA6,
                        IOMUXC_PAD_DSE(63U));
    IOMUXC_SetPinMux(IOMUXC_PAD_XSPI1_DATA7__XSPI1_A_DATA7, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_XSPI1_DATA7__XSPI1_A_DATA7,
                        IOMUXC_PAD_DSE(63U));
}

/*******************************************************************************
 * Encoder Pin Configuration
 ******************************************************************************/

void BOARD_InitEncoder1Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD0__DIG_ENCODER1_DATA_EN, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD1__DIG_ENCODER1_DATA_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD2__DIG_ENCODER1_DATA_OUT, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD3__DIG_ENCODER1_DATA_IN, 0U);
}

void BOARD_InitEncoder2Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD0__DIG_ENCODER2_DATA_EN, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD1__DIG_ENCODER2_DATA_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD2__DIG_ENCODER2_DATA_OUT ,0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD3__DIG_ENCODER2_DATA_IN ,0U);
}



/*******************************************************************************
 * Motor Control Pin Configuration
 ******************************************************************************/

void BOARD_Init_M1_PWM(void)
{
  /* M1_PWM_AT	ETH2_TXD3 / SEMC_ADDR7 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TXD3__FLEXPWM2_PWMA0, 0);
  /* M1_PWM_AB	ETH2_TXD2 / USB2.OTG_OC / SEMC_ADDR6 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TXD2__FLEXPWM2_PWMB0, 0);
  /* M1_PWM_BT	ETH2_TXD1 / USB1.OTG_OC / SEMC_ADDR5 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TXD1__FLEXPWM2_PWMA1, 0);
  /* M1_PWM_BB	ETH2_TXD0 / SEMC_ADDR4 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TXD0__FLEXPWM2_PWMB1, 0);
  /* M1_PWM_CT	ETH2_TXCTL / SEMC_DATA16 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TX_CTL__FLEXPWM2_PWMA2, 0);
  /* M1_PWM_CB	ETH2_TXCLK / SEMC_DATA17 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TX_CLK__FLEXPWM2_PWMB2, 0);

}

void BOARD_Init_M1_FAULTS(void)
{
  /* Over-currrent protection */
  BLK_CTRL_WAKEUPMIX->IOMUX_GPIO_CTRL_0 = ((BLK_CTRL_WAKEUPMIX->IOMUX_GPIO_CTRL_0 &
    (~(BLK_CTRL_WAKEUPMIX_IOMUX_GPIO_CTRL_0_sel5_MASK | BLK_CTRL_WAKEUPMIX_IOMUX_GPIO_CTRL_0_ovr5_MASK))) /* Mask bits to zero which are setting */
      | BLK_CTRL_WAKEUPMIX_IOMUX_GPIO_CTRL_0_sel5(0x01U)        /* MUX8 selected */ 
      | BLK_CTRL_WAKEUPMIX_IOMUX_GPIO_CTRL_0_ovr5(0x00U)        /* input direction */
  );   

  IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO57__GPIO3_IO25, 0U);

  XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn21, kXBAR1_OutputFlexpwm2IppIndFault0);
  
  /* Over-votlage protection */
  /* Set XBAR_INOUT20 as input*/
  BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL1 = ((BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL1 &
    (~(BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_20_MASK))) /* Mask bits to zero which are setting */
      | BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_20(0x00U) /* IOMUXC XBAR_INOUT20 function direction select: XBAR_INOUT as input */ 
  );
  
  IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO56__XBAR1_XBAR_INOUT20, 0U);
  
  XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn20, kXBAR1_OutputFlexpwm2IppIndFault1);
}

void BOARD_Init_M1_Encoder(void)
{
  /* Init Encoder 1 routed to Motor controller 1 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD0__DIG_ENCODER2_DATA_EN, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD1__DIG_ENCODER2_DATA_CLK, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD2__DIG_ENCODER2_DATA_OUT ,0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD3__DIG_ENCODER2_DATA_IN ,0U);
}

void BOARD_Init_M1_SINC(void)
{
  /* M1_SINCA_D0	SD1_CMD */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD1_CMD__SINC1_EMBIT0, 0);
  /* M1_SINCA_D1	SD1_DATA1 */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD1_DATA1__SINC1_EMBIT1, 0);
  /* M1_SINCA_D2	SD1_DATA3 */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD1_DATA3__SINC1_EMBIT2, 0);
  /* M1_SINCA_D3	SD1_DATA5 */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD1_DATA5__SINC1_EMBIT3, 0);
  /* M1_SINCA_CLK0	SD1_CLK */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD1_DATA6__SINC1_MOD_CLK0, 0);
}

void BOARD_Init_M1_ENDAT2P2(void)
{
  /* Init Encoder 1 routed to Motor controller 1 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD0__DIG_ENCODER2_DATA_EN, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD1__DIG_ENCODER2_DATA_CLK, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD2__DIG_ENCODER2_DATA_OUT ,0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD3__DIG_ENCODER2_DATA_IN ,0U);
}

void BOARD_Init_M2_PWM(void)
{
  /* M2_PWM_AT	ETH3_TXD3 / SEMC_DATA8 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TXD3__FLEXPWM1_PWMA0, 0);
  /* M2_PWM_AB	ETH3_TXD2 / SEMC_DATA9 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TXD2__FLEXPWM1_PWMB0, 0);
  /* M2_PWM_BT	ETH3_TXD1 / SEMC_DATA10 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TXD1__FLEXPWM1_PWMA1, 0);
  /* M2_PWM_BB	ETH3_TXD0 / SEMC_DATA11 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TXD0__FLEXPWM1_PWMB1, 0);
  /* M2_PWM_CT	ETH3_TXCTL / SEMC_ADDR12 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TX_CTL__FLEXPWM1_PWMA2, 0);
  /* M2_PWM_CB	ETH3_TXCLK / SEMC_ADDR11 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TX_CLK__FLEXPWM1_PWMB2, 0);

}

void BOARD_Init_M2_FAULTS(void)
{
  /* Over-currrent protection */
  /* Set XBAR_INOUT9 as input*/
  BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL1 = ((BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL1 &
    (~(BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
      | BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_9(0x00U) /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */ 
  );
  
  IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO33__XBAR1_XBAR_INOUT9, 0U); 
  
  XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn09, kXBAR1_OutputFlexpwm1IppIndFault0);

  
  /* Over-votlage protection */
  /* Set XBAR_INOUT8 as input*/
  BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL1 = ((BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL1 &
    (~(BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
      | BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_8(0x00U) /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */ 
  );
  
  IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO32__XBAR1_XBAR_INOUT8, 0U); 
  
  XBAR_SetSignalsConnection(kXBAR1_InputIomuxXbarIn08, kXBAR1_OutputFlexpwm1IppIndFault1);
}

void BOARD_Init_M2_Encoder(void)
{
  /* Init Encoder 2 routed to Motor controller 2 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD0__DIG_ENCODER1_DATA_EN, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD1__DIG_ENCODER1_DATA_CLK, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD2__DIG_ENCODER1_DATA_OUT, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD3__DIG_ENCODER1_DATA_IN, 0U);
}

void BOARD_Init_M2_SINC(void)
{
  /* M2_SINCA_D0	SD2_CLK */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD2_CLK__SINC2_EMBIT0, 0);
  /* M2_SINCA_D1	SD2_DATA0 */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD2_DATA0__SINC2_EMBIT1, 0);
  /* M2_SINCA_D2	SD2_DATA2 */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD2_DATA2__SINC2_EMBIT2, 0);
  /* M2_SINCA_D3	SD2_RESET_B */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD2_RESET_B__SINC2_EMBIT3, 0);
  /* M2_SINCA_CLK0	SD2_CD_B */
  IOMUXC_SetPinMux(IOMUXC_PAD_SD2_GPIO0__SINC2_MOD_CLK1, 0);
}

void BOARD_Init_M2_ENDAT2P2(void)
{
  /* Init Encoder 2 routed to Motor controller 2 */
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD0__DIG_ENCODER1_DATA_EN, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD1__DIG_ENCODER1_DATA_CLK, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD2__DIG_ENCODER1_DATA_OUT, 0U);
  IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD3__DIG_ENCODER1_DATA_IN, 0U);
}

/*******************************************************************************
 * PWM Pin Configuration
 ******************************************************************************/

void BOARD_InitPWM1Pins(void)
{
    IOMUXC_SetPinMux(
      IOMUXC_PAD_ETH3_TXD3__FLEXPWM1_PWMA0,      /* GPIO_AD_00 is configured as FLEXPWM1_PWMA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_PAD_ETH3_TXD1__FLEXPWM1_PWMA1,      /* GPIO_AD_01 is configured as FLEXPWM1_PWMB00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_PAD_ETH3_TX_CTL__FLEXPWM1_PWMA2,      /* GPIO_AD_01 is configured as FLEXPWM1_PWMB00 */
      0U);
}

/*******************************************************************************
 * SINC Pin Configuration
 ******************************************************************************/

void BOARD_InitSinc2Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_SD2_CLK__SINC2_EMBIT0, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_SD2_CLK__SINC2_EMBIT0,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_SD2_CD_B__SINC2_EMCLK0, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_SD2_CD_B__SINC2_EMCLK0,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);
}

/*******************************************************************************
 * NETC/Ethernet Pin Configuration
 ******************************************************************************/

void BOARD_InitNETC_ETH0_Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD3__NETC_PINMUX_ETH0_TXD3, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD2__NETC_PINMUX_ETH0_TXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD1__NETC_PINMUX_ETH0_TXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TXD0__NETC_PINMUX_ETH0_TXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TX_EN__NETC_PINMUX_ETH0_TX_EN, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TX_ER__NETC_PINMUX_ETH0_TX_ER, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_TX_CLK__NETC_PINMUX_ETH0_TX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RX_DV__NETC_PINMUX_ETH0_RX_DV, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RX_ER__NETC_PINMUX_ETH0_RX_ER, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_CRS__NETC_PINMUX_ETH0_CRS, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_COL__NETC_PINMUX_ETH0_COL, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RX_CLK__NETC_PINMUX_ETH0_RX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD0__NETC_PINMUX_ETH0_RXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD1__NETC_PINMUX_ETH0_RXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD2__NETC_PINMUX_ETH0_RXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH0_RXD3__NETC_PINMUX_ETH0_RXD3, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_TXD3__NETC_PINMUX_ETH0_TXD3, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_TXD2__NETC_PINMUX_ETH0_TXD2, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_TXD1__NETC_PINMUX_ETH0_TXD1, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_TXD0__NETC_PINMUX_ETH0_TXD0, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_TX_EN__NETC_PINMUX_ETH0_TX_EN, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_TX_ER__NETC_PINMUX_ETH0_TX_ER, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_TX_CLK__NETC_PINMUX_ETH0_TX_CLK, 0x58eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_RX_DV__NETC_PINMUX_ETH0_RX_DV, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_RX_ER__NETC_PINMUX_ETH0_RX_ER, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_CRS__NETC_PINMUX_ETH0_CRS, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_COL__NETC_PINMUX_ETH0_COL, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_RX_CLK__NETC_PINMUX_ETH0_RX_CLK, 0x58eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_RXD0__NETC_PINMUX_ETH0_RXD0, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_RXD1__NETC_PINMUX_ETH0_RXD1, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_RXD2__NETC_PINMUX_ETH0_RXD2, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH0_RXD3__NETC_PINMUX_ETH0_RXD3, 0x50eU);
}

void BOARD_InitNETC_ETH1_Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD3__NETC_PINMUX_ETH1_TXD3, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD2__NETC_PINMUX_ETH1_TXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD1__NETC_PINMUX_ETH1_TXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TXD0__NETC_PINMUX_ETH1_TXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TX_EN__NETC_PINMUX_ETH1_TX_EN, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_TX_CLK__NETC_PINMUX_ETH1_TX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RX_DV__NETC_PINMUX_ETH1_RX_DV, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RX_CLK__NETC_PINMUX_ETH1_RX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD0__NETC_PINMUX_ETH1_RXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD1__NETC_PINMUX_ETH1_RXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD2__NETC_PINMUX_ETH1_RXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH1_RXD3__NETC_PINMUX_ETH1_RXD3, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_TXD3__NETC_PINMUX_ETH1_TXD3, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_TXD2__NETC_PINMUX_ETH1_TXD2, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_TXD1__NETC_PINMUX_ETH1_TXD1, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_TXD0__NETC_PINMUX_ETH1_TXD0, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_TX_EN__NETC_PINMUX_ETH1_TX_EN, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_TX_CLK__NETC_PINMUX_ETH1_TX_CLK, 0x58eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_RX_DV__NETC_PINMUX_ETH1_RX_DV, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_RX_CLK__NETC_PINMUX_ETH1_RX_CLK, 0x58eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_RXD0__NETC_PINMUX_ETH1_RXD0, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_RXD1__NETC_PINMUX_ETH1_RXD1, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_RXD2__NETC_PINMUX_ETH1_RXD2, 0x50eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH1_RXD3__NETC_PINMUX_ETH1_RXD3, 0x50eU);
}

void BOARD_InitNETC_ETH2_Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TXD3__NETC_PINMUX_ETH2_TXD3, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TXD2__NETC_PINMUX_ETH2_TXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TXD1__NETC_PINMUX_ETH2_TXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TXD0__NETC_PINMUX_ETH2_TXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TX_CTL__NETC_PINMUX_ETH2_TX_CTL, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_TX_CLK__NETC_PINMUX_ETH2_TX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RX_CTL__NETC_PINMUX_ETH2_RX_CTL, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RX_CLK__NETC_PINMUX_ETH2_RX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD0__NETC_PINMUX_ETH2_RXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD1__NETC_PINMUX_ETH2_RXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD2__NETC_PINMUX_ETH2_RXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH2_RXD3__NETC_PINMUX_ETH2_RXD3, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_TXD3__NETC_PINMUX_ETH2_TXD3, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_TXD2__NETC_PINMUX_ETH2_TXD2, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_TXD1__NETC_PINMUX_ETH2_TXD1, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_TXD0__NETC_PINMUX_ETH2_TXD0, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_TX_CTL__NETC_PINMUX_ETH2_TX_CTL, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_TX_CLK__NETC_PINMUX_ETH2_TX_CLK, 0x5feU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_RX_CTL__NETC_PINMUX_ETH2_RX_CTL, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_RX_CLK__NETC_PINMUX_ETH2_RX_CLK, 0x5feU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_RXD0__NETC_PINMUX_ETH2_RXD0, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_RXD1__NETC_PINMUX_ETH2_RXD1, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_RXD2__NETC_PINMUX_ETH2_RXD2, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH2_RXD3__NETC_PINMUX_ETH2_RXD3, 0x57eU);
}

void BOARD_InitNETC_ETH3_Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TXD3__NETC_PINMUX_ETH3_TXD3, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TXD2__NETC_PINMUX_ETH3_TXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TXD1__NETC_PINMUX_ETH3_TXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TXD0__NETC_PINMUX_ETH3_TXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TX_CTL__NETC_PINMUX_ETH3_TX_CTL, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_TX_CLK__NETC_PINMUX_ETH3_TX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RX_CTL__NETC_PINMUX_ETH3_RX_CTL, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RX_CLK__NETC_PINMUX_ETH3_RX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD0__NETC_PINMUX_ETH3_RXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD1__NETC_PINMUX_ETH3_RXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD2__NETC_PINMUX_ETH3_RXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD3__NETC_PINMUX_ETH3_RXD3, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_TXD3__NETC_PINMUX_ETH3_TXD3, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_TXD2__NETC_PINMUX_ETH3_TXD2, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_TXD1__NETC_PINMUX_ETH3_TXD1, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_TXD0__NETC_PINMUX_ETH3_TXD0, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_TX_CTL__NETC_PINMUX_ETH3_TX_CTL, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_TX_CLK__NETC_PINMUX_ETH3_TX_CLK, 0x5feU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_RX_CTL__NETC_PINMUX_ETH3_RX_CTL, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_RX_CLK__NETC_PINMUX_ETH3_RX_CLK, 0x5feU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_RXD0__NETC_PINMUX_ETH3_RXD0, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_RXD1__NETC_PINMUX_ETH3_RXD1, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_RXD2__NETC_PINMUX_ETH3_RXD2, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH3_RXD3__NETC_PINMUX_ETH3_RXD3, 0x57eU);
}

void BOARD_InitNETC_ETH4_Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_MDC_GPIO1__NETC_EMDC, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_MDIO_GPIO2__NETC_EMDIO, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_TXD3__NETC_PINMUX_ETH4_TXD3, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_TXD2__NETC_PINMUX_ETH4_TXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_TXD1__NETC_PINMUX_ETH4_TXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_TXD0__NETC_PINMUX_ETH4_TXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_TX_CTL__NETC_PINMUX_ETH4_TX_CTL, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_TX_CLK__NETC_PINMUX_ETH4_TX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_RX_CTL__NETC_PINMUX_ETH4_RX_CTL, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_RX_CLK__NETC_PINMUX_ETH4_RX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_RXD0__NETC_PINMUX_ETH4_RXD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_RXD1__NETC_PINMUX_ETH4_RXD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_RXD2__NETC_PINMUX_ETH4_RXD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH4_RXD3__NETC_PINMUX_ETH4_RXD3, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_MDC_GPIO1__NETC_EMDC, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_MDIO_GPIO2__NETC_EMDIO, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_TXD3__NETC_PINMUX_ETH4_TXD3, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_TXD2__NETC_PINMUX_ETH4_TXD2, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_TXD1__NETC_PINMUX_ETH4_TXD1, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_TXD0__NETC_PINMUX_ETH4_TXD0, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_TX_CTL__NETC_PINMUX_ETH4_TX_CTL, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_TX_CLK__NETC_PINMUX_ETH4_TX_CLK, 0x5feU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_RX_CTL__NETC_PINMUX_ETH4_RX_CTL, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_RX_CLK__NETC_PINMUX_ETH4_RX_CLK, 0x5feU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_RXD0__NETC_PINMUX_ETH4_RXD0, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_RXD1__NETC_PINMUX_ETH4_RXD1, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_RXD2__NETC_PINMUX_ETH4_RXD2, 0x57eU);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ETH4_RXD3__NETC_PINMUX_ETH4_RXD3, 0x57eU);
}



/*******************************************************************************
 * FlexIO Pin Configuration
 ******************************************************************************/

void BOARD_InitFlexio1Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO01__FLEXIO1_FLEXIO1, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO01__FLEXIO1_FLEXIO1,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO02__FLEXIO1_FLEXIO2, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO02__FLEXIO1_FLEXIO2,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO03__FLEXIO1_FLEXIO3, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO03__FLEXIO1_FLEXIO3,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));
}



void BOARD_InitFlexio1PwmPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO31__FLEXIO1_3_1_FLEXIO5, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO31__FLEXIO1_3_1_FLEXIO5,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));
}

void BOARD_InitFlexio1UartPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO30__FLEXIO1_3_1_FLEXIO4, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO30__FLEXIO1_3_1_FLEXIO4,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO31__FLEXIO1_3_1_FLEXIO5, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO31__FLEXIO1_3_1_FLEXIO5,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));
}

void BOARD_InitFlexio1SpiPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO00__FLEXIO1_FLEXIO0, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO00__FLEXIO1_FLEXIO0,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO01__FLEXIO1_FLEXIO1, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO01__FLEXIO1_FLEXIO1,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO02__FLEXIO1_FLEXIO2, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO02__FLEXIO1_FLEXIO2,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO03__FLEXIO1_FLEXIO3, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO03__FLEXIO1_FLEXIO3,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));
}

void BOARD_InitXbarPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO15__XBAR1_XBAR_INOUT8, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO15__XBAR1_XBAR_INOUT8,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(3U) |
                        IOMUXC_PAD_PU_MASK);
}


/*******************************************************************************
 * TPM Pin Configuration
 ******************************************************************************/

void BOARD_InitTpm6Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO08__TPM6_CH0, 0U);
}

void BOARD_InitTpm2Pins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_I2C2_SCL__TPM2_CH2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_I2C2_SDA__TPM2_CH3, 0U);
}

/*******************************************************************************
 * EQDC Pin Configuration
 ******************************************************************************/

void BOARD_InitEqdcPins(void)
{
    BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL2 = ((BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL2 &
      (~(BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL2_IOMUXC_XBAR_DIR_SEL_45_MASK | BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL2_IOMUXC_XBAR_DIR_SEL_46_MASK | BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL2_IOMUXC_XBAR_DIR_SEL_48_MASK)))
        | BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL2_IOMUXC_XBAR_DIR_SEL_45(0x00U)
        | BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL2_IOMUXC_XBAR_DIR_SEL_46(0x00U)
        | BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL2_IOMUXC_XBAR_DIR_SEL_48(0x00U)
    );

    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD0__XBAR1_XBAR_INOUT45, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD1__XBAR1_XBAR_INOUT46, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ETH3_RXD3__XBAR1_XBAR_INOUT48, 1U);
}


/*******************************************************************************
 * LPIT PWM Pin Configuration
 ******************************************************************************/

void BOARD_InitLpitPwmPins(void)
{
    BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL2 = ((BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL2 &
      (~(BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL2_IOMUXC_XBAR_DIR_SEL_40_MASK)))
        | BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL2_IOMUXC_XBAR_DIR_SEL_40(0x01U)
    );
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO01__XBAR1_XBAR_INOUT40, 0U);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
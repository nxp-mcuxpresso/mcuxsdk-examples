/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_02_LPUART2_TX,        /* GPIO_AD_B1_06 is configured as LPUART3_TX */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_03_LPUART2_RX,        /* GPIO_AD_B1_07 is configured as LPUART3_RX */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B1_02_LPUART2_TX,        /* GPIO_AD_B1_06 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                   Drive Strength Field: R0/6
                                                   Speed Field: medium(100MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B1_03_LPUART2_RX,        /* GPIO_AD_B1_07 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                   Drive Strength Field: R0/6
                                                   Speed Field: medium(100MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_USDHC2_CMD, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_USDHC2_CLK, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_USDHC2_DATA0, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_USDHC2_DATA1, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_USDHC2_DATA2, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_USDHC2_DATA3, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_USDHC2_DATA4, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_USDHC2_DATA5, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_USDHC2_DATA6, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_USDHC2_DATA7, 0);


    /* voltage select PIN */
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_38_USDHC2_VSELECT, 0);

     /* card RESET PIN */
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_USDHC2_RESET_B, 0);

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_USDHC2_CMD, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_SPEED(2) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                                       IOMUXC_SW_PAD_CTL_PAD_DSE(5));
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_USDHC2_CLK, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                                       IOMUXC_SW_PAD_CTL_PAD_DSE(5));
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_USDHC2_DATA0, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_USDHC2_DATA1, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_01_USDHC2_DATA2, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_00_USDHC2_DATA3, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_08_USDHC2_DATA4, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_09_USDHC2_DATA5, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_USDHC2_DATA6, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_USDHC2_DATA7, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5));


    /*voltage select pin*/
    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_38_USDHC2_VSELECT, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(4));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_06_USDHC2_RESET_B, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(1));
}

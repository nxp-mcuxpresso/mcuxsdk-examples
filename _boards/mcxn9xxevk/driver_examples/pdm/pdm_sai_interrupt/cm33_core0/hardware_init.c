/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_codec_common.h"
#include "fsl_dialog7212.h"
#include "fsl_codec_adapter.h"
#include "fsl_sai.h"
/*${header:end}*/

/*${variable:start}*/
da7212_pll_config_t pllConfig = {
    .source         = kDA7212_PLLClkSourceMCLK,
    .refClock_HZ    = 12288000U,
    .outputClock_HZ = kDA7212_PLLOutputClk12288000,
};
da7212_config_t da7212Config = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = 12000000},
    .dacSource    = kDA7212_DACSourceInputStream,
    .slaveAddress = DA7212_ADDRESS,
    .protocol     = kDA7212_BusI2S,
    .format       = {.mclk_HZ = 12288000U, .sampleRate = DEMO_AUDIO_SAMPLE_RATE, .bitWidth = DEMO_AUDIO_BIT_WIDTH},
    .pll          = &pllConfig,
    .sysClkSource = kDA7212_SysClkSourcePLL,
    .isMaster     = true,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_DA7212, .codecDevConfig = &da7212Config};

sai_master_clock_t mclkConfig;

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO 12M to LPFLEXCOMM2 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    /* attach FRO HF to SAI1 */
    CLOCK_SetClkDiv(kCLOCK_DivSai1Clk, 1u);
    CLOCK_AttachClk(kPLL1_CLK0_to_SAI1);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetupExtClocking(24000000U);
    /* Set up PLL1 */
    const pll_setup_t pll1Setup = {.pllctrl = SCG_SPLLCTRL_SOURCE(0U) | SCG_SPLLCTRL_SELI(15U) | SCG_SPLLCTRL_SELP(31U),
                                   .pllndiv = SCG_SPLLNDIV_NDIV(25U),
                                   .pllpdiv = SCG_SPLLPDIV_PDIV(2U),
                                   .pllmdiv = SCG_SPLLMDIV_MDIV(512U),
                                   .pllRate = 122880000U};
    /* Configure PLL1 to the 122.88MHz */
    CLOCK_SetPLL1Freq(&pll1Setup);

    /* Set PLL1CLK0DIV divider to value 10, PLL1_CLK0 = 12.288MHz */
    CLOCK_SetClkDiv(kCLOCK_DivPLL1Clk0, 10U);

    /* attach PLL1_CLK0 to PDM, PDM_CLK = 12.288MHz */
    CLOCK_SetClkDiv(kCLOCK_DivMicfilFClk, 1U);
    CLOCK_AttachClk(kPLL1_CLK0_to_MICFILF);
}

void BOARD_MasterClockConfig(void)
{
    mclkConfig.mclkOutputEnable = true, mclkConfig.mclkHz = 12288000U;
    mclkConfig.mclkSourceClkHz = 12288000U;
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
}

/*${function:end}*/

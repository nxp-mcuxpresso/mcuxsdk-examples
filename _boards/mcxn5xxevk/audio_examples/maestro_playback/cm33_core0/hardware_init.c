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
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_dialog7212.h"
#include "app.h"
#include "fsl_sai.h"
/*${header:end}*/

/*${variable:start}*/
codec_handle_t codecHandle    = {0};
da7212_pll_config_t pllConfig = {
    .source = kDA7212_PLLClkSourceMCLK,
#ifdef SUPPORT_48KHZ
    .refClock_HZ    = 12288000U,
    .outputClock_HZ = kDA7212_PLLOutputClk12288000,
#else
    .refClock_HZ    = 11289600U,
    .outputClock_HZ = kDA7212_PLLOutputClk11289600,
#endif
};
da7212_config_t da7212Config = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = 12000000},
    .dacSource    = kDA7212_DACSourceInputStream,
    .slaveAddress = DA7212_ADDRESS,
    .protocol     = kDA7212_BusI2S,
#ifdef SUPPORT_48KHZ
    .format = {.mclk_HZ = 12288000U, .sampleRate = 48000U, .bitWidth = 16U},
#else
    .format         = {.mclk_HZ = 11289600U, .sampleRate = 44100U, .bitWidth = 16U},
#endif
    .pll          = &pllConfig,
    .sysClkSource = kDA7212_SysClkSourcePLL,
    .isMaster     = true,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_DA7212, .codecDevConfig = &da7212Config};

sai_master_clock_t mclkConfig;

/*${variable:end}*/

/*${function:start}*/
void BOARD_MASTER_CLOCK_CONFIG(void);

int BOARD_CODEC_Init(void)
{
    CODEC_Init(&codecHandle, &boardCodecConfig);

    /* Initial volume kept low for hearing safety. */
    CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight, 50);

    return 0;
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockFROHF144M();
    BOARD_InitDebugConsole();

    /*!< Set the system clock to 144MHz */
    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 1U); /*!< Set AHBCLKDIV divider to value 1 */

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* attach FRO 12M to LPFLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO 12M to LPFLEXCOMM2 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    /* attach FRO HF to USDHC */
    CLOCK_SetClkDiv(kCLOCK_DivUSdhcClk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_USDHC);

    /* Enables the clock for GPIO0 */
    CLOCK_EnableClock(kCLOCK_Gpio0);
    /* Enables the clock for GPIO2 */
    CLOCK_EnableClock(kCLOCK_Gpio2);

    CLOCK_SetupExtClocking(24000000U);
    CLOCK_SetSysOscMonitorMode(kSCG_SysOscMonitorDisable); /* System OSC Clock Monitor is disabled */
#ifdef SUPPORT_48KHZ
    /*!< Set up PLL0 */
    const pll_setup_t pll0Setup = {
        .pllctrl = SCG_APLLCTRL_SOURCE(0U) | SCG_APLLCTRL_LIMUPOFF_MASK | SCG_APLLCTRL_SELI(4U) |
                   SCG_APLLCTRL_SELP(3U) | SCG_APLLCTRL_SELR(4U),
        .pllndiv = SCG_APLLNDIV_NDIV(6U),
        .pllpdiv = SCG_APLLPDIV_PDIV(7U),
        .pllsscg = {(SCG_APLLSSCG0_SS_MDIV_LSB(2886218023U)),
                    ((SCG0->APLLSSCG1 & ~SCG_APLLSSCG1_SS_PD_MASK) | SCG_APLLSSCG1_SS_MDIV_MSB(0U) |
                     (uint32_t)(kSS_MF_512) | (uint32_t)(kSS_MR_K0) | (uint32_t)(kSS_MC_NOC) |
                     SCG_APLLSSCG1_SEL_SS_MDIV_MASK)},
        .pllRate = 24576000U};
#else
    /*!< Set up PLL0 */
    const pll_setup_t pll0Setup = {
                .pllctrl = SCG_APLLCTRL_SOURCE(0U) | SCG_APLLCTRL_LIMUPOFF_MASK | SCG_APLLCTRL_SELI(4U) |
                   SCG_APLLCTRL_SELP(3U) | SCG_APLLCTRL_SELR(4U),
                .pllndiv = SCG_APLLNDIV_NDIV(15U),
                .pllpdiv = SCG_APLLPDIV_PDIV(9U),
                .pllsscg = {(SCG_APLLSSCG0_SS_MDIV_LSB(4228395303U)),
                            ((SCG0->APLLSSCG1 & ~SCG_APLLSSCG1_SS_PD_MASK) | SCG_APLLSSCG1_SS_MDIV_MSB(1U) |
                     (uint32_t)(kSS_MF_512) | (uint32_t)(kSS_MR_K0) | (uint32_t)(kSS_MC_NOC) |
                     SCG_APLLSSCG1_SEL_SS_MDIV_MASK)},
                .pllRate = 22579200U};
#endif
    CLOCK_SetPLL0Freq(&pll0Setup); /*!< Configure PLL0 to the desired values */

    /* force the APLL power on and gate on */
    SCG0->APLL_OVRD |= SCG_APLL_OVRD_APLLPWREN_OVRD_MASK | SCG_APLL_OVRD_APLLCLKEN_OVRD_MASK;
    SCG0->APLL_OVRD |= SCG_APLL_OVRD_APLL_OVRD_EN_MASK;
    CLOCK_SetClkDiv(kCLOCK_DivPllClk, 1U);

    /* attach PLL0 to SAI1 */
    CLOCK_SetClkDiv(kCLOCK_DivSai1Clk, 1u);
    CLOCK_AttachClk(kPLL0_to_SAI1);
}

void BOARD_MASTER_CLOCK_CONFIG(void)
{
#ifdef SUPPORT_48KHZ
    mclkConfig.mclkOutputEnable = true, mclkConfig.mclkHz = 12288000U;
    mclkConfig.mclkSourceClkHz = 24576000U;
#else
    mclkConfig.mclkOutputEnable = true, mclkConfig.mclkHz = 11289600U;
    mclkConfig.mclkSourceClkHz = 22579200U;
#endif
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
}

/*${function:end}*/

/*
 * Copyright 2022, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_cs42448.h"
#include "fsl_trdc.h"
#include "fsl_edma.h"
/*${header:end}*/

/*${function:start}*/
cs42448_config_t cs42448Config = {
    .DACMode      = kCS42448_ModeSlave,
    .ADCMode      = kCS42448_ModeSlave,
    .reset        = BORAD_CodecReset,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = DEMO_CS42448_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .format       = {.mclk_HZ = 24576000, .sampleRate = 48000U, .bitWidth = 24U},
    .bus          = kCS42448_BusTDM,
    .slaveAddress = CS42448_I2C_ADDR,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &cs42448Config};
/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM) / (2^POST)
 *                              = 24 * (32 + 768/1000)  / 2
 *                              = 393.216MHZ
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,   /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,    /* Divider after the PLL, should only be 0, 1, 2, 3, 4, 5 */
    .numerator   = 768,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 1000, /* 30 bit denominator of fractional loop divider */
};

/*${function:start}*/
void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL |= BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL &= ~BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
    }
}

void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_InitAudioPll(&audioPllConfig);
    /*Clock setting for SAI1*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Sai1, kCLOCK_SAI1_ClockRoot_MuxAudioPllOut);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, 16);

    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);
    
     /* enable codec power */
    RGPIO_PinWrite(DEMO_CODEC_POWER_GPIO, DEMO_CODEC_POWER_GPIO_PIN, 1U);
}

void BORAD_CodecReset(bool state)
{
    if (state)
    {
        RGPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 1U);
    }
    else
    {
        RGPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 0U);
    }
}
/*${function:end}*/

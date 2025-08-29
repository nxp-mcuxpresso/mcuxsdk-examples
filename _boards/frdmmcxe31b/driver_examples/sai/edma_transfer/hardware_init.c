/*
 * Copyright 2025 NXP
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
#include "fsl_wm8960.h"
#include "fsl_sai.h"
#include "fsl_dmamux.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
wm8960_config_t wm8960Config = {
    .i2cConfig = {.codecI2CInstance = DEMO_CODEC_I2C_INSTANCE},
    .route     = kWM8960_RoutePlaybackandRecord,
    .leftInputSource  = kWM8960_InputDifferentialMicInput3,
    .rightInputSource = kWM8960_InputDifferentialMicInput2,
    .playSource       = kWM8960_PlaySourceDAC,
    .slaveAddress     = WM8960_I2C_ADDR,
    .bus              = kWM8960_BusI2S,
    .masterClock = {.sysclkFreq = 12288000U, .sysclkSource = kWM8960_SysClkSourceInternalPLL},
    .format = {.mclk_HZ = 24000000U, .sampleRate = kWM8960_AudioSampleRate48KHz, .bitWidth = kWM8960_AudioBitWidth16bit},
    .master_slave = true,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960Config};

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitI2CPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitSAIPins();

    DMAMUX_Init(DEMO_DMAMUX);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_EDMA_CHANNEL, kDmaRequestMux0SAI0Tx);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_EDMA_CHANNEL);

    /* Need explicitly enable TCD clock. */
    CLOCK_EnableClock(kCLOCK_Tcd0);

    wm8960Config.i2cConfig.codecI2CSourceClock = CLOCK_GetFreq(kCLOCK_Lpi2c1Clk);
}



/*${function:end}*/

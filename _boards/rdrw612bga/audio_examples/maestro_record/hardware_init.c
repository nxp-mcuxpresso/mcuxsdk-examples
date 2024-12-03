/*
 * Copyright 2022-2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include <stdbool.h>
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_wm8904.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
codec_handle_t codecHandle = {0};

wm8904_config_t wm8904Config = {
    .i2cConfig          = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .recordSource       = kWM8904_RecordSourceLineInput,
    .recordChannelLeft  = kWM8904_RecordChannelLeft2,
    .recordChannelRight = kWM8904_RecordChannelRight2,
    .playSource         = kWM8904_PlaySourceDAC,
    .slaveAddress       = WM8904_I2C_ADDRESS,
    .protocol           = kWM8904_ProtocolI2S,
    .format             = {.sampleRate = kWM8904_SampleRate16kHz, .bitWidth = kWM8904_BitWidth16},
    .master             = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8904, .codecDevConfig = &wm8904Config};
/*${variable:end}*/

/*${function:start}*/
static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < 100; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;

    GPIO_PortInit(GPIO, BOARD_CODEC_I2C_SDA_PORT);
    GPIO_PortInit(GPIO, BOARD_CODEC_I2C_SCL_PORT);

    BOARD_InitI2CPinsAsGPIO();

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(GPIO, BOARD_CODEC_I2C_SDA_PORT, BOARD_CODEC_I2C_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(GPIO, BOARD_CODEC_I2C_SCL_PORT, BOARD_CODEC_I2C_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(GPIO, BOARD_CODEC_I2C_SDA_PORT, BOARD_CODEC_I2C_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(GPIO, BOARD_CODEC_I2C_SCL_PORT, BOARD_CODEC_I2C_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(GPIO, BOARD_CODEC_I2C_SCL_PORT, BOARD_CODEC_I2C_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, BOARD_CODEC_I2C_SDA_PORT, BOARD_CODEC_I2C_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, BOARD_CODEC_I2C_SCL_PORT, BOARD_CODEC_I2C_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, BOARD_CODEC_I2C_SDA_PORT, BOARD_CODEC_I2C_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

int BOARD_CODEC_Init(void)
{
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        return -1;
    }
    
    /* Invert the DAC data in order to output signal with correct polarity - set DACL_DATINV and DACR_DATINV = 1 */
    if (WM8904_WriteRegister((wm8904_handle_t *)codecHandle.codecDevHandle, WM8904_AUDIO_IF_0, 0x1850) !=
        kStatus_WM8904_Success)
    {
        return -1;
    }
    
    /* Initial volume kept low for hearing safety. */
    if (CODEC_SetVolume(&codecHandle, ALL_CHANNELS, DEMO_VOLUME) != kStatus_Success)
    {
        return -1;
    }
    
    return 0;
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_I2C_ReleaseBus();
    BOARD_InitI2CPins();

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* attach SFRO clock to I2C2 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM2);

    /* attach AUDIO PLL clock to FLEXCOMM1 (I2S1) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    /* DMIC source from audio pll, divider 6, 12.288M/6=2.048MHZ */
    CLOCK_AttachClk(kAUDIO_PLL_to_DMIC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 6);

    wm8904Config.i2cConfig.codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ;
    wm8904Config.mclk_HZ                       = CLOCK_GetMclkClkFreq();
}
/*${function:end}*/

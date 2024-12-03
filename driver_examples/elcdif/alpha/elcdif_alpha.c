/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_elcdif.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef APP_LCDIF_DATA_BUS
#define APP_LCDIF_DATA_BUS kELCDIF_DataBus24Bit
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBuffer[APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_asBuffer[APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);

/*******************************************************************************
 * Code
 ******************************************************************************/

void APP_FillPsBuffer(uint32_t buffer[][APP_IMG_WIDTH])
{
    /* PS buffer is a blue rectangle in the center. */

    uint32_t i, j;

    /* Background color. */
    memset(buffer, 0, sizeof(uint32_t) * APP_IMG_WIDTH * APP_IMG_HEIGHT);

    /* Foreground color. */
    for (i = APP_IMG_HEIGHT / 4; i < APP_IMG_HEIGHT * 3 / 4; i++)
    {
        for (j = APP_IMG_WIDTH / 4; j < APP_IMG_WIDTH * 3 / 4; j++)
        {
            buffer[i][j] = 0x000000FFU;
        }
    }
}

void APP_ELCDIF_Init(void)
{
    const elcdif_rgb_mode_config_t config = {
        .panelWidth    = APP_IMG_WIDTH,
        .panelHeight   = APP_IMG_HEIGHT,
        .hsw           = APP_HSW,
        .hfp           = APP_HFP,
        .hbp           = APP_HBP,
        .vsw           = APP_VSW,
        .vfp           = APP_VFP,
        .vbp           = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .bufferAddr    = (uint32_t)s_psBuffer,
        .pixelFormat   = kELCDIF_PixelFormatXRGB8888,
        .dataBus       = APP_LCDIF_DATA_BUS,
    };

    ELCDIF_RgbModeInit(APP_ELCDIF, &config);
}

void APP_FillAsBuffer(uint32_t buffer[][APP_IMG_WIDTH])
{
    /* AS buffer is full screen red. */

    uint32_t i, j;

    /* Foreground color. */
    for (i = 0; i < APP_IMG_HEIGHT; i++)
    {
        for (j = 0; j < APP_IMG_WIDTH; j++)
        {
            buffer[i][j] = 0x00FF0000U;
        }
    }
}

void APP_ELCDIF_AlphaRun(void)
{
    uint32_t alpha   = 0U;
    int32_t alphaInc = 1U;

    const elcdif_as_buffer_config_t asBufferConfig = {
        .bufferAddr  = (uint32_t)s_asBuffer,
        .pixelFormat = kELCDIF_AsPixelFormatRGB888,
    };

    elcdif_as_blend_config_t asBlendConfig = {
        .alpha       = alpha,
        .invertAlpha = false,
        .alphaMode   = kELCDIF_AlphaOverride,
        .ropMode     = kELCDIF_RopMaskAs, /* Don't care. */
    };

    ELCDIF_SetAlphaSurfaceBufferConfig(APP_ELCDIF, &asBufferConfig);
    ELCDIF_SetAlphaSurfaceBlendConfig(APP_ELCDIF, &asBlendConfig);
    ELCDIF_EnableAlphaSurface(APP_ELCDIF, true);

    ELCDIF_EnableInterrupts(APP_ELCDIF, kELCDIF_CurFrameDoneInterruptEnable);
    ELCDIF_RgbModeStart(APP_ELCDIF);

    while (1)
    {
        alpha += alphaInc;

        if (0xFF == alpha)
        {
            alphaInc = -1;
        }
        else if (0 == alpha)
        {
            alphaInc = 1;
        }

        asBlendConfig.alpha = alpha;

        /* Wait for previous frame complete. */
        while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
        {
        }
        ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);

        ELCDIF_SetAlphaSurfaceBlendConfig(APP_ELCDIF, &asBlendConfig);
    }
}

void setUp(void)
{
}

void tearDown(void)
{
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("LCDIF alpha example started ...\r\n");

    APP_FillPsBuffer(s_psBuffer);

    APP_FillAsBuffer(s_asBuffer);

    APP_ELCDIF_Init();

    APP_ELCDIF_AlphaRun();

    while (1)
    {
    }
}

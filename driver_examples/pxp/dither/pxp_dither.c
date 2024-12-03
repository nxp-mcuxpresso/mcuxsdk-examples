/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_pxp.h"
#include "fsl_elcdif.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef APP_LCDIF_DATA_BUS
#define APP_LCDIF_DATA_BUS kELCDIF_DataBus24Bit
#endif

#define APP_BPP 4U /* Use 24-bit RGB888 format. */

/* The pixel format. */
typedef struct _pixel
{
    uint32_t B : 8;
    uint32_t G : 8;
    uint32_t R : 8;
    uint8_t : 8;
} pixel_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitInputBuffer(void);
static void APP_InitLcdif(void);
static void APP_InitPxp(void);
static void APP_DitherOrdered(void);
static void APP_DitherQuantOnly(void);
static void APP_DitherPassThrough(void);
static void APP_UpdateFrame(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_BufferLcd[2][APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static pixel_t s_BufferPxp[APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);

/* PXP Output buffer config. */
static pxp_output_buffer_config_t outputBufferConfig;

/* Ordered dithering index matrix. */
static uint8_t s_matrix8[8][8];

/* Active frame buffer index for LCD. */
static uint8_t s_lcdActiveFbIdx = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\nPXP dither example start...\r\n");

    APP_InitInputBuffer();
    APP_InitPxp();
    APP_InitLcdif();

    while (1)
    {
        APP_DitherPassThrough();
        APP_DitherQuantOnly();
        APP_DitherOrdered();
    }
}

static void APP_InitLcdif(void)
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
        .bufferAddr    = (uint32_t)s_BufferLcd[s_lcdActiveFbIdx],
        .pixelFormat   = kELCDIF_PixelFormatXRGB8888,
        .dataBus       = APP_LCDIF_DATA_BUS,
    };

    ELCDIF_RgbModeInit(APP_ELCDIF, &config);

    ELCDIF_RgbModeStart(APP_ELCDIF);
}

static void APP_InitPxp(void)
{
    PXP_Init(APP_PXP);

    /* Output config. */
    outputBufferConfig.pixelFormat    = kPXP_OutputPixelFormatRGB888;
    outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    outputBufferConfig.buffer1Addr    = 0U;
    outputBufferConfig.pitchBytes     = APP_IMG_WIDTH * APP_BPP;
    outputBufferConfig.width          = APP_IMG_WIDTH;
    outputBufferConfig.height         = APP_IMG_HEIGHT;

    /* PS configure. */
    const pxp_ps_buffer_config_t psBufferConfig = {
#if (!(defined(FSL_FEATURE_PXP_HAS_NO_EXTEND_PIXEL_FORMAT) && FSL_FEATURE_PXP_HAS_NO_EXTEND_PIXEL_FORMAT)) && \
    (!(defined(FSL_FEATURE_PXP_V3) && FSL_FEATURE_PXP_V3))
        .pixelFormat = kPXP_PsPixelFormatARGB8888,
#else
        .pixelFormat = kPXP_PsPixelFormatRGB888,
#endif
        .swapByte    = false,
        .bufferAddr  = (uint32_t)s_BufferPxp,
        .bufferAddrU = 0U,
        .bufferAddrV = 0U,
        .pitchBytes  = APP_IMG_WIDTH * APP_BPP,
    };

#if defined(FSL_FEATURE_PXP_V3) && FSL_FEATURE_PXP_V3
    PXP_SetProcessSurfaceBackGroundColor(APP_PXP, 0U, 0U);
#else
    PXP_SetProcessSurfaceBackGroundColor(APP_PXP, 0U);
#endif

    PXP_SetProcessSurfaceBufferConfig(APP_PXP, &psBufferConfig);
    PXP_SetProcessSurfacePosition(APP_PXP, 0U, 0U, APP_IMG_WIDTH - 1U, APP_IMG_HEIGHT - 1U);

    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(APP_PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

    /* Disable CSC1, it is enabled by default. */
    PXP_EnableCsc1(APP_PXP, false);

    PXP_EnableDither(APP_PXP, true);
    PXP_SetPath(APP_PXP, kPXP_Mux14SelectDitherEngine);

    PXP_SetProcessBlockSize(APP_PXP, kPXP_BlockSize8);

    /* Set the Ordered dithering index matrix. */
    PXP_SetInternalRamData(APP_PXP, kPXP_RamDither0Lut, 8 * 8, (uint8_t *)s_matrix8, 0U);
    PXP_SetInternalRamData(APP_PXP, kPXP_RamDither1Lut, 8 * 8, (uint8_t *)s_matrix8, 0U);
    PXP_SetInternalRamData(APP_PXP, kPXP_RamDither2Lut, 8 * 8, (uint8_t *)s_matrix8, 0U);
}

static void APP_DitherPassThrough(void)
{
    const pxp_dither_config_t config = {.enableDither0  = 1,
                                        .enableDither1  = 1,
                                        .enableDither2  = 1,
                                        .ditherMode0    = kPXP_DitherPassThrough,
                                        .ditherMode1    = kPXP_DitherPassThrough,
                                        .ditherMode2    = kPXP_DitherPassThrough,
                                        .quantBitNum    = 1,
                                        .lutMode        = kPXP_DitherLutOff,
                                        .idxMatrixSize0 = kPXP_DitherMatrix8,
                                        .idxMatrixSize1 = kPXP_DitherMatrix8,
                                        .idxMatrixSize2 = kPXP_DitherMatrix8,
                                        .enableFinalLut = 0};

    PRINTF("Press any key to see the original picture...\r\n");
    GETCHAR();

    PXP_SetDitherConfig(APP_PXP, &config);

    APP_UpdateFrame();
}

static void APP_DitherQuantOnly(void)
{
    const pxp_dither_config_t config = {.enableDither0  = 1,
                                        .enableDither1  = 1,
                                        .enableDither2  = 1,
                                        .ditherMode0    = kPXP_DitherQuantOnly,
                                        .ditherMode1    = kPXP_DitherQuantOnly,
                                        .ditherMode2    = kPXP_DitherQuantOnly,
                                        .quantBitNum    = 4,
                                        .lutMode        = kPXP_DitherLutOff,
                                        .idxMatrixSize0 = kPXP_DitherMatrix8,
                                        .idxMatrixSize1 = kPXP_DitherMatrix8,
                                        .idxMatrixSize2 = kPXP_DitherMatrix8,
                                        .enableFinalLut = 0};

    PRINTF("Press any key to see the quantized picture...\r\n");
    GETCHAR();

    PXP_SetDitherConfig(APP_PXP, &config);

    APP_UpdateFrame();
}

static void APP_DitherOrdered(void)
{
    const pxp_dither_config_t config = {.enableDither0  = 1,
                                        .enableDither1  = 1,
                                        .enableDither2  = 1,
                                        .ditherMode0    = kPXP_DitherOrdered,
                                        .ditherMode1    = kPXP_DitherOrdered,
                                        .ditherMode2    = kPXP_DitherOrdered,
                                        .quantBitNum    = 4,
                                        .lutMode        = kPXP_DitherLutOff,
                                        .idxMatrixSize0 = kPXP_DitherMatrix8,
                                        .idxMatrixSize1 = kPXP_DitherMatrix8,
                                        .idxMatrixSize2 = kPXP_DitherMatrix8,
                                        .enableFinalLut = 0};

    PRINTF("Press any key to see the ordered dithering picture...\r\n");
    GETCHAR();

    PXP_SetDitherConfig(APP_PXP, &config);

    APP_UpdateFrame();
}

static void APP_InitInputBuffer(void)
{
    uint32_t i, j;
    uint8_t R = 0;
    uint8_t G = 0;

    const uint8_t matrix4[4][4] = {
        {0, 8, 2, 10},
        {12, 4, 14, 6},
        {3, 11, 1, 9},
        {15, 7, 13, 5},
    };

    memset(s_BufferLcd, 0x0U, sizeof(s_BufferLcd));
    memset(s_BufferPxp, 0x0U, sizeof(s_BufferPxp));

    /* Init the input buffer. */
    for (i = 0; i < APP_IMG_HEIGHT; i++)
    {
        for (j = 0; j < APP_IMG_WIDTH; j++)
        {
            s_BufferPxp[i][j].R = R;
            s_BufferPxp[i][j].G = G;
            G++;
        }
        G = 0;
        R++;
    }

    /* Init the ordered dithering input matrix. */
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            s_matrix8[i][j]         = 4 * matrix4[i][j];
            s_matrix8[i][j + 4]     = 4 * matrix4[i][j] + 2;
            s_matrix8[i + 4][j]     = 4 * matrix4[i][j] + 3;
            s_matrix8[i + 4][j + 4] = 4 * matrix4[i][j] + 1;
        }
    }
}

static void APP_UpdateFrame(void)
{
    s_lcdActiveFbIdx ^= 1;
    outputBufferConfig.buffer0Addr = (uint32_t)s_BufferLcd[s_lcdActiveFbIdx];
    PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

    /* Start PXP process and wait for complete. */
    PXP_ClearStatusFlags(APP_PXP, kPXP_CompleteFlag);
    PXP_Start(APP_PXP);
    while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(APP_PXP)))
    {
    }

    /* Set the new frame buffer to LCD and wait for active. */
    ELCDIF_SetNextBufferAddr(APP_ELCDIF, (uint32_t)s_BufferLcd[s_lcdActiveFbIdx]);
    ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);
    while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
    {
    }
}

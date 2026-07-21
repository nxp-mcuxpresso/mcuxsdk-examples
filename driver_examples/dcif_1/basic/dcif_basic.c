/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_dcif.h"
#include "dcif_support.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_INPUT_RGB565 0
#define DEMO_INPUT_INDEX  1

#define DEMO_INPUT_BUFFER_FORMAT DEMO_INPUT_RGB565

#define DEMO_ALIGN_STRIDE(bytes) (((bytes) + (DCIF_FB_ALIGN - 1U)) & ~((uint32_t)DCIF_FB_ALIGN - 1U))

#define DEMO_IMG_HEIGHT DEMO_PANEL_HEIGHT
#define DEMO_IMG_WIDTH  DEMO_PANEL_WIDTH

#if DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_INDEX
/* 4 bit per pixel, 2 pixels take 1 byte. */
#define DEMO_IMG_BYTES_PER_LINE DEMO_ALIGN_STRIDE(DEMO_IMG_WIDTH / 2U)
#else
/* RGB565 2 bytes per pixel. */
#define DEMO_IMG_BYTES_PER_LINE DEMO_ALIGN_STRIDE(DEMO_IMG_WIDTH * 2U)
#endif

#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DPI
#define DEMO_INTERFACE_FRAME_DONE_INTERRUPT kDCIF_InterruptVsync
#define DEMO_INTERFACE kDCIF_OutputDpi
#elif DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DBI
#define DEMO_INTERFACE_FRAME_DONE_INTERRUPT kDCIF_InterruptDbiCommandDone
#define DEMO_INTERFACE kDCIF_OutputDbi
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DPI
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_frameBuffer[2][DEMO_IMG_HEIGHT][DEMO_IMG_BYTES_PER_LINE],
    DCIF_FB_ALIGN);
static uint32_t s_frameBufferAddr[2] = {(uint32_t)s_frameBuffer[0], (uint32_t)s_frameBuffer[1]};
#else
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_frameBuffer[1][DEMO_IMG_HEIGHT][DEMO_IMG_BYTES_PER_LINE],
    DCIF_FB_ALIGN);
static uint32_t s_frameBufferAddr[1] = {(uint32_t)s_frameBuffer[0]};
#endif

static volatile bool s_frameDone = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_DCIF_IRQHandler(void)
{
    uint32_t intStatus;

    intStatus = DCIF_GetInterruptStatus(DEMO_DCIF, DEMO_DOMAIN);

    if (0 != (intStatus & DEMO_INTERFACE_FRAME_DONE_INTERRUPT))
    {
        s_frameDone = true;
        DCIF_ClearInterruptStatus(DEMO_DCIF, DEMO_DOMAIN, intStatus);
    }
    SDK_ISR_EXIT_BARRIER;
}

void DEMO_FillFrameBuffer(uint32_t frameBufferAddr)
{
    uint8_t(*frameBuffer)[DEMO_IMG_BYTES_PER_LINE] = (void *)frameBufferAddr;

    /* Foreground color. */
    static uint8_t fgColorIndex = 0U;
#if DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_RGB565
    static const uint16_t fgColorTable[] = {0xF800U, 0x7E0U, 0x1FU, 0xFFFFU};
#else
    static const uint8_t fgColorTable[] = {1U, 2U, 3U, 4U}; /* R, G, B, W CLUT indices */
#endif
    uint32_t fgColor = fgColorTable[fgColorIndex];

    /* Position of the foreground rectangle. */
    static uint16_t upperLeftX  = 0;
    static uint16_t upperLeftY  = 0;
    static uint16_t lowerRightX = (DEMO_IMG_WIDTH - 1U) / 2U;
    static uint16_t lowerRightY = (DEMO_IMG_HEIGHT - 1U) / 2U;

    static int8_t incX = 1;
    static int8_t incY = 1;

    /* Change color in next frame or not. */
    static bool changeColor = false;

    uint32_t i, j;

    /* Set background color to black. */
    memset(frameBuffer, 0, DEMO_IMG_BYTES_PER_LINE * DEMO_IMG_HEIGHT);

    /* Foreground color. */
    for (i = upperLeftY; i < lowerRightY; i++)
    {
        for (j = upperLeftX; j < lowerRightX; j++)
        {
#if DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_INDEX
            /* 4 bpp indexed: two horizontally-adjacent pixels share one byte.
             * Write only the 4-bit CLUT index into this pixel's nibble; a
             * 16-bit store here would clobber 4 pixels. Convention: even column
             * -> low nibble [3:0], odd column -> high nibble [7:4]. If the
             * rectangle edge lands one pixel off, flip the two nibble halves. */
            uint8_t *px = &frameBuffer[i][j / 2U];
            if ((j & 1U) == 0U)
            {
                *px = (uint8_t)((*px & 0xF0U) | (fgColor & 0x0FU));
            }
            else
            {
                *px = (uint8_t)((*px & 0x0FU) | ((fgColor & 0x0FU) << 4U));
            }
#else
            *((uint16_t *)&frameBuffer[i][j * 2]) = fgColor;
#endif
        }
    }

    /* Update the format: color and rectangle position. */
    upperLeftX += incX;
    upperLeftY += incY;
    lowerRightX += incX;
    lowerRightY += incY;

    changeColor = false;

    if (0U == upperLeftX)
    {
        incX        = 1;
        changeColor = true;
    }
    else if (DEMO_IMG_WIDTH - 1 == lowerRightX)
    {
        incX        = -1;
        changeColor = true;
    }

    if (0U == upperLeftY)
    {
        incY        = 1;
        changeColor = true;
    }
    else if (DEMO_IMG_HEIGHT - 1 == lowerRightY)
    {
        incY        = -1;
        changeColor = true;
    }

    if (changeColor)
    {
        fgColorIndex++;

        if (ARRAY_SIZE(fgColorTable) == fgColorIndex)
        {
            fgColorIndex = 0U;
        }
    }
}

void DEMO_DCIF_Init(void)
{
    static dcif_output_config_t outputConfig = {
        .interface = DEMO_INTERFACE,
        .width     = DEMO_PANEL_WIDTH,
        .height    = DEMO_PANEL_HEIGHT,
    };

    DCIF_Init(DEMO_DCIF);

    /* Set panel output config. */
    DCIF_SetOutputConfig(DEMO_DCIF, &outputConfig);

    /* Init the specific interface type according to the panel used. */
    DEMO_InitInterface();

    /* Background black. */
    DCIF_SetBackGroundLayerColor(DEMO_DCIF, 0x0U);

#if DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_INDEX
    /* Configure the CLUT table. */
    dcif_clut_config_t clutConfig;
    /* ARGB8888. Index 0 = black background; 1..4 = pure R, G, B, W (the colors
     * the rectangle rotates through); 5..7 unused. */
    static const uint32_t fgColorTable[] = {0U, 0xFFFF0000U, 0xFF00FF00U, 0xFF0000FFU,
                                        0xFFFFFFFFU, 0U, 0U, 0U};

    /* CLUT for indexed input format. */
    DCIF_ClutGetDefaultConfig(&clutConfig);
    DCIF_ClutSetConfig(DEMO_DCIF, &clutConfig);
    DCIF_ClutSetData(DEMO_DCIF, 0U, fgColorTable, 8U);
#endif

    NVIC_EnableIRQ(DEMO_DCIF_IRQn);

    /* Need to enable DCIF output before initializing the DBI panel.
     * For MIPI panel the DPI interface shall be enabled later. */
    DCIF_EnableOutput(DEMO_DCIF, true);

    /* Initialize panel. */
    if (kStatus_Success != BOARD_InitDisplayInterface())
    {
        PRINTF("Display panel initialize failed\r\n");

        while (1)
        {
        }
    }
}

void DEMO_DCIF_RUN(void)
{
    uint32_t frameBufferIndex = 0;
    dcif_layer_config_t config;

    DEMO_FillFrameBuffer(s_frameBufferAddr[frameBufferIndex]);

    memset(&config, 0, sizeof(config));

    config.enable           = true;
#if DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_RGB565
    config.format           = kDCIF_LayerPixelFormatRGB565;
#elif DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_INDEX
    config.format           = kDCIF_LayerPixelFormatIndex4BPP;
#endif
    config.topLeftX         = 0U;
    config.topLeftY         = 0U;
    config.width            = DEMO_IMG_WIDTH;
    config.height           = DEMO_IMG_HEIGHT;
    config.background       = 0U;
    config.panic.enable     = false;
    config.globalAlpha      = 0xFFU;
    config.alphaBlendMode   = kDCIF_AlphaBlendOverride;//kDCIF_AlphaBlendEmbedded;

    DCIF_SetLayerAddr(DEMO_DCIF, 0, (uint32_t)s_frameBufferAddr[frameBufferIndex]);

    DCIF_SetLayerStride(DEMO_DCIF, 0, DEMO_IMG_BYTES_PER_LINE);

    DCIF_SetLayerConfig(DEMO_DCIF, 0U, &config);

    DCIF_TriggerLayerShadowLoad(DEMO_DCIF, 0);

#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DBI
    DEMO_DbiSelectUpdateArea(0U, 0U, DEMO_PANEL_WIDTH - 1U, DEMO_PANEL_HEIGHT - 1U);
#endif

    /* Frame done interrupt. */
    DCIF_EnableInterrupts(DEMO_DCIF, DEMO_DOMAIN, DEMO_INTERFACE_FRAME_DONE_INTERRUPT);

    while (1)
    {
#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DPI
        /* Only DPI need 2 frame buffers. */
        frameBufferIndex ^= 1U;
#endif

        while (!s_frameDone)
        {
        }

        s_frameDone = false;
        DEMO_FillFrameBuffer(s_frameBufferAddr[frameBufferIndex]);

        /* Update frame according to the interface type. */
        DEMO_UpdateFrame((uint32_t)s_frameBufferAddr[frameBufferIndex]);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("DCIF basic example start\r\n");

#if DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_RGB565
    PRINTF("The input format is RGB565\r\n");
#else
    PRINTF("The input format is Index\r\n");
#endif

    DEMO_DCIF_Init();

    DEMO_DCIF_RUN();

    while (1)
    {
    }
}

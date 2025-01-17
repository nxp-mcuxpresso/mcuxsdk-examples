/*
 * Copyright 2023 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_lcdifv3.h"
#include "lcdifv3_support.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_BYTE_PER_PIXEL 2U

#define DEMO_IMG_HEIGHT         DEMO_PANEL_HEIGHT
#define DEMO_IMG_WIDTH          DEMO_PANEL_WIDTH
#define DEMO_IMG_BYTES_PER_LINE (DEMO_PANEL_WIDTH * DEMO_BYTE_PER_PIXEL)

/* Use layer 0 in this example. */
#define DEMO_LCDIFV3_LAYER 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t s_frameBufferAddr[2] = {DEMO_FB0_ADDR, DEMO_FB1_ADDR};

static volatile bool s_frameDone = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_LCDIFV3_IRQHandler(void)
{
    uint32_t intStatus;

    intStatus = LCDIFV3_GetInterruptStatus(DEMO_LCDIFV3);
    LCDIFV3_ClearInterruptStatus(DEMO_LCDIFV3, intStatus);

    if (0 != (intStatus & kLCDIFV3_VerticalBlankingInterrupt))
    {
        s_frameDone = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

void DEMO_FillFrameBuffer(uint32_t frameBufferAddr)
{
    uint32_t(*frameBuffer)[DEMO_IMG_WIDTH] = (void *)frameBufferAddr;

    /* Foreground color. */
    static uint8_t fgColorIndex          = 0U;
    static const uint16_t fgColorTable[] = {0x001FU, 0x07E0U, 0x07FFU, 0xF800U, 0xF81FU, 0xFFE0U, 0xFFFFU};
    uint16_t fgColor                     = fgColorTable[fgColorIndex];

    /* Position of the foreground rectangle. */
    static uint16_t upperLeftX  = 0;
    static uint16_t upperLeftY  = 0;
    static uint16_t lowerRightX = (DEMO_IMG_WIDTH - 1U) / 2U;
    static uint16_t lowerRightY = (DEMO_IMG_HEIGHT - 1U) / 2U;

    static int16_t incX = 1;
    static int16_t incY = 1;

    /* Change color in next forame or not. */
    static bool changeColor = false;

    uint32_t i, j;

    /* Set background color to black. */
    memset(frameBuffer, 0, DEMO_IMG_WIDTH * DEMO_IMG_HEIGHT * DEMO_BYTE_PER_PIXEL);

    /* Foreground color. */
    for (i = upperLeftY; i < lowerRightY; i++)
    {
        for (j = upperLeftX; j < lowerRightX; j++)
        {
            frameBuffer[i][j] = fgColor;
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

void DEMO_LCDIFV3_Init(void)
{
    const lcdifv3_display_config_t lcdifv3Config = {
        .panelWidth    = DEMO_PANEL_WIDTH,
        .panelHeight   = DEMO_PANEL_HEIGHT,
        .hsw           = DEMO_HSW,
        .hfp           = DEMO_HFP,
        .hbp           = DEMO_HBP,
        .vsw           = DEMO_VSW,
        .vfp           = DEMO_VFP,
        .vbp           = DEMO_VBP,
        .polarityFlags = DEMO_POL_FLAGS,
        .lineOrder     = kLCDIFV3_LineOrderRGB565,
    };

    const lcdifv3_buffer_config_t fbConfig = {
        .strideBytes = DEMO_IMG_WIDTH * DEMO_BYTE_PER_PIXEL,
        .pixelFormat = kLCDIFV3_PixelFormatRGB565,
    };

    if (kStatus_Success != BOARD_InitDisplayInterface())
    {
        PRINTF("Display interface initialize failed\r\n");

        while (1)
        {
        }
    }

    LCDIFV3_Init(DEMO_LCDIFV3);

    LCDIFV3_SetDisplayConfig(DEMO_LCDIFV3, &lcdifv3Config);

    LCDIFV3_EnableDisplay(DEMO_LCDIFV3, true);

    LCDIFV3_SetLayerBufferConfig(DEMO_LCDIFV3, DEMO_LCDIFV3_LAYER, &fbConfig);

    LCDIFV3_SetLayerSize(DEMO_LCDIFV3, DEMO_LCDIFV3_LAYER, DEMO_IMG_WIDTH, DEMO_IMG_HEIGHT);

    LCDIFV3_EnableLayer(DEMO_LCDIFV3, DEMO_LCDIFV3_LAYER, true);
    
    NVIC_EnableIRQ(DEMO_LCDIFV3_IRQn);

    LCDIFV3_EnableInterrupts(DEMO_LCDIFV3, kLCDIFV3_VerticalBlankingInterrupt);
}

void DEMO_LCDIFV3_RGB(void)
{

    uint32_t frameBufferIndex = 0;

    DEMO_FillFrameBuffer(s_frameBufferAddr[frameBufferIndex]);

    while (1)
    {
        LCDIFV3_SetLayerBufferAddr(DEMO_LCDIFV3, DEMO_LCDIFV3_LAYER, (uint32_t)s_frameBufferAddr[frameBufferIndex]);
        LCDIFV3_TriggerLayerShadowLoad(DEMO_LCDIFV3, DEMO_LCDIFV3_LAYER);

        /*
         * Wait for previous frame complete.
         * New frame buffer configuration load at the next VSYNC.
         */
        s_frameDone = false;
        while (!s_frameDone)
        {
        }

        frameBufferIndex ^= 1U;
        DEMO_FillFrameBuffer(s_frameBufferAddr[frameBufferIndex]);
    }

}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("LCDIF v3 RGB565 example start...\r\n");

    DEMO_LCDIFV3_Init();

    PRINTF("LCDIF v3 RGB565 example init...\r\n");

    DEMO_LCDIFV3_RGB();

    while (1)
    {
    }
}

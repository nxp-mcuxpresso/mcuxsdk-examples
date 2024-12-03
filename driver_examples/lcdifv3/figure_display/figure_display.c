/*
 * Copyright 2023 NXP
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
static uint32_t s_frameBufferAddr[DEMO_BUFFER_COUNT]   = {DEMO_FB0_ADDR, DEMO_FB1_ADDR};
static uint32_t s_pictureBitmapAddr[DEMO_BUFFER_COUNT] = {DEMO_FB0_BITMAP_ADDR, DEMO_FB1_BITMAP_ADDR};
static uint32_t s_updatePicFlagAddr[DEMO_BUFFER_COUNT] = {DEMO_FB0_UPDATE_PIC_FLAG_ADDR, DEMO_FB1_UPDATE_PIC_FLAG_ADDR};
static volatile bool s_frameDone                       = false;

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

void DEMO_FillFrameBuffer(uint32_t frameBufferIndex)
{
    uint32_t(*frameBuffer)[DEMO_IMG_WIDTH] = (void *)s_frameBufferAddr[frameBufferIndex];
    /* Set background color to black. */
    memset(frameBuffer, 0, DEMO_IMG_WIDTH * DEMO_IMG_HEIGHT * DEMO_BYTE_PER_PIXEL);
    /* Memory copy the RGB565 format file from RAM to DRAM for display */
    memcpy(frameBuffer, (void *)s_pictureBitmapAddr[frameBufferIndex],
           DEMO_IMG_WIDTH * DEMO_IMG_HEIGHT * DEMO_BYTE_PER_PIXEL);
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
        .strideBytes = DEMO_IMG_BYTES_PER_LINE,
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

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t frameBufferIndex = 0;
    uint8_t i                 = 0;
    BOARD_InitHardware();
    PRINTF("LCDIFV3 FIGURE DISPLAY example start...\r\n");

    for (i = 0; i < ARRAY_SIZE(s_pictureBitmapAddr); i++)
    {
        PRINTF("The bitmap address of picture(s_pictureBitmapAddr[%d]) is 0x%x\r\n", i, s_pictureBitmapAddr[i]);
    }

    for (i = 0; i < ARRAY_SIZE(s_frameBufferAddr); i++)
    {
        PRINTF("The framebuffer address (s_frameBufferAddr[%d)] is 0x%x\r\n", i, s_frameBufferAddr[i]);
    }
    for (i = 0; i < ARRAY_SIZE(s_updatePicFlagAddr); i++)
    {
        PRINTF("The flag address (s_updatePicFlagAddr[%d]) is 0x%x\r\n", i, s_updatePicFlagAddr[i]);
    }
    PRINTF(
        "Please use the uboot command to load picture binary to the bitmap address of picture: "
        "s_pictureBitmapAddr[0~1], and set a flag(s_updatePicFlagAddr[0~1]) in memory to notify mcu to update the "
        "picture\r\n");
    PRINTF("For example:\r\n");
    for (i = 0; i < ARRAY_SIZE(s_pictureBitmapAddr); i++)
    {
        PRINTF("load mmc 1:1 0x%x /pic%d.bin\r\n", s_pictureBitmapAddr[i], i);
    }
    for (i = 0; i < ARRAY_SIZE(s_updatePicFlagAddr); i++)
    {
        PRINTF("mw 0x%x 0x%x 1\r\n", s_updatePicFlagAddr[i], UPDATE_PIC_FLAG);
    }
    DEMO_LCDIFV3_Init();

    PRINTF("LCDIFV3 init...\r\n");

    for (;;)
    {
        if (*(uint32_t *)s_updatePicFlagAddr[frameBufferIndex] == UPDATE_PIC_FLAG)
        {
            DEMO_FillFrameBuffer(frameBufferIndex);
            LCDIFV3_SetLayerBufferAddr(DEMO_LCDIFV3, DEMO_LCDIFV3_LAYER, (uint32_t)s_frameBufferAddr[frameBufferIndex]);
            LCDIFV3_TriggerLayerShadowLoad(DEMO_LCDIFV3, DEMO_LCDIFV3_LAYER);
            /* clear update picture flags */
            *(uint32_t *)s_updatePicFlagAddr[frameBufferIndex] = 0U;
            /*
             *  Wait for previous frame complete.
             *  New frame buffer configuration load at the next VSYNC.
             */
            s_frameDone = false;
            while (!s_frameDone)
            {
            }
            frameBufferIndex ^= 1U;
            PRINTF("The figure is displaying on panel\r\n");
        }
        else
        {
            /* Nothing to do */
        }
    }
}

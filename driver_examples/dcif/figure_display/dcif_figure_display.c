/*
 * Copyright 2024 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_dcif.h"
#include "dcif_support.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_BYTE_PER_PIXEL 4U

#define DEMO_IMG_HEIGHT         DEMO_PANEL_HEIGHT
#define DEMO_IMG_WIDTH          DEMO_PANEL_WIDTH
#define DEMO_IMG_BYTES_PER_LINE (DEMO_PANEL_WIDTH * DEMO_BYTE_PER_PIXEL)

/* Use layer 1 in this example. */
#define DEMO_DCIF_LAYER 1

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
void DEMO_DCIF_IRQHandler(void)
{
    uint32_t intStatus;

    intStatus = DCIF_GetInterruptStatus(DEMO_DCIF);
    DCIF_ClearInterruptStatus(DEMO_DCIF, intStatus);

    if (0 != (intStatus & kDCIF_VerticalBlankingInterrupt))
    {
        s_frameDone = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

void DEMO_DCIF_Init(void)
{
    const dcif_display_config_t dcifConfig = {
        .panelWidth    = DEMO_PANEL_WIDTH,
        .panelHeight   = DEMO_PANEL_HEIGHT,
        .hsw           = DEMO_HSW,
        .hfp           = DEMO_HFP,
        .hbp           = DEMO_HBP,
        .vsw           = DEMO_VSW,
        .vfp           = DEMO_VFP,
        .vbp           = DEMO_VBP,
        .polarityFlags = DEMO_POL_FLAGS,
        .lineOrder     = kDCIF_LineOrderRGB,
    };

    const dcif_buffer_config_t fbConfig = {
        .strideBytes = DEMO_IMG_WIDTH * DEMO_BYTE_PER_PIXEL,
        .pixelFormat = kDCIF_PixelFormatARGB8888,
        .globalAlpha = 0xFF,
    };

    BOARD_InitDisplayInterface();

    DCIF_Init(DEMO_DCIF);

    DCIF_SetDisplayConfig(DEMO_DCIF, &dcifConfig);

    DCIF_EnableDisplay(DEMO_DCIF, true);

    DCIF_SetLayerBufferConfig(DEMO_DCIF, DEMO_DCIF_LAYER, &fbConfig);

    DCIF_SetLayerPosition(DEMO_DCIF, DEMO_DCIF_LAYER, 0U, 0U);

    DCIF_SetLayerSize(DEMO_DCIF, DEMO_DCIF_LAYER, DEMO_IMG_WIDTH, DEMO_IMG_HEIGHT);

    DCIF_EnableLayer(DEMO_DCIF, DEMO_DCIF_LAYER, true);
    
    EnableIRQ(DEMO_DCIF_IRQ);

    DCIF_EnableInterrupts(DEMO_DCIF, kDCIF_VerticalBlankingInterrupt);

}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("DCIF figure display example start...\r\n");

    DEMO_DCIF_Init();

    PRINTF("please load the picture to the bitmap address of picture\r\n");


    while (1)
    {
        DCIF_SetLayerBufferAddr(DEMO_DCIF, DEMO_DCIF_LAYER, (uint32_t)s_frameBufferAddr[0]);
        DCIF_TriggerLayerShadowLoad(DEMO_DCIF, DEMO_DCIF_LAYER);

        s_frameDone = false;
        while (!s_frameDone)
        {
        }
    }
}

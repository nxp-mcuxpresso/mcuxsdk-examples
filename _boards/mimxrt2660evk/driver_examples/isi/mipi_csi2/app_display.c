/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "app_display.h"
#include "display_support.h"
#include "isi_config.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define APP_DC_LAYER 0U

typedef struct
{
    uint32_t               activeFb;
    app_display_callback_t callback;
    volatile bool          isFramePending;
} app_display_handle_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

static app_display_handle_t s_display;
static dc_fb_info_t         s_fbInfo;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void APP_DcSwitchOffCallback(void *param, void *switchOffBuffer);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void APP_DcSwitchOffCallback(void *param, void *switchOffBuffer)
{
    (void)param;

    s_display.isFramePending = false;
    s_display.activeFb       = (uint32_t)(uintptr_t)switchOffBuffer;

    if (s_display.callback != NULL)
    {
        s_display.callback((uint32_t)(uintptr_t)switchOffBuffer);
    }
}

void APP_InitDisplay(uint32_t frameBuffer, app_display_callback_t callback)
{
    status_t status;

    (void)memset(&s_display, 0, sizeof(s_display));
    s_display.callback = callback;

    (void)BOARD_PrepareDisplayController();

    status = g_dc.ops->init(&g_dc);
    if (kStatus_Success != status)
    {
        assert(false);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, APP_DC_LAYER, &s_fbInfo);
    s_fbInfo.pixelFormat = kVIDEO_PixelFormatRGB565;
    s_fbInfo.width       = APP_CAMERA_OUTPUT_WIDTH;
    s_fbInfo.height      = APP_CAMERA_OUTPUT_HEIGHT;
    s_fbInfo.startX      = 0U;
    s_fbInfo.startY      = 0U;
    s_fbInfo.strideBytes = APP_CAMERA_OUTPUT_WIDTH * 2U;
    (void)g_dc.ops->setLayerConfig(&g_dc, APP_DC_LAYER, &s_fbInfo);

    g_dc.ops->setCallback(&g_dc, APP_DC_LAYER, APP_DcSwitchOffCallback, NULL);

    s_display.activeFb       = frameBuffer;
}

void APP_StartDisplay(uint32_t firstFrameBuffer)
{

    s_display.isFramePending = false;
    g_dc.ops->setFrameBuffer(&g_dc, APP_DC_LAYER, (void *)firstFrameBuffer);

    if (0U == (g_dc.ops->getProperty(&g_dc) & (uint32_t)kDC_FB_ReserveFrameBuffer))
    {
        while (!s_display.isFramePending)
        {
        }
    }

    s_display.isFramePending = false;

    g_dc.ops->enableLayer(&g_dc, APP_DC_LAYER);
}

bool APP_IsDisplayFramePending(void)
{
    return s_display.isFramePending;
}

void APP_SetDisplayFrameBuffer(uint32_t frameBuffer)
{
    s_display.isFramePending = true;
    s_display.activeFb       = frameBuffer;
    g_dc.ops->setFrameBuffer(&g_dc, APP_DC_LAYER, (void *)(uintptr_t)frameBuffer);
}

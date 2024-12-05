/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_dpu.h"
#include "display_support.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/
/* ARGB8888, 4 bytes per pixel. */
#define APP_BPP 4

#if 1
#define APP_MAKE_COLOR(red, green, blue, alpha) \
    ((((uint32_t)(alpha)) << 24U) | (((uint32_t)(red)) << 16U) | (((uint32_t)(green)) << 8U) | ((uint32_t)(blue)))

#define APP_COLOR_BLUE    APP_MAKE_COLOR(0, 0, 0xFF, 0xFF)
#define APP_COLOR_GREEN   APP_MAKE_COLOR(0, 0xFF, 0, 0xFF)
#define APP_COLOR_RED     APP_MAKE_COLOR(0xFF, 0, 0, 0xFF)
#define APP_COLOR_YELLOW  APP_MAKE_COLOR(0xFF, 0xFF, 0, 0xFF)
#define APP_COLOR_CYAN    APP_MAKE_COLOR(0, 0xFF, 0xFF, 0xFF)
#define APP_COLOR_MAGENTA APP_MAKE_COLOR(0xFF, 0, 0xFF, 0xFF)
#define APP_COLOR_BLACK   APP_MAKE_COLOR(0, 0, 0, 0xFF)
#define APP_COLOR_WHITE   APP_MAKE_COLOR(0xFF, 0xFF, 0xFF, 0xFF)
#define APP_COLOR_SILVER  APP_MAKE_COLOR(0xC0, 0xC0, 0xC0, 0xFF)
#define APP_COLOR_GRAY    APP_MAKE_COLOR(0x80, 0x80, 0x80, 0xFF)
#endif

#define APP_PANEL_FB_ADDR_ALIGN_BYTE   (32U)
#define APP_PANEL_FB_STRIDE_ALIGN_BYTE APP_BPP

#define APP_PANEL_FB_STRIDE_BYTE  (SDK_SIZEALIGN(APP_BPP * APP_PANEL_WIDTH, APP_PANEL_FB_STRIDE_ALIGN_BYTE))
#define APP_PANEL_FB_STRIDE_PIXEL (APP_PANEL_FB_STRIDE_BYTE / APP_BPP)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Frame buffer to display. */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_displayFrameBuffer[2][APP_PANEL_HEIGHT][APP_PANEL_FB_STRIDE_PIXEL],
                              APP_PANEL_FB_ADDR_ALIGN_BYTE);

volatile bool s_isContentStreamShadowPending = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

bool APP_IsContentStreamShadowPending(void)
{
    return s_isContentStreamShadowPending;
}

void APP_TriggerContentStreamShadowLoad(void)
{
    s_isContentStreamShadowPending = true;
    DPU_TriggerPipelineShadowLoad(APP_DPU, APP_CONTENT_STREAM_PIPELINE);
}

void APP_DPU_Display(void)
{
    uint8_t displayFbIdx = 0;
    uint32_t bufferAddr  = 0;

    dpu_display_timing_config_t displayTimingConfig;
    dpu_const_frame_config_t cfConfig;
    dpu_layer_blend_config_t lbConfig;
    dpu_fetch_unit_config_t fetchConfig;
    dpu_src_buffer_config_t sbConfig;
    dpu_display_config_t displayConfig;

    /* Step 1: Configure the const stream */

    /* Pipeline Configuration */
    DPU_InitPipeline(APP_DPU, APP_CONTENT_STREAM_PIPELINE);

    /* ExtDst: set the dynamic source as layerblend 1 */
    DPU_InitExtDst(APP_DPU, APP_CONTENT_STREAM_EXTDST, DPU_MAKE_SRC_REG1(kDPU_UnitSrcLayerBlend1));

    /* Layer blend 1 source: prim input constframe0, sec input FetchYUV0. */
    DPU_InitLayerBlend(APP_DPU, kDPU_LayerBlend1, DPU_MAKE_SRC_REG2(kDPU_UnitSrcConstFrame0, kDPU_UnitSrcFetchYUV0));
    DPU_LayerBlendGetDefaultConfig(&lbConfig);
    DPU_SetLayerBlendConfig(APP_DPU, kDPU_LayerBlend1, &lbConfig);
    DPU_EnableLayerBlend(APP_DPU, kDPU_LayerBlend1, true);

    /* Init domain blend */
    DPU_InitDomainBlend(APP_DPU, APP_CONTENT_DOMAINBLEND);

    /* Const frame 0 configuration. */
    cfConfig.frameWidth  = APP_PANEL_WIDTH;
    cfConfig.frameHeight = APP_PANEL_HEIGHT;
    cfConfig.constColor  = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    DPU_InitConstFrame(APP_DPU, kDPU_ConstFrame0);
    DPU_SetConstFrameConfig(APP_DPU, kDPU_ConstFrame0, &cfConfig);

    /*
     * Fetch unit.
     */
    DPU_FetchUnitGetDefaultConfig(&fetchConfig);
    fetchConfig.frameHeight = APP_PANEL_HEIGHT;
    fetchConfig.frameWidth  = APP_PANEL_WIDTH;
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchYuv0, &fetchConfig);

    /* Fetch unit source buffer configure. */
    DPU_SrcBufferGetDefaultConfig(&sbConfig);

    sbConfig.bitsPerPixel = 32;
    sbConfig.pixelFormat  = kDPU_PixelFormatARGB8888;
    sbConfig.constColor   = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);

    /* FetchYUV0 configuration. */
    sbConfig.strideBytes  = APP_PANEL_FB_STRIDE_BYTE;
    sbConfig.bufferHeight = APP_PANEL_HEIGHT;
    sbConfig.bufferWidth  = APP_PANEL_WIDTH;
    sbConfig.baseAddr     = (uint32_t)s_displayFrameBuffer[displayFbIdx];
    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchYuv0, 0, &sbConfig);
    DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchYuv0, 0, 0, 0);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchYuv0, 0, true);

    /* Configuration complete, trigger the shadow load. */
    APP_TriggerContentStreamShadowLoad();

    DPU_EnableInterrupts(APP_DPU_DC, 0, APP_CONTENT_STREAM_INT_GROUP0);
    DPU_EnableInterrupts(APP_DPU_DC, 1, APP_CONTENT_STREAM_INT_GROUP1);

    DPU_TriggerDisplayDbShadowLoad(APP_DPU, APP_CONTENT_DOMAINBLEND);

    /* Step 2: Configure the display stream. */
    DPU_DisplayTimingGetDefaultConfig(&displayTimingConfig);

    displayTimingConfig.flags  = APP_DPU_TIMING_FLAGS;
    displayTimingConfig.width  = APP_PANEL_WIDTH;
    displayTimingConfig.hsw    = APP_HSW;
    displayTimingConfig.hfp    = APP_HFP;
    displayTimingConfig.hbp    = APP_HBP;
    displayTimingConfig.height = APP_PANEL_HEIGHT;
    displayTimingConfig.vsw    = APP_VSW;
    displayTimingConfig.vfp    = APP_VFP - 1U;
    displayTimingConfig.vbp    = APP_VBP + 1U;

    DPU_InitDisplayTiming(APP_DPU, APP_DPU_DISPLAY_INDEX, &displayTimingConfig);

    DPU_DisplayGetDefaultConfig(&displayConfig);

    /* Only show the content stream in normal mode. */
    displayConfig.displayMode = kDPU_DisplayOnlyPrim;

    displayConfig.primAreaStartX = 1;
    displayConfig.primAreaStartY = 1;

    DPU_SetDisplayConfig(APP_DPU, APP_DPU_DISPLAY_INDEX, &displayConfig);

    DPU_TriggerDisplayShadowLoad(APP_DPU, APP_DPU_DISPLAY_INDEX);

    /*
     * Initialize workflow:
     * 1. Init DPU
     * 2. Configure pixel link.
     * 3. Init and start display interface.
     * 4. Start DPU
     */
    APP_InitPixelLink();

    BOARD_InitDisplayInterface();

    DPU_StartDisplay(APP_DPU, APP_DPU_DISPLAY_INDEX);

    /* Display figure in different buffer */
    for (;;)
    {
        /* Delay some time. */
        for (uint32_t i = 0; i < 0x40000000; i++)
        {
            __NOP();
        }

        while (APP_IsContentStreamShadowPending())
        {
        }

        /* Show the other frame buffer. */
        displayFbIdx ^= 1;
        bufferAddr = (uint32_t)s_displayFrameBuffer[displayFbIdx];

        DPU_SetFetchUnitSrcBufferAddr(APP_DPU, kDPU_FetchYuv0, 0, bufferAddr);

        APP_TriggerContentStreamShadowLoad();
    }
}

void DPU_IRQHandler(void)
{
    uint32_t intGroup0Flags, intGroup1Flags;
    intGroup0Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 0);
    intGroup1Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 1);

    /* Check the ExtDstxShadowLoadInterrupt in status reg for Display engine */
    if ((intGroup0Flags & APP_CONTENT_STREAM_INT_GROUP0) || (intGroup1Flags & APP_CONTENT_STREAM_INT_GROUP0))
    {
        s_isContentStreamShadowPending = false;
        DPU_ClearInterruptsPendingFlags(APP_DPU_DC, 0, intGroup0Flags);
        DPU_ClearInterruptsPendingFlags(APP_DPU_DC, 1, intGroup1Flags);
    }

    DPU_ClearInterruptsPendingFlags(APP_DPU, 0, intGroup0Flags);
    DPU_ClearInterruptsPendingFlags(APP_DPU, 1, intGroup1Flags);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();
    PRINTF("DPU Figure Display Example:\r\n");
    PRINTF("Please Load The Figure Hex file by Using Jlink:\r\n");

    DPU_Init(APP_DPU);

    DPU_PreparePathConfig(APP_DPU);

    /* Display frame buffer ready, start to show. */
    APP_DPU_Display();

    while (1)
    {
    }
}

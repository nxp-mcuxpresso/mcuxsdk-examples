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

#define APP_WIN_WIDTH  (APP_PANEL_WIDTH * 3 / 4)
#define APP_WIN_HEIGHT (APP_PANEL_HEIGHT * 3 / 4)

#define APP_WIN0_OFFSET_X (0)
#define APP_WIN0_OFFSET_Y (0)

#define APP_WIN1_OFFSET_X (APP_PANEL_WIDTH / 4)
#define APP_WIN1_OFFSET_Y (0)

#define APP_WIN2_OFFSET_X (APP_PANEL_WIDTH / 8)
#define APP_WIN2_OFFSET_Y (APP_PANEL_HEIGHT / 4)

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
/* Frame buffer for red window */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_redWinFrameBuffer[APP_WIN_HEIGHT][APP_WIN_WIDTH], 32);
/* Frame buffer for green window */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_greenWinFrameBuffer[APP_WIN_HEIGHT][APP_WIN_WIDTH], 32);
/* Frame buffer for blue window */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_blueWinFrameBuffer[APP_WIN_HEIGHT][APP_WIN_WIDTH], 32);

/* Frame buffer to display. */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_displayFrameBuffer[2][APP_PANEL_HEIGHT][APP_PANEL_FB_STRIDE_PIXEL],
                              APP_PANEL_FB_ADDR_ALIGN_BYTE);

volatile bool s_isContentStreamShadowPending = false;
volatile bool s_isSafetyStreamShadowPending  = false;
volatile bool s_isBlitEngineFrameComplete    = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_InitFrameBuffer(uint32_t frameBuffer, uint16_t height, uint16_t width, uint16_t strideBytes, uint32_t color)
{
    uint32_t i, j;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            ((uint32_t *)(frameBuffer + strideBytes * i))[j] = color;
        }
    }
}

bool APP_IsContentStreamShadowPending(void)
{
    return s_isContentStreamShadowPending;
}

void APP_TriggerContentStreamShadowLoad(void)
{
    s_isContentStreamShadowPending = true;
    DPU_TriggerPipelineShadowLoad(APP_DPU, APP_CONTENT_STREAM_PIPELINE);
}

bool APP_IsBlitEngineFrameComplete(void)
{
    return s_isBlitEngineFrameComplete;
}

void APP_TriggerAndStartBlitEngine(void)
{
    s_isBlitEngineFrameComplete = false;
    DPU_TriggerPipelineShadowLoad(APP_DPU, kDPU_PipelineStore9);
    DPU_StartStore(APP_DPU, kDPU_Store9);
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

    /* Moving the float window. */
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

/*
 * Use the blit engine, merge the output of fetch decode 9, fetch rot 9, and
 * fetch eco 9 into one framebuffer.
 */
void APP_DPU_Rop(void)
{
    dpu_fetch_unit_config_t fetchConfig;
    dpu_dst_buffer_config_t dbConfig;
    dpu_rop_config_t ropConfig;
    dpu_src_buffer_config_t sbConfig;

    /* Pipeline Store9 initialize */
    DPU_InitPipeline(APP_DPU, kDPU_PipelineStore9);

    /* Store unit. */
    DPU_InitStore(APP_DPU, kDPU_Store9, DPU_MAKE_SRC_REG1(kDPU_UnitSrcBlitBlend9));

    DPU_DstBufferGetDefaultConfig(&dbConfig);
    dbConfig.baseAddr     = (uint32_t)s_displayFrameBuffer[0];
    dbConfig.strideBytes  = APP_PANEL_FB_STRIDE_BYTE;
    dbConfig.bitsPerPixel = 32;
    dbConfig.pixelFormat  = kDPU_PixelFormatARGB8888;
    dbConfig.bufferHeight = APP_PANEL_HEIGHT;
    dbConfig.bufferWidth  = APP_PANEL_WIDTH;

    DPU_SetStoreDstBufferConfig(APP_DPU, kDPU_Store9, &dbConfig);

    /* Blit blend: Set dynamic prim source input as Rop 9. */
    DPU_InitBlitBlend(APP_DPU, kDPU_BlitBlend9, DPU_MAKE_SRC_REG2(kDPU_UnitSrcRop9, kDPU_UnitSrcNone));
    /* When the blitBlend unit disabled, only the primary input is output directly. */
    DPU_EnableBlitBlend(APP_DPU, kDPU_BlitBlend9, false);

    /* Rop 9 : set dymamic source prim input as Eco 9, sec input as Decode 9*/
    DPU_InitRop(APP_DPU, kDPU_Rop9,
                DPU_MAKE_SRC_REG3(kDPU_UnitSrcFetchRot9, kDPU_UnitSrcFetchEco9, kDPU_UnitSrcFetchDecode9));
    DPU_RopGetDefaultConfig(&ropConfig);
    ropConfig.controlFlags = kDPU_RopAddRed | kDPU_RopAddGreen | kDPU_RopAddBlue | kDPU_RopAddAlpha;
    DPU_SetRopConfig(APP_DPU, kDPU_Rop9, &ropConfig);
    DPU_EnableRop(APP_DPU, kDPU_Rop9, true);

    /* Configure the fetch units. */
    DPU_FetchUnitGetDefaultConfig(&fetchConfig);
    fetchConfig.frameHeight = APP_PANEL_HEIGHT;
    fetchConfig.frameWidth  = APP_PANEL_WIDTH;

    DPU_SrcBufferGetDefaultConfig(&sbConfig);
    sbConfig.bitsPerPixel = 32;
    sbConfig.pixelFormat  = kDPU_PixelFormatARGB8888;
    sbConfig.constColor   = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    sbConfig.strideBytes  = 4 * APP_WIN_WIDTH;
    sbConfig.bufferHeight = APP_WIN_HEIGHT;
    sbConfig.bufferWidth  = APP_WIN_WIDTH;

    /* Fetch decode 9 used for the red window. */
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchDecode9, &fetchConfig);
    sbConfig.baseAddr = (uint32_t)s_redWinFrameBuffer;
    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchDecode9, 0, &sbConfig);
    DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchDecode9, 0, APP_WIN0_OFFSET_X, APP_WIN0_OFFSET_Y);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchDecode9, 0, true);

    DPU_SetFetchUnitClipColor(APP_DPU, kDPU_FetchDecode9, kDPU_ClipColorNull, 0);

    /* Fetch eco 9 used for the blue window. */
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchEco9, &fetchConfig);
    sbConfig.baseAddr = (uint32_t)s_blueWinFrameBuffer;
    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchEco9, 0, &sbConfig);
    DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchEco9, 0, APP_WIN2_OFFSET_X, APP_WIN2_OFFSET_Y);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchEco9, 0, true);

    DPU_SetFetchUnitClipColor(APP_DPU, kDPU_FetchEco9, kDPU_ClipColorNull, 0);

    /* Fetch Rot 9 used for the green window. */
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchRot9, &fetchConfig);
    sbConfig.baseAddr = (uint32_t)s_greenWinFrameBuffer;
    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchRot9, 0, &sbConfig);
    DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchRot9, 0, APP_WIN1_OFFSET_X, APP_WIN1_OFFSET_Y);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchRot9, 0, true);

    DPU_SetFetchUnitClipColor(APP_DPU, kDPU_FetchRot9, kDPU_ClipColorNull, 0);

    DPU_EnableInterrupts(APP_DPU_BLIT, 0, APP_BLIT_COM_INT_GROUP0);
    DPU_EnableInterrupts(APP_DPU_BLIT, 1, APP_BLIT_COM_INT_GROUP1);

    /* Pipeline configuration finished, start the process. */
    APP_TriggerAndStartBlitEngine();

    /* Wait for the blit completed. */
    while (!APP_IsBlitEngineFrameComplete())
    {
    }

    /* Set blit configuration for the next frame. */
    ropConfig.controlFlags = 0;
    ropConfig.blueIndex    = 0x1U;
    ropConfig.redIndex     = 0x1U;
    ropConfig.greenIndex   = 0x1U;
    ropConfig.alphaIndex   = 0xFFU;
    DPU_SetRopConfig(APP_DPU, kDPU_Rop9, &ropConfig);

    DPU_SetStoreDstBufferAddr(APP_DPU, kDPU_Store9, (uint32_t)s_displayFrameBuffer[1]);

    /* Pipeline configuration finished, start the process. */
    APP_TriggerAndStartBlitEngine();

    /* Wait for the blit completed. */
    while (!APP_IsBlitEngineFrameComplete())
    {
    }

    DPU_DeinitPipeline(APP_DPU, kDPU_PipelineStore9);

    DPU_DisableInterrupts(APP_DPU_BLIT, 0, APP_BLIT_COM_INT_GROUP0);
    DPU_DisableInterrupts(APP_DPU_BLIT, 1, APP_BLIT_COM_INT_GROUP1);
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

    /* Check the Store9FrameCompleteInterrupt in status reg for Blit engine */
    if ((intGroup0Flags & APP_BLIT_COM_INT_GROUP0) || (intGroup1Flags & APP_BLIT_COM_INT_GROUP1))
    {
        s_isBlitEngineFrameComplete = true;
        DPU_ClearInterruptsPendingFlags(APP_DPU_BLIT, 0, intGroup0Flags);
        DPU_ClearInterruptsPendingFlags(APP_DPU_BLIT, 1, intGroup1Flags);
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
    PRINTF("DPU ROP Example:\r\n");

    APP_InitFrameBuffer((uint32_t)s_redWinFrameBuffer, APP_WIN_HEIGHT, APP_WIN_WIDTH, sizeof(s_redWinFrameBuffer[0]),
                        APP_COLOR_RED);
    APP_InitFrameBuffer((uint32_t)s_blueWinFrameBuffer, APP_WIN_HEIGHT, APP_WIN_WIDTH, sizeof(s_blueWinFrameBuffer[0]),
                        APP_COLOR_BLUE);
    APP_InitFrameBuffer((uint32_t)s_greenWinFrameBuffer, APP_WIN_HEIGHT, APP_WIN_WIDTH,
                        sizeof(s_greenWinFrameBuffer[0]), APP_COLOR_GREEN);

    DPU_Init(APP_DPU);

    DPU_PreparePathConfig(APP_DPU);

    /* Generate frame buffer using blit engine. */
    APP_DPU_Rop();

    /* Display frame buffer ready, start to show. */
    APP_DPU_Display();

    while (1)
    {
    }
}

/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "app_display.h"
#include "isi_config.h"
#include "isi_example.h"
#include "fsl_dpu.h"
#include "dpu_example.h"
#include "fsl_ldb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct
{
    uint32_t activeFb;
    uint32_t inactiveFb;
    app_display_callback_t callback;
    volatile bool isFramePending;
} app_display_handle_t;

#ifndef APP_RESIZE_CAMERA_IMAGE
#define APP_RESIZE_CAMERA_IMAGE 0
#endif

/*
 * Scaler unit.
 * For up-scaling horizontally, the path is ->VSCALER -> HSCALER ->
 * For Down-scaling horizontally, the path is ->HSCALER -> VSCALER ->
 */
#if APP_RESIZE_CAMERA_IMAGE

#if (APP_CAMERA_WIDTH < APP_PANEL_WIDTH)
/* FETCHDECODE ->VSCALER -> HSCALER -> LAYERBLEND. */
#define APP_LAYERBLEND_SECOND_SRC kDPU_UnitSrcHScaler4
#define APP_VSCALER_SRC           kDPU_UnitSrcFetchYUV0
#define APP_HSCALER_SRC           kDPU_UnitSrcVScaler4

#else
/* FETCHDECODE ->HSCALER -> VSCALER -> LAYERBLEND. */
#define APP_LAYERBLEND_SECOND_SRC kDPU_UnitSrcVScaler4
#define APP_VSCALER_SRC           kDPU_UnitSrcHScaler4
#define APP_HSCALER_SRC           kDPU_UnitSrcFetchYUV0

#endif

#else
#define APP_LAYERBLEND_SECOND_SRC kDPU_UnitSrcFetchYUV0
#endif /* APP_RESIZE_CAMERA_IMAGE */

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

#define APP_PANEL_FB_ADDR_ALIGN_BYTE   (32U)
#define APP_BPP_NUM                2 /* In this example, the camera pixel format is RGB565. */
#define APP_PANEL_FB_STRIDE_ALIGN_BYTE APP_BPP_NUM

#define APP_PANEL_FB_STRIDE_BYTE  (SDK_SIZEALIGN(APP_BPP_NUM * APP_PANEL_WIDTH, APP_PANEL_FB_STRIDE_ALIGN_BYTE))
#define APP_PANEL_FB_STRIDE_PIXEL (APP_PANEL_FB_STRIDE_BYTE / APP_BPP_NUM)

/*******************************************************************************
 * Variables
 ******************************************************************************/
static app_display_handle_t s_displayHandle;
volatile bool s_isContentStreamShadowPending = false;
volatile bool s_isSafetyStreamShadowPending  = false;
volatile bool s_isBlitEngineFrameComplete    = false;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APP_StartDisplayInterface(void);

/*******************************************************************************
 * Codes
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

void APP_DPU_Display(uint32_t frameBuffer)
{
    dpu_display_timing_config_t displayTimingConfig;
    dpu_const_frame_config_t cfConfig;
    dpu_layer_blend_config_t lbConfig;
    dpu_fetch_unit_config_t fetchConfig;
    dpu_src_buffer_config_t sbConfig;
    dpu_display_config_t displayConfig;
#if APP_RESIZE_CAMERA_IMAGE
    dpu_scaler_config_t scConfig;
#endif

    /* Step 1: Configure the const stream */

    /* Pipeline Extdst0 Configuration */
    DPU_InitPipeline(APP_DPU, APP_CONTENT_STREAM_PIPELINE);

    /* ExtDst: set the dynamic source as layerblend 1 */
    DPU_InitExtDst(APP_DPU, APP_CONTENT_STREAM_EXTDST, DPU_MAKE_SRC_REG1(kDPU_UnitSrcLayerBlend1));

    /* Layer blend 1 source: prim input constframe0, sec input FetchYUV0. */
    DPU_InitLayerBlend(APP_DPU, kDPU_LayerBlend1, DPU_MAKE_SRC_REG2(kDPU_UnitSrcConstFrame0, APP_LAYERBLEND_SECOND_SRC));
    DPU_LayerBlendGetDefaultConfig(&lbConfig);
    DPU_SetLayerBlendConfig(APP_DPU, kDPU_LayerBlend1, &lbConfig);
    DPU_EnableLayerBlend(APP_DPU, kDPU_LayerBlend1, true);

    /* Init domain blend 0 */
    DPU_InitDomainBlend(APP_DPU, APP_CONTENT_DOMAINBLEND);

    /* Const frame 0 configuration. */
    cfConfig.frameWidth  = APP_PANEL_WIDTH;
    cfConfig.frameHeight = APP_PANEL_HEIGHT;
    cfConfig.constColor  = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    DPU_InitConstFrame(APP_DPU, kDPU_ConstFrame0);
    DPU_SetConstFrameConfig(APP_DPU, kDPU_ConstFrame0, &cfConfig);

#if APP_RESIZE_CAMERA_IMAGE
    /* Scaler */
    DPU_ScalerGetDefaultConfig(&scConfig);
    DPU_InitScaler(APP_DPU, kDPU_Hscaler4);
    scConfig.srcReg     = DPU_MAKE_SRC_REG1(APP_HSCALER_SRC);
    scConfig.inputSize  = APP_CAMERA_WIDTH;
    scConfig.outputSize = APP_PANEL_WIDTH;
    DPU_SetScalerConfig(APP_DPU, kDPU_Hscaler4, &scConfig);

    DPU_InitScaler(APP_DPU, kDPU_Vscaler4);
    scConfig.srcReg     = DPU_MAKE_SRC_REG1(APP_VSCALER_SRC);
    scConfig.inputSize  = APP_CAMERA_HEIGHT;
    scConfig.outputSize = APP_PANEL_HEIGHT;
    DPU_SetScalerConfig(APP_DPU, kDPU_Vscaler4, &scConfig);
#endif /* APP_RESIZE_CAMERA_IMAGE */
    /*
     * Fetch unit.
     */
    DPU_FetchUnitGetDefaultConfig(&fetchConfig);
#if APP_RESIZE_CAMERA_IMAGE
    fetchConfig.frameHeight = APP_CAMERA_HEIGHT;
    fetchConfig.frameWidth  = APP_CAMERA_WIDTH;
#else
    fetchConfig.frameHeight = APP_PANEL_HEIGHT;
    fetchConfig.frameWidth  = APP_PANEL_WIDTH;
#endif
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchYuv0, &fetchConfig);

    /* Fetch unit source buffer configure. */
    DPU_SrcBufferGetDefaultConfig(&sbConfig);

    sbConfig.bitsPerPixel = 16;
    sbConfig.pixelFormat  = kDPU_PixelFormatRGB565;
    sbConfig.constColor   = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);

    /* FetchYUV0 configuration. */
    sbConfig.strideBytes  = APP_PANEL_FB_STRIDE_BYTE;
    sbConfig.bufferHeight = APP_CAMERA_HEIGHT;
    sbConfig.bufferWidth  = APP_CAMERA_WIDTH;
    sbConfig.baseAddr     = frameBuffer;
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
    displayTimingConfig.vfp    = APP_VFP -1;
    displayTimingConfig.vbp    = APP_VBP +1;

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
        /* Init PASS */
        break;
    }
}


void APP_InitDisplay(uint32_t frameBuffer, app_display_callback_t callback)
{
    memset(&s_displayHandle, 0, sizeof(s_displayHandle));
    DPU_Init(APP_DPU);
    DPU_PreparePathConfig(APP_DPU);

    /* Display frame buffer ready, start to show. */
    APP_DPU_Display(frameBuffer);

    s_displayHandle.callback       = callback;
    s_displayHandle.activeFb       = frameBuffer;
    s_displayHandle.isFramePending = false;
}

void APP_SetDisplayFrameBuffer(uint32_t frameBuffer)
{
    DPU_SetFetchUnitSrcBufferAddr(APP_DPU, kDPU_FetchYuv0, 0, frameBuffer);
    s_displayHandle.isFramePending = true;
    s_displayHandle.inactiveFb     = frameBuffer;
    DPU_TriggerPipelineShadowLoad(APP_DPU, APP_CONTENT_STREAM_PIPELINE);
}

bool APP_IsDisplayFramePending(void)
{
    return s_displayHandle.isFramePending;
}

void APP_DisplayIRQHandler(void)
{
    uint32_t intGroup0Flags, intGroup1Flags;
    uint32_t oldActiveFb;
    intGroup0Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 0);
    intGroup1Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 1);

    DPU_ClearInterruptsPendingFlags(APP_DPU, 0, intGroup0Flags);
    DPU_ClearInterruptsPendingFlags(APP_DPU, 1, intGroup1Flags);

    if ((intGroup0Flags & APP_CONTENT_STREAM_INT_GROUP0) || (intGroup1Flags & APP_CONTENT_STREAM_INT_GROUP1))
    {
        s_isContentStreamShadowPending = false;
        s_displayHandle.isFramePending = false;

        DPU_ClearInterruptsPendingFlags(APP_DPU_DC, 0, intGroup0Flags);
        DPU_ClearInterruptsPendingFlags(APP_DPU_DC, 1, intGroup1Flags);
        oldActiveFb = s_displayHandle.activeFb;

        if (s_displayHandle.callback)
        {
            s_displayHandle.activeFb = s_displayHandle.inactiveFb;
            s_displayHandle.callback(oldActiveFb);
        }
    }
    SDK_ISR_EXIT_BARRIER;
}


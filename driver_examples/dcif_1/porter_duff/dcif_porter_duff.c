/*
 * Copyright 2026 NXP
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
#define DEMO_ALIGN_STRIDE(bytes) (((bytes) + (DCIF_FB_ALIGN - 1U)) & ~((uint32_t)DCIF_FB_ALIGN - 1U))

#define DEMO_IMG_HEIGHT DEMO_PANEL_HEIGHT
#define DEMO_IMG_WIDTH  DEMO_PANEL_WIDTH
/* Use ARGB8888 format, 4 bytes per pixel. */
#define DEMO_IMG_BYTES_PER_LINE DEMO_ALIGN_STRIDE(DEMO_IMG_WIDTH * 4U)

#define DEMO_MAKE_COLOR(a, r, g, b) (((a) << 24U) | ((r) << 16U) | ((g) << 8U) | ((b) << 0U))
#define DEMO_COLOR_RED              DEMO_MAKE_COLOR(0xFF, 0xFF, 0, 0)
#define DEMO_COLOR_BLUE             DEMO_MAKE_COLOR(0xFF, 0, 0, 0xFF)

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
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_srcFrameBuffer[DEMO_IMG_HEIGHT][DEMO_IMG_BYTES_PER_LINE],
    DCIF_FB_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_dstFrameBuffer[DEMO_IMG_HEIGHT][DEMO_IMG_BYTES_PER_LINE],
    DCIF_FB_ALIGN);
static uint32_t s_srcFrameBufferAddr = (uint32_t)s_srcFrameBuffer;
static uint32_t s_dstFrameBufferAddr = (uint32_t)s_dstFrameBuffer;
static uint32_t s_bufferWidth        = DEMO_IMG_WIDTH / 4U * 3U;
static uint32_t s_bufferHeight       = DEMO_IMG_HEIGHT / 4U * 3U;
static volatile bool s_frameDone     = false;
static uint32_t loops                = 0U;
static dcif_layer_porter_duff_blend_mode_t porterDuffModeArray[]    = {
    kDCIF_PorterDuffSrc,   kDCIF_PorterDuffAtop,   kDCIF_PorterDuffOver,    kDCIF_PorterDuffIn,
    kDCIF_PorterDuffOut,   kDCIF_PorterDuffDst,    kDCIF_PorterDuffDstAtop, kDCIF_PorterDuffDstOver,
    kDCIF_PorterDuffDstIn, kDCIF_PorterDuffDstOut, kDCIF_PorterDuffPlus,    kDCIF_PorterDuffXor,
    kDCIF_PorterDuffClear};
static char *porterDuffNameArray[] = {"PorterDuffSrc",   "PorterDuffAtop",   "PorterDuffOver",    "PorterDuffIn",
                                      "PorterDuffOut",   "PorterDuffDst",    "PorterDuffDstAtop", "PorterDuffDstOver",
                                      "PorterDuffDstIn", "PorterDuffDstOut", "PorterDuffPlus",    "PorterDuffXor",
                                      "PorterDuffClear"};

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

void DEMO_FillFrameBuffer(void)
{
    uint32_t i, j;
    uint32_t(*srcFb)[DEMO_IMG_WIDTH] = (void *)s_srcFrameBufferAddr;
    uint32_t(*dstFb)[DEMO_IMG_WIDTH] = (void *)s_dstFrameBufferAddr;

    memset(srcFb, 0, DEMO_IMG_WIDTH * DEMO_IMG_HEIGHT * 4);
    memset(dstFb, 0, DEMO_IMG_WIDTH * DEMO_IMG_HEIGHT * 4);

    /* Draw a solid red rectangle at the top left of the screen, then the rest is transparent. */
    for (i = 0; i < DEMO_IMG_HEIGHT; i++)
    {
        for (j = 0; j < DEMO_IMG_WIDTH; j++)
        {
            if ((i <= s_bufferHeight) && (j <= s_bufferWidth))
            {
                ((uint32_t *)(((uint8_t *)srcFb) + DEMO_IMG_BYTES_PER_LINE * i))[j] = DEMO_COLOR_RED;
            }
            else
            {
                ((uint32_t *)(((uint8_t *)srcFb) + DEMO_IMG_BYTES_PER_LINE * i))[j] = 0U;
            }
        }
    }

    /* Draw a solid blue rectangle at the bottom right of the screen, then the rest is transparent. */
    for (i = 0; i < DEMO_IMG_HEIGHT; i++)
    {
        for (j = 0; j < DEMO_IMG_WIDTH; j++)
        {
            if ((i >= (s_bufferHeight / 3U)) && (j >= (s_bufferWidth / 3U)))
            {
                ((uint32_t *)(((uint8_t *)dstFb) + DEMO_IMG_BYTES_PER_LINE * i))[j] = DEMO_COLOR_BLUE;
            }
            else
            {
                ((uint32_t *)(((uint8_t *)dstFb) + DEMO_IMG_BYTES_PER_LINE * i))[j] = 0U;
            }
        }
    }
}

void DEMO_DCIF_Init(void)
{
    dcif_output_config_t outputConfig = {
        .interface = DEMO_INTERFACE,
        .width     = DEMO_IMG_WIDTH,
        .height    = DEMO_IMG_HEIGHT,
    };

    DCIF_Init(DEMO_DCIF);

    /* Set panel output config. */
    DCIF_SetOutputConfig(DEMO_DCIF, &outputConfig);

    /* Init the specific interface type according to the panel used. */
    DEMO_InitInterface();

    /* Background black. */
    DCIF_SetBackGroundLayerColor(DEMO_DCIF, 0x0U);

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

void DEMO_DCIF_Porterduff(void)
{
    static dcif_layer_config_t dstLayerConfig = {0};
    static dcif_layer_config_t srcLayerConfig = {0};

    (void)memset(&dstLayerConfig, 0, sizeof(dstLayerConfig));
    (void)memset(&srcLayerConfig, 0, sizeof(srcLayerConfig));

    DEMO_FillFrameBuffer();

    /* Set the layer 0 config, it performs as the destination layer. */
    dstLayerConfig.enable           = true;
    dstLayerConfig.format           = kDCIF_LayerPixelFormatARGB8888;
    dstLayerConfig.topLeftX         = 0U;
    dstLayerConfig.topLeftY         = 0U;
    dstLayerConfig.width            = DEMO_IMG_WIDTH;
    dstLayerConfig.height           = DEMO_IMG_HEIGHT;
    dstLayerConfig.background       = 0U;
    dstLayerConfig.panic.enable     = false;
    dstLayerConfig.globalAlpha      = 0xFFU;
    dstLayerConfig.alphaBlendMode   = kDCIF_AlphaBlendPorterDuff;
    DCIF_GetPorterDuffConfig(kDCIF_PorterDuffSrc, &(dstLayerConfig.alphaBlendConfig));

    DCIF_SetLayerAddr(DEMO_DCIF, 0, (uint32_t)s_dstFrameBufferAddr);
    DCIF_SetLayerStride(DEMO_DCIF, 0, DEMO_IMG_BYTES_PER_LINE);
    DCIF_SetLayerConfig(DEMO_DCIF, 0, &dstLayerConfig);

    /* Set the overlay layer 0 config, it performs as the source layer. */
    srcLayerConfig.enable           = true;
    srcLayerConfig.format           = kDCIF_LayerPixelFormatARGB8888;
    srcLayerConfig.topLeftX         = 0U;
    srcLayerConfig.topLeftY         = 0U;
    srcLayerConfig.width            = DEMO_IMG_WIDTH;
    srcLayerConfig.height           = DEMO_IMG_HEIGHT;
    srcLayerConfig.background       = 0U;
    srcLayerConfig.panic.enable     = false;
    srcLayerConfig.globalAlpha      = 0xFFU;
    srcLayerConfig.alphaBlendMode   = kDCIF_AlphaBlendPorterDuff;

    DCIF_GetPorterDuffConfig(porterDuffModeArray[loops % ARRAY_SIZE(porterDuffModeArray)], &(srcLayerConfig.alphaBlendConfig));

    DCIF_SetLayerAddr(DEMO_DCIF, 1, (uint32_t)s_srcFrameBufferAddr);
    DCIF_SetLayerStride(DEMO_DCIF, 1, DEMO_IMG_BYTES_PER_LINE);
    DCIF_SetLayerConfig(DEMO_DCIF, 1, &srcLayerConfig);

    PRINTF("Currently show %s mode\r\n\r\n", porterDuffNameArray[loops % ARRAY_SIZE(porterDuffNameArray)]);

    /* Two layers are configured (destination on layer 0, source overlay on
     * layer 1); shadow-load both so their initial configuration takes effect. */
    DCIF_TriggerLayerShadowLoad(DEMO_DCIF, 0);
    DCIF_TriggerLayerShadowLoad(DEMO_DCIF, 1);

#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DBI
    DEMO_DbiSelectUpdateArea(0U, 0U, DEMO_IMG_WIDTH - 1U, DEMO_IMG_HEIGHT - 1U);
    DEMO_UpdateFrame(0U);
#endif

    /* Frame done interrupt. */
    DCIF_EnableInterrupts(DEMO_DCIF, DEMO_DOMAIN, DEMO_INTERFACE_FRAME_DONE_INTERRUPT);

    while (1)
    {
        /* Wait for previous frame complete. */
        while (!s_frameDone)
        {
        }
        loops++;

        s_frameDone = false;

        /* Let the picture be displayed for 2s then change. */
        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        PRINTF("Currently show %s mode\r\n\r\n", porterDuffNameArray[loops % ARRAY_SIZE(porterDuffNameArray)]);

        DCIF_GetPorterDuffConfig(porterDuffModeArray[loops % ARRAY_SIZE(porterDuffModeArray)], &(srcLayerConfig.alphaBlendConfig));

        DCIF_SetLayerConfig(DEMO_DCIF, 1, &srcLayerConfig);

        /* The Porter-Duff mode changed on the source layer; shadow-load layer 1
         * so the new blend configuration is latched on the next VSYNC.
         * DEMO_UpdateFrame() only reloads layer 0. */
        DCIF_TriggerLayerShadowLoad(DEMO_DCIF, 1);

        /* Update frame according to the interface type. */
        DEMO_UpdateFrame(0U);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    BOARD_InitDcifPowerClockReset();

    PRINTF("DCIF porter duff example start...\r\n");

    DEMO_DCIF_Init();

    DEMO_DCIF_Porterduff();

    while (1)
    {
    }
}

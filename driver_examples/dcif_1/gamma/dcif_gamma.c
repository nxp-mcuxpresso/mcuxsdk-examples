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
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_frameBuffer[DEMO_IMG_HEIGHT][DEMO_IMG_BYTES_PER_LINE],
    DCIF_FB_ALIGN);
AT_NONCACHEABLE_SECTION_INIT(static uint32_t gammaTable[FSL_FEATURE_DCIF_GAMMA_INDEX_MAX]) = {0};
AT_NONCACHEABLE_SECTION_INIT(static volatile bool s_frameDone)                 = false;

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
    uint8_t(*frameBuffer)[DEMO_IMG_BYTES_PER_LINE] = s_frameBuffer;
    uint8_t element;

    /* Fill the buffer with gradual changed gray bars. */
    for (uint32_t i = 0; i < DEMO_IMG_HEIGHT; i++)
    {
        for (uint32_t j = 0; j < DEMO_IMG_WIDTH; j++)
        {
            *((uint32_t *)&frameBuffer[i][j * 4]) = ((j & 0xFFU) << 16U) | ((j & 0xFFU) << 8U) | ((j & 0xFFU));
        }
    }

    /*
     * In this example, the gamma correction inverts the original picture.
     */
    for (uint32_t i = 0; i < FSL_FEATURE_DCIF_GAMMA_INDEX_MAX; i++)
    {
        element       = 255U - i;
        gammaTable[i] = (element << 16) | (element << 8) | (element << 0);
    }
}

void DEMO_DCIF_Init(void)
{
    DCIF_Init(DEMO_DCIF);

    static dcif_output_config_t outputConfig = {
        .interface = DEMO_INTERFACE,
        .width     = DEMO_IMG_WIDTH,
        .height    = DEMO_IMG_HEIGHT,
    };

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

void DEMO_DCIF_Gamma(void)
{
    bool enableGamma = false;
    dcif_layer_config_t config;

    DEMO_FillFrameBuffer();

    DCIF_SetGammaData(DEMO_DCIF, kDCIF_GammaAccessBluetable | kDCIF_GammaAccessGreentable | kDCIF_GammaAccessRedtable,
        0U, gammaTable, FSL_FEATURE_DCIF_GAMMA_INDEX_MAX);

    memset(&config, 0, sizeof(config));

    config.enable           = true;
    config.format           = kDCIF_LayerPixelFormatARGB8888;
    config.topLeftX         = 0U;
    config.topLeftY         = 0U;
    config.width            = DEMO_IMG_WIDTH;
    config.height           = DEMO_IMG_HEIGHT;
    config.background       = 0U;
    /* Enable FIFO panic so the DCIF raises its AXI fetch priority when the layer
     * FIFO runs low, reducing underrun (which the inverting gamma shows as a
     * bright/white panel) when the PSRAM framebuffer read is briefly starved.
     * Layer 0 FIFO is 512 words deep: assert panic below ~1/3, clear above ~2/3. */
    config.panic.enable     = true;
    config.panic.lowLevel   = 170U;
    config.panic.highLevel  = 340U;
    config.globalAlpha      = 0xFFU;
    /* The ARGB8888 gradient carries a zero alpha, so override the pixel alpha
     * with the opaque global alpha instead of blending it away. */
    config.alphaBlendMode   = kDCIF_AlphaBlendOverride;

    DCIF_SetLayerAddr(DEMO_DCIF, 0, (uint32_t)s_frameBuffer);

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
        /*
         * Wait for previous frame complete.
         * Interrupt happens when the last pixel sent out. New frame buffer configuration
         * load at the next VSYNC.
         */
        while (!s_frameDone)
        {
        }

        s_frameDone = false;

        /* Toggle gamma correction each frame to alternate between the original
         * picture and the gamma-inverted picture. */
        enableGamma = !enableGamma;
        DCIF_EnableGamma(DEMO_DCIF, enableGamma);

        /* Let the picture be displayed for 2s then change. */
        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Update frame according to the interface type. */
        DEMO_UpdateFrame((uint32_t)s_frameBuffer);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    BOARD_InitDcifPowerClockReset();

    PRINTF("DCIF gamma example start...\r\n");

    DEMO_DCIF_Init();

    DEMO_DCIF_Gamma();

    while (1)
    {
    }
}
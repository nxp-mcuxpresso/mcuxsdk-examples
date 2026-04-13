/*
 * Copyright 2025 NXP
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
#define DEMO_INPUT_RGB565 0
#define DEMO_INPUT_YUV422 1
#define DEMO_INPUT_INDEX  2

#define DEMO_INPUT_BUFFER_FORMAT DEMO_INPUT_RGB565

#if DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_INDEX
#define DEMO_IMG_HEIGHT DEMO_PANEL_HEIGHT
#define DEMO_IMG_WIDTH  DEMO_PANEL_WIDTH
/* 4 bit per pixel, 2 pixels take 1 byte. */
#define DEMO_IMG_BYTES_PER_LINE (DEMO_IMG_WIDTH / 2U)
#else
#define DEMO_IMG_HEIGHT DEMO_PANEL_HEIGHT / 2
#define DEMO_IMG_WIDTH  DEMO_PANEL_WIDTH / 2
/* RGB565 or YUV422, 2 bytes per pixel. */
#define DEMO_IMG_BYTES_PER_LINE (DEMO_IMG_WIDTH * 2U)
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
static uint32_t s_frameBufferAddr[2] = {DEMO_FB0_ADDR, DEMO_FB1_ADDR};
static volatile bool s_frameDone     = false;

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
    static const uint16_t fgColorTable[] = {0x001FU, 0x07E0U, 0x07FFU, 0xF800U, 0xF81FU, 0xFFE0U, 0xFFFFU};
#elif DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_YUV422
    static const uint32_t fgColorTable[] = {0x1DFF1D6BU, 0x952B9515U, 0xB2ABB200U,
        0x4C544CFFU, 0x69D469EAU, 0xE100E194U, 0xFF80FF80U};
#else
    static const uint8_t fgColorTable[] = {1U, 2U, 3U, 4U, 5U, 6U, 7U};
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
            *((uint16_t *)&frameBuffer[i][j * 2]) = fgColor;
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
    static const uint32_t fgColorTable[] = {0U, 0xFF0000FFU, 0xFF00FF00U, 0xFF00FFFFU,
                                        0xFFFF0000U, 0xFFFF00FFU, 0xFFFFFF00U, 0xFFFFFFFFU};

    /* CLUT for indexed input format. */
    DCIF_ClutGetDefaultConfig(&clutConfig);
    DCIF_ClutSetConfig(DEMO_DCIF, &clutConfig);
    DCIF_ClutSetData(DEMO_DCIF, 0U, fgColorTable, 8U);
#endif

    /* Enable DCIF. */
    DCIF_EnableOutput(DEMO_DCIF, true);

    /* Initialize panel. */
    if (kStatus_Success != BOARD_InitDisplayInterface())
    {
        PRINTF("Display panel initialize failed\r\n");

        while (1)
        {
        }
    }

    NVIC_EnableIRQ(DEMO_DCIF_IRQn);
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
#elif DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_YUV422
    config.format           = kDCIF_LayerPixelFormatVYUY;
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
    config.alphaBlendMode   = kDCIF_AlphaBlendEmbedded;

    DCIF_SetLayerAddr(DEMO_DCIF, 0, (uint32_t)s_frameBufferAddr[frameBufferIndex]);

    DCIF_SetLayerStride(DEMO_DCIF, 0, DEMO_IMG_BYTES_PER_LINE);

#if DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_YUV422
    DCIF_SetCscMode(DEMO_DCIF, 0U, kDCIF_CscYUV2RGB);
#endif

    DCIF_SetLayerConfig(DEMO_DCIF, 0U, &config);

#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DPI
    /* Only DPI needs trigger shadow load. */
    DCIF_TriggerLayerShadowLoad(DEMO_DCIF, 0);
#elif DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DBI
    /* For DBI interface, need to send the frame update area command first. */
    uint8_t cmdParam[4];

    /* Select the panel region used. */
    cmdParam[0] = (uint8_t)((DEMO_BUFFER_START_X >> 8U) & 0xFFU);
    cmdParam[1] = (uint8_t)(DEMO_BUFFER_START_X & 0xFFU);
    cmdParam[2] = (uint8_t)((DEMO_BUFFER_END_X >> 8U) & 0xFFU);
    cmdParam[3] = (uint8_t)(DEMO_BUFFER_END_X & 0xFFU);
    DCIF_DbiWriteCommand(DEMO_DCIF, DBI_CMD_SET_COLUMN_ADDRESS);

    while ((DEMO_DCIF->DBI_CTRL & DCIF_DBI_CTRL_DBI_CMD_TRIG_MASK) != 0U)
    {
        /* Wait for the command to be completed. */
    }

    DCIF_DbiWriteParam(DEMO_DCIF, cmdParam, 4U);

    while ((DEMO_DCIF->DBI_CTRL & DCIF_DBI_CTRL_DBI_CMD_TRIG_MASK) != 0U)
    {
        /* Wait for the command to be completed. */
    }

    cmdParam[0] = (uint8_t)((DEMO_BUFFER_START_Y >> 8U) & 0xFFU);
    cmdParam[1] = (uint8_t)(DEMO_BUFFER_START_Y & 0xFFU);
    cmdParam[2] = (uint8_t)((DEMO_BUFFER_END_Y >> 8U) & 0xFFU);
    cmdParam[3] = (uint8_t)(DEMO_BUFFER_END_Y & 0xFFU);
    DCIF_DbiWriteCommand(DEMO_DCIF, DBI_CMD_SET_PAGE_ADDRESS);

    while ((DEMO_DCIF->DBI_CTRL & DCIF_DBI_CTRL_DBI_CMD_TRIG_MASK) != 0U)
    {
        /* Wait for the command to be completed. */
    }

    DCIF_DbiWriteParam(DEMO_DCIF, cmdParam, 4U);

    while ((DEMO_DCIF->DBI_CTRL & DCIF_DBI_CTRL_DBI_CMD_TRIG_MASK) != 0U)
    {
        /* Wait for the command to be completed. */
    }
#endif

    /* Frame done interrupt. */
    DCIF_EnableInterrupts(DEMO_DCIF, DEMO_DOMAIN, DEMO_INTERFACE_FRAME_DONE_INTERRUPT);

    while (1)
    {
#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DPI
        /* Only DPI need 2 frame buffers. */
        frameBufferIndex ^= 1U;
#endif
        /*
         * Wait for previous frame complete.
         * Interrupt happens when the last pixel sent out. New frame buffer configuration
         * load at the next VSYNC.
         */
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
#elif DEMO_INPUT_BUFFER_FORMAT == DEMO_INPUT_YUV422
    PRINTF("The input format is YUV422\r\n");
#else
    PRINTF("The input format is Index\r\n");
#endif

    DEMO_DCIF_Init();

    DEMO_DCIF_RUN();

    while (1)
    {
    }
}

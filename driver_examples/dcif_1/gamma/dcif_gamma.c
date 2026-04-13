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
#define DEMO_IMG_HEIGHT         DEMO_PANEL_HEIGHT / 2U
#define DEMO_IMG_WIDTH          DEMO_PANEL_WIDTH / 2U
/* Use ARGB8888 format, 4 bytes per pixel. */
#define DEMO_IMG_BYTES_PER_LINE (DEMO_IMG_WIDTH * 4U)

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
    uint8_t(*frameBuffer)[DEMO_IMG_BYTES_PER_LINE] = (void *)DEMO_FB0_ADDR;
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

void DEMO_DCIF_Gamma(void)
{
    bool enableGamma = true;
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
    config.panic.enable     = false;
    config.globalAlpha      = 0xFFU;
    config.alphaBlendMode   = kDCIF_AlphaBlendEmbedded;

    DCIF_SetLayerAddr(DEMO_DCIF, 0, DEMO_FB0_ADDR);

    DCIF_SetLayerStride(DEMO_DCIF, 0, DEMO_IMG_BYTES_PER_LINE);

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
        /*
         * Wait for previous frame complete.
         * Interrupt happens when the last pixel sent out. New frame buffer configuration
         * load at the next VSYNC.
         */
        while (!s_frameDone)
        {
        }

        s_frameDone = false;
        DCIF_EnableGamma(DEMO_DCIF, !enableGamma);

        /* Update frame according to the interface type. */
        DEMO_UpdateFrame(DEMO_FB0_ADDR);
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
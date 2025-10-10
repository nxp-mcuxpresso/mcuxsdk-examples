/*
 * Copyright 2025 NXP
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

#define DSI_RED         0xFFFF0000
#define DSI_GREEN       0xFF00FF00
#define DSI_BLUE        0xFF0000FF
#define DSI_WHITE       0xFFFFFFFF
#define DSI_BLACK       0xFF000000
#define DSI_YELLOW      0xFFFFFF00
#define DSI_LIGHTGREEN  0xFF90EE90
#define DSI_PINK        0xFFFFC0CB
#define DSI_LIGHTBLUE   0xFF87CEFA
#define DSI_LIGHTGRAY   0xFFD3D3D3
#define DSI_ORANGE      0xFFFFA500
#define DSI_PURPLE      0xFF800080
#define DSI_BROWN       0xFFA52A2A
#define DSI_GOLD        0xFFFFD700
#define DSI_CYAN        0xFF00FFFF
#define DSI_NAVY        0xFF000080

#define DEMO_BYTE_PER_PIXEL 4U
#define DEMO_BG_LAYER       0
#define DEMO_BG_HEIGHT      400
#define DEMO_BG_WIDTH       400
#define DEMO_BG_BYTES_PER_LINE (DEMO_BG_WIDTH / 2)
#define DEMO_BG_FB0_ADDR DEMO_OCRAM_ADDR
#define DEMO_BG_FB1_ADDR (DEMO_OCRAM_ADDR + DEMO_BG_BYTES_PER_LINE * DEMO_BG_HEIGHT)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t s_ocramBufferAddr[2] = { DEMO_BG_FB0_ADDR, DEMO_BG_FB1_ADDR };

static uint32_t clutInitArray[16] = {DSI_RED, DSI_GREEN, DSI_BLUE, DSI_WHITE,
                                DSI_BLACK, DSI_YELLOW, DSI_LIGHTGREEN, DSI_PINK,
                                DSI_LIGHTBLUE, DSI_LIGHTGRAY, DSI_ORANGE, DSI_PURPLE,
                                DSI_BROWN, DSI_GOLD, DSI_CYAN, DSI_NAVY};
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

void DEMO_DCIF_InitCLUT(void)
{
    uint32_t i;
    uint32_t *cluts = (uint32_t *)(DCIF_BASE + 0x10c00);

    for (i = 0; i < 16; i++)
    {
        cluts[i] = clutInitArray[i];
    }
}

void DEMO_FillColor4bpp(uint32_t index, uint32_t size, uint8_t cindex)
{
    uint32_t *fb = (void *)s_ocramBufferAddr[index];
    uint8_t val = (cindex & 0xF) << 4 | (cindex & 0xF);
    memset((void *)fb, val, size);
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

    const dcif_buffer_config_t bgConfig = {
        .strideBytes = DEMO_BG_WIDTH / 2U,
        .pixelFormat = kDCIF_PixelFormat4bpp,
        .globalAlpha = 0xFF,
    };

    BOARD_InitDisplayInterface();

    DCIF_Init(DEMO_DCIF);

    DCIF_SetDisplayConfig(DEMO_DCIF, &dcifConfig);

    DCIF_EnableDisplay(DEMO_DCIF, true);

    /* configure for background layer */
    DCIF_SetLayerBufferConfig(DEMO_DCIF, DEMO_BG_LAYER, &bgConfig);

    /* background layer size 400 x 400 in middle */
    DCIF_SetLayerPosition(DEMO_DCIF, DEMO_BG_LAYER, (DEMO_PANEL_HEIGHT - DEMO_BG_HEIGHT) / 2U, (DEMO_PANEL_WIDTH - DEMO_BG_WIDTH) / 2U);

    DCIF_SetLayerSize(DEMO_DCIF, DEMO_BG_LAYER, DEMO_BG_WIDTH, DEMO_BG_HEIGHT);

    DCIF_EnableLayer(DEMO_DCIF, DEMO_BG_LAYER, true);
    
    EnableIRQ(DEMO_DCIF_IRQ);

    DCIF_EnableInterrupts(DEMO_DCIF, kDCIF_VerticalBlankingInterrupt);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t bindex = 0;
    uint8_t cindex = 0;
    BOARD_InitHardware();

    PRINTF("DCIF CLUT 4bpp example start...\r\n");

    PRINTF("DCIF CLUT 4bpp init DCIF ...\r\n");
    DEMO_DCIF_Init();

    PRINTF("DCIF CLUT 4bpp init CLUT ...\r\n");
    DEMO_DCIF_InitCLUT();

    PRINTF("DCIF CLUT 4bpp Fill Layer0 ...\r\n");
    DEMO_FillColor4bpp(bindex, DEMO_BG_BYTES_PER_LINE * DEMO_BG_HEIGHT, cindex++);

    while (1)
    {
        /* Set current display buffer addr */
        DCIF_SetLayerBufferAddr(DEMO_DCIF, DEMO_BG_LAYER, (uint32_t)s_ocramBufferAddr[bindex]);
        DCIF_TriggerLayerShadowLoad(DEMO_DCIF, DEMO_BG_LAYER);
        /*
         * Wait for previous frame complete.
         * New frame buffer configuration load at the next VSYNC.
         */
        s_frameDone = false;
        while (!s_frameDone)
        {
        }
	/* fill free buffer */
        DEMO_FillColor4bpp(!bindex, DEMO_BG_BYTES_PER_LINE * DEMO_BG_HEIGHT, cindex++);
        bindex = !bindex;
	if (cindex >= 16)
        {
            cindex = 0;
        }
        SDK_DelayAtLeastUs(1000000U, SystemCoreClock);
    }
}

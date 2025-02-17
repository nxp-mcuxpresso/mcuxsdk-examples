/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_rgpio.h"
#include "display_support.h"
#include "fsl_debug_console.h"
#include "fsl_dc_fb_dcif.h"

/*******************************************************************************
 * Codes
 ******************************************************************************/

void BOARD_InitDisplayInterface(void)
{
    hal_clk_t hal_ldbpllvcoCLKCfg = {
        .clk_id = hal_clock_ldbpllctl,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 2986200000UL,
    };

    hal_clk_t hal_ldbpllCLKCfg = {
        .clk_id = hal_clock_ldbpll,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 497700000UL,
    };
    HAL_ClockSetRate(&hal_ldbpllvcoCLKCfg);
    HAL_ClockEnable(&hal_ldbpllvcoCLKCfg);
    HAL_ClockSetRate(&hal_ldbpllCLKCfg);
    HAL_ClockEnable(&hal_ldbpllCLKCfg);

    /* Select LVDS0 pin by using PCAL6416 */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, LVDS_EN);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, LVDS_RESET_B);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C3_S5_21_ID, LVDS_EN);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C3_S5_21_ID, LVDS_RESET_B);

    ((dc_fb_dcif_config_t *)g_dc.config)->hsw    = DEMO_HSW;
    ((dc_fb_dcif_config_t *)g_dc.config)->hfp    = DEMO_HFP;
    ((dc_fb_dcif_config_t *)g_dc.config)->hbp    = DEMO_HBP;
    ((dc_fb_dcif_config_t *)g_dc.config)->vsw    = DEMO_VSW;
    ((dc_fb_dcif_config_t *)g_dc.config)->vfp    = DEMO_VFP;
    ((dc_fb_dcif_config_t *)g_dc.config)->vbp    = DEMO_VBP;

    BLK_CTRL_DISPLAYMIX->DISPLAY_ENGINES_CLOCK_CONTROL |= DISPLAY_BLK_CTRL_DISPLAYMIX_DISPLAY_ENGINES_CLOCK_CONTROL_DSIP_CLK0_SEL(0x1);
    BLK_CTRL_LVDS->PIXEL_MAPPER_CONTROL = 0U;
    BLK_CTRL_LVDS->PIXEL_MAPPER_CONTROL |= DISPLAY_BLK_CTRL_LVDS_PIXEL_MAPPER_CONTROL_CH0_MODE(0x1) | DISPLAY_BLK_CTRL_LVDS_PIXEL_MAPPER_CONTROL_CH0_DATA_WIDTH(0x1)
                                        | DISPLAY_BLK_CTRL_LVDS_PIXEL_MAPPER_CONTROL_ASYNC_FIFO_THRESHOLD(0x1);
    BLK_CTRL_LVDS->LVDS0_CONTROL = 0U;
    BLK_CTRL_LVDS->LVDS0_CONTROL |= DISPLAY_BLK_CTRL_LVDS_LVDS0_CONTROL_CH0_EN(0x1) | DISPLAY_BLK_CTRL_LVDS_LVDS0_CONTROL_BG_EN(0x1);

}

dc_fb_dcif_handle_t s_dcFbDcifHandle = {0}; /* The handle must be initialized to 0. */
dc_fb_dcif_config_t s_dcFbDcifConfig = {
    .dcif          = DEMO_DCIF,
    .polarityFlags = DEMO_POL_FLAGS,
    .lineOrder     = kDCIF_LineOrderRGB,
    .width         = DEMO_PANEL_WIDTH,
    .height        = DEMO_PANEL_HEIGHT,
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsDcif,
    .prvData = &s_dcFbDcifHandle,
    .config  = &s_dcFbDcifConfig,
};

void DEMO_DCIF_IRQHandler(void)
{
    DC_FB_DCIF_IRQHandler(&g_dc);
}

void BOARD_PrepareDisplayController(void)
{

    BOARD_InitDisplayInterface();
    NVIC_ClearPendingIRQ(DEMO_DCIF_IRQ);
    NVIC_SetPriority(DEMO_DCIF_IRQ, 3);
    EnableIRQ(DEMO_DCIF_IRQ);
}
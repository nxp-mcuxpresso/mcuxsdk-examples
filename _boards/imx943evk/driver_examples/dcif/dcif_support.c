/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_rgpio.h"
#include "dcif_support.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Codes
 ******************************************************************************/

void BOARD_InitDisplayInterface(void)
{
    clk_t ldbpllvcoCLKCfg = {
        .clkId = kCLOCK_Ldbpllctl,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 2986200000UL,
    };

    clk_t ldbpllCLKCfg = {
        .clkId = kCLOCK_Ldbpll,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 497700000UL,
    };
    CLOCK_SetRate(&ldbpllvcoCLKCfg);
    CLOCK_EnableClock(ldbpllvcoCLKCfg.clkId);
    CLOCK_SetRate(&ldbpllCLKCfg);
    CLOCK_EnableClock(ldbpllCLKCfg.clkId);

    /* Select LVDS0 pin by using PCAL6416 */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, LVDS_EN);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, LVDS_RESET_B);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C3_S5_21_ID, LVDS_EN);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C3_S5_21_ID, LVDS_RESET_B);

    BLK_CTRL_DISPLAYMIX->DISPLAY_ENGINES_CLOCK_CONTROL |= DISPLAY_BLK_CTRL_DISPLAYMIX_DISPLAY_ENGINES_CLOCK_CONTROL_DSIP_CLK0_SEL(0x1);
    BLK_CTRL_LVDS->PIXEL_MAPPER_CONTROL = 0U;
    BLK_CTRL_LVDS->PIXEL_MAPPER_CONTROL |= DISPLAY_BLK_CTRL_LVDS_PIXEL_MAPPER_CONTROL_CH0_MODE(0x1) | DISPLAY_BLK_CTRL_LVDS_PIXEL_MAPPER_CONTROL_CH0_DATA_WIDTH(0x1)
                                        | DISPLAY_BLK_CTRL_LVDS_PIXEL_MAPPER_CONTROL_ASYNC_FIFO_THRESHOLD(0x1);
    BLK_CTRL_LVDS->LVDS0_CONTROL = 0U;
    BLK_CTRL_LVDS->LVDS0_CONTROL |= DISPLAY_BLK_CTRL_LVDS_LVDS0_CONTROL_CH0_EN(0x1) | DISPLAY_BLK_CTRL_LVDS_LVDS0_CONTROL_BG_EN(0x1);

}

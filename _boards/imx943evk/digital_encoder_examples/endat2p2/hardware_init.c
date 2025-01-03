/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
/*${header:end}*/

#define DIG_ENCODER_MUX_HIPERFACE_DSL   0x0
#define DIG_ENCODER_MUX_ENDAT2P2        0x1
#define DIG_ENCODER_MUX_ENDAT3          0x2
#define DIG_ENCODER_NUX_BISS            0x3

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* EnDat2.2 100MHz */
    hal_clk_t hal_endat2p2Clk = {
        .clk_id = hal_clock_endat21,
        .pclk_id = hal_clock_syspll1dfs1div2, /* 400 MHz */
        .div = 4,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    BLK_CTRL_WAKEUPMIX_Type *blk_ctrl = BLK_CTRL_WAKEUPMIX;

    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRootClk(&hal_endat2p2Clk);

    blk_ctrl->DIAG_ENCODER_MUX_SEL =
        BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc1_sel(DIG_ENCODER_MUX_ENDAT2P2) |
        BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc2_sel(DIG_ENCODER_MUX_ENDAT2P2);
}
/*${function:end}*/
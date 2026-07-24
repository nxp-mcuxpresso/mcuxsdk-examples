/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitUSDHC0Pins();
    BOARD_InitUSDHC1Pins();
    /* MIMXRT2660-EVK ships two LPI2C0 IO expanders driving SDHC pins: U23 PCAL6524
     * for SD_PWREN and a second PCA9555 for card-detect / EXP_nIRQ2. Both pad-mux
     * helpers live in pin_mux.c; their I2C SDA/SCL muxes overlap (idempotent),
     * and each adds its own INT pin (PIO2_27 for PCAL6524, PIO3_29 for PCA9555). */
    BOARD_Init6524Pins();
    BOARD_InitPCA9555Pins();
}
/*${function:end}*/

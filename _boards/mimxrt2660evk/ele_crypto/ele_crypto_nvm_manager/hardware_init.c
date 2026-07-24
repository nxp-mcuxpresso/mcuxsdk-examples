/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitUSDHC0Pins();
    /* MIMXRT2660-EVK ships two LPI2C0 IO expanders driving SDHC pins: U23
     * PCAL6524 for SD_PWREN and a second PCA9555 for card-detect / EXP_nIRQ2.
     * Same pattern as sdmmc_examples/sdcard_fatfs on this board. */
    BOARD_Init6524Pins();
    BOARD_InitPCA9555Pins();
}
/*${function:end}*/

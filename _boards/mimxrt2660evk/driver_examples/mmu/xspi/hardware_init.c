/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
/* fsl_llc.h has non-static function bodies causing ODR errors when included twice.
 * Forward-declare only what we need; LLC_Type and CMPT__LLC come from device header via board.h. */
extern int LLC_CleanInvalidateCache(LLC_Type *base);
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitXSPI1PsRamPins();
}

void DEMO_CleanInvalidateL2Cache(void)
{
    /* RT2660 LLC (Last-Level Cache) clean+invalidate for AHB coherency */
    (void)LLC_CleanInvalidateCache(CMPT__LLC);
}
/*${function:end}*/

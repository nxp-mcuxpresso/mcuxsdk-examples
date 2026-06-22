/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
/*
 * Enable the inter-core MU2 peripheral clock in MC_ME.
 *
 * The primary core (M7_0) already gates both MU2 sides before releasing this
 * core, so this call is normally redundant, but the write is idempotent and is
 * kept here so the secondary image is self-contained.
 *
 * Both MU2 sides are gated by PRTN0_COFB1_CLKEN:
 *   MU_2_MUA -> PRTN0_COFB1_CLKEN[REQ46] (bit 14)
 *   MU_2_MUB -> PRTN0_COFB1_CLKEN[REQ47] (bit 15)
 * The clock-enable change is committed through the CTL_KEY 0x5AF0/0xA50F
 * handshake, then we poll PRTN0_COFB1_STAT until both blocks report enabled.
 */
void APP_EnableMu2Clock(void)
{
    MC_ME->PRTN0_COFB1_CLKEN |=
        (MC_ME_PRTN0_COFB1_CLKEN_REQ46_MASK | MC_ME_PRTN0_COFB1_CLKEN_REQ47_MASK);
    MC_ME->PRTN0_PUPD |= MC_ME_PRTN0_PUPD_PCUD_MASK;
    MC_ME->CTL_KEY = MC_ME_CTL_KEY_KEY(0x5AF0U);
    MC_ME->CTL_KEY = MC_ME_CTL_KEY_KEY(0xA50FU);
    while ((MC_ME_PRTN0_COFB1_STAT_BLOCK46_MASK | MC_ME_PRTN0_COFB1_STAT_BLOCK47_MASK) !=
           (MC_ME->PRTN0_COFB1_STAT &
            (MC_ME_PRTN0_COFB1_STAT_BLOCK46_MASK | MC_ME_PRTN0_COFB1_STAT_BLOCK47_MASK)))
    {
    }
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    /* Make sure the MU2_B clock is gated on before this core touches MU2. */
    APP_EnableMu2Clock();
}
/*${function:end}*/

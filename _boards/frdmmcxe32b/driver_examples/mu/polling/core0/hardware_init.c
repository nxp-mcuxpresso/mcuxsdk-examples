/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
/*
 * Enable the inter-core MU2 peripheral clock in MC_ME.
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

/*
 * Release the secondary core (M7_1) so it boots and runs in place from flash.
 *
 * The secondary image is embedded in the primary flash image and linked to
 * CORE1_BOOT_ADDRESS (0x00600000). M7_1 boots directly from flash; there is no
 * copy to RAM and no TCM backdoor. MC_ME start sequence:
 *   1. Write the secondary core reset-vector base to PRTN0_CORE1_ADDR. Hardware
 *      fetches the initial SP/PC from this flash address.
 *   2. Set CCE=1 in PRTN0_CORE1_PCONF to enable the core clock.
 *   3. Set CCUPD=1 in PRTN0_CORE1_PUPD to flag a pending update.
 *   4. Write CTL_KEY 0x5AF0 then 0xA50F to commit the pending changes.
 *   5. Poll PRTN0_CORE1_PUPD.CCUPD until the update completes.
 */
void APP_BootCore1(void)
{
    MC_ME->PRTN0_CORE1_ADDR  = (uint32_t)CORE1_BOOT_ADDRESS & MC_ME_PRTN0_CORE1_ADDR_ADDR_MASK;
    MC_ME->PRTN0_CORE1_PCONF |= MC_ME_PRTN0_CORE1_PCONF_CCE_MASK;
    MC_ME->PRTN0_CORE1_PUPD  |= MC_ME_PRTN0_CORE1_PUPD_CCUPD_MASK;
    MC_ME->CTL_KEY = MC_ME_CTL_KEY_KEY(0x5AF0U);
    MC_ME->CTL_KEY = MC_ME_CTL_KEY_KEY(0xA50FU);
    while (0U != (MC_ME->PRTN0_CORE1_PUPD & MC_ME_PRTN0_CORE1_PUPD_CCUPD_MASK))
    {
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootClocks();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();

    /* Gate on the inter-core MU2 peripheral clock; otherwise the first MU2
     * register access in main() takes a precise bus fault. */
    APP_EnableMu2Clock();
}


/*${function:end}*/

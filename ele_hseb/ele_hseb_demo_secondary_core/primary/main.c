/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"

/**
 * @brief Start the secondary core that will run the application
 *
 * This function is taken from the E32B implementation of mcmgr_start_core_internal()
 * as a minimal function to kick off the secondary core.
 */
void start_secondary_core(void)
{
    /*
     * Release M7_1 (secondary, kMCMGR_Core1) from reset via the MC_ME
     * PRTN0_CORE1 interface. This is the standard S32K3/MCXE dual-core
     * start sequence documented in the MCXE32B Reference Manual (MC_ME chapter):
     *
     *  1. Write the secondary core reset-vector base address to PRTN0_CORE1_ADDR.
     *     The core fetches its IVT from this address after reset release.
     *  2. Set CCE=1 in PRTN0_CORE1_PCONF to request the core clock.
     *  3. Set CCUPD=1 in PRTN0_CORE1_PUPD to flag a pending update.
     *  4. Write CTL_KEY = 0x5AF0 (first phase) then 0xA50F (second phase) to
     *     commit all pending PCONF/PUPD changes and release the core.
     *  5. Poll PRTN0_CORE1_PUPD.CCUPD until 0 to confirm the pending update has
     *     been consumed and the core has been released from reset.
     *
     * PRTN0_CORE0 maps to M7_0 (primary,   kMCMGR_Core0) - already running.
     * PRTN0_CORE1 maps to M7_1 (secondary, kMCMGR_Core1).
     */
    MC_ME->PRTN0_CORE1_ADDR = (uint32_t)CORE1_BOOT_ADDRESS & MC_ME_PRTN0_CORE1_ADDR_ADDR_MASK;
    MC_ME->PRTN0_CORE1_PCONF |= MC_ME_PRTN0_CORE1_PCONF_CCE_MASK;
    MC_ME->PRTN0_CORE1_PUPD  |= MC_ME_PRTN0_CORE1_PUPD_CCUPD_MASK;
    MC_ME->CTL_KEY = MC_ME_CTL_KEY_KEY(0x5AF0U);
    MC_ME->CTL_KEY = MC_ME_CTL_KEY_KEY(0xA50FU);
    /* Wait for the pending update to be consumed (CCUPD self-clears when MC_ME
     * has committed the PCONF change and released the core). This is the same
     * handshake used by the verified APP_BootCore1() in the MU driver example. */
    while (0U != (MC_ME->PRTN0_CORE1_PUPD & MC_ME_PRTN0_CORE1_PUPD_CCUPD_MASK))
    {
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Role of the primary core is only to start the secondary one */
    start_secondary_core();

    while(1)
    {
    }
}

/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "mcmgr.h"
/*${header:end}*/

static const uint32_t froPostDivFreq[] = {
    16000000U, 24000000U, 32000000U, 48000000U, 64000000U
};
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();

    uint32_t froPostDivSel = (FRO192M0->FROCCSR & FRO192M_FROCCSR_POSTDIV_SEL_MASK) >> FRO192M_FROCCSR_POSTDIV_SEL_SHIFT;
    SystemCoreClock = froPostDivFreq[froPostDivSel];
}

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}
/*${function:end}*/

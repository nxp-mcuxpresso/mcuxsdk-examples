/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "app.h"
#include "fsl_mu.h"
#include <stdbool.h>

#include "fsl_power.h"
#include "fsl_smm.h"

#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_BootAs10MHzClocks(void)
{
    CLOCK_EnableClock(kCLOCK_GateAonAPB);
    AON__CGU->CLOCK_DIV |= CGU_CLOCK_DIV_CLK_DIV_EN_MASK;

    /* After DPD2 exit both AON FROs may still be disabled.
     * Re-enable them so CLOCK_SetupFROAonClocking can compute a
     * valid coreClock_Hz for its internal delays. */
    AON__CGU->CLK_CONFIG |= CGU_CLK_CONFIG_ULPIRC_EN_MASK | CGU_CLK_CONFIG_LPIRC_EN_MASK;

    CLOCK_SetupFROAonClocking(10000000U);
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);

    AON__CGU->CLOCK_DIV |= 1UL << 1UL;
    CLOCK_EnableClock(kCLOCK_GateAonUART);
    SystemCoreClock = 10000000U;

    CLOCK_AttachClk(kFROdiv1_to_AON_CPU);
}

void BOARD_DeinitDebugConsole(void)
{
    DbgConsole_Deinit();
    RESET_SetPeripheralReset(kAonUART_RST_SHIFT_RSTn);
    CLOCK_DisableClock(kCLOCK_GateAonPORT);
    CLOCK_DisableClock(kCLOCK_GateAonGPIO);
}

void BOARD_InitHardware(void)
{
    CLOCK_DisableADVCControl();
    BOARD_BootAs10MHzClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();

    /* Clear stale NVIC pending bits.  The CM0+ CPU reset via CGU
     * RST_SUB_BLK does not clear the NVIC.  After DPD2 wakeup the
     * LPUART0_AON pending bit may survive from the previous cycle. */
    NVIC_ClearPendingIRQ(LPUART0_AON_IRQn);

    SMM_DisableAonCpuIso(AON__SMM);
    EnableIRQ(SMM_EXT_IRQn);
    AON__SMM->PWDN_CONFIG &= ~SMM_PWDN_CONFIG_Q_TMT_EN_MASK;
}

/*${function:end}*/

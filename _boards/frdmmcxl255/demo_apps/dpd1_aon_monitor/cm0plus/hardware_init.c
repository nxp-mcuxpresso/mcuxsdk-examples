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

/*
 * MCUXpresso IDE uses a C startup file that does not call Power_LowPowerBoot().
 * The GCC/IAR/MDK assembly startups call it via __ENABLE_LP_BOOT.
 * Override SystemInitHook() so that Power_LowPowerBoot() runs after SystemInit()
 * but before data/bss initialisation — exactly where the context-restore check
 * must happen.
 */
#if defined(__MCUXPRESSO)
void SystemInitHook(void)
{
    Power_LowPowerBoot();
}
#endif

void BOARD_BootAs3MHzClocks(void)
{
    CLOCK_EnableClock(kCLOCK_GateAonAPB);
    AON__CGU->CLOCK_DIV |= CGU_CLOCK_DIV_CLK_DIV_EN_MASK;

    /* Re-enable both AON FROs so CLOCK_SetupFROAonClocking can compute a
     * valid coreClock_Hz.  Needed after DPD2 exit or ROM CGU clobber. */
    AON__CGU->CLK_CONFIG |= CGU_CLK_CONFIG_ULPIRC_EN_MASK | CGU_CLK_CONFIG_LPIRC_EN_MASK;

    CLOCK_SetupFROAonClocking(3000000U);
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);

    AON__CGU->CLOCK_DIV |= 1UL << 1UL;
    CLOCK_EnableClock(kCLOCK_GateAonUART);
    SystemCoreClock = 3000000U;

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
    BOARD_BootAs3MHzClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();

    SMM_DisableAonCpuIso(AON__SMM);
    EnableIRQ(SMM_EXT_IRQn);
    AON__SMM->PWDN_CONFIG &= ~SMM_PWDN_CONFIG_Q_TMT_EN_MASK;
}

/*${function:end}*/

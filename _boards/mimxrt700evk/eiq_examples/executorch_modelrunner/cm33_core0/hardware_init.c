/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board_init.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pmic_support.h"

/*
 * NOTE: Unlike executorch_cifarnet, this example does NOT embed a model in
 * flash, so there is no .modeldata section to copy from flash to RAM at
 * startup. SystemInitHook is therefore left empty. Models are uploaded at
 * runtime (later phases).
 */
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#elif defined(__ICCARM__)
#elif defined(__MCUXPRESSO)
#elif defined(__GNUC__)
void SystemInitHook(void)
{
    /* Nothing to do: no embedded model data to copy. */
}
#endif

void BOARD_Init()
{
    BOARD_InitPins();

    BOARD_InitAHBSC();
    BOARD_ConfigMPU();

    // Disable LDO
    POWER_SetVddnSupplySrc(kVddSrc_PMIC);
    POWER_SetVdd1SupplySrc(kVddSrc_PMIC);
    POWER_SetVdd2SupplySrc(kVddSrc_PMIC);
    POWER_ApplyPD();

    BOARD_InitPmicPins();
    BOARD_InitPmic();
    BOARD_SetPmicVdd2Voltage(1100000U); /* 1.1v for 325MHz clock. */

    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_APD_NPU);
    POWER_DisablePD(kPDRUNCFG_PPD_NPU);
    POWER_ApplyPD();
}

/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board_init.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "board.h"
#include "pmic_support.h"
#include "flash_opts.h"

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

    /* Map the PSRAM model window as NonCacheable (same attribute as the
       on-chip NC region, Attr 1, already set by BOARD_ConfigMPU). With the
       default cacheable mapping, CPU reads of model weights during
       load_method dead-lock the bus (observed with a CPU-only model whose
       .pte lives in PSRAM); the payload receive window behind 0x61000000
       stays cacheable - CPU-only traffic benefits from the cache. */
    ARM_MPU_SetRegion(3U, ARM_MPU_RBAR(0x60000000U, ARM_MPU_SH_OUTER, 0U, 1U, 0U),
                      ARM_MPU_RLAR(0x60FFFFFFU, 1U)); /* 16MB model window */

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

    /* XSPI2 HyperRAM (PSRAM): storage for uploaded models that exceed the
     * on-chip SRAM pte buffer. 500 MHz root clock from main PLL PFD3. */
    BOARD_InitPsRamPins_Xspi2();
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_XSPI2);
    CLOCK_SetClkDiv(kCLOCK_DivXspi2Clk, 1u);
    FlashInit(NULL); /* prints "XSPI Psram Enabled!" via the debug console */
}

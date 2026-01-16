/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_crc.h"
#include "fsl_glikey.h"
#include "board.h"

#include "fsl_trng.h"
/*${header:end}*/

/*${function:start}*/
static status_t GlikeyWriteEnable(GLIKEY_Type *base, uint8_t idx)
{
    if (kStatus_Success != GLIKEY_StartEnable(base, idx))
    {
        return kStatus_Fail;
    }
    if (kStatus_Success != GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP1))
    {
        return kStatus_Fail;
    }
    if (kStatus_Success != GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP2))
    {
        return kStatus_Fail;
    }
    if (kStatus_Success != GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP3))
    {
        return kStatus_Fail;
    }
    if (kStatus_Success != GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP_EN))
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Turn on PKC RAM inteleaving. This setting is need for PKC L0 memory access. */
    SYSCON->RAM_INTERLEAVE |= SYSCON_RAM_INTERLEAVE_INTERLEAVE_MASK;

    RESET_ReleasePeripheralReset(kPKC0_RST_SHIFT_RSTn);

    RESET_ReleasePeripheralReset(kTRNG0_RST_SHIFT_RSTn);

    CLOCK_EnableClock(kCLOCK_GateSGI0);

    CLOCK_EnableClock(kCLOCK_GateTRNG0);

    CLOCK_EnableClock(kCLOCK_GatePKC0);

    trng_config_t trngcon;
    TRNG_GetDefaultConfig(&trngcon);
    trngcon.oscillatorMode = kTRNG_DualOscillatorMode;

    TRNG_Init(TRNG0,&trngcon);

    CRC_Type *base = CRC0;
    crc_config_t config;
    CRC_GetDefaultConfig(&config);
    CRC_Init(base, &config);

    /* By default SRAM have R/W enabled, execute permission is disabled */
    /* Enable execute permission in RAM atleast where stack is located (RAM C3 0x20038000 - 0x2003BFF) by setting SRAM_XEN register in SYSCON */
    /* This is needed so PKC can fetch FUP program */
    SYSCON->SRAM_XEN |= SYSCON_SRAM_XEN_RAMC_XEN_MASK;

    /* SRAM_XEN is duplicated by SRAM_XEN_DP which is protected by Glikey, SRAM_XEN_DP is protected by Glikey, enable write, and write same value as in SRAM_XEN register */
    GLIKEY_SyncReset(GLIKEY0);

    /* Move Glikey FSM to write enable */
    GlikeyWriteEnable(GLIKEY0, 2u);

    /* Write value into SRAM_XEN_DP */
    SYSCON->SRAM_XEN_DP |= SYSCON_SRAM_XEN_DP_RAMC_XEN_MASK ;

    /* End of write enable */
    GLIKEY_EndOperation(GLIKEY0);
}
/*${function:end}*/

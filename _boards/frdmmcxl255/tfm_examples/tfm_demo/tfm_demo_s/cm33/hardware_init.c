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
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Turn on PKC RAM inteleaving. This setting is need for PKC L0 memory access. */
    SYSCON->SRAM_INTERLEAVE |= SYSCON_SRAM_INTERLEAVE_INTERLEAVE_MASK;

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
    /* Enable execute permission in RAM atleast where stack is located (RAM B2 0x2801C000 - 0x2801DFFF) by setting SRAM_XEN register in SYSCON */
    /* This is needed so PKC can fetch FUP program */
    SYSCON->SRAM_XEN |= SYSCON_SRAM_XEN_RAMB2_XEN_MASK;

    /* SRAM_XEN is duplicated by SRAM_XEN_DP which is protected by Glikey, SRAM_XEN_DP is protected by Glikey, enable write, and write same value as in SRAM_XEN register */
    GLIKEY_SyncReset(GLIKEY0);

    /* Move Glikey FSM to write enable */
    GlikeyWriteEnable(GLIKEY0, 2u);

    /* Write value into SRAM_XEN_DP */
    SYSCON->SRAM_XEN_DP |= SYSCON_SRAM_XEN_DP_RAMB2_XEN_DP_MASK ;

    /* End of write enable */
    GLIKEY_EndOperation(GLIKEY0);
}

void SystemInit(void)
{
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access in Secure mode */
  #if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  SCB_NS->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access in Non-secure mode */
  #endif /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

  extern void *__VECTOR_TABLE[];
  SCB->VTOR = (uint32_t) & (__VECTOR_TABLE[0]);

#if defined(MCXL254_cm33_SERIES)  
  /* Disable ECC check for all RAMs */
  SYSCON->RAM_CTRL = SYSCON_RAM_CTRL_RAMA_ECC_ENABLE(0) ;
#endif

  //SystemInitHook();
}

/*${function:end}*/

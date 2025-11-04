/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "app.h"
#include "fsl_trng.h"
#include "fsl_crc.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    
    CLOCK_EnableClock(kCLOCK_GateSGI0);
    
    SGI0->SGI_CTRL2 = SGI_SGI_CTRL2_FLUSH_MASK;
    
    RESET_ReleasePeripheralReset(kTRNG0_RST_SHIFT_RSTn);
    
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
}
/*${function:end}*/


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

  //SystemInitHook();
}

/*${function:end}*/

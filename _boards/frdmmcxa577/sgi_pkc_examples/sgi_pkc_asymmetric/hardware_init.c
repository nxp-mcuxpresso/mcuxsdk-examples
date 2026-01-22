/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_crc.h"
#include "board.h"

#include "fsl_trng.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
            
    SYSCON->RAM_CASP_CTRL = 0x00000001u;
    
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
}
/*${function:end}*/

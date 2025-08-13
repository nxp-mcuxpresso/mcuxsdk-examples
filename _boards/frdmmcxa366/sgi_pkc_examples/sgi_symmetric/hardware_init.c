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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    
    CLOCK_EnableClock(kCLOCK_GateSGI0);

    CRC_Type *base = CRC0;
    crc_config_t config;
    CRC_GetDefaultConfig(&config);
    CRC_Init(base, &config);
}
/*${function:end}*/

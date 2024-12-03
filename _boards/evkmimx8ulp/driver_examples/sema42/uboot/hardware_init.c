/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "fsl_trdc.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${variable:start}*/
uint32_t mrc_start_addr[2] = {0x4000000, 0x40000000};
uint32_t mrc_end_addr[2]   = {0x5000000, 0x50000000};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot();
    }
}

void APP_SetTrdcGlobalConfig(void)
{
    if (BOARD_IsLowPowerBootType() == true) /* low power boot type */
    {
        PRINTF("Pls running the demo with uboot that running on A Core\r\n");
        assert(false);
    }
}

void APP_InitDomainConfig(void)
{
    APP_SetTrdcGlobalConfig();
}

void APP_DeinitDomainConfig(void)
{
    TRDC_Deinit(TRDC);
}
/*${function:end}*/

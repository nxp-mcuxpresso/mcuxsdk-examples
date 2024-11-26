/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
/*${header:end}*/

/* only for test on zebu */
int g_core_id = 1;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Introduce some delay to wait the remote boot up first.
     * Otherwise link_state will be set before the remote bootup,
     * link_up cannot be finished. */
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);
}
/*${function:end}*/

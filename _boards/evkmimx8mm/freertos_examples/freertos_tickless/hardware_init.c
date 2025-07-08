/*
 * Copyright 2018-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_uart.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();
#if configUSE_TICKLESS_IDLE == 2
    CLOCK_SetRootMux(kCLOCK_RootGpt1, kCLOCK_GptRootmuxSysPll1Div20); /* Set GPT1 source to SysPLL1 Div20 40MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootGpt1, 1U, 5U);                    /* Set root clock to 40MHZ / 5 = 8MHZ */
#endif
}
/*${function:end}*/

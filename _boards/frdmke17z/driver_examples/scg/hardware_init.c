/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_smc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeVlp);

    BOARD_InitBootPins();

    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);

    uint32_t uartClkSrcFreq = CLOCK_GetIpFreq(kCLOCK_Lpuart0);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}
/*${function:end}*/

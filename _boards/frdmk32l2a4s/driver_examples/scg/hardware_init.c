/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_smc.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    uint32_t uartClkSrcFreq;

    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    BOARD_InitBootPins();

    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);
    uartClkSrcFreq = CLOCK_GetIpFreq(kCLOCK_Lpuart0);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}
/*${function:end}*/

/*!
** @}
*/

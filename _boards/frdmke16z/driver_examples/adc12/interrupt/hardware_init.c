/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set ADC12's clock source to be Slow IRC async clock. */
    CLOCK_SetIpSrc(DEMO_ADC12_CLOCK_NAME, kCLOCK_IpSrcSircAsync);
}
/*${function:end}*/

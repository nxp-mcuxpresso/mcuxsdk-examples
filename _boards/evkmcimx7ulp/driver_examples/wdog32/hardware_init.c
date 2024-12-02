/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

#ifdef ENABLE_RAM_VECTOR_TABLE
AT_QUICKACCESS_SECTION_CODE(void WDOG_IRQHandler(void));
#endif

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#ifdef ENABLE_RAM_VECTOR_TABLE
    InstallIRQHandler(WDOG0_IRQn, (uint32_t)WDOG_IRQHandler);
#endif
    NVIC_EnableIRQ(WDOG0_IRQn);
}
/*${function:end}*/

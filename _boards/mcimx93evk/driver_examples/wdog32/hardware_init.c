/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
/*${header:end}*/

#ifdef ENABLE_RAM_VECTOR_TABLE
AT_QUICKACCESS_SECTION_CODE(void WDOG_IRQHandler(void));
#endif

/*${function:start}*/
#define SRC_RBASE      (0x44460000)
#define REG_SRC_SRMASK (SRC_RBASE + 0x18)

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(EXAMPLE_WDOG_CLOCK);

    *(volatile uint32_t *)REG_SRC_SRMASK &= (~1);

#ifdef ENABLE_RAM_VECTOR_TABLE
    InstallIRQHandler(EXAMPLE_WDOG_IRQ, (uint32_t)WDOG_IRQHandler);
#endif
    NVIC_EnableIRQ(EXAMPLE_WDOG_IRQ);
}
/*${function:end}*/

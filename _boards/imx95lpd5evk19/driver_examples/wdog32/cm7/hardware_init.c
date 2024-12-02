/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "sm_platform.h"
#include "hal_clock.h"
/*${header:end}*/

#ifdef ENABLE_RAM_VECTOR_TABLE
AT_QUICKACCESS_SECTION_CODE(void WDOG_IRQHandler(void));
#endif

/*${function:start}*/

void BOARD_InitHardware(void)
{
    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CCMSRCGPC__SRC__SRC_CENTRAL_REG->SRTMR = SRC_GENERAL_REG_SRTMR_RST_EVT_12_MODE(1); 
    CCMSRCGPC__SRC__SRC_CENTRAL_REG->SRMASK = (~SRC_GENERAL_REG_SRMASK_RST_EVT_12_MASK(1));
    CCMSRCGPC__SRC__SRC_CENTRAL_REG->SRMR[12] = SRC_GENERAL_REG_SRMR_RSTR_28(1) |
        SRC_GENERAL_REG_SRMR_RSTR_20(1);

#ifdef ENABLE_RAM_VECTOR_TABLE
    InstallIRQHandler(EXAMPLE_WDOG_IRQ, (uint32_t)WDOG_IRQHandler);
#endif
    NVIC_EnableIRQ(EXAMPLE_WDOG_IRQ);
}
/*${function:end}*/

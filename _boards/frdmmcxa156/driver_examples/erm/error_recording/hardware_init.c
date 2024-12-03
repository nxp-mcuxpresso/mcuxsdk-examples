/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include <stdbool.h>
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    RESET_PeripheralReset(kEIM0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kERM0_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void BOARD_MemoryInit(void)
{
    SYSCON->RAM_CTRL |= SYSCON_RAM_CTRL_RAMA_ECC_ENABLE_MASK;
    /* RAMA0(8KB): 0x20000000 ~ 0x20001FFF */
    uint32_t *ramAddress = (uint32_t *)APP_ERM_RAM_START_ADDR;
    uint32_t ramSize     = APP_ERM_RAM_SIZE;

    for (uint32_t i = 0x00U; i < ramSize; i++)
    {
        *ramAddress = APP_ERM_MAGIC_NUMBER;
        ramAddress++;
    }
}
/*${function:end}*/

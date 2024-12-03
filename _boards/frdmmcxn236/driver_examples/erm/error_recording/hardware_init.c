/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void BOARD_MemoryInit(void)
{
    /* Enable memory channel RAMA ECC functionality. */
    SYSCON->ECC_ENABLE_CTRL = APP_ERM_RAM_ECC_ENABLE;
    /* RAMA3(8KB): 0x20008000 ~ 0x2000FFFF */
    uint32_t *ramAddress = (uint32_t *)APP_ERM_RAM_START_ADDR;
    uint32_t ramSize     = APP_ERM_RAM_SIZE;

    for (uint32_t i = 0x00U; i < ramSize; i++)
    {
        *ramAddress = APP_ERM_MAGIC_NUMBER;
        ramAddress++;
    }
}

/*${function:end}*/

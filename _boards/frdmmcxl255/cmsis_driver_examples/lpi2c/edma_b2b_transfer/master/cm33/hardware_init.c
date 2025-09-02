/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    BOARD_InitI2CPins();
    
    /* Enable DMA0 requests for I2C Receive and Transmit */
    AHBSC__AHBSC0->SEC_GP_REG[(kDma0RequestLPI2C0Tx >> 5U)] |= (1UL << (kDma0RequestLPI2C0Tx & 31U));
    AHBSC__AHBSC0->SEC_GP_REG[(kDma0RequestLPI2C0Rx >> 5U)] |= (1UL << (kDma0RequestLPI2C0Rx & 31U));
}

uint32_t LPI2C0_GetFreq(void)
{
    return LPI2C_CLOCK_FREQUENCY;
}
/*${function:end}*/
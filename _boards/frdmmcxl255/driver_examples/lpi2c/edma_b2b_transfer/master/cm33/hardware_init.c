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
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    BOARD_InitI2CPins();
    
    /* Enable DMA0 requests for I2C Receive and Transmit */
    AHBSC__AHBSC0->SEC_GP_REG[(DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL >> 5U)] |= (1UL << (DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL & 31U));
    AHBSC__AHBSC0->SEC_GP_REG[(DEMO_LPI2C_RECEIVE_EDMA_CHANNEL  >> 5U)] |= (1UL << (DEMO_LPI2C_RECEIVE_EDMA_CHANNEL  & 31U));
}
/*${function:end}*/
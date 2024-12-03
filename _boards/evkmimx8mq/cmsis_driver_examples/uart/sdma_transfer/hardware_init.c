/*
 * Copyright 2017 NXP
 * All rights reserved.
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
    sdma_config_t sdmaConfig;
    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();

    /* Init the SDMA module */
    SDMA_GetDefaultConfig(&sdmaConfig);
    SDMA_Init(EXAMPLE_UART_DMA_BASEADDR, &sdmaConfig);
}

uint32_t UART2_GetFreq(void)
{
    return BOARD_DEBUG_UART_CLK_FREQ;
}
/*${function:end}*/

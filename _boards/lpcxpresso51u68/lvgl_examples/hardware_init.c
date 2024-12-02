/*
 * Copyright 2020 (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_dma.h"
/*${header:end}*/

/*${macro:start}*/
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR DMA0
/*${macro:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to I2C0 */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

    /* attach 12 MHz clock to I2C1 */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM5);

    /* attach 12 MHz clock to I2C1 */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);

    CLOCK_EnableClock(kCLOCK_Gpio1);

    /* reset FLEXCOMM for SPI and I2C */
    RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC5_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO1_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();
    DMA_Init(EXAMPLE_LPSPI_MASTER_DMA_BASEADDR);
}
/*${function:end}*/

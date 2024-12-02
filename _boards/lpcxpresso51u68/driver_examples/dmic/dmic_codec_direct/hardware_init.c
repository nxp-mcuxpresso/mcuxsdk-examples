/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_InputMux);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio1);

    /* USART0 clock */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* I2C clock */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

    /* I2S clocks */
    CLOCK_AttachClk(kFRO_HF_to_FLEXCOMM6);
    CLOCK_AttachClk(kFRO_HF_to_FLEXCOMM7);

    /* DMIC uses 12MHz FRO clock */
    CLOCK_AttachClk(kFRO12M_to_DMIC);

    /*12MHz divided by 15 = 800KHz PDM clock */
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 14, false);

    /* Enable high speed clock for I2S*/
    SYSCON->FROCTRL |= (uint32_t)SYSCON_FROCTRL_HSPDCLK(1U);
    SYSCON->MCLKDIV = SYSCON_MCLKDIV_DIV(1 - 1);
    SYSCON->MCLKIO  = 1U;

    /* Attach high speed clock to MCLK for I2S */
    CLOCK_AttachClk(kFRO_HF_to_MCLK);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);

    /* reset FLEXCOMM for I2S */
    RESET_PeripheralReset(kFC6_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC7_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/

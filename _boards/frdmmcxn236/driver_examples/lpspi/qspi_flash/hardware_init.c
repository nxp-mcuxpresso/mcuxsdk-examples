/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void SPI_Flash_PullCS(uint8_t level)
{
    GPIO_PinWrite(APP_LPSPI_CS_GPIO, APP_LPSPI_CS_PIN, level);
}

static void APP_InitLpspiClock(void)
{
    /* SPI clock */
    CLOCK_SetClkDiv(kCLOCK_DivPllClk, 1u);
    CLOCK_AttachClk(kPLL0_to_PLLCLKDIV);

    CLOCK_SetClkDiv(kCLOCK_DivFlexcom6Clk, 1u);
    CLOCK_AttachClk(kPLL_DIV_to_FLEXCOMM6);
}

void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    APP_InitLpspiClock();
}
/*${function:end}*/

/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "fsl_inputmux.h"
#include "fsl_reset.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitLcdicClock()
{
    /* LCDIC clock.
     * SPI baud rate is the same with LCDIC functional clock.
     */
    CLOCK_AttachClk(kMAIN_CLK_to_LCD_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 26);
    RESET_PeripheralReset(kLCDIC_RST_SHIFT_RSTn);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitLcdicClock();

    INPUTMUX_Init(INPUTMUX);
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);

    INPUTMUX_AttachSignal(INPUTMUX, APP_LCD_TX_DMA_CH, kINPUTMUX_LcdTxRegToDmaSingleToDma0);

    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmac0InputTriggerLcdTxRegToDmaSingleEna, true);
}
/*${function:end}*/

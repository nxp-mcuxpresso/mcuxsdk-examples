/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_inputmux.h"
#include "fsl_dma.h"
#include "fsl_gpio.h"
#include "display_support.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
    BOARD_InitLcd1Pins();

    /* LCDIC clock. */
    CLOCK_AttachClk(kMAIN_CLK_to_LCD_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 13);
    RESET_PeripheralReset(kLCDIC_RST_SHIFT_RSTn);
#else
    BOARD_InitLcd0Pins();

    /* SPI clock. */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM1);
    RESET_PeripheralReset(kFC1_RST_SHIFT_RSTn);
#endif

    /* Use 16 MHz clock for the FLEXCOMM2 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM2);

    /* DMA */
    DMA_Init(DMA0);
    /* GPIO. */
    GPIO_PortInit(GPIO, 0);
    GPIO_PortInit(GPIO, 1);

    /* inputmux */
    INPUTMUX_Init(INPUTMUX);
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);
}
/*${function:end}*/

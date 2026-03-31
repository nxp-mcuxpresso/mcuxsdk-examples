/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_dmamux.h"
#include "fsl_siul2.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitLCDPins();

    /* Need explicitly enable TCD clock. */
    CLOCK_EnableClock(kCLOCK_Tcd0);
    /* Init DMAMUX */
    DMAMUX_Init(EXAMPLE_FLEXIO_MCULCD_DMAMUX_BASEADDR);
    /* Set channel for LPUART */
    DMAMUX_SetSource(EXAMPLE_FLEXIO_MCULCD_DMAMUX_BASEADDR, DEMO_FLEXIO_TX_DMAMUX_CHANNEL, DEMO_FLEXIO_TX_DMA_REQUEST);
    DMAMUX_EnableChannel(EXAMPLE_FLEXIO_MCULCD_DMAMUX_BASEADDR, DEMO_FLEXIO_TX_DMAMUX_CHANNEL);
}

void BOARD_SetCSPin(bool set)
{
    SIUL2_PortPinWrite(BOARD_SIUL2_BASE, BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, set);
}

void BOARD_SetRSPin(bool set)
{
    SIUL2_PortPinWrite(BOARD_SIUL2_BASE, BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, set);
}

void BOARD_SetResetPin(bool set)
{
    SIUL2_PortPinWrite(BOARD_SIUL2_BASE, BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, set);
}
/*${function:end}*/

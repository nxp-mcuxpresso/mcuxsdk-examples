/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_debug_console.h"
#include "fsl_inputmux.h"
#include "fsl_pint.h"
/*${header:end}*/

/*${function:start}*/
extern volatile bool g_userPress;

void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    g_userPress = true;
}

void BOARD_InitKey(void)
{
    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, kINPUTMUX_GpioPort0Pin29ToPintsel);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    /* Initialize PINT */
    PINT_Init(PINT);

    /* Setup Pin Interrupt 0 for rising edge */
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableRiseEdge, pint_intr_callback);

    /* Enable callbacks for PINT0 by Index */
    PINT_EnableCallbackByIndex(PINT, kPINT_PinInt0);
}

void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitKey();

    CLOCK_SetClkDiv(kCLOCK_DivTraceClk, 2U);
    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 2U);
    /*!< Switch TRACE to TRACE_DIV */
    CLOCK_AttachClk(kTRACE_DIV_to_TRACE);
}

void BOARD_InitDebugConsoleSWO(uint32_t port, uint32_t baudrate)
{
    SystemCoreClockUpdate();
    uint32_t clkSrcFreq = SystemCoreClock / 2;

    DbgConsole_Init(port, baudrate, kSerialPort_Swo, clkSrcFreq);
}

/*${function:end}*/

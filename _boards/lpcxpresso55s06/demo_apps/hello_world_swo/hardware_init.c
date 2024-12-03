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
#if defined(FSL_FEATURE_SYSCON_HAS_PINT_SEL_REGISTER) && FSL_FEATURE_SYSCON_HAS_PINT_SEL_REGISTER
#include "fsl_syscon.h"
#else
#include "fsl_inputmux.h"
#endif /* FSL_FEATURE_SYSCON_HAS_PINT_SEL_REGISTER */
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
#if defined(FSL_FEATURE_SYSCON_HAS_PINT_SEL_REGISTER) && FSL_FEATURE_SYSCON_HAS_PINT_SEL_REGISTER
    /* Connect trigger sources to PINT */
    SYSCON_AttachSignal(SYSCON, kPINT_PinInt0, kINPUTMUX_GpioPort0Pin5ToPintsel);
#else
    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, kINPUTMUX_GpioPort0Pin5ToPintsel);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);
#endif /* FSL_FEATURE_SYSCON_HAS_PINT_SEL_REGISTER */

    /* Initialize PINT */
    PINT_Init(PINT);

    /* Setup Pin Interrupt 0 for rising edge */
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableRiseEdge, pint_intr_callback);

    /* Enable callbacks for PINT */
    PINT_EnableCallback(PINT);
}

void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    /*!< Switch TRACE to TRACE_DIV */
    CLOCK_AttachClk(kTRACE_DIV_to_TRACE);

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();

    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 2U, false);      /* Set AHBCLKDIV divider to value 2 */
    CLOCK_SetClkDiv(kCLOCK_DivArmTrClkDiv, 2U, false); /* Set ARMTRCLKDIV divider to value 2 */

    BOARD_InitDebugConsole();
#if !defined(DONT_ENABLE_FLASH_PREFETCH)
    /* enable flash prefetch for better performance */
    SYSCON->FMCCR |= SYSCON_FMCCR_PREFEN_MASK;
#endif
    BOARD_InitKey();
}

void BOARD_InitDebugConsoleSWO(uint32_t port, uint32_t baudrate)
{
    SystemCoreClockUpdate();
    uint32_t clkSrcFreq = SystemCoreClock;

    DbgConsole_Init(port, baudrate, kSerialPort_Swo, clkSrcFreq);
}

/*${function:end}*/

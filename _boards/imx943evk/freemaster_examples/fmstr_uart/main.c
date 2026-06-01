/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////


#include "pin_mux.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_serial_lpuart.h"

#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Local macros define
////////////////////////////////////////////////////////////////////////////////

#define _FMSTR_LPUART_BASE_ADDR(x)   LPUART##x
#define _FMSTR_LPUART_IRQn(x)        LPUART##x##_IRQn
#define _FMSTR_LPUART_IRQ_HANDLER(x) LPUART##x##_IRQHandler

#define FMSTR_LPUART_BASE_ADDR(x)    _FMSTR_LPUART_BASE_ADDR(x)
#define FMSTR_LPUART_IRQn(x)         _FMSTR_LPUART_IRQn(x)
#define FMSTR_LPUART_IRQ_HANDLER(x)  _FMSTR_LPUART_IRQ_HANDLER(x)

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void init_freemaster_lpuart(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Board initialization */
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();

    /* FreeMASTER communication layer initialization */
    init_freemaster_lpuart();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    while (1)
    {
        /* FreeMASTER example increments several variables periodically,
           use the FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll();
    }
}

/*!
 * @brief LPUART Module initialization (LPUART is a the standard block included e.g. in K66F)
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    /* clang-format off */
    clk_t clk = {
        .clkId = kCLOCK_IpInvalid,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    clk.clkId = BOARD_GetUartClkId(BOARD_DEBUG_CONSOLE_PORT);

    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200U;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init(FMSTR_LPUART_BASE_ADDR(BOARD_DEBUG_UART_INSTANCE), &config, CLOCK_GetRate(clk.clkId));

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(FMSTR_LPUART_BASE_ADDR(BOARD_DEBUG_UART_INSTANCE));

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(FMSTR_LPUART_IRQn(BOARD_DEBUG_UART_INSTANCE));
    EnableGlobalIRQ(0);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
/*
 *   Application interrupt handler of communication peripheral used in interrupt modes
 *   of FreeMASTER communication.
 *
 *   NXP MCUXpresso SDK framework defines interrupt vector table as a part of "startup_XXXXXX.x"
 *   assembler/C file. The table points to weakly defined symbols, which may be overwritten by the
 *   application specific implementation. FreeMASTER overrides the original weak definition and
 *   redirects the call to its own handler.
 *
 */

void FMSTR_LPUART_IRQ_HANDLER(BOARD_DEBUG_UART_INSTANCE)(void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_SerialIsr();
}
#endif

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////

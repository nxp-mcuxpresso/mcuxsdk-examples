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

#include "board.h"
#include "pin_mux.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"

#include "freemaster.h"
#include "freemaster_serial_lpuart.h"

#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_UART_BASE                     BOARD_DEBUG_UART_BASEADDR
#define EXAMPLE_UART_INTERRUPT                BOARD_UART_IRQ
#define EXAMPLE_UART_INTERRUPT_HANDLER        BOARD_UART_IRQ_HANDLER
#define EXAMPLE_UART_CLOCK_FREQUENCY          BOARD_DEBUG_UART_CLK_FREQ
#define EXAMPLE_UART_BAUD_RATE                115200U

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
    BOARD_InitBootClocks();
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();

    /* FreeMASTER communication layer initialization */
    init_freemaster_lpuart();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    while(1)
    {
        /* FreeMASTER example increments several variables periodically,
           use the FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll();
    }
}

/*!
 * @brief LPUART Module initialization
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    /* Reset UART module */
    RESET_PeripheralReset(BOARD_DEBUG_UART_RST);

    /* Get default configuration */
    LPUART_GetDefaultConfig(&config);

    /* Update the configuration */
    config.baudRate_Bps = EXAMPLE_UART_BAUD_RATE;
    config.enableTx = false;
    config.enableRx = false;

    /* Initialize UART module */
    LPUART_Init((LPUART_Type*)EXAMPLE_UART_BASE, &config, BOARD_DEBUG_UART_CLK_FREQ);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((LPUART_Type*)EXAMPLE_UART_BASE);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(EXAMPLE_UART_INTERRUPT);
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

void EXAMPLE_UART_INTERRUPT_HANDLER (void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_SerialIsr();
}
#endif

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////

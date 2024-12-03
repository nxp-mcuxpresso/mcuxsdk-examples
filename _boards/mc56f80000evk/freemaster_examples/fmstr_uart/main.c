/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_qsci.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_serial_qsci.h"

#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Local macros define what QSCI port is used
////////////////////////////////////////////////////////////////////////////////

#if 0
    /* Use SCI_1 at Boot_SCI USB-to-serial port */
    #define BOARD_FMSTR_UART_INSTANCE   1
    #define BOARD_FMSTR_UART_CLOCK_NAME kCLOCK_QSCI1
#else
    /* Use SCI_0 at OSJTAG virtual serial port */
    #define BOARD_FMSTR_UART_INSTANCE   0
    #define BOARD_FMSTR_UART_CLOCK_NAME kCLOCK_QSCI0
#endif

#define BOARD_FMSTR_UART_BASE_ADDR  qsciBases[BOARD_FMSTR_UART_INSTANCE]

/* Macro to access the QSCI IRQn interrupt vector number */
#define QSCIx_IRQn(x, irq)  QSCI##x##_##irq##_IRQn
#define QSCIx_RERR_IRQn(x)  QSCIx_IRQn(x, RERR)
#define QSCIx_RCV_IRQn(x)   QSCIx_IRQn(x, RCV)
#define QSCIx_TDRE_IRQn(x)  QSCIx_IRQn(x, TDRE)
#define QSCIx_TRIDLE_IRQn(x) QSCIx_IRQn(x, TRIDLE)

/* Macro to declare IRQ handler for QSCI module number defined by a macro.
 * Refer to vector table and associated IRQ handlers. */
#define QSCI_IRQ_HANDER2(x, vec) ivINT_QSCI##x##_##vec
#define QSCI_IRQ_HANDER(x, vec) QSCI_IRQ_HANDER2(x, vec) /* two-phase to enable x macro to expand */

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

// array of all QSCI base addresses
const QSCI_Type *qsciBases[] = QSCI_BASE_PTRS;

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void init_freemaster_qsci(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Board initialization */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    CLOCK_EnableClock(BOARD_FMSTR_UART_CLOCK_NAME);

    /* FreeMASTER communication layer initialization */
    init_freemaster_qsci();

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
 * @brief QSCI Module initialization
 */
static void init_freemaster_qsci(void)
{
    qsci_config_t config;
    uint32_t uartClkSrcFreq;

    uartClkSrcFreq = CLOCK_GetIpClkSrcFreq(BOARD_FMSTR_UART_CLOCK_NAME);
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    QSCI_GetDefaultConfig(&config, 115200U, uartClkSrcFreq);
    config.bEnableTx = false;
    config.bEnableRx = false;

    QSCI_Init((QSCI_Type*)BOARD_FMSTR_UART_BASE_ADDR, &config);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((QSCI_Type*)BOARD_FMSTR_UART_BASE_ADDR);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQWithPriority(QSCIx_RERR_IRQn(BOARD_FMSTR_UART_INSTANCE), 1);
    EnableIRQWithPriority(QSCIx_RCV_IRQn(BOARD_FMSTR_UART_INSTANCE), 1);
    EnableIRQWithPriority(QSCIx_TDRE_IRQn(BOARD_FMSTR_UART_INSTANCE), 1);
    EnableIRQWithPriority(QSCIx_TRIDLE_IRQn(BOARD_FMSTR_UART_INSTANCE), 1);
    SetIRQBasePriority(0);
    EnableGlobalIRQ(1);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR

/* Prototypes of interrupt handlers */
void QSCI_IRQ_HANDER(BOARD_FMSTR_UART_INSTANCE, RERR)(void);
void QSCI_IRQ_HANDER(BOARD_FMSTR_UART_INSTANCE, RCV)(void);
void QSCI_IRQ_HANDER(BOARD_FMSTR_UART_INSTANCE, TRIDLE)(void);
void QSCI_IRQ_HANDER(BOARD_FMSTR_UART_INSTANCE, TDRE)(void);

/* All interrupt handlers call FreeMASTER Interrupt routine handler */
#pragma interrupt alignsp saveall
void QSCI_IRQ_HANDER(BOARD_FMSTR_UART_INSTANCE, RERR)(void)
{
    FMSTR_SerialIsr();
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void QSCI_IRQ_HANDER(BOARD_FMSTR_UART_INSTANCE, RCV)(void)
{
    FMSTR_SerialIsr();
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void QSCI_IRQ_HANDER(BOARD_FMSTR_UART_INSTANCE, TRIDLE)(void)
{
    FMSTR_SerialIsr();
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void QSCI_IRQ_HANDER(BOARD_FMSTR_UART_INSTANCE, TDRE)(void)
{
    FMSTR_SerialIsr();
}
#pragma interrupt off
#endif

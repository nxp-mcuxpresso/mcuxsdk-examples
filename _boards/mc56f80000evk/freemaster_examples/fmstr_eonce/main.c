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
#include "board.h"

#include "freemaster.h"
#include "freemaster_56f800e_eonce.h"

#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void init_freemaster_eonce(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Board initialization */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    /* FreeMASTER communication layer initialization */
    init_freemaster_eonce();

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
 * @brief JTAG EOnCE Module initialization
 */
static void init_freemaster_eonce(void)
{
#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQWithPriority(TX_REG_IRQn, 1);
    EnableIRQWithPriority(RX_REG_IRQn, 1);
    SetIRQBasePriority(0);
    EnableGlobalIRQ(1);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR

/* Both EOnCE interrupt handlers call FreeMASTER Interrupt routine handler */
#pragma interrupt alignsp saveall
void ivINT_TX_REG(void);
void ivINT_TX_REG(void)
{
    FMSTR_SerialIsr();
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void ivINT_RX_REG(void);
void ivINT_RX_REG(void)
{
    FMSTR_SerialIsr();
}
#pragma interrupt off

#endif

/*
 * Copyright 2024 NXP
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

#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "usb_device_config.h"

#include "freemaster.h"
#include "freemaster_usb.h"
#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Board initialization */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Initialize the USB peripheral clock */
    RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
    CLOCK_EnableUsbfsClock();
    
    /* FreeMASTER communication layer initialization */
    FMSTR_ExampleUsbInit();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    PRINTF(
        "\nFreeMASTER USB Example.\n"
        "Connect using USB Serial Device COM port.\n\n");

    while(1)
    {
        // FreeMASTER example increments several variables periodically,
        // use the FreeMASTER PC Host tool to visualize the variables
        FMSTR_Example_Poll();
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////

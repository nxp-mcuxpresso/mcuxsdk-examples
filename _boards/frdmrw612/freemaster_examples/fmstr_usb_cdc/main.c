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
#include "clock_config.h"

#include "usb_device_config.h"
#include "fsl_debug_console.h"

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
void USB_DeviceClockInit(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Init hardware */
    BOARD_InitDEBUG_UARTPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Init USB clocks */
    USB_DeviceClockInit();

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

void USB_DeviceClockInit(void)
{
    /* reset USB */
    RESET_PeripheralReset(kUSB_RST_SHIFT_RSTn);

    /* enable usb clock */
    CLOCK_EnableClock(kCLOCK_Usb);

    /* enable usb phy clock */
    CLOCK_EnableUsbhsPhyClock();
}

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////

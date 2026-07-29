/*
 * Copyright 2026 NXP
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

#include "usb.h"
#include "usb_phy.h"

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
    BOARD_InitPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    uint32_t usbClockFreq;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    usbClockFreq = 24000000;
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);

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

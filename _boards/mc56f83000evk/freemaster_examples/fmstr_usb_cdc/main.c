/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2020 NXP
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
#include "usb.h"
#include "board.h"

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
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Board initialization */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    USB_DeviceClockInit();

    /* FreeMASTER communication layer initialization */
    FMSTR_ExampleUsbInit();

    SetIRQBasePriority(0);
    EnableGlobalIRQ(1);

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    while(1)
    {
        /* FreeMASTER example increments several variables periodically,
           use the FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll();
    }
}

void USB_DeviceClockInit(void)
{
    CLOCK_EnableUsbfs0Clock();
}

void USB_DeviceIsrEnable(void)
{
    EnableIRQWithPriority(USB_IRQn, 0);
}

#pragma interrupt alignsp saveall
void ivINT_USB(void);
void ivINT_USB(void)
{
    /* Implementation in freemaster_usb.c is common for all platforms */
    void USB_MCU_INT_HANDLER(void);
    USB_MCU_INT_HANDLER();
}
#pragma interrupt off

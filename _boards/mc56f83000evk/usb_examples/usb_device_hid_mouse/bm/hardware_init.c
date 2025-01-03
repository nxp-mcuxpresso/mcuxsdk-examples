/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_device_registers.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "mouse.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
/*${header:end}*/

/*${macro:start}*/
#define USB_IRQ_HANDLER ivINT_USB
/*${macro:end}*/

extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
void USB_DeviceClockInit();
void USB_DeviceIsrEnable(void);

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_USB);

    SetIRQBasePriority(0);
}

void USB_DeviceClockInit(void)
{
    CLOCK_EnableUsbfs0Clock();
}

void USB_DeviceIsrEnable(void)
{
    EnableIRQWithPriority(USB_IRQn, USB_DEVICE_INTERRUPT_PRIORITY);
}

#pragma interrupt alignsp saveall
void USB_IRQ_HANDLER(void);
void USB_IRQ_HANDLER(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#pragma interrupt off

/*${function:end}*/

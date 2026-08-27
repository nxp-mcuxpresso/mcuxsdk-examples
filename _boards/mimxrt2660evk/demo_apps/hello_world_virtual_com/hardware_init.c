/*
 * Copyright 2025 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "board.h"
#include "usb_device_config.h"
#include "usb_phy.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_descriptor.h"
#include "fsl_debug_console.h"
#include "fsl_component_serial_port_usb.h"

void USB_DeviceClockInit(void);

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootClocks();
    BOARD_ConfigTRDC();
    BOARD_InitDEBUG_UARTPins();
    /* Initialize the debug console as CDC virtual com. */
    USB_DeviceClockInit();
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    DbgConsole_Init((uint8_t)kSerialManager_UsbControllerEhci0, (uint32_t)NULL, kSerialPort_UsbCdc, (uint32_t)NULL);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    DbgConsole_Init((uint8_t)kSerialManager_UsbControllerKhci0, (uint32_t)NULL, kSerialPort_UsbCdc, (uint32_t)NULL);
#endif
}

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbHsClock();
    USB_EhciPhyInit(kUSB_ControllerEhci0, CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_usb0_phyclk), &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    CLOCK_EnableUsbFsClock(kCLOCK_UsbFsSrcUsb1Root);
    /* Crystal-less: trim the FRO against the USB FS (KHCI) frame timing. */
    CLOCK_EnableFroTuner(kCLOCK_FroTuner192M, kCLOCK_FroTunerRefUsbFsSof1kHz, 0x02U);
#endif
}

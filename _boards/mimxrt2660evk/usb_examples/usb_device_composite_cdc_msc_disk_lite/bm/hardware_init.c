/*
 * Copyright 2025 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "usb_disk_adapter.h"
#include "composite.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"

extern usb_device_composite_struct_t g_composite;

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitUSDHC0Pins();
    /* MIMXRT2660-EVK ships two LPI2C0 IO expanders driving SDHC pins: U23 PCAL6524
     * for SD_PWREN and a second PCA9555 for card-detect / EXP_nIRQ2. Both pad-mux
     * helpers live in pin_mux.c; their I2C SDA/SCL muxes overlap (idempotent),
     * and each adds its own INT pin (PIO2_27 for PCAL6524, PIO3_29 for PCA9555). */
    BOARD_Init6524Pins();
    BOARD_InitPCA9555Pins();
    BOARD_USB_Disk_Config(USB_DEVICE_INTERRUPT_PRIORITY);
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_composite.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USBFS_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_composite.deviceHandle);
}
#endif

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbHsClock();
    USB_EhciPhyInit(CONTROLLER_ID, CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_usb0_phyclk), &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    CLOCK_EnableUsbFsClock(kCLOCK_UsbFsSrcUsb1Root);
    /* Crystal-less: trim the FRO against the USB FS (KHCI) frame timing. */
    CLOCK_EnableFroTuner(kCLOCK_FroTuner192M, kCLOCK_FroTunerRefUsbFsSof1kHz, 0x02U);
#endif
}

void USB_DeviceIsrEnable(void)
{
    IRQn_Type irqNumber;

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    IRQn_Type usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                    = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority(irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ(irqNumber);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    IRQn_Type usbDeviceKhciIrq[] = USBFS_IRQS;
    irqNumber                    = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority(irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ(irqNumber);
#endif
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    USB_DeviceEhciTaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    USB_DeviceKhciTaskFunction(deviceHandle);
#endif
}
#endif

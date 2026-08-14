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
#include "dfu_app.h"
#include "dfu_timer.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"

extern usb_device_dfu_app_struct_t g_UsbDeviceDfu;

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
}

void SysTick_Handler(void)
{
    DFU_TimerISR();
}

void DFU_TimerHWInit()
{
    /* Configure the SysTick timer */
    SysTick_Config(SystemCoreClock / 1000U);
    /* Disable the SysTick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void HW_TimerControl(uint8_t enable)
{
    if (enable)
    {
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    }
    else
    {
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceDfu.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USBFS_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceDfu.deviceHandle);
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
    CLOCK_TrimUsbFroClock(kCLOCK_UsbFroTrimFs);
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

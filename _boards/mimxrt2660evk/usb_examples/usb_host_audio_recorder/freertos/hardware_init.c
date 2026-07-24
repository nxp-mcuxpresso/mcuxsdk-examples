/*
 * Copyright 2025 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "app.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"

extern usb_host_handle g_hostHandle;

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
}

#if defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_hostHandle);
}
#endif

#if defined(USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI > 0U)
void USBFS_IRQHandler(void)
{
    USB_HostKhciIsrFunction(g_hostHandle);
}
#endif

void USB_HostClockInit(void)
{
#if defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U)
    uint32_t usbClockFreq = 24000000;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbhsPhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
    CLOCK_EnableUsbhsClock(kCLOCK_UsbSrcUnused, usbClockFreq);
    USB_EhciPhyInit(CONTROLLER_ID, usbClockFreq, &phyConfig);
#endif
#if defined(USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI > 0U)
    CLOCK_EnableUsbfsClock();
#endif
}

void USB_HostIsrEnable(void)
{
    IRQn_Type irqNumber;

#if defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U)
    IRQn_Type usbHostEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbHostEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    /* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority(irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority(irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ(irqNumber);
#endif
#if defined(USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI > 0U)
    IRQn_Type usbHostKhciIrq[] = USBFS_IRQS;
    irqNumber                  = usbHostKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    /* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority(irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority(irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ(irqNumber);
#endif
}

void USB_HostTaskFn(void *param)
{
#if defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U)
    USB_HostEhciTaskFunction(param);
#endif
#if defined(USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI > 0U)
    USB_HostKhciTaskFunction(param);
#endif
}

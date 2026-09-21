/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "usb_device_config.h"
#include "usb_host_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_host.h"

#include "fsl_device_registers.h"
#include "fsl_iomuxc.h"

#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "usb_phy.h"

/* COMM__USBNC CTRL2[20] = GPIO_IDDIG_SELECT. Not described by PERI_USBNC.h, which only defines
 * VBUS_SOURCE_SEL and UTMI_CLK_VLD, so it is named here rather than left as a bare bit. */
#define BOARD_USBNC_CTRL2_GPIO_IDDIG_SELECT_MASK (1UL << 20U)
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, Debug Console and GPIO handle. */
    BOARD_CommonSetting();

    /* Route the Type-C CC-logic (PTN5150A) ID output into the USBHS OTG controller so that
     * OTGSC[ID] reflects the cable orientation. The net has an external 10K pull-up, so only
     * the input buffer is enabled here.
     * NOTE: PIO2_22 is shared between COMM_USB0_ID and AUDIO_SAI1_MCLK through a 3-pin 0402
     * jumper. The board must be strapped to USB0_ID for role detection to work. */
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO2_22_COMM_USB0_ID, IOMUXC_PIO_IBENA_MASK);
}

static void USB_ClockInit(void)
{
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbHsClock();
    USB_EhciPhyInit(CONTROLLER_ID, CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_usb0_phyclk), &phyConfig);

    /* By default the OTG core samples ID from the USB PHY. On this board USB0_ID is an IOMUXC
     * pin (muxed in BOARD_InitHardware), so point the ID source at the pin instead -- without
     * this, OTGSC[ID] never tracks the cable. Kept after USB_EhciPhyInit() so that the PHY
     * bring-up cannot clobber it. */
    COMM__USBNC->CTRL2 |= BOARD_USBNC_CTRL2_GPIO_IDDIG_SELECT_MASK;
}

static IRQn_Type USB_GetIrqNumber(void)
{
    IRQn_Type usbEhciIrq[] = USBHS_IRQS;

    return usbEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
}

/* USB_HostInit()/USB_DeviceInit() reset the EHCI controller, which clears OTGSC[IDIE]. The
 * shared app only arms it once in APP_init(), so after the first role switch the ID edge would
 * arrive with IDIE clear and USB_Comom_IRQHandler() would dispatch it to the stack ISR instead
 * of reporting a role change -- the board would never switch back. Both IsrEnable() hooks run
 * after their stack init, so re-arming here covers every switch.
 * IDIS is deliberately left alone: a stale edge just makes the handler re-read the live ID,
 * whereas clearing it here could drop a real one. */
static void USB_ArmIdPinDetect(void)
{
    COMM__USBC->OTGSC |= USBHS_OTGSC_IDIE_MASK;
}

void USB_DeviceClockInit(void)
{
    USB_ClockInit();
}

void USB_HostClockInit(void)
{
    USB_ClockInit();
}

void USB_DeviceIsrEnable(void)
{
    IRQn_Type irqNumber = USB_GetIrqNumber();

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority(irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ(irqNumber);

    USB_ArmIdPinDetect();
}

void USB_DeviceIsrDisable(void)
{
    DisableIRQ(USB_GetIrqNumber());
}

void USB_HostIsrEnable(void)
{
    IRQn_Type irqNumber = USB_GetIrqNumber();

    /* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority(irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority(irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ(irqNumber);

    USB_ArmIdPinDetect();
}

void USB_HostIsrDisable(void)
{
    DisableIRQ(USB_GetIrqNumber());
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceEhciTaskFunction(deviceHandle);
}
#endif

void USB_HostTaskFn(void *param)
{
    USB_HostEhciTaskFunction(param);
}

/* RT2660 routes both the host and the device role through the single USBHS instance,
 * so the one vector is dispatched by USB_Comom_IRQHandler() according to the ID pin. */
void USBHS_IRQHandler(void)
{
    USB_Comom_IRQHandler();
}

/*!
 * @brief  Switch the USB0 Type-C VBUS supply on or off.
 *
 * VBUS is gated by the NX3P1108 load switch driven from PCAL6524 (U23) USB0_PWR. Driving the
 * expander needs the board GPIO handle API (gpio_handle_t / GPIO_SetPinCallback), which this
 * tree does not provide yet, so this only waits for the rail to settle -- the same behaviour as
 * the other boards that support these examples.
 */
void BOARD_UsbVbusOn(uint8_t on)
{
    (void)on;

    /* Wait for the VBUS rail to settle before the stack starts enumerating. */
    SDK_DelayAtLeastUs(20000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}
/*${function:end}*/

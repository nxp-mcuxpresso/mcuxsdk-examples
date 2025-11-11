/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"

#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "app.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "sm_platform.h"
/*${header:end}*/

extern usb_host_handle g_HostHandle;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();

    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    pcal6524_handle_t handle;

    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_USB2_PWR_EN), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_USB2_PWR_EN));
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_USB2_PWR_EN));
}

void USB2_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
}

void USB_HostClockInit(void)
{
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    irqNumber                = USB2_IRQn;
/* USB_HOST_CONFIG_EHCI */

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}

void USB_HostTaskFn(void *param)
{
    USB_HostEhciTaskFunction(param);
}
/*${function:end}*/

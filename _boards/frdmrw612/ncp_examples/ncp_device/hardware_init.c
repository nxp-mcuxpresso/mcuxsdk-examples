/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#if (CONFIG_WIFI_USB_FILE_ACCESS || (defined(CONFIG_BT_SNOOP) && (CONFIG_BT_SNOOP > 0)))
#include "usb_host_config.h"
#include "usb_host.h"
#if (CONFIG_WIFI_USB_FILE_ACCESS || (!defined(CONFIG_BT_SNOOP) || (CONFIG_BT_SNOOP == 0)))
#include "usb_support.h"
#endif
#endif
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
/*${header:end}*/

/*${macro:start}*/
#if (defined(CONFIG_BT_SNOOP) && (CONFIG_BT_SNOOP > 0))
#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_HOST_INTERRUPT_PRIORITY (6U)
#else
#define USB_HOST_INTERRUPT_PRIORITY (3U)
#endif
#endif
/*${macro:end}*/

/*${variable:start}*/
#if CONFIG_WIFI_USB_FILE_ACCESS
extern usb_host_handle g_HostHandle;
#endif
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    if (BOARD_IS_XIP())
    {
        BOARD_BootClockLPR();
        CLOCK_EnableClock(kCLOCK_Otp);
        CLOCK_EnableClock(kCLOCK_Els);
        CLOCK_EnableClock(kCLOCK_ElsApb);
        RESET_PeripheralReset(kOTP_RST_SHIFT_RSTn);
        RESET_PeripheralReset(kELS_APB_RST_SHIFT_RSTn);
    }
    else
    {
        BOARD_InitBootClocks();
    }
    BOARD_InitDebugConsole();
    /* Reset GMDA */
    RESET_PeripheralReset(kGDMA_RST_SHIFT_RSTn);
    /* Keep CAU sleep clock here. */
    /* CPU1 uses Internal clock when in low power mode. */
    POWER_ConfigCauInSleep(false);
    BOARD_InitSleepPinConfig();
}

#if (CONFIG_WIFI_USB_FILE_ACCESS || (defined(CONFIG_BT_SNOOP) && (CONFIG_BT_SNOOP > 0)))

#if (CONFIG_WIFI_USB_FILE_ACCESS || (!defined(CONFIG_BT_SNOOP) || (CONFIG_BT_SNOOP == 0)))
void USBHS_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
}
#endif

void USB_HostClockInit(void)
{
    /* reset USB */
    RESET_PeripheralReset(kUSB_RST_SHIFT_RSTn);
    /* enable usb clock */
    CLOCK_EnableClock(kCLOCK_Usb);
    /* enable usb phy clock */
    CLOCK_EnableUsbhsPhyClock();
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTEhciIrq[] = USBHS_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
    /* USB_HOST_CONFIG_EHCI */

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}

#if (CONFIG_WIFI_USB_FILE_ACCESS || (!defined(CONFIG_BT_SNOOP) || (CONFIG_BT_SNOOP == 0)))
void USB_HostTaskFn(void *param)
{
    USB_HostEhciTaskFunction(param);
}
#endif
#endif
/*${function:end}*/

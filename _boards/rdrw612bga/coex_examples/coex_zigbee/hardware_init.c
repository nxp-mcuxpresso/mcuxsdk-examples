/*
 * Copyright 2020-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "els_pkc_mbedtls.h"
#include "usb_host_config.h"
#include "usb_host.h"
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)) || defined(RW612_SERIES))
#include "els_pkc_mbedtls.h"
#include "platform_hw_ip.h"
#endif /* CONFIG_BT_SMP */

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

/* On RD-RW612-BGA boards, the Xtal32k is present and connected by default
 * so we use it by default. If an hardware rework done on the board disconnects
 * the Xtal32k, this flag should be set to 1 to fall back to the Fro32k */
#ifndef gBoardUseFro32k_d
#define gBoardUseFro32k_d 0
#endif

#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_HOST_INTERRUPT_PRIORITY (6U)
#else
#define USB_HOST_INTERRUPT_PRIORITY (3U)
#endif
/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    (void)CRYPTO_InitHardware();
#if defined(gBoardUseFro32k_d) && (gBoardUseFro32k_d > 0)
    CLOCK_AttachClk(kRC32K_to_CLK32K);
#else
    CLOCK_EnableXtal32K(true);
    CLOCK_AttachClk(kXTAL32K_to_CLK32K);
#endif

#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)) || defined(RW612_SERIES))
    CRYPTO_InitHardware();
#endif /* CONFIG_BT_SMP */
}

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

/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "clock_config.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "controller.h"
#include "fsl_power.h"
#include "usb_host_config.h"
#include "usb_host.h"
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#include "psa/crypto.h"
#include "mcux_mbedtls_psa_crypto_config.h"
#endif /* CONFIG_BT_SMP */
/*${header:end}*/

/*${function:start}*/
#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_HOST_INTERRUPT_PRIORITY (6U)
#else
#define USB_HOST_INTERRUPT_PRIORITY (3U)
#endif

#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
void bt_psa_crypto_init(void)
{
    psa_status_t status;

    status = psa_crypto_init();
    if (status != PSA_SUCCESS) {
        PRINTF("Failed to initialize PSA crypto");
    }
    assert(status == PSA_SUCCESS);
}
#endif /* CONFIG_BT_SMP */

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
#if defined(gBoardUseFro32k_d) && (gBoardUseFro32k_d > 0)
    CLOCK_AttachClk(kRC32K_to_CLK32K);
#else
    CLOCK_EnableXtal32K(true);
    CLOCK_AttachClk(kXTAL32K_to_CLK32K);
#endif
    BOARD_InitDebugConsole();
    BOARD_InitSleepPinConfig();
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
/*${function:end}*/

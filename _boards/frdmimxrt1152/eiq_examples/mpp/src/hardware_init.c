/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 /*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "display_support.h"
#include "board.h"
#include "fsl_soc_src.h"
/*${header:end}*/

#ifdef USE_USB_CAMERA
#include "usb_host_config.h"
#include "usb_host.h"
#include "app.h"
#include "usb_phy.h"
#include "board.h"

extern usb_host_handle g_HostHandle;

void USB_OTG1_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
}

void USB_HostClockInit(void)
{
    uint32_t usbClockFreq;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    usbClockFreq = 24000000;
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;
    uint8_t usbHOSTEhciIrq[] = USBHS_IRQS;
    irqNumber = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
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
#endif /* USE_USB_CAMERA */

/*!
 * @brief Resets display controller.
 */
static void BOARD_ResetDisplayMix(void)
{
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
    while (kSRC_SliceResetInProcess == SRC_GetSliceResetState(SRC, kSRC_DisplaySlice))
    {
    }
}

/*${function:start}*/
void BOARD_Init(void)
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();

    /* Reset display mix before pin/clock init, matching the validated
     * clock_freertos init sequence for frdmimxrt1152. */
    BOARD_ResetDisplayMix();

    BOARD_InitBootPins();
    BOARD_InitDEBUG_UARTPins();

    /* Mux GPIO_DISP_B2_12/13 to LPI2C4 (SCL/SDA) and configure GPIO_AD_27 as
     * the PCAL6524 interrupt input. The LPI2C4 peripheral is initialised lazily
     * inside BOARD_EnsurePCAL6524Init() -> BOARD_InitPCAL6524(), so only the
     * pin mux is needed here.
     *
     * NOTE: BOARD_PrepareDisplayController() is NOT called here because it
     * internally calls VIDEO_DelayMs() which maps to vTaskDelay() in FreeRTOS
     * builds. Calling vTaskDelay() before vTaskStartScheduler() hangs forever.
     * The MPP display HAL (hal_display_lcdifv2_rk055.c) calls
     * BOARD_PrepareDisplayController() from task context when the display
     * element is opened, which is safe. */
    BOARD_Init6524Pins();

    BOARD_InitDebugConsole();
}
/*${function:end}*/

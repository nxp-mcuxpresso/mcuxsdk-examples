/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${standard_header:start}*/
#include <stdbool.h>
/*${standard_header:end}*/
/*${header:start}*/
#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "usb_host_hid.h"
#include "host_mouse.h"

#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "app.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"

#include "fsl_adapter_gpio.h"
#include "fsl_adapter_timer.h"

/*${header:end}*/

/*${macro:start}*/
#if ((defined(USB_HOST_CONFIG_OHCI)) && (USB_HOST_CONFIG_OHCI > 0U))
#define APP_EXCLUDE_FROM_DEEPSLEEP  (kPDRUNCFG_PD_LDOFLASHNV | kPDRUNCFG_PD_USBFSPHY)
#define APP_DEEPSLEEP_WAKEUP_SOURCE (WAKEUP_GPIO_GLOBALINT0 | WAKEUP_USB0_NEEDCLK)
#endif
/*${macro:end}*/

/*${variable:start}*/
#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
extern usb_host_mouse_instance_t g_HostHidMouse;
extern usb_host_handle g_HostHandle;
static uint32_t systemTickControl;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
uint32_t g_gpioHandle[(HAL_GPIO_HANDLE_SIZE + 3) / 4];
/*${variable:end}*/

extern usb_host_handle g_HostHandle;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();

    NVIC_ClearPendingIRQ(USB0_IRQn);
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);

    POWER_DisablePD(kPDRUNCFG_PD_USBFSPHY); /*< Turn on USB0 Phy */

    /* reset the IP to make sure it's in reset state. */
    RESET_PeripheralReset(kUSB0_DEV_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HSL_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HMR_RST_SHIFT_RSTn);

#if (defined USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI)
    POWER_DisablePD(kPDRUNCFG_PD_USBFSPHY); /*< Turn on USB Phy */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1, false);
    CLOCK_AttachClk(kFRO_HF_to_USB0);
    /* enable usb0 host clock */
    CLOCK_EnableClock(kCLOCK_Usbhmr0);
    CLOCK_EnableClock(kCLOCK_Usbhsl0);
    /* enable usb0 device clock */
    CLOCK_EnableClock(kCLOCK_Usbd0);
    *((uint32_t *)(USBFSH_BASE + 0x5C)) &= ~USBFSH_PORTMODE_DEV_ENABLE_MASK;
    while (SYSCON->USB0NEEDCLKSTAT & (SYSCON_USB0NEEDCLKSTAT_DEV_NEEDCLK_MASK))
    {
        __ASM("nop");
    }
    /* disable usb0 device clock */
    CLOCK_DisableClock(kCLOCK_Usbd0);
    /* disable usb0 host clock */
    CLOCK_DisableClock(kCLOCK_Usbhsl0);
    CLOCK_DisableClock(kCLOCK_Usbhmr0);
#endif
}

void BOARD_DeinitPins(void)
{
}

void SW_IntControl(uint8_t enable)
{
    if (enable)
    {
        g_HostHidMouse.selfWakeup = 0U;
    }
    HAL_GpioWakeUpSetting(g_gpioHandle, enable);
}

void SW_Callback(void *param)
{
    g_HostHidMouse.selfWakeup = 1U;
    SW_IntControl(0);
}

void SW_Init(void)
{
    hal_gpio_pin_config_t s_GpioInputPin;
    s_GpioInputPin.direction = kHAL_GpioDirectionIn;
    s_GpioInputPin.port      = BOARD_SW1_GPIO_PORT;
    s_GpioInputPin.pin       = BOARD_SW1_GPIO_PIN;

    HAL_GpioInit(g_gpioHandle, &s_GpioInputPin);
    HAL_GpioInstallCallback(g_gpioHandle, SW_Callback, NULL);
    HAL_GpioSetTriggerMode(g_gpioHandle, kHAL_GpioInterruptFallingEdge);
}

char *SW_GetName(void)
{
    return BOARD_SW1_NAME;
}

void HW_TimerCallback(void *param)
{
    g_HostHidMouse.hwTick++;
    USB_HostUpdateHwTick(g_HostHandle, g_HostHidMouse.hwTick);
}

void HW_TimerInit(void)
{
    hal_timer_config_t halTimerConfig;
    halTimerConfig.timeout            = 1000;
    halTimerConfig.srcClock_Hz        = TIMER_SOURCE_CLOCK;
    halTimerConfig.instance           = 0U;
    hal_timer_handle_t halTimerHandle = &g_halTimerHandle[0];
    HAL_TimerInit(halTimerHandle, &halTimerConfig);
    HAL_TimerInstallCallback(halTimerHandle, HW_TimerCallback, NULL);
}

void HW_TimerControl(uint8_t enable)
{
    if (enable)
    {
        HAL_TimerEnable(g_halTimerHandle);
    }
    else
    {
        HAL_TimerDisable(g_halTimerHandle);
    }
}

void USB_LowpowerModeInit(void)
{
    SW_Init();
    HW_TimerInit();
}

void USB_PreLowpowerMode(void)
{
    if (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)
    {
        systemTickControl = SysTick->CTRL;
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    }
    __disable_irq();
#if ((defined(USB_HOST_CONFIG_OHCI)) && (USB_HOST_CONFIG_OHCI > 0U))
    if (SYSCON->USB0NEEDCLKSTAT & (SYSCON_USB0NEEDCLKSTAT_DEV_NEEDCLK_MASK))
    {
        /* enable usb0 device clock */
        CLOCK_EnableClock(kCLOCK_Usbd0);
        while (SYSCON->USB0NEEDCLKSTAT & (SYSCON_USB0NEEDCLKSTAT_DEV_NEEDCLK_MASK))
        {
            __ASM("nop");
        }
        /* disable usb0 device clock */
        CLOCK_DisableClock(kCLOCK_Usbd0);
    }
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
#endif

    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK); /*!< Switch to 12MHz first to ensure we can change voltage without
                                          accidentally being below the voltage for current speed */
    ANACTRL->FRO192M_CTRL &= ~(ANACTRL_FRO192M_CTRL_USBCLKADJ_MASK);
    POWER_SetVoltageForFreq(12000000U); /*!< Set voltage for core */
}

uint8_t USB_EnterLowpowerMode(void)
{
    /* Enter Deep Sleep mode */
    uint32_t exclude_from_pd[2];
    uint32_t wakeup_interrupts[4];
    exclude_from_pd[0]   = APP_EXCLUDE_FROM_DEEPSLEEP;
    wakeup_interrupts[0] = APP_DEEPSLEEP_WAKEUP_SOURCE;
    POWER_EnterDeepSleep(exclude_from_pd, 0x0, wakeup_interrupts, 0x0);
    return kStatus_Success;
}

void USB_PostLowpowerMode(void)
{
    __enable_irq();
    SysTick->CTRL = systemTickControl;
}

void USB_ControllerSuspended(void)
{
#if ((defined(USB_HOST_CONFIG_OHCI)) && (USB_HOST_CONFIG_OHCI > 0U))
    while (SYSCON->USB0NEEDCLKSTAT & (SYSCON_USB0NEEDCLKSTAT_HOST_NEEDCLK_MASK))
    {
        __ASM("nop");
    }
    SYSCON->USB0NEEDCLKCTRL |= SYSCON_USB0NEEDCLKCTRL_POL_FS_HOST_NEEDCLK_MASK;
#endif
}

void USB0_NEEDCLK_IRQHandler(void)
{
}

void USB1_NEEDCLK_IRQHandler(void)
{
}

void USB_WaitClockLocked(void)
{
    BOARD_BootClockPLL150M();
}

#if (defined(USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI > 0U))
void USB0_IRQHandler(void)
{
    USB_HostOhciIsrFunction(g_HostHandle);
}
#endif /* USB_HOST_CONFIG_OHCI */

void USB_HostClockInit(void)
{
#if ((defined USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI > 0U))
    CLOCK_EnableUsbfs0HostClock(kCLOCK_UsbfsSrcPll1, 48000000U);
#endif /* USB_HOST_CONFIG_OHCI */
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;
#if ((defined USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI > 0U))
    IRQn_Type usbHsIrqs[] = {(IRQn_Type)USB0_IRQn};
    irqNumber             = usbHsIrqs[CONTROLLER_ID - kUSB_ControllerOhci0];
#endif /* USB_HOST_CONFIG_OHCI */

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
#if ((defined USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI > 0U))
    USB_HostOhciTaskFunction(param);
#endif /* USB_HOST_CONFIG_OHCI */
}
/*${function:end}*/

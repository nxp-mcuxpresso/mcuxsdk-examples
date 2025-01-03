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
#include "usb_phy.h"
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
#define APP_EXCLUDE_FROM_DEEPSLEEP  (kPDRUNCFG_PD_LDOFLASHNV | kPDRUNCFG_PD_USB0_PHY | kPDRUNCFG_PD_LDOMEM)
#define APP_DEEPSLEEP_WAKEUP_SOURCE (WAKEUP_GPIO_GLOBALINT0 | WAKEUP_USB0_NEEDCLK)
#endif
#if ((defined(USB_HOST_CONFIG_IP3516HS)) && (USB_HOST_CONFIG_IP3516HS > 0U))
#define APP_EXCLUDE_FROM_DEEPSLEEP \
    (kPDRUNCFG_PD_LDOFLASHNV | kPDRUNCFG_PD_USB1_PHY | kPDRUNCFG_PD_LDOMEM | kPDRUNCFG_PD_LDOUSBHS)
#define APP_DEEPSLEEP_WAKEUP_SOURCE (WAKEUP_GPIO_GLOBALINT0 | WAKEUP_USB1_NEEDCLK)
#endif
/*${macro:end}*/

/*${variable:start}*/
#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
extern usb_host_mouse_instance_t g_HostHidMouse;
extern usb_host_handle g_HostHandle;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
uint32_t g_gpioHandle[(HAL_GPIO_HANDLE_SIZE + 3) / 4];
/*${variable:end}*/

extern usb_host_handle g_HostHandle;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    NVIC_ClearPendingIRQ(USB0_IRQn);
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
    NVIC_ClearPendingIRQ(USB1_IRQn);
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);

    POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*< Turn on USB0 Phy */
    POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY); /*< Turn on USB1 Phy */

    /* reset the IP to make sure it's in reset state. */
    RESET_PeripheralReset(kUSB0D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HSL_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HMR_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1H_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1RAM_RST_SHIFT_RSTn);
#if (defined USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS)
    POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY); /*< Turn on USB Phy */
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0HostClock(kCLOCK_UsbSrcUnused, 0U);
    /* enable usb1 host clock */
    CLOCK_EnableClock(kCLOCK_Usbh1);
    /* enable usb1 device clock */
    CLOCK_EnableClock(kCLOCK_Usbd1);
    *((uint32_t *)(USBHSH_BASE + 0x50)) &= ~USBHSH_PORTMODE_DEV_ENABLE_MASK;
    *((uint32_t *)(USBHSH_BASE + 0x50)) &= ~USBHSH_PORTMODE_SW_CTRL_PDCOM_MASK;
    /* disable usb1 device clock */
    CLOCK_DisableClock(kCLOCK_Usbd1);
    /* disable usb1 host clock */
    CLOCK_DisableClock(kCLOCK_Usbh1);
#endif
#if (defined USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI)
    POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*< Turn on USB Phy */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1, false);
    CLOCK_AttachClk(kFRO_HF_to_USB0_CLK);
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
#if ((defined(USB_HOST_CONFIG_IP3516HS)) && (USB_HOST_CONFIG_IP3516HS > 0U))
    if (SYSCON->USB1NEEDCLKSTAT & (SYSCON_USB1NEEDCLKSTAT_DEV_NEEDCLK_MASK))
    {
        /* enable usb1 device clock */
        CLOCK_EnableClock(kCLOCK_Usbd1);
        while (SYSCON->USB1NEEDCLKSTAT & (SYSCON_USB1NEEDCLKSTAT_DEV_NEEDCLK_MASK))
        {
            __ASM("nop");
        }
        /* disable usb1 device clock */
        CLOCK_DisableClock(kCLOCK_Usbd1);
    }
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);
    *((uint32_t *)(USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_SW_CTRL_PDCOM_MASK;
    *((uint32_t *)(USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_SW_PDCOM_MASK;

    /* clear this field to decrease power consumption */
    USBPHY->USB1_VBUS_DETECT_CLR = USBPHY_USB1_VBUS_DETECT_PWRUP_CMPS_MASK;
#endif

    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK); /*!< Switch to 12MHz first to ensure we can change voltage without
                                          accidentally being below the voltage for current speed */
    ANACTRL->FRO192M_CTRL &= ~(ANACTRL_FRO192M_CTRL_USBCLKADJ_MASK);
    POWER_SetVoltageForFreq(12000000U); /*!< Set voltage for core */
}

uint8_t USB_EnterLowpowerMode(void)
{
    /* Enter Deep Sleep mode */
    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP, 0x0, APP_DEEPSLEEP_WAKEUP_SOURCE, 0x0);
    return kStatus_Success;
}

void USB_PostLowpowerMode(void)
{
    __enable_irq();
#if ((defined(USB_HOST_CONFIG_IP3516HS)) && (USB_HOST_CONFIG_IP3516HS > 0U))
    *((uint32_t *)(USBHSH_BASE + 0x50)) &= ~USBHSH_PORTMODE_SW_PDCOM_MASK;
    *((uint32_t *)(USBHSH_BASE + 0x50)) &= ~USBHSH_PORTMODE_SW_CTRL_PDCOM_MASK;
#endif
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
#if ((defined(USB_HOST_CONFIG_IP3516HS)) && (USB_HOST_CONFIG_IP3516HS > 0U))
    while (SYSCON->USB1NEEDCLKSTAT & (SYSCON_USB1NEEDCLKSTAT_HOST_NEEDCLK_MASK))
    {
        __ASM("nop");
    }
    SYSCON->USB1NEEDCLKCTRL |= SYSCON_USB1NEEDCLKCTRL_POL_HS_HOST_NEEDCLK_MASK;
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
    BOARD_InitBootClocks();
}

#if (defined(USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI > 0U))
void USB0_IRQHandler(void)
{
    USB_HostOhciIsrFunction(g_HostHandle);
}
#endif /* USB_HOST_CONFIG_OHCI */
#if (defined(USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS > 0U))
void USB1_IRQHandler(void)
{
    USB_HostIp3516HsIsrFunction(g_HostHandle);
}
#endif /* USB_HOST_CONFIG_IP3516HS */

void USB_HostClockInit(void)
{
#if ((defined USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS > 0U))
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif

#if ((defined USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI > 0U))
    CLOCK_EnableUsbfs0HostClock(kCLOCK_UsbfsSrcPll1, 48000000U);
#if ((defined FSL_FEATURE_USBFSH_USB_RAM) && (FSL_FEATURE_USBFSH_USB_RAM > 0U))
    for (int i = 0; i < (FSL_FEATURE_USBFSH_USB_RAM >> 2); i++)
    {
        ((uint32_t *)FSL_FEATURE_USBFSH_USB_RAM_BASE_ADDRESS)[i] = 0U;
    }
#endif
#endif /* USB_HOST_CONFIG_OHCI */

#if ((defined USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS > 0U))
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_UsbPhySrcExt, BOARD_XTAL0_CLK_HZ);
    CLOCK_EnableUsbhs0HostClock(kCLOCK_UsbSrcUnused, 0U);
    USB_EhciLowPowerPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#if ((defined FSL_FEATURE_USBHSH_USB_RAM) && (FSL_FEATURE_USBHSH_USB_RAM > 0U))
    for (int i = 0; i < (FSL_FEATURE_USBHSH_USB_RAM >> 2); i++)
    {
        ((uint32_t *)FSL_FEATURE_USBHSH_USB_RAM_BASE_ADDRESS)[i] = 0U;
    }
#endif
#endif /* USB_HOST_CONFIG_IP3511HS */
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;
#if ((defined USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI > 0U))
    IRQn_Type usbHsIrqs[] = {(IRQn_Type)USB0_IRQn};
    irqNumber             = usbHsIrqs[CONTROLLER_ID - kUSB_ControllerOhci0];
#endif /* USB_HOST_CONFIG_OHCI */
#if ((defined USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS > 0U))
    IRQn_Type usbHsIrqs[] = {(IRQn_Type)USB1_IRQn};
    irqNumber             = usbHsIrqs[CONTROLLER_ID - kUSB_ControllerIp3516Hs0];
#endif /* USB_HOST_CONFIG_IP3511HS */

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
#if ((defined USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS > 0U))
    USB_HostIp3516HsTaskFunction(param);
#endif /* USB_HOST_CONFIG_IP3516HS */
}
/*${function:end}*/

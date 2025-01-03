/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${standard_header:start}*/
#include <stdbool.h>
/*${standard_header:end}*/
/*${header:start}*/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "fsl_device_registers.h"
#include "mouse.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "mouse.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_adapter_gpio.h"
#include "fsl_adapter_timer.h"
/*${header:end}*/
/*${macro:start}*/
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                               \
    (SYSCON_PDRUNCFG_PDEN_SRAMX_MASK | SYSCON_PDRUNCFG_PDEN_SRAM0_MASK | SYSCON_PDSLEEPCFG_PDEN_SRAM1_2_3_MASK | \
     SYSCON_PDSLEEPCFG_PDEN_USB_RAM_MASK | SYSCON_PDRUNCFG_PDEN_VD6_MASK)
#define APP_EXCLUDE_FROM_DEEPSLEEP1 0U
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                               \
    (SYSCON_PDRUNCFG_PDEN_SRAMX_MASK | SYSCON_PDRUNCFG_PDEN_SRAM0_MASK | SYSCON_PDSLEEPCFG_PDEN_SRAM1_2_3_MASK | \
     SYSCON_PDSLEEPCFG_PDEN_USB_RAM_MASK | SYSCON_PDRUNCFG_PDEN_VD2_ANA_MASK | SYSCON_PDRUNCFG_PDEN_VD5_MASK |   \
     SYSCON_PDRUNCFG_PDEN_VD6_MASK)
#define APP_EXCLUDE_FROM_DEEPSLEEP1 ((SYSCON_PDRUNCFG_PDEN_USB1_PHY_MASK | SYSCON_PDRUNCFG_PDEN_SYSOSC_MASK))
#endif
/*${macro:end}*/
/*${variable:start}*/
#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
uint32_t g_gpioHandle[(HAL_GPIO_HANDLE_SIZE + 3) / 4];
uint32_t isConnectedToFsHost = 0U;
uint32_t isConnectedToHsHost = 0U;
/*${variable:end}*/
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* reset USB0 and USB1 device */
    RESET_PeripheralReset(kUSB0D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HMR_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB0HSL_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1H_RST_SHIFT_RSTn);

    NVIC_ClearPendingIRQ(USB0_IRQn);
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
    NVIC_ClearPendingIRQ(USB1_IRQn);
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);

    /* reset USB0 and USB1 device */
    RESET_PeripheralReset(kUSB0D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);

    NVIC_ClearPendingIRQ(USB0_IRQn);
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
    NVIC_ClearPendingIRQ(USB1_IRQn);
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);

    BOARD_InitBootPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();
#if (defined USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS)
    POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY);
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0DeviceClock(kCLOCK_UsbSrcUsbPll, (48000000U));
    /* enable usb1 host clock */
    CLOCK_EnableClock(kCLOCK_Usbh1);
    *((uint32_t *)(USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_DEV_ENABLE_MASK;
    *((uint32_t *)(USBHSH_BASE + 0x50)) &= ~USBHSH_PORTMODE_SW_CTRL_PDCOM_MASK;
    while (SYSCON->USB1CLKSTAT & (SYSCON_USB1CLKSTAT_HOST_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    /* enable usb1 host clock */
    CLOCK_DisableClock(kCLOCK_Usbh1);
#endif
#if (defined USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS)
    POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*< Turn on USB Phy */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1, false);
    CLOCK_AttachClk(kFRO_HF_to_USB0_CLK);
    /* enable usb0 host clock */
    CLOCK_EnableClock(kCLOCK_Usbhmr0);
    CLOCK_EnableClock(kCLOCK_Usbhsl0);
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    *((uint32_t *)(USBFSH_BASE + 0x5C)) |= USBFSH_PORTMODE_DEV_ENABLE_MASK;
    while (SYSCON->USB0CLKSTAT & (SYSCON_USB0CLKSTAT_HOST_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
    /* disable usb0 host clock */
    CLOCK_DisableClock(kCLOCK_Usbhsl0);
    CLOCK_DisableClock(kCLOCK_Usbhmr0);
#endif
}
void USB_DeviceDisconnected(void)
{
    isConnectedToFsHost = 0U;
}
/*
 * This is a work-around to fix the HS device Chirping issue.
 * The device (IP3511HS controller) will not work sometimes when the cable
 * is attached at the first time after a Power-on Reset.
 */
void USB_DeviceHsPhyChirpIssueWorkaround(void)
{
    uint32_t startFrame = USBHSD->INFO & USBHSD_INFO_FRAME_NR_MASK;
    uint32_t currentFrame;
    uint32_t isConnectedToFsHostFlag = 0U;
    if ((!isConnectedToHsHost) && (!isConnectedToFsHost))
    {
        if (((USBHSD->DEVCMDSTAT & USBHSD_DEVCMDSTAT_Speed_MASK) >> USBHSD_DEVCMDSTAT_Speed_SHIFT) == 0x01U)
        {
            USBHSD->DEVCMDSTAT = (USBHSD->DEVCMDSTAT & (~(0x0F000000U | USBHSD_DEVCMDSTAT_PHY_TEST_MODE_MASK))) |
                                 USBHSD_DEVCMDSTAT_PHY_TEST_MODE(0x05U);
            HAL_TimerDisable(g_halTimerHandle); /* Disabling timer will clear the current timer counter */
            g_UsbDeviceHidMouse.hwTick = 0;
            HAL_TimerEnable(g_halTimerHandle);
            usb_echo("The USB device PHY chirp work-around is working\r\n");
            while (g_UsbDeviceHidMouse.hwTick < 100U)
            {
            }
            currentFrame = USBHSD->INFO & USBHSD_INFO_FRAME_NR_MASK;
            if (currentFrame != startFrame)
            {
                isConnectedToHsHost = 1U;
            }
            else
            {
                HAL_TimerDisable(g_halTimerHandle);
                g_UsbDeviceHidMouse.hwTick = 0;
                HAL_TimerEnable(g_halTimerHandle);
                while (g_UsbDeviceHidMouse.hwTick < 1U)
                {
                }
                currentFrame = USBHSD->INFO & USBHSD_INFO_FRAME_NR_MASK;
                if (currentFrame != startFrame)
                {
                    isConnectedToHsHost = 1U;
                }
                else
                {
                    isConnectedToFsHostFlag = 1U;
                }
            }
            USBHSD->DEVCMDSTAT = (USBHSD->DEVCMDSTAT & (~(0x0F000000U | USBHSD_DEVCMDSTAT_PHY_TEST_MODE_MASK)));
            USBHSD->DEVCMDSTAT = (USBHSD->DEVCMDSTAT & (~(0x0F000000U | USBHSD_DEVCMDSTAT_DCON_MASK)));
            HAL_TimerDisable(g_halTimerHandle);
            g_UsbDeviceHidMouse.hwTick = 0;
            HAL_TimerEnable(g_halTimerHandle);
            while (g_UsbDeviceHidMouse.hwTick < 510U)
            {
            }
            USBHSD->DEVCMDSTAT = (USBHSD->DEVCMDSTAT & (~(0x0F000000U))) | USB_DEVCMDSTAT_DCON_C_MASK;
            USBHSD->DEVCMDSTAT =
                (USBHSD->DEVCMDSTAT & (~(0x0F000000U))) | USBHSD_DEVCMDSTAT_DCON_MASK | USB_DEVCMDSTAT_DRES_C_MASK;
            if (isConnectedToFsHostFlag)
            {
                isConnectedToFsHost = 1U;
            }
        }
    }
}
void BOARD_DeinitPins(void)
{
}
void SW_IntControl(uint8_t enable)
{
    if (enable)
    {
        g_UsbDeviceHidMouse.selfWakeup = 0U;
    }
    HAL_GpioWakeUpSetting(g_gpioHandle, enable);
}
void SW_Callback(void *param)
{
    g_UsbDeviceHidMouse.selfWakeup = 1U;
    SW_IntControl(0);
}
void SW_Init(void)
{
    hal_gpio_pin_config_t s_GpioInputPin;
    s_GpioInputPin.direction = kHAL_GpioDirectionIn;
    s_GpioInputPin.port      = BOARD_SW2_GPIO_PORT;
    s_GpioInputPin.pin       = BOARD_SW2_GPIO_PIN;

    HAL_GpioInit(g_gpioHandle, &s_GpioInputPin);
    HAL_GpioInstallCallback(g_gpioHandle, SW_Callback, NULL);
    HAL_GpioSetTriggerMode(g_gpioHandle, kHAL_GpioInterruptFallingEdge);
}
char *SW_GetName(void)
{
    return BOARD_SW2_NAME;
}
void HW_TimerCallback(void *param)
{
    g_UsbDeviceHidMouse.hwTick++;
    USB_DeviceUpdateHwTick(g_UsbDeviceHidMouse.deviceHandle, g_UsbDeviceHidMouse.hwTick);
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
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    if (SYSCON->USB0CLKSTAT & (SYSCON_USB0CLKSTAT_HOST_NEED_CLKST_MASK))
    {
        /* enable usb0 host clock */
        CLOCK_EnableClock(kCLOCK_Usbhmr0);
        CLOCK_EnableClock(kCLOCK_Usbhsl0);
        while (SYSCON->USB0CLKSTAT & (SYSCON_USB0CLKSTAT_HOST_NEED_CLKST_MASK))
        {
            __ASM("nop");
        }
        /* disable usb0 host clock */
        CLOCK_DisableClock(kCLOCK_Usbhsl0);
        CLOCK_DisableClock(kCLOCK_Usbhmr0);
    }
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
    EnableDeepSleepIRQ(USB0_NEEDCLK_IRQn);
    SYSCON->STARTER[0] |= SYSCON_STARTER_USB0_NEEDCLK_MASK;
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    if (SYSCON->USB1CLKSTAT & (SYSCON_USB1CLKSTAT_HOST_NEED_CLKST_MASK))
    {
        /* enable usb1 host clock */
        CLOCK_EnableClock(kCLOCK_Usbh1);
        while (SYSCON->USB1CLKSTAT & (SYSCON_USB1CLKSTAT_HOST_NEED_CLKST_MASK))
        {
            __ASM("nop");
        }
        /* disable usb1 host clock */
        CLOCK_DisableClock(kCLOCK_Usbh1);
    }
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);
    EnableDeepSleepIRQ(USB1_NEEDCLK_IRQn);

    SYSCON->STARTER[1] |= SYSCON_STARTER_USB1_ACT_MASK;
#endif
#if 0
    CLOCK_AttachClk(
        kFRO12M_to_MAIN_CLK);          /*!< Switch to 12MHz first to ensure we can change voltage without accidentally
                                       being below the voltage for current speed */
    SYSCON->FROCTRL &= ~(SYSCON_FROCTRL_USBCLKADJ_MASK | SYSCON_FROCTRL_HSPDCLK_MASK);
    POWER_SetVoltageForFreq(12000000U); /*!< Set voltage for core */
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    SYSCON->USB0CLKDIV  = (1 << 29); /* artf208952 has been fixed, refer to USB_ControllerSuspended */
    SYSCON->MAINCLKSELA = 0;
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    SYSCON->USB1CLKDIV  = (1 << 29);
    SYSCON->MAINCLKSELA = 0;
#endif
}
uint8_t USB_EnterLowpowerMode(void)
{
    /* Enter Deep Sleep mode */
    POWER_EnterDeepSleep((uint64_t)(APP_EXCLUDE_FROM_DEEPSLEEP | ((uint64_t)APP_EXCLUDE_FROM_DEEPSLEEP1 << 32)));
    return kStatus_Success;
}
void USB_PostLowpowerMode(void)
{
    __enable_irq();
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    DisableDeepSleepIRQ(USB0_NEEDCLK_IRQn);
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    DisableDeepSleepIRQ(USB1_NEEDCLK_IRQn);
#endif
#if 0
    BOARD_BootClockFROHF96M();
#endif
}
void USB_ControllerSuspended(void)
{
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    while (SYSCON->USB0CLKSTAT & (SYSCON_USB0CLKSTAT_DEV_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
    SYSCON->USB0CLKCTRL |= SYSCON_USB0CLKCTRL_POL_FS_DEV_CLK_MASK;
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    while (SYSCON->USB1CLKSTAT & (SYSCON_USB1CLKSTAT_DEV_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
    SYSCON->USB1CLKCTRL |= SYSCON_USB1CLKCTRL_POL_FS_DEV_CLK_MASK;
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
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
    SYSCON->MAINCLKSELA = 3; /* only applies to BOARD_BootClockFROHF96M */
    SYSCON->USB0CLKDIV  = 1;
    /* artf208952 has been fixed, refer to USB_ControllerSuspended */ /* only applies to kCLOCK_UsbSrcFro and
                                                                         BOARD_BootClockFROHF96M */
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    SYSCON->MAINCLKSELA = 3; /* only applies to BOARD_BootClockFROHF96M */
    SYSCON->USB1CLKDIV  = 0;
#endif
#if ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
    if ((SYSCON->USB1CLKSEL & SYSCON_USB1CLKSEL_SEL_MASK) == SYSCON_USB1CLKSEL_SEL(0x2))
    {
        while (CLOCK_IsUsbPLLLocked() == false)
        {
        }
    }
#endif
}
#if (defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
void USB1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif
void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    /* enable USB IP clock */
    CLOCK_EnableUsbfs0DeviceClock(kCLOCK_UsbSrcFro, CLOCK_GetFroHfFreq());
#if defined(FSL_FEATURE_USB_USB_RAM) && (FSL_FEATURE_USB_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USB_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USB_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif

#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0DeviceClock(kCLOCK_UsbSrcUsbPll, (48000000U));
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif
#endif
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USB_IRQS;
    irqNumber                    = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Fs0];
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber                    = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
#endif
    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    USB_DeviceLpcIp3511TaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    USB_DeviceLpcIp3511TaskFunction(deviceHandle);
#endif
}
#endif
/*${function:end}*/

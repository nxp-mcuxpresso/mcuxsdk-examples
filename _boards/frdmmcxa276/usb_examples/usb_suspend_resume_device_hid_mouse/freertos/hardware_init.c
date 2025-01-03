/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${standard_header:start}*/
#include <stdbool.h>
/*${standard_header:end}*/

/*${header:start}*/
#include "fsl_device_registers.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "usb_device_hid.h"
#include "mouse.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "board.h"
#include "fsl_port.h"
#include "fsl_adapter_gpio.h"
#include "fsl_adapter_timer.h"
#include "fsl_cmc.h"
/*${header:end}*/

/*${variable:start}*/
#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_Fro16K)
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
uint32_t g_gpioHandle[(HAL_GPIO_HANDLE_SIZE + 3) / 4];
/*${variable:end}*/

/*${prototype:start}*/
void USB_WaitClockLocked(void);
/*${prototype:end}*/

extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBUTTONsPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

/*!
 * @brief Initialize all pins used in this example
 *
 */
void BOARD_InitPins(void)
{
    BOARD_InitDEBUG_UARTPins();
}

/*!
 * @brief De-initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
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
    s_GpioInputPin.port      = 1;
    s_GpioInputPin.pin       = BOARD_SW2_GPIO_PIN;

    HAL_GpioInit(g_gpioHandle, &s_GpioInputPin);
    HAL_GpioInstallCallback(g_gpioHandle, SW_Callback, NULL);
    HAL_GpioSetTriggerMode(g_gpioHandle, kHAL_GpioInterruptFallingEdge);

    NVIC_SetPriority(BOARD_SW2_IRQ, 1U);
    NVIC_EnableIRQ(BOARD_SW2_IRQ);
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

    halTimerConfig.clockSrcSelect     = 1u;
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
}

uint8_t USB_EnterLowpowerMode(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepSleepMode;

    CMC_EnterLowPowerMode(CMC, &config);

    return 0;
}

void USB_PostLowpowerMode(void)
{
    __enable_irq();
}

void USB_ControllerSuspended(void)
{
}

void USB_WaitClockLocked(void)
{
}

#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
    CLOCK_EnableUsbfsClock();
#endif
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USBFS_IRQS;
    irqNumber                  = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];
#endif
    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    USB_DeviceKhciTaskFunction(deviceHandle);
#endif
}
#endif
/*${function:end}*/

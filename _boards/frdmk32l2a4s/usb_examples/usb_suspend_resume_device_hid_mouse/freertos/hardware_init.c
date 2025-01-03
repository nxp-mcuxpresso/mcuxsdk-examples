/*
 * Copyright 2019 NXP
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
#include "usb_device_class.h"
#include "usb_device_hid.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "fsl_device_registers.h"
#include "mouse.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "mouse.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lpit.h"
#include "fsl_port.h"
#include "fsl_smc.h"

#include "fsl_adapter_timer.h"
/*${header:end}*/
/*${variable:start}*/
#define TIMER_SOURCE_CLOCK CLOCK_GetIpFreq(kCLOCK_Lpit0)
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
/*${variable:end}*/
/*${prototype:start}*/
void USB_WaitClockLocked(void);
/*${prototype:end}*/
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
/*${function:start}*/
void BOARD_InitHardware(void)
{
    gpio_pin_config_t pinConfig = {kGPIO_DigitalInput, 1U};
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /* Set the LLWU pin */
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &pinConfig);

    /* Set the source for the LPIT module */
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcSircAsync);
}
/*!
 * @brief De-initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
void BOARD_DeinitPins(void)
{
    /* Affects PORTE_PCR0 register */
    PORT_SetPinMux(PORTB, 16u, kPORT_PinDisabledOrAnalog);
    /* Affects PORTE_PCR1 register */
    PORT_SetPinMux(PORTB, 17u, kPORT_PinDisabledOrAnalog);
    /* Gate the port clock */
    CLOCK_DisableClock(kCLOCK_PortB);
}
void BOARD_SW3_IRQ_HANDLER(void)
{
    if ((1U << BOARD_SW3_GPIO_PIN) & PORT_GetPinsInterruptFlags(BOARD_SW3_PORT))
    {
        /* Disable interrupt. */
        PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(BOARD_SW3_PORT, (1U << BOARD_SW3_GPIO_PIN));
        g_UsbDeviceHidMouse.selfWakeup = 1U;
    }
}
void SW_IntControl(uint8_t enable)
{
    if (enable)
    {
        g_UsbDeviceHidMouse.selfWakeup = 0U;
        PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, kPORT_InterruptFallingEdge);
    }
    else
    {
        PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, kPORT_InterruptOrDMADisabled);
    }
}
void SW_Callback(void *param)
{
    g_UsbDeviceHidMouse.selfWakeup = 1U;
    SW_IntControl(0);
}
void SW_Init(void)
{
    NVIC_SetPriority(BOARD_SW3_IRQ, 1U);
    NVIC_EnableIRQ(BOARD_SW3_IRQ);
}
char *SW_GetName(void)
{
    return BOARD_SW3_NAME;
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
    /* Set to allow entering vlps mode */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeVlp);
    SW_Init();
    HW_TimerInit();
}
void USB_PreLowpowerMode(void)
{
    SMC_PreEnterStopModes();
    SIM->SOPT1 |= SIM_SOPT1_USBSSTBY_MASK;
}
uint8_t USB_EnterLowpowerMode(void)
{
    /* Enter Deep Sleep mode */
    return SMC_SetPowerModeVlps(SMC);
}
void USB_PostLowpowerMode(void)
{
    USB_WaitClockLocked();
    SMC_PostExitStopModes();
}
void USB_ControllerSuspended(void)
{
}
void USB_WaitClockLocked(void)
{
#if (defined(FSL_FEATURE_SOC_MCG_COUNT) && (FSL_FEATURE_SOC_MCG_COUNT > 0U))
    /* Wait for PLL lock. */
    while (!(kMCG_Pll0LockFlag & CLOCK_GetStatusFlags()))
    {
    }
    CLOCK_SetPeeMode();
#endif
}

void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    SystemCoreClockUpdate();
    CLOCK_EnableUsbfs0Clock(kCLOCK_IpSrcFircAsync, CLOCK_GetFreq(kCLOCK_ScgFircAsyncDiv1Clk));
/*
 * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
 * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
 * RAM can not be used and the memory can't be accessed.
 */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber                  = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceKhciTaskFunction(deviceHandle);
}
#endif
/*${function:end}*/

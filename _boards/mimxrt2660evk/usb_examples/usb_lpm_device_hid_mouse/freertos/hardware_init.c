/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "mouse.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "fsl_adapter_timer.h"
#include "fsl_power.h"
#include "board.h"

/* Get source clock for the HAL timer (LPTMR) adapter. */
#define LPTMR_SOURCE_CLOCK (32 * 1000U)

uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];

extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

static const power_deep_sleep_config_t s_usbDeepSleepConfig = {
    .setCpuDomainEvent   = kPDCON_EventNoneOrActive,
    .setNpuDomainEvent   = kPDCON_EventNoneOrActive,
    .setCommDomainEvent  = kPDCON_EventNoneOrActive,
    .setMediaDomainEvent = kPDCON_EventNoneOrActive,

    .setPmuMode        = 0x02U,
    .setDcdcMode       = kPMU_DcdcModePWM,
    .setCoreLevel      = 0x11U,
    .setLdo0V8Mode     = kPMU_LdoModeHP,
    .setLdo1V8Mode     = kPMU_LdoModeHP,
    .setLdoVdda1V8Mode = 0x01U,
    .enableHqref       = true,
    .enableSensors     = 0x00U,

    .enableLdoa0V8ClockSource  = true,
    .enableFro192MClockSource  = true,
    .enableFro12MClockSource   = true,
    .enableMainPllClockSource  = false,
    .enableCorePllClockSource  = false,
    .enableSysPllClockSource   = false,
    .enableLdoq0V8ClockSource  = false,
    .enableSxoscClockSource    = false,
    .enableFro12MLpClockSource = true,

    .enableComputeRootClock = false,
    .enableMainRootClock    = false,
    .enableWakeRootClock    = true,
    .enableCommRootClock    = false,
    .enableMediaRootClock   = false,
    .enableAudioRootClock   = false,
    .enableWake1MRootClock  = true,
    .enableWake2MRootClock  = true,

    .setWakeSramMode      = kMEMCON_PowerModeActive,
    .setComputeSram01Mode = kMEMCON_PowerModeRetention,
    .setOcram0M23Mode     = kMEMCON_PowerModeRetention,
    .setOcram0M4Mode      = kMEMCON_PowerModeRetention,
    .setOcram0M5Mode      = kMEMCON_PowerModeRetention,
    .setOcram1M0Mode      = kMEMCON_PowerModeRetention,
    .setOcram1M1Mode      = kMEMCON_PowerModeRetention,
    .setOcram2M0Mode      = kMEMCON_PowerModeRetention,
    .setOcram2M1Mode      = kMEMCON_PowerModeRetention,
    .setLlcCacheMode      = kMEMCON_PowerModeRetention,
    .setAudio8KbMode      = kMEMCON_PowerModeRetention,
    .setM85TcmMode        = kMEMCON_PowerModeRetention,
    .setNpuTcmMode        = kMEMCON_PowerModeRetention,
    .setCommTcmMode       = kMEMCON_PowerModeActive,
    .setMediaTcmMode      = kMEMCON_PowerModeActive,
};

void BOARD_InitPins(void)
{
}

void BOARD_DeinitPins(void)
{
}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitBUTTONsPins();

    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };
    GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &sw_config);
    GPIO_SetPinInterruptChannel(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, kGPIO_InterruptOutput0);
    GPIO_SetPinInterruptConfig(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, kGPIO_InterruptFallingEdge);
}

void SW_IntControl(uint8_t enable)
{
    if (enable)
    {
        EnableIRQ(BOARD_USER_BUTTON_IRQ);
    }
    else
    {
        DisableIRQ(BOARD_USER_BUTTON_IRQ);
    }
}

char *SW_GetName(void)
{
    return BOARD_USER_BUTTON_NAME;
}

void HW_TimerControl(uint8_t enable)
{
    hal_timer_handle_t halTimerHandle = &g_halTimerHandle[0];

    if (enable)
    {
        HAL_TimerEnable(halTimerHandle);
    }
    else
    {
        HAL_TimerDisable(halTimerHandle);
    }
}

void SW_Init(void)
{
    NVIC_SetPriority(BOARD_USER_BUTTON_IRQ, 1U);
    NVIC_EnableIRQ(BOARD_USER_BUTTON_IRQ);
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
    halTimerConfig.srcClock_Hz        = LPTMR_SOURCE_CLOCK;
    halTimerConfig.instance           = 0U;
    hal_timer_handle_t halTimerHandle = &g_halTimerHandle[0];
    HAL_TimerInit(halTimerHandle, &halTimerConfig);
    HAL_TimerInstallCallback(halTimerHandle, HW_TimerCallback, NULL);
}

void USB_LowpowerModeInit(void)
{
    SW_Init();
    HW_TimerInit();
}

/*!
 * @brief Arm the wakeup sources before the device enters Deep Sleep on a USB bus suspend.
 *
 * Called from USB_PowerPreSwitchHook(), i.e. after the HAL timer has been stopped and the debug
 * console has been de-initialised. Programming the POWERCON wakeup mask is the only preparation
 * needed: the clock tree and the run mode are left untouched, so the device suspends and resumes on
 * its active-mode clocks and the USB controller / PHY stay coherent for the whole suspend window.
 *
 * The mask is cleared first on purpose. The power driver flushes its accumulated mask to the CMC on
 * every POWER_Enter*(), and an empty mask is flushed as "every IRQ is a wakeup source" -- the 1 ms
 * HAL timer would then pull the SoC straight back out of standby.
 */
void USB_PreLowpowerMode(void)
{
    POWER_ClearAllWakeupSources();

    POWER_EnableWakeupSource(kPOWER_WakeupIrq_VbatGpioCh0);

#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
    if (CONTROLLER_ID == kUSB_ControllerKhci0)
    {
        POWER_EnableWakeupSource(kPOWER_WakeupIrq_CommUsb1);
    }
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        POWER_EnableWakeupSource(kPOWER_WakeupIrq_CommUsb0);
        POWER_EnableWakeupSource(kPOWER_WakeupIrq_CommUsbPhy);
    }
#endif
}

/*!
 * @brief Put the SoC into Deep Sleep while the USB bus is suspended.
 *
 * Applies s_usbDeepSleepConfig, which holds the complete standby resource state, and performs the
 * entry. The function returns once a wakeup source armed by USB_PreLowpowerMode() fires: either an
 * SW5 press for a remote wakeup, or the USB controller / PHY on a host-driven bus resume.
 *
 * @return 0 when the SoC entered and left Deep Sleep, 1 when the entry was rejected.
 */
uint8_t USB_EnterLowpowerMode(void)
{
    if (kStatus_Success != POWER_EnterDeepSleep(&s_usbDeepSleepConfig))
    {
        return 1U;
    }

    return 0U;
}

/*!
 * @brief Release the wakeup sources after the SoC has left Deep Sleep.
 *
 * Called from USB_PowerPostSwitchHook() and mirrors USB_PreLowpowerMode(), so that no wakeup mask is
 * carried into the next bus suspend.
 */
void USB_PostLowpowerMode(void)
{
    POWER_DisableWakeupSource(kPOWER_WakeupIrq_VbatGpioCh0);

#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
    if (CONTROLLER_ID == kUSB_ControllerKhci0)
    {
        POWER_DisableWakeupSource(kPOWER_WakeupIrq_CommUsb1);
    }
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        POWER_DisableWakeupSource(kPOWER_WakeupIrq_CommUsb0);
        POWER_DisableWakeupSource(kPOWER_WakeupIrq_CommUsbPhy);
    }
#endif
}

void USB_ControllerSuspended(void)
{
    /* Nothing to do: the USB controller needs no explicit clock-request handshake on bus suspend. */
}

void USB_WaitClockLocked(void)
{
    /* Nothing to wait for: the clock tree is not reconfigured for Deep Sleep, so the SoC resumes on
     * the same clocks it suspended with and the USB controller / PHY clocks are already valid. */
}

void BOARD_USER_BUTTON_IRQ_HANDLER(void)
{
    if (GPIO_PinGetInterruptFlag(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN) != 0U)
    {
        GPIO_PinClearInterruptFlag(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN);

        g_UsbDeviceHidMouse.selfWakeup = 1U;
    }

    SDK_ISR_EXIT_BARRIER;
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif

#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USBFS_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbHsClock();
    USB_EhciLowPowerPhyInit(CONTROLLER_ID, CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_usb0_phyclk), &phyConfig);

    /* Let the USB PHY PLL power itself down while the bus is suspended and bring itself back up on
     * resume. Without this the PHY PLL keeps running through Deep Sleep, wasting power and leaving
     * the PHY in a state the suspend/resume flow does not expect. Written through the PLL_SIC_SET
     * alias so only this bit is touched, leaving the PLL settings USB_EhciLowPowerPhyInit just
     * programmed intact. */
    COMM__USBPHY->PLL_SIC_SET = USBPHY_PLL_SIC_PLL_MISC2_CONTROL0(1U);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    CLOCK_EnableUsbFsClock(kCLOCK_UsbFsSrcUsb1Root);
    /* Crystal-less: trim the FRO against the USB FS (KHCI) frame timing. */
    CLOCK_EnableFroTuner(kCLOCK_FroTuner192M, kCLOCK_FroTunerRefUsbFsSof1kHz, 0x02U);
#endif
}

void USB_DeviceIsrEnable(void)
{
    IRQn_Type irqNumber;

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    IRQn_Type usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                    = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority(irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ(irqNumber);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    IRQn_Type usbDeviceKhciIrq[] = USBFS_IRQS;
    irqNumber                    = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority(irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ(irqNumber);
#endif
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    USB_DeviceEhciTaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    USB_DeviceKhciTaskFunction(deviceHandle);
#endif
}
#endif

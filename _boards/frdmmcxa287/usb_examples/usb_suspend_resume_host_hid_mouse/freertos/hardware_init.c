/*
 * Copyright 2026 NXP
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
#include "usb_phy.h"
#include "clock_config.h"

#include "clock_config.h"
#include "board.h"
#include "fsl_port.h"
#include "fsl_adapter_gpio.h"
#include "fsl_adapter_timer.h"
/*${header:end}*/

/*${variable:start}*/
#define TIMER_SOURCE_CLOCK (16384)
extern usb_host_mouse_instance_t g_HostHidMouse;
static uint32_t systemTickControl;
static uint8_t s_timer_enable = 0;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
uint32_t g_gpioHandle[(HAL_GPIO_HANDLE_SIZE + 3) / 4];
/*${variable:end}*/

/*${prototype:start}*/
void USB_WaitClockLocked(void);
/*${prototype:end}*/
extern usb_host_handle g_HostHandle;
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBUTTONsPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    
    /* Attach peripheral clock */
    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN | kCLKE_16K_VBAT);
    CLOCK_AttachClk(kCLK_16K_to_OSTIMER);
    
    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kOSTIMER0_RST_SHIFT_RSTn);
}

void BOARD_InitPins(void)
{
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
    s_GpioInputPin.port      = 0;
    s_GpioInputPin.pin       = BOARD_SW3_GPIO_PIN;

    HAL_GpioInit(g_gpioHandle, &s_GpioInputPin);
    HAL_GpioInstallCallback(g_gpioHandle, SW_Callback, NULL);
    HAL_GpioSetTriggerMode(g_gpioHandle, kHAL_GpioInterruptFallingEdge);

    NVIC_SetPriority(BOARD_SW3_IRQ, 1U);
    NVIC_EnableIRQ(BOARD_SW3_IRQ);
}

char *SW_GetName(void)
{
    return BOARD_SW3_NAME;
}

void HW_TimerCallback(void *param)
{
    uint64_t timerTicks;

    g_HostHidMouse.hwTick++;
    USB_HostUpdateHwTick(g_HostHandle, g_HostHidMouse.hwTick);
    if (1 == s_timer_enable)
    {
		timerTicks = HAL_TimerGetCurrentTicks(&g_halTimerHandle[0]);
		HAL_TimerUpdateMatchValueInTicks(&g_halTimerHandle[0], timerTicks + MSEC_TO_COUNT(1, TIMER_SOURCE_CLOCK));
    }
}

void HW_TimerInit(void)
{
    hal_timer_config_t halTimerConfig;
    uint64_t timerTicks;

    halTimerConfig.timeout            = 1000;
    halTimerConfig.srcClock_Hz        = TIMER_SOURCE_CLOCK;
    halTimerConfig.instance           = 0U;
    hal_timer_handle_t halTimerHandle = &g_halTimerHandle[0];
    HAL_TimerInit(halTimerHandle, &halTimerConfig);
    HAL_TimerInstallCallback(halTimerHandle, HW_TimerCallback, NULL);
    timerTicks = HAL_TimerGetCurrentTicks(&g_halTimerHandle[0]);
    HAL_TimerUpdateMatchValueInTicks(&g_halTimerHandle[0], timerTicks + MSEC_TO_COUNT(1, TIMER_SOURCE_CLOCK));
}

void HW_TimerControl(uint8_t enable)
{
    uint64_t timerTicks;

    if (enable)
    {
        s_timer_enable = 1;
        HAL_TimerEnable(g_halTimerHandle);
        timerTicks = HAL_TimerGetCurrentTicks(&g_halTimerHandle[0]);
        HAL_TimerUpdateMatchValueInTicks(&g_halTimerHandle[0], timerTicks + MSEC_TO_COUNT(1, TIMER_SOURCE_CLOCK));
        EnableIRQ(OS_EVENT_IRQn);
    }
    else
    {
        s_timer_enable = 0;
        HAL_TimerDisable(g_halTimerHandle);
        DisableIRQ(OS_EVENT_IRQn);
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
}
uint8_t USB_EnterLowpowerMode(void)
{
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
    return 0;
}
void USB_PostLowpowerMode(void)
{
    __enable_irq();
    SysTick->CTRL = systemTickControl;
    USB_WaitClockLocked();
}
void USB_ControllerSuspended(void)
{
}
void USB_WaitClockLocked(void)
{
}

#if defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U)
void USB1_HS_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
}
#endif /* USB_HOST_CONFIG_EHCI */

void USB_HostClockInit(void)
{
#if defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif

#if defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U)
    SPC0->ACTIVE_VDELAY = 0x0500;
    /* Change the power DCDC to 1.8v (By deafult, DCDC is 1.8V), CORELDO to 1.1v (By deafult, CORELDO is 1.0V) */
    SPC0->ACTIVE_CFG &= ~SPC_ACTIVE_CFG_CORELDO_VDD_DS_MASK;
    SPC0->ACTIVE_CFG |= SPC_ACTIVE_CFG_CORELDO_VDD_LVL(0x3);
    /* Wait until it is done */
    while (SPC0->SC & SPC_SC_BUSY_MASK)
        ;
    if (0u == (SCG0->LDOCSR & SCG_LDOCSR_LDOEN_MASK))
    {
        SCG0->TRIM_LOCK = 0x5a5a0001U;
        SCG0->LDOCSR |= SCG_LDOCSR_LDOEN_MASK;
        /* wait LDO ready */
        while (0U == (SCG0->LDOCSR & SCG_LDOCSR_VOUT_OK_MASK))
            ;
    }
    
    MRCC0->MRCC_GLB_CC2_SET = MRCC_MRCC_GLB_CC2_USB1_MASK | MRCC_MRCC_GLB_CC2_USB1_PHY_MASK;
    SCG0->SOSCCFG &= ~(SCG_SOSCCFG_RANGE_MASK | SCG_SOSCCFG_EREFS_MASK);
    /* xtal = 20 ~ 30MHz */
    SCG0->SOSCCFG = (1U << SCG_SOSCCFG_RANGE_SHIFT) | (1U << SCG_SOSCCFG_EREFS_SHIFT);
    SCG0->SOSCCSR |= SCG_SOSCCSR_SOSCEN_MASK;
    while (1)
    {
        if (SCG0->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)
        {
            break;
        }
    }

    CLOCK_AttachClk(kPHY_CLK_XTAL_to_USBHS);
    CLOCK_AttachClk(kCLK_IN_to_USBHS_PHY);
    CLOCK_EnableClock(kCLOCK_GateUSBHS);
    CLOCK_EnableClock(kCLOCK_GateUSBHS_PHY);
    CLOCK_SetClockDiv(kCLOCK_DivUSBHS_PHY, 1);
    CLOCK_EnableUsbhsPhyPllClock(BOARD_XTAL0_CLK_HZ);
    CLOCK_EnableUsbhsClock();

    CLOCK_SetupOsc32KClocking(0x0F);
    USB_EhciLowPowerPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;
#if defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U)
    uint8_t usbHOSTEhciIrq[] = USBHS_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
#endif /* USB_HOST_CONFIG_EHCI */

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
#if defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U)
    USB_HostEhciTaskFunction(param);
#endif
}
/*${function:end}*/

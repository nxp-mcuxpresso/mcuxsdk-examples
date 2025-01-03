/*
 * Copyright 2018 NXP
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
#include "fsl_power.h"
#include "lpm.h"
#include "app.h"
#include "pin_mux.h"

#include "clock_config.h"
#include "board.h"
#include "fsl_inputmux.h"
#include "fsl_pint.h"
#include "fsl_power.h"

#include "fsl_adapter_timer.h"
/*${header:end}*/

/*${macro:start}*/
/* Leave AON modules on in PM2 */
#define APP_PM2_MEM_PU_CFG ((uint32_t)kPOWER_Pm2MemPuAon1 | (uint32_t)kPOWER_Pm2MemPuAon0)
/* All ANA in low power mode in PM2 */
#define APP_PM2_ANA_PU_CFG ((uint32_t)kPOWER_Pm2AnaPuUsb | (uint32_t)kPOWER_Pm2AnaPuAnaTop)
/* Buck18 and Buck11 both in sleep level in PM3 */
#define APP_PM3_BUCK_CFG (0U)
/* clk_32k not derived from cau, capture_timer also not used in the app. It's safe to disable CAU clock in PM3,4. */
#define APP_SLEEP_CAU_PD (true)
/* All clock gated */
#define APP_SOURCE_CLK_GATE ((uint32_t)kPOWER_ClkGateAll)
/* All SRAM kept in retention in PM3, AON SRAM shutdown in PM4 */
#define APP_MEM_PD_CFG (1UL << 8)
/*${macro:end}*/

/*${variable:start}*/
#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)
extern usb_host_mouse_instance_t g_HostHidMouse;
extern usb_host_handle g_HostHandle;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
power_sleep_config_t s_slpCfg;
/*${variable:end}*/
/*${prototype:start}*/
void USB_WaitClockLocked(void);
/*${prototype:end}*/
extern usb_host_handle g_HostHandle;
/*${function:start}*/
void SW_IntControl(uint8_t enable)
{
    if (enable)
    {
        g_HostHidMouse.selfWakeup = 0U;
        PINT_EnableCallback(PINT);
    }
    else
    {
        PINT_DisableCallback(PINT);
    }
}

void APP_WakeupHandler(IRQn_Type irq)
{
    g_HostHidMouse.selfWakeup = 1U;
    SW_IntControl(0);
}

void PIN1_INT_IRQHandler()
{
    NVIC_ClearPendingIRQ(PIN1_INT_IRQn);
    APP_WakeupHandler(PIN1_INT_IRQn);
}

void BOARD_InitHardware(void)
{
    uint32_t resetSrc;
    lpm_config_t config = {
        /* PM2/tickless idle less than 10 ms will be skipped. */
        .threshold = 10U,
    };
    power_init_config_t initCfg = {
        .iBuck         = true,  /* VCORE AVDD18 supplied from iBuck on RD board. */
        .gateCauRefClk = false, /* CAU_SOC_SLP_REF_CLK needed for LPOSC. */
    };

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_EnableXtal32K(true);
    /* TODO: Use XTAL32K on real board */
    CLOCK_AttachClk(kRC32K_to_CLK32K);
    CLOCK_AttachClk(kLPOSC_to_OSTIMER_CLK);

    resetSrc = POWER_GetResetCause();

    POWER_ClearResetCause(resetSrc);
    LPM_Init(&config);
    POWER_InitPowerConfig(&initCfg);
    POWER_ConfigCauInSleep(APP_SLEEP_CAU_PD);
    LPM_EnableWakeupSource(PIN1_INT_IRQn);
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

char *SW_GetName(void)
{
    return "SW4";
}

void HW_TimerCallback(void *param)
{
    g_HostHidMouse.hwTick++;
    USB_HostUpdateHwTick(g_HostHandle, g_HostHidMouse.hwTick);
}

void HW_TimerInit(void)
{
    hal_timer_config_t halTimerConfig;
    halTimerConfig.timeout            = 1000U;
    halTimerConfig.srcClock_Hz        = CLOCK_GetCoreSysClkFreq();
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
    HW_TimerInit();
}

void USB_PreLowpowerMode(void)
{
}

/*
 * Enter the LowPower mode.
 */
void APP_LowPower_EnterLowPower(void)
{
    s_slpCfg.pm2MemPuCfg = APP_PM2_MEM_PU_CFG;
    s_slpCfg.pm2AnaPuCfg = APP_PM2_ANA_PU_CFG;
    s_slpCfg.clkGate     = APP_SOURCE_CLK_GATE;
    s_slpCfg.memPdCfg    = APP_MEM_PD_CFG;
    s_slpCfg.pm3BuckCfg  = APP_PM3_BUCK_CFG;

    /* PM2 */
    LPM_SetPowerMode(2U, &s_slpCfg);
    CLOCK_AttachClk(kLPOSC_to_MAIN_CLK);
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Set wait timeout to max to ensure only assigned source can wake up system. */
    LPM_WaitForInterrupt(0xFFFFFFFFU);
    CLOCK_AttachClk(kMAIN_PLL_to_MAIN_CLK);
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}
uint8_t USB_EnterLowpowerMode(void)
{
    APP_LowPower_EnterLowPower();
    return 0;
}
void USB_WaitClockLocked(void)
{
}
void USB_PostLowpowerMode(void)
{
    USB_WaitClockLocked();
}
void USB_ControllerSuspended(void)
{
    while (SYSCTL0->USBCLKSTAT & (SYSCTL0_USBCLKSTAT_HOST_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
}

void USBHS_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
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

void USB_HostTaskFn(void *param)
{
    USB_HostEhciTaskFunction(param);
}
/*${function:end}*/

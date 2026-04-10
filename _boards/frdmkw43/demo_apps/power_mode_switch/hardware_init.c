/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_cmc.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_spc.h"
#include "peripherals.h"
#include "board.h"
#include "app.h"

/*
 * KW43 is a dual-core SoC where Core0 and Core1 share 1.5 MB of flash.
 * When booting from flash the RFMC CPU_RST bit defaults to 0, which
 * releases Core1.  An un-programmed or unexpected Core1 image causes
 * extra current draw.  Assert CPU_RST to hold Core1 in reset until its
 * image is intentionally loaded and started.
 */
static inline void BOARD_HoldCore1InReset(void)
{
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_CPU_RST_MASK;
}

void APP_InitDebugConsole(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void APP_DeinitDebugConsole(void)
{
    DbgConsole_Deinit();
    PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(DEBUG_CONSOLE_TX_PORT, DEBUG_CONSOLE_TX_PIN, kPORT_PinDisabledOrAnalog);
    CLOCK_DeinitSysOsc();
}

void APP_DeinitWakeupButton(void)
{
    DisableIRQ(BOARD_SW4_IRQ);
    NVIC_ClearPendingIRQ(BOARD_SW4_IRQ);
    GPIO_SetPinInterruptConfig(BOARD_SW4_GPIO, BOARD_SW4_GPIO_PIN, kGPIO_InterruptStatusFlagDisabled);
    GPIO_PinClearInterruptFlag(BOARD_SW4_GPIO, BOARD_SW4_GPIO_PIN);

    NVIC_ClearPendingIRQ(DEMO_WUU_IRQN);
    WUU_ClearExternalWakeupPinsConfig(APP_WUU, WUU_WAKEUP_BUTTON_IDX);
    WUU_ClearExternalWakeUpPinsFlag(APP_WUU, WUU_WAKEUP_BUTTON_FLAG);
}

void APP_LPTMR_IRQ_HANDLER(void)
{
    if ((kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(APP_LPTMR)) != 0U)
    {
        LPTMR_DisableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(APP_LPTMR, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(APP_LPTMR);
    }
}

void WUU0_IRQHandler(void)
{
    uint32_t wakeupFlags = WUU_GetExternalWakeUpPinsFlag(APP_WUU);

    if (wakeupFlags != 0UL)
    {
        WUU_ClearExternalWakeUpPinsFlag(APP_WUU, wakeupFlags);
    }
}

void BOARD_SW4_IRQ_HANDLER(void)
{
    if (GPIO_PinGetInterruptFlag(BOARD_SW4_GPIO, BOARD_SW4_GPIO_PIN) != 0U)
    {
        GPIO_PinClearInterruptFlag(BOARD_SW4_GPIO, BOARD_SW4_GPIO_PIN);
    }
}

void BOARD_InitHardware(void)
{
    uint32_t resetStatus;

    BOARD_HoldCore1InReset();
    BOARD_InitBootPins();
    BOARD_InitButtonsPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    resetStatus = CMC_GetSystemResetStatus(APP_CMC);

    if ((resetStatus & kCMC_WakeUpReset) != 0UL)
    {
        SPC_ClearPeriphIOIsolationFlag(APP_SPC);

        if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain0))
        {
            SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain0);
        }
        if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain1))
        {
            SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain1);
        }
        if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain2))
        {
            RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK));
            RFMC->RF2P4GHZ_CTRL &= ~RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;
            SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain2);
        }
        if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain3))
        {
            SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain3);
        }
        SPC_ClearLowPowerRequest(APP_SPC);
    }
}

/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "peripherals.h"
#include "fsl_clock.h"

static void DEMO_WUU_init(void)
{
    EnableIRQ(DEMO_WUU_IRQN);
}

const lptmr_config_t DEMO_LPTMR_config = {
    .timerMode = kLPTMR_TimerModeTimeCounter,
    .pinSelect = kLPTMR_PinSelectInput_0,
    .pinPolarity = kLPTMR_PinPolarityActiveHigh,
    .enableFreeRunning = false,
    .bypassPrescaler = true,
    .prescalerClockSource = kLPTMR_PrescalerClock_0,
    .value = kLPTMR_Prescale_Glitch_0,
};

static void DEMO_LPTMR_init(void)
{
    CLOCK_SetIpSrc(kCLOCK_Lptmr0, kCLOCK_IpSrc32kClk);
    LPTMR_Init(DEMO_LPTMR_PERIPHERAL, &DEMO_LPTMR_config);
    /*
        * LPTMR_Init() calls CLOCK_EnableClock() which sets MRCC CC=1.
        * Restore CC=3 so the LPTMR functional clock stays alive in
        * low-power modes for timer wake-up.
        */
    CLOCK_EnableClockLPMode(kCLOCK_Lptmr0, kCLOCK_IpClkControl_fun3);
    LPTMR_EnableInterrupts(DEMO_LPTMR_PERIPHERAL, kLPTMR_TimerInterruptEnable);
    EnableIRQ(DEMO_LPTMR_IRQN);
}

void BOARD_InitPeripherals(void)
{
    DEMO_WUU_init();
    DEMO_LPTMR_init();
}

void BOARD_InitBootPeripherals(void)
{
    BOARD_InitPeripherals();
}

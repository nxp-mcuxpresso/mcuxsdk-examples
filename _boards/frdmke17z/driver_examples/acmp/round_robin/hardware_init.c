/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_lptmr.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void BOARD_InitAcmpRoundRobinTrigger(uint32_t triggerPeroid_Ms)
{
    lptmr_config_t lptmrConfig;

    /* Configure LPTMR */
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* Enable prescaler */
    lptmrConfig.bypassPrescaler = false;
    /* Prescaler divides the prescaler clock(LPO 128kHz) by 128 */
    lptmrConfig.value = kLPTMR_Prescale_Glitch_6;

    /* Initialize the LPTMR */
    LPTMR_Init(LPTMR0, &lptmrConfig);

    /* Set timer period */
    LPTMR_SetTimerPeriod(LPTMR0, triggerPeroid_Ms);

    /* Start counting */
    LPTMR_StartTimer(LPTMR0);
}

void BOARD_ClearAcmpRoundRobinTrigger(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
}

void BOARD_EnterStopMode(void)
{
    volatile uint32_t i;

    /* Allows to enter stop mode. */
    SMC->PMPROT |= SMC_PMPROT_AVLP_MASK;
    /* Select to enter STOP mode. */
    SMC->PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
    SMC->PMCTRL |= SMC_PMCTRL_STOPM(0U);

    /* Clear the SLEEPDEEP bit to disable deep sleep mode - enter wait mode*/
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    /*
     * Delay some time to ensure that all register writes associated with setting up the low power mode being entered
     * have completed before the MCU enters the low power mode.
     */
    for (i = 0U; i <= 100000U; i++)
    {
        __ASM("NOP");
    }
    __WFI();
}
/*${function:end}*/

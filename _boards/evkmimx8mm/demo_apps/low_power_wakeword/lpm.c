/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS header */
#include "board.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"
#include "lpm.h"
#include "fsl_debug_console.h"
#include "fsl_gpt.h"
#include "fsl_gpc.h"
#include "app_srtm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Systick counter source clock frequency. Need config the Systick counter's(GPT) clock
 * source to this frequency in application.
 */
#define SYSTICK_COUNTER_SOURCE_CLK_FREQ (SYSTICK_CLOCK)
/* Define the counter clock of the systick (GPT).*/
#define SYSTICK_COUNTER_FREQ (8000000U)
/* Define the count per tick of the systick in run mode. For accuracy purpose,
 * please make SYSTICK_SOURCE_CLOCK times of configTICK_RATE_HZ.
 */
#define SYSTICK_COUNT_PER_TICK (SYSTICK_COUNTER_FREQ / configTICK_RATE_HZ)

#ifndef BOARD_MU_IRQ_NUM
#define BOARD_MU_IRQ_NUM BOARD_MU_IRQ
#endif

/* If block envent exists, then do not allow M4 to enter low power mode.*/
static uint32_t s_BlockEventCnt = 0;
/* FreeRTOS implemented Systick handler. */
extern void xPortSysTickHandler(void);

static LPM_POWER_STATUS_MCORE mcore_lpm_state = LPM_MCORE_STATE_RUN;

extern volatile app_srtm_state_t srtmState;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Override the default definition of vPortSetupTimerInterrupt() that is weakly
 * defined in the FreeRTOS Cortex-M4F port layer with a version that configures
 * GPT to generate the tick interrupt. */
void vPortSetupTimerInterrupt(void)
{
    gpt_config_t config;
    GPT_GetDefaultConfig(&config);
    config.enableRunInDoze = true;
    config.clockSource     = kGPT_ClockSource_Osc;

    /* Initialize timer */
    GPT_Init(SYSTICK_BASE, &config);
    /* Divide GPT clock source frequency to SYSTICK_COUNTER_FREQ */
    GPT_SetOscClockDivider(SYSTICK_BASE, SYSTICK_COUNTER_SOURCE_CLK_FREQ / SYSTICK_COUNTER_FREQ);

    /* Set timer period */
    GPT_SetOutputCompareValue(SYSTICK_BASE, kGPT_OutputCompare_Channel1, SYSTICK_COUNT_PER_TICK - 1U);
    /* Enable timer interrupt */
    GPT_EnableInterrupts(SYSTICK_BASE, kGPT_OutputCompare1InterruptEnable);

    NVIC_SetPriority(SYSTICK_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY);
    /* Enable NVIC interrupt */
    NVIC_EnableIRQ(SYSTICK_IRQn);
    /* Start counting */
    GPT_StartTimer(SYSTICK_BASE);
}

uint32_t LPM_EnterTicklessIdle(uint32_t timeoutMilliSec, uint64_t *pCounter)
{
    uint32_t counter, expired = 0;
    uint32_t ms, maxMS;
    uint32_t flag;
    uint32_t timeoutTicks;
    uint32_t countPerTick = SYSTICK_COUNT_PER_TICK;

    /* If timeout < 2 ticks, don't do tickless idle. */
    if ((uint64_t)timeoutMilliSec * configTICK_RATE_HZ < 2 * 1000)
    {
        return 0;
    }

    maxMS = 0xFFFFFFFFU / SYSTICK_COUNTER_FREQ * 1000;
    ms    = timeoutMilliSec > maxMS ? maxMS : timeoutMilliSec;

    /* Calculate the timer counter needed for timeout */
    timeoutTicks = (uint64_t)ms * configTICK_RATE_HZ / 1000;
    counter      = timeoutTicks * countPerTick;

    GPT_StopTimer(SYSTICK_BASE); /* Timer stopped. */
    flag = GPT_GetStatusFlags(SYSTICK_BASE, kGPT_OutputCompare1Flag);
    if (flag)
    {
        GPT_ClearStatusFlags(SYSTICK_BASE, kGPT_OutputCompare1Flag);
        NVIC_ClearPendingIRQ(SYSTICK_IRQn);
        expired = countPerTick;
    }
    expired += GPT_GetCurrentTimerCount(SYSTICK_BASE);
    /* Minus those already expired to get accurate waiting counter. */
    counter -= expired;

    /* Enable GPT free-run mode, the counter is not reset when compare events occur.
     * In this way, the counter is not overflow during tickless and the exact time
     * since tickless enter can be calculated.
     */
    SYSTICK_BASE->CR |= GPT_CR_FRR_MASK;
    /* Convert count in systick freq to tickless clock count */
    GPT_SetOutputCompareValue(SYSTICK_BASE, kGPT_OutputCompare_Channel1, counter - 1UL);
    /* Restart timer, GPT CR_ENMOD=1, counter value is reset when restart timer. */
    GPT_StartTimer(SYSTICK_BASE);

    /* return waiting counter */
    *pCounter = counter;

    return timeoutTicks;
}

void LPM_ExitTicklessIdle(uint32_t timeoutTicks, uint64_t timeoutCounter)
{
    uint32_t flag, counter, expired, expiredTicks;
    uint32_t completeTicks;
    uint32_t countPerTick = SYSTICK_COUNT_PER_TICK;

    GPT_StopTimer(SYSTICK_BASE);
    flag = GPT_GetStatusFlags(SYSTICK_BASE, kGPT_OutputCompare1Flag);
    /* Convert tickless count to systick count. */
    expired = GPT_GetCurrentTimerCount(SYSTICK_BASE);

    if (flag)
    {
        /* If counter already exceeds 1 tick, it means wakeup takes too much time
         * and we have already lost some ticks. Calcaute and add the
         * lost ticks into completeTicks. Completed ticks minus 1 because pending interrupt will be handled immediately
         * when interrupt unmasked.
         */
        expiredTicks  = (expired - timeoutCounter) / countPerTick;
        completeTicks = expiredTicks + timeoutTicks - 1;

        /* Continue the uncompleted tick. */
        counter = countPerTick - (expired - timeoutCounter) % countPerTick;
    }
    else
    {
        /* Remaining counter. */
        counter       = timeoutCounter - expired;
        completeTicks = timeoutTicks - (counter - 1) / countPerTick - 1;
        counter       = (counter - 1) % countPerTick + 1;
    }

    /* Now reinit Systick. */
    SYSTICK_BASE->CR &= ~GPT_CR_FRR_MASK;
    GPT_SetOutputCompareValue(SYSTICK_BASE, kGPT_OutputCompare_Channel1, counter - 1UL);
    /* Restart timer */
    GPT_StartTimer(SYSTICK_BASE);

    vTaskStepTick(completeTicks);
}

/* The systick interrupt handler. */
void SYSTICK_HANDLER(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(SYSTICK_BASE, kGPT_OutputCompare1Flag);

    /* This is the first tick since the MCU left a low power mode the
     * compare value need to be reset to its default.
     */
    if (GPT_GetOutputCompareValue(SYSTICK_BASE, kGPT_OutputCompare_Channel1) != SYSTICK_COUNT_PER_TICK - 1)
    {
        /* Counter will be reset and cause minor accuracy loss */
        GPT_SetOutputCompareValue(SYSTICK_BASE, kGPT_OutputCompare_Channel1, SYSTICK_COUNT_PER_TICK - 1);
    }

    /* Call FreeRTOS tick handler. */
    xPortSysTickHandler();

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}

void LPM_IncreseBlockSleepCnt(void)
{
    s_BlockEventCnt++;
}

void LPM_DecreaseBlockSleepCnt(void)
{
    s_BlockEventCnt--;
}

bool LPM_AllowSleep(void)
{
    if (s_BlockEventCnt)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void LPM_MCORE_ChangeMCoreClock(LPM_MCORE_CLOCK_SPEED target)
{
    /* Change CCM Root to change M4 clock*/
    switch (target)
    {
        case LPM_MCORE_LOW_FREQ:
            if (CLOCK_GetRootMux(kCLOCK_RootM4) != kCLOCK_M4RootmuxOsc24M)
            {
                CLOCK_SetRootMux(kCLOCK_RootM4, kCLOCK_M4RootmuxOsc24M);
                CLOCK_SetRootDivider(kCLOCK_RootM4, 1U, 1U);
            }
            break;
        case LPM_MCORE_HIGH_FREQ:
            if (CLOCK_GetRootMux(kCLOCK_RootM4) != kCLOCK_M4RootmuxSysPll1)
            {
                CLOCK_SetRootDivider(kCLOCK_RootM4, 1U, 2U);
                CLOCK_SetRootMux(kCLOCK_RootM4, kCLOCK_M4RootmuxSysPll1); /* switch cortex-m4 to SYSTEM PLL1 */
            }
            break;
        default:
            break;
    }
}

void LPM_MCORE_SetPowerStatus(GPC_Type *base, LPM_POWER_STATUS_MCORE targetPowerMode)
{
    gpc_lpm_config_t config;
    config.enCpuClk              = false;
    config.enFastWakeUp          = false;
    config.enDsmMask             = false;
    config.enWfiMask             = false;
    config.enVirtualPGCPowerdown = true;
    config.enVirtualPGCPowerup   = true;
    switch (targetPowerMode)
    {
        case LPM_MCORE_STATE_RUN:
            GPC->LPCR_M4 = GPC->LPCR_M4 & (~GPC_LPCR_M4_LPM0_MASK);
            break;
        case LPM_MCORE_STATE_WAIT:
            GPC_EnterWaitMode(GPC, &config);
            break;
        case LPM_MCORE_STATE_STOP:
            GPC_EnterStopMode(GPC, &config);
            break;
        default:
            break;
    }
}
void PreSleepProcessing(void)
{
    APP_SRTM_Suspend();
    DbgConsole_Deinit();
}

void PostSleepProcessing(void)
{
    APP_SRTM_Resume();
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ);
}

void ShowMCoreStatus(void)
{
    if (mcore_lpm_state == LPM_MCORE_STATE_STOP)
    {
        PRINTF("\r\nNo audio, M core enters STOP mode!\r\n");
    }
    else if (mcore_lpm_state == LPM_MCORE_STATE_RUN)
    {
        PRINTF("\r\nPlayback or record is running, M core enters RUN mode!\r\n");
    }
    else
    {
        ; /* For MISRA C-2012 rule 15.7. */
    }
}

void UpdateTargetPowerStatus(void)
{
    /*
     * The m4_lpm_state merely indicates what the power state the M core finally should be.
     * In this demo, if there is no audio playback, M core will be set to STOP mode finally.
     */
    LPM_POWER_STATUS_MCORE mcore_target_lpm;

    if (APP_SRTM_ServiceIdle())
    {
        mcore_target_lpm = LPM_MCORE_STATE_STOP;
    }
    else
    {
        mcore_target_lpm = LPM_MCORE_STATE_RUN;
    }

    if (mcore_target_lpm != mcore_lpm_state)
    {
        mcore_lpm_state = mcore_target_lpm;
        ShowMCoreStatus();
    }
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t irqMask;
    uint64_t counter = 0;
    uint32_t timeoutTicks;
    uint32_t timeoutMilliSec = (uint32_t)((uint64_t)1000 * xExpectedIdleTime / configTICK_RATE_HZ);

    irqMask = DisableGlobalIRQ();

    UpdateTargetPowerStatus();

    /* Only when no context switch is pending and no task is waiting for the scheduler
     * to be unsuspended then enter low power entry.
     */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        timeoutTicks = LPM_EnterTicklessIdle(timeoutMilliSec, &counter);
        if (timeoutTicks)
        {
            if (APP_SRTM_ServiceIdle() && LPM_AllowSleep())
            {
                LPM_MCORE_ChangeMCoreClock(LPM_MCORE_LOW_FREQ);
                LPM_MCORE_SetPowerStatus(BOARD_GPC_BASEADDR, LPM_MCORE_STATE_STOP);
                PreSleepProcessing();
                ServiceFlagAddr = ServiceIdle;
                __DSB();
                __ISB();
                __WFI();
                ServiceFlagAddr = ServiceBusy;
                PostSleepProcessing();
                LPM_MCORE_ChangeMCoreClock(LPM_MCORE_HIGH_FREQ);
                LPM_MCORE_SetPowerStatus(BOARD_GPC_BASEADDR, LPM_MCORE_STATE_RUN);
            }
            else
            {
                __DSB();
                __ISB();
                __WFI();
            }
        }
        LPM_ExitTicklessIdle(timeoutTicks, counter);
    }

    EnableGlobalIRQ(irqMask);
}

void LPM_Init(void)
{
    /* Treat M core as busy status by default.*/
    ServiceFlagAddr = ServiceBusy;

    /*
     * Wait For A core Side Become Ready
     */
    PRINTF("********************************\r\n");
    PRINTF("Wait the Linux kernel boot up to create the link between M core and A core.\r\n");
    PRINTF("\r\n");
    PRINTF("********************************\r\n");
    while (srtmState != APP_SRTM_StateLinkedUp)
        ;
    PRINTF("The rpmsg channel between M core and A core created!\r\n");
    PRINTF("********************************\r\n");
    PRINTF("\r\n");

    /* Configure GPC */
    GPC_Init(BOARD_GPC_BASEADDR, APP_PowerUpSlot, APP_PowerDnSlot);
    GPC_EnableIRQ(BOARD_GPC_BASEADDR, BOARD_MU_IRQ_NUM);
    GPC_EnableIRQ(BOARD_GPC_BASEADDR, SYSTICK_IRQn);
    GPC_EnableIRQ(BOARD_GPC_BASEADDR, I2C3_IRQn);
}

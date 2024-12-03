/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "fsl_rgpio.h"
#include "fsl_lptmr.h"
#include "fsl_lpuart.h"
#include "fsl_mu.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "lpm.h"
#include "power_mode_switch.h"
#include "fsl_rgpio.h"
#include "scmi.h"
#include "pin_mux.h"
#include "app_srtm.h"

/*******************************************************************************
 * Struct Definitions
 ******************************************************************************/
typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceLptmr,  /*!< Wakeup by LPTMR.        */
    kAPP_WakeupSourceLpuart, /*!< Wakeup by LPUART.       */
    kAPP_WakeupSourceSM     /*!< Wakeup by SM message.    */
} app_wakeup_source_t;

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
extern void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode);
extern void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode, bool result);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t s_wakeupTimeout;           /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */
static SemaphoreHandle_t s_wakeupSig;
static const char *s_modeNames[] = {"RUN", "WAIT", "STOP", "SUSPEND"};
extern lpm_power_mode_t s_curMode;
static TimerHandle_t suspendTimer;
TaskHandle_t pxPowerModeSwitch;
TaskHandle_t pxWorkingTask;
extern bool scmiRequestM7IntoSuspend;
/*******************************************************************************
 * Function Code
 ******************************************************************************/

/* Get input from user about wakeup timeout. */
static uint32_t APP_GetWakeupTimeout(void)
{
    uint32_t timeout = 0U;
    uint8_t c;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 999s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        do
        {
            c = GETCHAR();
            if ((c >= '0') && (c <= '9'))
            {
                PRINTF("%c", c);
                timeout = timeout * 10U + c - '0';
            }
            else if ((c == '\r') || (c == '\n'))
            {
                break;
            }
            else
            {
                PRINTF("%c\r\nWrong value!\r\n", c);
                timeout = 0U;
            }
        } while (timeout != 0U && timeout < 100U);

        if (timeout > 0U)
        {
            PRINTF("\r\n");
            break;
        }
    }

    return timeout;
}

/* Get wakeup source by user input. */
static app_wakeup_source_t APP_GetWakeupSource(void)
{
    uint8_t ch;

    while (1)
    {
        PRINTF("Select the wake up source:\r\n");
        PRINTF("Press T for LPTMR - Low Power Timer\r\n");
        PRINTF("Press U for LPUART - Low Power Uart\r\n");
        PRINTF("Press S for SM source: Sm console 'wake' command or ONOFF button\r\n");

        PRINTF("\r\nWaiting for key press..\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceLptmr;
        }
        else if (ch == 'U')
        {
            return kAPP_WakeupSourceLpuart;
        }
        else if (ch == 'S')
        {
            return kAPP_WakeupSourceSM;
        }
        else
        {
            PRINTF("Wrong value!\r\n");
        }
    }
}

/* Get wakeup timeout and wakeup source. */
static void APP_GetWakeupConfig(void)
{
    /* Get wakeup source by user input. */
    s_wakeupSource = APP_GetWakeupSource();

    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        /* Wakeup source is LPTMR, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    if (kAPP_WakeupSourceLpuart == s_wakeupSource)
    {
        PRINTF("Press any char to wake up.\r\n");
    }
    else if (s_wakeupSource == kAPP_WakeupSourceSM)
    {
        PRINTF("Input wake command in SM console or Press ONOFF to wake up.\r\n");
    }
}

static void APP_SetWakeupConfig(lpm_power_mode_t targetMode)
{
    int32_t status = SCMI_ERR_SUCCESS;

     /* Default to no wakeup IRQs */
     uint32_t wakeMask[12] =
     {
        [0 ... 11]  = 0xFFFFFFFFU
     };
     /* IRQs enabled at NVIC level become GPC wake sources */
     for (uint32_t idx = 0; idx < 8U; idx++)
     {
         wakeMask[idx] = ~(NVIC->ISER[idx]);
     }

    if (kAPP_WakeupSourceLptmr == s_wakeupSource || kAPP_WakeupSourceLpuart == s_wakeupSource)
    {
        /* deal with MU5 MU7 interrupt, configure IRQ wake sources. */
        wakeMask[6] = 0xffffffff;

        /*
         * LPUART3 and LPTMR2 in wakeupmix, need assure its power on state in system suspend state.
         * Set WAKEUPMIX LPM response to M7 based on wakeup configuration.
         */
        scmi_pd_lpm_config_t lpmConfig;
        lpmConfig.domainId = PWR_MIX_SLICE_IDX_WAKEUP;
        lpmConfig.lpmSetting = SCMI_CPU_LPM_SETTING_ON_ALWAYS;
        lpmConfig.retMask = 0U;
        SCMI_CpuPdLpmConfigSet(SCMI_A2P, CPU_IDX_M7P, 1U, &lpmConfig);

        if (kAPP_WakeupSourceLptmr == s_wakeupSource)
        {
            LPTMR_SetTimerPeriod(LPTMR2, (32000UL * s_wakeupTimeout / 1U));
            LPTMR_StartTimer(LPTMR2);
            LPTMR_EnableInterrupts(LPTMR2, kLPTMR_TimerInterruptEnable);
            /* Ensure not to be woken up by sm messages */
            DisableIRQ(MU5_A_IRQn);
            /* Ensure not to be woken up by A55 messages */
            DisableIRQ(MU7_B_IRQn);
        }
        else
        {
            PRINTF("LPUART3 is used for wakeup source.\r\n");
            LPUART_EnableInterrupts(LPUART3, kLPUART_RxDataRegFullInterruptEnable);
            /* In low power mode, LPUART can generate wakeup via STAT[RXEDGIF]
             * Config STAT[RXINV] and BAUD[RXEDGIE] to enable STAT[RXEDGIF].
             */
            LPUART3->STAT &= ~LPUART_STAT_RXINV_MASK;
            LPUART3->BAUD |= LPUART_BAUD_RXEDGIE_MASK;

            DisableIRQ(MU5_A_IRQn);
            /* Ensure not to be woken up by A55 messages */
            DisableIRQ(MU7_B_IRQn);

            /*
             * Set lpuart3 can be async wakeup when its clock is gated, LPCGs of lpuart3 need to be changed to CPU LPM controlled.
             * M7 sned per lpm list to system manager, sm check and record request and will deal with when M7 domain enter suspend mode.
             * In to assure lpuart3 can be used as system suspend wakeup source.
             */
            scmi_per_lpm_config_t perLpmConfig;
            perLpmConfig.perId = CPU_PER_LPI_IDX_LPUART3;
            perLpmConfig.lpmSetting = SCMI_CPU_LPM_SETTING_ON_RUN_WAIT_STOP;
            status = SCMI_CpuPerLpmConfigSet(SCMI_A2P, CPU_IDX_M7P, 1U, &perLpmConfig);
            if (status != SCMI_ERR_SUCCESS)
            {
                PRINTF("SCMI_CpuPerLpmConfigSet set fail\r\n");
            }
        }
    }

    if (s_wakeupSource == kAPP_WakeupSourceSM)
    {
        /* Ensure not to be woken up by A55 messages */
        DisableIRQ(MU7_B_IRQn);
        /* M7 can be only wakeup by MU5_A(SM): MU5_A_IRQn=205, wakeMask data is 32bit width, 205 = 32 * 6 + 13, it means wakeMask[6] bit 13. */
        wakeMask[6] = 0xFFFFDFFF;
    }

     status = SCMI_CpuIrqWakeSet(SCMI_A2P, CPU_IDX_M7P, 0U, 12U, wakeMask);
     if (status != SCMI_ERR_SUCCESS)
     {
         PRINTF("SCMI_CpuIrqWakeSet set fail\r\n");
     }
}

/* LPTMR2 interrupt handler. */
void LPTMR2_IRQHandler(void)
{
    bool wakeup = false;
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR2))
    {
        LPTMR_ClearStatusFlags(LPTMR2, kLPTMR_TimerCompareFlag);
        LPTMR_DisableInterrupts(LPTMR2, kLPTMR_TimerInterruptEnable);
        LPTMR_StopTimer(LPTMR2);
        wakeup = true;
    }

    if (wakeup)
    {
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }
}

/* LPUART3 interrupt handler. */
void LPUART3_IRQHandler(void)
{
    bool wakeup = false;
    if (kLPUART_RxDataRegFullInterruptEnable & LPUART_GetEnabledInterrupts(LPUART3))
    {
        LPUART_DisableInterrupts(LPUART3, kLPUART_RxDataRegFullInterruptEnable);
        LPUART3->BAUD &= ~LPUART_BAUD_RXEDGIE_MASK;
        wakeup = true;
    }

    if (wakeup)
    {
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }
}

/* Power Mode Switch task */
void PowerModeSwitchTask(void *pvParameters)
{
    lptmr_config_t lptmrConfig;

    lpm_power_mode_t targetPowerMode;
    uint32_t freq = 0U;
    uint8_t ch;
    int32_t status = SCMI_ERR_SUCCESS;

    /* Setup LPTMR. */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_2;  /* Use RTC 32KHz as clock source. */
    lptmrConfig.bypassPrescaler      = true;
    lptmrConfig.value                = kLPTMR_Prescale_Glitch_0; /* Divide clock source by 2. */
    LPTMR_Init(LPTMR2, &lptmrConfig);
    NVIC_SetPriority(LPTMR2_IRQn, APP_LPTMR2_IRQ_PRIO);

    EnableIRQ(LPTMR2_IRQn);

    /* Setup LPUART. */
    NVIC_SetPriority(LPUART3_IRQn, APP_LPUART3_IRQ_PRIO);

    EnableIRQ(LPUART3_IRQn);

    for (;;)
    {
        freq = HAL_ClockGetIpFreq(hal_clock_m7);
        PRINTF("\r\n####################  Power Mode Switch Task ####################\n\r\n");
        PRINTF("    Build Time: %s--%s \r\n", __DATE__, __TIME__);
        PRINTF("    Core Clock: %dHz \r\n", freq);
        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c to enter: Normal RUN mode\r\n", kAPP_PowerModeRun);
        PRINTF("Press  %c to enter: WAIT mode\r\n", kAPP_PowerModeWait);
        PRINTF("Press  %c to enter: STOP mode\r\n", kAPP_PowerModeStop);
        PRINTF("Press  %c to enter: SUSPEND mode\r\n", kAPP_PowerModeSuspend);
        PRINTF("Press  P to suspend A55 core\r\n");
        PRINTF("Press  W to wakeup A55 core\r\n");
        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        /* Wait for user response */
        do
        {
            ch = GETCHAR();
        } while ((ch == '\r') || (ch == '\n'));

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        targetPowerMode = (lpm_power_mode_t)(ch - 'A');

        if (targetPowerMode <= LPM_PowerModeSuspend)
        {
            if (targetPowerMode == s_curMode)
            {
                /* Same mode, skip it */
                continue;
            }
            if (targetPowerMode == LPM_PowerModeSuspend)
            {
                PRINTF("Application get in Suspend mode\r\n");
            }
            if (!LPM_SetPowerMode(targetPowerMode))
            {
                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[targetPowerMode]);
            }
            else /* Idle task will handle the low power state. */
            {
                APP_GetWakeupConfig();
                APP_SetWakeupConfig(targetPowerMode);
                PRINTF("Target powermode get in %s\r\n", s_modeNames[targetPowerMode]);
                xSemaphoreTake(s_wakeupSig, portMAX_DELAY);
            }
        }
        else if ('P' == ch)
        {
            /* skip this command if A55 already in suspend mode. */
            uint32_t runMode, velow, vehigh;
            uint32_t sleepMode = CPU_SLEEP_MODE_RUN;
            int32_t status = SCMI_ERR_SUCCESS;

            status = SCMI_CpuInfoGet(SCMI_A2P, AP_DOMAIN_LD, &runMode, &sleepMode, &velow, &vehigh);
            if (status != SCMI_ERR_SUCCESS)
            {
                PRINTF("Get AP info fail\r\n");
            }

            if (sleepMode == CPU_SLEEP_MODE_SUSPEND)
            {
                PRINTF("AP already in suspend mode, skip this command\r\n");
                continue;
            }
            else
            {
                status = SCMI_LmmSuspend(SCMI_A2P, AP_DOMAIN_LD);
                if (status != SCMI_ERR_SUCCESS)
                {
                    PRINTF("SCMI_LmmSuspend A55 fail\r\n");
                }
            }
        }
        else if ('W' == ch)
        { 
            status = SCMI_LmmWake(SCMI_A2P, AP_DOMAIN_LD);
            if (status != SCMI_ERR_SUCCESS)
            {
                PRINTF("SCMI_LmmWake A55 fail\r\n");
            }
        }
        else
        {
            PRINTF("Invalid command %c[0x%x]\r\n", ch, ch);
        }

        /* update Mode state */
        s_curMode = LPM_PowerModeRun;      
        PRINTF("\r\nNext loop\r\n");
    }
}

void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc Failed!!!\r\n");
}

/* Called in PowerModeSwitchTask */
static bool APP_LpmListener(lpm_power_mode_t curMode, lpm_power_mode_t newMode, void *data)
{
    PRINTF("WorkingTask %d: Transfer from %s to %s\r\n", (uint32_t)data, s_modeNames[curMode], s_modeNames[newMode]);

    /* Do necessary preparation for this mode change */

    return true; /* allow this switch */
}

/*!
 * @brief simulating working task.
 */
static void WorkingTask(void *pvParameters)
{
    LPM_RegisterPowerListener(APP_LpmListener, pvParameters);

    for (;;)
    {
        /* Use App task logic to replace vTaskDelay */
        PRINTF("Task %d is working now\r\n", (uint32_t)pvParameters);
        vTaskDelay(portMAX_DELAY);
    }
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t irqMask;
    lpm_power_mode_t targetPowerMode;
    bool result;

    irqMask = DisableGlobalIRQ();

    /* Only when no context switch is pending and no task is waiting for the scheduler
     * to be unsuspended then enter low power entry.
     */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        targetPowerMode = LPM_GetPowerMode();
        if (targetPowerMode != LPM_PowerModeRun)
        {
            /* Only wait when target power mode is not running */
            APP_PowerPreSwitchHook(targetPowerMode);
            result = LPM_WaitForInterrupt((uint64_t)1000 * xExpectedIdleTime / configTICK_RATE_HZ);

            APP_PowerPostSwitchHook(targetPowerMode, result);
        }
    }
    EnableGlobalIRQ(irqMask);
}

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode)
{
    int32_t status = SCMI_ERR_SUCCESS;
    scmi_pd_lpm_config_t lpmConfig[2];

    /* M7 mix will be poweroff when M7 core enter suspend mode, TCM data reserved. */
    lpmConfig[0].domainId = PWR_MIX_SLICE_IDX_M7;
    lpmConfig[0].lpmSetting = SCMI_CPU_LPM_SETTING_ON_RUN_WAIT_STOP;
    lpmConfig[0].retMask = 1U << PWR_MEM_SLICE_IDX_M7;
     /* Configure LPM setting */
     status = SCMI_CpuPdLpmConfigSet(SCMI_A2P, CPU_IDX_M7P, 1U, lpmConfig);
     if (status != SCMI_ERR_SUCCESS)
     {
         PRINTF("SCMI_CpuPdLpmConfigSet SET FAIL\r\n");
     }
}

void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode, bool result)
{
    int32_t status = SCMI_ERR_SUCCESS;

    if (LPM_PowerModeRun != targetMode)
    {
        /* Set CPU sleep mode back to RUN */
        status = SCMI_CpuSleepModeSet(SCMI_A2P, CPU_IDX_M7P, SCMI_CPU_FLAGS_IRQ_MUX(0), SCMI_CPU_SLEEP_RUN);
        if (status != SCMI_ERR_SUCCESS)
        {
            PRINTF("SCMI_CpuSleepModeSet into RUN STATE FAIL\r\n");
        }
    }
    if (s_wakeupSource == kAPP_WakeupSourceSM)
    {
        /*
         * if select SM as M7 wakeup source, M7 only support message from MU5(system manager).
         * M7 don't need wakeup mix source, so that it will be poweroff after system suspend mode.
         * reinitialize LPUART3 and LPTMR2 and MU7 which locate at wakeupmix.
         */
        if (targetMode == LPM_PowerModeSuspend)
        {
            BOARD_InitBootPins();
            BOARD_InitDebugConsole();
            lptmr_config_t lptmrConfig;

            /* Resetup LPTMR. */
            LPTMR_GetDefaultConfig(&lptmrConfig);
            lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_2;  /* Use RTC 32KHz as clock source. */
            lptmrConfig.bypassPrescaler      = true;
            lptmrConfig.value                = kLPTMR_Prescale_Glitch_0; /* Divide clock source by 2. */
            LPTMR_Init(LPTMR2, &lptmrConfig);

            MU_Init(RPMSG_LITE_M7_A55_MU);
            NVIC_SetPriority(RPMSG_LITE_M7_A55_MU_IRQn, RPMSG_LITE_MU_IRQ_PRIORITY);
            NVIC_EnableIRQ(RPMSG_LITE_M7_A55_MU_IRQn);
#if !(defined(FSL_FEATURE_MU_NO_CORE_STATUS) && (0 != FSL_FEATURE_MU_NO_CORE_STATUS))
            MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterPowerDownInterruptEnable);
            MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterRunInterruptEnable);
            MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterHaltInterruptEnable);
            MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterWaitInterruptEnable);
            MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterStopInterruptEnable);
#endif
        }

        /* re-enable A55 message unit */
        EnableIRQ(MU7_B_IRQn);

        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }
    else
    {
        /* re-enable SM and A55 message unit */
        EnableIRQ(MU5_A_IRQn);
        EnableIRQ(MU7_B_IRQn);
    }

    uint32_t wakeMask[12] = {0x0};
    status = SCMI_CpuIrqWakeSet(SCMI_A2P, CPU_IDX_M7P, 0U, 12U, wakeMask);
    if (status != SCMI_ERR_SUCCESS)
    {
        PRINTF("cpu irq mask reset fail\r\n");
    }

    if (scmiRequestM7IntoSuspend == true)
    {
        scmiRequestM7IntoSuspend = false;
        /* Resume rtos task, M7 PowerModeSwitch task will resume in getchar() side. */
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
        vTaskResume(pxWorkingTask);
        vTaskResume(pxPowerModeSwitch);
        xTimerStart(suspendTimer, portMAX_DELAY);

        s_curMode = LPM_PowerModeRun;
    }
    PRINTF("== Power switch %s ==\r\n", result ? "OK" : "FAIL");
}

static void APP_SuspendTimerCallback(TimerHandle_t xTimer)
{
    int32_t status = SCMI_ERR_SUCCESS;
    /* Start timer to poll suspend status. */
    if (scmiRequestM7IntoSuspend == true)
    {
        DisableIRQ(MU7_B_IRQn);
        /* Default to no wakeup IRQs */
        uint32_t wakeMask[12] =
        {
           [0 ... 11]  = 0xFFFFFFFFU
        };
        /* IRQs enabled at NVIC level become GPC wake sources */
        for (uint32_t idx = 0; idx < 8U; idx++)
        {
            wakeMask[idx] = ~(NVIC->ISER[idx]);
        }
        /* deal with MU7 interrupt, configure IRQ wake sources. */
        wakeMask[6] = 0xFFFFDFFF;

        status = SCMI_CpuIrqWakeSet(SCMI_A2P, CPU_IDX_M7P, 0U, 12U, wakeMask);
        if (status != SCMI_ERR_SUCCESS)
        {
            PRINTF("SCMI_CpuIrqWakeSet set fail\r\n");
        }

        LPM_SetPowerMode(LPM_PowerModeSuspend);
        xTimerStop(suspendTimer, portMAX_DELAY);

        /* suspend below rtos task let M7 into idel task. */
        vTaskSuspend(pxWorkingTask);
        vTaskSuspend(pxPowerModeSwitch);
        xSemaphoreTake(s_wakeupSig, portMAX_DELAY);
    }
    else
    {
        xTimerStart(suspendTimer, portMAX_DELAY);
    }
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    LPM_Init();
    s_wakeupSig = xSemaphoreCreateBinary();

    suspendTimer =
        xTimerCreate("suspend", pdMS_TO_TICKS(APP_LINKUP_TIMER_PERIOD_MS), pdFALSE, NULL, APP_SuspendTimerCallback);
    assert(suspendTimer);
    xTimerStart(suspendTimer, portMAX_DELAY);

    xTaskCreate(PowerModeSwitchTask, "Main Task", 512U, NULL, tskIDLE_PRIORITY + 1U, &pxPowerModeSwitch);
    xTaskCreate(WorkingTask, "Working Task", configMINIMAL_STACK_SIZE, (void *)1, tskIDLE_PRIORITY + 2U, &pxWorkingTask);

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}



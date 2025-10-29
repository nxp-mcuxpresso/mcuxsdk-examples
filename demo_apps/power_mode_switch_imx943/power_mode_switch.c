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
//#include "sm_platform.h"
#include "fsl_adapter_timer.h"
#if defined(CPU_MIMX94398AVKM_cm7_core0)
#include "fsl_lptmr.h"
#endif

/*******************************************************************************
 * Struct Definitions
 ******************************************************************************/
typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceTimer,  /*!< Wakeup by TMR, M33_CORE1:TPM6, M7_CORE0:LPTMR2, M7_CORE1:TPM3. */
    kAPP_WakeupSourceLpuart, /*!< Wakeup by LPUART */
    kAPP_WakeupSourceSM      /*!< Wakeup by SM message, both of wakeup message from sm console and ONOFF button. */
} app_wakeup_source_t;

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
extern void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode);
extern void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode, bool result);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static IRQn_Type irqTbl[] = {SYSTEM_PLATFORM_MU_IRQ, RPMSG_LITE_MU_IRQ};
static uint32_t s_wakeupTimeout;           /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */
static SemaphoreHandle_t s_wakeupSig;
static const char *s_modeNames[] = {"RUN", "WAIT", "STOP", "SUSPEND"};
extern lpm_power_mode_t s_curMode;
static TimerHandle_t suspendTimer;
TaskHandle_t pxPowerModeSwitch;
TaskHandle_t pxWorkingTask;
TIMER_HANDLE_DEFINE(halWakupTimerHandle);

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
        PRINTF("Press T for TIMER - Timer\r\n");
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
            return kAPP_WakeupSourceTimer;
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

    if (kAPP_WakeupSourceTimer == s_wakeupSource)
    {
        /* Wakeup source is TIMER, user should input wakeup timeout value. */
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
    int wakeMaskIdx = 0;
    int wakeMaskBitPos = 0;


     /* Default to no wakeup IRQs */
     uint32_t wakeMask[GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT] =
     {
        [0 ... GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT - 1]  = 0xFFFFFFFFU
     };

     /* IRQs enabled at NVIC level become GPC wake sources */
     for (uint32_t idx = 0; idx < GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT && idx < MCU_NVIC_ISER_NUM; idx++)
     {
         wakeMask[idx] = ~(NVIC->ISER[idx]);
     }

    if (kAPP_WakeupSourceTimer == s_wakeupSource || kAPP_WakeupSourceLpuart == s_wakeupSource)
    {
        for (uint32_t idx = 0; idx < ARRAY_SIZE(irqTbl); idx++)
	{
            /* deal with interrupt mask from A55 and system manager. */
            wakeMaskIdx = irqTbl[idx] / 32;
            wakeMaskBitPos = irqTbl[idx] % 32;
            wakeMask[wakeMaskIdx] |= (1 << wakeMaskBitPos);
        }

        /*
         * LPUART and TIMER used for wakeup in wakeupmix, need assure its power on state in system suspend state.
         * Set WAKEUPMIX LPM response to Mcore based on wakeup configuration.
         */
        scmi_pd_lpm_config_t lpmConfig;
        lpmConfig.lpmSetting = SCMI_CPU_LPM_SETTING_ON_ALWAYS;
        lpmConfig.retMask = 0U;
        lpmConfig.domainId = APP_UART_PWR_MIX_SLICE_IDX;
        SCMI_CpuPdLpmConfigSet(SCMI_A2P, APP_CPU_ID, 1U, &lpmConfig);
        lpmConfig.domainId = APP_WAKEUP_TIMER_PWR_MIX_SLICE_IDX;
        SCMI_CpuPdLpmConfigSet(SCMI_A2P, APP_CPU_ID, 1U, &lpmConfig);

        if (kAPP_WakeupSourceTimer == s_wakeupSource)
        {
            /* s_wakeupTimeOut * 1000 * 1000: convert second to microsecond */
            HAL_TimerUpdateTimeout((hal_timer_handle_t)halWakupTimerHandle, s_wakeupTimeout * 1000U * 1000U);
            HAL_TimerEnable((hal_timer_handle_t)halWakupTimerHandle);
            /* Ensure not to be woken up by sm messages */
	    DisableIRQ(SYSTEM_PLATFORM_MU_IRQ);
            /* Ensure not to be woken up by A55 messages */
            DisableIRQ(RPMSG_LITE_MU_IRQ);
        }
        else
        {
            PRINTF("LPUART is used for wakeup source.\r\n");
            LPUART_EnableInterrupts(APP_UART, kLPUART_RxDataRegFullInterruptEnable);
            /* In low power mode, LPUART can generate wakeup via STAT[RXEDGIF]
             * Config STAT[RXINV] and BAUD[RXEDGIE] to enable STAT[RXEDGIF].
             */
            APP_UART->STAT &= ~LPUART_STAT_RXINV_MASK;
            APP_UART->BAUD |= LPUART_BAUD_RXEDGIE_MASK;

	    DisableIRQ(SYSTEM_PLATFORM_MU_IRQ);
            /* Ensure not to be woken up by A55 messages */
            DisableIRQ(RPMSG_LITE_MU_IRQ);

            /*
             * Set lpuart can be async wakeup when its clock is gated, LPCGs of lpuart need to be changed to CPU LPM controlled.
             * Mcore sned per lpm list to system manager, sm check and record request and will deal with when Mcore domain enter suspend mode.
             * In to assure lpuart can be used as system suspend wakeup source.
             */
            scmi_per_lpm_config_t perLpmConfig;
            perLpmConfig.perId = APP_CPU_PER_LPI_IDX_UART;
            perLpmConfig.lpmSetting = SCMI_CPU_LPM_SETTING_ON_RUN_WAIT_STOP;
            status = SCMI_CpuPerLpmConfigSet(SCMI_A2P, APP_CPU_ID, 1U, &perLpmConfig);
            if (status != SCMI_ERR_SUCCESS)
            {
                PRINTF("SCMI_CpuPerLpmConfigSet set fail\r\n");
            }
        }
    }

    if (s_wakeupSource == kAPP_WakeupSourceSM)
    {
        /* Ensure not to be woken up by A55 messages */
        DisableIRQ(RPMSG_LITE_MU_IRQ);
        /* Mcu can be only woken by MU(between mcu and sm), set rpmsg channel irq from Acore as wakeup mask. */
        wakeMaskIdx = RPMSG_LITE_MU_IRQ / 32;
        wakeMaskBitPos = RPMSG_LITE_MU_IRQ % 32;
        /* deal with MU interrupts, configure IRQ wake sources. */
        wakeMask[wakeMaskIdx] |= (1 << wakeMaskBitPos);
    }

     status = SCMI_CpuIrqWakeSet(SCMI_A2P, APP_CPU_ID, APP_CPU_ID_MASK_IDX, GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT, wakeMask);
     if (status != SCMI_ERR_SUCCESS)
     {
         PRINTF("SCMI_CpuIrqWakeSet set fail\r\n");
     }
}

void APP_ResumePeriperal()
{
    /*
     * In some special cases, such as using system manager as wakeup source, the mcore not require any periperal
     * located in wakeup mix to remained powered, so that wakeup mix will be poweroff in system suspend mode.
     * Do same resume periperal process to align it.
     */
    hal_timer_config_t halTimerConfig;
    BOARD_InitDebugConsole();

    halTimerConfig.timeout = 1000;
    halTimerConfig.srcClock_Hz = APP_WAKEUP_TIMER_CLOCK_RATE;
    halTimerConfig.instance = APP_WAKEUP_TIMER_INSTANCE_IDX;
#if defined(CPU_MIMX94398AVKM_cm7_core0)
    /* lptmr wakeup source use clock source2. */
    halTimerConfig.clockSrcSelect = kLPTMR_PrescalerClock_2;
#endif
    HAL_TimerInit((hal_timer_handle_t)halWakupTimerHandle, &halTimerConfig);

    MU_Init(RPMSG_LITE_MU);
    NVIC_SetPriority(RPMSG_LITE_MU_IRQ, RPMSG_LITE_MU_IRQ_PRIORITY);
    NVIC_EnableIRQ(RPMSG_LITE_MU_IRQ);
}

/* TIMER(use as wakeup source) interrupt handler. */
void APP_WAKEUP_TIMER_IRQHANDLER(void* param)
{
    HAL_TimerDisable((hal_timer_handle_t)halWakupTimerHandle);

    xSemaphoreGiveFromISR(s_wakeupSig, NULL);
    portYIELD_FROM_ISR(pdTRUE);
}

/* LPUART(use as wakeup source) interrupt handler. */
void APP_UART_IRQHANDLER(void)
{
    bool wakeup = false;
    if (kLPUART_RxDataRegFullInterruptEnable & LPUART_GetEnabledInterrupts(APP_UART))
    {
        LPUART_DisableInterrupts(APP_UART, kLPUART_RxDataRegFullInterruptEnable);
        APP_UART->BAUD &= ~LPUART_BAUD_RXEDGIE_MASK;
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
    hal_timer_config_t halTimerConfig;
    lpm_power_mode_t targetPowerMode;
    uint32_t freq = 0U;
    uint8_t ch;
    int32_t status = SCMI_ERR_SUCCESS;

    halTimerConfig.timeout = 1000;
    halTimerConfig.srcClock_Hz = APP_WAKEUP_TIMER_CLOCK_RATE;
    halTimerConfig.instance = APP_WAKEUP_TIMER_INSTANCE_IDX;
#if defined(CPU_MIMX94398AVKM_cm7_core0)
    /* lptmr wakeup source use clock source2. */
    halTimerConfig.clockSrcSelect = kLPTMR_PrescalerClock_2;
#endif
    HAL_TimerInit((hal_timer_handle_t)halWakupTimerHandle, &halTimerConfig);
    HAL_TimerInstallCallback((hal_timer_handle_t)halWakupTimerHandle, APP_WAKEUP_TIMER_IRQHANDLER, NULL);

    NVIC_SetPriority(APP_WAKEUP_TIMER_IRQ, APP_WAKEUP_TIMER_IRQ_PRIO);

    EnableIRQ(APP_WAKEUP_TIMER_IRQ);

    /* Setup LPUART. */
    NVIC_SetPriority(APP_UART_IRQ, APP_UART_IRQ_PRIO);

    EnableIRQ(APP_UART_IRQ);

    for (;;)
    {
        freq = CLOCK_GetRate(MCU_HAL_CLOCK_ID);
        PRINTF("\r\n####################  Power Mode Switch Task ####################\n\r\n");
        PRINTF("    Build Time: %s--%s \r\n", __DATE__, __TIME__);
        PRINTF("    Core Clock: %dHz \r\n", freq);
        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c to enter: Normal RUN mode\r\n", kAPP_PowerModeRun);
        PRINTF("Press  %c to enter: WAIT mode\r\n", kAPP_PowerModeWait);
        PRINTF("Press  %c to enter: STOP mode\r\n", kAPP_PowerModeStop);
        PRINTF("Press  %c to enter: SUSPEND mode\r\n", kAPP_PowerModeSuspend);
#if defined(CPU_MIMX94398AVKM_cm7_core0) || defined(CPU_MIMX94398AVKM_cm7_core1)
	/*
	 * The CM33S core is for network processing and not have access rights to shutdown and boot the AP now.
	 * Please update system manager config if you want enable this case in M33s core.
	 */
        PRINTF("Press  P to suspend A55 core\r\n");
        PRINTF("Press  W to wakeup A55 core\r\n");
#endif
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

        int modeIndex = ch - 'A';

#if defined(CPU_MIMX94398AVKM_cm7_core0) || defined(CPU_MIMX94398AVKM_cm7_core1)
        if ((ch != 'P') && (ch != 'W') && ((modeIndex < 0) || (modeIndex >= LPM_PowerModeCount)))
#else
        if (modeIndex < 0 || modeIndex >= LPM_PowerModeCount)
#endif
        {
            PRINTF("Invalid power mode input: %c\r\n", ch);
            continue;
        }

        targetPowerMode = (lpm_power_mode_t)modeIndex;

        if (targetPowerMode < LPM_PowerModeCount)
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

            status = SCMI_CpuInfoGet(SCMI_A2P, SYSTEM_PLATFORM_AP_ID, &runMode, &sleepMode, &velow, &vehigh);
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
                status = SCMI_LmmSuspend(SCMI_A2P, SM_PLATFORM_AP_LMM_ID);
                if (status != SCMI_ERR_SUCCESS)
                {
                    PRINTF("SCMI_LmmSuspend A55 fail\r\n");
                }
            }
        }
        else if ('W' == ch)
        { 
            status = SCMI_LmmWake(SCMI_A2P, SM_PLATFORM_AP_LMM_ID);
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
    lpmConfig[0].domainId = APP_CPU_PWR_MIX_SLICE_IDX;
    lpmConfig[0].lpmSetting = APP_LPM_SETTING;
    lpmConfig[0].retMask = 1U << APP_CPU_PWR_MEM_SLICE_IDX;
     /* Configure LPM setting */
     status = SCMI_CpuPdLpmConfigSet(SCMI_A2P, APP_CPU_ID, 1U, lpmConfig);
     if (status != SCMI_ERR_SUCCESS)
     {
         PRINTF("SCMI_CpuPdLpmConfigSet SET FAIL\r\n");
     }

    if (s_wakeupSource == kAPP_WakeupSourceSM)
    {
        /* Disable LMM notification from system manager before Mcore enter low power state. */
        SCMI_LmmNotify(SCMI_A2P, SYSTEM_LMID_A55, SCMI_LMM_NOTIFY_BOOT(0U) |
                        SCMI_LMM_NOTIFY_SHUTDOWN(0U) | SCMI_LMM_NOTIFY_SUSPEND(0U) | SCMI_LMM_NOTIFY_WAKE(0U));
    }
}

void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode, bool result)
{
    int32_t status = SCMI_ERR_SUCCESS;
    uint32_t wakeMask[GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT] = {0x0};

    if (LPM_PowerModeRun != targetMode)
    {
        /* Set CPU sleep mode back to RUN */
        status = SCMI_CpuSleepModeSet(SCMI_A2P, APP_CPU_ID, SCMI_CPU_FLAGS_IRQ_MUX(0), SCMI_CPU_SLEEP_RUN);
        if (status != SCMI_ERR_SUCCESS)
        {
            PRINTF("SCMI_CpuSleepModeSet into RUN STATE FAIL\r\n");
        }
    }
    if (SystemPlatformGetSystemState() == SCMI_SYS_STATE_FULL_SUSPEND ||
		    SystemPlatformGetSystemState() == SCMI_SYS_STATE_SUSPEND ||
		    s_wakeupSource == kAPP_WakeupSourceSM)
    {
        APP_ResumePeriperal();
    }
    if (s_wakeupSource == kAPP_WakeupSourceSM)
    {
        /* re-enable A55 message unit */
        EnableIRQ(RPMSG_LITE_MU_IRQ);
        /* Re-enable LMM notification from system manager after Mcore exit low power state. */
        SCMI_LmmNotify(SCMI_A2P, SYSTEM_LMID_A55, SCMI_LMM_NOTIFY_BOOT(1U) |
                        SCMI_LMM_NOTIFY_SHUTDOWN(1U) | SCMI_LMM_NOTIFY_SUSPEND(1U) | SCMI_LMM_NOTIFY_WAKE(1U));
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }
    else
    {
        /* re-enable SM and A55 message unit */
	EnableIRQ(SYSTEM_PLATFORM_MU_IRQ);
        EnableIRQ(RPMSG_LITE_MU_IRQ);
    }

    status = SCMI_CpuIrqWakeSet(SCMI_A2P, APP_CPU_ID, APP_CPU_ID_MASK_IDX, GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT, wakeMask);
    if (status != SCMI_ERR_SUCCESS)
    {
        PRINTF("cpu irq mask reset fail\r\n");
    }
    if (SystemPlatformGetSystemState() == SCMI_SYS_STATE_FULL_SUSPEND || SystemPlatformGetSystemState() == SCMI_SYS_STATE_SUSPEND)
    {
        /* Re-enable LMM notification from system manager after Mcore exit low power state. */
        SCMI_LmmNotify(SCMI_A2P, SYSTEM_LMID_A55, SCMI_LMM_NOTIFY_BOOT(1U) |
                        SCMI_LMM_NOTIFY_SHUTDOWN(1U) | SCMI_LMM_NOTIFY_SUSPEND(1U) | SCMI_LMM_NOTIFY_WAKE(1U));
        SystemPlatformSetSystemState(0xFFFFFFFF);
        /* Resume rtos task, Mcu PowerModeSwitch task will resume in getchar() side. */
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
    int wakeMaskIdx = 0;
    int wakeMaskBitPos = 0;

    /* Start timer to poll suspend status. */
    if (SystemPlatformGetSystemState() == SCMI_SYS_STATE_FULL_SUSPEND || SystemPlatformGetSystemState() == SCMI_SYS_STATE_SUSPEND)
    {
        /* Disable LMM notification from system manager before Mcore enter low power state. */
        SCMI_LmmNotify(SCMI_A2P, SYSTEM_LMID_A55, SCMI_LMM_NOTIFY_BOOT(0U) |
                        SCMI_LMM_NOTIFY_SHUTDOWN(0U) | SCMI_LMM_NOTIFY_SUSPEND(0U) | SCMI_LMM_NOTIFY_WAKE(0U));
        DisableIRQ(RPMSG_LITE_MU_IRQ);
        /* Default to no wakeup IRQs */
        uint32_t wakeMask[GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT] =
        {
           [0 ... GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT - 1]  = 0xFFFFFFFFU
        };
        /* IRQs enabled at NVIC level become GPC wake sources */
        for (uint32_t idx = 0; idx < GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT && idx < MCU_NVIC_ISER_NUM; idx++)
        {
            wakeMask[idx] = ~(NVIC->ISER[idx]);
        }
        /* deal with MU(between mcu and mpu) interrupt, configure IRQ wake sources. */
        wakeMaskIdx = RPMSG_LITE_MU_IRQ / 32;
        wakeMaskBitPos = RPMSG_LITE_MU_IRQ % 32;
        wakeMask[wakeMaskIdx] |= (1 << wakeMaskBitPos);

        status = SCMI_CpuIrqWakeSet(SCMI_A2P, APP_CPU_ID, APP_CPU_ID_MASK_IDX, GPC_CPU_CTRL_CMC_IRQ_WAKEUP_MASK_COUNT, wakeMask);
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




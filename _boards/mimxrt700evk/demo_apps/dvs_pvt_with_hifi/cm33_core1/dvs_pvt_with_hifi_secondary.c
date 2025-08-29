/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "pmic_support.h"
#include "pvt_app_config.h"
#include "fsl_lpuart.h"
#include "fsl_gpio.h"
#include "fsl_power.h"
#include "fsl_mu.h"
#include "fsl_pvts.h"
#include "pmic_support.h"
#include "fsl_utick.h"
#include "lpm.h"
#include "dsp_support.h"
#include "fsl_dsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "coremark.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * Task priorities.
 * PVT Should be highest priority so it can increase VDD1 ASAP if the PVT interrupt triggers.
 */
/*Workload task is a simulation of workload, pvt task is for reduce vdd1 voltage*/
#define workload_task_PRIORITY (configMAX_PRIORITIES - 2)
#define pvt_task_PRIORITY      (configMAX_PRIORITIES - 1)

#define WORKLOAD_TIMES (5U)

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t delay;
#if (CPU1_PVT_ENABLED != 0U)
TaskHandle_t pvts_task_handle;
volatile uint32_t cur_voltage           = MAX_VDD1;
volatile static uint32_t workload_index = 0;

/* Used for pvt_task to make sure that when pvt_task is running, even if there is a vtaskDelay, CPU won't go to
 * deepsleep mode.*/
volatile bool adjusting = false;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void PVTS1_CPU1_DriverIRQHandler(void);
void PVTS1_HIFI1_DriverIRQHandler(void);

static void pvt_task(void *pvParameters);
static void workload_task(void *pvParameters);
static void pvt_wait_timer_callback(void);
static bool adjust_vdd1(void);
static void config_pvts(void);
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
#if (CPU1_PVT_ENABLED != 0U)
void LPM_WaitForInterrupt(void)
{
#if defined(CPU1_DEMO_PVT_ON_CPU_DSP) && (CPU1_DEMO_PVT_ON_CPU_DSP != 0U) /* When DSP used, only enter sleep mode. */
    BOARD_EnterSleep();
#else
    BOARD_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
#endif
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint64_t xOstimerStartValue = 0;
    uint32_t irqMask;
    TickType_t timeOutTicks;

    if (xExpectedIdleTime == 0)
    {
        return;
    }

    irqMask = DisableGlobalIRQ();

    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        if (!adjusting)
        {
            timeOutTicks = LPM_EnterTicklessIdle(xExpectedIdleTime, &xOstimerStartValue);

            if (timeOutTicks != 0U)
            {
                LPM_WaitForInterrupt();
                LPM_ExitTicklessIdle(timeOutTicks, xOstimerStartValue);
            }
        }
        else
        {
            __DSB();
            __ISB();
            __WFI();
        }
    }

    EnableGlobalIRQ(irqMask);
}
#if defined(CPU1_DEMO_PVT_ON_CPU_DSP) && (CPU1_DEMO_PVT_ON_CPU_DSP != 2U)
void PVTS1_CPU1_DriverIRQHandler(void)
{
#if defined(CPU1_DEMO_PRINTF_INTERRUPT_STATUS) && (CPU1_DEMO_PRINTF_INTERRUPT_STATUS == 1)
    PRINTF("PVT CPU INT\r\n");
#endif
    NVIC_ClearPendingIRQ((IRQn_Type)PVTS1_CPU1_IRQn);
    PVTS_Stop(kPVTS_Sensor0);

    /* Wake-up the PVT task to increase VDD1 immediately. */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(pvts_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif

#if defined(CPU1_DEMO_PVT_ON_CPU_DSP) && (CPU1_DEMO_PVT_ON_CPU_DSP != 0U)
void PVTS1_HIFI1_DriverIRQHandler(void)
{
#if defined(CPU1_DEMO_PRINTF_INTERRUPT_STATUS) && (CPU1_DEMO_PRINTF_INTERRUPT_STATUS == 1)
    DEMO_LOG("PVT DSP INT\r\n");
#endif
    NVIC_ClearPendingIRQ((IRQn_Type)PVTS1_HIFI1_IRQn);
    PVTS_Stop(kPVTS_Sensor1);
    /* Wake-up the PVT task to increase VDD1 immediately */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(pvts_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif

/*UTick timer callback*/
static void pvt_wait_timer_callback(void)
{
    /* Wake-up the PVT task to increase VDD1 immediately */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(pvts_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void config_pvts(void)
{
    PVTS_Init();
#if defined(CPU1_DEMO_PVT_ON_CPU_DSP) && (CPU1_DEMO_PVT_ON_CPU_DSP != 2U)
    /*Enable interupt*/
    NVIC_ClearPendingIRQ((IRQn_Type)PVTS1_CPU1_IRQn);
    NVIC_SetPriority((IRQn_Type)PVTS1_CPU1_IRQn, 2U);
    EnableIRQ((IRQn_Type)PVTS1_CPU1_IRQn);
    /* This is used to check if the PVT interrupt triggered when trying to optimize VDD1 */
    PVTS_EnableAlertCount(kPVTS_Sensor0);
    /* Set the delay and start sensing */
    PVTS_SetDelay(kPVTS_Sensor0, PVTS_GET_DELAY0_FROM_FUSE_VALUE(delay));

    PVTS_Start(kPVTS_Sensor0);
#endif

#if defined(CPU1_DEMO_PVT_ON_CPU_DSP) && (CPU1_DEMO_PVT_ON_CPU_DSP != 0U)
    NVIC_ClearPendingIRQ(PVTS1_HIFI1_IRQn);
    NVIC_SetPriority(PVTS1_HIFI1_IRQn, 3U);
    EnableIRQ(PVTS1_HIFI1_IRQn);
    /* This is used to check if the PVT interrupt triggered when trying to optimize VDD1 */
    PVTS_EnableAlertCount(kPVTS_Sensor1);
    /* Set the delay and start sensing */
    PVTS_SetDelay(kPVTS_Sensor1, PVTS_GET_DELAY1_FROM_FUSE_VALUE(delay));

    PVTS_Start(kPVTS_Sensor1);
#endif
}
#endif /* CPU1_PVT_ENABLED */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();
    POWER_ClearEventFlags(0xFFFFFFFF);

    DEMO_LOG(
        "PVT Application Demo on Core1\r\n\n"
        "Build Time: %s--%s \r\n",
        __DATE__, __TIME__);

    DEMO_LOG("Core Clock: %dHz \r\n", CLOCK_GetCoreSysClkFreq());

#if (CPU1_DEMO_PVT_ON_CPU_DSP != 0U)
    BOARD_BootDSP();
#endif

    BOARD_NotifyBoot(); /* Set boot flag. */

#if defined(CPU1_PVT_ENABLED) && (CPU1_PVT_ENABLED == 1)
    /* Wait for OTP values sent from CPU0 */
    delay = MU_ReceiveMsg(APP_MU, kMU_MsgReg0);
    MU_Deinit(MU1_MUB);

    PRINTF("PVTS delay = 0x%x\r\n", delay);

    /*Create tickless task*/
    if (xTaskCreate(pvt_task, "PVT_task", configMINIMAL_STACK_SIZE + 500U, NULL, pvt_task_PRIORITY,
                    &pvts_task_handle) != pdPASS)
    {
        PRINTF("PVT Task creation failed!.\r\n");
        while (1)
        {
        }
    }

    if (xTaskCreate(workload_task, "Workload_task", configMINIMAL_STACK_SIZE + 1000U, NULL, workload_task_PRIORITY,
                    NULL) != pdPASS)
    {
        PRINTF("Workload Task creation failed!.\r\n");
        while (1)
        {
        }
    }

    /*Task Scheduler*/
    vTaskStartScheduler();
    for (;;)
        ;
#else
    PRINTF("Entering Deep Sleep...\r\n");
    BOARD_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
#endif
}

#if (CPU1_PVT_ENABLED != 0U)
/**
 * Increases VDD1 if the PVT interrupt triggered, else it tries
 * to decrease VDD1 as much as possible.
 *
 * Returns true if VDD1 == MIN_VDD1, false otherwise.
 */
static bool adjust_vdd1(void)
{
    cur_voltage = BOARD_GetPmicVdd1Voltage();

    if ((PVTS_GetAlertCount(kPVTS_Sensor0) > 0) || (PVTS_GetAlertCount(kPVTS_Sensor1) > 0))
    {
        /* PVT interrupt fired, so we increase VDD1 */
        cur_voltage = cur_voltage + DEMO_PMIC_ADJUST_STEP;
        BOARD_SetPmicVdd1Voltage(cur_voltage);
        pvt_wait_timer_start(CPU1_PMIC_SETTLING_TIME_MS, pvt_wait_timer_callback);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#if defined(CPU1_DEMO_PVT_ON_CPU_DSP) && (CPU1_DEMO_PVT_ON_CPU_DSP != 2U)
        if (PVTS_GetAlertCount(kPVTS_Sensor0) > 0)
        {
            PVTS_ClearAlertCount(kPVTS_Sensor0);
            PVTS_Start(kPVTS_Sensor0);
        }
#endif
#if defined(CPU1_DEMO_PVT_ON_CPU_DSP) && (CPU1_DEMO_PVT_ON_CPU_DSP != 0U)
        if (PVTS_GetAlertCount(kPVTS_Sensor1) > 0)
        {
            PVTS_ClearAlertCount(kPVTS_Sensor1);
            PVTS_Start(kPVTS_Sensor1);
        }
#endif
        return false;
    }

    /* Try to decrease VDD1 as much as possible until we reach MIN_VDD1 or PVT interrupt fires */
    while (cur_voltage > MIN_VDD1)
    {
        cur_voltage = cur_voltage - DEMO_PMIC_ADJUST_STEP;
        BOARD_SetPmicVdd1Voltage(cur_voltage);
        pvt_wait_timer_start(CPU1_PMIC_SETTLING_TIME_MS, pvt_wait_timer_callback);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if ((PVTS_GetAlertCount(kPVTS_Sensor0) > 0) || (PVTS_GetAlertCount(kPVTS_Sensor1) > 0))
        {
            cur_voltage = cur_voltage + DEMO_PMIC_ADJUST_STEP;
            BOARD_SetPmicVdd1Voltage(cur_voltage);
            pvt_wait_timer_start(CPU1_PMIC_SETTLING_TIME_MS, pvt_wait_timer_callback);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#if defined(CPU1_DEMO_PVT_ON_CPU_DSP) && (CPU1_DEMO_PVT_ON_CPU_DSP != 2U)
            if (PVTS_GetAlertCount(kPVTS_Sensor0) > 0)
            {
                PVTS_ClearAlertCount(kPVTS_Sensor0);
                PVTS_Start(kPVTS_Sensor0);
            }
#endif
#if defined(CPU1_DEMO_PVT_ON_CPU_DSP) && (CPU1_DEMO_PVT_ON_CPU_DSP != 0U)
            if (PVTS_GetAlertCount(kPVTS_Sensor1) > 0)
            {
                PVTS_ClearAlertCount(kPVTS_Sensor1);
                PVTS_Start(kPVTS_Sensor1);
            }
#endif
            break;
        }
    }

    return (cur_voltage <= MIN_VDD1);
}

/* pvt_task */
static void pvt_task(void *pvParameters)
{
    bool at_min_vdd1 = false;

    /* Start the pvt sensor */
    config_pvts();

    /* Initialize timer for PVT task */
    pvt_wait_timer_init();

    for (;;)
    {
        /* Adjust the VDD1. */
        {
            adjusting   = true;
            at_min_vdd1 = adjust_vdd1();
            adjusting   = false;
        }

        DEMO_LOG("PVT Task: Current VDD1 = %f V\r\n", (double)cur_voltage / 1000000.0);

        if (!at_min_vdd1)
        {
            /*Delay using utick and Notify */
            /* Only enable periodic timer if it's possible for VDD1 to go lower */
            pvt_wait_timer_start(CPU1_PVT_TASK_WAIT_MS, pvt_wait_timer_callback);
        }

        /* Wait for UTICK interrupt notification */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}

/* workload_task */
static void workload_task(void *pvParameters)
{
    for (;;)
    {
        for (workload_index = 0; workload_index < WORKLOAD_TIMES; workload_index++)
        {
            /* Simulate workload. */
            coremark_main();
        }

        PRINTF("Workload Task: Current VDD1 = %f V\r\n", (double)cur_voltage / 1000000.0);

        vTaskDelay(pdMS_TO_TICKS(CPU1_WORKLOAD_DELAY_MS));
    }
}
#endif
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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "coremark.h"

#include "fsl_lpadc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * Task priorities.
 * PVT Should be highest priority so it can increase VDD2 ASAP if the PVT interrupt triggers.
 */
/*Workload task is a simulation of workload, pvt task is for reduce vdd2 voltage*/
#define workload_task_PRIORITY        (configMAX_PRIORITIES - 3)
#define temp_monitoring_task_PRIORITY (configMAX_PRIORITIES - 2)
#define pvt_task_PRIORITY             (configMAX_PRIORITIES - 1)

#define WORKLOAD_TIMES (5U)

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t delay;

TaskHandle_t pvts_task_handle;
volatile uint32_t cur_voltage           = MAX_VDD2;
volatile static uint32_t workload_index = 0;

/* Used for pvt_task to make sure that when pvt_task is running, even if there is a vtaskDelay, CPU won't go to
 * deepsleep mode.*/
volatile bool adjusting = false;

/*
 * Temperature monitoring variables
 */
volatile bool g_LpadcConversionCompletedFlag = false;
volatile uint16_t g_LpadcConvValue           = 0U;
lpadc_conv_result_t g_LpadcResultConfigStruct;
#if (defined(APP_LPADC_USE_HIGH_RESOLUTION) && APP_LPADC_USE_HIGH_RESOLUTION)
const uint32_t g_LpadcFullRange   = 65536U;
const uint32_t g_LpadcResultShift = 0U;
#else
const uint32_t g_LpadcFullRange   = 4096U;
const uint32_t g_LpadcResultShift = 3U;
#endif /* APP_LPADC_USE_HIGH_RESOLUTION */
volatile static double temperature_result = 0;

enum temperature_range
{
    range_m30C_to_30C = 0,
    range_30C_to_85C  = 1
};

uint32_t prev_temp_range = 0;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void pvt_task(void *pvParameters);
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 2U)
static void workload_task(void *pvParameters);
#endif
#if (DEMO_TEMPERATURE_MONITORING != 0U)
static void temp_monitoring_task(void *pvParameters);
#endif

void PVTS0_CPU0_DriverIRQHandler(void);
void PVTS0_HIFI4_DriverIRQHandler(void);

static void pvt_wait_timer_callback(void);
static bool adjust_vdd2(void);
static void config_pvts(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(DEMO_TEMPERATURE_MONITORING) && (DEMO_TEMPERATURE_MONITORING == 1)
void APP_LPADC_IRQ_HANDLER_FUNC(void)
{
#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
    if (LPADC_GetConvResult(APP_LPADC_BASE, &g_LpadcResultConfigStruct, 0U))
#else
    if (LPADC_GetConvResult(APP_LPADC_BASE, &g_LpadcResultConfigStruct))
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
    {
        g_LpadcConvValue               = (g_LpadcResultConfigStruct.convValue) >> g_LpadcResultShift;
        g_LpadcConversionCompletedFlag = true;
    }
    SDK_ISR_EXIT_BARRIER;
}
#endif

void LPM_WaitForInterrupt(void)
{
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 0U) /* When DSP used, only enter sleep mode. */
    BOARD_EnterSleep();
#else                                                                     /* Only run PVT on CPU. */
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

#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 2U)
void PVTS0_CPU0_DriverIRQHandler(void)
{
#if defined(CPU0_DEMO_PRINTF_INTERRUPT_STATUS) && (CPU0_DEMO_PRINTF_INTERRUPT_STATUS == 1)
    PRINTF("PVT CPU INT\r\n");
#endif
    NVIC_ClearPendingIRQ((IRQn_Type)PVTS0_CPU0_IRQn);
    PVTS_Stop(kPVTS_Sensor0);

    /* Wake-up the PVT task to increase VDD2 immediately. */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(pvts_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif

#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 0U)
void PVTS0_HIFI4_DriverIRQHandler(void)
{
#if defined(CPU0_DEMO_PRINTF_INTERRUPT_STATUS) && (CPU0_DEMO_PRINTF_INTERRUPT_STATUS == 1)
    DEMO_LOG("PVT DSP INT\r\n");
#endif
    NVIC_ClearPendingIRQ((IRQn_Type)PVTS0_HIFI4_IRQn);
    PVTS_Stop(kPVTS_Sensor1);
    /* Wake-up the PVT task to increase VDD2 immediately */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(pvts_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif

/*UTick timer callback*/
static void pvt_wait_timer_callback(void)
{
    /* Wake-up the PVT task to increase VDD2 immediately */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(pvts_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void config_pvts(void)
{
    PVTS_Init();

#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 2U)
    /*Enable interupt*/
    NVIC_ClearPendingIRQ((IRQn_Type)PVTS0_CPU0_IRQn);
    NVIC_SetPriority((IRQn_Type)PVTS0_CPU0_IRQn, 2U);
    EnableIRQ((IRQn_Type)PVTS0_CPU0_IRQn);
    /* This is used to check if the PVT interrupt triggered when trying to optimize VDD2 */
    PVTS_EnableAlertCount(kPVTS_Sensor0);
    /* Set the delay and start sensing */
    PVTS_SetDelay(kPVTS_Sensor0, PVTS_GET_DELAY0_FROM_FUSE_VALUE(delay));

    PVTS_Start(kPVTS_Sensor0);
#endif

#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 0U)
    NVIC_ClearPendingIRQ(PVTS0_HIFI4_IRQn);
    NVIC_SetPriority(PVTS0_HIFI4_IRQn, 3U);
    EnableIRQ(PVTS0_HIFI4_IRQn);
    /* This is used to check if the PVT interrupt triggered when trying to optimize VDD2 */
    PVTS_EnableAlertCount(kPVTS_Sensor1);
    /* Set the delay and start sensing */
    PVTS_SetDelay(kPVTS_Sensor1, PVTS_GET_DELAY1_FROM_FUSE_VALUE(delay));

    PVTS_Start(kPVTS_Sensor1);
#endif
}

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    status_t ret;

    /* Init board hardware. */
    BOARD_InitHardware();
    POWER_ClearEventFlags(0xFFFFFFFF);

    DEMO_LOG(
        "PVT Application Demo on Core0\r\n\n"
        "This application uses the PMIC for power supply.\r\n"
        "SILICON_REV_ID = %X%X\r\n"
        "Build Time: %s--%s \r\n",
        (SYSCON3->SILICONREV_ID & SYSCON3_SILICONREV_ID_MAJOR_MASK) >> SYSCON3_SILICONREV_ID_MAJOR_SHIFT,
        SYSCON3->SILICONREV_ID & SYSCON3_SILICONREV_ID_MINOR_MASK, __DATE__, __TIME__);

#if (CPU1_PVT_ENABLED != 0U)
    uint32_t sense_pvt_delays = 0;
    ret = PVTS_ReadDelayFromOTP(false, kPVTS_Vdd1Sense, CPU1_DEMO_MAINCLK_FREQ, &sense_pvt_delays);
    if (ret == kStatus_Fail)
    {
        DEMO_LOG("Failed to read Sense PVTS delay, please check the sample!\r\n");
        sense_pvt_delays = CPU1_DEMO_TYPICAL_DELAY;
    }
#endif

    ret = PVTS_ReadDelayFromOTP(false, kPVTS_Vdd2Com, CPU0_DEMO_MAINCLK_FREQ, &delay);

    /* !!!NOTE!!!
     * Use typical value to run the demo if failed to read delay value from OTP.
     * This is only used to allow runing the demo on some early samples which without delay values programed in fuse
     * and can't be used in production. The function may not work for some samples and temperature.
     */
    if (ret == kStatus_Fail)
    {
        DEMO_LOG("Failed to read Compute PVTS delay, please check the sample!\r\n");
        delay = CPU0_DEMO_TYPICAL_DELAY;
    }
    PRINTF("PVTS delay = 0x%x\r\n", delay);

    DEMO_LOG("Core Clock: %dHz \r\n", CLOCK_GetCoreSysClkFreq());
    DEMO_LOG("Input any key to start\r\n");
    (void)GETCHAR();

#if defined(DEMO_TEMPERATURE_MONITORING) && (DEMO_TEMPERATURE_MONITORING == 1)
    BOARD_InitTempSensor();
#endif
    /* Initialze power/clock configuration. */
    BOARD_InitPowerConfig();

    /* Boot and wait CPU1 booted. */
    DEMO_BootCore1();

    /* After the CPU1 booted, CPU0 relinquish the domain's control over the modules, and give the other domain exclusive
     * control. */
    BOARD_PowerConfigAfterCPU1Booted();

    /* Send PVT delays to CPU1 */
#if defined(CPU1_PVT_ENABLED) && (CPU1_PVT_ENABLED != 0U)
    MU_SendMsg(APP_MU, APP_MU_REG, sense_pvt_delays);
#endif

#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 0U)
    BOARD_BootDSP();
#endif

    /*Create tickless task*/
    if (xTaskCreate(pvt_task, "PVT_task", configMINIMAL_STACK_SIZE + 500U, NULL, pvt_task_PRIORITY,
                    &pvts_task_handle) != pdPASS)
    {
        PRINTF("PVT Task creation failed!.\r\n");
        while (1)
        {
        }
    }

#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 2U)
    if (xTaskCreate(workload_task, "Workload_task", configMINIMAL_STACK_SIZE + 1000U, NULL, workload_task_PRIORITY,
                    NULL) != pdPASS)
    {
        PRINTF("Workload Task creation failed!.\r\n");
        while (1)
        {
        }
    }
#endif
#if defined(DEMO_TEMPERATURE_MONITORING) && (DEMO_TEMPERATURE_MONITORING == 1)
    if (xTaskCreate(temp_monitoring_task, "Temperature_monitoring_task", configMINIMAL_STACK_SIZE + 200U, NULL,
                    temp_monitoring_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("temperature monitoring Task creation failed!.\r\n");
        while (1)
        {
        }
    }
#endif
    /*Task Scheduler*/
    vTaskStartScheduler();
    for (;;)
        ;
}

/**
 * Increases VDD2 if the PVT interrupt triggered, else it tries
 * to decrease VDD2 as much as possible.
 *
 * Returns true if VDD2 == MIN_VDD2, false otherwise.
 */
static bool adjust_vdd2(void)
{
    cur_voltage = BOARD_GetPmicVdd2Voltage();

    if ((PVTS_GetAlertCount(kPVTS_Sensor0) > 0) || (PVTS_GetAlertCount(kPVTS_Sensor1) > 0))
    {
        /* PVT interrupt fired, so we increase VDD2 */
        cur_voltage = cur_voltage + DEMO_PMIC_ADJUST_STEP;
        BOARD_SetPmicVdd2Voltage(cur_voltage);
        pvt_wait_timer_start(CPU0_PMIC_SETTLING_TIME_MS, pvt_wait_timer_callback);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 2U)
        if (PVTS_GetAlertCount(kPVTS_Sensor0) > 0)
        {
            PVTS_ClearAlertCount(kPVTS_Sensor0);
            PVTS_Start(kPVTS_Sensor0);
        }
#endif
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 0U)
        if (PVTS_GetAlertCount(kPVTS_Sensor1) > 0)
        {
            PVTS_ClearAlertCount(kPVTS_Sensor1);
            PVTS_Start(kPVTS_Sensor1);
        }
#endif
        return false;
    }

    /* Try to decrease VDD2 as much as possible until we reach MIN_VDD2 or PVT interrupt fires */
    while (cur_voltage > MIN_VDD2)
    {
        cur_voltage = cur_voltage - DEMO_PMIC_ADJUST_STEP;
        BOARD_SetPmicVdd2Voltage(cur_voltage);
        pvt_wait_timer_start(CPU0_PMIC_SETTLING_TIME_MS, pvt_wait_timer_callback);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if ((PVTS_GetAlertCount(kPVTS_Sensor0) > 0) || (PVTS_GetAlertCount(kPVTS_Sensor1) > 0))
        {
            cur_voltage = cur_voltage + DEMO_PMIC_ADJUST_STEP;
            BOARD_SetPmicVdd2Voltage(cur_voltage);
            pvt_wait_timer_start(CPU0_PMIC_SETTLING_TIME_MS, pvt_wait_timer_callback);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 2U)
            if (PVTS_GetAlertCount(kPVTS_Sensor0) > 0)
            {
                PVTS_ClearAlertCount(kPVTS_Sensor0);
                PVTS_Start(kPVTS_Sensor0);
            }
#endif
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 0U)
            if (PVTS_GetAlertCount(kPVTS_Sensor1) > 0)
            {
                PVTS_ClearAlertCount(kPVTS_Sensor1);
                PVTS_Start(kPVTS_Sensor1);
            }
#endif
            break;
        }
    }

    return (cur_voltage <= MIN_VDD2);
}

/* pvt_task */
static void pvt_task(void *pvParameters)
{
    bool at_min_vdd2 = false;

    /* Start the pvt sensor */
    config_pvts();

    /* Initialize timer for PVT task */
    pvt_wait_timer_init();

    for (;;)
    {
        /* Adjust the VDD2. */
        {
            adjusting   = true;
            at_min_vdd2 = adjust_vdd2();
            adjusting   = false;
        }

        DEMO_LOG("PVT Task: Current VDD2 = %f V\r\n", (double)cur_voltage / 1000000.0);

        if (!at_min_vdd2)
        {
            /*Delay using utick and Notify */
            /* Only enable periodic timer if it's possible for VDD2 to go lower */
            pvt_wait_timer_start(CPU0_PVT_TASK_WAIT_MS, pvt_wait_timer_callback);
        }

        /* Wait for UTICK interrupt notification */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 2U)
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

        PRINTF("Workload Task: Current VDD2 = %f V\r\n", (double)cur_voltage / 1000000.0);

        vTaskDelay(pdMS_TO_TICKS(CPU0_WORKLOAD_DELAY_MS));
    }
}
#endif

#if defined(DEMO_TEMPERATURE_MONITORING) && (DEMO_TEMPERATURE_MONITORING == 1)
/* temperature monitoring task */
static void temp_monitoring_task(void *pvParameters)
{
    uint32_t cur_temp_range = range_m30C_to_30C;
    for (;;)
    {
        temperature_result = (double)DEMO_GetTempsenorValue();
        cur_temp_range     = (temperature_result < TEMPERATURE_RANGE_LIMIT) ? range_m30C_to_30C : range_30C_to_85C;

        /* if current temperature switches from low temp range to high temp range, one can reduce the PVT delay */
        if (cur_temp_range == range_30C_to_85C && prev_temp_range == range_m30C_to_30C)
        {
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 2U)
            PVTS_Stop(kPVTS_Sensor0);
            PVTS_SetDelay(kPVTS_Sensor0, PVTS_GET_DELAY0_FROM_FUSE_VALUE(delay) - PVT_DELAY_TEMP_VARIATION);
            PVTS_ClearAlertCount(kPVTS_Sensor0);
            PVTS_Start(kPVTS_Sensor0);
#endif
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 0U)
            PVTS_Stop(kPVTS_Sensor1);
            PVTS_SetDelay(kPVTS_Sensor1, PVTS_GET_DELAY1_FROM_FUSE_VALUE(delay) - PVT_DELAY_TEMP_VARIATION);
            PVTS_ClearAlertCount(kPVTS_Sensor1);
            PVTS_Start(kPVTS_Sensor1);
#endif
        } /*if current temperature switches from high temp range to low temp range, one can restore original PVT delay
           */
        else if (cur_temp_range == range_m30C_to_30C && prev_temp_range == range_30C_to_85C)
        {
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && ((CPU0_DEMO_PVT_ON_CPU_DSP == 0U) || (CPU0_DEMO_PVT_ON_CPU_DSP == 1U))
            PVTS_Stop(kPVTS_Sensor0);
            PVTS_SetDelay(kPVTS_Sensor0, PVTS_GET_DELAY0_FROM_FUSE_VALUE(delay) + PVT_DELAY_TEMP_VARIATION);
            PVTS_ClearAlertCount(kPVTS_Sensor0);
            PVTS_Start(kPVTS_Sensor0);
#endif
#if defined(CPU0_DEMO_PVT_ON_CPU_DSP) && (CPU0_DEMO_PVT_ON_CPU_DSP != 0U)
            PVTS_Stop(kPVTS_Sensor1);
            PVTS_SetDelay(kPVTS_Sensor1, PVTS_GET_DELAY1_FROM_FUSE_VALUE(delay) + PVT_DELAY_TEMP_VARIATION);
            PVTS_ClearAlertCount(kPVTS_Sensor1);
            PVTS_Start(kPVTS_Sensor1);
#endif
        }
        prev_temp_range = cur_temp_range;

        PRINTF("Temperature monitoring task = %f C\r\n", temperature_result);

        vTaskDelay(pdMS_TO_TICKS(TEMP_MONITORING_DELAY_MS));
    }
}
#endif

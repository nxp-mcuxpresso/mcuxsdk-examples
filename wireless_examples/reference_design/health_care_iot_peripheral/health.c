/*
 * Copyright 2022-2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_os_abstraction.h"
#include "fsl_component_mem_manager.h"
#include "fsl_debug_console.h"
#include "msgq.h"
#include "fsl_component_timer_manager.h"
#include "connectivity.h"
#include "safety.h"
#include "datastore.h"
#ifdef ENABLE_LOW_POWER
#include "fsl_pm_core.h"
#endif
#include "measurement.h"
#include "health.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** The priority of the health task */
#define HEALTH_TASK_PRIO 2
/** The stack size of the health task */
#define HEALTH_TASK_STACK_SIZE 2048
/** The number of elements that the message queue can hold */
#define MESSAGE_QUEUE_DEPTH 10
/** The size of the datastore in bytes */
#define DATASTORE_SIZE FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES
/** All resources required to wake up from sensor pin */
#define SENSOR_PIN_RESOURCES 3, kResource_Hfdsm, kResource_WakeupPinInt0, kResource_WakeupPinInt2
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
/** The datastore in flash */
extern uint32_t Image$$NVM_region_datastore$$ZI$$Base[];
#define DATASTORE_ADDRESS (const uint8_t *)(Image$$NVM_region_datastore$$ZI$$Base)
#elif defined(__GNUC__)
extern uint32_t DATASTORE_START_ADDRESS[];
#define DATASTORE_ADDRESS (const uint8_t *)(DATASTORE_START_ADDRESS)
#else
#endif /* Compiler choice. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void HealthTask(void *argument);
static void IdleCallback(void *taskHandle, void *context);
static void TimerCallback(void *pParam);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** The different health messages */
typedef enum
{
    /** Message to deactivate the health process */
    kMSG_Deactivate = 0,
    /** Message to request the initiation of a measurement */
    kMSG_InitiateMeasurement = 1,
    /** Message to indicate that the requested idle time has been found and the task can start */
    kMSG_IdleReported = 2,
    /** Message used to indicate that the measurement is done and the sample is available */
    kMSG_MeasurementReady = 3
} health_message_type_t;
/** Health message structure */
typedef struct
{
    /** The message type */
    health_message_type_t msgType;
} health_message_t;
/** Handle of the health task */
static OSA_TASK_HANDLE_DEFINE(s_healthTaskHandle);
/** The health task */
static OSA_TASK_DEFINE(HealthTask, HEALTH_TASK_PRIO, 1, HEALTH_TASK_STACK_SIZE, 0);
/** Handle of the queue that contains the messages for the health task */
static OSA_MSGQ_HANDLE_DEFINE(s_messageQueue, MESSAGE_QUEUE_DEPTH, sizeof(void *));
/** Is true when the health process must be deactivated */
static bool s_deactivate = false;
/** Idle task handle */
static void *s_idleTaskHandle;
/** Measurement timer handle */
static TIMER_MANAGER_HANDLE_DEFINE(s_measurementTimerHandle);
/** Health service initialized */
static bool s_initialized = false;
/** True when a measurement is ongoing */
static bool s_measurementOngoing = false;
/** The requested idle time */
static uint32_t s_idleTime;
/** The callback to call when idle time is found */
static connectivity_idle_task_t s_idleTask;
/** The context part of the request */
static void *s_idleContext;
/** Watchdog user handle */
static safety_watchdog_handle_t s_watchdogHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * Processes the health messages.
 *
 * @param msg The message to handle
 */
static void HealthMsgHandler(health_message_t *msg)
{
    switch (msg->msgType)
    {
        case kMSG_Deactivate:
            PRINTF("Health task deactivated\n");
            s_deactivate = true;
            break;
        case kMSG_InitiateMeasurement:
            PRINTF("Measurement initiated\n");
            /* Check whether the previous measurement has finished. If not, something serious went wrong */
            assert_equal(s_measurementOngoing, false);
            s_measurementOngoing = true;
            MEASUREMENT_InitiateMeasurement();
            break;
        case kMSG_IdleReported:
        {
            uint32_t sr;
            /* Enter critical section to avoid interrupts messing up the timing */
            OSA_EnterCritical(&sr);
            /* Does the measurement fit in the remainder of the radio idle time ? */
            if (CONNECTIVITY_GetRadioIdleTime() < s_idleTime)
            {
                /* No, reschedule the measurement process */
                CONNECTIVITY_RescheduleIdleTask(s_idleTaskHandle);
            }
            else
            {
#ifdef ENABLE_LOW_POWER
                /* Set all resources required to wake up from the sensor interrupt pin */
                PM_SetRequiredResources(SENSOR_PIN_RESOURCES);
#endif /* ENABLE_LOW_POWER */
                void *taskHandle = s_idleTaskHandle;
                s_idleTask(s_idleTaskHandle, s_idleContext);
                CONNECTIVITY_FinishIdleTask(taskHandle);
            }
            OSA_ExitCritical(sr);
            break;
        }
        case kMSG_MeasurementReady:
        {
#ifdef ENABLE_LOW_POWER
            PM_RemoveRequiredResources(SENSOR_PIN_RESOURCES);
#endif /* ENABLE_LOW_POWER */
            s_idleTaskHandle = NULL;
            status_t status  = MEASUREMENT_Process();
            (void)status;
            assert_equal(status, kStatus_Success);
            s_measurementOngoing = false;
            (void)s_measurementOngoing;
            SAFETY_WatchdogFeed(s_watchdogHandle);
            break;
        }
    }
}

static void MeasurementDone(void)
{
    health_message_t *msg = MSGQ_CreateMsg((msgq_handler_t)HealthMsgHandler, sizeof(health_message_t));
    assert(msg);
    msg->msgType = kMSG_MeasurementReady;
    MSGQ_Put(s_messageQueue, msg);
}

static void ScheduleIdleTask(uint32_t idleTime, connectivity_idle_task_t task, void *context)
{
    /* Only one health idle task at a time is supported */
    assert_equal(s_idleTaskHandle, NULL);
    s_idleTime       = idleTime;
    s_idleTask       = task;
    s_idleContext    = context;
    s_idleTaskHandle = CONNECTIVITY_ScheduleIdleTask(idleTime, IdleCallback, NULL);
}

/**
 * Function called when the timer expires.
 * The measurement is initiated by this function.
 *
 * @param param Not used
 */
static void TimerCallback(void *pParam)
{
    health_message_t *msg = MSGQ_CreateMsg((msgq_handler_t)HealthMsgHandler, sizeof(health_message_t));
    assert(msg);
    msg->msgType = kMSG_InitiateMeasurement;
    MSGQ_Put(s_messageQueue, msg);
}

/**
 * Function called when the radio is idle.
 * This function hands over the processing to the health process
 *
 * @param context Not used
 */
static void IdleCallback(void *taskHandle, void *context)
{
    health_message_t *msg = MSGQ_CreateMsg((msgq_handler_t)HealthMsgHandler, sizeof(health_message_t));
    assert(msg);
    msg->msgType = kMSG_IdleReported;
    MSGQ_Put(s_messageQueue, msg);
}

/**
 * The health task. Upon startup, this task will initiate the system before
 * it will repeatedly wait for a message and process it.
 * When the health process is deactivated, the system is deinitialized.
 *
 * @param argument
 */
static void HealthTask(void *argument)
{
    timer_status_t timerStatus;
    (void)timerStatus;
    if (!s_initialized)
    {
        /* Initialize the datastore */
        DATASTORE_Init(DATASTORE_ADDRESS, DATASTORE_SIZE, sizeof(uint16_t));

        /* Create a timer for the measurements */
        timerStatus = TM_Open(s_measurementTimerHandle);
        assert_equal(timerStatus, kStatus_TimerSuccess);
        timerStatus = TM_InstallCallback((timer_handle_t)s_measurementTimerHandle, TimerCallback, NULL);
        assert_equal(timerStatus, kStatus_TimerSuccess);
        timerStatus =
            TM_Start((timer_handle_t)s_measurementTimerHandle, kTimerModeLowPowerTimer | kTimerModeIntervalTimer,
                     MEASUREMENT_GetPreferredIntervalInMs());
        assert_equal(timerStatus, kStatus_TimerSuccess);
        MEASUREMENT_Init(MeasurementDone, ScheduleIdleTask);

        s_initialized = true;
    }

    while (!s_deactivate)
    {
        void *message = MSGQ_Get(s_messageQueue, osaWaitForever_c);
        if (message == NULL)
        {
            break;
        }
        MSGQ_GetHandler(message)(message);
        MSGQ_FreeMsg(message);
    }

    if (s_deactivate)
    {
        /* Stop and destroy the timer again */
        timerStatus = TM_Stop(s_measurementTimerHandle);
        assert_equal(timerStatus, kStatus_TimerSuccess);
        timerStatus = TM_Close(s_measurementTimerHandle);
        assert_equal(timerStatus, kStatus_TimerSuccess);
        /* Destroy the queue */
        osa_status_t osaStatus = MSGQ_DestroyQueue(s_messageQueue);
        (void)osaStatus;
        assert_equal(osaStatus, KOSA_StatusSuccess);
        MEASUREMENT_DeInit();
    }
}

/**
 * Initializes the health process.
 */
void HEALTH_Init(void)
{
    /* Create the message queue */
    MSGQ_CreateQueue(s_messageQueue, MESSAGE_QUEUE_DEPTH, sizeof(void *));
    s_watchdogHandle = SAFETY_WatchdogRegisterUser();
}

/**
 * Activates the health task/process.
 */
void HEALTH_Activate(void)
{
    s_deactivate           = false;
    osa_status_t osaStatus = OSA_TaskCreate((osa_task_handle_t)s_healthTaskHandle, OSA_TASK(HealthTask), NULL);
    (void)osaStatus;
    assert_equal(osaStatus, KOSA_StatusSuccess);
}

/**
 * Deactivates the health process and stops the task
 */
void HEALTH_Deactivate(void)
{
    health_message_t *msg = MSGQ_CreateMsg((msgq_handler_t)HealthMsgHandler, sizeof(health_message_t));
    assert(msg);
    msg->msgType = kMSG_Deactivate;
    MSGQ_Put(s_messageQueue, msg);
}

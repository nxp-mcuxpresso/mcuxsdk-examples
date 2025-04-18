/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* This component implements the connectivity task. The connectivity task is
 * responsible for managing the BLE connections.
 *
 * # Idle tasks
 * Idle tasks are put on a queue, first come, first serve. When the first task
 * is added, the radio idle time is checked. When the idle gap is large enough
 * for the task, the task is immediately called. When the gap is too small,
 * a timer is started that is planned to expire after the radio activity and
 * at the beginning of the next radio idle gap. It is however, impossible to
 * predict how long the radio will be active. If the timer expires before the
 * radio activity is done, a new timer is started to expire shortly after. This
 * is repeated until the radio reports idle.
 * When a task is reported as done, the next task is taken from the queue and
 * the process starts over.
 */

#include "fsl_os_abstraction.h"
#include "fsl_component_mem_manager.h"
#include "blemgr.h"
#include "msgq.h"
#include "connectivity.h"
#include "safety.h"
#include "fsl_debug_console.h"
#include "ble_controller.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** The priority of the connectivity task */
#define CONN_TASK_PRIO 1
/** The stack size of the connectivity task */
#define CONN_TASK_STACK_SIZE 1300
/** The number of elements that the message queue can hold */
#define MESSAGE_QUEUE_DEPTH 10
/** The number of idle tasks that can be scheduled */
#define IDLE_TASK_QUEUE_DEPTH 10

/** The different health messages */
typedef enum
{
    /** Message to update the temperature */
    kMSG_SetTemperature = 0,
    /** Message to update the battery level */
    kMSG_SetBatteryLevel = 1,
} connectivity_message_type_t;

/** Information about the scheduled idle task */
typedef struct
{
    /** The length of the idle task in us */
    uint32_t idleTime;
    /** The function to be called when an idle gap of idleTime is found */
    connectivity_idle_task_t task;
    /** The provided context */
    void *context;
} connectivity_idle_task_info_t;

/** Connectivity message structure */
typedef struct
{
    /** The message type */
    connectivity_message_type_t msgType;
    union
    {
        /* Only valid for kMSG_SetTemperature. Holds the new temperature */
        int16_t temperature;
        /* Only valid for kMSG_SetBatteryLevel. Holds the new battery level */
        uint8_t batteryLevel;
    } data;
} connectivity_message_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ConnectivityTask(void *argument);
static void CheckIdleTask(void *);
static void StartIdleTaskScheduler(void *);
static void RadioIdleCallback();

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** Handle of the connectivity task */
static OSA_TASK_HANDLE_DEFINE(s_connectivityTaskHandle);
/** The connectivity task */
static OSA_TASK_DEFINE(ConnectivityTask, CONN_TASK_PRIO, 1, CONN_TASK_STACK_SIZE, 0);
/** Handle of the queue that contains the messages for the connectivity task */
static OSA_MSGQ_HANDLE_DEFINE(s_messageQueue, MESSAGE_QUEUE_DEPTH, sizeof(void *));
/** Handle of the queue that contains the scheduled idle tasks */
static OSA_MSGQ_HANDLE_DEFINE(s_idleTaskQueue, IDLE_TASK_QUEUE_DEPTH, sizeof(connectivity_idle_task_info_t *));
/** The currently active idle tasks */
static connectivity_idle_task_info_t *s_currentIdleTask;
/** True when the connectivity process is activated */
static bool s_isActive = false;
/** Handle to watchdog user */
static safety_watchdog_handle_t s_watchdogUser;
/** Pointer to the original link layer isr */
static void (*s_llIsr)(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * Intercepted link layer interrupt used as heartbeat for the link layer.
 * Signal used to feed the watchdog */
static void LinklayerIsr(void)
{
    s_llIsr();
    SAFETY_WatchdogFeed(s_watchdogUser);
}

/**
 * Processes the connectivity messages.
 *
 * @param msg The message to handle
 */
static void ConnectivityMsgHandler(connectivity_message_t *msg)
{
    switch (msg->msgType)
    {
        case kMSG_SetTemperature:
            BLEMGR_SetTemperature(msg->data.temperature);
            break;
        case kMSG_SetBatteryLevel:
            BLEMGR_SetBatteryLevel(msg->data.batteryLevel);
            break;
    }
}

/**
 * The connectivity task. Upon startup, this task will initiate the radio system
 * before it will repeatedly wait for a message and process it.
 * When the connectivity process is deactivated, the system is deinitialized.
 *
 * @param argument
 */
static void ConnectivityTask(void *argument)
{
    while (true)
    {
        void *message = MSGQ_Get(s_messageQueue, osaWaitForever_c);
        if (message == NULL)
        {
            break;
        }
        MSGQ_GetHandler(message)(message);
        MSGQ_FreeMsg(message);
    }
}

/**
 * Callback function for the idle task timer
 */
static void RadioIdleCallback()
{
    /* This callback was registered because the previous idle time was
     * not large enough. Disable the callback again and check whether
     * this idle gap is large enough. If not, the callback will be
     * reinstalled */
    BLEController_RegisterIdleCallback(NULL);
    CheckIdleTask(NULL);
}

/**
 * Checks whether the current idle task can be executed, i.e.
 * is the current idle gap large enough to fit the idle task.
 *
 * @param unused Not used
 */
static void CheckIdleTask(void *unused)
{
    uint32_t sr;
    uint32_t idleTime;

    /* Enter critical section to avoid interrupts messing up the timing */
    OSA_EnterCritical(&sr);

    idleTime = CONNECTIVITY_GetRadioIdleTime();
    /* Check whether the task fits in current idle time */
    if (idleTime > s_currentIdleTask->idleTime)
    {
        /* Yes, it fits, execute the task */
        OSA_ExitCritical(sr);
        s_currentIdleTask->task(s_currentIdleTask, s_currentIdleTask->context);
        /* Note that after this function returns, the task may continue asynchronously.
         * We cannot start the next idle task yet until CONNECTIVITY_FinishIdleTask
         * is called */
    }
    else
    {
        /* No, wait till the next idle gap */
        /* Since interrupts are disabled, there's no risk of a race condition in which
         * the radio becomes idle between the call to CONNECTIVITY_GetRadioIdleTime and the
         * moment the callback is registered below */
        BLEController_RegisterIdleCallback(RadioIdleCallback);
        OSA_ExitCritical(sr);
    }
}

/**
 * This will start the idle task scheduler if it's not already active.
 *
 * @param p Not used
 */
static void StartIdleTaskScheduler(void *p)
{
    /* Is the scheduler currently idle? */
    if (s_currentIdleTask == NULL)
    {
        /* Is there a task on the queue? */
        if (OSA_MsgQGet(s_idleTaskQueue, &s_currentIdleTask, 0) == KOSA_StatusSuccess)
        {
            /* Yes, check whether it can be executed right away or needs to be scheduled */
            CheckIdleTask(NULL);
        }
    }
}

static void BleMgrCallback(blemgr_event_t event)
{
    switch (event)
    {
        case kBLEMGR_Event_AdvertisingStarted:
            SAFETY_RecordRadioState(kSAFETY_RadioState_Advertising);
            break;
        case kBLEMGR_Event_Connected:
            SAFETY_RecordRadioState(kSAFETY_RadioState_Connected);
            break;
        default:
            break;
    }
}

/**
 * Initializes the connectivity process.
 */
void CONNECTIVITY_Init(void)
{
    MSGQ_CreateQueue(s_messageQueue, MESSAGE_QUEUE_DEPTH, sizeof(void *));
    OSA_MsgQCreate(s_idleTaskQueue, IDLE_TASK_QUEUE_DEPTH, sizeof(connectivity_idle_task_info_t *));

    osa_status_t status;
    (void)status;
    status = OSA_TaskCreate((osa_task_handle_t)s_connectivityTaskHandle, OSA_TASK(ConnectivityTask), NULL);
    assert_equal(status, KOSA_StatusSuccess);
}

/**
 * Starts the BLE connectivity stack
 */
void CONNECTIVITY_StartBle(void)
{
    status_t status;
    (void)status;
    status = BLEMGR_Init(s_messageQueue, BleMgrCallback);
    assert_equal(status, kStatus_Success);
    s_isActive     = true;
    s_watchdogUser = SAFETY_WatchdogRegisterUser();
    s_llIsr        = (void (*)(void))NVIC_GetVector(BLE_LL_IRQn);
    NVIC_SetVector(BLE_LL_IRQn, (uint32_t)LinklayerIsr);
}

/**
 * Sets the temperature
 *
 * @param temperature The new temperature expressed in hundredths of a degree
 */
void CONNECTIVITY_SetTemperature(int16_t temperature)
{
    connectivity_message_t *msg =
        MSGQ_CreateMsg((msgq_handler_t)ConnectivityMsgHandler, sizeof(connectivity_message_t));

    assert(msg);
    msg->msgType          = kMSG_SetTemperature;
    msg->data.temperature = temperature;
    MSGQ_Put(s_messageQueue, msg);
}

/**
 * Sets the battery level
 *
 * @param level The new battery level
 */
void CONNECTIVITY_SetBatteryLevel(uint8_t level)
{
    connectivity_message_t *msg =
        MSGQ_CreateMsg((msgq_handler_t)ConnectivityMsgHandler, sizeof(connectivity_message_t));
    assert(msg);
    msg->msgType           = kMSG_SetBatteryLevel;
    msg->data.batteryLevel = level;
    MSGQ_Put(s_messageQueue, msg);
}

/**
 * Returns the time till next radio activity in us or 0 when busy
 *
 * @return the radio idle time
 */
uint32_t CONNECTIVITY_GetRadioIdleTime()
{
    uint32_t remainingTime;

    /* Is the radio active? */
    if (s_isActive)
    {
        /* Yes, query the LL for the idle time */
        blec_result_t status = BLEController_GetRemainingTimeForNextEvent(&remainingTime);
        if (status != kBLEC_Success)
        {
            remainingTime = 0;
        }
    }
    else
    {
        /* No, the radio is idle forever */
        remainingTime = 0xffffffff;
    }
    return remainingTime;
}

/**
 * Schedules an idle task. The registered function will be called when the radio
 * is idle for at least idleTime.
 *
 * @param idleTime The minimum idle gap length in us
 * @param task The function to be called when the radio is idle for at least * idleTime
 * @param context Optional context pointer
 * @return Handle to the idle task
 */
void *CONNECTIVITY_ScheduleIdleTask(uint32_t idleTime, connectivity_idle_task_t task, void *context)
{
    connectivity_idle_task_info_t *idleInfo = OSA_MemoryAllocate(sizeof(connectivity_idle_task_info_t));
    assert(idleInfo != NULL);
    idleInfo->idleTime = idleTime;
    idleInfo->task     = task;
    idleInfo->context  = context;
    OSA_MsgQPut(s_idleTaskQueue, &idleInfo);

    /* Yes, queue the scheduler task to schedule this task */
    void *msg = MSGQ_CreateMsg(StartIdleTaskScheduler, 0);
    assert(msg);
    MSGQ_Put(s_messageQueue, msg);

    return idleInfo;
}

/**
 * Function to request the idle task to be scheduled again. This function must
 * be used when the idle gap is no longer large enough at the time the task was
 * about to execute. This can happen due to OS overhead and interrupts.
 *
 * @param Handle to the idle task that needs to be rescheduled
 */
void CONNECTIVITY_RescheduleIdleTask(void *idleTaskHandle)
{
    assert_equal(idleTaskHandle, s_currentIdleTask);
    void *msg = MSGQ_CreateMsg(CheckIdleTask, 0);
    assert(msg);
    MSGQ_Put(s_messageQueue, msg);
}

/**
 * Called when the idle task is finished. This function will delete the idle
 * task and schedule the next queued idle task
 *
 * @param Handle to the idle task that has finished
 */
void CONNECTIVITY_FinishIdleTask(void *idleTaskHandle)
{
    assert_equal(idleTaskHandle, s_currentIdleTask);
    s_currentIdleTask = NULL;
    OSA_MemoryFree(idleTaskHandle);
    void *msg = MSGQ_CreateMsg(StartIdleTaskScheduler, 0);
    assert(msg);
    MSGQ_Put(s_messageQueue, msg);
}

bool CONNECTIVITY_IsActive(void)
{
    return s_isActive;
}

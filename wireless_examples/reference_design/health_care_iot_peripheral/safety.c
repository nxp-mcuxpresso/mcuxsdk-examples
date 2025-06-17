/*
 * Copyright 2022-2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifdef BLE_POWER_PROFILING
/* No memory profiling during BLE power profiling */
#undef MEM_PROFILING
#endif /* BLE_POWER_PROFILING */

#include "fsl_os_abstraction.h"
#include "fsl_component_timer_manager.h"
#include "fsl_component_mem_manager.h"
#include "msgq.h"
#include "health.h"
#include "connectivity.h"
#include "board.h"
#include "safety.h"
#include "eventstore.h"
#include "fsl_iap.h"
#include "fsl_power.h"
#ifdef ENABLE_LOW_POWER
#include "fsl_pm_core.h"
#include "fsl_pm_device.h"
#include "fsl_pm_app.h"
#endif
#include "ble_controller.h"
#include "fwk_platform.h"
#ifdef CTIMER_ADAPTER
#include "fsl_ctimer.h"
#else
#include "fsl_ostimer.h"
#endif /* CTIMER_ADAPTER */
#if gAppUseNvm_d
#include "NVM_Interface.h"
#endif /* gAppUseNvm_d */
#include "fsl_usart.h"
#include "fsl_i2c.h"
#ifdef ENABLE_LOW_POWER
#include "fsl_pm_core.h"
#endif
#include "pin_mux.h"

#ifndef BLE_POWER_PROFILING
#include "fsl_wwdt.h"
#endif /* BLE_POWER_PROFILING */
#include "hciot_common.h"
#ifdef MEM_PROFILING
#ifdef SDK_OS_FREE_RTOS
#include "fsl_os_abstraction_mem_profiling_free_rtos.h"
#else
#include "fsl_os_abstraction_mem_profiling_bm.h"
#endif /* SDK_OS_FREE_RTOS */
#endif /* MEM_PROFILING */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef MEM_PROFILING
#define MEM_PROFILING_INTERVAL_MS 10000
#endif /* MEM_PROFILING */

/** The priority of the safety task */
#define SAFETY_TASK_PRIO 7
/** The stack size of the safety task */
#define SAFETY_TASK_STACK_SIZE 1248
/** The number of elements that the message queue can hold */
#define MESSAGE_QUEUE_DEPTH 10
/** The size of the event store. Must be equal to 2 sectors */
#define EVENTSTORE_SIZE (2 * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES)
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
/** The datastore in flash */
extern uint32_t Image$$NVM_region_eventstore$$ZI$$Base[];
#define EVENTSTORE_ADDRESS (const uint8_t *)(Image$$NVM_region_eventstore$$ZI$$Base)
#elif defined(__GNUC__) || defined(__ICCARM__)
extern uint32_t EVENTSTORE_START_ADDRESS[];
#define EVENTSTORE_ADDRESS (const uint8_t *)(EVENTSTORE_START_ADDRESS)
#endif /* Compiler choice. */
/** Erase time in us */
#define ERASE_TIME 20000
/** Flash write time in us */
#define WRITE_TIME 1000

#ifdef BLE_POWER_PROFILING
#define ADDITIONAL_ACTIVE_RESOURCES
#define ADDITIONAL_ACTIVE_RESOURCES_COUNT 0
#else
/** Additional resources when BLE_POWER_PROFILING is not enabled */
#define ADDITIONAL_ACTIVE_RESOURCES \
    , kResource_Bod1, kResource_Bod2, kResource_WakeupBod1, kResource_Fro1m, kResource_WakeupWdt
#define ADDITIONAL_ACTIVE_RESOURCES_COUNT 5
#endif /* BLE_POWER_PROFILING */

/** The required resources in shelf mode */
#define RESOURCES_SHELFMODE 1, kResource_WakeupWakePad
/* Default resources during active, independent of supply mode */
#define DEFAULT_RESOURCES_ACTIVE \
    kResource_Xtal32k, kResource_BleWup, kResource_WakeupOsEvent ADDITIONAL_ACTIVE_RESOURCES
#define DEFAULT_RESOURCES_ACTIVE_COUNT (3 + ADDITIONAL_ACTIVE_RESOURCES_COUNT)
/** The required resources in active mode */
#ifdef SUPPLY_MODE_DCDCBYPASS
#define RESOURCES_ACTIVE (1 + DEFAULT_RESOURCES_ACTIVE_COUNT), DEFAULT_RESOURCES_ACTIVE, kResource_DcdcBypass
#else
#define RESOURCES_ACTIVE      (1 + DEFAULT_RESOURCES_ACTIVE_COUNT), DEFAULT_RESOURCES_ACTIVE, kResource_Dcdc
#define RESOURCES_ACTIVE_XRSM DEFAULT_RESOURCES_ACTIVE_COUNT, DEFAULT_RESOURCES_ACTIVE
#endif /* SUPPLY_MODE_DCDCBYPASS */
/** The maximum sleep time */
#define MAX_SLEEP_TIME 0x7fffffffu

/** Structure containing flash write info */
typedef struct
{
    /** The address to write to */
    uint32_t address;
    /** Pointer to the data that needs to be written */
    uint8_t *data;
    /** The length in bytes of the data */
    uint32_t length;
    /** Erase before write */
    bool eraseFirst;
    /** The function called when the data is written */
    safety_written_cb_t writtenCb;
    /** The callback context */
    void *context;
} flash_info_t;

/** Structure containing write flash command info */
typedef struct
{
    flash_info_t *flashInfo;
    void *taskHandle;
} write_flash_info_t;

/** The different safety messages */
typedef enum
{
    /** Message to write data to flash */
    kMSG_WriteFlash       = 0,
    kMSG_RecordRadioState = 1
} safety_message_type_t;

/** Safety message structure */
typedef struct
{
    /** The message type */
    safety_message_type_t msgType;
    union
    {
        /** Only valid for kMSG_WriteFlash. Holds the information of the data to be written */
        write_flash_info_t writeFlashInfo;
        /** Only valid for kMSG_RecordRadioState. Holds the state to record */
        safety_radio_state_t radioState;
    } data;
} safety_message_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#ifdef MEM_PROFILING
static void ProfilingTimerCallback(void *pParam);
#endif /* MEM_PROFILING */

static void SafetyTask(void *argument);
static void FlashEraseWriteTask(void *taskHandle, void *context);
void SysTick_Handler(void);
#ifdef ENABLE_LOW_POWER
static status_t PrepareForLowPower(pm_event_type_t eventType, pm_mode_t mode, void *data);
#endif /* ENABLE_LOW_POWER */
/*******************************************************************************
 * Variables
 ******************************************************************************/

#ifdef MEM_PROFILING
/* Memory profiling timer handle. */
static TIMER_MANAGER_HANDLE_DEFINE(s_memProfilingTimerHandle);
#endif /* MEM_PROFILING */
/** Handle of the safety task */
static OSA_TASK_HANDLE_DEFINE(s_safetyTaskHandle);
/** The safety task */
static OSA_TASK_DEFINE(SafetyTask, SAFETY_TASK_PRIO, 1, SAFETY_TASK_STACK_SIZE, 0);
/** Handle of the queue that contains the messages for the health task */
static OSA_MSGQ_HANDLE_DEFINE(s_messageQueue, MESSAGE_QUEUE_DEPTH, sizeof(void *));
/** The flash config */
static flash_config_t s_flashConfig;
/** Device state */
static safety_device_state_t s_deviceState;
/** Holds the original value of the systick load register */
static uint32_t s_systickLoad;
/** Indicates whether the safety task has already been initialized */
static bool s_initialized = false;
#ifdef ENABLE_LOW_POWER
/** Holds power manager notification information */
static pm_notify_element_t s_pmNotifyInfo = {.notifyCallback = PrepareForLowPower, .data = NULL};
#endif
/** Number of watchdog users */
static uint32_t s_watchdogUserCount = 0;
#ifndef BLE_POWER_PROFILING
/** Contains a bitmask of the users that fed the watchdog */
static uint32_t s_watchdogUserFeeds = 0;
/* BOD interrupt checking timer handle */
static TIMER_MANAGER_HANDLE_DEFINE(s_bodIrqCheckerTimerHandle);
#endif /* BLE_POWER_PROFILING */

/** Holds used supply mode */
static dcdc_mode_t s_supplyMode;

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifndef BLE_POWER_PROFILING
/**
 * Enable low battery detection.
 * When a voltage level below the configured threshold is detected
 * by the BOD1, the ISR will be called.
 * When a voltage level below the configured threshold is detected
 * by the BOD2, the device will reset.
 */
static void EnableLowBatteryDetection()
{
    /* Configure BOD1 to generate interrupt when battery level becomes too low */
    POWER_ClearBOD1Interrupt();

    PRINTF("Low battery warning enabled at ");
    if (s_supplyMode == kDCDC_MODE_LV_SM)
    {
        POWER_SetBod1Level(kBOD_LEVEL_1200mv);
        PRINTF("@1.2V\n");
    }
    else if (s_supplyMode == kDCDC_MODE_XR_SM_SS || s_supplyMode == kDCDC_MODE_XR_SM_DS)
    {
        POWER_SetBod1Level(kBOD_LEVEL_1700mv);
        PRINTF("@1.7V\n");
    }
    else if (s_supplyMode == kDCDC_MODE_HV_SM)
    {
        POWER_SetBod1Level(kBOD_LEVEL_2200mv);
        PRINTF("@2.2V\n");
    }
    else
    {
        assert(false);
    }

    POWER_ConfigureBOD1(kBOD_INTERRUPT);
    NVIC_EnableIRQ(BOD1_IRQn);

    /* Configure BOD2 to reset device when battery level becomes critically low (minimum voltage for flash to work
     * reliably is 1.55V, after DC/DC)*/
    if (s_supplyMode == kDCDC_MODE_LV_SM)
    {
        POWER_SetBod2Level(kBOD_LEVEL_1125mv);
        POWER_ConfigureBOD2(kBOD_RESET);
        PRINTF("Low battery reset enabled at @1.125V\n");
    }
    else
    {
        POWER_SetBod2Level(kBOD_LEVEL_1550mv);
        POWER_ConfigureBOD2(kBOD_RESET);
        PRINTF("Low battery reset enabled at @1.55V\n");
    }
}

static void BodIrqCheckerTimerCallback(void *pParam)
{
    /* The BOD interrupt is level triggered. To prevent a continuous train of interrupts,
    the interrupt is disabled after the first event. This function is repetitively called
    by a timer until the level is again above the threshold and before the interrupt is enabled
    again to detect the next dip. */
    if (!POWER_IsBOD1Active())
    {
        timer_status_t timerStatus;
        (void)timerStatus;

        timerStatus = TM_Stop(s_bodIrqCheckerTimerHandle);
        assert_equal(timerStatus, kStatus_TimerSuccess);
        timerStatus = TM_Close(s_bodIrqCheckerTimerHandle);
        assert_equal(timerStatus, kStatus_TimerSuccess);

        POWER_ClearBOD1Interrupt();
        POWER_ConfigureBOD1(kBOD_INTERRUPT);
        PRINTF("Battery level above warning threshold again.\n");
    }
}

/**
 * BOD1 interrupt service routine. Used to detect low battery
 */
void BOD1_IRQHandler(void)
{
    timer_status_t timerStatus;
    (void)timerStatus;

    /* Disable the BOD */
    POWER_ConfigureBOD1(kBOD_NO_ACTION);

    /* Clear the interrupt in the BOD status registers. */
    POWER_ClearBOD1Interrupt();

    PRINTF("Low battery detected!\n");

    /* Start recurring timer every ms to check if voltage level is above the threshold again, when this happens, BOD
     * interrupt will be re-enabled.*/
    timerStatus = TM_Open(s_bodIrqCheckerTimerHandle);
    assert_equal(timerStatus, kStatus_TimerSuccess);
    timerStatus = TM_InstallCallback((timer_handle_t)s_bodIrqCheckerTimerHandle, BodIrqCheckerTimerCallback, NULL);
    assert_equal(timerStatus, kStatus_TimerSuccess);
    timerStatus =
        TM_Start((timer_handle_t)s_bodIrqCheckerTimerHandle, kTimerModeLowPowerTimer | kTimerModeIntervalTimer, 1);
    assert_equal(timerStatus, kStatus_TimerSuccess);
}
#endif /* BLE_POWER_PROFILING */

#ifdef ENABLE_LOW_POWER
static status_t PrepareForLowPower(pm_event_type_t eventType, pm_mode_t mode, void *data)
{
#ifndef DISABLE_POWER_DOWN_CPU
    if (eventType == kPM_EventEnteringSleep)
    {
        if (mode == kPM_Mode_PowerDownWithCpuRetention)
        {
            status_t status = DbgConsole_EnterLowpower();
            (void)status;
            assert_equal(status, kStatus_Success);
        }
    }
    else if (eventType == kPM_EventExitingSleep)
    {
        if (mode == kPM_Mode_PowerDownWithCpuRetention)
        {
            status_t status = DbgConsole_ExitLowpower();
            (void)status;
            assert_equal(status, kStatus_Success);
        }
    }
#endif /* DISABLE_POWER_DOWN_CPU */
    return kStatus_Success;
}
#endif

#ifdef ENABLE_LOW_POWER
/**
 * Enter low power mode for the minimum duration between radio idle time and the expected idle time.
 *
 * @param expectedIdleTimeUs The expected idle time by the system.
 */
void EnterLowPower(uint32_t expectedIdleTimeUs)
{
    uint32_t radioRemainingTimeUs = MAX_SLEEP_TIME;

    if (CONNECTIVITY_IsActive())
    {
        blec_result_t status = BLEController_GetRemainingTimeForNextEventUnsafe(&radioRemainingTimeUs);
        (void)status;
        assert_equal(status, kBLEC_Success);
    }

    status_t ret = PMAPP_EnterLowPower(MIN(MAX_SLEEP_TIME, MIN(expectedIdleTimeUs, radioRemainingTimeUs)));
    (void)ret;
    assert_equal(ret, kStatus_Success);
}
#endif /*ENABLE_LOW_POWER */

#if (gAppUseNvm_d)
void vApplicationIdleHook(void)
{
    NvIdle();
}
#endif /* gAppUseNvm_d */

/**
 * Processes the health messages.
 *
 * @param msg The message to handle
 */
static void SafetyMsgHandler(safety_message_t *msg)
{
    switch (msg->msgType)
    {
        case kMSG_WriteFlash:
        {
            uint32_t sr;
            OSA_EnterCritical(&sr);
            /* Check whether the idle gap is large enough */
            if (CONNECTIVITY_GetRadioIdleTime() >=
                (msg->data.writeFlashInfo.flashInfo->eraseFirst ? ERASE_TIME : WRITE_TIME))
            {
                status_t status;
                (void)status;

                /* Do we need to erase first ? */
                if (msg->data.writeFlashInfo.flashInfo->eraseFirst)
                {
                    /* Align the address to the sector boundaries */
                    uint32_t address =
                        msg->data.writeFlashInfo.flashInfo->address & ~(FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES - 1);
                    /* The region to erase starts from the aligned address to the end of the data length */
                    uint32_t length = msg->data.writeFlashInfo.flashInfo->length +
                                      (msg->data.writeFlashInfo.flashInfo->address - address);
                    /* Round to sector boundary */
                    length = (((uint32_t)((length - 1) / FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES)) + 1) *
                             FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES;

                    status = FLASH_Erase(&s_flashConfig, address, length, kFLASH_ApiEraseKey);
                    assert_equal(status, kStatus_Success);
                    OSA_ExitCritical(sr);

                    /* Erase is done, now schedule an idle task for write */
                    msg->data.writeFlashInfo.flashInfo->eraseFirst = false;
                    CONNECTIVITY_ScheduleIdleTask(WRITE_TIME, FlashEraseWriteTask, msg->data.writeFlashInfo.flashInfo);
                    PRINTF("Flash erase done\n");
                }
                else
                {
                    /* Program the page */
                    status = FLASH_Program(&s_flashConfig, msg->data.writeFlashInfo.flashInfo->address,
                                           msg->data.writeFlashInfo.flashInfo->data,
                                           msg->data.writeFlashInfo.flashInfo->length);
                    assert_equal(status, kStatus_Success);
                    OSA_ExitCritical(sr);
                    /* Inform the caller */
                    if (msg->data.writeFlashInfo.flashInfo->writtenCb != NULL)
                    {
                        msg->data.writeFlashInfo.flashInfo->writtenCb(msg->data.writeFlashInfo.flashInfo->data,
                                                                      msg->data.writeFlashInfo.flashInfo->length,
                                                                      msg->data.writeFlashInfo.flashInfo->context);
                    }
                    MEM_BufferFree(msg->data.writeFlashInfo.flashInfo);
                    PRINTF("Flash write done\n");
                }
                /* Idle task is finished */
                CONNECTIVITY_FinishIdleTask(msg->data.writeFlashInfo.taskHandle);
            }
            else
            {
                OSA_ExitCritical(sr);
                /* The gap no longer fits, ask to reschedule the task again */
                CONNECTIVITY_RescheduleIdleTask(msg->data.writeFlashInfo.taskHandle);
            }
            break;
        }
        case kMSG_RecordRadioState:
            EVENTSTORE_RecordRadioState(msg->data.radioState);
            break;
    }
}

/**
 * Function called when there's an idle gap in the radio activity.
 *
 * This function is called from the connectivity task. A message is crated to
 * offload the actual flash activities to the safety task.
 *
 * @param taskHandle Handle to the idle task
 * @param context The flash info
 */
static void FlashEraseWriteTask(void *taskHandle, void *context)
{
    flash_info_t *flashInfo = (flash_info_t *)context;
    safety_message_t *msg   = MSGQ_CreateMsg((msgq_handler_t)SafetyMsgHandler, sizeof(safety_message_t));
    if (msg)
    {
        msg->msgType                        = kMSG_WriteFlash;
        msg->data.writeFlashInfo.flashInfo  = flashInfo;
        msg->data.writeFlashInfo.taskHandle = taskHandle;
        MSGQ_Put(s_messageQueue, msg);
    }
}

/**
 * Switches the system to the requested device state
 */
static void ChangeDeviceState(safety_device_state_t state)
{
    PRINTF("Entering device state: ");
    switch (state)
    {
        case kSAFETY_DeviceState_Shelfmode:
            PRINTF("shelf mode. Press wakeup button to continue.");
#ifdef ENABLE_LOW_POWER
            PM_SetRequiredResources(RESOURCES_SHELFMODE);
#endif /* ENABLE_LOW_POWER */
            break;
        case kSAFETY_DeviceState_Active:
            PRINTF("active\n");
#ifndef BLE_POWER_PROFILING
            EnableLowBatteryDetection();
#endif /* BLE_POWER_PROFILING */
#ifdef ENABLE_LOW_POWER
#ifndef SUPPLY_MODE_DCDCBYPASS
            if (s_supplyMode == kDCDC_MODE_XR_SM_SS || s_supplyMode == kDCDC_MODE_XR_SM_DS)
            {
                PM_SetRequiredResources(RESOURCES_ACTIVE_XRSM);
            }
            else
#endif /* SUPPLY_MODE_DCDCBYPASS */
            {
                PM_SetRequiredResources(RESOURCES_ACTIVE);
            }
#endif /* ENABLE_LOW_POWER */
            CONNECTIVITY_StartBle();
#ifdef BLE_POWER_PROFILING
            /* No health activities during BLE power profiling */
#else
            HEALTH_Activate();
#endif /* BLE_POWER_PROFILING */
            break;
        default:
            break;
    }
}

static void ChangeStateAfterFlashWrite(uint8_t *data, uint32_t length, void *context)
{
    safety_device_state_t state = (safety_device_state_t)(uint32_t)context;
    ChangeDeviceState(state);
}

/**
 * The safety task. Upon startup, this task will initiate the system before
 * it will repeatedly wait for a message and process it.
 * When the health process is deactivated, the system is deinitialized.
 *
 * @param argument
 */
static void SafetyTask(void *argument)
{
    if (!s_initialized)
    {
        timer_status_t tmState;
        status_t status;
        (void)tmState;
        (void)status;

        /* Capture the original systick reload value */
        s_systickLoad = SysTick->LOAD + 1;
#ifdef SDK_OS_FREE_RTOS
#ifdef ENABLE_LOW_POWER
        PMAPP_Init(s_systickLoad);
#endif
#endif /* SDK_OS_FREE_RTOS */

        /* Initialize timer manager */
        PLATFORM_InitTimerManager();

        CONNECTIVITY_Init();
        HEALTH_Init();

        status = EVENTSTORE_Init(EVENTSTORE_ADDRESS, EVENTSTORE_SIZE);
        assert_equal(status, kStatus_Success);

        status = FLASH_Init(&s_flashConfig);
        assert_equal(status, kStatus_Success);

        MSGQ_CreateQueue(s_messageQueue, MESSAGE_QUEUE_DEPTH, sizeof(void *));
#ifdef ENABLE_LOW_POWER
        PM_RegisterNotify(kPM_NotifyGroup2, &s_pmNotifyInfo);
#endif
        s_deviceState = EVENTSTORE_GetLatestDeviceState();
        PRINTF("Last recorded device state: %d\n", s_deviceState);
        /* Is this the first time the application is booted? */
        if (s_deviceState == kSAFETY_DeviceState_Uninitialized)
        {
#if defined(ENABLE_LOW_POWER)
            /* Yes, change the state to shelf mode and store the new state in flash */
            s_deviceState = kSAFETY_DeviceState_Shelfmode;
            EVENTSTORE_RecordDeviceState(s_deviceState, ChangeStateAfterFlashWrite, (void *)s_deviceState);
        }
        /* Is the device in shelf mode and did the system not wake up due to the reset button? */
        else if (s_deviceState == kSAFETY_DeviceState_Shelfmode && !(POWER_GetResetCause() & kRESET_CAUSE_PAD))
        {
#endif /* ENABLE_LOW_POWER */

            /* Enter active state and safe the new state persistently in flash */
            s_deviceState = kSAFETY_DeviceState_Active;
            /* Only change state after flash is written to avoid the radio is activated while
             * data is being written to flash */
            EVENTSTORE_RecordDeviceState(s_deviceState, ChangeStateAfterFlashWrite, (void *)s_deviceState);
        }
        else
        {
            /* Activate the previous state */
            ChangeDeviceState(s_deviceState);
        }

#ifdef MEM_PROFILING
        /* Initializing the timer to call stack profiling function at frequency of PROFILING_INTERVAL_MS 	 */
        timer_status_t tmStatus = TM_Open(s_memProfilingTimerHandle);
        (void)tmStatus;
        assert_equal(tmStatus, kStatus_TimerSuccess);
        tmStatus = TM_InstallCallback((timer_handle_t)s_memProfilingTimerHandle, ProfilingTimerCallback, NULL);
        assert_equal(tmStatus, kStatus_TimerSuccess);
        tmStatus = TM_Start((timer_handle_t)s_memProfilingTimerHandle,
                            kTimerModeLowPowerTimer | kTimerModeIntervalTimer, MEM_PROFILING_INTERVAL_MS);
        assert_equal(tmStatus, kStatus_TimerSuccess);
#ifdef SDK_OS_FREE_RTOS
        /* Manually add IDLE task to stack profiling loop. Other tasks created by application are automatically added in
         * OSA_TaskCreate. */
        OSA_TaskTracking(xTaskGetIdleTaskHandle(), configIDLE_TASK_NAME, configMINIMAL_STACK_SIZE);
#endif /* SDK_OS_FREE_RTOS */
#endif /* MEM_PROFILING */

        s_initialized = true;
    }

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

static void SAFETY_SysTick(void)
{
    if (SysTick->LOAD != s_systickLoad)
    {
        SysTick->LOAD = s_systickLoad;
        SysTick->VAL  = 0;
        /* Clear the pending st interrupt in case it may have fired again due to a low
         * LOAD value.
         */
        SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;
    }

    SysTick_Handler();
}

/**
 * Initializes the safety process
 */
void SAFETY_Init(void)
{
    if (POWER_GetResetCause() & kRESET_CAUSE_WDT)
    {
        PRINTF("!!! System reset by watchdog !!!\n");
    }

    /* Default priorities already set in SystemInit but make sure pin interrupts have lowest priority. */
    NVIC_SetPriority(PIN_INT0_IRQn, NVIC_DEFAULT_PRIORITY + 1);

    s_supplyMode = POWER_DCDC_GetSupplyMode();

    OSA_InstallIntHandler((uint32_t)SysTick_IRQn, SAFETY_SysTick);
    osa_status_t osaStatus = OSA_TaskCreate((osa_task_handle_t)s_safetyTaskHandle, OSA_TASK(SafetyTask), NULL);
    (void)osaStatus;
    assert_equal(osaStatus, KOSA_StatusSuccess);


#ifdef NO_FAST_CONNECTION
    /* Start the watchdog with more than 1min timeout in case of measurement scheduled every minute*/
    status_t status = SAFETY_WatchdogStart(60800);
    assert_equal(status, kStatus_Success);
#else
    /* Start the watchdog with a 2s timeout */
    SAFETY_WatchdogStart(2000);
#endif /* NO_FAST_CONNECTION */

#ifdef SDK_OS_FREE_RTOS
    OSA_Start();
#else /* SDK_OS_FREE_RTOS */


    while (TRUE)
    {
        /* Handle all LL events */
        BLEController_EmngrHandleAllEvents();
        /* Handle OS tasks */
        OSA_ProcessTasks();

        /* Before executing WFI, need to execute some connectivity background tasks
            (usually done in Idle thread) such as NVM save in Idle, etc.. */
#if (gAppUseNvm_d)
        NvIdle();
#endif /* gAppUseNvm_d */

        /* We expect primask always to be 0 (irqs not masked) before disabling them for the low power section. */
        assert_equal(__get_PRIMASK(), 0);
        OSA_DisableIRQGlobal();

        /* Check if some connectivity tasks have turned to ready state from interrupts or
              if messages are to be processed in Application process */
        if ((OSA_TaskShouldYield() == FALSE) && BLEController_EmngrIsIdle())
        {
#ifdef ENABLE_LOW_POWER
            if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0)
            {
                /* High value because, in this case, the duration of low power is constrained only by the radio */
                EnterLowPower(MAX_SLEEP_TIME);
            }
            else
            {
                POWER_EnterSleep();
            }
#else  /* ENABLE_LOW_POWER */
            __WFI();
#endif /* ENABLE_LOW_POWER */
        }
        OSA_EnableIRQGlobal();
#ifdef PROFILE_WAKEUP_TIME
        PMAPP_PrintSmallestHeadroom();
#endif /* PROFILE_WAKEUP_TIME */
    }
#endif /* SDK_OS_FREE_RTOS */
}

/**
 * Writes data to flash
 *
 * @param address The address to write the data to
 * @param data Pointer to the data to be written
 * @param length The length in bytes of the data
 * @param eraseFirst The containing sectors are erased first
 * @param writtenCb Function to be called when data is written
 */
void SAFETY_WriteFlash(
    uint32_t address, uint8_t *data, uint32_t length, bool eraseFirst, safety_written_cb_t writtenCb, void *context)
{
    if (length > 0)
    {
        flash_info_t *flashInfo = MEM_BufferAlloc(sizeof(flash_info_t));
        assert(flashInfo != NULL);
        flashInfo->address    = address;
        flashInfo->data       = data;
        flashInfo->length     = length;
        flashInfo->eraseFirst = eraseFirst;
        flashInfo->writtenCb  = writtenCb;
        flashInfo->context    = context;

        /* Schedule an idle task */
        CONNECTIVITY_ScheduleIdleTask(eraseFirst ? ERASE_TIME : WRITE_TIME, FlashEraseWriteTask, flashInfo);
    }
}

/**
 * Records the radio state
 *
 * @param radioState The radio state to record
 */
void SAFETY_RecordRadioState(safety_radio_state_t radioState)
{
    safety_message_t *msg = MSGQ_CreateMsg((msgq_handler_t)SafetyMsgHandler, sizeof(safety_message_t));
    if (msg)
    {
        msg->msgType         = kMSG_RecordRadioState;
        msg->data.radioState = radioState;
        MSGQ_Put(s_messageQueue, msg);
    }
}

#ifdef MEM_PROFILING
static void ProfilingTimerCallback(void *pParam)
{
    OSA_TaskProfileStacks();
}
#endif /* MEM_PROFILING */

#ifdef ENABLE_LOW_POWER
#ifdef SDK_OS_FREE_RTOS
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    eSleepModeStatus eSleepStatus;

    /* The OSA_InterruptDisable() API will prevent us to wakeup so we use
     * OSA_DisableIRQGlobal() */
    OSA_DisableIRQGlobal();

    /* Check that no debugger is attached. Otherwise entering low power will result in
     * SWD connection loss */
    if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0)
    {
        /* Make sure it's still ok to enter low power mode */
        eSleepStatus = eTaskConfirmSleepModeStatus();

        if (eSleepStatus != eAbortSleep)
        {
            uint32_t expectedIdleTimeUs;

            uint32_t maxIdleTime = 0xffffffff / portTICK_PERIOD_MS / 1000;
            /* Convert the idle time to microseconds. If no tasks are waiting,
             * we set the expectedIdleTimeUs to max */
            expectedIdleTimeUs = eSleepStatus == eNoTasksWaitingTimeout || xExpectedIdleTime >= maxIdleTime ?
                                     MAX_SLEEP_TIME :
                                     xExpectedIdleTime * (portTICK_PERIOD_MS * 1000);

            EnterLowPower(expectedIdleTimeUs);
        }
    }
    else
    {
        POWER_EnterSleep();
    }

    /* Exit from critical section */
    OSA_EnableIRQGlobal();

#ifdef PROFILE_WAKEUP_TIME
    PMAPP_PrintSmallestHeadroom();
#endif /* PROFILE_WAKEUP_TIME */
}
#endif /* SDK_OS_FREE_RTOS */
#endif /* ENABLE_LOW_POWER */

status_t SAFETY_WatchdogStart(uint32_t timeout)
{
#ifdef BLE_POWER_PROFILING
    return kStatus_Success;
#else
    status_t status = kStatus_InvalidArgument;
    if (timeout >= 1024)
    {
        wwdt_config_t config;
        WWDT_GetDefaultConfig(&config);
        config.enableWatchdogReset = true;
        config.clockFreq_Hz        = CLOCK_GetWdtClkFreq();
        config.timeoutValue        = (config.clockFreq_Hz / 4000) * timeout;
        /* To wakeup the system prior to reset. Otherwise, the system won't get reset */
        config.warningValue = 0;
        WWDT_Init(WWDT, &config);
        status = kStatus_Success;
    }
    return status;
#endif /* BLE_POWER_PROFILING */
}

safety_watchdog_handle_t SAFETY_WatchdogRegisterUser(void)
{
    return (safety_watchdog_handle_t)(1 << s_watchdogUserCount++);
}

void SAFETY_WatchdogFeed(safety_watchdog_handle_t handle)
{
#ifndef BLE_POWER_PROFILING
    OSA_DisableIRQGlobal();
    s_watchdogUserFeeds |= handle;
    if (s_watchdogUserFeeds == (1 << s_watchdogUserCount) - 1)
    {
        WWDT_Refresh(WWDT);
        s_watchdogUserFeeds = 0;
    }
    OSA_EnableIRQGlobal();
#endif /* BLE_POWER_PROFILING */
}

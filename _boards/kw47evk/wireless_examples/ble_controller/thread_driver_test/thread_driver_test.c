/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_component_mem_manager.h"
#include "fsl_os_abstraction.h"
#include "fsl_component_timer_manager.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void Test_task(void *param);

/*event test task*/
void EventTest_task(void *param);

/*mutex test task*/
void MutexTest_task(void *param);

/*semaphore test task*/
void SemaphoreTest_task(void *param);

/*timer manager test task*/
static void tm_test_task(void *param);

/*idle task*/
void Idle_task(void *param);

/*******************************************************************************
 * Private memory declarations
 ******************************************************************************/
/*idle task*/
static OSA_TASK_HANDLE_DEFINE(s_IdleTaskHandle);
static OSA_TASK_DEFINE(Idle_task, 15, 1, 512, 0);

/*test task*/
static OSA_TASK_HANDLE_DEFINE(s_TestTaskHandle);
static OSA_TASK_DEFINE(Test_task, 0, 1, 512, 0);

/*semaphore test*/
static OSA_TASK_HANDLE_DEFINE(s_SemaphoreTaskHandle);
static OSA_TASK_DEFINE(SemaphoreTest_task, 2, 1, 512, 0);

/*event test task*/
static OSA_TASK_HANDLE_DEFINE(s_EventTaskHandle);
static OSA_TASK_DEFINE(EventTest_task, 3, 1, 512, 0);

/*mutex test task*/
static OSA_TASK_HANDLE_DEFINE(s_MutexTaskHandle);
static OSA_TASK_DEFINE(MutexTest_task, 4, 1, 512, 0);

static OSA_EVENT_HANDLE_DEFINE(s_testHandle);
static OSA_EVENT_HANDLE_DEFINE(s_testEventHandle);
static OSA_MUTEX_HANDLE_DEFINE(s_testMutexHandle);
static OSA_SEMAPHORE_HANDLE_DEFINE(s_testSemaphoreHandle);

static uint8_t s_semaphoreCount = 0U;

TIMER_MANAGER_HANDLE_DEFINE(timerManagerHandle_1);
TIMER_MANAGER_HANDLE_DEFINE(timerManagerHandle_2);
TIMER_MANAGER_HANDLE_DEFINE(timerManagerHandle_3);
static timer_config_t timerConfig = {

    .instance    = 2,

};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
void tx_application_define(void *first_unused_memory)
{
    osa_status_t status = KOSA_StatusError;
    ULONG entry_input = 100U;

    /* Code that run before tx_kernel_enter() */

    /*create test task*/
    status = OSA_TaskCreate((osa_task_handle_t)s_TestTaskHandle, OSA_TASK(Test_task), &entry_input);
    assert(KOSA_StatusSuccess == status);

    (void)first_unused_memory;
}

void BleDispatchIRQ(void)
{
}

void DispatchIRQ(void)
{
}

void Test_task(void *param)
{
    osa_status_t status = KOSA_StatusError;
    ULONG entry_input = 101U;
    ULONG entry_input1 = 103U;
    osa_event_flags_t waitedEvent = 0U;
    
    tm_test_task(NULL);

    /*create semaphore, mutex, event variable*/
    {
        /*create test event handle*/
        status = OSA_EventCreate((osa_event_handle_t)s_testHandle, 1U);
        assert(KOSA_StatusSuccess == status);

        /*create event*/
        status = OSA_EventCreate((osa_event_handle_t)s_testEventHandle, 1U);
        assert(KOSA_StatusSuccess == status);

        /*create mutex*/
        status = OSA_MutexCreate((osa_mutex_handle_t)&s_testMutexHandle);
        assert(KOSA_StatusSuccess == status);

        /*create semaphore*/
        status = OSA_SemaphoreCreate((osa_semaphore_handle_t)&s_testSemaphoreHandle, 0U);
        assert(KOSA_StatusSuccess == status);
    }

    /*create semaphore test task*/
    status = OSA_TaskCreate((osa_task_handle_t)s_SemaphoreTaskHandle, OSA_TASK(SemaphoreTest_task), &entry_input1);
    assert(KOSA_StatusSuccess == status);

    while(1)
    {
        status = OSA_EventWait((osa_event_handle_t)s_testHandle, 0xF, 0U, osaWaitForever_c, &waitedEvent);
        if(KOSA_StatusSuccess == status)
        {
            if (waitedEvent & 1U)
            {
                /*post semaphore*/
                status = OSA_SemaphorePost((osa_semaphore_handle_t)&s_testSemaphoreHandle);
                assert(KOSA_StatusSuccess == status);
            }

            if(waitedEvent & 2U)
            {
                /*set event*/
                status = OSA_EventSet((osa_event_handle_t)s_testEventHandle, 1U);
                assert(KOSA_StatusSuccess == status);
            }

            if(waitedEvent & 4U)
            {
                /*create idle task*/
                status = OSA_TaskCreate((osa_task_handle_t)s_IdleTaskHandle, OSA_TASK(Idle_task), &entry_input);
                assert(KOSA_StatusSuccess == status);

                break;
            }
        }
    }
}

void SemaphoreTest_task(void *param)
{
    osa_status_t status = KOSA_StatusError;
    ULONG entry_input = 102U;

    while(1)
    {
        status = OSA_EventSet((osa_event_handle_t)s_testHandle, 1U);
        assert(KOSA_StatusSuccess == status);

        status = OSA_SemaphoreWait((osa_semaphore_handle_t)&s_testSemaphoreHandle, osaWaitForever_c);
        if(KOSA_StatusSuccess == status)
        {
            /*when post/wait semaphore 10 times, text mutex*/
            if(s_semaphoreCount++ > 10U)
            {
                /*create event test task*/
                status = OSA_TaskCreate((osa_task_handle_t)s_EventTaskHandle, OSA_TASK(EventTest_task), &entry_input);
                assert(KOSA_StatusSuccess == status);

                break;
            }
        }
    }
}

void EventTest_task(void *param)
{
    osa_status_t status = KOSA_StatusError;
    osa_event_flags_t waitedEvent = 0U;

    /*create mutex test task*/
    status = OSA_TaskCreate((osa_task_handle_t)s_MutexTaskHandle, OSA_TASK(MutexTest_task), NULL);
    assert(KOSA_StatusSuccess == status);

    while(1)
    {
        status = OSA_EventSet((osa_event_handle_t)s_testHandle, 2U);
        assert(KOSA_StatusSuccess == status);

        status = OSA_MutexLock((osa_mutex_handle_t)&s_testMutexHandle, osaWaitForever_c);
        assert(KOSA_StatusSuccess == status);

        status = OSA_EventWait((osa_event_handle_t)s_testEventHandle, 0x1, 0U, osaWaitForever_c, &waitedEvent);
        if(KOSA_StatusSuccess == status)
        {
            status = OSA_MutexUnlock((osa_mutex_handle_t)&s_testMutexHandle);
            assert(KOSA_StatusSuccess == status);

            break;
        }
    }
}

void MutexTest_task(void *param)
{
    osa_status_t status = KOSA_StatusError;

    while(1)
    {
        status = OSA_MutexLock((osa_mutex_handle_t)&s_testMutexHandle, osaWaitForever_c);
        if(KOSA_StatusSuccess == status)
        {
            status = OSA_MutexUnlock((osa_mutex_handle_t)&s_testMutexHandle);
            assert(KOSA_StatusSuccess == status);

            status = OSA_EventSet((osa_event_handle_t)s_testHandle, 4U);
            assert(KOSA_StatusSuccess == status);

            break;
        }
    }
}

void Idle_task(void *param)
{
    osa_status_t status = KOSA_StatusError;

    /*destroy semaphore*/
    status = OSA_SemaphoreDestroy((osa_semaphore_handle_t)&s_testSemaphoreHandle);
    assert(KOSA_StatusSuccess == status);

    /*destroy mutex*/
    status = OSA_MutexDestroy((osa_mutex_handle_t)&s_testMutexHandle);
    assert(KOSA_StatusSuccess == status);

    /*destroy event*/
    status = OSA_EventDestroy((osa_event_handle_t)&s_testHandle);
    assert(KOSA_StatusSuccess == status);

    status = OSA_EventDestroy((osa_event_handle_t)&s_testEventHandle);
    assert(KOSA_StatusSuccess == status);

    /*destroy semaphore test task*/
    status = OSA_TaskDestroy((osa_task_handle_t)s_SemaphoreTaskHandle);
    assert(KOSA_StatusSuccess == status);

    /*destroy event test task*/
 //   status = OSA_TaskDestroy((osa_task_handle_t)s_EventTaskHandle);
   // assert(KOSA_StatusSuccess == status);

    /*destroy mutex test task*/
    status = OSA_TaskDestroy((osa_task_handle_t)s_MutexTaskHandle);
    assert(KOSA_StatusSuccess == status);

    status = OSA_TaskDestroy((osa_task_handle_t)s_TestTaskHandle);
    assert(KOSA_StatusSuccess == status);

    while(1)
    {
        status = KOSA_StatusSuccess;
    }
}




/*******************************************************************************
 * Code
 ******************************************************************************/


void TIMER_MANAGER_CALLBACK(void *parameter)
{
    static uint32_t i = 0;

    if (i == 2)
    {
      TM_Start(timerManagerHandle_1, kTimerModeIntervalTimer, 6 * 1000);
    }
    i++;
}

static void tm_test_task(void *param)
{
    static uint32_t platformInitialized = false;
    if (false == platformInitialized)
    {
        platformInitialized = true;
        
        /* Board pin, clock, debug console init */
        CLOCK_EnableClock(kCLOCK_Sirc_vsys_gating);
          
        /*Initialization lptmr timer*/
        timerConfig.srcClock_Hz = CLOCK_GetFreq(kCLOCK_ScgSircClk);
        TM_Init(&timerConfig);
        TM_Open(timerManagerHandle_1);
        TM_Open(timerManagerHandle_2);
        TM_Open(timerManagerHandle_3);
        TM_InstallCallback(timerManagerHandle_1, TIMER_MANAGER_CALLBACK, NULL);
        TM_InstallCallback(timerManagerHandle_2, TIMER_MANAGER_CALLBACK, NULL);
        TM_InstallCallback(timerManagerHandle_3, TIMER_MANAGER_CALLBACK, NULL);

        TM_Start(timerManagerHandle_3, kTimerModeSingleShot, 15 * 1000);
        TM_Start(timerManagerHandle_2, kTimerModeSingleShot, 10 * 1000);
        TM_Start(timerManagerHandle_1, kTimerModeSingleShot, 5 * 1000);
    }
}


/*!
 * @brief Main function
 */

int main(void)
{
    OSA_Init();

    OSA_Start();
    return 0;
}

/* Definition missing from marvell NBU libs
   ThreadX requires this two variables to be set to know the location o*/
uint32_t *      _tx_initialize_low_level_ptr;
uint32_t *      tx_application_define_ptr;


void * _memcpy (void *dest, const void * src, size_t n)
{
    return memcpy(dest, src, n);
}

/* called from Marvell reset vector in name. It is not be used in our code */
int __main(void)
{
    assert(0);
    return 0;
}

void HardFault_Handler(void)
{
    while(1);
}

void BusFault_Handler(void)
{
    while(1);
}

void UsageFault_Handler(void)
{
    while(1);
}

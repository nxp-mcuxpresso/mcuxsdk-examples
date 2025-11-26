/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
#include "fsl_common.h"
#include "fsl_os_abstraction.h"
#include "fsl_os_abstraction_free_rtos.h"
#include <string.h>
#include "fsl_component_generic_list.h"
#include "ncp_utils.h"


/*! @brief Converts milliseconds to ticks*/
#define MSEC_TO_TICK(msec) \
    (((uint32_t)(msec) + 500uL / (uint32_t)configTICK_RATE_HZ) * (uint32_t)configTICK_RATE_HZ / 1000uL)
#define TICKS_TO_MSEC(tick) ((uint32_t)((uint64_t)(tick)*1000uL / (uint64_t)configTICK_RATE_HZ))

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQPutBlock
 * Description   : This function is used to put a message to a message queue and blcok when queue full.
 * Return         : KOSA_StatusSuccess if the message is put successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQPutBlock(osa_msgq_handle_t msgqHandle, osa_msg_handle_t pMessage, uint32_t millisec)
{
    osa_status_t osaStatus;
    assert(NULL != msgqHandle);
    portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;
    QueueHandle_t handler    = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)msgqHandle);
    uint32_t timeoutTicks;

    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(millisec);
    }
    if (0U != __get_IPSR())
    {
        if (((BaseType_t)1) == (BaseType_t)xQueueSendToBackFromISR(handler, pMessage, &taskToWake))
        {
            portYIELD_FROM_ISR(((bool)(taskToWake)));
            osaStatus = KOSA_StatusSuccess;
        }
        else
        {
            osaStatus = KOSA_StatusError;
        }
    }
    else
    {
        osaStatus = (xQueueSendToBack(handler, pMessage, timeoutTicks) == pdPASS) ? (KOSA_StatusSuccess) : (KOSA_StatusError);
    }

    return osaStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimerCreateEx
 * Description   : This function is used to create a timer.
 * Return        : Timer handle of the new timer, or NULL if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_TimerCreateEx(osa_timer_handle_t timerHandle,
                             osa_timer_tick_ex ticks,
                             void (*call_back)(osa_timer_arg_ex_t),
                             void *cb_arg,
                             osa_timer_t reload,
                             osa_timer_activate_ex_t activate)
{
    int auto_reload = (reload == KOSA_TimerOnce) ? pdFALSE : pdTRUE;

#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    assert((sizeof(osa_timer_handle_t) + sizeof(StaticTimer_t)) == OSA_TIMER_HANDLE_SIZE);
#else
    assert(sizeof(osa_timer_handle_t) == OSA_TIMER_HANDLE_SIZE);
#endif
    assert(NULL != timerHandle);

    union
    {
        TimerHandle_t tm;
        uint32_t timerhandle;
    } xTimerHandle;

#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    xTimerHandle.tm =
        xTimerCreateStatic(NULL, ticks, (UBaseType_t)auto_reload, cb_arg, call_back,
                           (StaticTimer_t *)(void *)((uint8_t *)timerHandle + sizeof(osa_timer_handle_t)));
#else
    xTimerHandle.tm = xTimerCreate(NULL, ticks, (UBaseType_t)auto_reload, cb_arg, call_back);
#endif
    if (NULL != xTimerHandle.tm)
    {
        *(uint32_t *)timerHandle = xTimerHandle.timerhandle;

        if (activate == OSA_TIMER_AUTO_ACTIVATE_EX)
        {
            return OSA_TimerActivateEx(timerHandle);
        }

        return KOSA_StatusSuccess;
    }

    return KOSA_StatusError;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimerActivateEx
 * Description   : This function activates (or starts) a timer.
 * Return        : KOSA_StatusSuccess if successfully, or KOSA_StatusError if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_TimerActivateEx(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    osa_status_t status = KOSA_StatusError;
    TimerHandle_t tm    = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    if (0U != __get_IPSR())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)1) == (BaseType_t)xTimerStartFromISR(tm, &taskToWake))
        {
            portYIELD_FROM_ISR(((bool)(taskToWake)));
            status = KOSA_StatusSuccess;
        }
        else
        {
            status = KOSA_StatusError;
        }
    }
    else
    {
        if (((BaseType_t)1) == (BaseType_t)xTimerStart(tm, 0))
        {
            status = KOSA_StatusSuccess; /* sync object given */
        }
        else
        {
            status = KOSA_StatusError;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimerChangeEx
 * Description   : This function changes timer period.
 * Return        : KOSA_StatusSuccess if successfully, or KOSA_StatusError if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_TimerChangeEx(osa_timer_handle_t timerHandle, osa_timer_tick_ex ntime, osa_timer_tick_ex block_time)
{
    assert(NULL != timerHandle);
    osa_status_t status = KOSA_StatusError;
    TimerHandle_t tm    = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    if (0U != __get_IPSR())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)1) == (BaseType_t)xTimerChangePeriodFromISR(tm, ntime, &taskToWake))
        {
            portYIELD_FROM_ISR(((bool)(taskToWake)));
            status = KOSA_StatusSuccess;
        }
        else
        {
            status = KOSA_StatusError;
        }
    }
    else
    {
        if (((BaseType_t)1) == (BaseType_t)xTimerChangePeriod(tm, ntime, 100))
        {
            status = KOSA_StatusSuccess; /* sync object given */
        }
        else
        {
            status = KOSA_StatusError;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimerIsRunningEx
 * Description   : This function checks the timer active state.
 * Return        : true if timer is active, false if time is not active.
 *
 *END**************************************************************************/
bool OSA_TimerIsRunningEx(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    TimerHandle_t tm = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    if (((BaseType_t)0) == (BaseType_t)xTimerIsTimerActive(tm))
    {
        return false; /* Timer not active */
    }
    else
    {
        return true; /* Timer active */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimerDeactivateEx
 * Description   : This function deactivates (or stops) a timer that was previously started.
 * Return        : KOSA_StatusSuccess if successfully, or KOSA_StatusError if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_TimerDeactivateEx(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    osa_status_t status = KOSA_StatusError;
    TimerHandle_t tm    = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    if (0U != __get_IPSR())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)1) == (BaseType_t)xTimerStopFromISR(tm, &taskToWake))
        {
            portYIELD_FROM_ISR(((bool)(taskToWake)));
            status = KOSA_StatusSuccess;
        }
        else
        {
            status = KOSA_StatusError;
        }
    }
    else
    {
        if (((BaseType_t)1) == (BaseType_t)xTimerStop(tm, 0))
        {
            status = KOSA_StatusSuccess; /* sync object given */
        }
        else
        {
            status = KOSA_StatusError;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimerDestroyEx
 * Description   : This function deletes a timer.
 * Return        : KOSA_StatusSuccess if successfully, or KOSA_StatusError if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_TimerDestroyEx(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    TimerHandle_t tm = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);
    int ret;

    /* Below timer handle invalidation needs to be protected as a context
     * switch may create issues if same handle is used before
     * invalidation.
     */
    OSA_SR_ALLOC();

    OSA_ENTER_CRITICAL();
    /* Note: Block time is set as 0, thus signifying non-blocking
       API. Can be changed later if required. */
    ret = xTimerDelete(tm, 0);

    OSA_EXIT_CRITICAL();

    return ret == pdPASS ? KOSA_StatusSuccess : KOSA_StatusError;
}

#if CONFIG_NCP_TP_DEBUG
#define NCP_DEBUG_TIME_COUNT 512
#define NCP_DEBUG_TIME_FUNC  128
int ncp_debug_time_num                                             = 0;
unsigned long ncp_debug_time[NCP_DEBUG_TIME_COUNT]                 = {0};
char ncp_debug_time_pos[NCP_DEBUG_TIME_COUNT][NCP_DEBUG_TIME_FUNC] = {0};
extern int os_get_reltime(struct os_reltime *t);
void print_ncp_debug_time(void)
{
    for (int i = 0; i < ncp_debug_time_num; i++)
        (void)PRINTF("%d-%s-%lu\r\n", i, ncp_debug_time_pos[i], ncp_debug_time[i]);
    for (int i = 1; i < ncp_debug_time_num; i++)
        (void)PRINTF("[%d-%lu]\r\n", i, ncp_debug_time[i] - ncp_debug_time[i - 1]);
    ncp_debug_time_num = 0;
}
void add_ncp_debug_time_item(const char *func)
{
    struct os_reltime time;
    int func_len = strlen(func) + 1 <= NCP_DEBUG_TIME_FUNC ? strlen(func) + 1 : NCP_DEBUG_TIME_FUNC;
    if (ncp_debug_time_num >= NCP_DEBUG_TIME_COUNT)
    {
        (void)PRINTF("the array is full, please increase NCP_DEBUG_TIME_COUNT\r\n");
        return;
    }
    os_get_reltime(&time);
    ncp_debug_time[ncp_debug_time_num] = time.sec*1000*1000+time.usec;
    memcpy(ncp_debug_time_pos[ncp_debug_time_num++], func, func_len);
}

#if CONFIG_NCP_DEBUG_SCHED
#define NCP_DEBUG_TIME_SWITCH 5 * 4096
extern unsigned long task_switch_num;
extern unsigned long ncp_debug_task_switch_interval[NCP_DEBUG_TIME_SWITCH];
extern char *ncp_debug_task_switch[NCP_DEBUG_TIME_SWITCH];
extern int ncp_debug_task_switch_start, ncp_debug_task_switch_print;
void trace_task_switch_print()
{
    if (ncp_debug_task_switch_print)
    {
        for (unsigned long i = 0; i < task_switch_num; i++)
            (void)PRINTF("%d-%s-%lu\r\n", i, ncp_debug_task_switch[i], ncp_debug_task_switch_interval[i]);
        ncp_debug_task_switch_print = 0;
    }
}
static int wlan_sched_info(void *tlv)
{
/* rtos kernel need to add code below:
extern unsigned int OSA_GetTimestamp(void);
#define NCP_DEBUG_TIME_SWITCH 4 * 4096
unsigned long task_switch_num = 0;
unsigned long ncp_debug_task_switch_interval[NCP_DEBUG_TIME_SWITCH] = {0};
char *ncp_debug_task_switch[NCP_DEBUG_TIME_SWITCH] = {0};
int ncp_debug_task_switch_start = 0, ncp_debug_task_switch_print = 0;
unsigned long ncp_debug_task_switch_time_in;
void trace_task_switch(int in)
{
    if (ncp_debug_task_switch_start && task_switch_num < NCP_DEBUG_TIME_SWITCH)
    {
        ncp_debug_task_switch[task_switch_num] = pxCurrentTCB->pcTaskName;
        if (in)
        {
           ncp_debug_task_switch_time_in = OSA_GetTimestamp();
        }
        if (!in)
        {
           ncp_debug_task_switch_interval[task_switch_num] = OSA_GetTimestamp() - ncp_debug_task_switch_time_in;
        }
        task_switch_num++;
    }
}
*/
    trace_task_switch_print();
    ncp_debug_task_switch_start = !ncp_debug_task_switch_start;
    ncp_debug_task_switch_print = !ncp_debug_task_switch_print;
}
#endif
#endif

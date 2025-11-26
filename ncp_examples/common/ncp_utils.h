/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
#ifndef __NCP_UTILS__
#define __NCP_UTILS__

/**
 * OS Timer Activate Options
 */
typedef enum osa_timer_activation_ex
{
    /** Start the timer on creation. */
    OSA_TIMER_AUTO_ACTIVATE_EX,
    /** Do not start the timer on creation. */
    OSA_TIMER_NO_ACTIVATE_EX,
} osa_timer_activate_ex_t;

typedef TickType_t osa_timer_tick_ex;
typedef TimerHandle_t osa_timer_arg_ex_t;

/*!
 * @brief Puts a message at the end of the queue.
 *
 * This function puts a message to the end of the message queue. If the queue
 * is full, this function returns the KOSA_StatusError;
 *
 * @param msgqHandle  Message Queue handler.
 * @param pMessage Pointer to the message to be put into the queue.
 * @param millisec  block timeout.
 *
 * @retval KOSA_StatusSuccess Message successfully put into the queue.
 * @retval KOSA_StatusError   The queue was full or an invalid parameter was passed.
 */
osa_status_t OSA_MsgQPutBlock(osa_msgq_handle_t msgqHandle, osa_msg_handle_t pMessage, uint32_t millisec);

/*** Timer Management ***/
/*!
 * @brief Create timer
 *
 * This function creates a timer.
 *
 * @param[in] timerHandle Pointer to the timer handle
 * @param[in] ticks Period in ticks
 * @param[in] call_back Timer expire callback function
 * @param[in] cb_arg Timer callback data
 * @param[in] reload Reload Options, valid values include \ref KOSA_TimerOnce
 * or \ref KOSA_TimerPeriodic.
 * @param[in] activate Activate Options, valid values include \ref
 * OSA_TIMER_AUTO_ACTIVATE or \ref OSA_TIMER_NO_ACTIVATE
 *
 * @return KOSA_StatusSuccess if timer created successfully
 * @return KOSA_StatusError if timer creation fails
 */
osa_status_t OSA_TimerCreateEx(osa_timer_handle_t timerHandle,
                             osa_timer_tick_ex ticks,
                             void (*call_back)(osa_timer_arg_ex_t),
                             void *cb_arg,
                             osa_timer_t reload,
                             osa_timer_activate_ex_t activate);

/*!
 * @brief Activate timer
 *
 * This function activates (or starts) a timer that was previously created using
 * OSA_TimerCreate(). If the timer had already started and was already in the
 * active state, then this call is equivalent to OSA_TimerReset().
 *
 * @param[in] timerHandle Pointer to a timer handle
 *
 * @return KOSA_StatusSuccess if timer activated successfully
 * @return KOSA_StatusError if timer activation fails
 *
 */
osa_status_t OSA_TimerActivateEx(osa_timer_handle_t timerHandle);

/*!
 * @brief Change timer period
 *
 * This function changes the period of a timer that was previously created using
 * OSA_TimerCreate(). This function changes the period of an active or dormant
 * state timer.
 *
 * @param[in] timerHandle Pointer to a timer handle
 * @param[in] ntime Time in ticks after which the timer will expire
 * @param[in] block_time  This option is currently not supported
 *
 * @return KOSA_StatusSuccess if timer change successfully
 * @return KOSA_StatusError if timer change fails
 */
osa_status_t OSA_TimerChangeEx(osa_timer_handle_t timerHandle, osa_timer_tick_ex ntime, osa_timer_tick_ex block_time);

/*!
 * @brief Check the timer active state
 *
 * This function checks if the timer is in the active or dormant state. A timer
 * is in the dormant state if (a) it has been created but not started, or (b) it
 * has expired and a one-shot timer.
 *
 * @param [in] timerHandle Pointer to a timer handle
 *
 * @return true if timer is active
 * @return false if time is not active
 */
bool OSA_TimerIsRunningEx(osa_timer_handle_t timerHandle);

/*!
 * @brief Deactivate timer
 *
 * This function deactivates (or stops) a timer that was previously started.
 *
 * @param [in] timerHandle handle populated by OSA_TimerCreate().
 *
 * @return KOSA_StatusSuccess if timer deactivate successfully
 * @return KOSA_StatusError if timer deactivate fails
 */
osa_status_t OSA_TimerDeactivateEx(osa_timer_handle_t timerHandle);

/*!
 * @brief Destroy timer
 *
 * This function deletes a timer.
 *
 * @param[in] timerHandle Pointer to a timer handle
 *
 * @return KOSA_StatusSuccess if timer destroy successfully
 * @return KOSA_StatusError if timer destroy fails
 */
osa_status_t OSA_TimerDestroyEx(osa_timer_handle_t timerHandle);

#if CONFIG_NCP_TP_DEBUG
void print_ncp_debug_time(void);
void add_ncp_debug_time_item(const char *func);

#if CONFIG_NCP_DEBUG_SCHED
void trace_task_switch_print();
#endif

#endif

#endif

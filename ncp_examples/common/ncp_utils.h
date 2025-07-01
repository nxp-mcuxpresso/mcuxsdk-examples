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

#if CONFIG_NCP_TP_DEBUG
void print_ncp_debug_time(void);
void add_ncp_debug_time_item(const char *func);

#if CONFIG_NCP_DEBUG_SCHED
void trace_task_switch_print();
#endif

#endif

#endif

/*
 * Copyright 2022, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file msgq.h
 * @brief Message queue handling API
 *
 * This file provides functions for creating and managing message queues,
 * as well as utilities for handling messages and queues 
 * using operating system abstraction layer (OSA).
 * 
 */

#ifndef _MSGQ_H_
#define _MSGQ_H_

#include "fsl_common.h"
#include "fsl_os_abstraction.h"

/**
 * @brief Function pointer type for message handlers.
 * @param data Pointer to the message data.
 */
typedef void (*msgq_handler_t)(void *data);

/**
 * @brief Handle type for message queues.
 */
typedef osa_msgq_handle_t msgq_handle_t;

/**
 * @brief Creates a message with the specified handler and size.
 * 
 * @param handler The handler function for processing the message.
 * @param size The size of the message.
 * @return Pointer to the created message, or NULL on failure.
 */
void *MSGQ_CreateMsg(msgq_handler_t handler, uint32_t size);

/**
 * @brief Frees the memory allocated for a message.
 * 
 * @param message Pointer to the message to free.
 */
void MSGQ_FreeMsg(void *message);

/**
 * @brief Adds a message to the specified message queue.
 * 
 * @param handle Handle of the message queue.
 * @param message Pointer to the message to add.
 * @return Status of the operation.
 */
osa_status_t MSGQ_Put(msgq_handle_t handle, void *message);

/**
 * @brief Retrieves a message from the specified queue, waiting if necessary.
 * 
 * @param handle Handle of the message queue.
 * @param timeout Timeout value in milliseconds. Use 0 for no wait.
 * @return Pointer to the retrieved message, or NULL if timeout occurs.
 */
void *MSGQ_Get(msgq_handle_t handle, uint32_t timeout);

/**
 * @brief Creates a message queue with the specified parameters.
 * 
 * @param handle Handle for the new message queue.
 * @param count Maximum number of messages in the queue.
 * @param size Size of each message.
 * @return Status of the operation.
 */
osa_status_t MSGQ_CreateQueue(msgq_handle_t handle, uint32_t count, uint32_t size);

/**
 * @brief Destroys the specified message queue.
 * 
 * @param handle Handle of the message queue to destroy.
 * @return Status of the operation.
 */
osa_status_t MSGQ_DestroyQueue(msgq_handle_t handle);

/**
 * @brief Retrieves the handler function associated with a message.
 * 
 * @param message Pointer to the message.
 * @return The message handler function.
 */
msgq_handler_t MSGQ_GetHandler(void *message);

#endif /* _MSGQ_H_ */
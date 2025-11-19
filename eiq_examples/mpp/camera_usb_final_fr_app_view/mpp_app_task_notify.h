/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MPP_TASK_NOTIFY_H
#define MPP_TASK_NOTIFY_H

#include "FreeRTOS.h"
#include "queue.h"
#include <stdint.h>
#include "app_msg.h"

#define MPP_CMD_QUEUE_SIZE      8
/**
 * @brief Set the MPP task queue handle
 * @param queue The queue handle to use for MPP task notifications
 */
void mpp_task_set_queue(QueueHandle_t queue);

/**
 * @brief Send a notification message to the MPP task
 * @param cmd Command to send
 * @param para Parameter pointer (can be NULL)
 */
void mpp_task_notify(uint8_t cmd, msg_t* para);

#endif /* MPP_TASK_NOTIFY_H */

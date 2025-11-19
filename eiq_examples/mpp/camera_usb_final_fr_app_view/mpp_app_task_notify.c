/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "queue.h"
#include "fsl_debug_console.h"
#include "app_msg.h"
#include "mpp_app_task_notify.h"

static QueueHandle_t s_MppTaskQueue = NULL;

void mpp_task_set_queue(QueueHandle_t queue)
{
    s_MppTaskQueue = queue;
}

void mpp_task_notify(uint8_t cmd, msg_t* para)
{
    if (para == NULL) {
		para = (msg_t*)pvPortMalloc(sizeof(msg_t));
		para->msgID = cmd;
	}else{
		para->msgID = cmd;
	}

    if (s_MppTaskQueue == NULL)
    {
        PRINTF("Error: MPP task queue not initialized\r\n");
        vPortFree(para);
        return;
    }

    if (__get_IPSR() == 0)
    {
        xQueueSend(s_MppTaskQueue, &para, portMAX_DELAY);
    }
    else
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(s_MppTaskQueue, &para, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

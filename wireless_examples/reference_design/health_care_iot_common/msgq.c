/*
 * Copyright 2022, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "msgq.h"
#include "fsl_os_abstraction.h"
#include "fsl_component_mem_manager.h"

#define DATA_OFFSET ((uint32_t)(&(((msgq_message_t *)0x0)->data)))

typedef struct
{
    msgq_handler_t handler;
    uint8_t data[];
} msgq_message_t;

void *MSGQ_CreateMsg(msgq_handler_t handler, uint32_t size)
{
    msgq_message_t *msg = MEM_BufferAlloc(sizeof(msgq_message_t) + size);
    if (msg)
    {
        msg->handler = handler;
        return &msg->data;
    }
    return NULL;
}

void MSGQ_FreeMsg(void *message)
{
    MEM_BufferFree(((uint8_t *)message) - DATA_OFFSET);
}

msgq_handler_t MSGQ_GetHandler(void *message)
{
    return ((msgq_message_t *)((uint8_t *)message - DATA_OFFSET))->handler;
}

osa_status_t MSGQ_Put(msgq_handle_t handle, void *message)
{
    return OSA_MsgQPut(handle, &message);
}

void *MSGQ_Get(msgq_handle_t handle, uint32_t timeout)
{
    msgq_message_t *message;
    if (OSA_MsgQGet(handle, &message, timeout) != KOSA_StatusSuccess)
    {
        message = NULL;
    }
    return message;
}

osa_status_t MSGQ_CreateQueue(msgq_handle_t handle, uint32_t count, uint32_t size)
{
    return OSA_MsgQCreate(handle, count, size);
}

osa_status_t MSGQ_DestroyQueue(msgq_handle_t handle)
{
    return OSA_MsgQDestroy(handle);
}

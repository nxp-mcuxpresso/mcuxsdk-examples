/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stp_freertos_adapter.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

extern stp_context_t g_stp_ctx;

static void stp_one_second_timer_callback(TimerHandle_t xTimer)
{
    stp_event_t event = {
        .type = STP_EVENT_ONE_SECOND_TICK,
    };
    stp_send_event(&event);
}

static void stp_task(void *pvParameters)
{
    unsigned int timestamp;
    stp_event_t event;

    g_stp_ctx.one_second_timer = xTimerCreate(
        "STP_Timer",
        pdMS_TO_TICKS(1000),
        pdTRUE,
        NULL,
        stp_one_second_timer_callback
    );
    configASSERT(g_stp_ctx.one_second_timer != NULL);
    xTimerStart(g_stp_ctx.one_second_timer, 0);

    PRINTF("[STP] Task started\r\n");

    while (1)
    {
        if (xQueueReceive(g_stp_ctx.event_queue, &event, portMAX_DELAY) == pdPASS)
        {
            xSemaphoreTake(g_stp_ctx.mutex, portMAX_DELAY);
            timestamp = xTaskGetTickCount();

            switch (event.type)
            {
                case STP_EVENT_BPDU_RECEIVED:
                    PRINTF("[STP] BPDU received on port %u, %u bytes\r\n",
                           event.port_index + 1, (unsigned int)event.bpdu_len);

                    STP_OnBpduReceived(
                        g_stp_ctx.bridge,
                        event.port_index,
                        event.bpdu_data,
                        event.bpdu_len,
                        timestamp
                    );

                    vPortFree(event.bpdu_data);
                    break;

                case STP_EVENT_PORT_LINK_UP:
                    PRINTF("[STP] Port %u link up: %u Mbps, %s\r\n",
                           event.port_index + 1,
                           event.speed_mbps,
                           event.point_to_point ? "P2P" : "Shared");

                    STP_OnPortEnabled(
                        g_stp_ctx.bridge,
                        event.port_index,
                        event.speed_mbps,
                        event.point_to_point,
                        timestamp
                    );
                    break;

                case STP_EVENT_PORT_LINK_DOWN:
                    PRINTF("[STP] Port %u link down\r\n", event.port_index + 1);

                    STP_OnPortDisabled(
                        g_stp_ctx.bridge,
                        event.port_index,
                        timestamp
                    );
                    break;

                case STP_EVENT_ONE_SECOND_TICK:
                    STP_OnOneSecondTick(g_stp_ctx.bridge, timestamp);
                    break;

                default:
                    PRINTF("[STP] Unknown event type: %d\r\n", event.type);
                    break;
            }

            xSemaphoreGive(g_stp_ctx.mutex);
        }
    }
}

void stp_task_create(void)
{
    BaseType_t ret = xTaskCreate(
        stp_task,
        "STP_Task",
        configMINIMAL_STACK_SIZE + 1024,
        NULL,
        configMAX_PRIORITIES - 3,
        NULL
    );
    configASSERT(ret == pdPASS);
}

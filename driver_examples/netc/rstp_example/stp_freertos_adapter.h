/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef STP_FREERTOS_ADAPTER_H
#define STP_FREERTOS_ADAPTER_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"
#include "stp.h"
#include "fsl_debug_console.h"

typedef enum {
    STP_EVENT_BPDU_RECEIVED,
    STP_EVENT_PORT_LINK_UP,
    STP_EVENT_PORT_LINK_DOWN,
    STP_EVENT_ONE_SECOND_TICK,
} stp_event_type_t;

typedef struct {
    stp_event_type_t type;
    unsigned int port_index;
    unsigned int speed_mbps;
    bool point_to_point;
    uint8_t *bpdu_data;
    size_t bpdu_len;
} stp_event_t;

typedef struct {
    struct STP_BRIDGE *bridge;
    QueueHandle_t event_queue;
    SemaphoreHandle_t mutex;
    TimerHandle_t one_second_timer;
    uint8_t bridge_mac[6];
    unsigned int port_count;
} stp_context_t;

void stp_freertos_init(const uint8_t *bridge_mac, unsigned int port_count);
void stp_freertos_start(void);
void stp_freertos_stop(void);
void stp_send_event(stp_event_t *event);
void stp_rx_handler(unsigned int port_index, uint8_t *frame, size_t frame_len);
void stp_link_status_changed(unsigned int port_index, 
                             bool link_up, 
                             unsigned int speed_mbps,
                             bool full_duplex);
void stp_task_create(void);

#endif

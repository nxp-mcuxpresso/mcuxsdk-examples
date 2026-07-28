/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stp_freertos_adapter.h"
#include "netc_switch.h"
#include <string.h>
#include "app.h"

typedef struct
{
    uint32_t port_index;
    uint32_t bpdu_size;
} bpdu_tx_header_t;

/* Timer handles for per-port rapid-ageing FDB flush */
#define STP_RAPID_AGE_DELAY_MS 15000U
static TimerHandle_t g_rapid_age_timers[EXAMPLE_SWT_MAX_PORT_NUM]; /* one per switch port */

/* Upper bound for a BPDU payload; guards buffer allocation against bad sizes.
 * The largest standard MSTP BPDU is well under this limit. */
#define STP_MAX_BPDU_SIZE 256U

/* Debug log buffer size passed to STP_CreateBridge; also caps debug output. */
#define STP_DEBUG_LOG_BUF_SIZE 512U

stp_context_t g_stp_ctx;

static void *stp_alloc_memory(unsigned int size)
{
    void *ptr = pvPortMalloc(size);
    if (ptr != NULL)
    {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void stp_free_memory(void *ptr)
{
    vPortFree(ptr);
}

static void stp_enable_bpdu_trapping(const struct STP_BRIDGE *bridge,
                                     bool enable,
                                     unsigned int timestamp)
{
    netc_sw_trap_bpdu(enable);

    PRINTF("[STP] BPDU trapping %s at %u\r\n", enable ? "enabled" : "disabled", timestamp);
}

static void stp_enable_learning(const struct STP_BRIDGE *bridge,
                                unsigned int port_index,
                                unsigned int tree_index,
                                bool enable,
                                unsigned int timestamp)
{
    netc_sw_stp_port_learning(port_index, enable);

    PRINTF("[STP] Port %u Tree %u: Learning %s at %u\r\n",
           port_index + 1, tree_index, enable ? "enabled" : "disabled", timestamp);
}

static void stp_enable_forwarding(const struct STP_BRIDGE *bridge,
                                  unsigned int port_index,
                                  unsigned int tree_index,
                                  bool enable,
                                  unsigned int timestamp)
{
    netc_sw_stp_port_forwarding(port_index, enable);

    PRINTF("[STP] Port %u Tree %u: Forwarding %s at %u\r\n",
           port_index + 1, tree_index, enable ? "enabled" : "disabled", timestamp);
}

static void stp_rapid_age_timer_callback(TimerHandle_t xTimer)
{
    uint32_t port_index = (uint32_t)(uintptr_t)pvTimerGetTimerID(xTimer);
    netc_sw_port_fdb_flush(port_index);
    PRINTF("[STP] Port %u: Rapid-ageing FDB flush completed\r\n", port_index + 1);
}

static void stp_flush_fdb(const struct STP_BRIDGE *bridge,
                          unsigned int port_index,
                          unsigned int tree_index,
                          enum STP_FLUSH_FDB_TYPE flush_type,
                          unsigned int timestamp)
{
    if (flush_type == STP_FLUSH_FDB_TYPE_IMMEDIATE)
    {
        netc_sw_port_fdb_flush(port_index);
        PRINTF("[STP] Port %u Tree %u: Flush FDB (immediate) at %u\r\n",
               port_index + 1, tree_index, timestamp);
    }
    else
    {
        /* Rapid ageing: schedule flush after FwdDelay (15 s) instead of flushing immediately */
        if (port_index < (sizeof(g_rapid_age_timers) / sizeof(g_rapid_age_timers[0])))
        {
            if (g_rapid_age_timers[port_index] == NULL)
            {
                g_rapid_age_timers[port_index] = xTimerCreate(
                    "RapidAge",
                    pdMS_TO_TICKS(STP_RAPID_AGE_DELAY_MS),
                    pdFALSE, /* one-shot */
                    (void *)(uintptr_t)port_index,
                    stp_rapid_age_timer_callback
                );
            }
            if (g_rapid_age_timers[port_index] != NULL)
            {
                xTimerStart(g_rapid_age_timers[port_index], 0);
            }
        }
        PRINTF("[STP] Port %u Tree %u: Flush FDB (rapid ageing, 15s) at %u\r\n",
               port_index + 1, tree_index, timestamp);
    }
}

static void *stp_transmit_get_buffer(const struct STP_BRIDGE *bridge,
                                     unsigned int port_index,
                                     unsigned int bpdu_size,
                                     unsigned int timestamp)
{
    uint8_t *buffer;
    uint8_t *frame;

    if (bpdu_size > STP_MAX_BPDU_SIZE)
    {
        return NULL;
    }

    buffer = pvPortMalloc(sizeof(bpdu_tx_header_t) + 14 + 3 + bpdu_size);
    if (buffer == NULL)
    {
        return NULL;
    }

    bpdu_tx_header_t *header = (bpdu_tx_header_t *)buffer;
    header->port_index       = port_index;
    header->bpdu_size        = bpdu_size;

    frame = buffer + sizeof(bpdu_tx_header_t);

    /* Destination MAC: 01:80:C2:00:00:00 (Bridge Group Address) */
    frame[0] = 0x01; frame[1] = 0x80; frame[2] = 0xC2;
    frame[3] = 0x00; frame[4] = 0x00; frame[5] = 0x00;

    /* Source MAC */
    memcpy(&frame[6], g_stp_ctx.bridge_mac, 6);

    /* Length field: BPDU size + 3 bytes for LLC header */
    frame[12] = 0x00;
    frame[13] = (uint8_t)(bpdu_size + 3U);

    /* Return pointer past Ethernet header and LLC header; caller fills BPDU */
    return &frame[14 + 3];
}

static void stp_transmit_release_buffer(const struct STP_BRIDGE *bridge, void *buffer_address)
{
    uint8_t *frame           = (uint8_t *)buffer_address - 14 - 3;
    uint8_t *buffer          = frame - sizeof(bpdu_tx_header_t);
    bpdu_tx_header_t *header = (bpdu_tx_header_t *)buffer;

    uint8_t *llc = frame + 14;
    llc[0] = 0x42; /* DSAP: 802.1 Bridge Spanning Tree */
    llc[1] = 0x42; /* SSAP: 802.1 Bridge Spanning Tree */
    llc[2] = 0x03; /* Control: Unnumbered Information  */

    size_t frame_len = 14 + 3 + header->bpdu_size;

    if (netc_sw_transmit(header->port_index, frame, frame_len) != kStatus_Success)
    {
        PRINTF("[STP] Transmit BPDU failed on port %u\r\n", header->port_index + 1);
    }
    else
    {
        PRINTF("[STP] Transmit BPDU: %u bytes\r\n", (unsigned int)frame_len);
    }

    vPortFree(buffer);
}

static void stp_on_port_role_changed(const struct STP_BRIDGE *bridge,
                                     unsigned int port_index,
                                     unsigned int tree_index,
                                     enum STP_PORT_ROLE role,
                                     unsigned int timestamp)
{
    const char *role_str = STP_GetPortRoleString(role);
    PRINTF("[STP] Port %u Tree %u: Role changed to %s at %u\r\n",
           port_index + 1, tree_index, role_str, timestamp);
}

static void stp_on_topology_change(const struct STP_BRIDGE *bridge,
                                   unsigned int tree_index,
                                   unsigned int timestamp)
{
    PRINTF("[STP] Tree %u: Topology change detected at %u\r\n", tree_index, timestamp);
}

static void stp_debug_log(const struct STP_BRIDGE *bridge, int portIndex, int treeIndex,
                          const char *str, unsigned int str_len, unsigned int flush)
{
    (void)bridge;
    (void)portIndex;
    (void)treeIndex;
    (void)flush;

    if (str == NULL || str_len == 0U)
    {
        return;
    }
    if (str_len > STP_DEBUG_LOG_BUF_SIZE)
    {
        str_len = STP_DEBUG_LOG_BUF_SIZE;
    }
    PRINTF("%.*s", str_len, str);
}

static const struct STP_CALLBACKS stp_callbacks = {
    .allocAndZeroMemory    = stp_alloc_memory,
    .freeMemory            = stp_free_memory,
    .enableBpduTrapping    = stp_enable_bpdu_trapping,
    .enableLearning        = stp_enable_learning,
    .enableForwarding      = stp_enable_forwarding,
    .flushFdb              = stp_flush_fdb,
    .transmitGetBuffer     = stp_transmit_get_buffer,
    .transmitReleaseBuffer = stp_transmit_release_buffer,
    .onPortRoleChanged     = stp_on_port_role_changed,
    .onTopologyChange      = stp_on_topology_change,
    .debugStrOut           = stp_debug_log,
};

void stp_freertos_init(const uint8_t *bridge_mac, unsigned int port_count)
{
    memcpy(g_stp_ctx.bridge_mac, bridge_mac, 6);
    g_stp_ctx.port_count = port_count;

    g_stp_ctx.mutex = xSemaphoreCreateMutex();
    configASSERT(g_stp_ctx.mutex != NULL);

    g_stp_ctx.event_queue = xQueueCreate(20, sizeof(stp_event_t));
    configASSERT(g_stp_ctx.event_queue != NULL);

    g_stp_ctx.bridge = STP_CreateBridge(
        port_count,      /* number of ports               */
        0,               /* MSTI count (0 for RSTP mode)  */
        1,               /* max VLAN ID                   */
        &stp_callbacks,  /* callback table                */
        bridge_mac,      /* bridge MAC address            */
        STP_DEBUG_LOG_BUF_SIZE /* debug log buffer size (bytes) */
    );
    configASSERT(g_stp_ctx.bridge != NULL);

    /* Use RSTP mode */
    STP_SetStpVersion(g_stp_ctx.bridge, STP_VERSION_RSTP, 0);

    /* Configure bridge parameters */
    STP_SetBridgePriority(g_stp_ctx.bridge, 0, 0x8000, 0); /* CIST priority 32768 */

    for (unsigned int i = 0; i < port_count; i++)
    {
        STP_SetPortAutoEdge(g_stp_ctx.bridge, i, true, 0);
        STP_SetAdminPointToPointMAC(g_stp_ctx.bridge, i, STP_ADMIN_P2P_AUTO, 0);
    }

    PRINTF("[STP] Initialized with %u ports\r\n", port_count);
}

void stp_freertos_start(void)
{
    xSemaphoreTake(g_stp_ctx.mutex, portMAX_DELAY);

    unsigned int timestamp = xTaskGetTickCount();

    STP_StartBridge(g_stp_ctx.bridge, timestamp);
    if (g_stp_ctx.one_second_timer != NULL)
    {
        xTimerStart(g_stp_ctx.one_second_timer, 0);
    }

    xSemaphoreGive(g_stp_ctx.mutex);

    PRINTF("[STP] Bridge started\r\n");
}

void stp_freertos_stop(void)
{
    xSemaphoreTake(g_stp_ctx.mutex, portMAX_DELAY);

    unsigned int timestamp = xTaskGetTickCount();
    STP_StopBridge(g_stp_ctx.bridge, timestamp);

    if (g_stp_ctx.one_second_timer != NULL)
    {
        xTimerStop(g_stp_ctx.one_second_timer, 0);
    }

    xSemaphoreGive(g_stp_ctx.mutex);

    PRINTF("[STP] Bridge stopped\r\n");
}

void stp_send_event(stp_event_t *event)
{
    if (xQueueSend(g_stp_ctx.event_queue, event, 0) != pdPASS)
    {
        PRINTF("[STP] Event queue full!\r\n");
        if (event->bpdu_data != NULL)
        {
            vPortFree(event->bpdu_data);
        }
    }
}

void stp_rx_handler(unsigned int port_index, uint8_t *frame, size_t frame_len)
{
    /* Validate: minimum length, BPDU dst MAC, and LLC header (DSAP=0x42, SSAP=0x42, ctrl=0x03) */
    if (frame_len >= (14U + 3U) &&
        frame[0] == 0x01 && frame[1] == 0x80 && frame[2] == 0xC2 &&
        frame[3] == 0x00 && frame[4] == 0x00 && frame[5] == 0x00 &&
        frame[14] == 0x42 && frame[15] == 0x42 && frame[16] == 0x03)
    {
        uint8_t *bpdu     = frame + 14 + 3;
        size_t bpdu_len   = frame_len - 14 - 3;

        uint8_t *bpdu_copy = pvPortMalloc(bpdu_len);
        if (bpdu_copy != NULL)
        {
            memcpy(bpdu_copy, bpdu, bpdu_len);

            stp_event_t event = {
                .type       = STP_EVENT_BPDU_RECEIVED,
                .port_index = port_index,
                .bpdu_data  = bpdu_copy,
                .bpdu_len   = bpdu_len,
            };
            stp_send_event(&event);
        }
    }
}

void stp_link_status_changed(unsigned int port_index,
                             bool link_up,
                             unsigned int speed_mbps,
                             bool full_duplex)
{
    stp_event_t event = {0};

    if (link_up)
    {
        event.type           = STP_EVENT_PORT_LINK_UP;
        event.port_index     = port_index;
        event.speed_mbps     = speed_mbps;
        event.point_to_point = full_duplex;
    }
    else
    {
        event.type       = STP_EVENT_PORT_LINK_DOWN;
        event.port_index = port_index;
    }

    stp_send_event(&event);
}

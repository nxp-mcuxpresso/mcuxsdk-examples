/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "prp_forward.h"
#include "prp_device.h"
#include "prp_types.h"
#include "prp_node.h"
#include <string.h>

static void prp_send_data(prp_priv_t *prp, uint8_t *data, uint16_t len);
void prp_announce_timer_callback(TimerHandle_t timer);

prp_priv_t *prp_create_device(uint8_t *master_mac, uint8_t *interlink_mac)
{
    uint8_t def_multicast[] = {0x01, 0x15, 0x4e, 0x00, 0x01, 0x00};
    prp_priv_t *prp;

    prp = pvPortMalloc(sizeof(prp_priv_t));
    if (!prp) {
        return NULL;
    }

    memset(prp, 0, sizeof(prp_priv_t));

    prp->ports[PRP_PT_SLAVE_A].type = PRP_PT_SLAVE_A;
    memcpy(prp->ports[PRP_PT_SLAVE_A].mac_addr, master_mac, ETH_ALEN);

    prp->ports[PRP_PT_SLAVE_B].type = PRP_PT_SLAVE_B;
    memcpy(prp->ports[PRP_PT_SLAVE_B].mac_addr, master_mac, ETH_ALEN);

    prp->ports[PRP_PT_MASTER].type = PRP_PT_MASTER;
    memcpy(prp->ports[PRP_PT_MASTER].mac_addr, master_mac, ETH_ALEN);

    if (interlink_mac) {
        prp->ports[PRP_PT_INTERLINK].type = PRP_PT_INTERLINK;
        memcpy(prp->ports[PRP_PT_INTERLINK].mac_addr, interlink_mac, ETH_ALEN);
        memcpy(prp->macaddress_redbox, interlink_mac, ETH_ALEN);
        prp->redbox = true;
    }

    prp->sequence_nr = PRP_SEQNR_START;
    prp->sup_sequence_nr = PRP_SUP_SEQNR_START;
    prp->net_id = PRP_LAN_ID;

    memcpy(prp->sup_multicast_addr, def_multicast, ETH_ALEN);

    prp->seqnr_mutex = xSemaphoreCreateMutex();
    prp->nodes_mutex = xSemaphoreCreateMutex();
    if (!prp->seqnr_mutex || !prp->nodes_mutex) {
        goto error;
    }

    prp->node_db = prp_hash_table_create();
    if (!prp->node_db) {
        PRINTF("Failed to create node_db\r\n");
        goto error;
    }

    prp->proxy_node_db = prp_hash_table_create();
    if (!prp->proxy_node_db) {
        PRINTF("Failed to create proxy_node_db\r\n");
        goto error;
    }

    prp->prp_rx = prp_forward_frame;
    prp->prp_tx = prp_send_data;

    prp->announce_timer = xTimerCreate("PRP_Announce",
                                       pdMS_TO_TICKS(PRP_ANNOUNCE_INTERVAL),
                                       pdTRUE,
                                       prp,
                                       prp_announce_timer_callback);

    prp->prune_timer = xTimerCreate("PRP_Prune",
                                    pdMS_TO_TICKS(PRP_PRUNE_PERIOD),
                                    pdTRUE,
                                    prp,
                                    prp_prune_nodes);

    if (!prp->announce_timer || !prp->prune_timer) {
        goto error;
    }

    xTimerStart(prp->announce_timer, 0);
    xTimerStart(prp->prune_timer, 0);

    return prp;

error:
    prp_destroy_device(prp);
    return NULL;
}

void prp_destroy_device(prp_priv_t *prp)
{
    if (!prp) {
        return;
    }

    if (prp->announce_timer) {
        xTimerStop(prp->announce_timer, 0);
        xTimerDelete(prp->announce_timer, 0);
    }

    if (prp->prune_timer) {
        xTimerStop(prp->prune_timer, 0);
        xTimerDelete(prp->prune_timer, 0);
    }

    if (prp->seqnr_mutex) {
        vSemaphoreDelete(prp->seqnr_mutex);
    }

    if (prp->node_db) {
        prp_hash_table_destroy(prp->node_db);
        prp->node_db = NULL;
    }
    if (prp->proxy_node_db) {
        prp_hash_table_destroy(prp->proxy_node_db);
        prp->proxy_node_db = NULL;
    }

    if (prp->nodes_mutex) {
        vSemaphoreDelete(prp->nodes_mutex);
    }

    vPortFree(prp);
}

static void prp_send_data(prp_priv_t *prp, uint8_t *data, uint16_t len)
{
    prp_frame_t frame;

    if (!prp || !data || len == 0) {
        return;
    }

    memset(&frame, 0, sizeof(frame));
    frame.data = data;
    frame.len = len;
    frame.port_rcv = PRP_PT_MASTER;
    frame.is_from_san = false;
    frame.is_supervision = false;

    xSemaphoreTake(prp->seqnr_mutex, portMAX_DELAY);
    frame.sequence_nr = prp->sequence_nr++;
    xSemaphoreGive(prp->seqnr_mutex);

    prp_forward_frame(prp, &frame);
}

/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "prp_forward.h"
#include "prp_types.h"
#include "prp_frame.h"
#include "prp_node.h"
#include <string.h>
#include "fsl_debug_console.h"

static void prp_handle_supervision_frame(prp_priv_t *prp, prp_frame_t *frame);
static void prp_handle_normal_frame(prp_priv_t *prp, prp_frame_t *frame);
static uint8_t* prp_get_source_mac_from_supervision(prp_frame_t *frame);
static void prp_forward_by_destination(prp_priv_t *prp, prp_frame_t *frame,
				       prp_node_t *snode, node_type_t dst_type);
static bool prp_is_local_mac(prp_priv_t *prp, uint8_t *mac);
static void prp_broadcast_frame(prp_priv_t *prp, prp_frame_t *frame,
				prp_node_t *snode);
static void prp_forward_to_port(prp_priv_t *prp, prp_node_t *snode,
				prp_frame_t *frame, prp_port_type_t port_type,
				bool need_tag);
static void prp_deliver_to_master(prp_priv_t *prp, prp_frame_t *frame);
static void prp_send_frame(prp_priv_t *prp, prp_port_type_t port_type,
                           prp_frame_t *frame);

static void prp_forward_do(prp_priv_t *prp, prp_frame_t *frame)
{
    if (!prp || !frame) {
        return;
    }

    if (frame->is_supervision) {
        prp_handle_supervision_frame(prp, frame);
    } else {
        prp_handle_normal_frame(prp, frame);
    }
}

static void prp_handle_supervision_frame(prp_priv_t *prp, prp_frame_t *frame)
{
    uint8_t *source_mac;
    prp_node_t *node;

    source_mac = prp_get_source_mac_from_supervision(frame);
    if (!source_mac) {
        return;
    }

    if (!prp->node_db) {
        return;
    }

    xSemaphoreTake(prp->nodes_mutex, portMAX_DELAY);

    node = prp_find_node_by_mac(prp->node_db, source_mac);
    if (node) {
        node->node_type = NODE_TYPE_DAN;
        node->time_in[frame->port_rcv] = xTaskGetTickCount();
        node->time_in_stale[frame->port_rcv] = false;
    } else {
        node = prp_create_node(source_mac, NODE_TYPE_DAN);
        if (node) {
            node->time_in[frame->port_rcv] = xTaskGetTickCount();
            node->time_in_stale[frame->port_rcv] = false;
            if (!prp_hash_table_add(prp->node_db, node)) {
                vPortFree(node);
                node = NULL;
            }
        }
    }
    xSemaphoreGive(prp->nodes_mutex);
}

static void prp_handle_normal_frame(prp_priv_t *prp, prp_frame_t *frame)
{
    uint8_t *dmac = frame->mac;
    uint8_t *smac = &frame->mac[6];
    prp_node_t *snode = NULL, *dnode = NULL;
    node_type_t dst_type;

    if (!prp->node_db || !prp->proxy_node_db) {
        return;
    }

    xSemaphoreTake(prp->nodes_mutex, portMAX_DELAY);

    if (frame->port_rcv == PRP_PT_INTERLINK) {
	snode = prp_find_node_by_mac(prp->proxy_node_db, smac);
	dnode = prp_find_node_by_mac(prp->node_db, dmac);
    } else if (frame->port_rcv == PRP_PT_SLAVE_A || frame->port_rcv == PRP_PT_SLAVE_B) {
	snode = prp_find_node_by_mac(prp->node_db, smac);
	dnode = prp_find_node_by_mac(prp->proxy_node_db, dmac);
    } else {
	dnode = prp_find_node_by_mac(prp->node_db, dmac);
	if (!dnode)
	    dnode = prp_find_node_by_mac(prp->proxy_node_db, dmac);
    }

    if (dnode)
        dst_type = dnode->node_type;
    else
        dst_type = NODE_TYPE_NONE;

    if (!snode) {
        if (frame->port_rcv == PRP_PT_SLAVE_A || frame->port_rcv == PRP_PT_SLAVE_B) {
            node_type_t node_type = (frame->port_rcv == PRP_PT_SLAVE_A) ?
                                    NODE_TYPE_SAN_A : NODE_TYPE_SAN_B;
            snode = prp_create_node(smac, node_type);

            if (snode) {
                snode->san_a = (frame->port_rcv == PRP_PT_SLAVE_A);
                snode->san_b = (frame->port_rcv == PRP_PT_SLAVE_B);
                snode->time_in[frame->port_rcv] = xTaskGetTickCount();
                snode->time_in_stale[frame->port_rcv] = false;

                if (!prp_hash_table_add(prp->node_db, snode)) {
                    vPortFree(snode);
                    snode = NULL;
                }
            }
        } else if (frame->port_rcv == PRP_PT_INTERLINK) {
            snode = prp_create_node(smac, NODE_TYPE_SAN);
            if (snode) {
                snode->time_in[frame->port_rcv] = xTaskGetTickCount();
                snode->time_in_stale[frame->port_rcv] = false;

                if (!prp_hash_table_add(prp->proxy_node_db, snode)) {
                    vPortFree(snode);
                    snode = NULL;
                }
            }
        }
    } else if (frame->port_rcv != PRP_PT_INTERLINK) {
        if (frame->port_rcv == PRP_PT_SLAVE_A && snode->node_type == NODE_TYPE_SAN_B) {
            snode->node_type = NODE_TYPE_SAN_AB;
            snode->san_a = true;
        } else if (frame->port_rcv == PRP_PT_SLAVE_B && snode->node_type == NODE_TYPE_SAN_A) {
            snode->node_type = NODE_TYPE_SAN_AB;
            snode->san_b = true;
        }

        snode->time_in[frame->port_rcv] = xTaskGetTickCount();
        snode->time_in_stale[frame->port_rcv] = false;
    } else {
        snode->time_in[frame->port_rcv] = xTaskGetTickCount();
        snode->time_in_stale[frame->port_rcv] = false;
    }

    if (snode)
        snode->used = 1;

    xSemaphoreGive(prp->nodes_mutex);

    prp_forward_by_destination(prp, frame, snode, dst_type);

    xSemaphoreTake(prp->nodes_mutex, portMAX_DELAY);
    if (snode)
        snode->used = 0;
    xSemaphoreGive(prp->nodes_mutex);
}

static void prp_forward_by_destination(prp_priv_t *prp, prp_frame_t *frame,
				       prp_node_t *snode, node_type_t dst_type)
{
    if (prp_is_local_mac(prp, frame->mac)) {
        prp_forward_to_port(prp, snode, frame, PRP_PT_MASTER, false);
        return;
    }

    if (dst_type == NODE_TYPE_NONE) {
        prp_broadcast_frame(prp, frame, snode);
    } else if (dst_type == NODE_TYPE_SAN) {
        prp_forward_to_port(prp, snode, frame, PRP_PT_INTERLINK, false);
    } else {
        switch (dst_type) {
            case NODE_TYPE_SAN_A:
                prp_forward_to_port(prp, snode, frame, PRP_PT_SLAVE_A, false);
                break;

            case NODE_TYPE_SAN_B:
                prp_forward_to_port(prp, snode, frame, PRP_PT_SLAVE_B, false);
                break;

            case NODE_TYPE_SAN_AB:
            case NODE_TYPE_DAN:
            default:
                if (frame->port_rcv != PRP_PT_SLAVE_A) {
                    prp_forward_to_port(prp, snode, frame, PRP_PT_SLAVE_A, true);
                }
                if (frame->port_rcv != PRP_PT_SLAVE_B) {
                    prp_forward_to_port(prp, snode, frame, PRP_PT_SLAVE_B, true);
                }
                break;
        }
    }
}

static void prp_forward_to_port(prp_priv_t *prp, prp_node_t *snode,
				prp_frame_t *frame, prp_port_type_t port_type,
				bool need_tag)
{
    uint16_t frame_len = 0;
    uint8_t *frame_data = NULL;

    if (prp_drop_frame(frame, port_type)) {
        return;
    }

    if (port_type != PRP_PT_SLAVE_A && port_type != PRP_PT_SLAVE_B &&
        !frame->is_from_san && snode &&
        prp_register_frame_out(snode, port_type, frame->sequence_nr)) {
        return;
    }

    if (need_tag) {
        frame_data = prp_create_tagged_frame(prp, frame, port_type, &frame_len);
    } else {
        frame_data = prp_get_untagged_frame(frame, &frame_len);
    }

    if (frame_data) {
        if (port_type == PRP_PT_MASTER) {
            prp_deliver_to_master(prp, frame);
        } else {
            prp_send_frame(prp, port_type, frame);
        }
    }
}

static void prp_broadcast_frame(prp_priv_t *prp, prp_frame_t *frame, prp_node_t *snode)
{
    for (prp_port_type_t port_type = PRP_PT_SLAVE_A; port_type < PRP_PT_PORTS; port_type++) {
        if (prp->ports[port_type].type == PRP_PT_NONE) {
            continue;
        }

        if (port_type == frame->port_rcv) {
            continue;
        }

        if ((frame->port_rcv == PRP_PT_SLAVE_A && port_type == PRP_PT_SLAVE_B) ||
            (frame->port_rcv == PRP_PT_SLAVE_B && port_type == PRP_PT_SLAVE_A)) {
            continue;
        }

        if (port_type == PRP_PT_SLAVE_A || port_type == PRP_PT_SLAVE_B) {
            prp_forward_to_port(prp, snode, frame, port_type, true);
        } else {
            prp_forward_to_port(prp, snode, frame, port_type, false);
        }
    }
}

static uint8_t* prp_get_source_mac_from_supervision(prp_frame_t *frame)
{
    eth_hdr_t *eth_hdr = (eth_hdr_t *)frame->data;
    uint16_t eth_hdr_len = sizeof(eth_hdr_t);
    uint8_t *mac = NULL;

    if (eth_hdr->h_proto == htons(ETH_P_8021Q)) {
        eth_hdr_len += VLAN_HLEN;
    }

    if (frame->len < eth_hdr_len + sizeof(prp_sup_tag_t)) {
        return NULL;
    }

    prp_sup_tag_t *sup_tag = (prp_sup_tag_t *)(frame->data + eth_hdr_len);

    if (sup_tag->tlv_mac.tlv.tlv_type == PRP_TLV_LIFE_CHECK_DD)
        mac = sup_tag->tlv_mac.macaddress;

    return mac;
}

static bool prp_is_local_mac(prp_priv_t *prp, uint8_t *mac)
{
    for (int i = 0; i < PRP_PT_PORTS; i++) {
        if (prp->ports[i].type != PRP_PT_NONE &&
            memcmp(prp->ports[i].mac_addr, mac, ETH_ALEN) == 0) {
            return true;
        }
    }

    if (memcmp(prp->macaddress_redbox, mac, ETH_ALEN) == 0) {
        return true;
    }

    return false;
}

void prp_forward_frame(prp_priv_t *prp, prp_frame_t *frame)
{
    if (!prp || !frame) {
        return;
    }

    prp_forward_do(prp, frame);
}

static void prp_deliver_to_master(prp_priv_t *prp, prp_frame_t *frame)
{
    if (prp->master_rx_callback) {
        prp->master_rx_callback(frame->data, frame->len);
    }
}

static void prp_send_frame(prp_priv_t *prp, prp_port_type_t port_type,
                           prp_frame_t *frame)
{
    int result;

    if (!prp->ports[port_type].send_frame) {
        return;
    }

    result = prp->ports[port_type].send_frame(&prp->ports[port_type],
                                              frame->data, frame->len);

    if (result)
        PRINTF("Send failed, buffer will be freed by rx_task\r\n");
}

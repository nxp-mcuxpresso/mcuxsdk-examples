/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "prp_types.h"
#include "prp_frame.h"
#include <string.h>
#include "fsl_debug_console.h"

#define SUP_BUFFER_SIZE 256
#define PRP_SUP_VERSION 45

static void prp_send_supervision_frame(prp_priv_t *prp, uint8_t *node_mac)
{
    uint8_t *sup_frame;
    eth_hdr_t *eth_hdr;
    prp_sup_payload_t *sup_payload;
    prp_port_type_t port_type;
    uint16_t sup_len;
    prp_frame_t frame;
    uint8_t *tagged_data;
    uint16_t tagged_len;

    if (!prp || !node_mac) {
        PRINTF("ERROR: Invalid parameters for supervision frame\r\n");
        return;
    }

    sup_frame = prp->supervision_buffer;
    if (sup_frame == NULL) {
        PRINTF("ERROR: No supervision buffer allocated\r\n");
        return;
    }

    sup_len = sizeof(eth_hdr_t) + sizeof(prp_sup_payload_t);
    memset(sup_frame, 0, sup_len);

    eth_hdr = (eth_hdr_t *)sup_frame;

    memcpy(eth_hdr->h_dest, prp->sup_multicast_addr, ETH_ALEN);
    memcpy(eth_hdr->h_source, prp->ports[PRP_PT_SLAVE_A].mac_addr, ETH_ALEN);
    eth_hdr->h_proto = htons(ETH_P_PRP);

    sup_payload = (prp_sup_payload_t *)(sup_frame + sizeof(eth_hdr_t));
    sup_payload->sup_tag.sup_hdr.path_version = htons(PRP_SUP_VERSION);

    xSemaphoreTake(prp->seqnr_mutex, portMAX_DELAY);
    sup_payload->sup_tag.sup_hdr.sequence_nr = htons(prp->sup_sequence_nr);
    prp->sup_sequence_nr++;
    xSemaphoreGive(prp->seqnr_mutex);

    sup_payload->sup_tag.tlv_mac.tlv.tlv_type = PRP_TLV_LIFE_CHECK_DD;
    sup_payload->sup_tag.tlv_mac.tlv.tlv_length = ETH_ALEN;
    memcpy(sup_payload->sup_tag.tlv_mac.macaddress, node_mac, ETH_ALEN);

    sup_len -= sizeof(prp_rct_t);

    if (prp->redbox) {
        sup_payload->sup_tag.tlv_redbox.tlv.tlv_type = PRP_TLV_REDBOX_MAC;
        sup_payload->sup_tag.tlv_redbox.tlv.tlv_length = ETH_ALEN;
        memcpy(sup_payload->sup_tag.tlv_redbox.redbox_mac,
               prp->macaddress_redbox, ETH_ALEN);

        sup_payload->sup_tag.tlv_end.tlv.tlv_type = 0;
        sup_payload->sup_tag.tlv_end.tlv.tlv_length = 0;

    } else {
        sup_payload->sup_tag.tlv_redbox.tlv.tlv_type = 0;
        sup_payload->sup_tag.tlv_redbox.tlv.tlv_length = 0;
        sup_len -= sizeof(prp_sup_tlv_end_t);
    }

    memset(&frame, 0, sizeof(frame));
    frame.buffer = sup_frame;
    frame.data = sup_frame;
    frame.buffer_size = SUP_BUFFER_SIZE;
    frame.len = sup_len;
    frame.port_rcv = PRP_PT_MASTER;
    frame.is_supervision = true;
    frame.sequence_nr = ntohs(sup_payload->sup_tag.sup_hdr.sequence_nr);
    frame.rct_position = (uint8_t *)&sup_payload->rct;

    for (port_type = PRP_PT_SLAVE_A; port_type <= PRP_PT_SLAVE_B;  port_type++) {
        tagged_data = prp_create_tagged_frame(prp, &frame, port_type, &tagged_len);
        if (tagged_data)
            if (prp->ports[port_type].send_frame)
                prp->ports[port_type].send_frame(&prp->ports[port_type], 
                                                 sup_frame, tagged_len);
    }
}

void prp_announce_timer_callback(TimerHandle_t timer)
{
    prp_priv_t *prp = (prp_priv_t *)pvTimerGetTimerID(timer);

    prp_send_supervision_frame(prp, prp->ports[PRP_PT_MASTER].mac_addr);
}


/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "prp_frame.h"
#include "prp_types.h"
#include <string.h>
#include "fsl_debug_console.h"

static prp_rct_t *prp_get_rct(uint8_t *frame_data, uint16_t frame_len)
{
    if (frame_len < ETH_HLEN + PRP_HLEN) {
        return NULL;
    }

    prp_rct_t *rct = (prp_rct_t *)(frame_data + frame_len - PRP_HLEN);

    if (rct->suffix != htons(ETH_P_PRP)) {
        return NULL;
    }

    return rct;
}

static uint16_t prp_get_sequence_nr(prp_rct_t *rct)
{
    return ntohs(rct->sequence_nr);
}

static void prp_set_lan_id(prp_rct_t *rct, uint8_t lan_id)
{
    uint16_t lsdu_size = ntohs(rct->lsdu_size);
    lsdu_size = (lsdu_size & 0x0FFF) | ((uint16_t)lan_id << 12);
    rct->lsdu_size = htons(lsdu_size);
}

static void prp_set_lsdu_size(prp_rct_t *rct, uint16_t size)
{
    uint16_t lsdu_size = ntohs(rct->lsdu_size);
    lsdu_size = (lsdu_size & 0xF000) | (size & 0x0FFF);
    rct->lsdu_size = htons(lsdu_size);
}

bool prp_fill_frame_info(prp_priv_t *prp, prp_frame_t *frame,
                         uint16_t frame_len, prp_port_type_t port_type)
{
    prp_rct_t *rct = prp_get_rct(frame->data, frame_len);

    frame->len = frame_len;
    frame->port_rcv = port_type;
    frame->is_from_san = false;
    memcpy(frame->mac, frame->data, 12);

    if (memcmp(frame->mac, prp->sup_multicast_addr, ETH_ALEN - 1) == 0)
        frame->is_supervision = 1;

    if (rct) {
        frame->sequence_nr = prp_get_sequence_nr(rct);
        frame->rct_position = (uint8_t *)rct;
    } else {
        frame->rct_position = NULL;
    }

    if (port_type != PRP_PT_MASTER && port_type != PRP_PT_INTERLINK && !rct) {
        frame->is_from_san = true;
    }

    if (port_type == PRP_PT_MASTER || port_type == PRP_PT_INTERLINK) {
        xSemaphoreTake(prp->seqnr_mutex, portMAX_DELAY);
        frame->sequence_nr = prp->sequence_nr++;
        xSemaphoreGive(prp->seqnr_mutex);
    }

    return true;
}

uint8_t *prp_create_tagged_frame(prp_priv_t *prp, prp_frame_t *frame,
                                 prp_port_type_t port_type, uint16_t *out_len)
{
    uint16_t original_len;
    uint16_t tagged_len;
    eth_hdr_t *eth_hdr;
    uint16_t lsdu_size;
    prp_rct_t *rct;
    uint8_t lan_id;

    rct = (prp_rct_t *)frame->rct_position;

    if (rct && rct->suffix == htons(ETH_P_PRP)) {
        lan_id = (port_type == PRP_PT_SLAVE_A) ? 0 : 1;
        lan_id |= prp->net_id;
        prp_set_lan_id(rct, lan_id);

        *out_len = frame->len;

	return frame->data;
    }

    original_len = frame->len;
    tagged_len = original_len + PRP_HLEN;

    if (tagged_len < ETH_ZLEN + PRP_HLEN) {
        tagged_len = ETH_ZLEN + PRP_HLEN;
    }

    if (frame->buffer_size < tagged_len) {
        PRINTF("ERROR: Buffer overflow risk, need %u bytes, have %u bytes\r\n",
               tagged_len, frame->buffer_size);
	return NULL;
    }

    if (original_len < ETH_ZLEN) {
        memset(frame->data + original_len, 0, ETH_ZLEN - original_len);
    }

    rct = (prp_rct_t *)(frame->data + tagged_len - PRP_HLEN);

    eth_hdr = (eth_hdr_t *)frame->data;
    lsdu_size = tagged_len - ETH_HLEN;
    if (eth_hdr->h_proto == htons(ETH_P_8021Q)) {
        lsdu_size -= VLAN_HLEN;
    }

    lan_id = (port_type == PRP_PT_SLAVE_A) ? 0 : 1;
    lan_id |= prp->net_id;

    prp_set_lan_id(rct, lan_id);
    prp_set_lsdu_size(rct, lsdu_size);
    rct->sequence_nr = htons(frame->sequence_nr);
    rct->suffix = htons(ETH_P_PRP);

    frame->len = tagged_len;
    frame->rct_position = (uint8_t *)rct;

    *out_len = tagged_len;

    return frame->data;
}

uint8_t *prp_get_untagged_frame(prp_frame_t *frame, uint16_t *out_len)
{
    prp_rct_t *rct = (prp_rct_t *)frame->rct_position;
    uint16_t untagged_len;

    if (!rct || rct->suffix != htons(ETH_P_PRP)) {
        *out_len = frame->len;
        return frame->data;
    }

    untagged_len = frame->len - PRP_HLEN;

    frame->len = untagged_len;
    frame->rct_position = NULL;

    *out_len = untagged_len;

    return frame->data;
}

bool prp_drop_frame(prp_frame_t *frame, prp_port_type_t out_port)
{
    return ((frame->port_rcv == PRP_PT_SLAVE_A && out_port == PRP_PT_SLAVE_B) ||
            (frame->port_rcv == PRP_PT_SLAVE_B && out_port == PRP_PT_SLAVE_A));
}


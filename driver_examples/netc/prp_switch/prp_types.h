/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PRP_TYPES_H
#define PRP_TYPES_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include <stdint.h>
#include <stdbool.h>

#define ETH_ALEN            6
#define ETH_HLEN            14
#define ETH_ZLEN            60
#define VLAN_HLEN           4
#define PRP_HLEN            6

#define ETH_P_PRP           0x88FB
#define ETH_P_8021Q         0x8100

#define PRP_LAN_ID          0x0A
#define PRP_SEQNR_START     1
#define PRP_SUP_SEQNR_START 1

#define PRP_LIFE_CHECK_INTERVAL     2000  // ms
#define PRP_ANNOUNCE_INTERVAL       2000   // ms
#define PRP_NODE_FORGET_TIME        60000 // ms
#define PRP_PRUNE_PERIOD            10000  // ms

#define PRP_TLV_LIFE_CHECK_DD       20
#define PRP_TLV_LIFE_CHECK_DA       21
#define PRP_TLV_ANNOUNCE            22
#define PRP_TLV_REDBOX_MAC          30

#define PRP_HASH_TABLE_SIZE         256
#define PRP_HASH_TABLE_MASK         (PRP_HASH_TABLE_SIZE - 1)

#define PRP_MAX_NODE_NUM            1024

#ifndef htons
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define htons(x) ((uint16_t)((((x) & 0x00ff) << 8) | (((x) & 0xff00) >> 8)))
#define ntohs(x) htons(x)
#define htonl(x) ((uint32_t)((((x) & 0x000000ff) << 24) | \
                             (((x) & 0x0000ff00) << 8) | \
                             (((x) & 0x00ff0000) >> 8) | \
                             (((x) & 0xff000000) >> 24)))
#define ntohl(x) htonl(x)
#else
#define htons(x) (x)
#define ntohs(x) (x)
#define htonl(x) (x)
#define ntohl(x) (x)
#endif
#endif

typedef enum {
    PRP_PT_NONE = 0,
    PRP_PT_SLAVE_A,
    PRP_PT_SLAVE_B,
    PRP_PT_MASTER,
    PRP_PT_INTERLINK,
    PRP_PT_PORTS
} prp_port_type_t;

typedef enum {
    NODE_TYPE_NONE = 0,
    NODE_TYPE_SAN_A,
    NODE_TYPE_SAN_B,
    NODE_TYPE_SAN_AB,
    NODE_TYPE_DAN,
    NODE_TYPE_SAN
} node_type_t;

typedef struct {
    uint8_t h_dest[ETH_ALEN];
    uint8_t h_source[ETH_ALEN];
    uint16_t h_proto;
} __attribute__((packed)) eth_hdr_t;

typedef struct {
    uint16_t sequence_nr;
    uint16_t lsdu_size;
    uint16_t suffix;
} __attribute__((packed)) prp_rct_t;

typedef struct {
    uint16_t path_version;
    uint16_t sequence_nr;
} __attribute__((packed)) prp_sup_hdr_t;

typedef struct {
    uint8_t tlv_type;
    uint8_t tlv_length;
} __attribute__((packed)) prp_sup_tlv_t;

typedef struct {
    prp_sup_tlv_t tlv;
    uint8_t macaddress[ETH_ALEN];
} __attribute__((packed)) prp_sup_tlv_mac_t;

typedef struct {
    prp_sup_tlv_t tlv;
    uint8_t redbox_mac[ETH_ALEN];
} __attribute__((packed)) prp_sup_tlv_redbox_t;

typedef struct {
    prp_sup_tlv_t tlv;
} __attribute__((packed)) prp_sup_tlv_end_t;

typedef struct {
    prp_sup_hdr_t sup_hdr;
    prp_sup_tlv_mac_t tlv_mac;
    prp_sup_tlv_redbox_t tlv_redbox;
    prp_sup_tlv_end_t tlv_end;
} __attribute__((packed)) prp_sup_tag_t;

typedef struct {
    prp_sup_tag_t sup_tag;
    uint8_t padding[34];
    prp_rct_t rct;
} __attribute__((packed)) prp_sup_payload_t;

typedef struct prp_frame {
    uint8_t *buffer;
    uint8_t *data;
    uint16_t buffer_size;
    uint16_t len;
    uint16_t sequence_nr;
    bool is_supervision;
    bool is_from_san;
    prp_port_type_t port_rcv;
    uint8_t mac[12];
    uint8_t *rct_position;
    struct prp_frame *next;
} prp_frame_t;

typedef struct prp_node {
    struct prp_node *next;
    struct prp_node *list_next;
    uint8_t macaddress_A[ETH_ALEN];
    uint8_t macaddress_B[ETH_ALEN];
    uint16_t seq_out[PRP_PT_PORTS];
    uint64_t seq_window[PRP_PT_PORTS];
    TickType_t time_in[PRP_PT_PORTS];
    TickType_t time_out[PRP_PT_PORTS];
    bool time_in_stale[PRP_PT_PORTS];
    bool san_a;
    bool san_b;
    bool used;
    bool removed;
    node_type_t node_type;
    prp_port_type_t addr_B_port;
} prp_node_t;

typedef struct {
    prp_node_t *buckets[PRP_HASH_TABLE_SIZE];
    prp_node_t *list_head;
    uint32_t node_count;
} prp_hash_table_t;

typedef struct prp_port {
    prp_port_type_t type;
    uint8_t portid;
    uint8_t mac_addr[ETH_ALEN];
    bool is_up;
    int (*send_frame)(struct prp_port *port, uint8_t *data, uint16_t len);
    int (*recv_frame)(struct prp_port *port, uint8_t *data, uint16_t *len);
} prp_port_t;

typedef struct prp_priv prp_priv_t;
struct prp_priv {
    prp_port_t ports[PRP_PT_PORTS];
    prp_hash_table_t *node_db;
    prp_hash_table_t *proxy_node_db;
    SemaphoreHandle_t nodes_mutex;

    uint16_t sequence_nr;
    uint16_t sup_sequence_nr;
    uint8_t net_id;
    uint8_t sup_multicast_addr[ETH_ALEN];
    uint8_t macaddress_redbox[ETH_ALEN];
    uint8_t *supervision_buffer;
    bool redbox;
    bool fwd_offloaded;

    void (*master_rx_callback)(uint8_t *data, uint16_t len);
    void (*prp_rx)(prp_priv_t *prp, prp_frame_t *frame);
    void (*prp_tx)(prp_priv_t *prp, uint8_t *data, uint16_t len, uint16_t size);

    SemaphoreHandle_t seqnr_mutex;

    TimerHandle_t announce_timer;
    TimerHandle_t prune_timer;
    TimerHandle_t announce_proxy_timer;
};

#endif // PRP_TYPES_H


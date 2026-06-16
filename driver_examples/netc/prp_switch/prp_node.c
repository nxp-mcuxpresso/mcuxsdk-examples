/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "prp_node.h"
#include "prp_types.h"
#include <string.h>
#include "fsl_debug_console.h"

#define PRP_SEQ_WINDOW_SIZE 64

static uint32_t prp_hash_mac(const uint8_t *mac)
{
    uint32_t hash = 0;

    for (int i = 0; i < 6; i++) {
        hash ^= (uint32_t)mac[i];
        hash = (hash << 5) | (hash >> 27);
    }

    return hash & PRP_HASH_TABLE_MASK;
}

prp_hash_table_t *prp_hash_table_create(void)
{
    prp_hash_table_t *table;

    table = pvPortMalloc(sizeof(prp_hash_table_t));
    if (!table) {
        PRINTF("Failed to allocate hash table\r\n");
        return NULL;
    }

    memset(table, 0, sizeof(prp_hash_table_t));

    PRINTF("Hash table created successfully\r\n");
    return table;
}

void prp_hash_table_destroy(prp_hash_table_t *table)
{
    if (!table) {
        return;
    }

    prp_node_t *node = table->list_head;
    while (node) {
        prp_node_t *next = node->list_next;
        vPortFree(node);
        node = next;
    }

    vPortFree(table);

    PRINTF("Hash table destroyed\r\n");
}

prp_node_t *prp_find_node_by_mac(prp_hash_table_t *table, uint8_t *mac)
{
    if (!table || !mac) {
        return NULL;
    }

    uint32_t hash = prp_hash_mac(mac);
    prp_node_t *node = table->buckets[hash];

    while (node) {
        if (memcmp(node->macaddress_A, mac, ETH_ALEN) == 0) {
            return node;
        }
        node = node->next;
    }

    return NULL;
}

bool prp_hash_table_add(prp_hash_table_t *table, prp_node_t *node)
{
    if (!table || !node) {
        return false;
    }

    if (table->node_count >= PRP_MAX_NODE_NUM)
        return false;

    uint32_t hash = prp_hash_mac(node->macaddress_A);

    node->next = table->buckets[hash];
    table->buckets[hash] = node;
    node->list_next = table->list_head;
    table->list_head = node;
    table->node_count++;

    return true;
}

static bool prp_hash_table_remove(prp_hash_table_t *table, uint8_t *mac)
{
    if (!table || !mac) {
        return false;
    }

    uint32_t hash = prp_hash_mac(mac);
    prp_node_t *node = table->buckets[hash];
    prp_node_t *prev = NULL;

    while (node) {
        if (memcmp(node->macaddress_A, mac, ETH_ALEN) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->buckets[hash] = node->next;
            }
            break;
        }
        prev = node;
        node = node->next;
    }

    if (!node) {
        return false;
    }

    prp_node_t *list_node = table->list_head;
    prp_node_t *list_prev = NULL;

    while (list_node) {
        if (list_node == node) {
            if (list_prev) {
                list_prev->list_next = node->list_next;
            } else {
                table->list_head = node->list_next;
            }
            break;
        }
        list_prev = list_node;
        list_node = list_node->list_next;
    }

    table->node_count--;

    vPortFree(node);

    return true;
}

prp_node_t* prp_create_node(uint8_t *mac, node_type_t node_type)
{
    prp_node_t *new_node;
    TickType_t now;

    new_node = pvPortMalloc(sizeof(prp_node_t));
    if (!new_node) {
        PRINTF("prp create node failed\r\n");
        return NULL;
    }

    memset(new_node, 0, sizeof(prp_node_t));
    memcpy(new_node->macaddress_A, mac, ETH_ALEN);
    new_node->node_type = node_type;

    now = xTaskGetTickCount();
    for (int i = 0; i < PRP_PT_PORTS; i++) {
        new_node->time_in[i] = now;
        new_node->seq_out[i] = PRP_SEQNR_START;
        new_node->time_in_stale[i] = true;
    }

    switch (node_type) {
        case NODE_TYPE_SAN_A:
            new_node->san_a = true;
            new_node->san_b = false;
            break;
        case NODE_TYPE_SAN_B:
            new_node->san_a = false;
            new_node->san_b = true;
            break;
        case NODE_TYPE_SAN_AB:
            new_node->san_a = true;
            new_node->san_b = true;
            break;
        case NODE_TYPE_DAN:
        default:
            new_node->san_a = false;
            new_node->san_b = false;
            break;
    }

    new_node->next = NULL;
    new_node->list_next = NULL;
    new_node->removed = false;

    return new_node;
}

bool prp_register_frame_out(prp_node_t *node, prp_port_type_t port_type,
                            uint16_t sequence_nr)
{
    if (!node || port_type >= PRP_PT_PORTS) {
        return false;
    }

    uint16_t last_seq = node->seq_out[port_type];
    bool duplicate = false;
    uint64_t bit_value;
    int32_t diff;

    TickType_t now = xTaskGetTickCount();
    if (!node->time_out[port_type] ||
	(now - node->time_out[port_type]) >= pdMS_TO_TICKS(PRP_NODE_FORGET_TIME)) {
        node->seq_out[port_type] = sequence_nr;
        node->time_out[port_type] = now;
	node->seq_window[port_type] = 1ULL;
        duplicate = false;

	return duplicate;
    }

    diff = (int32_t)sequence_nr - (int32_t)last_seq;
    if (diff > 32768)
        diff -= 65536;
    else if (diff < -32768)
        diff += 65536;

    if (diff == 0) {
	duplicate = true;
    } else if (diff > 0) {
        if (diff >= PRP_SEQ_WINDOW_SIZE) {
            node->seq_out[port_type] = sequence_nr;
            node->time_out[port_type] = now;
            node->seq_window[port_type] = 1ULL;
            duplicate = false;
	} else {
	    node->seq_window[port_type] = (node->seq_window[port_type] << diff) | 1ULL;
	    node->seq_out[port_type] = sequence_nr;
            node->time_out[port_type] = now;
	    duplicate = false;
	}
    } else {
	diff = -diff;
	if (diff < PRP_SEQ_WINDOW_SIZE) {
	    bit_value = 1ULL << diff;
	    if (node->seq_window[port_type] & bit_value)
	        duplicate = true;
	    else {
	        node->seq_window[port_type] |= bit_value;
	        duplicate = false;
	    }
	} else {
	    duplicate = true;
	}
    }

    return duplicate;
}

static void prp_prune_hash_table(prp_hash_table_t *table, TickType_t now)
{
    if (!table) {
        return;
    }

    prp_node_t *node = table->list_head;
    prp_node_t *prev = NULL;

    while (node) {
        prp_node_t *next = node->list_next;
        bool should_remove = true;

        for (int i = 0; i < PRP_PT_PORTS; i++) {
            if (node->used ||
		(!node->time_in_stale[i] &&
                 (now - node->time_in[i]) <= pdMS_TO_TICKS(PRP_NODE_FORGET_TIME))) {
                should_remove = false;
                break;
            }
        }

        if (should_remove) {
            uint32_t hash = prp_hash_mac(node->macaddress_A);
            prp_node_t *bucket_node = table->buckets[hash];
            prp_node_t *bucket_prev = NULL;

            while (bucket_node) {
                if (bucket_node == node) {
                    if (bucket_prev) {
                        bucket_prev->next = node->next;
                    } else {
                        table->buckets[hash] = node->next;
                    }
                    break;
                }
                bucket_prev = bucket_node;
                bucket_node = bucket_node->next;
            }

            if (prev) {
                prev->list_next = next;
            } else {
                table->list_head = next;
            }

            table->node_count--;

            vPortFree(node);
        } else {
            prev = node;
        }

        node = next;
    }
}

void prp_prune_nodes(TimerHandle_t timer)
{
    prp_priv_t *prp = (prp_priv_t *)pvTimerGetTimerID(timer);
    TickType_t now = xTaskGetTickCount();

    if (!prp) {
        return;
    }

    xSemaphoreTake(prp->nodes_mutex, portMAX_DELAY);

    if (prp->node_db) {
        prp_prune_hash_table(prp->node_db, now);
    }

    if (prp->proxy_node_db) {
        prp_prune_hash_table(prp->proxy_node_db, now);
    }

    xSemaphoreGive(prp->nodes_mutex);
}

void prp_remove_node(prp_hash_table_t *table, uint8_t *mac)
{
    if (!table || !mac) {
        return;
    }

    prp_hash_table_remove(table, mac);
}

void prp_clear_node_db(prp_hash_table_t *table)
{
    if (!table) {
        return;
    }

    prp_node_t *node = table->list_head;
    while (node) {
        prp_node_t *next = node->list_next;
        vPortFree(node);
        node = next;
    }

    memset(table->buckets, 0, sizeof(table->buckets));
    table->list_head = NULL;
    table->node_count = 0;
}

void prp_print_node_info(prp_node_t *node)
{
    if (!node) {
        return;
    }

    PRINTF("Node MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           node->macaddress_A[0], node->macaddress_A[1], node->macaddress_A[2],
           node->macaddress_A[3], node->macaddress_A[4], node->macaddress_A[5]);

    PRINTF("Node Type: ");
    switch (node->node_type) {
        case NODE_TYPE_SAN_A:
            PRINTF("SAN_A\n");
            break;
        case NODE_TYPE_SAN_B:
            PRINTF("SAN_B\n");
            break;
        case NODE_TYPE_SAN_AB:
            PRINTF("SAN_AB\n");
            break;
        case NODE_TYPE_DAN:
            PRINTF("DAN\n");
            break;
        case NODE_TYPE_SAN:
            PRINTF("SAN\n");
            break;
        default:
            PRINTF("UNKNOWN\n");
            break;
    }

    PRINTF("SAN_A: %s, SAN_B: %s\n",
           node->san_a ? "true" : "false",
           node->san_b ? "true" : "false");
}

void prp_print_hash_table_stats(prp_hash_table_t *table)
{
    if (!table) {
        return;
    }

    PRINTF("=== Hash Table Statistics ===\n");
    PRINTF("Total nodes: %u\n", table->node_count);

    uint32_t used_buckets = 0;
    uint32_t max_chain_len = 0;

    for (int i = 0; i < PRP_HASH_TABLE_SIZE; i++) {
        if (table->buckets[i]) {
            used_buckets++;

            uint32_t chain_len = 0;
            prp_node_t *node = table->buckets[i];
            while (node) {
                chain_len++;
                node = node->next;
            }

            if (chain_len > max_chain_len) {
                max_chain_len = chain_len;
            }
        }
    }

    PRINTF("Used buckets: %u / %d\n", used_buckets, PRP_HASH_TABLE_SIZE);
    PRINTF("Max chain length: %u\n", max_chain_len);
}


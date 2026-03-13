/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PRP_NODE_H
#define PRP_NODE_H

#include "prp_types.h"

prp_hash_table_t *prp_hash_table_create(void);
prp_node_t* prp_find_node_by_mac(prp_hash_table_t *table, uint8_t *mac);
bool prp_hash_table_add(prp_hash_table_t *table, prp_node_t *node);
prp_node_t* prp_create_node(uint8_t *mac, node_type_t node_type);
bool prp_register_frame_out(prp_node_t *node, prp_port_type_t port_type,
                            uint16_t sequence_nr);
void prp_prune_nodes(TimerHandle_t timer);
void prp_hash_table_destroy(prp_hash_table_t *table);
void prp_remove_node(prp_hash_table_t *table, uint8_t *mac);
void prp_clear_node_db(prp_hash_table_t *table);
void prp_print_node_info(prp_node_t *node);
void prp_print_hash_table_stats(prp_hash_table_t *table);
#endif // PRP_NODE_H


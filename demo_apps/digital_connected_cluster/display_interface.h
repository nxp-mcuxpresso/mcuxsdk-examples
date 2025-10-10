/*
 *  Copyright 2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef DISPLAY_INTERFACE_H_
#define DISPLAY_INTERFACE_H_

#include "lfs.h"

void br_discovery_complete(struct bt_br_discovery_result *results, size_t count);
uint32_t get_br_discover_result_count();
void get_paired_device_list(void);


void print_paired_devices(void);
static void paired_device_callback(const struct bt_bond_info *info, void *user_data);
void read_paired_dev_from_memory();
void save_new_paired_device(struct bt_conn *conn , uint8_t isRiderHeadset);
int remove_paired_device(const uint8_t addr[6]);
int delete_device(int index);
void connect_paired_device(uint8_t device_index);
bool is_valid_device(uint8_t *addr);
void check_and_clear_bonded_devices();
void hfp_dial(const char *number);
void hfp_last_dial(void);
uint8_t get_device_type_from_cod(const uint8_t cod[3]);
void app_auto_connect_device(int device_type);
void app_update_last_connected_device(const uint8_t addr[6] , uint8_t device_type);

#endif /* DISPLAY_INTERFACE_H_ */

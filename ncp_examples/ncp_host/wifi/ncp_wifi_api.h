/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ncp_host_command_wifi.h"
#include "ncp_cmd_node.h"

typedef struct ncp_current_network_t
{
    uint16_t result;
    NCP_CMD_GET_CURRENT_NETWORK sta_network;
} ncp_current_network;

typedef struct ncp_get_mac_addr_t
{
    uint16_t result;
    NCP_CMD_GET_MAC_ADDRESS mac_addr;
} ncp_get_mac_addr;

typedef struct ncp_pkt_stats_t
{
    uint16_t result;
    NCP_CMD_PKT_STATS pkt_stats;
} ncp_pkt_stats;

typedef struct ncp_mac_addr_t
{
    /** Mac address array */
    char mac[MLAN_MAC_ADDR_LENGTH];
} ncp_mac_addr;

typedef struct ncp_ip_config_t
{
    uint16_t result;
    NCP_CMD_IP_CONFIG ip_config;
} ncp_ip_config;

char * wlan_ncp_get_state(void);
bool wlan_ncp_remove_network(NCP_WLAN_NETWORK * network);
bool wlan_ncp_add_network(char * ssid, char * key, int8_t mode, int8_t frequency, char * network_name, int8_t key_len);
bool wlan_ncp_disconnect(void);
bool wlan_ncp_scan(void);
bool wlan_ncp_get_scan_result_count(uint8_t * count);
bool wlan_ncp_connect(char * network_name);
int wlan_ncp_get_current_network(NCP_WLAN_NETWORK * net_work);
bool wlan_ncp_remove_all_networks(void);
int wlan_ncp_get_mac_address(unsigned char * dest);
int wlan_ncp_get_pkt_stats(NCP_CMD_PKT_STATS *stats);
int wlan_ncp_get_scan_result(unsigned int index, NCP_WLAN_SCAN_RESULT * res);
int wlan_ncp_get_current_rssi(short * rssi);
uint8_t wlan_ncp_get_current_channel(void);
int wlan_ncp_get_ip_config(NCP_CMD_IP_CONFIG * addr);
/*
 * Copyright 2020 - 2021,2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_CONNECT_H__
#define __APP_CONNECT_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define AUTO_CONNECT_USE_BOND_INFO (0U)
#define RIDER_PHONE   0x00
#define RIDER_HEADSET  0x01
#define PASSENGER_HEADSET  0x02

#define RIDER_PHONE_BIT        (1 << RIDER_PHONE)
#define RIDER_HEADSET_BIT      (1 << RIDER_HEADSET)
#define PASSENGER_HEADSET_BIT  (1 << PASSENGER_HEADSET)

#define LAST_CONNECTED_SHIFT   4
#define LAST_CONNECTED_MASK    (1 << LAST_CONNECTED_SHIFT)  // 0x1

#define PHONE   0x00
#define HEADSET  0x01
#define INVALID_DEVICE 0x04

extern struct bt_conn *conn_rider_phone;
extern struct bt_conn *conn_rider_hs;
extern struct bt_conn *conn_passenger_hs;

extern uint8_t g_profileConnectedRiderHs;
extern uint8_t g_profileConnectedPassengerHs;

extern int g_pairedDeviceCount;

extern uint8_t g_auto_connection_status;

typedef struct {
    uint8_t addr[6];     // MAC Address
    char name[64];       // Device Name
    uint8_t device_type; // Device Type (Mobile, rider Headset, passenger Headset)
} paired_device_t;

extern paired_device_t paired_devices[MAX_PAIRED_DEVICES];

/*******************************************************************************
 * API
 ******************************************************************************/

void app_connect_init(void);
void app_connect(uint8_t profile,uint8_t *addr);
void app_disconnect(uint8_t profile);
void app_lfs_init(void);
void app_a2dp_snk_auto_connect(void);
int app_read_paired_devices();
int app_save_paired_device(const uint8_t addr[6], const char *name, uint8_t device_type);
int app_save_paired_devices();
int app_clear_paired_devices();
void app_auto_connect_paired_devices();
void app_schedule_auto_connect();

#endif /* __APP_CONNECT_H__ */

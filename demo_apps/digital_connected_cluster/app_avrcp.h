/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APPL_AVRCP_H_
#define _APPL_AVRCP_H_

/* --------------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"


/* --------------------------------------------- External Global Variables */
extern bool g_avrcpControlConnectHfpHf;

int cmd_init_ct();
int cmd_init_tg();

void avrcp_control_connect(struct bt_conn *default_conn);
void avrcp_browsing_connect(struct bt_conn *default_conn);

void avrcp_play_button(uint8_t press);
void avrcp_pause_button(uint8_t press);
void avrcp_forward_backward(uint8_t ind);
void avrcp_volume_up_down(uint8_t ind);

void avrcp_ct_get_element_attributes();
int avrcp_tg_notify_track_changed();
int avrcp_tg_notify_playback_status_changed();
int avrcp_tg_notify_volume_changed(uint8_t value);
int avrcp_tg_notify(uint8_t event,uint8_t status);
void avrcp_tg_set_absolute_volume(uint8_t value);

void avrcp_ct_register_notification(struct bt_conn *conn,uint8_t reg_notify_type);
void avrcp_ca_init_initiator();
void avrcp_ca_init_connect();
void avrcp_ca_get_image();
void cmd_control_connect();
void cmd_browsing_connect();
void avrcp_get_folder_items();
void avrcp_play_folder_items(uint8_t select_index);
void  avrcp_browse_get_folder_items();
void avrcp_get_playsong_detail();
void avrcp_browse_play_folder_items(uint8_t select_index);
static void cover_art_get_image(uint8_t print);
static void cover_art_set_handle(uint8_t **image_handle, uint16_t *image_handle_len);
//void avrcp_ct_tg_init();


#endif /* _APPL_AVRCP_H_ */




/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __APP_PBAP_PCE_H__
#define __APP_PBAP_PCE_H__

#define CURRENT_PATH_MAX_LEN 26
#define MAX_PHONE_CONNECTION 1
typedef struct app_pbap_pce_
{
    struct bt_pbap_pce *pbap_pceHandle;
    struct bt_conn *conn;
    uint8_t peer_bd_addr[6];
    uint16_t goep_version;
    uint16_t pbap_version;
    uint32_t peer_feature;
    uint32_t loacal_feature;
    uint8_t supported_repositories;
    char currentpath[CURRENT_PATH_MAX_LEN];
    bool lcl_srmp_wait; /* local srmp */
    bool rem_srmp_wait; /* remote srmp */
    int8_t num_srmp_wait;
} app_pbap_pce_t;

typedef enum {
	CALL_TYPE_FAVORITE,
    CALL_TYPE_MISSED,
    CALL_TYPE_INCOMING,
    CALL_TYPE_OUTGOING
} call_type_t;

typedef enum {
    PBAP_DOWNLOAD_FAILED = 0,        // "Phonebook download failed."
    PBAP_DOWNLOAD_IN_PROGRESS = 1,   // "Phonebook download in progress."
    PBAP_DOWNLOAD_SUCCESSFUL = 2     // "Phonebook download successful."
} pbap_download_status_t;


void pbap_pce_task(void *pvParameters);
//int app_pbap_connect();
int app_pull_phonebook(char *name);
int app_set_phonebook_path(char *name);
int app_pull_vcard_listing(char *name);
int app_pull_vcard_entry(char *name);
void print_contact_list(int type);
void sdp_discover_for_pbap_client(struct bt_conn *conn_handle);
void pbap_register_service();
int pbap_pull_phonebook(int type); //pp
void pbap_contacts_list_infotoApplication(uint16_t length, uint8_t *value);
void pbap_download_status_infotoApplication(pbap_download_status_t status);
void clear_pbap_data(int index);


#endif /* __APP_PBAP_PCE_H__ */

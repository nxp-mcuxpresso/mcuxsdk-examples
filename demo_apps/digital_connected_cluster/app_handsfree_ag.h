/*
 * Copyright 2020 - 2021,2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __APPL_PERIPHERAL_HFP_AG_MAIN_H__
#define __APPL_PERIPHERAL_HFP_AG_MAIN_H__

#include <bluetooth/hfp_hf.h>
#include <bluetooth/hfp_ag.h>

#define APP_MAX_HFP_AG_CONN (2U)

typedef struct app_hfp_ag_
{
    struct bt_hfp_ag *hfp_agHandle;
    struct bt_conn *conn;
    uint8_t peerKeyMissed;
    uint8_t appl_acl_initiated;
    uint8_t peer_bd_addr[6];
    struct k_work ataRespWork;
    TimerHandle_t xTimers;
    TimerHandle_t xTwcTimers;
    uint8_t selectCodec;
    uint8_t hfp_in_calling_status;
} app_hfp_ag_t;


//extern app_hfp_ag_t g_HfpAgs[CONFIG_BT_HFP_AG_MAX_CONN];
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

extern uint8_t g_hfWbsEnable, g_agWbsEnable,g_hfOutCall;
extern uint8_t g_sHfpInCallingStatus;
extern uint8_t g_sHfpAgRhDisconnecting;
extern uint8_t g_sHfpAgPhDisconnecting;
extern volatile uint8_t g_sCallStatus;
//extern app_hfp_ag_t *g_HfpAg;
extern uint8_t g_hfNbs,g_agRhNbs,g_agPhNbs;

extern uint16_t g_phoneESCO;
extern uint16_t g_rhsESCO;
extern uint16_t g_phsESCO;
extern uint8_t g_appAgConnectInitialized;

void hfp_ag_register_service();
void bt_work_init_ag();
int app_hfp_ag_discover(struct bt_conn *conn, uint8_t channel);
int app_hfp_ag_disconnect();
int app_hfp_ag_start_incoming_call(void);
void app_hfp_ag_transfer_ring_ind(void);
int app_hfp_ag_start_twc_incoming_call(void);
void app_hfp_ag_open_audio(uint8_t hs_type);
void app_hfp_ag_close_audio(uint8_t hs_type);
int app_hfp_ag_accept_incoming_call(void);
int app_hfp_ag_stop_incoming_call(void);
int app_hfp_ag_codec_select(uint8_t hs_type, uint8_t codec);
void app_hfp_ag_set_phnum_tag(char *name);
void app_hfp_ag_volume_update(hf_ag_volume_type_t type, int volume);
void app_hfp_ag_transfer_call_ind(void);
void enable_esco_bridge(uint16_t rhsESCO, uint16_t phsESCO);
void ag_nb_codec_select(uint8_t hs_type,uint32_t value);
void app_intercom_to_rider_call();
void app_hfp_ag_select_conn(uint8_t index);
void app_hfp_ag_send_setup_call_indication(uint32_t value);
int app_hfp_intercom_status(void);
void app_rider_call_to_intercom();
void app_hfp_hf_disconnected();
void app_enable_intercom(uint8_t en);
void app_hfp_ag_send_call_end_indicator();

#endif /* __APPL_PERIPHERAL_HFP_AG_MAIN_H__ */

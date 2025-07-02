/*
 * Copyright 2020 - 2021, 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_A2DP_SOURCE_H__
#define __APP_A2DP_SOURCE_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

typedef struct {
    uint8_t sample_rate;       // Sampling frequency
    uint8_t channel_mode;      // Channel mode
    uint8_t block_length;      // Block length
    uint8_t subbands;          // Subbands
    uint8_t allocation_method; // Allocation method
    uint8_t min_bitpool;       // Minimum bitpool
    uint8_t max_bitpool;       // Maximum bitpool
} a2dp_codec_config_t;

#define A2DP_LINK_POLICY (0x0004U)

void app_a2dp_source_task(void *param);
void sdp_discover_for_a2dp_sink(uint8_t isRiderHS);
void a2dp_source_ready(void);
int data_send_source(uint8_t *data, uint32_t length);
void a2dp_source_register_service();
void app_a2dp_src_start(uint8_t rider_hs);
void app_a2dp_src_stop(uint8_t rider_hs);
void app_a2dp_bridge_src_stop();
uint8_t app_set_a2dp_music_source(uint8_t music_source);
void app_set_dual_a2dp_src_mode(uint8_t dual_src_enable);
void app_dual_a2dp_src_pause();
void app_dual_a2dp_src_resume();
void app_a2dp_music_play(uint8_t rider_hs);
void app_a2dp_music_stop(uint8_t rider_hs);
//uint8_t app_sdp_a2sink_user(struct bt_conn *conn,struct bt_sdp_client_result *result);
void read_audio(void *param);
uint8_t app_get_a2dp_mode();
void app_a2dp_snk_pause();
void app_a2dp_snk_resume();
#endif /* __APP_A2DP_SOURCE_H__ */

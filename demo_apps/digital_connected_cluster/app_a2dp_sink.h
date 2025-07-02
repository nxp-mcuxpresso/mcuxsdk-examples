/*
 * Copyright 2020 - 2021, 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_A2DP_SINK_H__
#define __APP_A2DP_SINK_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

void a2dp_sink_register_service();
void app_a2dp_sink_task(void *param);
void a2dp_sink_ready();
void sdp_discover_for_a2dp_source(void);
uint8_t app_get_snk_a2dp_status();

#endif /* __BT_BR_A2DP_SINK_H__ */

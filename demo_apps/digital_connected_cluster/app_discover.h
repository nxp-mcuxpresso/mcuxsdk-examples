/*
 * Copyright 2020 - 2021,2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_DISCOVER_H__
#define __APP_DISCOVER_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#define APP_INQUIRY_LENGTH        (10) //10 /* 10 * 1.28 Sec */
#define APP_INQUIRY_NUM_RESPONSES (20) //20

extern struct bt_br_discovery_result br_discovery_results[APP_INQUIRY_NUM_RESPONSES];
void app_discover(void);

uint8_t *app_get_addr(uint8_t select);
void get_last_discover_list();

#endif /* __APP_DISCOVER_H__ */

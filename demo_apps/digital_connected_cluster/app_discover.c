/*
 * Copyright 2020 - 2021,2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <porting.h>
#include <string.h>
#include <errno/errno.h>
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/a2dp_codec_sbc.h>
#include <bluetooth/sdp.h>
#include "clock_config.h"
#include "board.h"
#include "app_connect.h"
#include "fsl_debug_console.h"
#include "display_interface.h"
#include "app_discover.h"


struct bt_br_discovery_result br_discovery_results[APP_INQUIRY_NUM_RESPONSES];

void app_discover(void)
{
    int err;
    struct bt_br_discovery_param param;

    param.length = APP_INQUIRY_LENGTH;
    param.limited = 0U;
    err = bt_br_discovery_start(&param, br_discovery_results,
                                APP_INQUIRY_NUM_RESPONSES,
                                br_discovery_complete);
    if (err != 0)
    {
        PRINTF("Failed to start discovery\r\n");
    }
    else
    {
        PRINTF("Discovery started. Please wait ...\r\n");
    }
}

uint8_t *app_get_addr(uint8_t select)
{
	uint32_t loc_cnt = get_br_discover_result_count();
    if (select < get_br_discover_result_count())
    {
        return &br_discovery_results[select].addr.val[0];
    }

    return NULL;
}





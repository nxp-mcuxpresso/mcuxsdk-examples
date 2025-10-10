/*
 *  Copyright 2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
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
#include <bluetooth/hfp_ag.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/a2dp_codec_sbc.h>
#include <bluetooth/sdp.h>
#include <app_dcc.h>
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app_connect.h"
#include "a2dp_pl_media_48KHz.h"
#include "app_shell.h"

#include "BT_config.h"
#include "app_handsfree_ag.h"
#include "app_handsfree.h"
#include "app_a2dp_sink.h"
#include "app_a2dp_source.h"
#include "app_pbap_pce.h"
#include "app_map_mce.h"

#define A2DP_CLASS_OF_DEVICE (0x200404U)
#define BT_HCI_READ_FW_VERSION BT_OP(BT_OGF_VS, 0x000F)

static void auth_cancel(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    PRINTF("Pairing cancelled: %s\n", addr);
}

static void passkey_display(struct bt_conn *conn, unsigned int passkey)
{
    PRINTF("Passkey %06u\n", passkey);
}

#if 0
static void passkey_confirm(struct bt_conn *conn, unsigned int passkey)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    PRINTF("Confirm passkey for %s: %06u", addr, passkey);
    s_passkeyConfirm = 1;
}
#endif

static struct bt_conn_auth_cb auth_cb_display = {
    .cancel = auth_cancel, .passkey_display = passkey_display, /* Passkey display callback */
                                                               //  .passkey_confirm = passkey_confirm,
};

static void bt_ready(int err)
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;

    if (err) {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

#if (defined(CONFIG_BT_SETTINGS) && (CONFIG_BT_SETTINGS > 0))
    settings_load();
#endif /* CONFIG_BT_SETTINGS */

    PRINTF("Bluetooth initialized\n");

    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(A2DP_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        err = -ENOBUFS;
    }

    if (err)
    {
#ifdef APP_DEBUG_EN
        PRINTF("setting class of device failed with err%d\n", err);
#endif
    }

   // PRINTF("Read FW version");
    err = bt_hci_cmd_send_sync(BT_HCI_READ_FW_VERSION, NULL, NULL);

    if (err)
    {
    	PRINTF("Failed to read FW version");
    }


    a2dp_sink_ready();
    a2dp_source_ready();
    hfp_hf_init();
    bt_hfp_ag_init();
   
    app_connect_init();
    bt_conn_auth_cb_register(&auth_cb_display);
    app_shell_init();
    a2dp_sink_register_service();
    a2dp_source_register_service();
    hfp_ag_register_service();
    hfp_hf_register_service();
    app_lfs_init();
    bt_work_init_ag();
#if defined(PBAP_PROFILE_ENABLE) && (PBAP_PROFILE_ENABLE == 1)
    pbap_register_service();
#endif
#if defined(MAP_PROFILE_ENABLE) && (MAP_PROFILE_ENABLE == 1)
    map_register_service();
#endif
    app_schedule_auto_connect();
//    cmd_init_ct();
//    avrcp_ct_tg_init();
}

void app_dcc_task(void *pvParameters)
{
    int err = 0;
    (void)err;

    PRINTF("DIGITAL CONNECTED CLUSTER Demo started...\n");

    /* Initializate BT Host stack */
    err = bt_enable(bt_ready);
    if (err) {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

    vTaskDelete(NULL);
}


/*
 * Copyright 2021-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#if CONFIG_NCP_BLE

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <errno.h>
#include <zephyr/toolchain.h>
#include <porting.h>
#include <fsl_debug_console.h>
#include <zephyr/sys/byteorder.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include "service.h"

#include "ncp_glue_ble.h"
#include "peripheral_matter.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/
//Matter bt_conn pointer store
extern struct bt_conn *conn_store[CONFIG_BT_MAX_CONN];
static uint16_t matter_conn_id = 0;

static struct bt_conn *default_conn = NULL;
static bool is_registered = false;

static uint8_t C3_data[256];

static bool indication_enabled = 0;
static uint8_t indicating;
static struct bt_gatt_indicate_params ind_params;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void matter_adv_start(void);

uint16_t get_conn_id(struct bt_conn *conn)
{
    uint8_t conn_idx = 0;

    for (conn_idx = 0; conn_idx < CONFIG_BT_MAX_CONN; conn_idx++)
    {
        if (conn_store[conn_idx] == conn)
        {
            break;
        }
    }

    if (conn_idx == CONFIG_BT_MAX_CONN)
    {
        PRINTF("fail to get conn id\n");
    }

    return conn_idx;
}


static ssize_t handle_rx_write(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
    PRINTF("handle_rx_write, len: %d \n", len);

    matter_conn_id = get_conn_id(conn);

    NCP_MATTER_RX_WRITE_EV event;
    event.conn_id = matter_conn_id;
    event.offset = offset;
    event.flags = flags;
    event.len = len;

    memcpy(event.data, buf, len);

    ble_prepare_status(NCP_EVENT_BLE_MATTER_RX_WRITE, NCP_CMD_RESULT_OK, (uint8_t *)&event, sizeof(event));

    return len;
}

static void handle_ccc_cfg(const struct bt_gatt_attr *attr, uint16_t value)
{
    indication_enabled = (value == BT_GATT_CCC_INDICATE);

    PRINTF("matter indication %s \n", indication_enabled ? "enabled" : "disabled");

    NCP_MATTER_CCC_CFG_EV event;
    event.conn_id = matter_conn_id;
    event.value = value;

    ble_prepare_status(NCP_EVENT_BLE_MATTER_CCC_CFG, NCP_CMD_RESULT_OK, (uint8_t *)&event, sizeof(event));
}

static ssize_t handle_C3_read(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
    //get data from host store to read_data.
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &C3_data, sizeof(C3_data));
}

/*******************************************************************************
 * Definitions
 ******************************************************************************/

static struct bt_gatt_attr matter_attrs[] = {

    BT_GATT_PRIMARY_SERVICE(UUID16_CHIPoBLEService),

    BT_GATT_CHARACTERISTIC(UUID128_CHIPoBLEChar_RX, 
                           BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                           BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                           NULL, handle_rx_write, NULL),

    BT_GATT_CHARACTERISTIC(UUID128_CHIPoBLEChar_TX,
                           BT_GATT_CHRC_INDICATE,
                           BT_GATT_PERM_NONE,
                           NULL, NULL, NULL),

    BT_GATT_CCC(handle_ccc_cfg, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
    BT_GATT_CHARACTERISTIC(UUID128_CHIPoBLEChar_C3,
                           BT_GATT_CHRC_READ,
                           BT_GATT_PERM_READ,
                           handle_C3_read, NULL, NULL),
#endif
};

static struct bt_gatt_service matter_svc = BT_GATT_SERVICE(matter_attrs);

/*******************************************************************************
 * Code
 ******************************************************************************/

void peripheral_matter_connect(struct bt_conn *conn, uint8_t conn_err)
{
    default_conn = bt_conn_ref(conn);
}

void peripheral_matter_disconnect(struct bt_conn *conn, uint8_t reason)
{
    if (default_conn)
    {
        bt_conn_unref(default_conn);
        default_conn = NULL;
    }
}

static void indicate_cb(struct bt_conn *conn, struct bt_gatt_indicate_params *params, uint8_t err)
{
    PRINTF("Indication %s\n", err != 0U ? "fail" : "success");
    indicating = 0U;
    
    NCP_MATTER_IND_CONFIRM_EV event;
    event.conn_id = matter_conn_id;
    event.err = err;

    ble_prepare_status(NCP_EVENT_BLE_MATTER_IND_CONFIRM, NCP_CMD_RESULT_OK, (uint8_t *)&event, sizeof(event));
}

void peripheral_matter_indication(uint8_t *buff, uint16_t len)
{

    if (indication_enabled)
    {
        /* check to not send consecutive indications before receiving a response */
        if (indicating)
        {
            return;
        }

        ind_params.attr = &matter_svc.attrs[3];
        ind_params.func = indicate_cb;
        ind_params.data = buff;
        ind_params.len = len;

        if (bt_gatt_indicate(NULL, &ind_params) == 0)
        {
            indicating = 1U;
        }
    }
}

void unregister_matter_service(void) {
    if (is_registered)
    {
        bt_gatt_service_unregister(&matter_svc);
        is_registered = false;
    }
}

void init_matter_service(void) {
    if (!is_registered)
    {
        bt_gatt_service_register(&matter_svc);
        is_registered = true;
    }
}

void peripheral_matter_task(void *pvParameters)
{

    while(1)
    {
        vTaskDelay(1000);
    }
}

#endif /* CONFIG_NCP_BLE */
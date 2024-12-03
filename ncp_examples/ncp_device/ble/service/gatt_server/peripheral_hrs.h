/*
 * Copyright 2021 NXP
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

void peripheral_hrs_task(void *pvParameters);

void peripheral_hrs_connect(struct bt_conn *conn, uint8_t conn_err);

void peripheral_hrs_disconnect(struct bt_conn *conn, uint8_t reason);

void init_hrs_service(void);

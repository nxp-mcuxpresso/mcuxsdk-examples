/*
 * Copyright (c) 2019 Aaron Tsui <aaron.tsui@outlook.com>
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HTS_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HTS_H_
/**
 * @brief Health Thermometer Service (HTS)
 * @defgroup bt_hts Health Thermometer Service (HTS)
 * @ingroup bluetooth
 * @{
 *
 * [Experimental] Users should note that the APIs can change
 * as a part of ongoing development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Definitions
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/
void peripheral_hts_task(void *args);

void peripheral_hts_connect(struct bt_conn *conn, uint8_t conn_err);

void peripheral_hts_disconnect(struct bt_conn *conn, uint8_t reason);

void init_hts_service(void);




#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HTS_H_ */

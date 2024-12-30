/*
 * Copyright 2021 NXP
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef NCP_HRS_H_
#define NCP_HRS_H_
/**
 * @brief Health Rate Service (HRS)
 * @defgroup bt_hrs Health Rate Service (HRS)
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
void peripheral_hrs_task(void *pvParameters);

void peripheral_hrs_connect(struct bt_conn *conn, uint8_t conn_err);

void peripheral_hrs_disconnect(struct bt_conn *conn, uint8_t reason);

void init_hrs_service(void);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* NCP_HRS_H_ */
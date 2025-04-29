/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SOURCES_CSEC_UTILS_H_
#define SOURCES_CSEC_UTILS_H_

#include "ela_csec_driver.h"

/* This function sets the MASTER_ECU key with a key (g_authIdKey) defined in the
 * ela_csec_utils.c file. This key will be used as an authorization secret for updating
 * user keys.
 * Setting the MASTER_ECU key will work only for the first time, in order to use
 * another value for the key, there are two options:
 * - erase the keys and then update g_authIdKey;
 * - use loadKey with counter > 1 and then update g_authIdKey;
 * */
bool setAuthKey(void);

/* This function erases all the key. After using it, the Flash needs to be partitioned
 * again.
 * */
bool eraseKeys(void);

/* This function loads/updates a non-volatile key.
 * When updating the key, the counter needs to be greater then the previous one.
 * */
bool loadKey(csec_key_id_t keyId, uint8_t *keyNew, uint8_t counter);

#endif /* SOURCES_CSEC_UTILS_H_ */

/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PRP_DEVICE_H
#define PRP_DEVICE_H

#include "prp_types.h"

prp_priv_t *prp_create_device(uint8_t *master_mac, uint8_t *interlink_mac);
void prp_destroy_device(prp_priv_t *prp);

#endif // PRP_DEVICE_H

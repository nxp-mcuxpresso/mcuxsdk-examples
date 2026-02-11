/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PRP_FRAME_H
#define PRP_FRAME_H

#include "prp_types.h"

bool prp_fill_frame_info(prp_priv_t *prp, prp_frame_t *frame,
			 uint16_t frame_len, prp_port_type_t port_type);
uint8_t *prp_create_tagged_frame(prp_priv_t *prp, prp_frame_t *frame,
                                 prp_port_type_t port_type, uint16_t *out_len);
uint8_t *prp_get_untagged_frame(prp_frame_t *frame, uint16_t *out_len);
bool prp_drop_frame(prp_frame_t *frame, prp_port_type_t out_port);

#endif // PRP_FRAME_H


/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FACE_BOX_UTILS_H_
#define FACE_BOX_UTILS_H_

#include "models/utils.h"

/* Translate boxes into labeled rectangles using display characteristics */
bool boxes_to_rects(box_data final_boxes[], uint32_t num_boxes, uint32_t max_boxes, mpp_labeled_rect_t *rects);

#endif /* FACE_BOX_UTILS_H_ */

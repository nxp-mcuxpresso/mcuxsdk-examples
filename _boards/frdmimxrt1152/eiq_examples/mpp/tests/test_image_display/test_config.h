/*
 * Copyright 2022, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration for evkmimxrt1070
 */

/*******************************************************************************
 * TEST configuration
 ******************************************************************************/
#include "images/skigirl_COCO_320_320_bgra.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_WIDTH
void *image_data = (void *)skigirl_COCO_320_320_bgra_data;
#define EXPECTED_CHECKSUM 0x5547a1d5

#endif /* _TEST_CONFIG_H */

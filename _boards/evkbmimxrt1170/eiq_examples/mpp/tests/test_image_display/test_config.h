/*
 * Copyright 2022, 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration for evkbmimxrt1170
 */

/*******************************************************************************
 * TEST configuration
 ******************************************************************************/
#include "images/skigirl_COCO_320_256_rgb565.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_SKIGIRL_COCO_320_256_RGB565_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_SKIGIRL_COCO_320_256_RGB565_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_SKIGIRL_COCO_320_256_RGB565_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_SKIGIRL_COCO_320_256_RGB565_WIDTH
void *image_data = (void *)skigirl_COCO_320_256_rgb565_data;
#define EXPECTED_CHECKSUM 0x31e29bda

#endif /* _TEST_CONFIG_H */

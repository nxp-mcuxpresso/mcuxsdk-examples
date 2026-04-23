/*
 * Copyright 2024-2026 NXP
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
#define IMG_couple_COCO  0

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_couple_COCO
#endif

#ifndef APP_CONFIG
#define APP_CONFIG 1
#endif

#if (APP_CONFIG==1) /* default app config */
#include "images/couple_COCO_320_240_bgra.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_COUPLE_COCO_320_240_BGRA_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_COUPLE_COCO_320_240_BGRA_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_COUPLE_COCO_320_240_BGRA_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_COUPLE_COCO_320_240_BGRA_WIDTH
void *image_data = (void *)couple_COCO_320_240_bgra_data;
#define EXPECTED_PERSON_CONFIDENCE_MIN  50
#define EXPECTED_FACE_CONFIDENCE_MIN    99
#define EXPECTED_NUM_DETECTED_PERSONS   1
#define EXPECTED_NUM_DETECTED_FACES     2
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

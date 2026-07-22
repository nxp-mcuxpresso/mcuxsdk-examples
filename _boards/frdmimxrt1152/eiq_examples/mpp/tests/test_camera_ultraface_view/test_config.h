/*
 * Copyright 2023-2025 NXP
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
#define IMG_couple_COCO   0
#define IMG_skigirl_COCO  1

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
#define EXPECTED_CONFIDENCE_MIN       99
#define EXPECTED_NUM_DETECTED_FACES   2
#define EXPECTED_CHECKSUM             0x6ecd0a1f
#elif (APP_CONFIG==2)
#include "images/skigirl_COCO_427_284_bgra.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_SKIGIRL_COCO_427_284_BGRA_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_SKIGIRL_COCO_427_284_BGRA_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_SKIGIRL_COCO_427_284_BGRA_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_SKIGIRL_COCO_427_284_BGRA_WIDTH
void *image_data = (void *)skigirl_COCO_427_284_bgra_data;
#define EXPECTED_CONFIDENCE_MIN       87
#define EXPECTED_NUM_DETECTED_FACES   1
#define EXPECTED_CHECKSUM             0xb7d8dd65
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

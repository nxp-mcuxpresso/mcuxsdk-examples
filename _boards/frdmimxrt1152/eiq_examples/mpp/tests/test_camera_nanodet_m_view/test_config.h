/*
 * Copyright 2022-2025 NXP
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
#define IMG_skigirl_COCO 0

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_skigirl_COCO
#endif

#ifndef APP_CONFIG
#define APP_CONFIG 1
#endif

#if (APP_CONFIG==1) /* default app config */
/* TFlite is enabled by default */
#include "images/skigirl_COCO_320_320_bgra.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_WIDTH
void *image_data = (void *)skigirl_COCO_320_320_bgra_data;
#define EXPECTED_CONFIDENCE_MIN         79
#define EXPECTED_NUM_DETECTED_OBJECTS   1
const char* expected_labels[] =         {"person"};
#define EXPECTED_CHECKSUM 0x8a6d1bc8
#elif (APP_CONFIG==3)
#include "images/skigirl_COCO_320_320_bgra.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_SKIGIRL_COCO_320_320_BGRA_WIDTH
void *image_data = (void *)skigirl_COCO_320_320_bgra_data;
#define EXPECTED_CONFIDENCE_MIN         79
#define EXPECTED_NUM_DETECTED_OBJECTS   2   /* WA to issue MPP-297 */
const char* expected_labels[] =         {"person", "person"};
#define EXPECTED_CHECKSUM 0x2e5a0041
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

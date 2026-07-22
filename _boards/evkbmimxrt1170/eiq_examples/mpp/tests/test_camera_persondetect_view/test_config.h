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
#define IMG_skigirl_COCO  0

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_skigirl_COCO
#endif

#ifndef APP_CONFIG
#define APP_CONFIG 1
#endif

#if (APP_CONFIG==1) /* default app config */
#ifndef USE_NAS_OPTIMIZED_MODEL
#include APP_STATIC_IMAGE_NAME
#define SRC_IMAGE_FORMAT SRC_IMAGE_SKIGIRL_COCO_160_128_RGB565_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_SKIGIRL_COCO_160_128_RGB565_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_SKIGIRL_COCO_160_128_RGB565_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_SKIGIRL_COCO_160_128_RGB565_WIDTH
void *image_data = (void *)skigirl_COCO_160_128_rgb565_data;
#define EXPECTED_CONFIDENCE_MIN       0
#define EXPECTED_NUM_DETECTED_PERSONS 1
#define EXPECTED_CHECKSUM             0x5470aa4c //TODO fix moving boxes with the old 160 x 128 model
#else
#include APP_STATIC_IMAGE_NAME
#define SRC_IMAGE_FORMAT SRC_IMAGE_COUPLE_COCO_220_220_RGB565_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_COUPLE_COCO_220_220_RGB565_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_COUPLE_COCO_220_220_RGB565_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_COUPLE_COCO_220_220_RGB565_WIDTH
void *image_data = (void *)couple_COCO_220_220_rgb565_data;
#define EXPECTED_CONFIDENCE_MIN       64
#define EXPECTED_NUM_DETECTED_PERSONS 2
#define EXPECTED_CHECKSUM             0xbcbfaf0d
#endif
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

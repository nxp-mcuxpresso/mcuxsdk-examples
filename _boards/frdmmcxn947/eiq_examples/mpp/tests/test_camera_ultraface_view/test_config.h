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
#if !defined(RTOS_HEAP_SIZE) || (defined(RTOS_HEAP_SIZE) && (RTOS_HEAP_SIZE < 110))
#error "APP requires at least 110k of FreeRTOS heap size"
#endif

/* checksum used for this test is PISANO */
#define CHECKSUM_TYPE_EXPECTED_PISANO 1

#define IMG_couple_COCO   0

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_couple_COCO
#endif

#ifndef APP_CONFIG
#define APP_CONFIG 1
#endif

#if (APP_CONFIG==1) /* default app config */
#include "images/couple_COCO_128_128_rgb.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_COUPLE_COCO_128_128_RGB_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_COUPLE_COCO_128_128_RGB_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_COUPLE_COCO_128_128_RGB_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_COUPLE_COCO_128_128_RGB_WIDTH
void *image_data = (void *)couple_COCO_128_128_rgb_data;
#define EXPECTED_CONFIDENCE_MIN       99
#define EXPECTED_NUM_DETECTED_FACES   2
#define EXPECTED_CHECKSUM             0x6ecd0a1f
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

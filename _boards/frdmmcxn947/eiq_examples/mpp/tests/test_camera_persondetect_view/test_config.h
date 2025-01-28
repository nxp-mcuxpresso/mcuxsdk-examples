/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration for frdmmcxn947
 */

/*******************************************************************************
 * TEST configuration
 ******************************************************************************/
#if !defined(RTOS_HEAP_SIZE) || (defined(RTOS_HEAP_SIZE) && (RTOS_HEAP_SIZE < 100))
#error "APP requires at least 100k of FreeRTOS heap size"
#endif

/* checksum used for this test is PISANO */
#define CHECKSUM_TYPE_EXPECTED_PISANO 1

#define IMG_skigirl_COCO   0

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_skigirl_COCO
#endif

#ifndef APP_CONFIG
#error "ERROR: test configuration APP_CONFIG is not defined"
#elif (APP_CONFIG == 1)
#if (APP_STRIPE_MODE == 0)
#include "images/skigirl_COCO_160_128_rgb565.h"
#define EXPECTED_CONFIDENCE_MIN       87
#else
#include "images/skigirl_COCO_320_256_rgb565.h"
#define EXPECTED_CONFIDENCE_MIN       86
#endif
#define EXPECTED_NUM_DETECTED_PERSONS 1
#define EXPECTED_CHECKSUM             0x00000000
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

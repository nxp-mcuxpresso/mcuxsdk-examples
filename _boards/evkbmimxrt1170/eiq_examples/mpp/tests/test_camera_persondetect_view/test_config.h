/*
 * Copyright 2024 NXP
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
#error "ERROR: test configuration APP_CONFIG is not defined"
#elif (APP_CONFIG ==1 )
#ifndef USE_NAS_OPTIMIZED_MODEL
#include APP_STATIC_IMAGE_NAME
#define EXPECTED_CONFIDENCE_MIN       0
#define EXPECTED_NUM_DETECTED_PERSONS 2
#define EXPECTED_CHECKSUM             0x00000000 //TODO fix moving boxes with the old 160 x 128 model
#else
#include APP_STATIC_IMAGE_NAME
#define EXPECTED_CONFIDENCE_MIN       64
#define EXPECTED_NUM_DETECTED_PERSONS 2
#define EXPECTED_CHECKSUM             0xbcbfaf0d
#endif
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

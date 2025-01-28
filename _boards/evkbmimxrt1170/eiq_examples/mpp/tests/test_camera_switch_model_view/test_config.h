/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration for evkmimxrt1170
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
#error "ERROR: test configuration APP_CONFIG is not defined"
#elif (APP_CONFIG ==1 )
#include "images/couple_COCO_320_240_bgra.h"
#define EXPECTED_PERSON_CONFIDENCE_MIN  71
#define EXPECTED_FACE_CONFIDENCE_MIN    99
#define EXPECTED_NUM_DETECTED_PERSONS   1
#define EXPECTED_NUM_DETECTED_FACES     2
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

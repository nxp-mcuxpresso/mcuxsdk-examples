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
#define IMG_stopwatch 0

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_stopwatch
#endif

#ifndef APP_CONFIG
#define APP_CONFIG 1
#endif

#if (APP_CONFIG==1) || (APP_CONFIG==3) || (APP_CONFIG==5)
#include "images/stopwatch168_208_vuyx.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_VUYX_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_VUYX_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_VUYX_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_VUYX_WIDTH
void *image_data = (void *)stopwatch168_208_vuyx_data;
#define EXPECTED_CONFIDENCE_MIN 62
#define EXPECTED_LABEL          "stopwatch"
#define EXPECTED_CHECKSUM       0xec15b997
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

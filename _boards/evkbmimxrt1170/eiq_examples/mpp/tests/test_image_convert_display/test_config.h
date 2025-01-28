/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration for evkbmimxrt1070
 */

/*******************************************************************************
 * TEST configuration
 ******************************************************************************/
#define IMG_90_160_rgb565le          0
#define IMG_stopwatch168_208_vuyx    1
#define IMG_dogs_COCO_320_320_bgra   2
#define IMG_stopwatch128_128_rgb     3
#define IMG_stopwatch168_208_rgb565  4
#define IMG_stopwatch168_208_uyvy422 5
#define IMG_stopwatch168_208_vyuy422 6

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_90_160_rgb565le
#endif

#ifndef APP_CONFIG
#error "ERROR: test configuration APP_CONFIG is not defined"
#elif (APP_CONFIG==0) /* default app config */
#include "images/90_160_rgb565le.h"
#define IMAGE_NAME "90_160_rgb565le"
#define EXPECTED_CHECKSUM 0x4113b668
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

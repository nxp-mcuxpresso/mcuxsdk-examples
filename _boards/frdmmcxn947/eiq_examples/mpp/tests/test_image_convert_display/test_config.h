/*
 * Copyright 2024-2025 NXP
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
#define IMG_90_160_rgb565le          0
#define IMG_stopwatch168_208_rgb565  4

/* checksum used for this test is PISANO */
#define CHECKSUM_TYPE_EXPECTED_PISANO 1

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_90_160_rgb565le
#endif

#ifndef APP_CONFIG
#define APP_CONFIG 0
#endif

#if (APP_CONFIG==0) /* default app config */
#include "images/90_160_rgb565le.h"
#define IMAGE_NAME "90_160_rgb565le"
#define EXPECTED_CHECKSUM 0x00000000
#elif (APP_CONFIG==1)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x66c61722
#elif (APP_CONFIG==2)
#include "images/90_160_rgb565le.h"
#define IMAGE_NAME "90_160_rgb565le"
#define EXPECTED_CHECKSUM 0x00000000
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

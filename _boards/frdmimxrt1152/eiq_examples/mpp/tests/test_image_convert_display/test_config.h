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
#define IMG_90_160_rgb565le          0
#define IMG_stopwatch168_208_vuyx    1
#define IMG_dogs_COCO_320_320_bgra   2
#define IMG_stopwatch128_128_rgb     3
#define IMG_stopwatch168_208_rgb565  4
#define IMG_stopwatch168_208_uyvy422 5
#define IMG_stopwatch168_208_vyuy422 6
#define IMG_couple_COCO_320_240_rgba 7

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_90_160_rgb565le
#endif

#ifndef APP_CONFIG
#define APP_CONFIG 0
#endif

#if (APP_CONFIG==0) /* default app config */
#include "images/90_160_rgb565le.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_90_160_RGB565LE_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_90_160_RGB565LE_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_90_160_RGB565LE_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_90_160_RGB565LE_WIDTH
void *image_data = (void *)image_90_160_rgb565le_data;
#define IMAGE_NAME "90_160_rgb565le"
#define EXPECTED_CHECKSUM 0x4113b668
#elif (APP_CONFIG==1)
#include "images/stopwatch168_208_vuyx.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_VUYX_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_VUYX_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_VUYX_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_VUYX_WIDTH
void *image_data = (void *)stopwatch168_208_vuyx_data;
#define IMAGE_NAME "stopwatch168_208_vuyx"
#define EXPECTED_CHECKSUM 0xe45089ed
#elif (APP_CONFIG==2)
#include "images/stopwatch128_128_rgb.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH128_128_RGB_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH128_128_RGB_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH128_128_RGB_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH128_128_RGB_WIDTH
void *image_data = (void *)stopwatch128_128_rgb_data;
#define IMAGE_NAME "stopwatch_RGB888"
#define EXPECTED_CHECKSUM 0xff157638
#elif (APP_CONFIG==3)
#include "images/stopwatch168_208_rgb565.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_RGB565_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_RGB565_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_RGB565_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_RGB565_WIDTH
void *image_data = (void *)stopwatch168_208_rgb565_data;
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0xaa22b500
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

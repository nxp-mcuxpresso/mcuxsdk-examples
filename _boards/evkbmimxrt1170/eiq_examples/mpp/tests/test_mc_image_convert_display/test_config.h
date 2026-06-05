/*
 * Copyright 2026 NXP
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
#define IMG_90_160_rgb565le          0
#define IMG_stopwatch168_208_vuyx    1
#define IMG_dogs_COCO_320_320_bgra   2
#define IMG_stopwatch128_128_rgb     3
#define IMG_stopwatch168_208_rgb565  4
#define IMG_stopwatch168_208_uyvy422 5
#define IMG_stopwatch168_208_vyuy422 6
#define IMG_stopwatch168_208_gray    7

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
#include "images/90_160_rgb565le.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_90_160_RGB565LE_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_90_160_RGB565LE_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_90_160_RGB565LE_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_90_160_RGB565LE_WIDTH
void *image_data = (void *)image_90_160_rgb565le_data;
#define IMAGE_NAME "90_160_rgb565le"
#define EXPECTED_CHECKSUM 0x03debd00
#elif (APP_CONFIG==2)
#include "images/stopwatch168_208_gray.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_GRAY_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_GRAY_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_GRAY_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_GRAY_WIDTH
void *image_data = (void *)stopwatch168_208_gray_data;
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==3)
#include "images/stopwatch168_208_gray.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_GRAY_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_GRAY_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_GRAY_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_GRAY_WIDTH
void *image_data = (void *)stopwatch168_208_gray_data;
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0xccad358b
#elif (APP_CONFIG==4)
#include "images/stopwatch168_208_gray.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_GRAY_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_GRAY_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_GRAY_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_GRAY_WIDTH
void *image_data = (void *)stopwatch168_208_gray_data;
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0x7b4a9e63
#elif (APP_CONFIG==5)
#include "images/stopwatch168_208_gray.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_GRAY_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_GRAY_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_GRAY_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_GRAY_WIDTH
void *image_data = (void *)stopwatch168_208_gray_data;
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0xff3077b9
#elif (APP_CONFIG==6)
#include "images/stopwatch168_208_gray.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_GRAY_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_GRAY_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_GRAY_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_GRAY_WIDTH
void *image_data = (void *)stopwatch168_208_gray_data;
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0x55b6e0cd
#elif (APP_CONFIG==7)
#include "images/stopwatch168_208_gray.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_GRAY_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_GRAY_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_GRAY_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_GRAY_WIDTH
void *image_data = (void *)stopwatch168_208_gray_data;
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0x47a61e63
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

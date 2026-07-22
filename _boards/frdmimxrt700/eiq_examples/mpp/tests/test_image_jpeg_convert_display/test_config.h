/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration for mimxrt700evk
 */

/*******************************************************************************
 * TEST configuration
 ******************************************************************************/

#ifndef APP_CONFIG
#define APP_CONFIG 0
#endif

#if (APP_CONFIG==0) /* default app config YUYV image */
#include "images/tiger_yuyv_jpg.h"
#define SRC_IMAGE_FORMAT            SRC_IMAGE_TIGER_JPEG_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER   SRC_IMAGE_TIGER_JPEG_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT            SRC_IMAGE_TIGER_JPEG_HEIGHT
#define SRC_IMAGE_WIDTH             SRC_IMAGE_TIGER_JPEG_WIDTH
void *image_data = (void *)tiger_yuv_jpg;
unsigned int image_data_len = tiger_jpeg_data_len;
#define IMAGE_NAME "tiger_jpeg"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==1) /* YUV444 */
#include "images/tiger_jpeg.h"
#define SRC_IMAGE_FORMAT            SRC_IMAGE_TIGER_JPEG_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER   SRC_IMAGE_TIGER_JPEG_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT            SRC_IMAGE_TIGER_JPEG_HEIGHT
#define SRC_IMAGE_WIDTH             SRC_IMAGE_TIGER_JPEG_WIDTH
void *image_data = (void *)tiger_jpeg_data;
unsigned int image_data_len = tiger_jpeg_data_len;
#define IMAGE_NAME "tiger_jpeg"
#define EXPECTED_CHECKSUM 0x0
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

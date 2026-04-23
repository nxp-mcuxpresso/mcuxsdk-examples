/*
 * Copyright 2023, 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration (board/peripheral independent).
 *
 * The checksum is calculated on the converted image using Pisano
 * with End-Around Carry algorithm).
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

#define IMG_COLOR_RGB565             0
#define IMG_COLOR_RGB888             1
#define IMG_COLOR_BGR888             2
#define IMG_COLOR_GRAY               3

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_stopwatch168_208_vuyx
#endif

/* Input image */
#if (IMAGE_TYPE == IMG_stopwatch168_208_vuyx)
#include "images/stopwatch168_208_vuyx.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_VUYX_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_VUYX_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_VUYX_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_VUYX_WIDTH
void *image_data = (void *)stopwatch168_208_vuyx_data;
#define IMAGE_NAME "stopwatch168_208_vuyx"
#if (IMG_SCALE == 4)
#define EXPECTED_CHECKSUM 0xf55db576
#elif (IMG_COLOR_CONVERT == IMG_COLOR_RGB565)
#define EXPECTED_CHECKSUM 0x6b0e0a4f
#elif (IMG_COLOR_CONVERT == IMG_COLOR_RGB888)
#define EXPECTED_CHECKSUM 0xf348b351
#elif (IMG_COLOR_CONVERT == IMG_COLOR_BGR888)
#define EXPECTED_CHECKSUM 0xa671510
#elif (IMG_COLOR_CONVERT == IMG_COLOR_GRAY)
#define EXPECTED_CHECKSUM 0x5b73ee5a
#endif
#elif (IMAGE_TYPE == IMG_dogs_COCO_320_320_bgra)
#include "images/dogs_COCO_320_320_bgra.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_DOGS_COCO_320_320_BGRA_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_DOGS_COCO_320_320_BGRA_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_DOGS_COCO_320_320_BGRA_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_DOGS_COCO_320_320_BGRA_WIDTH
void *image_data = (void *)dogs_COCO_320_320_bgra_data;
#define IMAGE_NAME "dogs_COCO_320_320_bgra.h"
#define EXPECTED_CHECKSUM 0x51dcda0d
#elif (IMAGE_TYPE ==IMG_stopwatch128_128_rgb)
#include "images/stopwatch128_128_rgb.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH128_128_RGB_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH128_128_RGB_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH128_128_RGB_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH128_128_RGB_WIDTH
void *image_data = (void *)stopwatch128_128_rgb_data;
#define IMAGE_NAME "stopwatch_RGB888"
#if (IMG_SCALE == 4)
#define EXPECTED_CHECKSUM 0x2770913f
#else
#define EXPECTED_CHECKSUM 0xdeadbeef
#endif
#elif (IMAGE_TYPE == IMG_stopwatch168_208_rgb565)
#include "images/stopwatch168_208_rgb565.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_RGB565_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_RGB565_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_RGB565_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_RGB565_WIDTH
void *image_data = (void *)stopwatch168_208_rgb565_data;
#define IMAGE_NAME "stopwatch168_208_rgb565"
#if (IMG_COLOR_CONVERT == IMG_COLOR_RGB888)
#define EXPECTED_CHECKSUM 0xee5e73c4
#elif (IMG_COLOR_CONVERT == IMG_COLOR_BGR888)
#define EXPECTED_CHECKSUM 0x11bd5213
#elif (IMG_COLOR_CONVERT == IMG_COLOR_GRAY)
#define EXPECTED_CHECKSUM 0xf33c1d54
#endif
#elif (IMAGE_TYPE == IMG_stopwatch168_208_uyvy422)
#include "images/stopwatch168_208_uyvy422.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_UYVY422_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_UYVY422_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_UYVY422_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_UYVY422_WIDTH
void *image_data = (void *)stopwatch168_208_uyvy422_data;
#define IMAGE_NAME "stopwatch_uyvy422"
#define EXPECTED_CHECKSUM 0xdeadbeef
#elif (IMAGE_TYPE == IMG_stopwatch168_208_vyuy422)
#include "images/stopwatch168_208_vyuy422.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_STOPWATCH168_208_VYUY422_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_STOPWATCH168_208_VYUY422_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_STOPWATCH168_208_VYUY422_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_STOPWATCH168_208_VYUY422_WIDTH
void *image_data = (void *)stopwatch168_208_vyuy422_data;
#define IMAGE_NAME "stopwatch_vyuy422"
#define EXPECTED_CHECKSUM 0xdeadbeef
#else
#include "images/90_160_rgb565le.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_90_160_RGB565LE_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_90_160_RGB565LE_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_90_160_RGB565LE_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_90_160_RGB565LE_WIDTH
void *image_data = (void *)image_90_160_rgb565le_data;
#define IMAGE_NAME "90_160_rgb565le"
#define EXPECTED_CHECKSUM 0xcd9996c5
#endif

#endif /* _TEST_CONFIG_H */

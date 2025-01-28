/*
 * Copyright 2024 NXP
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
#define IMG_stopwatch168_208_rgb565  0
#define IMG_dogs_COCO_320_320_bgra   1
#define IMG_stopwatch128_128_rgb     2
#define IMG_couple_COCO_320_240_rgba 3
#define IMG_skigirl_COCO_640_480_rgb 4

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_stopwatch168_208_rgb565
#endif

#ifndef APP_CONFIG
#error "ERROR: test configuration APP_CONFIG is not defined"
#elif (APP_CONFIG==0) /* default app config */
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==1)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==2)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==3)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==4)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==5)
#include "images/stopwatch128_128_rgb.h"
#define IMAGE_NAME "stopwatch128_128_rgb"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==7)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==8)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==9)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==10)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==11)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==12)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==13)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==14)
#include "images/stopwatch128_128_rgb.h"
#define IMAGE_NAME "stopwatch128_128_rgb"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==15)
#include "images/dogs_COCO_320_320_bgra.h"
#define IMAGE_NAME "dogs_COCO_320_320_bgra"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==16)
#include "images/couple_COCO_320_240_rgba.h"
#define IMAGE_NAME "couple_COCO_320_240_rgba"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==17)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==18)
#include "images/skigirl_COCO_640_480_rgb_v2.h"
#define IMAGE_NAME "skigirl_COCO_640_480_rgb"
#define EXPECTED_CHECKSUM 0x0
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

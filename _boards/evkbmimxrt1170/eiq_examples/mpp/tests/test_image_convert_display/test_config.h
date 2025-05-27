/*
 * Copyright 2022-2025 NXP
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
#define IMAGE_NAME "90_160_rgb565le"
#define EXPECTED_CHECKSUM 0x4113b668
#elif (APP_CONFIG==1)
#include "images/stopwatch168_208_vuyx.h"
#define IMAGE_NAME "stopwatch168_208_vuyx"
#define EXPECTED_CHECKSUM 0x4deb696c
#elif (APP_CONFIG==2)
#include "images/dogs_COCO_320_320_bgra.h"
#define IMAGE_NAME "dogs_COCO_320_320_bgra.h"
#define EXPECTED_CHECKSUM 0xd0513fe8
#elif (APP_CONFIG==3)
#include "images/stopwatch128_128_rgb.h"
#define IMAGE_NAME "stopwatch_RGB888"
#define EXPECTED_CHECKSUM 0xff157638
#elif (APP_CONFIG==4)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x66c61722
#elif (APP_CONFIG==5)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x3782fc9b
#elif (APP_CONFIG==6)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x2fda06d2
#elif (APP_CONFIG==7)
#include "images/stopwatch168_208_vuyx.h"
#define IMAGE_NAME "stopwatch168_208_vuyx"
#define EXPECTED_CHECKSUM 0x82e4d8b4
#elif (APP_CONFIG==8)
#include "images/stopwatch168_208_uyvy422.h"
#define IMAGE_NAME "stopwatch_uyvy422"
#define EXPECTED_CHECKSUM 0x9012c4ea
#elif (APP_CONFIG==9)
#include "images/stopwatch168_208_vyuy422.h"
#define IMAGE_NAME "stopwatch_vyuy422"
#define EXPECTED_CHECKSUM 0x9012c4ea
#elif (APP_CONFIG==10)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x1c57f68b
#elif (APP_CONFIG==11)
#include "images/stopwatch168_208_vuyx.h"
#define IMAGE_NAME "stopwatch168_208_vuyx"
#define EXPECTED_CHECKSUM 0x212f2e1f
#elif (APP_CONFIG==12)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x777fa657
#elif (APP_CONFIG==13)
#include "images/90_160_rgb565le.h"
#define IMAGE_NAME "90_160_rgb565le"
#define EXPECTED_CHECKSUM 0xd449c001
#elif (APP_CONFIG==14)
#include "images/stopwatch168_208_vuyx.h"
#define IMAGE_NAME "stopwatch168_208_vuyx"
#define EXPECTED_CHECKSUM 0x52c519a1
#elif (APP_CONFIG==15)
#include "images/stopwatch168_208_uyvy422.h"
#define IMAGE_NAME "stopwatch_uyvy422"
#define EXPECTED_CHECKSUM 0xeddc4f5e
#elif (APP_CONFIG==16)
#include "images/stopwatch128_128_rgb.h"
#define IMAGE_NAME "stopwatch_RGB888"
#define EXPECTED_CHECKSUM 0x88be2b8f
#elif (APP_CONFIG==17)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x189777d3
#elif (APP_CONFIG==18)
#include "images/stopwatch168_208_vuyx.h"
#define IMAGE_NAME "stopwatch168_208_vuyx"
#define EXPECTED_CHECKSUM 0xba0de12b
#elif (APP_CONFIG==19)
#include "images/stopwatch168_208_vuyx.h"
#define IMAGE_NAME "stopwatch168_208_vuyx"
#define EXPECTED_CHECKSUM 0x63854252
#elif (APP_CONFIG==20)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x0d68e866
#elif (APP_CONFIG==21)
#include "images/stopwatch168_208_vuyx.h"
#define IMAGE_NAME "stopwatch168_208_vuyx"
#define EXPECTED_CHECKSUM 0x0f66dcf3
#elif (APP_CONFIG==22)
#include "images/stopwatch168_208_vuyx.h"
#define IMAGE_NAME "stopwatch168_208_vuyx"
#define EXPECTED_CHECKSUM 0x52d38045
#elif (APP_CONFIG==23)
#include "images/stopwatch168_208_uyvy422.h"
#define IMAGE_NAME "stopwatch_uyvy422"
#define EXPECTED_CHECKSUM 0xeddc4f5e
#elif (APP_CONFIG==24)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0xaa22b500
#elif (APP_CONFIG==25) /* 90*/
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x5b02f800
#elif (APP_CONFIG==26)/* UPSCALE + 90*/
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x50f51338
#elif (APP_CONFIG==27)/* 180*/
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0xaa11c2f6
#elif (APP_CONFIG==28) /* UPSCALE + 180*/
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x165c01da
#elif (APP_CONFIG==29)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x9b8caadb
#elif (APP_CONFIG==30)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x6f5060b4
#elif (APP_CONFIG==31)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x038bf987
#elif (APP_CONFIG==32)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0xe168508e
#elif (APP_CONFIG==33)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x6872f3af
#elif (APP_CONFIG==34)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x05ede218
#elif (APP_CONFIG==35)
#include "images/stopwatch168_208_rgb565.h"
#define IMAGE_NAME "stopwatch168_208_rgb565"
#define EXPECTED_CHECKSUM 0x4487ccd0
#elif (APP_CONFIG==36)
#include "images/couple_COCO_320_240_rgba.h"
#define IMAGE_NAME "couple_COCO_320_240_rgba"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==37)
#include "images/dogs_COCO_320_320_bgra.h"
#define IMAGE_NAME "dogs_COCO_320_320_bgra.h"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==38)
#include "images/skigirl_COCO_320_256_rgb565.h"
#define IMAGE_NAME "skigirl_COCO_320_256_rgb565.h"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==39)
#include "images/skigirl_COCO_320_256_rgb565.h"
#define IMAGE_NAME "skigirl_COCO_320_256_rgb565.h"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==40)
#include "images/stopwatch168_208_gray.h"
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0x0
#elif (APP_CONFIG==41)
#include "images/stopwatch168_208_gray.h"
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0xccad358b
#elif (APP_CONFIG==42)
#include "images/stopwatch168_208_gray.h"
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0x7b4a9e63
#elif (APP_CONFIG==43)
#include "images/stopwatch168_208_gray.h"
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0xff3077b9
#elif (APP_CONFIG==44)
#include "images/stopwatch168_208_gray.h"
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0x55b6e0cd
#elif (APP_CONFIG==45)
#include "images/stopwatch168_208_gray.h"
#define IMAGE_NAME "stopwatch168_208_gray.h"
#define EXPECTED_CHECKSUM 0x47a61e63
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

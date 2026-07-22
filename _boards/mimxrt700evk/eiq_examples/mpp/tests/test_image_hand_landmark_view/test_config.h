/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

#include <stdbool.h>

/*
 * This is the test configuration for mimxrt700evk
 */

/*******************************************************************************
 * TEST configuration
 ******************************************************************************/

#ifndef APP_CONFIG
#define APP_CONFIG 0
#endif

#define EXPECTED_HAND_LANDMARK_MAX_INF_TIME     23U  /* ms */
#define EXPECTED_GESTURE_EMB_MAX_INF_TIME       4U  /* ms */
#define EXPECTED_GESTURE_CLS_MAX_INF_TIME       1U  /* ms */

#if (APP_CONFIG==0) /* default app config */
#include "internal/images/hand_224_224_rgb.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_HAND_224_224_RGB_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_HAND_224_224_RGB_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_HAND_224_224_RGB_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_HAND_224_224_RGB_WIDTH
void *image_data = (void *)hand_224_224_rgb_data;
#define EXPECTED_HAND_SCORE_MIN         99U  /* % */
#define EXPECTED_LEFT_HAND_VALUE        false
#define EXPECTED_GESTURE                "Open_Palm"
#define EXPECTED_GESTURE_SCORE_MIN      71U  /* % */
#elif (APP_CONFIG==1)
#include "internal/images/hand_victory_224_224_rgb.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_HAND_VICTORY_224_224_RGB_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_HAND_VICTORY_224_224_RGB_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_HAND_VICTORY_224_224_RGB_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_HAND_VICTORY_224_224_RGB_WIDTH
void *image_data = (void *)hand_victory_224_224_rgb_data;
#define EXPECTED_HAND_SCORE_MIN         99U  /* % */
#define EXPECTED_LEFT_HAND_VALUE        true
#define EXPECTED_GESTURE                "Victory"
#define EXPECTED_GESTURE_SCORE_MIN      86U  /* % */
#elif (APP_CONFIG==2)
#include "internal/images/hand_closed_fist_224_224_rgb.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_HAND_CLOSED_FIST_224_224_RGB_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_HAND_CLOSED_FIST_224_224_RGB_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_HAND_CLOSED_FIST_224_224_RGB_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_HAND_CLOSED_FIST_224_224_RGB_WIDTH
void *image_data = (void *)hand_closed_fist_224_224_rgb_data;
#define EXPECTED_HAND_SCORE_MIN         97U  /* % */
#define EXPECTED_LEFT_HAND_VALUE        true
#define EXPECTED_GESTURE                "Closed_Fist"
#define EXPECTED_GESTURE_SCORE_MIN      60U  /* % */
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */

/*
 * Copyright 2026 NXP
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
#include "internal/images/thispersondoesnotexist_4_96_rgb.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_THISPERSONDOESNOTEXIST_4_96_RGB_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_THISPERSONDOESNOTEXIST_4_96_RGB_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_THISPERSONDOESNOTEXIST_4_96_RGB_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_THISPERSONDOESNOTEXIST_4_96_RGB_WIDTH
void *image_data = (void *)thispersondoesnotexist_4_96_rgb_data;
#define EXPECTED_BRIGHTNESS 118
#define EXPECTED_CONTRAST 29

#endif /* _TEST_CONFIG_H */

/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _IMAGE_PROCESS
#define _IMAGE_PROCESS

#define IMG_FORMAT_JPG 0
#define IMG_FORMAT_YUV422 1
#define IMG_FORMAT_RGB888 2
#define IMG_FORMAT_RGB565 3

#include "board.h"

int Process_Image_Frame(uint8_t *imagebuffer, uint32_t imagesize, uint32_t width, uint32_t height, uint8_t format);

#endif

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

 /* File source configuration */
#define H264_FILE_PATH "/000.264"
#define H264_FILE_LOOP false

/* Decoded H.264 video parameters */
#define DECODED_VIDEO_FORMAT MPP_PIXEL_YUV420P
#define DECODED_VIDEO_HEIGHT 288
#define DECODED_VIDEO_WIDTH 352

#endif /* _TEST_CONFIG_H */

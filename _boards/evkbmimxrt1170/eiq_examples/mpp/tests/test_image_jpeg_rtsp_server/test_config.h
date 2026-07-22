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

#ifndef APP_CONFIG
#define APP_CONFIG 0
#endif

#if (APP_CONFIG==0) /* default app config */
#include "images/tiger_jpeg.h"
#define SRC_IMAGE_FORMAT            SRC_IMAGE_TIGER_JPEG_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER   SRC_IMAGE_TIGER_JPEG_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT            SRC_IMAGE_TIGER_JPEG_HEIGHT
#define SRC_IMAGE_WIDTH             SRC_IMAGE_TIGER_JPEG_WIDTH
void *image_data = (void *)tiger_jpeg_data;
unsigned int image_data_len = tiger_jpeg_data_len;
#define IMAGE_NAME "tiger_jpeg"
#define EXPECTED_CHECKSUM 0x394094e3
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#define RTSP_SERVER_PORT 8554       /* Default RTSP port */

/* IP address configuration. */
#ifndef configIP_ADDR0
#define configIP_ADDR0 192
#endif
#ifndef configIP_ADDR1
#define configIP_ADDR1 168
#endif
#ifndef configIP_ADDR2
#define configIP_ADDR2 0
#endif
#ifndef configIP_ADDR3
#define configIP_ADDR3 102
#endif

/* Netmask configuration. */
#ifndef configNET_MASK0
#define configNET_MASK0 255
#endif
#ifndef configNET_MASK1
#define configNET_MASK1 255
#endif
#ifndef configNET_MASK2
#define configNET_MASK2 255
#endif
#ifndef configNET_MASK3
#define configNET_MASK3 0
#endif

/* Gateway address configuration. */
#ifndef configGW_ADDR0
#define configGW_ADDR0 192
#endif
#ifndef configGW_ADDR1
#define configGW_ADDR1 168
#endif
#ifndef configGW_ADDR2
#define configGW_ADDR2 0
#endif
#ifndef configGW_ADDR3
#define configGW_ADDR3 100
#endif

#endif /* _TEST_CONFIG_H */

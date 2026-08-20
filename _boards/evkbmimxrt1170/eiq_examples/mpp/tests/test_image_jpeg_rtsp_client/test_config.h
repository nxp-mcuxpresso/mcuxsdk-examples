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

/* RTSP stream path published on the server (without leading slash). */
#define RTSP_STREAM_PATH "photo"

/* Source image dimensions - must match the RTSP stream resolution */
#ifndef SRC_IMAGE_WIDTH
#define SRC_IMAGE_WIDTH  1024
#endif
#ifndef SRC_IMAGE_HEIGHT
#define SRC_IMAGE_HEIGHT 824
#endif

/* Maximum encoded frame size passed to the RTSP source element.
 * The HAL allocates a ping-pong reassembly buffer of 2x this size internally.
 * 512 KB provides comfortable headroom over the test image (tiger_rtp.jpg, ~158 KB). */
#define RTSP_BUF_SIZE (512 * 1024)

#endif /* _TEST_CONFIG_H */

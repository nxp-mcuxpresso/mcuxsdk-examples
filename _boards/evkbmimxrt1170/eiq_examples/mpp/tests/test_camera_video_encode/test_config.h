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

#define SOURCE_STATIC_IMAGE 0

#if (SOURCE_STATIC_IMAGE == 1)
#include "images/skigirl_COCO_320_320_yuyv.h"
#define SRC_IMAGE_FORMAT SRC_IMAGE_SKIGIRL_COCO_320_320_YUYV_FORMAT
#define SRC_IMAGE_CHANNELS_NUMBER SRC_IMAGE_SKIGIRL_COCO_320_320_YUYV_CHANNELS_NUMBER
#define SRC_IMAGE_HEIGHT SRC_IMAGE_SKIGIRL_COCO_320_320_YUYV_HEIGHT
#define SRC_IMAGE_WIDTH SRC_IMAGE_SKIGIRL_COCO_320_320_YUYV_WIDTH
void *image_data = (void *)skigirl_COCO_320_320_yuyv_data;
#endif

 /* File source configuration */
#define H264_OUTPUT_FILE_PATH "/002.264"

/* H.264 encoder configuration parameters */
#define H264_ENCODER_FPS 30
#define H264_ENCODER_INTRA_PERIOD 30              /* I-frame every 30 frames */
#define H264_ENCODER_NUM_REF_FRAME 1              /* Single reference frame (low latency) */
#define H264_ENCODER_RC_MODE 1                    /* RC_BITRATE_MODE */
#define H264_ENCODER_TARGET_BITRATE 1470000       /* 1.47 Mbps */
#define H264_ENCODER_MAX_BITRATE 1470000          /* Force constant bitrate */
#define H264_ENCODER_ENABLE_FRAME_SKIP false      /* Disable frame skipping */
#define H264_ENCODER_TEMPORAL_LAYER_NUM 1         /* Single temporal layer */
#define H264_ENCODER_PROFILE_IDC 66               /* PRO_BASELINE */
#define H264_ENCODER_LEVEL_IDC 30                 /* LEVEL_3_0 */
#define H264_ENCODER_SPATIAL_BITRATE 1470000      /* 1.47 Mbps */
#define H264_ENCODER_MAX_SPATIAL_BITRATE 1470000  /* 1.47 Mbps */
#define H264_ENCODER_ENTROPY_CODING_MODE 0        /* 0: CAVLC, 1: CABAC */

#define RTSP_SERVER_PORT 8554                     /* Default RTSP port */

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

/*
 * Copyright 2023-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* This header configures the MPP HAL and the application according to the board model */

/*
 * USB camera format selection:
 *   0: MATCH_FORMAT_ANY
 *   1: MATCH_FORMAT_MJPEG
 *   2: MATCH_FORMAT_UNCOMPRESSED
 */
#define MATCH_FORMAT 2  /* MATCH_FORMAT_UNCOMPRESSED - camera may not support MJPEG at 320x240 */
#include "host_video.h"

/*******************************************************************************
 * HAL configuration (Mandatory)
 ******************************************************************************/
/* Set here all the static configuration of the Media Processing Pipeline HAL */

/**
 * This is the frdmimxrt1152 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_CAMERA
#define HAL_ENABLE_CAMERA_DEV_MipiOv5640      0
#define HAL_ENABLE_CAMERA_DEV_USB             1
#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055   1
#define HAL_ENABLE_2D_IMGPROC
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                1
#define HAL_ENABLE_GFX_DEV_GPU                1

/* VGLite GPU configuration for FRDM-IMXRT1152 (GC355) */
#define HAL_VGLITE_HEAP_SZ                    8912896 /* 8.5 MB */
#define HAL_VGLITE_BUFFER_ALIGN               64
/* GPU chip ID for FRDM-IMXRT1152 board (GC355). */
#define HAL_GPU_CHIPID                        0x355

/**
 * This is the inference HAL configuration
 */
#define HAL_ENABLE_INFERENCE_TFLITE           0

/**
 * The display max byte per pixel
 */
#define HAL_DISPLAY_MAX_BPP                   2

/**
 * This is HAL debug configuration
 */

/* Log level configuration
 * ERR:   0
 * INFO:  1
 * DEBUG: 2
 */
#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL 0
#endif

/**
 *  Mutex lock timeout definition
 *  An arbitrary default value is defined to 5 seconds
 *  value unit should be milliseconds
 * */
#define HAL_MUTEX_TIMEOUT_MS   (5000)

/*******************************************************************************
 * Application configuration (Optional)
 ******************************************************************************/

/* Set here all the static configuration of the Application */

/* camera parameters */
#define APP_CAMERA_NAME    "USB_cam"
#define APP_CAMERA_FPS     5
#define APP_CAMERA_WIDTH   160
#define APP_CAMERA_HEIGHT  120
#define APP_CAMERA_FORMAT  MPP_PIXEL_YUYV

/* display parameters */
#define APP_DISPLAY_NAME   "Lcdifv2Rk055"
#define APP_DISPLAY_WIDTH  720
#define APP_DISPLAY_HEIGHT 1280
#define APP_DISPLAY_FORMAT MPP_PIXEL_RGB565

/* YUYV path at 320x240:
 *   Step 1: CPU color convert YUYV->RGB565 (small frame, fast on CPU)
 *   Step 2: VGLite GPU rotate 90 degrees + scale to full screen (720x1280)
 *   VGLite GC355 does not support YUYV input, so separate steps are required.
 */

/* Two-step pipeline: CPU for color convert, GPU for rotate+scale.
 * APP_COLOR_BACKEND_NAME triggers the two-element path in camera_view.c. */
#define APP_COLOR_BACKEND_NAME                "gfx_CPU"
#define APP_GFX_BACKEND_NAME                  "gfx_GPU"

#define APP_DISPLAY_LANDSCAPE_ROTATE ROTATE_90

/* ~5fps capture */
#define APP_RC_CYCLE_INC 3
#define APP_RC_CYCLE_MIN 160

/* Scale camera output (320x240) to full display (720x1280) after 90-degree rotation */
#define SCALED_VIEW
#define SCALED_VIEW_WIDTH  APP_DISPLAY_WIDTH
#define SCALED_VIEW_HEIGHT APP_DISPLAY_HEIGHT

#define APP_SRC_DISPLAY_FLIP                  FLIP_NONE

#endif /* _MPP_CONFIG_H */

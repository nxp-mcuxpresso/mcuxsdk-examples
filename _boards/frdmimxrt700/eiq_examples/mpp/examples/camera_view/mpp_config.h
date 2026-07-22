/* Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* This header configures the MPP HAL and the application according to the board model */

/*
 * 0: MATCH_FORMAT_ANY
 * 1: MATCH_FORMAT_MJPEG
 * 2: MATCH_FORMAT_UNCOMPRESSED
 *
 */
#ifdef USE_USB_CAMERA
#include "host_video.h"
#define MATCH_FORMAT MATCH_FORMAT_MJPEG
#endif

/*******************************************************************************
 * HAL configuration (Mandatory)
 ******************************************************************************/

/**
 * This is the frdmimxrt700 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_CAMERA
#ifdef USE_USB_CAMERA
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     0
#define HAL_ENABLE_CAMERA_DEV_USB             1
#else /* Use Flexio camera */
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     1
#define HAL_ENABLE_CAMERA_DEV_USB             0
#endif
#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_LcdifWaveshare5Inch   1
#define HAL_ENABLE_2D_IMGPROC

/**
 * This is the image decoder HAL configuration
 */

/* enable JPEG SW decoder */
#if (MATCH_FORMAT != MATCH_FORMAT_UNCOMPRESSED)
#define HAL_ENABLE_JPEG_CPU                   0
#define HAL_ENABLE_JPEG_HW                    1
#endif

/* use GPU backend */
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                1

/* use TFlite micro inference engine for this application */
#define HAL_ENABLE_INFERENCE_TFLITE           0


/**
 * VGLite heap size for MIMXRT700 EVK.
 */
#define HAL_VGLITE_HEAP_SZ                    0x100000 /* 1 MB */
#define HAL_VGLITE_BUFFER_ALIGN               64

/* GPU chip ID for MIMXRT700EVK board.*/
#define HAL_GPU_CHIPID                        0x555

/**
 * This is HAL debug configuration
 */

/*
 * Log level configuration
 * ERR:   0
 * INFO:  1
 * DEBUG: 2
 */
#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL                         0
#endif

/**
 *  Mutex lock timeout definition
 *  An arbitrary default value is defined to 5 seconds
 *  value unit should be milliseconds
 * */
#define HAL_MUTEX_TIMEOUT_MS                  (5000)

/*******************************************************************************
 * Application configuration (Optional)
 ******************************************************************************/

/* Set here all the static configuration of the Application */

/* camera parameters */
#ifdef USE_USB_CAMERA
#define APP_CAMERA_NAME    "USB_cam"
#if (MATCH_FORMAT == MATCH_FORMAT_UNCOMPRESSED)
#define APP_CAMERA_WIDTH   320   //320 //1280 //640 //352
#define APP_CAMERA_HEIGHT  240   //240 // 720 //480 //288
#define APP_CAMERA_FORMAT  MPP_PIXEL_YUYV
#else
#define APP_CAMERA_WIDTH   640   //320 //1280 //640 //352
#define APP_CAMERA_HEIGHT  480   //240 // 720 //480 //288
#define APP_CAMERA_FORMAT  MPP_PIXEL_JPEG
#endif
#else /* Flexio camera parameters */
#define APP_CAMERA_NAME    "EzhV_Ov7670"
#define APP_CAMERA_WIDTH   640
#define APP_CAMERA_HEIGHT  480
#define APP_CAMERA_FORMAT  MPP_PIXEL_RGB565
#endif

/* display parameters */
#define APP_DISPLAY_NAME   "LcdifWaveshare5Inch"
#define APP_DISPLAY_WIDTH                     800
#define APP_DISPLAY_HEIGHT                    480
#define APP_DISPLAY_FORMAT MPP_PIXEL_RGB565

/* Scale parameters */
#define SCALED_VIEW
#define VIEW_SMALL_DIM (APP_CAMERA_WIDTH <= APP_CAMERA_HEIGHT ? APP_CAMERA_WIDTH : APP_CAMERA_HEIGHT)
#define VIEW_LARGE_DIM (APP_CAMERA_WIDTH >= APP_CAMERA_HEIGHT ? APP_CAMERA_WIDTH : APP_CAMERA_HEIGHT)
/* display small & large dims */
#define DISPLAY_SMALL_DIM (APP_DISPLAY_WIDTH <= APP_DISPLAY_HEIGHT ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT)
#define DISPLAY_LARGE_DIM (APP_DISPLAY_WIDTH >= APP_DISPLAY_HEIGHT ? APP_DISPLAY_WIDTH : APP_DISPLAY_HEIGHT)
/* if display_aspect_ratio > view_aspect_ratio */
#if (DISPLAY_LARGE_DIM * VIEW_SMALL_DIM > VIEW_LARGE_DIM * DISPLAY_SMALL_DIM)
#define SCALED_VIEW_WIDTH APP_DISPLAY_WIDTH
#define SCALED_VIEW_HEIGHT (APP_DISPLAY_WIDTH * APP_CAMERA_WIDTH / APP_CAMERA_HEIGHT )
#else /* if display_aspect_ratio < view_aspect_ratio */
#define SCALED_VIEW_WIDTH (APP_DISPLAY_HEIGHT * APP_CAMERA_HEIGHT / APP_CAMERA_WIDTH )
#define SCALED_VIEW_HEIGHT APP_DISPLAY_HEIGHT
#endif

#define APP_GFX_BACKEND_NAME    "gfx_GPU"
#define APP_DECODE_BACKEND_NAME "jpeg_HW"

/* 30fps capture */
#define APP_RC_CYCLE_INC 3
#ifdef USE_USB_CAMERA
#define APP_RC_CYCLE_MIN 66
#else
#define APP_RC_CYCLE_MIN 33
#endif

/* other parameters */
/* Waveshare 5inch DSI is natively landscape (800x480), no rotation needed */
#ifdef USE_USB_CAMERA
#define APP_DISPLAY_LANDSCAPE_ROTATE ROTATE_0
#define APP_SRC_DISPLAY_FLIP         FLIP_HORIZONTAL
#else /* OV7670 */
#define APP_DISPLAY_LANDSCAPE_ROTATE ROTATE_0
#define APP_SRC_DISPLAY_FLIP         FLIP_NONE
#endif

#endif /* _MPP_CONFIG_H */

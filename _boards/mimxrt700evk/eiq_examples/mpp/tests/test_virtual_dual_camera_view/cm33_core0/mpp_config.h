/* Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

#include <stdbool.h>

/* This header configures the MPP HAL and the application according to the board model */

/*******************************************************************************
 * HAL configuration (Mandatory)
 ******************************************************************************/

/**
 * This is the mimxrt700evk board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

 /* Use virtual camera running on core 1 */
#define HAL_ENABLE_CAMERA
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     0
#define HAL_ENABLE_CAMERA_DEV_USB             0
#define HAL_ENABLE_VIRTUAL_CAMERA             1

#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055   1
#define HAL_ENABLE_2D_IMGPROC

/* use GPU backend */
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                1

/* Enable JPEG CPU decoder */
#if ((APP_CONFIG == 3) || (APP_CONFIG == 4) || (APP_CONFIG == 5))
#define HAL_ENABLE_JPEG_CPU                   0
#define HAL_ENABLE_JPEG_HW                    1
#else
#define HAL_ENABLE_JPEG_CPU                   1
#define HAL_ENABLE_JPEG_HW                    0
#endif

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

#ifndef APP_CONFIG
#define APP_CONFIG                            0
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

/* Virtual usb dual camera parameters */
#define APP_CAMERA_NAME    "Virtual_USB_cam"
#if ((APP_CONFIG == 3) || (APP_CONFIG == 4) || (APP_CONFIG == 5))
#define APP_CAMERA_WIDTH   640
#define APP_CAMERA_HEIGHT  480
#else
#define APP_CAMERA_WIDTH   320
#define APP_CAMERA_HEIGHT  240
#endif
#define APP_CAMERA_FORMAT  MPP_PIXEL_JPEG
#if (APP_CONFIG == 5)
#define CAMERA_IN_ADVANCE_ENQUEUE false
#else
#define CAMERA_IN_ADVANCE_ENQUEUE true
#endif

/* display parameters */
#define APP_DISPLAY_NAME   "Lcdifv2Rk055"
#define APP_DISPLAY_WIDTH  720
#define APP_DISPLAY_HEIGHT 1280
#define APP_DISPLAY_FORMAT MPP_PIXEL_RGB565

#define APP_GFX_BACKEND_NAME "gfx_GPU"
#ifndef IMG_DECODE_DEV_NAME
#if ((APP_CONFIG == 3) || (APP_CONFIG == 4) || (APP_CONFIG == 5))
#define IMG_DECODE_DEV_NAME "jpeg_HW"
#else
#define IMG_DECODE_DEV_NAME "jpeg_CPU"
#endif
#endif

/* 30fps capture */
#define APP_RC_CYCLE_INC 3
#ifdef USE_USB_CAMERA
#define APP_RC_CYCLE_MIN 200
#else
#define APP_RC_CYCLE_MIN 33
#endif

/* other parameters */
/* rotation is needed to display in landscape because display RK055 is portrait */
#define APP_DISPLAY_LANDSCAPE_ROTATE          ROTATE_90

#define APP_SRC_DISPLAY_FLIP                  FLIP_HORIZONTAL

#endif /* _MPP_CONFIG_H */

/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* This header configures the MPP HAL and the application according to the board model */

/*******************************************************************************
 * HAL configuration (Mandatory)
 ******************************************************************************/
/* Set here all the static configuration of the Media Processing Pipeline HAL */

/**
 * This is the frdmimxrt700 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_LcdifWaveshare5Inch   1
#define HAL_ENABLE_2D_IMGPROC
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                0

/**
 * This is the inference HAL configuration
 */

/* disable inference engines */
#define HAL_ENABLE_INFERENCE_TFLITE           0

/**
 * This is the display HAL configuration
 */

/* The display max byte per pixel */
#define HAL_DISPLAY_MAX_BPP                   4


/* Log level configuration
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
#define HAL_MUTEX_TIMEOUT_MS   (5000)

/*******************************************************************************
 * Application configuration (Optional)
 ******************************************************************************/

/* Set here all the static configuration of the Application */

/* display parameters */
#define APP_DISPLAY_NAME                      "LcdifWaveshare5Inch"
#define APP_DISPLAY_WIDTH                     800
#define APP_DISPLAY_HEIGHT                    480
#define APP_DISPLAY_FORMAT                    MPP_PIXEL_RGB565

/* other parameters */
/* Waveshare 5inch DSI panel is natively landscape (800x480), no rotation needed */
#define APP_DISPLAY_LANDSCAPE_ROTATE          ROTATE_0

/* enable checksum computation on display framebuffer [TESTS ONLY] */
#ifndef ENABLE_FB_CHEKSUM
#define ENABLE_FB_CHEKSUM                     0
#endif

#endif /* _MPP_CONFIG_H */

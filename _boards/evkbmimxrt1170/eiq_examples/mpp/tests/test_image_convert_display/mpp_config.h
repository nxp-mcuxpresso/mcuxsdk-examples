/*
 * Copyright 2021-2024 NXP
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
 * This is the evkbmimxrt1170 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055   1
#define HAL_ENABLE_2D_IMGPROC
/* use PXP backend by default */
#if !defined(IMG_CONVERT_CPU) && !defined(IMG_CONVERT_GPU)
#define HAL_ENABLE_GFX_DEV_Pxp                1
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                0
#elif defined(IMG_CONVERT_CPU)
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                1
#define HAL_ENABLE_GFX_DEV_GPU                0
#elif defined(IMG_CONVERT_GPU)
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                1
#endif

/**
 * This is the inference HAL configuration
 */

/* disable inference engines */
#define HAL_ENABLE_INFERENCE_TFLITE           0
#define HAL_ENABLE_INFERENCE_GLOW             0
#define HAL_ENABLE_INFERENCE_DVRT             0

/**
 * This is the PXP HAL configuration
 */

/* Workaround for the PXP bug where BGR888 is output instead of RGB888 [MPP-97] */
#define HAL_PXP_WORKAROUND_OUT_RGB            1

/**
 * This is the display HAL configuration
 */

/* The display max byte per pixel */
#define HAL_DISPLAY_MAX_BPP                   2

/**
 * VGLite heap size for MIMXRT1170 CM7.
 */
#define HAL_VGLITE_HEAP_SZ                    8912896 /* 8.5 MB */
#define HAL_VGLITE_BUFFER_ALIGN               64

/* GPU chip ID for EVKMIMXRT1170 board.*/
#define HAL_GPU_CHIPID                        0x355

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
#define APP_DISPLAY_NAME                      "Lcdifv2Rk055"
#define APP_DISPLAY_WIDTH                     720
#define APP_DISPLAY_HEIGHT                    1280
#define APP_DISPLAY_FORMAT                    MPP_PIXEL_RGB565

/* other parameters */
/* rotation is needed to display in landscape because display RK055 is portrait */
#define APP_DISPLAY_LANDSCAPE_ROTATE          ROTATE_90

/* enable checksum computation on display framebuffer [TESTS ONLY] */
#ifndef ENABLE_FB_CHEKSUM
#define ENABLE_FB_CHEKSUM                     1
#endif

#endif /* _MPP_CONFIG_H */
